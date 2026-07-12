#include <QDomDocument>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include "skinselector.h"
#include "configpresetstore.h"
#include "ui_skinselector.h"
#include "inputdisplay.h"
#include "skinparser.h"
#include "sqpath.h"
#include "inputmirrormanager.h"
#include "mirrortargetsdialog.h"
#include "inputtriggers.h"
#include "triggersdialog.h"

#include <QDir>
#include <QProcess>
#include <QProcessEnvironment>
#include <QProcessEnvironment>
#include <QShortcut>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QFont>
#include <QLineEdit>

QSettings* globalSetting;

static QSettings *createButtonMashSettings()
{
    const QString overridePath =
        QProcessEnvironment::systemEnvironment().value(QStringLiteral("BUTTONMASH_CONFIG"));
    if (!overridePath.isEmpty()) {
        // Explicit file path: read/save only this config (for multiple test instances).
        return new QSettings(overridePath, QSettings::IniFormat);
    }
#ifdef Q_OS_WINDOWS
    return new QSettings("ButtonMash", QSettings::IniFormat);
#else
    return new QSettings("ButtonMash");
#endif
}

SkinSelector::SkinSelector(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SkinSelector)
{
    ui->setupUi(this);
    if (logicalDpiX() > 120) {
        QFont f = font();
        f.setPointSize(f.pointSize() + 2);
        setFont(f);
    }
    ui->statusLabel->setWordWrap(true);
    listModel = new QStandardItemModel();
    ui->skinListView->setModel(listModel);
    subSkinModel = new QStandardItemModel();
    ui->subSkinListView->setModel(subSkinModel);
    pianoModel = new QStandardItemModel();
    ui->pianoSkinListView->setModel(pianoModel);
    globalSetting = createButtonMashSettings();
    //globalSetting->clear();
    if (globalSetting->contains("skinFolder"))
    {
        setSkinPath(globalSetting->value("skinFolder").toString());
    } else {
        setSkinPath(SQPath::softwareDatasPath() + "/Skins");
        if (listModel->item(0) != nullptr)
            on_skinListView_clicked(listModel->item(0)->index());
    }
    timer.setInterval(50);
    timer.start();
    display = nullptr;
    inputProvider = nullptr;
    connect(&timer, &QTimer::timeout, this, &SkinSelector::onTimerTimeout);
    inputSelector = new InputSourceSelector(this);
    mirrorManager = new InputMirrorManager(this);
    mirrorManager->loadSettings(*globalSetting);
    ui->shareMirrorCheckBox->setChecked(mirrorManager->shareEnabled());
    triggersEngine = new InputTriggersEngine(this);
    triggersEngine->loadSettings(*globalSetting);
    ui->inputTriggersCheckBox->setChecked(triggersEngine->enabled());
    refreshPresetCombo();
}

void    SkinSelector::setPreviewScene(const RegularSkin& skin)
{
    QGraphicsScene* scene = ui->previewGraphicView->scene();
    if (scene == nullptr)
    {
        scene = new QGraphicsScene();
        ui->previewGraphicView->setScene(scene);
    }
    scene->clear();
    QFileInfo fi(skin.file);
    QPixmap background(fi.absolutePath() + "/" + skin.background);
    scene->setSceneRect(0, 0, background.size().width(), background.size().height());
    scene->addPixmap(background);
    foreach(RegularButtonSkin but, skin.buttons)
    {
        QPixmap pix(fi.absolutePath() + "/" + but.image);
        QGraphicsPixmapItem* newPix = new QGraphicsPixmapItem(pix.scaled(but.width, but.height));
        newPix->setPos(but.x, but.y);
        newPix->setZValue(1);
        scene->addItem(newPix);
    }
    ui->previewGraphicView->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void    SkinSelector::restoreLastSkin()
{
    //Skin
    for (unsigned int i = 0; i < listModel->rowCount(); i++)
    {
        const RegularSkin& sk = listModel->item(i)->data(Qt::UserRole + 2).value<RegularSkin>();
        if (sk.file == globalSetting->value("lastSkin/regularSkinPath").toString())
        {
            ui->skinListView->setCurrentIndex(listModel->item(i)->index());
            on_skinListView_clicked(listModel->item(i)->index());
            currentSkin = sk;
            if (!sk.subSkins.isEmpty())
            {
                unsigned int j = 0;
                qDebug() << "Sub skin" << globalSetting->value("lastSkin/regularSubSkin").toString();
                foreach(RegularSkin ssk, sk.subSkins)
                {
                    qDebug() << ssk.name;
                    if (ssk.name == globalSetting->value("lastSkin/regularSubSkin").toString())
                    {
                        ui->subSkinListView->setCurrentIndex(subSkinModel->item(j)->index());
                        qDebug() << "Crash is here?";
                        currentSkin = ssk;
                        break;
                    }
                    j++;
                }
            }
            qDebug() << "Set preview";
            setPreviewScene(currentSkin);

            break;
        }
    }
    //Piano Display
    qDebug() << "Piano Display" << globalSetting->value("lastSkin/pianoDisplay");
    ui->pianoCheckBox->setChecked(globalSetting->value("lastSkin/pianoDisplay").toBool());
    if (!globalSetting->value("lastSkin/pianoDisplay").toBool())
        return;
    for (unsigned int i = 0; i < pianoModel->rowCount(); i++)
    {
        const PianoSkin& sk = pianoModel->item(i)->data(Qt::UserRole + 2).value<PianoSkin>();
        if (sk.file == globalSetting->value("lastSkin/pianoSkinPath").toString())
        {
            ui->pianoSkinListView->setCurrentIndex(pianoModel->item(i)->index());
            break;
        }
    }
}


void    SkinSelector::saveSkinStarted()
{
    globalSetting->setValue("lastSkin/pianoDisplay", ui->pianoCheckBox->isChecked());
    globalSetting->setValue("lastSkin/regularSkinPath", currentSkin.file);
    if (ui->pianoSkinListView->currentIndex().isValid())
    {
        globalSetting->setValue("lastSkin/pianoSkinPath", pianoModel->itemFromIndex(
                                ui->pianoSkinListView->currentIndex())->data(Qt::UserRole + 2).value<PianoSkin>().file);
    }
    globalSetting->setValue("lastSkin/regularSubSkin", currentSkin.name);

}

void    SkinSelector::setSkinPath(QString path)
{
    listModel->clear();
    pianoModel->clear();
#ifdef SQPROJECT_INSTALLED
    addSkinPath(SQPath::softwareDatasPath() + "/Skins");
    if (path == SQPath::softwareDatasPath() + "/Skins")
        return ;
#endif
    globalSetting->setValue("skinFolder", path);
    addSkinPath(path);
}

void    SkinSelector::addSkinPath(QString path)
{
    ui->skinPathEdit->setText(path);
    QDir dir(path);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    auto list = dir.entryInfoList();
    foreach(QFileInfo fi, list)
    {
        qDebug() << fi.fileName();
        if (QFileInfo::exists(fi.absoluteFilePath() + "/skin.xml"))
        {
            qDebug() << "Found a skin file";
            RegularSkin skin = SkinParser::parseRegularSkin(fi.absoluteFilePath() + "/skin.xml");
            if (!SkinParser::xmlError.isEmpty())
                qDebug() << SkinParser::xmlError;
            //qDebug() << skin;
            QStandardItem* item = new QStandardItem(QString(tr("%1 by %2")).arg(skin.name).arg(skin.author));
            QVariant var;
            var.setValue(skin);
            item->setData(var, Qt::UserRole + 2);
            listModel->appendRow(item);
            //ui->skinListView->setCurrentIndex(listModel->item(0)->index());
        }
        if (QFileInfo::exists(fi.absoluteFilePath() + "/pianodisplay.xml"))
        {
            ui->pianoCheckBox->setEnabled(true);
            qDebug() << "Found a piano skin file";
            PianoSkin pSkin = SkinParser::parsePianoSkin(fi.absoluteFilePath() + "/pianodisplay.xml");
            //qDebug() << pSkin;
            QStandardItem* item = new QStandardItem(QString(tr("%1 by %2")).arg(pSkin.name).arg(pSkin.author));
            QVariant var;
            var.setValue(pSkin);
            item->setData(var, Qt::UserRole + 2);
            pianoModel->appendRow(item);
            //ui->pianoSkinListView->setCurrentIndex(pianoModel->item(0)->index());
        }

    }
}

SkinSelector::~SkinSelector()
{
    delete ui;
}



void SkinSelector::on_startButton_clicked()
{
    if (inputProvider == nullptr)
        return;
    PianoSkin pSkin;
    if (ui->pianoCheckBox->isChecked())
    {
        QModelIndex pianoIndex = ui->pianoSkinListView->currentIndex();
        QStandardItem* pianoItem = pianoModel->itemFromIndex(pianoIndex);
        if (!pianoIndex.isValid() || pianoItem == nullptr)
        {
            QMessageBox::warning(this, tr("Button Mash"),
                                 tr("Please select a piano display skin from the list below."));
            return;
        }
        pSkin = pianoItem->data(Qt::UserRole + 2).value<PianoSkin>();
    }
    display = new InputDisplay(currentSkin, pSkin);
    display->setInputProvider(inputProvider);
    if (inputSelector->delai() != 0)
        display->setDelai(inputSelector->delai());
    connect(display, &InputDisplay::closed, this, &SkinSelector::onDisplayClosed);
    connect(inputProvider, &InputProvider::buttonPressed, mirrorManager, &InputMirrorManager::onButtonPressed);
    connect(inputProvider, &InputProvider::buttonReleased, mirrorManager, &InputMirrorManager::onButtonReleased);
    connect(inputProvider, &InputProvider::buttonPressed, triggersEngine, &InputTriggersEngine::onButtonPressed);
    connect(inputProvider, &InputProvider::buttonReleased, triggersEngine, &InputTriggersEngine::onButtonReleased);
    mirrorManager->setShareEnabled(ui->shareMirrorCheckBox->isChecked());
    mirrorManager->saveSettings(*globalSetting);
    triggersEngine->setEnabled(ui->inputTriggersCheckBox->isChecked());
    triggersEngine->saveSettings(*globalSetting);
    mirrorManager->startSession();
    triggersEngine->startSession();
    display->show();
    inputProvider->start();
    this->hide();
    saveSkinStarted();
  }

void SkinSelector::on_pianoCheckBox_stateChanged(int arg1)
{
    ui->pianoSkinListView->setEnabled(arg1 != 0);
    if (arg1 != 0 && !ui->pianoSkinListView->currentIndex().isValid())
        ui->startButton->setEnabled(false);
}

void SkinSelector::on_skinListView_clicked(const QModelIndex &index)
{
    const RegularSkin& skin = listModel->itemFromIndex(index)->data(Qt::UserRole + 2).value<RegularSkin>();
    subSkinModel->clear();
    if (skin.subSkins.isEmpty())
        currentSkin = skin;
    else
        currentSkin = skin.subSkins.first();
    foreach(RegularSkin sk, skin.subSkins)
    {
        QStandardItem* item = new QStandardItem(sk.name);
        QVariant var;
        var.setValue(sk);
        item->setData(var, Qt::UserRole + 2);
        subSkinModel->appendRow(item);
        ui->subSkinListView->setCurrentIndex(subSkinModel->item(0)->index());
    }
    setPreviewScene(skin);
}

void SkinSelector::onTimerTimeout()
{
    static bool first = true;
    if (first)
    {
        first = false;
        timer.setInterval(1000);
        restoreLastSkin();
        inputProvider = inputSelector->getLastProvider();
        if (inputProvider != nullptr)
            ui->sourceLabel->setText(QString("<b>%1</b>").arg(inputProvider->name()));
        else
            ui->sourceLabel->setText(tr("No Source provider selected"));
        return ;
    }
    if ((display != nullptr && display->isVisible()) || inputProvider == nullptr)
        return ;
    bool pianoReady = !ui->pianoCheckBox->isChecked() || ui->pianoSkinListView->currentIndex().isValid();
    if (inputProvider->isReady() && !currentSkin.name.isEmpty() && pianoReady)
        ui->startButton->setEnabled(true);
    else {
        ui->startButton->setEnabled(false);
    }
    if (currentSkin.name.isEmpty())
        ui->statusLabel->setText(tr("You need to select a skin. ") + inputProvider->statusText());
    else if (ui->pianoCheckBox->isChecked() && !ui->pianoSkinListView->currentIndex().isValid())
        ui->statusLabel->setText(tr("You need to select a piano display skin."));
    else {
        ui->statusLabel->setText(inputProvider->statusText());
    }
}

void SkinSelector::on_skinPathButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose the default folder for skins"), qApp->applicationDirPath() + "/Skins");
    if (!dir.isEmpty())
    {
        setSkinPath(dir);
        if (listModel->item(0) != nullptr)
            on_skinListView_clicked(listModel->item(0)->index());
    }
}

