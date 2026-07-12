/****************************************************************************
** Meta object code from reading C++ file 'mirrortargetsdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../mirrortargetsdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mirrortargetsdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MirrorTargetsDialog_t {
    QByteArrayData data[10];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MirrorTargetsDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MirrorTargetsDialog_t qt_meta_stringdata_MirrorTargetsDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "MirrorTargetsDialog"
QT_MOC_LITERAL(1, 20, 20), // "on_addButton_clicked"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 23), // "on_removeButton_clicked"
QT_MOC_LITERAL(4, 66, 22), // "on_clearButton_clicked"
QT_MOC_LITERAL(5, 89, 21), // "on_buttonBox_accepted"
QT_MOC_LITERAL(6, 111, 26), // "on_browseBinButton_clicked"
QT_MOC_LITERAL(7, 138, 31), // "on_browseFullTextButton_clicked"
QT_MOC_LITERAL(8, 170, 31), // "on_browseSyncTextButton_clicked"
QT_MOC_LITERAL(9, 202, 28) // "on_browseLastNButton_clicked"

    },
    "MirrorTargetsDialog\0on_addButton_clicked\0"
    "\0on_removeButton_clicked\0"
    "on_clearButton_clicked\0on_buttonBox_accepted\0"
    "on_browseBinButton_clicked\0"
    "on_browseFullTextButton_clicked\0"
    "on_browseSyncTextButton_clicked\0"
    "on_browseLastNButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MirrorTargetsDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MirrorTargetsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MirrorTargetsDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_addButton_clicked(); break;
        case 1: _t->on_removeButton_clicked(); break;
        case 2: _t->on_clearButton_clicked(); break;
        case 3: _t->on_buttonBox_accepted(); break;
        case 4: _t->on_browseBinButton_clicked(); break;
        case 5: _t->on_browseFullTextButton_clicked(); break;
        case 6: _t->on_browseSyncTextButton_clicked(); break;
        case 7: _t->on_browseLastNButton_clicked(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject MirrorTargetsDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_MirrorTargetsDialog.data,
    qt_meta_data_MirrorTargetsDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MirrorTargetsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MirrorTargetsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MirrorTargetsDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MirrorTargetsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
