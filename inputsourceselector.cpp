#include "inputsourceselector.h"
#include "mapbuttondialog.h"
#include "localcontrollermanager.h"
#include "buttonmashdebug.h"
#include "ui_inputsourceselector.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QTcpSocket>
#include <QGamepadManager>
#include <QGamepad>
#include <QTemporaryFile>
#include <QSettings>

extern QSettings* globalSetting;

const QString SETTING_INPUTSOURCE = "inputSource/inputSource";
const QString SETTING_DELAI = "delai";

const QString SETTING_SNESCLASSIC_TELNET = "SnesClassicTelnet";
const QString SETTING_SNESCLASSIC_STUFF = "SnesClassicStuff";
const QString SETTING_USB2SNES = "Usb2Snes";
const QString SETTING_USB2SNES_DEVICE =  "Usb2SnesDevice";
const QString SETTING_USB2SNES_GAME =  "Usb2SnesGame";
const QString SETTING_ARDUINO = "Arduino";
const QString SETTING_ARDUINOCOM = "ArduinoCOM";
const QString SETTING_DIRECT_INPUT = "DirectInput";
const QString SETTING_LOCAL_CONTROLLER = "LocalController";
const QString SETTING_LOCALCONTROLLER_MAPPING = "LocalController/mapping";
const QString SETTING_LOCALCONTROLLER_DEVICEID = "LocalControllerDeviceId";
const QString SETTING_REMOTE_SERVER = "RemoteServer";
const QString SETTING_REMOTE_PROTOCOL = "RemoteProtocol";
const QString SETTING_REMOTE_PORT = "RemotePort";

namespace {

QString resolvedSettingValue(const QString &uiValue, const QString &savedValue)
{
    if (!uiValue.isEmpty())
        return uiValue;
    return savedValue;
}

} // namespace

InputSourceSelector::InputSourceSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputSourceSelector)
{
    ui->setupUi(this);
    usb2snes = nullptr;
    snesClassicTelnet = nullptr;
    arduinoCom = nullptr;
    usb2snesProvider = nullptr;
    m_currentProvider = nullptr;
    localcontrollerProvider = nullptr;
    retroArchRemoteProvider = nullptr;
    buttonMashRemoteProvider = nullptr;
    m_delai = 0;
    if (globalSetting->contains(SETTING_DELAI))
    {
        m_delai = globalSetting->value(SETTING_DELAI).toUInt();
        ui->delaiSpinBox->setValue(m_delai);
    }
    connect(ui->sourceRadioGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &InputSourceSelector::onSourceButtonClicked);
}

InputSourceSelector::~InputSourceSelector()
{
    delete ui;
}

InputProvider *InputSourceSelector::currentProvider()
{
    return m_currentProvider;
}

