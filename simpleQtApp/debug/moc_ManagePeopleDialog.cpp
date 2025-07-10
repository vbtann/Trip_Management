/****************************************************************************
** Meta object code from reading C++ file 'ManagePeopleDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../UI/ManagePeopleDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ManagePeopleDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ManagePeopleDialog_t {
    QByteArrayData data[11];
    char stringdata0[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ManagePeopleDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ManagePeopleDialog_t qt_meta_stringdata_ManagePeopleDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ManagePeopleDialog"
QT_MOC_LITERAL(1, 19, 18), // "onAddPersonClicked"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 19), // "onEditPersonClicked"
QT_MOC_LITERAL(4, 59, 21), // "onDeletePersonClicked"
QT_MOC_LITERAL(5, 81, 19), // "onViewPersonClicked"
QT_MOC_LITERAL(6, 101, 24), // "onPersonSelectionChanged"
QT_MOC_LITERAL(7, 126, 16), // "onRefreshClicked"
QT_MOC_LITERAL(8, 143, 14), // "onCloseClicked"
QT_MOC_LITERAL(9, 158, 21), // "onImportPeopleClicked"
QT_MOC_LITERAL(10, 180, 21) // "onExportPeopleClicked"

    },
    "ManagePeopleDialog\0onAddPersonClicked\0"
    "\0onEditPersonClicked\0onDeletePersonClicked\0"
    "onViewPersonClicked\0onPersonSelectionChanged\0"
    "onRefreshClicked\0onCloseClicked\0"
    "onImportPeopleClicked\0onExportPeopleClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ManagePeopleDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
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

void ManagePeopleDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ManagePeopleDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAddPersonClicked(); break;
        case 1: _t->onEditPersonClicked(); break;
        case 2: _t->onDeletePersonClicked(); break;
        case 3: _t->onViewPersonClicked(); break;
        case 4: _t->onPersonSelectionChanged(); break;
        case 5: _t->onRefreshClicked(); break;
        case 6: _t->onCloseClicked(); break;
        case 7: _t->onImportPeopleClicked(); break;
        case 8: _t->onExportPeopleClicked(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject ManagePeopleDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ManagePeopleDialog.data,
    qt_meta_data_ManagePeopleDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ManagePeopleDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ManagePeopleDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ManagePeopleDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ManagePeopleDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
