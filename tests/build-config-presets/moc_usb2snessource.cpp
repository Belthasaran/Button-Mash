/****************************************************************************
** Meta object code from reading C++ file 'usb2snessource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../usb2snessource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usb2snessource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Usb2SnesSource_t {
    QByteArrayData data[4];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Usb2SnesSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Usb2SnesSource_t qt_meta_stringdata_Usb2SnesSource = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Usb2SnesSource"
QT_MOC_LITERAL(1, 15, 11), // "onTimerTick"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 19) // "onUsb2SnesConnected"

    },
    "Usb2SnesSource\0onTimerTick\0\0"
    "onUsb2SnesConnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Usb2SnesSource[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    0,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Usb2SnesSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Usb2SnesSource *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onTimerTick(); break;
        case 1: _t->onUsb2SnesConnected(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject Usb2SnesSource::staticMetaObject = { {
    QMetaObject::SuperData::link<InputProvider::staticMetaObject>(),
    qt_meta_stringdata_Usb2SnesSource.data,
    qt_meta_data_Usb2SnesSource,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Usb2SnesSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Usb2SnesSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Usb2SnesSource.stringdata0))
        return static_cast<void*>(this);
    return InputProvider::qt_metacast(_clname);
}

int Usb2SnesSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = InputProvider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