InputProvider *InputSourceSelector::getLastProvider()
{
    resetProviders();
    InputProvider* toret = nullptr;
    if (globalSetting->contains(SETTING_INPUTSOURCE))
    {
        QString inputSource = globalSetting->value(SETTING_INPUTSOURCE).toString();
        if (inputSource == SETTING_SNESCLASSIC_TELNET)
        {
            snesClassicTelnet = new SNESClassicTelnet();
            ui->snesClassicRadioButton->setChecked(true);
            m_currentProvider = snesClassicTelnet;
        }
        if (inputSource == SETTING_ARDUINO)
        {
            arduinoCom = new ArduinoCOM(globalSetting->value("inputSource/" + SETTING_ARDUINOCOM).toString());
            ui->arduinoRadioButton->setChecked(true);
            qCDebug(buttonmashLog) << "Using Arduino" << arduinoCom->port();
            m_currentProvider = arduinoCom;
        }
        if (inputSource == SETTING_USB2SNES)
        {
            usb2snes = new USB2snes(false);
            usb2snesProvider = new Usb2SnesSource(usb2snes);
            usb2snesProvider->loadGamesList();
            usb2snesProvider->setDevice(globalSetting->value("inputSource/" + SETTING_USB2SNES_DEVICE).toString());
            usb2snesProvider->setGame(globalSetting->value("inputSource/" + SETTING_USB2SNES_GAME).toString());
            ui->usb2snesRadioButton->setChecked(true);
            m_currentProvider = usb2snesProvider;
        }
        if (inputSource == SETTING_LOCAL_CONTROLLER)
        {
            QString deviceId = globalSetting->value("inputSource/" + SETTING_LOCALCONTROLLER_DEVICEID).toString();
            if (deviceId.isEmpty()) {
                const auto gamepads = LocalControllerManager::getManager()->listController();
                if (!gamepads.isEmpty()) {
                    deviceId = gamepads.first().id;
                    qCDebug(buttonmashLog) << "LocalController device ID empty; falling back to" << deviceId;
                }
            }
            localcontrollerProvider = LocalControllerManager::getManager()->createProvider(deviceId);
            localcontrollerMapping = LocalControllerManager::getManager()->loadMapping(*globalSetting, "inputSource/" + SETTING_LOCALCONTROLLER_MAPPING);
            qCDebug(buttonmashLog) << "Number of key binded :" << localcontrollerMapping.size();
            ui->xinputRadioButton->setChecked(true);
            if (localcontrollerProvider == nullptr) {
                qCDebug(buttonmashLog) << "Could not create LocalController provider for" << deviceId;
                return nullptr;
            }
            localcontrollerProvider->setMapping(localcontrollerMapping);
            m_currentProvider = localcontrollerProvider;
        }
        if (inputSource == SETTING_REMOTE_SERVER)
        {
            ui->remoteRadioButton->setChecked(true);
            const QString proto = globalSetting->value("inputSource/" + SETTING_REMOTE_PROTOCOL, QStringLiteral("ButtonMash")).toString();
            const int idx = ui->remoteProtocolCombo->findText(proto);
            ui->remoteProtocolCombo->setCurrentIndex(idx >= 0 ? idx : 0);
            ui->remotePortSpin->setValue(int(globalSetting->value("inputSource/" + SETTING_REMOTE_PORT, 27151).toUInt()));
            ensureRemoteProvider();
            m_currentProvider = (proto == QLatin1String("RetroArchRemotePad"))
                    ? static_cast<InputProvider*>(retroArchRemoteProvider)
                    : static_cast<InputProvider*>(buttonMashRemoteProvider);
        }
        return m_currentProvider;
    } else {
        return nullptr;
    }
    //arduinoCom = new ArduinoCOM("COM6");
    //return arduinoCom;
    return toret;
}

void InputSourceSelector::scanDevices()
{
    if (usb2snes == nullptr)
        usb2snes = new USB2snes(false);
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    foreach (QSerialPortInfo pfi, infos)
    {
        qCDebug(buttonmashLog) << pfi.description() << pfi.portName();
    }

    if (!infos.isEmpty())
        setArduinoInfo();
    auto gamepads = LocalControllerManager::getManager()->listController();
    qCDebug(buttonmashLog) << "Gamepads : " << gamepads.size();
    if (!gamepads.isEmpty())
        setLocalControllers();
    QTcpSocket  testSocket;
    testSocket.connectToHost("169.254.13.37", 23);
    if (testSocket.waitForConnected(50))
        activateSnesClassicTelnet();
    testSocket.close();
    testSocket.connectToHost("169.254.13.37", 1042);
    if (testSocket.waitForConnected(50))
        activateSnesClassicStuff();
    testSocket.close();
    testSocket.connectToHost("localhost", 23074);
    if (testSocket.waitForConnected(50))
        activateUsb2SnesStuff();
    testSocket.close();
}

unsigned int InputSourceSelector::delai() const
{
    return m_delai;
}

void InputSourceSelector::activateSnesClassicTelnet()
{
    ui->snesClassicRadioButton->setEnabled(true);
}

void InputSourceSelector::activateSnesClassicStuff()
{
    ui->snesClassicStuffRadioButton->setEnabled(true);
}

void InputSourceSelector::activateUsb2SnesStuff()
{
    if (usb2snesProvider == nullptr)
    {
        usb2snes = new USB2snes(false);
        usb2snesProvider = new Usb2SnesSource(usb2snes);
    }
    auto gamesList = usb2snesProvider->loadGamesList();
    ui->usb2gameComboBox->clear();
    foreach (QString game, gamesList)
    {
        ui->usb2gameComboBox->addItem(game);
    }
    if (usb2snes->state() == USB2snes::None)
        usb2snes->connect();
    if (usb2snes->state() == USB2snes::Connected || usb2snes->state() == USB2snes::Ready)
        onUsb2SnesConnected();
    connect(usb2snes, &USB2snes::connected, this, &InputSourceSelector::onUsb2SnesConnected, Qt::UniqueConnection);
    ui->usb2snesRadioButton->setEnabled(true);
}

