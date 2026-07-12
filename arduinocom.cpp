#include "arduinocom.h"
#include "buttonmashdebug.h"

ArduinoCOM::ArduinoCOM(QString port, QObject *parent)
{
    comPort.setPortName(port);
    connect(&comPort, &QSerialPort::readyRead, this, &ArduinoCOM::portReadyRead);
    qCDebug(buttonmashLog) << "Creating arduino com, port " << port << comPort.portName();
    packetSize = 3;
    buttonState = 0;
    configurePacket();
    m_type = NintendoSpy;
}

void ArduinoCOM::configurePacket()
{
    maskToButton[0x0001] = InputProvider::SNESButton::A;
    maskToButton[0x0100] = InputProvider::SNESButton::B;
    maskToButton[0x0002] = InputProvider::SNESButton::X;
    maskToButton[0x0200] = InputProvider::SNESButton::Y;
    maskToButton[0x0800] = InputProvider::SNESButton::Start;
    maskToButton[0x0400] = InputProvider::SNESButton::Select;
    maskToButton[0x0004] = InputProvider::SNESButton::L;
    maskToButton[0x0008] = InputProvider::SNESButton::R;
    maskToButton[0x4000] = InputProvider::SNESButton::Left;
    maskToButton[0x8000] = InputProvider::SNESButton::Right;
    maskToButton[0x1000] = InputProvider::SNESButton::Up;
    maskToButton[0x2000] = InputProvider::SNESButton::Down;

//"b", "y", "select", "start", "up", "down", "left", "right", "a", "x", "l", "r", null, null, null, null
    nintendoSpyButtons = {B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R};
    NSButtonState.fill(0, 12);
}

void ArduinoCOM::setPort(QString port)
{
    qCDebug(buttonmashLog) << "Set port to " << port;
    comPort.setPortName(port);
}

void ArduinoCOM::setType(int type)
{
    m_type = type;
}

QString ArduinoCOM::port() const
{
    return comPort.portName();
}

/* FDFF = Y pressed 1111 1101
 FFFD = X
 FFFE = A
 FEFF = B
 BFFF = < 1011
 7FFF = > 0111
 DFFF = v 1101
 EFFF = ^ 1110
 FFFB = L
 FFF7 = R
 FBFF = start
 F7FF = select

00000001 = FFFFFFF0

XXX0XXX0 ? FFFFFFF0 =

XXXE xor 0001 = FFF0

 */


void    ArduinoCOM::processNintendoSpy(QByteArray data)
{
    //qCDebug(buttonmashLog) << "BLOCK" << data.toHex();
    if (data.size() < 12)
        return ;
    for (int i = 0; i < 12; i++)
    {
        if (data.at(i) != NSButtonState.at(i))
        {
            if (data.at(i) == 0x00)
                emit buttonReleased(nintendoSpyButtons.at(i));
            else {
                emit buttonPressed(nintendoSpyButtons.at(i));
            }
        }
        NSButtonState[i] = data.at(i);
    }
}

// Blue, .,  Red, , Yellow,


void ArduinoCOM::portReadyRead()
{
    static QByteArray dataRead;
    QByteArray data = comPort.readAll();
    dataRead.append(data);
    //qCDebug(buttonmashLog) << "Received " << data.size();
    if (m_type == Type::NintendoSpy)
    {
        while (!dataRead.isEmpty())
        {
            //qCDebug(buttonmashLog) << dataRead.toHex();
            int nextSplit = dataRead.indexOf('\n');
            if (nextSplit == -1)
                break;
            //qCDebug(buttonmashLog) << nextSplit;
            processNintendoSpy(dataRead.left(nextSplit));
            dataRead.remove(0, nextSplit + 1);
        }
    }
    return ;
    if (data.size() > packetSize)
        return;

    if (dataRead.size() == packetSize)
    {
        /*if (dataRead != QByteArray::fromHex("FFFFFF"))
            qCDebug(buttonmashLog) << dataRead;*/
        quint16 byte2 = ((uchar)dataRead.at(0) << 8) | (uchar)(dataRead.at(1));
        byte2 = ~byte2;
        //qCDebug(buttonmashLog) << dataRead << QString::number(byte2, 16);
        QMapIterator<quint16, InputProvider::SNESButton> it(maskToButton);
        //qCDebug(buttonmashLog) << QString::number(buttonState, 16);
        while (it.hasNext())
        {
            it.next();
            quint16 mask = it.key();
            //qCDebug(buttonmashLog) << it.value() << QString::number(mask, 16) << QString::number(static_cast<quint16>(~mask), 16);
            if ((byte2 & mask) == mask)
            {
                if ((buttonState & mask) != mask)
                {
                    qCDebug(buttonmashLog) << it.value() << "pressed";
                    emit buttonPressed(it.value());
                    buttonState = buttonState | mask;
                }
            } else {
                if ((buttonState & mask) == mask)
                {
                    buttonState = buttonState ^ mask;
                    qCDebug(buttonmashLog) << it.value() << "released";
                    emit buttonReleased(it.value());
                }
            }
        }
        dataRead.clear();
    }
}


void ArduinoCOM::start()
{
    qCDebug(buttonmashLog) << "opening port connection" << comPort.open(QIODevice::ReadWrite);
    comPort.clear();
    comPort.setBaudRate(115200);
    comPort.setDataTerminalReady(true);
    qCDebug(buttonmashLog) << "BaudRate : " << comPort.baudRate();
    qCDebug(buttonmashLog) << "Databits : " << comPort.dataBits();
    qCDebug(buttonmashLog) << "DataTerminalReady : " << comPort.isDataTerminalReady();
    qCDebug(buttonmashLog) << "Parity : " << comPort.parity();
    qCDebug(buttonmashLog) << "FlowControl : " << comPort.flowControl();
    qCDebug(buttonmashLog) << "Stop bits : " << comPort.stopBits();
}

void ArduinoCOM::stop()
{
    comPort.close();
}

bool ArduinoCOM::isReady()
{
    return true;
}

QString ArduinoCOM::statusText()
{
    return "Ready";
}

QString ArduinoCOM::name() const
{
    return "Arduino on " + comPort.portName();
}
