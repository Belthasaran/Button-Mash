/****************************************************************************
** Meta object code from reading C++ file 'telnetconnection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../telnetconnection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'telnetconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TelnetConnection_t {
    QByteArrayData data[37];
    char stringdata0[508];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TelnetConnection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TelnetConnection_t qt_meta_stringdata_TelnetConnection = {
    {
QT_MOC_LITERAL(0, 0, 16), // "TelnetConnection"
QT_MOC_LITERAL(1, 17, 9), // "connected"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12), // "disconnected"
QT_MOC_LITERAL(4, 41, 5), // "error"
QT_MOC_LITERAL(5, 47, 13), // "commandReturn"
QT_MOC_LITERAL(6, 61, 22), // "commandReturnedNewLine"
QT_MOC_LITERAL(7, 84, 15), // "connectionError"
QT_MOC_LITERAL(8, 100, 33), // "TelnetConnection::ConnectionE..."
QT_MOC_LITERAL(9, 134, 8), // "conneect"
QT_MOC_LITERAL(10, 143, 14), // "executeCommand"
QT_MOC_LITERAL(11, 158, 6), // "toSend"
QT_MOC_LITERAL(12, 165, 5), // "close"
QT_MOC_LITERAL(13, 171, 17), // "onSocketConnected"
QT_MOC_LITERAL(14, 189, 13), // "onSocketError"
QT_MOC_LITERAL(15, 203, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(16, 232, 20), // "onSocketDisconnected"
QT_MOC_LITERAL(17, 253, 17), // "onSocketReadReady"
QT_MOC_LITERAL(18, 271, 15), // "ConnectionError"
QT_MOC_LITERAL(19, 287, 17), // "ConnectionRefused"
QT_MOC_LITERAL(20, 305, 14), // "SNESNotRunning"
QT_MOC_LITERAL(21, 320, 5), // "State"
QT_MOC_LITERAL(22, 326, 7), // "Offline"
QT_MOC_LITERAL(23, 334, 9), // "Connected"
QT_MOC_LITERAL(24, 344, 5), // "Ready"
QT_MOC_LITERAL(25, 350, 17), // "WaitingForCommand"
QT_MOC_LITERAL(26, 368, 13), // "InternalState"
QT_MOC_LITERAL(27, 382, 4), // "Init"
QT_MOC_LITERAL(28, 387, 17), // "AttemptConnection"
QT_MOC_LITERAL(29, 405, 15), // "SocketConnected"
QT_MOC_LITERAL(30, 421, 12), // "LoginWritten"
QT_MOC_LITERAL(31, 434, 6), // "Logged"
QT_MOC_LITERAL(32, 441, 19), // "PromptChangeWritten"
QT_MOC_LITERAL(33, 461, 13), // "PromptChanged"
QT_MOC_LITERAL(34, 475, 6), // "IReady"
QT_MOC_LITERAL(35, 482, 11), // "DataWritten"
QT_MOC_LITERAL(36, 494, 13) // "WaitingForCmd"

    },
    "TelnetConnection\0connected\0\0disconnected\0"
    "error\0commandReturn\0commandReturnedNewLine\0"
    "connectionError\0TelnetConnection::ConnectionError\0"
    "conneect\0executeCommand\0toSend\0close\0"
    "onSocketConnected\0onSocketError\0"
    "QAbstractSocket::SocketError\0"
    "onSocketDisconnected\0onSocketReadReady\0"
    "ConnectionError\0ConnectionRefused\0"
    "SNESNotRunning\0State\0Offline\0Connected\0"
    "Ready\0WaitingForCommand\0InternalState\0"
    "Init\0AttemptConnection\0SocketConnected\0"
    "LoginWritten\0Logged\0PromptChangeWritten\0"
    "PromptChanged\0IReady\0DataWritten\0"
    "WaitingForCmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TelnetConnection[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       3,  102, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    0,   81,    2, 0x06 /* Public */,
       5,    1,   82,    2, 0x06 /* Public */,
       6,    1,   85,    2, 0x06 /* Public */,
       7,    1,   88,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   91,    2, 0x0a /* Public */,
      10,    1,   92,    2, 0x0a /* Public */,
      12,    0,   95,    2, 0x0a /* Public */,
      13,    0,   96,    2, 0x08 /* Private */,
      14,    1,   97,    2, 0x08 /* Private */,
      16,    0,  100,    2, 0x08 /* Private */,
      17,    0,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, 0x80000000 | 8,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,    4,
    QMetaType::Void,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      18,   18, 0x0,    2,  117,
      21,   21, 0x0,    4,  121,
      26,   26, 0x0,   10,  129,

 // enum data: key, value
      19, uint(TelnetConnection::ConnectionRefused),
      20, uint(TelnetConnection::SNESNotRunning),
      22, uint(TelnetConnection::Offline),
      23, uint(TelnetConnection::Connected),
      24, uint(TelnetConnection::Ready),
      25, uint(TelnetConnection::WaitingForCommand),
      27, uint(TelnetConnection::Init),
      28, uint(TelnetConnection::AttemptConnection),
      29, uint(TelnetConnection::SocketConnected),
      30, uint(TelnetConnection::LoginWritten),
      31, uint(TelnetConnection::Logged),
      32, uint(TelnetConnection::PromptChangeWritten),
      33, uint(TelnetConnection::PromptChanged),
      34, uint(TelnetConnection::IReady),
      35, uint(TelnetConnection::DataWritten),
      36, uint(TelnetConnection::WaitingForCmd),

       0        // eod
};

void TelnetConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TelnetConnection *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->error(); break;
        case 3: _t->commandReturn((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->commandReturnedNewLine((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->connectionError((*reinterpret_cast< TelnetConnection::ConnectionError(*)>(_a[1]))); break;
        case 6: _t->conneect(); break;
        case 7: _t->executeCommand((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->close(); break;
        case 9: _t->onSocketConnected(); break;
        case 10: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 11: _t->onSocketDisconnected(); break;
        case 12: _t->onSocketReadReady(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
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
            using _t = void (TelnetConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TelnetConnection::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TelnetConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TelnetConnection::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TelnetConnection::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TelnetConnection::error)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TelnetConnection::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TelnetConnection::commandReturn)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TelnetConnection::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TelnetConnection::commandReturnedNewLine)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TelnetConnection::*)(TelnetConnection::ConnectionError );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TelnetConnection::connectionError)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TelnetConnection::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TelnetConnection.data,
    qt_meta_data_TelnetConnection,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TelnetConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TelnetConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TelnetConnection.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TelnetConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void TelnetConnection::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TelnetConnection::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TelnetConnection::error()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TelnetConnection::commandReturn(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TelnetConnection::commandReturnedNewLine(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TelnetConnection::connectionError(TelnetConnection::ConnectionError _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
