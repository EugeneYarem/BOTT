/****************************************************************************
** Meta object code from reading C++ file 'hospitalmenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../Projects/Battle-of-the-Towns/BOTT/gameMenus/hospitalmenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hospitalmenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_HospitalMenu_t {
    QByteArrayData data[4];
    char stringdata[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HospitalMenu_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HospitalMenu_t qt_meta_stringdata_HospitalMenu = {
    {
QT_MOC_LITERAL(0, 0, 12), // "HospitalMenu"
QT_MOC_LITERAL(1, 13, 17), // "H_EnterQuarantine"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16) // "H_DoctorsCountUp"

    },
    "HospitalMenu\0H_EnterQuarantine\0\0"
    "H_DoctorsCountUp"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HospitalMenu[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    0,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HospitalMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HospitalMenu *_t = static_cast<HospitalMenu *>(_o);
        switch (_id) {
        case 0: _t->H_EnterQuarantine(); break;
        case 1: _t->H_DoctorsCountUp(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HospitalMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HospitalMenu::H_EnterQuarantine)) {
                *result = 0;
            }
        }
        {
            typedef void (HospitalMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HospitalMenu::H_DoctorsCountUp)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject HospitalMenu::staticMetaObject = {
    { &GameMenu::staticMetaObject, qt_meta_stringdata_HospitalMenu.data,
      qt_meta_data_HospitalMenu,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HospitalMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HospitalMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HospitalMenu.stringdata))
        return static_cast<void*>(const_cast< HospitalMenu*>(this));
    return GameMenu::qt_metacast(_clname);
}

int HospitalMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GameMenu::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void HospitalMenu::H_EnterQuarantine()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void HospitalMenu::H_DoctorsCountUp()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE