/****************************************************************************
** Meta object code from reading C++ file 'multicastfileserver.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../multicastfiletransferserver/multicastfileserver.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multicastfileserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_MulticastServer_t {
    uint offsetsAndSizes[46];
    char stringdata0[16];
    char stringdata1[22];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[8];
    char stringdata5[22];
    char stringdata6[7];
    char stringdata7[21];
    char stringdata8[25];
    char stringdata9[24];
    char stringdata10[20];
    char stringdata11[17];
    char stringdata12[7];
    char stringdata13[9];
    char stringdata14[5];
    char stringdata15[3];
    char stringdata16[24];
    char stringdata17[19];
    char stringdata18[20];
    char stringdata19[18];
    char stringdata20[15];
    char stringdata21[8];
    char stringdata22[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MulticastServer_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MulticastServer_t qt_meta_stringdata_MulticastServer = {
    {
        QT_MOC_LITERAL(0, 15),  // "MulticastServer"
        QT_MOC_LITERAL(16, 21),  // "clientProgressChanged"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 6),  // "client"
        QT_MOC_LITERAL(46, 7),  // "percent"
        QT_MOC_LITERAL(54, 21),  // "clientDebInstallStart"
        QT_MOC_LITERAL(76, 6),  // "status"
        QT_MOC_LITERAL(83, 20),  // "clientDebInstallDone"
        QT_MOC_LITERAL(104, 24),  // "clientScriptInstallStart"
        QT_MOC_LITERAL(129, 23),  // "clientScriptInstallDone"
        QT_MOC_LITERAL(153, 19),  // "allTransferFinished"
        QT_MOC_LITERAL(173, 16),  // "transferFinished"
        QT_MOC_LITERAL(190, 6),  // "sender"
        QT_MOC_LITERAL(197, 8),  // "receiver"
        QT_MOC_LITERAL(206, 4),  // "file"
        QT_MOC_LITERAL(211, 2),  // "id"
        QT_MOC_LITERAL(214, 23),  // "processPendingDatagrams"
        QT_MOC_LITERAL(238, 18),  // "detectDefaultDelay"
        QT_MOC_LITERAL(257, 19),  // "getDefaultInterface"
        QT_MOC_LITERAL(277, 17),  // "detectNetworkType"
        QT_MOC_LITERAL(295, 14),  // "calculateBurst"
        QT_MOC_LITERAL(310, 7),  // "delayUs"
        QT_MOC_LITERAL(318, 22)   // "calculateAdaptiveBurst"
    },
    "MulticastServer",
    "clientProgressChanged",
    "",
    "client",
    "percent",
    "clientDebInstallStart",
    "status",
    "clientDebInstallDone",
    "clientScriptInstallStart",
    "clientScriptInstallDone",
    "allTransferFinished",
    "transferFinished",
    "sender",
    "receiver",
    "file",
    "id",
    "processPendingDatagrams",
    "detectDefaultDelay",
    "getDefaultInterface",
    "detectNetworkType",
    "calculateBurst",
    "delayUs",
    "calculateAdaptiveBurst"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MulticastServer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   92,    2, 0x06,    1 /* Public */,
       5,    2,   97,    2, 0x06,    4 /* Public */,
       7,    2,  102,    2, 0x06,    7 /* Public */,
       8,    2,  107,    2, 0x06,   10 /* Public */,
       9,    2,  112,    2, 0x06,   13 /* Public */,
      10,    0,  117,    2, 0x06,   16 /* Public */,
      11,    4,  118,    2, 0x06,   17 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      16,    0,  127,    2, 0x08,   22 /* Private */,
      17,    0,  128,    2, 0x08,   23 /* Private */,
      18,    0,  129,    2, 0x08,   24 /* Private */,
      19,    0,  130,    2, 0x08,   25 /* Private */,
      20,    1,  131,    2, 0x08,   26 /* Private */,
      22,    0,  134,    2, 0x08,   28 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,   12,   13,   14,   15,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::QString,
    QMetaType::QString,
    QMetaType::Int, QMetaType::Int,   21,
    QMetaType::Int,

       0        // eod
};

Q_CONSTINIT const QMetaObject MulticastServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MulticastServer.offsetsAndSizes,
    qt_meta_data_MulticastServer,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MulticastServer_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MulticastServer, std::true_type>,
        // method 'clientProgressChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'clientDebInstallStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'clientDebInstallDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'clientScriptInstallStart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'clientScriptInstallDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'allTransferFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'transferFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'processPendingDatagrams'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'detectDefaultDelay'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'getDefaultInterface'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'detectNetworkType'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'calculateBurst'
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'calculateAdaptiveBurst'
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void MulticastServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MulticastServer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clientProgressChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->clientDebInstallStart((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->clientDebInstallDone((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->clientScriptInstallStart((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->clientScriptInstallDone((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->allTransferFinished(); break;
        case 6: _t->transferFinished((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 7: _t->processPendingDatagrams(); break;
        case 8: { int _r = _t->detectDefaultDelay();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { QString _r = _t->getDefaultInterface();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 10: { QString _r = _t->detectNetworkType();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->calculateBurst((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->calculateAdaptiveBurst();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MulticastServer::*)(QString , int );
            if (_t _q_method = &MulticastServer::clientProgressChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MulticastServer::*)(QString , QString );
            if (_t _q_method = &MulticastServer::clientDebInstallStart; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MulticastServer::*)(QString , QString );
            if (_t _q_method = &MulticastServer::clientDebInstallDone; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MulticastServer::*)(QString , QString );
            if (_t _q_method = &MulticastServer::clientScriptInstallStart; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MulticastServer::*)(QString , QString );
            if (_t _q_method = &MulticastServer::clientScriptInstallDone; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MulticastServer::*)();
            if (_t _q_method = &MulticastServer::allTransferFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MulticastServer::*)(QString , QString , QString , QString );
            if (_t _q_method = &MulticastServer::transferFinished; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *MulticastServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MulticastServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MulticastServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MulticastServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MulticastServer::clientProgressChanged(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MulticastServer::clientDebInstallStart(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MulticastServer::clientDebInstallDone(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MulticastServer::clientScriptInstallStart(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MulticastServer::clientScriptInstallDone(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MulticastServer::allTransferFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MulticastServer::transferFinished(QString _t1, QString _t2, QString _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
