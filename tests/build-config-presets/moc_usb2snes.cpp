/****************************************************************************
** Meta object code from reading C++ file 'usb2snes.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../usb2snes.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usb2snes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_USB2snes_t {
    QByteArrayData data[41];
    char stringdata0[536];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_USB2snes_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_USB2snes_t qt_meta_stringdata_USB2snes = {
    {
QT_MOC_LITERAL(0, 0, 8), // "USB2snes"
QT_MOC_LITERAL(1, 9, 12), // "stateChanged"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 12), // "disconnected"
QT_MOC_LITERAL(4, 36, 9), // "connected"
QT_MOC_LITERAL(5, 46, 21), // "binaryMessageReceived"
QT_MOC_LITERAL(6, 68, 19), // "textMessageReceived"
QT_MOC_LITERAL(7, 88, 10), // "romStarted"
QT_MOC_LITERAL(8, 99, 11), // "menuStarted"
QT_MOC_LITERAL(9, 111, 16), // "fileSendProgress"
QT_MOC_LITERAL(10, 128, 4), // "size"
QT_MOC_LITERAL(11, 133, 8), // "fileSent"
QT_MOC_LITERAL(12, 142, 14), // "getFileDataGet"
QT_MOC_LITERAL(13, 157, 4), // "data"
QT_MOC_LITERAL(14, 162, 20), // "onWebSocketConnected"
QT_MOC_LITERAL(15, 183, 23), // "onWebSocketDisconnected"
QT_MOC_LITERAL(16, 207, 23), // "onWebSocketTextReceived"
QT_MOC_LITERAL(17, 231, 7), // "message"
QT_MOC_LITERAL(18, 239, 25), // "onWebSocketBinaryReceived"
QT_MOC_LITERAL(19, 265, 16), // "onWebSocketError"
QT_MOC_LITERAL(20, 282, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(21, 311, 3), // "err"
QT_MOC_LITERAL(22, 315, 11), // "onTimerTick"
QT_MOC_LITERAL(23, 327, 5), // "State"
QT_MOC_LITERAL(24, 333, 4), // "None"
QT_MOC_LITERAL(25, 338, 9), // "Connected"
QT_MOC_LITERAL(26, 348, 5), // "Ready"
QT_MOC_LITERAL(27, 354, 11), // "SendingFile"
QT_MOC_LITERAL(28, 366, 14), // "ReceivfingFile"
QT_MOC_LITERAL(29, 381, 12), // "sd2snesState"
QT_MOC_LITERAL(30, 394, 7), // "sd2menu"
QT_MOC_LITERAL(31, 402, 10), // "RomRunning"
QT_MOC_LITERAL(32, 413, 13), // "InternalState"
QT_MOC_LITERAL(33, 427, 5), // "INone"
QT_MOC_LITERAL(34, 433, 10), // "IConnected"
QT_MOC_LITERAL(35, 444, 19), // "DeviceListRequested"
QT_MOC_LITERAL(36, 464, 10), // "AttachSent"
QT_MOC_LITERAL(37, 475, 24), // "FirmwareVersionRequested"
QT_MOC_LITERAL(38, 500, 22), // "ServerVersionRequested"
QT_MOC_LITERAL(39, 523, 6), // "IReady"
QT_MOC_LITERAL(40, 530, 5) // "IBusy"

    },
    "USB2snes\0stateChanged\0\0disconnected\0"
    "connected\0binaryMessageReceived\0"
    "textMessageReceived\0romStarted\0"
    "menuStarted\0fileSendProgress\0size\0"
    "fileSent\0getFileDataGet\0data\0"
    "onWebSocketConnected\0onWebSocketDisconnected\0"
    "onWebSocketTextReceived\0message\0"
    "onWebSocketBinaryReceived\0onWebSocketError\0"
    "QAbstractSocket::SocketError\0err\0"
    "onTimerTick\0State\0None\0Connected\0Ready\0"
    "SendingFile\0ReceivfingFile\0sd2snesState\0"
    "sd2menu\0RomRunning\0InternalState\0INone\0"
    "IConnected\0DeviceListRequested\0"
    "AttachSent\0FirmwareVersionRequested\0"
    "ServerVersionRequested\0IReady\0IBusy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_USB2snes[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       3,  120, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    0,   95,    2, 0x06 /* Public */,
       4,    0,   96,    2, 0x06 /* Public */,
       5,    0,   97,    2, 0x06 /* Public */,
       6,    0,   98,    2, 0x06 /* Public */,
       7,    0,   99,    2, 0x06 /* Public */,
       8,    0,  100,    2, 0x06 /* Public */,
       9,    1,  101,    2, 0x06 /* Public */,
      11,    0,  104,    2, 0x06 /* Public */,
      12,    1,  105,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  108,    2, 0x08 /* Private */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    1,  110,    2, 0x08 /* Private */,
      18,    1,  113,    2, 0x08 /* Private */,
      19,    1,  116,    2, 0x08 /* Private */,
      22,    0,  119,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QByteArray,   17,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      23,   23, 0x0,    5,  135,
      29,   29, 0x0,    2,  145,
      32,   32, 0x0,    8,  149,

 // enum data: key, value
      24, uint(USB2snes::None),
      25, uint(USB2snes::Connected),
      26, uint(USB2snes::Ready),
      27, uint(USB2snes::SendingFile),
      28, uint(USB2snes::ReceivfingFile),
      30, uint(USB2snes::sd2menu),
      31, uint(USB2snes::RomRunning),
      33, uint(USB2snes::INone),
      34, uint(USB2snes::IConnected),
      35, uint(USB2snes::DeviceListRequested),
      36, uint(USB2snes::AttachSent),
      37, uint(USB2snes::FirmwareVersionRequested),
      38, uint(USB2snes::ServerVersionRequested),
      39, uint(USB2snes::IReady),
      40, uint(USB2snes::IBusy),

       0        // eod
};

void USB2snes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<USB2snes *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->stateChanged(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->connected(); break;
        case 3: _t->binaryMessageReceived(); break;
        case 4: _t->textMessageReceived(); break;
        case 5: _t->romStarted(); break;
        case 6: _t->menuStarted(); break;
        case 7: _t->fileSendProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->fileSent(); break;
        case 9: _t->getFileDataGet((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 10: _t->onWebSocketConnected(); break;
        case 11: _t->onWebSocketDisconnected(); break;
        case 12: _t->onWebSocketTextReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->onWebSocketBinaryReceived((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 14: _t->onWebSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 15: _t->onTimerTick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (USB2snes::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::connected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::binaryMessageReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::textMessageReceived)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::romStarted)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::menuStarted)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::fileSendProgress)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::fileSent)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (USB2snes::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&USB2snes::getFileDataGet)) {
                *result = 9;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject USB2snes::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_USB2snes.data,
    qt_meta_data_USB2snes,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *USB2snes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *USB2snes::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_USB2snes.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int USB2snes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void USB2snes::stateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void USB2snes::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void USB2snes::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void USB2snes::binaryMessageReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void USB2snes::textMessageReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void USB2snes::romStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void USB2snes::menuStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void USB2snes::fileSendProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void USB2snes::fileSent()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void USB2snes::getFileDataGet(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