void InputSourceSelector::onUsb2SnesConnected()
{
    auto devices = usb2snes->deviceList();
    ui->usb2snesComboBox->clear();
    ui->usb2snesComboBox->setEnabled(true);
    foreach(QString name, devices)
    {
        ui->usb2snesComboBox->addItem(name);
    }
}

void InputSourceSelector::setArduinoInfo()
{
    ui->arduinoComComboBox->blockSignals(true);
    ui->arduinoComComboBox->clear();
    ui->arduinoComComboBox->setEnabled(true);
    ui->arduinoPortLabel->setEnabled(true);
    ui->arduinoRadioButton->setEnabled(true);
    ui->arduiType1Radio->setEnabled(true);
    ui->arduiType1Radio->setChecked(true);
    //ui->arduiType2Radio->setEnabled(true);
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    unsigned int i = 0;
    int index_arduino = -1;
    foreach (QSerialPortInfo info, infos)
    {
        if (info.description().indexOf("Arduino") != -1)
        {
            ui->arduinoComComboBox->addItem(info.portName() + " - " + info.description());
            index_arduino = i;
        } else {
            ui->arduinoComComboBox->addItem(info.portName());
        }
        ui->arduinoComComboBox->setItemData(i, info.portName(), Qt::UserRole + 1);
        i++;
    }
    if (arduinoCom != nullptr)
    {
        qCDebug(buttonmashLog) << arduinoCom->port();
        unsigned int idx;
        for (idx = 0; idx < ui->arduinoComComboBox->count(); idx++)
        {
            qCDebug(buttonmashLog) << idx << ui->arduinoComComboBox->itemData(idx, Qt::UserRole + 1).toString();
            if (ui->arduinoComComboBox->itemData(idx, Qt::UserRole + 1).toString() == arduinoCom->port())
                break;
        }
        qCDebug(buttonmashLog) << "Found at : " << idx;
        ui->arduinoComComboBox->setCurrentIndex(idx);
    } else {
        if (index_arduino != -1)
            ui->arduinoComComboBox->setCurrentIndex(index_arduino);
    }
    ui->arduinoComComboBox->blockSignals(false);
}

void InputSourceSelector::setLocalControllers()
{
    ui->xinputComboBox->blockSignals(true);
    ui->xinputComboBox->setEnabled(true);
    ui->xinputRadioButton->setEnabled(true);
    ui->xinputComboBox->clear();
    auto gamepads = LocalControllerManager::getManager()->listController();
    localcontrollerList = gamepads;
    unsigned int cpt = 0;
    for (auto& gamepad : gamepads)
    {
        //QGamepad pad(id);
        qCDebug(buttonmashLog) << gamepad.id;
        qCDebug(buttonmashLog) << gamepad.name;
        ui->xinputComboBox->addItem(gamepad.name);
        ui->xinputComboBox->setItemData(cpt, gamepad.id, Qt::UserRole + 1);
        cpt++;
    }
    if (localcontrollerMapping.isEmpty() && globalSetting->contains(SETTING_LOCALCONTROLLER_DEVICEID))
    {
        localcontrollerMapping = LocalControllerManager::getManager()->loadMapping(*globalSetting, "inputSource/" + SETTING_LOCALCONTROLLER_MAPPING);
    }
    if (localcontrollerProvider != nullptr)
    {
        unsigned int idx;
        for (idx = 0; idx < ui->xinputComboBox->count(); idx++)
        {
            qCDebug(buttonmashLog) << localcontrollerProvider->id() << ui->xinputComboBox->itemData(idx, Qt::UserRole + 1).toString();
            if (ui->xinputComboBox->itemData(idx, Qt::UserRole + 1).toString() == localcontrollerProvider->id())
                break;
        }
        ui->xinputComboBox->setCurrentIndex(idx);
        ui->mappingButton->setEnabled(true);
    }
    ui->xinputComboBox->blockSignals(false);
}

