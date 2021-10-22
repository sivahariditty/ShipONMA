/****************************************************************************
** Meta object code from reading C++ file 'sonarcommunication.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "sonarcommunication.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sonarcommunication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SonarCommunication[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x0a,
      41,   39,   35,   19, 0x0a,
      69,   19,   19,   19, 0x0a,
      86,   19,   19,   19, 0x0a,
      96,   19,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SonarCommunication[] = {
    "SonarCommunication\0\0PendingData2()\0"
    "int\0,\0swapBuffer(const char*,int)\0"
    "Sonardataready()\0Oasread()\0Quit()\0"
};

void SonarCommunication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SonarCommunication *_t = static_cast<SonarCommunication *>(_o);
        switch (_id) {
        case 0: _t->PendingData2(); break;
        case 1: { int _r = _t->swapBuffer((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: _t->Sonardataready(); break;
        case 3: _t->Oasread(); break;
        case 4: _t->Quit(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SonarCommunication::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SonarCommunication::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SonarCommunication,
      qt_meta_data_SonarCommunication, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SonarCommunication::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SonarCommunication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SonarCommunication::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SonarCommunication))
        return static_cast<void*>(const_cast< SonarCommunication*>(this));
    return QThread::qt_metacast(_clname);
}

int SonarCommunication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
