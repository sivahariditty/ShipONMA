/****************************************************************************
** Meta object code from reading C++ file 'displaywindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "displaywindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'displaywindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DisplayWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   14,   14,   14, 0x0a,
      68,   14,   14,   14, 0x0a,
      84,   14,   14,   14, 0x0a,
     106,   14,   14,   14, 0x0a,
     128,   14,   14,   14, 0x0a,
     148,   14,   14,   14, 0x0a,
     173,   14,   14,   14, 0x0a,
     199,   14,   14,   14, 0x0a,
     223,   14,   14,   14, 0x0a,
     250,   14,   14,   14, 0x0a,
     272,   14,   14,   14, 0x0a,
     284,   14,   14,   14, 0x0a,
     309,  305,   14,   14, 0x0a,
     330,   14,   14,   14, 0x0a,
     342,   14,   14,   14, 0x0a,
     360,   14,   14,   14, 0x0a,
     378,   14,   14,   14, 0x0a,
     402,   14,   14,   14, 0x0a,
     426,   14,   14,   14, 0x0a,
     449,  305,   14,   14, 0x0a,
     470,  305,   14,   14, 0x0a,
     494,   14,   14,   14, 0x0a,
     512,   14,   14,   14, 0x0a,
     552,   14,   14,   14, 0x0a,
     578,   14,   14,   14, 0x0a,
     609,   14,   14,   14, 0x0a,
     632,   14,   14,   14, 0x0a,
     651,   14,   14,   14, 0x0a,
     668,   14,   14,   14, 0x0a,
     685,   14,   14,   14, 0x0a,
     704,   14,   14,   14, 0x0a,
     723,   14,   14,   14, 0x0a,
     733,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DisplayWindow[] = {
    "DisplayWindow\0\0SignalMouseLeftButtonPress()\0"
    "PageSelectFunction(int)\0ReplayTime(int)\0"
    "SclaeAdujustment(int)\0HydrophoneSelect(int)\0"
    "ReadEnergyFile(int)\0AccelerometerSelect(int)\0"
    "SensorGroupSelection(int)\0"
    "ZoomLofarCursorSet(int)\0"
    "ZoomSpectrumCursorSet(int)\0"
    "SaveHistogramEnergy()\0closePage()\0"
    "SetTimeConstant(int)\0arg\0Mgc_SetCmbo(int16_t)\0"
    "checkGain()\0RecordEnableFun()\0"
    "ReplayEnableFun()\0EnergyRecordEnableFun()\0"
    "EnergyRecordReplayFun()\0TrackDataInfo(int32_t)\0"
    "Agc_SetCmbo(int16_t)\0Preset_SetCmbo(int16_t)\0"
    "Gain_Set(int16_t)\0"
    "on_Formatcombobox_currentIndexChanged()\0"
    "Export_to_Format_Button()\0"
    "ExportDelayedSpectrum_to_CSV()\0"
    "ExportRawData_to_CSV()\0setReplayCntFlag()\0"
    "Xmax_dialogmsg()\0Ymax_dialogmsg()\0"
    "Zoom_XmaxAlertOK()\0Zoom_YmaxAlertOK()\0"
    "ZoomSet()\0ResetSet()\0"
};

void DisplayWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DisplayWindow *_t = static_cast<DisplayWindow *>(_o);
        switch (_id) {
        case 0: _t->SignalMouseLeftButtonPress(); break;
        case 1: _t->PageSelectFunction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->ReplayTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->SclaeAdujustment((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->HydrophoneSelect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->ReadEnergyFile((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->AccelerometerSelect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->SensorGroupSelection((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->ZoomLofarCursorSet((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->ZoomSpectrumCursorSet((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->SaveHistogramEnergy(); break;
        case 11: _t->closePage(); break;
        case 12: _t->SetTimeConstant((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->Mgc_SetCmbo((*reinterpret_cast< int16_t(*)>(_a[1]))); break;
        case 14: _t->checkGain(); break;
        case 15: _t->RecordEnableFun(); break;
        case 16: _t->ReplayEnableFun(); break;
        case 17: _t->EnergyRecordEnableFun(); break;
        case 18: _t->EnergyRecordReplayFun(); break;
        case 19: _t->TrackDataInfo((*reinterpret_cast< int32_t(*)>(_a[1]))); break;
        case 20: _t->Agc_SetCmbo((*reinterpret_cast< int16_t(*)>(_a[1]))); break;
        case 21: _t->Preset_SetCmbo((*reinterpret_cast< int16_t(*)>(_a[1]))); break;
        case 22: _t->Gain_Set((*reinterpret_cast< int16_t(*)>(_a[1]))); break;
        case 23: _t->on_Formatcombobox_currentIndexChanged(); break;
        case 24: _t->Export_to_Format_Button(); break;
        case 25: _t->ExportDelayedSpectrum_to_CSV(); break;
        case 26: _t->ExportRawData_to_CSV(); break;
        case 27: _t->setReplayCntFlag(); break;
        case 28: _t->Xmax_dialogmsg(); break;
        case 29: _t->Ymax_dialogmsg(); break;
        case 30: _t->Zoom_XmaxAlertOK(); break;
        case 31: _t->Zoom_YmaxAlertOK(); break;
        case 32: _t->ZoomSet(); break;
        case 33: _t->ResetSet(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DisplayWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DisplayWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DisplayWindow,
      qt_meta_data_DisplayWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DisplayWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DisplayWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DisplayWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DisplayWindow))
        return static_cast<void*>(const_cast< DisplayWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DisplayWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    return _id;
}

// SIGNAL 0
void DisplayWindow::SignalMouseLeftButtonPress()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
