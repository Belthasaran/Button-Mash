/****************************************************************************
** Meta object code from reading C++ file 'inputmirrormanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../inputmirrormanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inputmirrormanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InputMirrorManager_t {
    QByteArrayData data[7];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InputMirrorManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InputMirrorManager_t qt_meta_stringdata_InputMirrorManager = {
    {
QT_MOC_LITERAL(0, 0, 18), // "InputMirrorManager"
QT_MOC_LITERAL(1, 19, 15), // "onButtonPressed"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 25), // "InputProvider::SNESButton"
QT_MOC_LITERAL(4, 62, 6), // "button"
QT_MOC_LITERAL(5, 69, 16), // "onButtonReleased"
QT_MOC_LITERAL(6, 86, 13) // "onResyncTimer"

    },
    "InputMirrorManager\0onButtonPressed\0\0"
    "InputProvider::SNESButton\0button\0"
    "onButtonReleased\0onResyncTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InputMirrorManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       5,    1,   32,    2, 0x0a /* Public */,
       6,    0,   35,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void InputMirrorManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InputMirrorManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onButtonPressed((*reinterpret_cast< InputProvider::SNESButton(*)>(_a[1]))); break;
        case 1: _t->onButtonReleased((*reinterpret_cast< InputProvider::SNESButton(*)>(_a[1]))); break;
        case 2: _t->onResyncTimer(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InputMirrorManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_InputMirrorManager.data,
    qt_meta_data_InputMirrorManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *InputMirrorManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InputMirrorManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InputMirrorManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int InputMirrorManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
