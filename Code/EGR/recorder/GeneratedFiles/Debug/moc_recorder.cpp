/****************************************************************************
** Meta object code from reading C++ file 'recorder.h'
**
** Created: Mon 3. Jan 17:05:56 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../recorder.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recorder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_recorder[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      18,    9,    9,    9, 0x0a,
      32,    9,    9,    9, 0x0a,
      48,    9,    9,    9, 0x0a,
      62,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_recorder[] = {
    "recorder\0\0frame()\0saveTracker()\0"
    "recordTracker()\0showTracker()\0test()\0"
};

const QMetaObject recorder::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_recorder,
      qt_meta_data_recorder, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &recorder::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *recorder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *recorder::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_recorder))
        return static_cast<void*>(const_cast< recorder*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int recorder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: frame(); break;
        case 1: saveTracker(); break;
        case 2: recordTracker(); break;
        case 3: showTracker(); break;
        case 4: test(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
