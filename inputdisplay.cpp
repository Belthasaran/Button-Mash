#include "arduinocom.h"
#include "inputdisplay.h"
#include "buttonmashdebug.h"
#include "skinparser.h"
#include "skinpath.h"
#include "ui_inputdisplay.h"
#include <QDomDocument>
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QShowEvent>

extern QSettings *globalSetting;

#define SNES_CLASSIC_IP "169.254.13.37"

InputDisplay::InputDisplay(RegularSkin skin, PianoSkin pSkin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputDisplay)
{
    ui->setupUi(this);
    QDomDocument    doc;
    scene = new QGraphicsScene();
    QFileInfo fi(skin.file);
    const QString skinRoot = fi.absolutePath();
    QString pathErr;
    const QString bgPath = SkinPath::resolveSkinRelativePath(skinRoot, skin.background, &pathErr);
    QPixmap background(bgPath);
    if (background.isNull() && !skin.background.isEmpty()) {
        qCDebug(buttonmashLog) << "Failed to load skin background:" << skin.background << pathErr;
    }
    int sceneW = background.width();
    int sceneH = background.height();
    if (sceneW <= 0 || sceneH <= 0) {
        sceneW = 400;
        sceneH = 200;
    }
    scene->setSceneRect(0, 0, sceneW, sceneH);
    //this->setFixedSize(background.size().width() + 5, background.size().height() + 5);
    if (!background.isNull())
        scene->addPixmap(background);
    foreach(RegularButtonSkin but, skin.buttons)
    {
        QString imgErr;
        const QString imgPath = SkinPath::resolveSkinRelativePath(skinRoot, but.image, &imgErr);
        QPixmap pix(imgPath);
        if (pix.isNull()) {
            qCDebug(buttonmashLog) << "Failed to load button image:" << but.name << but.image << imgErr;
            continue;
        }
        QGraphicsPixmapItem* newPix = new QGraphicsPixmapItem(pix.scaled(but.width, but.height));
        newPix->setPos(but.x, but.y);
        newPix->setZValue(1);
        scene->addItem(newPix);
        mapItems[but.name] = newPix;
        newPix->hide();
    }
    ui->graphicsView->setScene(scene);
    mapButtonToText[InputProvider::SNESButton::A] = "a";
    mapButtonToText[InputProvider::SNESButton::B] = "b";
    mapButtonToText[InputProvider::SNESButton::X] = "x";
    mapButtonToText[InputProvider::SNESButton::Y] = "y";
    mapButtonToText[InputProvider::SNESButton::L] = "l";
    mapButtonToText[InputProvider::SNESButton::R] = "r";
    mapButtonToText[InputProvider::SNESButton::Start] = "start";
    mapButtonToText[InputProvider::SNESButton::Select] = "select";
    mapButtonToText[InputProvider::SNESButton::Up] = "up";
    mapButtonToText[InputProvider::SNESButton::Down] = "down";
    mapButtonToText[InputProvider::SNESButton::Right] = "right";
    mapButtonToText[InputProvider::SNESButton::Left] = "left";

    pianoTimer.setInterval(10);
    pianoTimer.start();
    pianoTimeRange = 3000;
    pianoHeight = 0;
    inputProvider = nullptr;
    m_delai = 0;
    ui->pianoLabel->setVisible(false);
    ui->pianoTagLabel->setVisible(false);
    int windowWidth = scene->sceneRect().width() + 10;
    if (!pSkin.name.isEmpty())
    {
        configPianoDisplay(pSkin);
        ui->pianoLabel->setPixmap(*pianoDisplay);
        setPianoLabel();
        if (pianoDisplay->width() > windowWidth)
            windowWidth = pianoDisplay->width();
        pianoHeight = pianoDisplay->height();
        connect(&pianoTimer, SIGNAL(timeout()), this, SLOT(onPianoTimerTimeout()));
        ui->pianoLabel->setVisible(true);
        ui->pianoTagLabel->setVisible(true);
    }
    m_baseWidth = windowWidth;
    m_baseHeight = static_cast<int>(scene->sceneRect().height() + pianoHeight + 45);
    m_legacyScaling = false;
    if (globalSetting != nullptr)
        m_legacyScaling = globalSetting->value(QStringLiteral("display/legacyScaling"), false).toBool();
    this->setFixedSize(m_baseWidth, m_baseHeight);
    this->setStyleSheet("background-color: black;");
}

void InputDisplay::showEvent(QShowEvent *ev)
{
    if (m_legacyScaling) {
        const qreal dpr = devicePixelRatioF();
        if (dpr > 1.0) {
            setFixedSize(qRound(m_baseWidth / dpr), qRound(m_baseHeight / dpr));
        } else {
            setFixedSize(m_baseWidth, m_baseHeight);
        }
    }
    QWidget::showEvent(ev);
}

void InputDisplay::setInputProvider(InputProvider *inp)
{
    inputProvider = inp;
    if (m_delai == 0) {
        connect(inputProvider, &InputProvider::buttonPressed, this, &InputDisplay::onButtonPressed, Qt::UniqueConnection);
        connect(inputProvider, &InputProvider::buttonReleased, this, &InputDisplay::onButtonReleased, Qt::UniqueConnection);
    } else {
        setConnectionWithDelai();
    }
    connect(inputProvider, &InputProvider::unReady, this, &InputDisplay::close);
}