void InputSourceSelector::ensureLocalControllerProvider(int comboIndex)
{
    if (comboIndex < 0)
        comboIndex = ui->xinputComboBox->currentIndex();
    if (comboIndex < 0 || ui->xinputComboBox->count() == 0)
        return;

    QString deviceId = ui->xinputComboBox->itemData(comboIndex, Qt::UserRole + 1).toString();
    if (localcontrollerProvider != nullptr && localcontrollerProvider->id() == deviceId)
    {
        if (ui->xinputRadioButton->isChecked())
            m_currentProvider = localcontrollerProvider;
        return;
    }

    if (m_currentProvider == localcontrollerProvider)
        m_currentProvider = nullptr;
    delete localcontrollerProvider;
    localcontrollerProvider = LocalControllerManager::getManager()->createProvider(deviceId);
    if (localcontrollerProvider == nullptr)
        return;

    if (localcontrollerMapping.isEmpty())
        localcontrollerMapping = LocalControllerManager::getManager()->loadMapping(*globalSetting, "inputSource/" + SETTING_LOCALCONTROLLER_MAPPING);
    localcontrollerProvider->setMapping(localcontrollerMapping);
    if (ui->xinputRadioButton->isChecked())
        m_currentProvider = localcontrollerProvider;
}

void InputSourceSelector::ensureRemoteProvider()
{
    const quint16 port = quint16(ui->remotePortSpin->value());
    const QString proto = ui->remoteProtocolCombo->currentText();
    if (proto == QLatin1String("RetroArchRemotePad")) {
        if (retroArchRemoteProvider == nullptr)
            retroArchRemoteProvider = new RetroArchRemotePadProvider(port);
        else
            retroArchRemoteProvider->setPort(port);
        if (ui->remoteRadioButton->isChecked())
            m_currentProvider = retroArchRemoteProvider;
    } else {
        if (buttonMashRemoteProvider == nullptr)
            buttonMashRemoteProvider = new ButtonMashRemoteProvider(port);
        else
            buttonMashRemoteProvider->setPort(port);
        if (ui->remoteRadioButton->isChecked())
            m_currentProvider = buttonMashRemoteProvider;
    }
}

void InputSourceSelector::resetProviders()
{
    m_currentProvider = nullptr;
    delete snesClassicTelnet;
    snesClassicTelnet = nullptr;
    delete arduinoCom;
    arduinoCom = nullptr;
    delete usb2snesProvider;
    usb2snesProvider = nullptr;
    delete usb2snes;
    usb2snes = nullptr;
    delete localcontrollerProvider;
    localcontrollerProvider = nullptr;
    delete retroArchRemoteProvider;
    retroArchRemoteProvider = nullptr;
    delete buttonMashRemoteProvider;
    buttonMashRemoteProvider = nullptr;
    localcontrollerMapping.clear();
}

void InputSourceSelector::applyUiFromSettings()
{
    if (!globalSetting->contains(SETTING_INPUTSOURCE))
        return;

    const QString inputSource = globalSetting->value(SETTING_INPUTSOURCE).toString();
    if (inputSource == SETTING_SNESCLASSIC_TELNET)
        ui->snesClassicRadioButton->setChecked(true);
    else if (inputSource == SETTING_SNESCLASSIC_STUFF)
        ui->snesClassicStuffRadioButton->setChecked(true);
    else if (inputSource == SETTING_ARDUINO)
        ui->arduinoRadioButton->setChecked(true);
    else if (inputSource == SETTING_USB2SNES)
        ui->usb2snesRadioButton->setChecked(true);
    else if (inputSource == SETTING_LOCAL_CONTROLLER)
        ui->xinputRadioButton->setChecked(true);
    else if (inputSource == SETTING_REMOTE_SERVER)
        ui->remoteRadioButton->setChecked(true);

    if (inputSource == SETTING_REMOTE_SERVER) {
        const QString proto = globalSetting->value("inputSource/" + SETTING_REMOTE_PROTOCOL,
                                                   QStringLiteral("ButtonMash")).toString();
        const int idx = ui->remoteProtocolCombo->findText(proto);
        ui->remoteProtocolCombo->setCurrentIndex(idx >= 0 ? idx : 0);
        ui->remotePortSpin->setValue(int(globalSetting->value("inputSource/" + SETTING_REMOTE_PORT, 27151).toUInt()));
    }

    if (globalSetting->contains(SETTING_DELAI)) {
        m_delai = globalSetting->value(SETTING_DELAI).toUInt();
        ui->delaiSpinBox->setValue(int(m_delai));
    }
}

