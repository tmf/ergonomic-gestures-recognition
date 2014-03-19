/****************************************************************************
** Meta object code from reading C++ file 'trainer.h'
**
** Created: Wed 26. Jan 11:57:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../trainer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_trainer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      17,    8,    8,    8, 0x0a,
      26,    8,    8,    8, 0x0a,
      38,    8,    8,    8, 0x0a,
      48,    8,    8,    8, 0x0a,
      55,    8,    8,    8, 0x0a,
      63,    8,    8,    8, 0x0a,
      78,    8,    8,    8, 0x0a,
      92,    8,    8,    8, 0x0a,
     106,    8,    8,    8, 0x0a,
     122,    8,    8,    8, 0x0a,
     139,    8,    8,    8, 0x0a,
     150,    8,    8,    8, 0x0a,
     164,    8,    8,    8, 0x0a,
     183,    8,    8,    8, 0x0a,
     206,    8,    8,    8, 0x0a,
     221,    8,    8,    8, 0x0a,
     232,    8,    8,    8, 0x0a,
     240,    8,    8,    8, 0x0a,
     260,    8,    8,    8, 0x0a,
     283,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_trainer[] = {
    "trainer\0\0frame()\0addAVI()\0removeAVI()\0"
    "collect()\0live()\0pause()\0frameForward()\0"
    "saveLibrary()\0loadLibrary()\0filterSamples()\0"
    "displaySamples()\0addLabel()\0removeLabel()\0"
    "selectNextSample()\0selectPreviousSample()\0"
    "removeSample()\0estimate()\0train()\0"
    "changeSampleLabel()\0loadDecisionFunction()\0"
    "saveDecisionFunction()\0"
};

const QMetaObject trainer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_trainer,
      qt_meta_data_trainer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &trainer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *trainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *trainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_trainer))
        return static_cast<void*>(const_cast< trainer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int trainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: frame(); break;
        case 1: addAVI(); break;
        case 2: removeAVI(); break;
        case 3: collect(); break;
        case 4: live(); break;
        case 5: pause(); break;
        case 6: frameForward(); break;
        case 7: saveLibrary(); break;
        case 8: loadLibrary(); break;
        case 9: filterSamples(); break;
        case 10: displaySamples(); break;
        case 11: addLabel(); break;
        case 12: removeLabel(); break;
        case 13: selectNextSample(); break;
        case 14: selectPreviousSample(); break;
        case 15: removeSample(); break;
        case 16: estimate(); break;
        case 17: train(); break;
        case 18: changeSampleLabel(); break;
        case 19: loadDecisionFunction(); break;
        case 20: saveDecisionFunction(); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