void InputDisplay::setConnectionWithDelai()
{
    disconnect(inputProvider, &InputProvider::buttonPressed, this, &InputDisplay::onButtonPressed);
    disconnect(inputProvider, &InputProvider::buttonReleased, this, &InputDisplay::onButtonReleased);
    connect(inputProvider, &InputProvider::buttonPressed, this, [=] (InputProvider::SNESButton button) {
        QTimer::singleShot(m_delai, this, [=]{
            onButtonPressed(button);
        });
    });
    connect(inputProvider, &InputProvider::buttonReleased, this, [=] (InputProvider::SNESButton button) {
        QTimer::singleShot(m_delai, this, [=]{
            onButtonReleased(button);
        });
    });
}

void InputDisplay::setDelai(unsigned int delai)
{
    m_delai = delai;
    if (inputProvider != nullptr)
        setConnectionWithDelai();
}




void    InputDisplay::configPianoDisplay(PianoSkin pSkin)
{
    QFileInfo fi(pSkin.file);
    pianoDisplay = new QPixmap(pSkin.width, pSkin.height);
    foreach(PianoButton pBut, pSkin.buttons)
    {
        InputProvider::SNESButton but = mapButtonToText.key(pBut.name);
        pianoButPos[but] = pBut.x;
        pianoButColor[but] = pBut.color;
        pianoButWidth[but] = pBut.width;
    }
}

void    InputDisplay::setPianoLabel()
{
    QPixmap*    tag = new QPixmap(400, 30);
    QMap<InputProvider::SNESButton, QString>        alternateText;
    tag->fill(Qt::black);
    QPainter painter(tag);
    QFont font("DejaVu Sans Mono");
    font.setBold(true);
    font.setPixelSize(12);
    painter.setFont(font);

    alternateText[InputProvider::Left] = "<";
    alternateText[InputProvider::Right] = ">";
    alternateText[InputProvider::Up] = "^";
    alternateText[InputProvider::Down] = "v";
    alternateText[InputProvider::Start] = "ST";
    alternateText[InputProvider::Select] = "SEL";

    for(InputProvider::SNESButton& but : mapButtonToText.keys())
    {
        QString butText;
        if (alternateText.contains(but))
            butText = alternateText[but];
        else
            butText = mapButtonToText[but].toUpper();
        painter.setPen(pianoButColor[but]);
        painter.drawText(QRect(pianoButPos[but] - 5, 0, 30, 20), Qt::AlignCenter, butText);
    }
    ui->pianoTagLabel->setPixmap(*tag);
}


void InputDisplay::onButtonPressed(InputProvider::SNESButton button)
{
    qCDebug(buttonmashLog) << button << "Pressed" << QTime::currentTime();
    if (mapItems.contains(mapButtonToText[button]))
        mapItems[mapButtonToText[button]]->show();
    PianoEvent pe;
    pe.startTime = QTime::currentTime();
    pianoEvents[button].append(pe);
}

void InputDisplay::onButtonReleased(InputProvider::SNESButton button)
{
    qCDebug(buttonmashLog) << button << "Released" << QTime::currentTime();
    if (mapItems.contains(mapButtonToText[button]))
        mapItems[mapButtonToText[button]]->hide();
    if (pianoEvents.contains(button) && pianoEvents[button].isEmpty() == false)
        pianoEvents[button].last().endTime = QTime::currentTime();
}

void    InputDisplay::filterPianoEvent()
{
        QTime now = QTime::currentTime();
        QTime bottomTime = now.addMSecs(static_cast<int>(pianoTimeRange) * -1);
        for (InputProvider::SNESButton& but : pianoEvents.keys())
        {
            QMutableListIterator<PianoEvent> iPe(pianoEvents[but]);
            while (iPe.hasNext())
            {
                PianoEvent& pe = iPe.next();
                if (pe.endTime.isNull() == false && pe.endTime.isValid() && pe.endTime <= bottomTime)
                {
                    //qCDebug(buttonmashLog) << "removing" << but << " Time : " << pe.startTime << " - " << pe.endTime;
                    iPe.remove();
                }

            }
        }
}

void InputDisplay::onPianoTimerTimeout()
{
    QTime now = QTime::currentTime();
    //QTime bottomTime = now.addMSecs(static_cast<int>(pianoTimeRange) * -1);
    filterPianoEvent();
    QPainter pa(pianoDisplay);
    pa.fillRect(pianoDisplay->rect(), Qt::black);

    for (InputProvider::SNESButton& but : pianoEvents.keys())
    {
        const QList<PianoEvent> ev = pianoEvents[but];
            for (const PianoEvent& pe : ev)
            {
                /*if (pe.endTime.isNull())
                {
                    qCDebug(buttonmashLog) << "Button without endtime" << but << pe.startTime;
                    if (pe.startTime.msecsTo(now) > 1000)
                        exit(0);
                }*/
                //qCDebug(buttonmashLog) << pe.startTime << pe.endTime;
                int yRect, hRect;
                if (pe.endTime.isNull())
                {
                    yRect = 0;
                    hRect = (pe.startTime.msecsTo(now) * pianoHeight) / pianoTimeRange;
                }
                else
                {
                    yRect = (pe.endTime.msecsTo(now) * pianoHeight) / pianoTimeRange;
                    hRect = (pe.startTime.msecsTo(pe.endTime) * pianoHeight) / pianoTimeRange;
                }
                QRect   rect(pianoButPos[but], yRect, pianoButWidth[but], hRect);
                //qCDebug(buttonmashLog) << rect;

                //pa.setPen(Qt::blue);
                pa.fillRect(rect, pianoButColor[but]);
            }

    }
    ui->pianoLabel->setPixmap(*pianoDisplay);
    ui->pianoLabel->update();
}

void InputDisplay::closeEvent(QCloseEvent *ev)
{
     emit closed();
}


InputDisplay::~InputDisplay()
{
    delete ui;
}
