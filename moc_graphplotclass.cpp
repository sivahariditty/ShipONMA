/****************************************************************************
** Meta object code from reading C++ file 'graphplotclass.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "graphplotclass.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphplotclass.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GraphPlotClass[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      50,   48,   15,   15, 0x0a,
      80,   15,   15,   15, 0x0a,
     112,   15,   15,   15, 0x0a,
     133,   15,   15,   15, 0x0a,
     174,   15,   15,   15, 0x0a,
     202,   15,   15,   15, 0x0a,
     230,   15,   15,   15, 0x0a,
     265,   15,   15,   15, 0x0a,
     296,   15,   15,   15, 0x0a,
     316,   15,   15,   15, 0x0a,
     356,   15,   15,   15, 0x0a,
     379,   15,   15,   15, 0x0a,
     402,   15,   15,   15, 0x0a,
     436,   15,   15,   15, 0x0a,
     473,   15,   15,   15, 0x0a,
     494,   15,   15,   15, 0x0a,
     540,   15,   15,   15, 0x0a,
     568,   15,   15,   15, 0x0a,
     596,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GraphPlotClass[] = {
    "GraphPlotClass\0\0freezeDelSpecPlot(QMouseEvent*)\0"
    "m\0freezeLoFarPlot(QMouseEvent*)\0"
    "showSpectrumValue(QMouseEvent*)\0"
    "CloseSpectrumvalue()\0"
    "showSpectrumRightWidgetVal(QMouseEvent*)\0"
    "ExportSpectrumData_to_CSV()\0"
    "ExportSpectrumData_to_JPG()\0"
    "ButtonJPG_CSV_Close_Spectrum_Tab()\0"
    "showRawDataValue(QMouseEvent*)\0"
    "CloseRawDatavalue()\0"
    "showRawDataRightWidgetVal(QMouseEvent*)\0"
    "ExportRawData_to_CSV()\0ExportRawData_to_JPG()\0"
    "ButtonJPG_CSV_Close_RawData_Tab()\0"
    "showSpectCompDataValue(QMouseEvent*)\0"
    "CloseSpecCompvalue()\0"
    "showSpectCompDataRightWidgetVal(QMouseEvent*)\0"
    "ExportSpecCompData_to_CSV()\0"
    "ExportSpecCompData_to_JPG()\0"
    "ButtonJPG_CSV_Close_SpecComp_Tab()\0"
};

void GraphPlotClass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GraphPlotClass *_t = static_cast<GraphPlotClass *>(_o);
        switch (_id) {
        case 0: _t->freezeDelSpecPlot((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: _t->freezeLoFarPlot((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->showSpectrumValue((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->CloseSpectrumvalue(); break;
        case 4: _t->showSpectrumRightWidgetVal((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: _t->ExportSpectrumData_to_CSV(); break;
        case 6: _t->ExportSpectrumData_to_JPG(); break;
        case 7: _t->ButtonJPG_CSV_Close_Spectrum_Tab(); break;
        case 8: _t->showRawDataValue((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 9: _t->CloseRawDatavalue(); break;
        case 10: _t->showRawDataRightWidgetVal((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 11: _t->ExportRawData_to_CSV(); break;
        case 12: _t->ExportRawData_to_JPG(); break;
        case 13: _t->ButtonJPG_CSV_Close_RawData_Tab(); break;
        case 14: _t->showSpectCompDataValue((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 15: _t->CloseSpecCompvalue(); break;
        case 16: _t->showSpectCompDataRightWidgetVal((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 17: _t->ExportSpecCompData_to_CSV(); break;
        case 18: _t->ExportSpecCompData_to_JPG(); break;
        case 19: _t->ButtonJPG_CSV_Close_SpecComp_Tab(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GraphPlotClass::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GraphPlotClass::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GraphPlotClass,
      qt_meta_data_GraphPlotClass, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GraphPlotClass::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GraphPlotClass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GraphPlotClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GraphPlotClass))
        return static_cast<void*>(const_cast< GraphPlotClass*>(this));
    return QWidget::qt_metacast(_clname);
}

int GraphPlotClass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