void InputSourceSelector::persistInputSourceSettings()
{
    if (ui->snesClassicRadioButton->isChecked())
        globalSetting->setValue(SETTING_INPUTSOURCE, SETTING_SNESCLASSIC_TELNET);
    if (ui->snesClassicStuffRadioButton->isChecked())
        globalSetting->setValue(SETTING_INPUTSOURCE, SETTING_SNESCLASSIC_STUFF);
    if (ui->arduinoRadioButton->isChecked()) {
        globalSetting->setValue(SETTING_INPUTSOURCE, SETTING_ARDUINO);
        const QString port = resolvedSettingValue(
            ui->arduinoComComboBox->currentData(Qt::UserRole + 1).toString(),
            arduinoCom != nullptr ? arduinoCom->port()
                                  : globalSetting->value("inputSource/" + SETTING_ARDUINOCOM).toString());
        if (!port.isEmpty())
            globalSetting->setValue("inputSource/" + SETTING_ARDUINOCOM, port);
    }
    if (ui->usb2snesRadioButton->isChecked()) {
        globalSetting->setValue(SETTING_INPUTSOURCE, SETTING_USB2SNES);
        const QString device = resolvedSettingValue(
            ui->usb2snesComboBox->currentText(),
            globalSetting->value("inputSource/" + SETTING_USB2SNES_DEVICE).toString());
        const QString game = resolvedSettingValue(
            ui->usb2gameComboBox->currentText(),
            globalSetting->value("inputSource/" + SETTING_USB2SNES_GAME).toString());
        if (!device.isEmpty())
            globalSetting->setValue("inputSource/" + SETTING_USB2SNES_DEVICE, device);
        if (!game.isEmpty())
            globalSetting->setValue("inputSource/" + SETTING_USB2SNES_GAME, game);
    }
    if (ui->xinputRadioButton->isChecked()) {
        globalSetting->setValue(SETTING_INPUTSOURCE, SETTING_LOCAL_CONTROLLER);
        QString deviceId = ui->xinputComboBox->currentData(Qt::UserRole + 1).toString();
        if (deviceId.isEmpty() && localcontrollerProvider != nullptr)
            deviceId = localcontrollerProvider->id();
        if (deviceId.isEmpty())
            deviceId = globalSetting->value("inputSource/" + SETTING_LOCALCONTROLLER_DEVICEID).toString();
        if (!deviceId.isEmpty())
            globalSetting->setValue("inputSource/" + SETTING_LOCALCONTROLLER_DEVICEID, deviceId);
        if (localcontrollerMapping.isEmpty())
            localcontrollerMapping = LocalControllerManager::getManager()->loadMapping(
                *globalSetting, "inputSource/" + SETTING_LOCALCONTROLLER_MAPPING);
        LocalControllerManager::getManager()->saveMapping(*globalSetting,
                                                          "inputSource/" + SETTING_LOCALCONTROLLER_MAPPING,
                                                          localcontrollerMapping);
        if (localcontrollerProvider != nullptr)
            localcontrollerProvider->setMapping(localcontrollerMapping);
    }
    if (ui->remoteRadioButton->isChecked()) {
        globalSetting->setValue(SETTING_INPUTSOURCE, SETTING_REMOTE_SERVER);
        globalSetting->setValue("inputSource/" + SETTING_REMOTE_PROTOCOL, ui->remoteProtocolCombo->currentText());
        globalSetting->setValue("inputSource/" + SETTING_REMOTE_PORT, ui->remotePortSpin->value());
    }
    m_delai = ui->delaiSpinBox->value();
    if (m_delai != 0)
        globalSetting->setValue(SETTING_DELAI, m_delai);
    else
        globalSetting->remove(SETTING_DELAI);
}

void InputSourceSelector::saveCurrentToSettings()
{
    applyUiFromSettings();
    syncCurrentProviderFromSelection();
    persistInputSourceSettings();
}

void InputSourceSelector::reloadFromSettings()
{
    resetProviders();
    if (globalSetting->contains(SETTING_DELAI)) {
        m_delai = globalSetting->value(SETTING_DELAI).toUInt();
        ui->delaiSpinBox->setValue(int(m_delai));
    } else {
        m_delai = 0;
        ui->delaiSpinBox->setValue(0);
    }
    applyUiFromSettings();
    getLastProvider();
}

void InputSourceSelector::syncCurrentProviderFromSelection()
{
    if (ui->snesClassicRadioButton->isChecked())
    {
        if (snesClassicTelnet == nullptr)
            snesClassicTelnet = new SNESClassicTelnet();
        m_currentProvider = snesClassicTelnet;
    }
    else if (ui->arduinoRadioButton->isChecked())
    {
        if (arduinoCom == nullptr)
            arduinoCom = new ArduinoCOM(ui->arduinoComComboBox->currentData(Qt::UserRole + 1).toString());
        m_currentProvider = arduinoCom;
    }
    else if (ui->usb2snesRadioButton->isChecked())
    {
        if (usb2snesProvider == nullptr)
            usb2snesProvider = new Usb2SnesSource(usb2snes);
        m_currentProvider = usb2snesProvider;
    }
    else if (ui->xinputRadioButton->isChecked())
    {
        ui->mappingButton->setEnabled(true);
        ensureLocalControllerProvider();
        m_currentProvider = localcontrollerProvider;
    }
    else if (ui->remoteRadioButton->isChecked())
    {
        ensureRemoteProvider();
    }
}

