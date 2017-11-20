/****************************************************************************
** Meta object code from reading C++ file 'sciencemenu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../Projects/Battle-of-the-Towns/BOTT/gameMenus/sciencemenu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sciencemenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ScienceMenu_t {
    QByteArrayData data[7];
    char stringdata[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScienceMenu_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScienceMenu_t qt_meta_stringdata_ScienceMenu = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ScienceMenu"
QT_MOC_LITERAL(1, 12, 21), // "SC_WaterWheelExplored"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 23), // "SC_BlastFurnaceExplored"
QT_MOC_LITERAL(4, 59, 20), // "SC_ArtilleryExplored"
QT_MOC_LITERAL(5, 80, 19), // "SC_ArquebusExplored"
QT_MOC_LITERAL(6, 100, 24) // "SC_SpinningWheelExplored"

    },
    "ScienceMenu\0SC_WaterWheelExplored\0\0"
    "SC_BlastFurnaceExplored\0SC_ArtilleryExplored\0"
    "SC_ArquebusExplored\0SC_SpinningWheelExplored"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScienceMenu[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,
       5,    0,   42,    2, 0x06 /* Public */,
       6,    0,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ScienceMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ScienceMenu *_t = static_cast<ScienceMenu *>(_o);
        switch (_id) {
        case 0: _t->SC_WaterWheelExplored(); break;
        case 1: _t->SC_BlastFurnaceExplored(); break;
        case 2: _t->SC_ArtilleryExplored(); break;
        case 3: _t->SC_ArquebusExplored(); break;
        case 4: _t->SC_SpinningWheelExplored(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ScienceMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScienceMenu::SC_WaterWheelExplored)) {
                *result = 0;
            }
        }
        {
            typedef void (ScienceMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScienceMenu::SC_BlastFurnaceExplored)) {
                *result = 1;
            }
        }
        {
            typedef void (ScienceMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScienceMenu::SC_ArtilleryExplored)) {
                *result = 2;
            }
        }
        {
            typedef void (ScienceMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScienceMenu::SC_ArquebusExplored)) {
                *result = 3;
            }
        }
        {
            typedef void (ScienceMenu::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ScienceMenu::SC_SpinningWheelExplored)) {
                *result = 4;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ScienceMenu::staticMetaObject = {
    { &GameMenu::staticMetaObject, qt_meta_stringdata_ScienceMenu.data,
      qt_meta_data_ScienceMenu,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ScienceMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScienceMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ScienceMenu.stringdata))
        return static_cast<void*>(const_cast< ScienceMenu*>(this));
    return GameMenu::qt_metacast(_clname);
}

int ScienceMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GameMenu::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ScienceMenu::SC_WaterWheelExplored()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void ScienceMenu::SC_BlastFurnaceExplored()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void ScienceMenu::SC_ArtilleryExplored()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void ScienceMenu::SC_ArquebusExplored()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void ScienceMenu::SC_SpinningWheelExplored()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