void SkinSelector::on_subSkinListView_clicked(const QModelIndex &index)
{
    const RegularSkin& skin = subSkinModel->itemFromIndex(index)->data(Qt::UserRole + 2).value<RegularSkin>();
    currentSkin = skin;
    setPreviewScene(skin);
}


void SkinSelector::onDisplayClosed()
{
    show();
    if (inputProvider != nullptr) {
        disconnect(inputProvider, &InputProvider::buttonPressed, mirrorManager, &InputMirrorManager::onButtonPressed);
        disconnect(inputProvider, &InputProvider::buttonReleased, mirrorManager, &InputMirrorManager::onButtonReleased);
        disconnect(inputProvider, &InputProvider::buttonPressed, triggersEngine, &InputTriggersEngine::onButtonPressed);
        disconnect(inputProvider, &InputProvider::buttonReleased, triggersEngine, &InputTriggersEngine::onButtonReleased);
        inputProvider->stop();
    }
    triggersEngine->stopSession();
    mirrorManager->stopSession();
    mirrorManager->saveSettings(*globalSetting);
    triggersEngine->saveSettings(*globalSetting);
}

void SkinSelector::on_changeSourceButton_clicked()
{
    timer.stop();
    if (inputSelector->exec())
    {
        inputProvider = inputSelector->currentProvider();
        if (inputProvider != nullptr)
            ui->sourceLabel->setText(QString("<b>%1</b>").arg(inputProvider->name()));
        else
            ui->sourceLabel->setText(tr("No Source provider selected"));
    }
    timer.start();
}