void InputSourceSelector::on_buttonBox_accepted()
{
    syncCurrentProviderFromSelection();
    persistInputSourceSettings();
    accept();
}

void InputSourceSelector::on_buttonBox_rejected()
{
    reject();
}


int InputSourceSelector::exec()
{
    scanDevices();
    return QDialog::exec();
}

void InputSourceSelector::on_refreshButton_clicked()
{
    scanDevices();
}

void InputSourceSelector::onSourceButtonClicked(QAbstractButton *but)
{
    if (but == ui->snesClassicRadioButton)
    {
        if (snesClassicTelnet == nullptr)
            snesClassicTelnet = new SNESClassicTelnet();
        m_currentProvider = snesClassicTelnet;
    }
    if (but == ui->arduinoRadioButton)
    {
        if (arduinoCom == nullptr)
            arduinoCom = new ArduinoCOM(ui->arduinoComComboBox->currentData(Qt::UserRole + 1).toString());
        m_currentProvider = arduinoCom;
    }
    if (but == ui->usb2snesRadioButton)
    {
        if (usb2snesProvider == nullptr)
            usb2snesProvider = new Usb2SnesSource(usb2snes);
        m_currentProvider = usb2snesProvider;

    }
    if (but == ui->xinputRadioButton)
    {
        ui->mappingButton->setEnabled(true);
        ensureLocalControllerProvider();
        m_currentProvider = localcontrollerProvider;
    }
    if (but == ui->remoteRadioButton)
    {
        ensureRemoteProvider();
    }
}

void InputSourceSelector::on_arduinoComComboBox_currentIndexChanged(const QString &arg1)
{
    if (arduinoCom != nullptr)
        arduinoCom->setPort(ui->arduinoComComboBox->currentData(Qt::UserRole + 1).toString());
}

void InputSourceSelector::on_usb2snesComboBox_currentTextChanged(const QString &arg1)
{
    usb2snesProvider->setDevice(arg1);
}

void InputSourceSelector::on_usb2gameComboBox_currentTextChanged(const QString &arg1)
{
    usb2snesProvider->setGame(arg1);
}

void InputSourceSelector::on_xinputComboBox_currentIndexChanged(int index)
{
    ensureLocalControllerProvider(index);
}


void InputSourceSelector::on_mappingButton_clicked()
{
    MapButtonDialog mapDiag;

    mapDiag.setDevice(localcontrollerList.at(ui->xinputComboBox->currentIndex()));
    mapDiag.setMapping(localcontrollerMapping);
    if (mapDiag.exec() == QDialog::Accepted)
    {
        localcontrollerMapping = mapDiag.mapping();
    }
}

int inputSourcePersistSelfTest()
{
    QTemporaryFile tmp;
    tmp.setAutoRemove(true);
    if (!tmp.open())
        return 1;

    const QString path = tmp.fileName();
    tmp.close();

    QSettings testSettings(path, QSettings::IniFormat);
    QSettings *previous = globalSetting;
    globalSetting = &testSettings;

    testSettings.setValue(SETTING_INPUTSOURCE, SETTING_LOCAL_CONTROLLER);
    testSettings.setValue(QStringLiteral("inputSource/") + SETTING_LOCALCONTROLLER_DEVICEID,
                          QStringLiteral("QGamepad 1"));
    testSettings.sync();
    ButtonMashDebug::applyLoggingRules();

    QWidget dummy;
    InputSourceSelector selector(&dummy);
    selector.saveCurrentToSettings();

    if (testSettings.value(QStringLiteral("inputSource/") + SETTING_LOCALCONTROLLER_DEVICEID).toString()
            != QStringLiteral("QGamepad 1")) {
        globalSetting = previous;
        return 2;
    }

    testSettings.setValue(QStringLiteral("inputSource/") + SETTING_LOCALCONTROLLER_DEVICEID, QString());
    testSettings.sync();
    selector.reloadFromSettings();

    globalSetting = previous;
    return 0;
}

