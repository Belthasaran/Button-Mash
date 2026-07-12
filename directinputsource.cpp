#include "directinputsource.h"
#include "buttonmashdebug.h"

DirectInputSource::DirectInputSource(int device)
{
    controller = new QGameController(device);
    connect(controller, &QGameController::gameControllerButtonEvent, this, &DirectInputSource::handleButtonEvent);
    connect(controller, &QGameController::gameControllerAxisEvent, this, &DirectInputSource::handleAxisEvent);
    connect(&timer, &QTimer::timeout, controller, &QGameController::readGameController);
    timer.setInterval(10);
    for (unsigned int i = 0; i < controller->axisCount(); i++)
    {
        oldAxisValue[i] = 0;
    }
}


void DirectInputSource::start()
{
    timer.start();
}

void DirectInputSource::stop()
{
    timer.stop();
}

bool DirectInputSource::isReady()
{
    return controller->isValid();
}

QString DirectInputSource::statusText()
{
    return tr("Gamepad connected");
}

QString DirectInputSource::name() const
{
    return controller->description();
}

void DirectInputSource::setMapping(QMap<InputProvider::SNESButton, LocalControllerButtonAxisInfos> map)
{
    QMapIterator<InputProvider::SNESButton, LocalControllerButtonAxisInfos> it(map);
    buttonMapping.clear();
    axisMapping.clear();
    while (it.hasNext())
    {
        it.next();
        if (it.value().button != -1)
        {
            buttonMapping[it.value().button] = it.key();
        } else {
            axisMapping[QPair<unsigned int, double>(it.value().axis, it.value().value)] = it.key();
        }
    }
    qCDebug(buttonmashLog) << buttonMapping;
    qCDebug(buttonmashLog) << axisMapping;
}

void DirectInputSource::handleButtonEvent(QGameControllerButtonEvent *buttonEvent)
{
    if (buttonMapping.contains(buttonEvent->button()))
    {
        if (buttonEvent->pressed())
        {
            emit buttonPressed(buttonMapping[buttonEvent->button()]);
        } else {
            emit buttonReleased(buttonMapping[buttonEvent->button()]);
        }
    }
    delete buttonEvent;
}

void DirectInputSource::handleAxisEvent(QGameControllerAxisEvent *axisEvent)
{
    QMapIterator<QPair<unsigned int, double>, InputProvider::SNESButton> it(axisMapping);
    qCDebug(buttonmashLog) << "Axis event" << axisEvent->axis() << axisEvent->value();
    while (it.hasNext())
    {
        it.next();
        if (it.key().first == axisEvent->axis())
        {
            qCDebug(buttonmashLog) << "Axis match " << it.value() << " old value " << oldAxisValue[axisEvent->axis()];
            if (axisEvent->value() == it.key().second)
            {
                qCDebug(buttonmashLog) << "Axis " << axisEvent->axis() << " pressed";
                emit buttonPressed(it.value());
                continue;
            }
            if (axisEvent->value() != oldAxisValue[axisEvent->axis()]
             && it.key().second == oldAxisValue[axisEvent->axis()])
            {
                qCDebug(buttonmashLog) << "Axis " << axisEvent->axis() << " released";
                emit buttonReleased(it.value());
            }
        }
    }
    oldAxisValue[axisEvent->axis()] = axisEvent->value();
    delete axisEvent;
}