void SkinSelector::on_shareMirrorCheckBox_toggled(bool checked)
{
    mirrorManager->setShareEnabled(checked);
    mirrorManager->saveSettings(*globalSetting);
}

void SkinSelector::on_mirrorTargetsButton_clicked()
{
    MirrorTargetsDialog dlg(mirrorManager, this);
    if (dlg.exec() == QDialog::Accepted)
        mirrorManager->saveSettings(*globalSetting);
}

void SkinSelector::on_inputTriggersCheckBox_toggled(bool checked)
{
    triggersEngine->setEnabled(checked);
    triggersEngine->saveSettings(*globalSetting);
}

void SkinSelector::on_configureTriggersButton_clicked()
{
    TriggersDialog dlg(triggersEngine, this);
    if (dlg.exec() == QDialog::Accepted)
        triggersEngine->saveSettings(*globalSetting);
}

void SkinSelector::on_skinEditorButton_clicked()
{
    QString editor = QCoreApplication::applicationDirPath() + QStringLiteral("/MashEditor");
#ifdef Q_OS_WIN
    editor += QStringLiteral(".exe");
#endif
    if (!QFileInfo::exists(editor)) {
        const QString alt = QCoreApplication::applicationDirPath()
                          + QStringLiteral("/../build-mash/MashEditor");
        if (QFileInfo::exists(alt))
            editor = alt;
    }
    if (!QFileInfo::exists(editor)) {
        QMessageBox::warning(this, tr("Skin Editor"),
                             tr("MashEditor not found at %1").arg(editor));
        return;
    }
    QStringList args;
    args << QStringLiteral("--skins-dir") << ui->skinPathEdit->text();
    const QModelIndex idx = ui->skinListView->currentIndex();
    if (idx.isValid()) {
        const RegularSkin sk = idx.data(Qt::UserRole + 2).value<RegularSkin>();
        if (!sk.file.isEmpty())
            args << QStringLiteral("--skin") << QFileInfo(sk.file).absoluteDir().dirName();
    }
    if (!QProcess::startDetached(editor, args)) {
        QMessageBox::warning(this, tr("Skin Editor"), tr("Failed to start MashEditor."));
    }
}

void SkinSelector::refreshPresetCombo()
{
    const QString previous = ui->presetComboBox->currentText();
    ui->presetComboBox->blockSignals(true);
    ui->presetComboBox->clear();
    const QStringList presetNames = ConfigPresetStore::names(*globalSetting);
    ui->presetComboBox->addItems(presetNames);

    QString selected = globalSetting->value(QStringLiteral("presets/lastSelected")).toString();
    if (!selected.isEmpty() && presetNames.contains(selected))
        ui->presetComboBox->setCurrentText(selected);
    else if (!previous.isEmpty() && presetNames.contains(previous))
        ui->presetComboBox->setCurrentText(previous);

    ui->presetComboBox->blockSignals(false);
}

QString SkinSelector::currentPresetName() const
{
    return ui->presetComboBox->currentText().trimmed();
}

void SkinSelector::flushActiveSettingsToStore()
{
    saveSkinStarted();
    mirrorManager->setShareEnabled(ui->shareMirrorCheckBox->isChecked());
    mirrorManager->saveSettings(*globalSetting);
    triggersEngine->setEnabled(ui->inputTriggersCheckBox->isChecked());
    triggersEngine->saveSettings(*globalSetting);
    inputSelector->saveCurrentToSettings();
    globalSetting->sync();
}

