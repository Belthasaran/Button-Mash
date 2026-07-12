/****************************************************************************
** Meta object code from reading C++ file 'mapbuttondialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../mapbuttondialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapbuttondialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MapButtonDialog_t {
    QByteArrayData data[13];
    char stringdata0[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapButtonDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapButtonDialog_t qt_meta_stringdata_MapButtonDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MapButtonDialog"
QT_MOC_LITERAL(1, 16, 20), // "onButtonGroupClicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 16), // "QAbstractButton*"
QT_MOC_LITERAL(4, 55, 6), // "button"
QT_MOC_LITERAL(5, 62, 25), // "onGamepadButtonPressEvent"
QT_MOC_LITERAL(6, 88, 8), // "deviceId"
QT_MOC_LITERAL(7, 97, 30), // "QGamepadManager::GamepadButton"
QT_MOC_LITERAL(8, 128, 5), // "value"
QT_MOC_LITERAL(9, 134, 27), // "onGamepadButtonReleaseEvent"
QT_MOC_LITERAL(10, 162, 18), // "onGamepadAxisEvent"
QT_MOC_LITERAL(11, 181, 28), // "QGamepadManager::GamepadAxis"
QT_MOC_LITERAL(12, 210, 4) // "axis"

    },
    "MapButtonDialog\0onButtonGroupClicked\0"
    "\0QAbstractButton*\0button\0"
    "onGamepadButtonPressEvent\0deviceId\0"
    "QGamepadManager::GamepadButton\0value\0"
    "onGamepadButtonReleaseEvent\0"
    "onGamepadAxisEvent\0QGamepadManager::GamepadAxis\0"
    "axis"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapButtonDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       5,    3,   37,    2, 0x08 /* Private */,
       9,    2,   44,    2, 0x08 /* Private */,
      10,    3,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7, QMetaType::Double,    6,    4,    8,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,    6,    4,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 11, QMetaType::Double,    6,   12,    8,

       0        // eod
};

void MapButtonDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MapButtonDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onButtonGroupClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 1: _t->onGamepadButtonPressEvent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QGamepadManager::GamepadButton(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 2: _t->onGamepadButtonReleaseEvent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QGamepadManager::GamepadButton(*)>(_a[2]))); break;
        case 3: _t->onGamepadAxisEvent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QGamepadManager::GamepadAxis(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGamepadManager::GamepadButton >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGamepadManager::GamepadButton >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGamepadManager::GamepadAxis >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MapButtonDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MapButtonDialog.data,
    qt_meta_data_MapButtonDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MapButtonDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapButtonDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MapButtonDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MapButtonDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
