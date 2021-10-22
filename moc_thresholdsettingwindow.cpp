/****************************************************************************
** Meta object code from reading C++ file 'thresholdsettingwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "thresholdsettingwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'thresholdsettingwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ThresholdSettingWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   23,   23,   23, 0x0a,
      45,   23,   23,   23, 0x0a,
      69,   23,   23,   23, 0x0a,
      90,   23,   23,   23, 0x0a,
     111,   23,   23,   23, 0x0a,
     132,   23,   23,   23, 0x0a,
     153,   23,   23,   23, 0x0a,
     174,   23,   23,   23, 0x0a,
     195,   23,   23,   23, 0x0a,
     216,   23,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ThresholdSettingWindow[] = {
    "ThresholdSettingWindow\0\0setThresholdConfig()\0"
    "cancelThresholdConfig()\0checkBox1Enable(int)\0"
    "checkBox2Enable(int)\0checkBox3Enable(int)\0"
    "checkBox4Enable(int)\0checkBox5Enable(int)\0"
    "checkBox6Enable(int)\0checkBox7Enable(int)\0"
    "checkBox8Enable(int)\0"
};

void ThresholdSettingWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ThresholdSettingWindow *_t = static_cast<ThresholdSettingWindow *>(_o);
        switch (_id) {
        case 0: _t->setThresholdConfig(); break;
        case 1: _t->cancelThresholdConfig(); break;
        case 2: _t->checkBox1Enable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->checkBox2Enable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->checkBox3Enable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->checkBox4Enable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->checkBox5Enable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->checkBox6Enable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->checkBox7Enable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->checkBox8Enable((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ThresholdSettingWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ThresholdSettingWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ThresholdSettingWindow,
      qt_meta_data_ThresholdSettingWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ThresholdSettingWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ThresholdSettingWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ThresholdSettingWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ThresholdSettingWindow))
        return static_cast<void*>(const_cast< ThresholdSettingWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int ThresholdSettingWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
