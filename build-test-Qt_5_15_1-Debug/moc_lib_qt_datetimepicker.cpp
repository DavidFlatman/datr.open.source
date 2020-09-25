/****************************************************************************
** Meta object code from reading C++ file 'lib_qt_datetimepicker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../qt/lib_qt_datetimepicker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lib_qt_datetimepicker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_lib__qt__DateTimePicker_t {
    QByteArrayData data[9];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_lib__qt__DateTimePicker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_lib__qt__DateTimePicker_t qt_meta_stringdata_lib__qt__DateTimePicker = {
    {
QT_MOC_LITERAL(0, 0, 23), // "lib::qt::DateTimePicker"
QT_MOC_LITERAL(1, 24, 13), // "acceptChanges"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 18), // "todayButtonPressed"
QT_MOC_LITERAL(4, 58, 16), // "nowButtonPressed"
QT_MOC_LITERAL(5, 75, 15), // "okButtonPressed"
QT_MOC_LITERAL(6, 91, 12), // "onDateChange"
QT_MOC_LITERAL(7, 104, 12), // "onTimeChange"
QT_MOC_LITERAL(8, 117, 14) // "updateLineEdit"

    },
    "lib::qt::DateTimePicker\0acceptChanges\0"
    "\0todayButtonPressed\0nowButtonPressed\0"
    "okButtonPressed\0onDateChange\0onTimeChange\0"
    "updateLineEdit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_lib__qt__DateTimePicker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    0,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void lib::qt::DateTimePicker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DateTimePicker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->acceptChanges(); break;
        case 1: _t->todayButtonPressed(); break;
        case 2: _t->nowButtonPressed(); break;
        case 3: _t->okButtonPressed(); break;
        case 4: _t->onDateChange(); break;
        case 5: _t->onTimeChange(); break;
        case 6: _t->updateLineEdit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DateTimePicker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DateTimePicker::acceptChanges)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject lib::qt::DateTimePicker::staticMetaObject = { {
    QMetaObject::SuperData::link<QGroupBox::staticMetaObject>(),
    qt_meta_stringdata_lib__qt__DateTimePicker.data,
    qt_meta_data_lib__qt__DateTimePicker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *lib::qt::DateTimePicker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *lib::qt::DateTimePicker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_lib__qt__DateTimePicker.stringdata0))
        return static_cast<void*>(this);
    return QGroupBox::qt_metacast(_clname);
}

int lib::qt::DateTimePicker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void lib::qt::DateTimePicker::acceptChanges()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