void SkinSelector::applyActiveSettingsFromStore()
{
    mirrorManager->loadSettings(*globalSetting);
    ui->shareMirrorCheckBox->setChecked(mirrorManager->shareEnabled());
    triggersEngine->loadSettings(*globalSetting);
    ui->inputTriggersCheckBox->setChecked(triggersEngine->enabled());

    const QString skinFolder = globalSetting->value(QStringLiteral("skinFolder")).toString();
    if (!skinFolder.isEmpty())
        setSkinPath(skinFolder);
    restoreLastSkin();

    if (inputProvider != nullptr)
        inputProvider->stop();
    inputSelector->reloadFromSettings();
    inputProvider = inputSelector->currentProvider();
    if (inputProvider != nullptr)
        ui->sourceLabel->setText(QStringLiteral("<b>%1</b>").arg(inputProvider->name()));
    else
        ui->sourceLabel->setText(tr("No Source provider selected"));

    ui->startButton->setEnabled(false);
}

void SkinSelector::on_presetSwitchButton_clicked()
{
    const QString name = currentPresetName();
    QString err;
    if (!ConfigPresetStore::isValidPresetName(name, &err)) {
        QMessageBox::warning(this, tr("Config Presets"), err);
        return;
    }
    if (!ConfigPresetStore::names(*globalSetting).contains(name)) {
        QMessageBox::warning(this, tr("Config Presets"), tr("Select a preset to switch to."));
        return;
    }

    if (!ConfigPresetStore::applyPreset(*globalSetting, name)) {
        QMessageBox::warning(this, tr("Config Presets"), tr("Failed to apply preset \"%1\".").arg(name));
        return;
    }

    applyActiveSettingsFromStore();
    refreshPresetCombo();
}

void SkinSelector::on_presetNewButton_clicked()
{
    bool ok = false;
    const QString name = QInputDialog::getText(this, tr("New Config Preset"),
                                               tr("Preset name:"), QLineEdit::Normal,
                                               QString(), &ok).trimmed();
    if (!ok)
        return;

    QString err;
    if (!ConfigPresetStore::isValidPresetName(name, &err)) {
        QMessageBox::warning(this, tr("Config Presets"), err);
        return;
    }
    if (ConfigPresetStore::names(*globalSetting).contains(name)) {
        QMessageBox::warning(this, tr("Config Presets"), tr("A preset named \"%1\" already exists.").arg(name));
        return;
    }

    flushActiveSettingsToStore();
    if (!ConfigPresetStore::savePreset(*globalSetting, name)) {
        QMessageBox::warning(this, tr("Config Presets"), tr("Failed to save preset \"%1\".").arg(name));
        return;
    }

    globalSetting->setValue(QStringLiteral("presets/lastSelected"), name);
    refreshPresetCombo();
    ui->presetComboBox->setCurrentText(name);
}

void SkinSelector::on_presetOverwriteButton_clicked()
{
    const QString name = currentPresetName();
    QString err;
    if (!ConfigPresetStore::isValidPresetName(name, &err)) {
        QMessageBox::warning(this, tr("Config Presets"), err);
        return;
    }
    if (!ConfigPresetStore::names(*globalSetting).contains(name)) {
        QMessageBox::warning(this, tr("Config Presets"), tr("Select a preset to overwrite."));
        return;
    }

    const auto answer = QMessageBox::question(this, tr("Overwrite Config Preset"),
                                              tr("Replace preset \"%1\" with the current settings?").arg(name));
    if (answer != QMessageBox::Yes)
        return;

    flushActiveSettingsToStore();
    if (!ConfigPresetStore::savePreset(*globalSetting, name)) {
        QMessageBox::warning(this, tr("Config Presets"), tr("Failed to overwrite preset \"%1\".").arg(name));
        return;
    }

    globalSetting->setValue(QStringLiteral("presets/lastSelected"), name);
    refreshPresetCombo();
}

void SkinSelector::on_presetDeleteButton_clicked()
{
    const QString name = currentPresetName();
    QString err;
    if (!ConfigPresetStore::isValidPresetName(name, &err)) {
        QMessageBox::warning(this, tr("Config Presets"), err);
        return;
    }
    if (!ConfigPresetStore::names(*globalSetting).contains(name)) {
        QMessageBox::warning(this, tr("Config Presets"), tr("Select a preset to delete."));
        return;
    }

    const auto answer = QMessageBox::question(this, tr("Delete Config Preset"),
                                              tr("Delete preset \"%1\"?").arg(name));
    if (answer != QMessageBox::Yes)
        return;

    if (!ConfigPresetStore::deletePreset(*globalSetting, name)) {
        QMessageBox::warning(this, tr("Config Presets"), tr("Failed to delete preset \"%1\".").arg(name));
        return;
    }

    refreshPresetCombo();
}
