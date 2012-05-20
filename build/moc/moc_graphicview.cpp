/****************************************************************************
** Meta object code from reading C++ file 'graphicview.h'
**
** Created: Sun May 20 23:12:40 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../graphicview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphicview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DiagramView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      34,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_DiagramView[] = {
    "DiagramView\0\0convertToSimpleNet()\0"
    "convertToColouredNet()\0"
};

const QMetaObject DiagramView::staticMetaObject = {
    { &View::staticMetaObject, qt_meta_stringdata_DiagramView,
      qt_meta_data_DiagramView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DiagramView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DiagramView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DiagramView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DiagramView))
        return static_cast<void*>(const_cast< DiagramView*>(this));
    return View::qt_metacast(_clname);
}

int DiagramView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = View::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: convertToSimpleNet(); break;
        case 1: convertToColouredNet(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void DiagramView::convertToSimpleNet()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DiagramView::convertToColouredNet()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_NetworkView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      20,   12,   12,   12, 0x08,
      27,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NetworkView[] = {
    "NetworkView\0\0play()\0stop()\0timeout()\0"
};

const QMetaObject NetworkView::staticMetaObject = {
    { &View::staticMetaObject, qt_meta_stringdata_NetworkView,
      qt_meta_data_NetworkView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NetworkView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NetworkView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NetworkView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkView))
        return static_cast<void*>(const_cast< NetworkView*>(this));
    return View::qt_metacast(_clname);
}

int NetworkView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = View::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: play(); break;
        case 1: stop(); break;
        case 2: timeout(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
