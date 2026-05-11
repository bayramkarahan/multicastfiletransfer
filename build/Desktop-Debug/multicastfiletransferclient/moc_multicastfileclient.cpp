/****************************************************************************
** Meta object code from reading C++ file 'multicastfileclient.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../multicastfiletransferclient/multicastfileclient.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multicastfileclient.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_MulticastClient_t {
    uint offsetsAndSizes[58];
    char stringdata0[16];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[9];
    char stringdata5[9];
    char stringdata6[11];
    char stringdata7[13];
    char stringdata8[13];
    char stringdata9[18];
    char stringdata10[17];
    char stringdata11[8];
    char stringdata12[4];
    char stringdata13[4];
    char stringdata14[16];
    char stringdata15[9];
    char stringdata16[13];
    char stringdata17[7];
    char stringdata18[9];
    char stringdata19[15];
    char stringdata20[8];
    char stringdata21[9];
    char stringdata22[11];
    char stringdata23[9];
    char stringdata24[17];
    char stringdata25[4];
    char stringdata26[9];
    char stringdata27[13];
    char stringdata28[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_MulticastClient_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_MulticastClient_t qt_meta_stringdata_MulticastClient = {
    {
        QT_MOC_LITERAL(0, 15),  // "MulticastClient"
        QT_MOC_LITERAL(16, 12),  // "fileReceived"
        QT_MOC_LITERAL(29, 0),  // ""
        QT_MOC_LITERAL(30, 7),  // "tmpPath"
        QT_MOC_LITERAL(38, 8),  // "destPath"
        QT_MOC_LITERAL(47, 8),  // "fileName"
        QT_MOC_LITERAL(56, 10),  // "transferId"
        QT_MOC_LITERAL(67, 12),  // "TransferType"
        QT_MOC_LITERAL(80, 12),  // "transferType"
        QT_MOC_LITERAL(93, 17),  // "transferOverwrite"
        QT_MOC_LITERAL(111, 16),  // "transferProgress"
        QT_MOC_LITERAL(128, 7),  // "percent"
        QT_MOC_LITERAL(136, 3),  // "log"
        QT_MOC_LITERAL(140, 3),  // "msg"
        QT_MOC_LITERAL(144, 15),  // "processDatagram"
        QT_MOC_LITERAL(160, 8),  // "datagram"
        QT_MOC_LITERAL(169, 12),  // "QHostAddress"
        QT_MOC_LITERAL(182, 6),  // "sender"
        QT_MOC_LITERAL(189, 8),  // "sendNack"
        QT_MOC_LITERAL(198, 14),  // "QList<quint32>"
        QT_MOC_LITERAL(213, 7),  // "missing"
        QT_MOC_LITERAL(221, 8),  // "saveFile"
        QT_MOC_LITERAL(230, 10),  // "resetState"
        QT_MOC_LITERAL(241, 8),  // "sendDone"
        QT_MOC_LITERAL(250, 16),  // "generateFileName"
        QT_MOC_LITERAL(267, 3),  // "dir"
        QT_MOC_LITERAL(271, 8),  // "baseName"
        QT_MOC_LITERAL(280, 12),  // "sendProgress"
        QT_MOC_LITERAL(293, 10)   // "getLocalIp"
    },
    "MulticastClient",
    "fileReceived",
    "",
    "tmpPath",
    "destPath",
    "fileName",
    "transferId",
    "TransferType",
    "transferType",
    "transferOverwrite",
    "transferProgress",
    "percent",
    "log",
    "msg",
    "processDatagram",
    "datagram",
    "QHostAddress",
    "sender",
    "sendNack",
    "QList<quint32>",
    "missing",
    "saveFile",
    "resetState",
    "sendDone",
    "generateFileName",
    "dir",
    "baseName",
    "sendProgress",
    "getLocalIp"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_MulticastClient[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    6,   80,    2, 0x06,    1 /* Public */,
      10,    1,   93,    2, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    1,   96,    2, 0x08,   10 /* Private */,
      14,    2,   99,    2, 0x08,   12 /* Private */,
      18,    2,  104,    2, 0x08,   15 /* Private */,
      21,    0,  109,    2, 0x08,   18 /* Private */,
      22,    0,  110,    2, 0x08,   19 /* Private */,
      23,    0,  111,    2, 0x08,   20 /* Private */,
      24,    2,  112,    2, 0x08,   21 /* Private */,
      27,    1,  117,    2, 0x08,   24 /* Private */,
      28,    0,  120,    2, 0x08,   26 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, 0x80000000 | 7, QMetaType::Bool,    3,    4,    5,    6,    8,    9,
    QMetaType::Void, QMetaType::Int,   11,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QByteArray, 0x80000000 | 16,   15,   17,
    QMetaType::Void, 0x80000000 | 19, 0x80000000 | 16,   20,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::QString, QMetaType::QString,   25,   26,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::QString,

       0        // eod
};

Q_CONSTINIT const QMetaObject MulticastClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MulticastClient.offsetsAndSizes,
    qt_meta_data_MulticastClient,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_MulticastClient_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MulticastClient, std::true_type>,
        // method 'fileReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<TransferType, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'transferProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'log'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'processDatagram'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QByteArray &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QHostAddress &, std::false_type>,
        // method 'sendNack'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVector<quint32> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QHostAddress &, std::false_type>,
        // method 'saveFile'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendDone'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'generateFileName'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendProgress'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'getLocalIp'
        QtPrivate::TypeAndForceComplete<QString, std::false_type>
    >,
    nullptr
} };

void MulticastClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MulticastClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->fileReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<TransferType>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[6]))); break;
        case 1: _t->transferProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->log((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->processDatagram((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 4: _t->sendNack((*reinterpret_cast< std::add_pointer_t<QList<quint32>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QHostAddress>>(_a[2]))); break;
        case 5: _t->saveFile(); break;
        case 6: _t->resetState(); break;
        case 7: _t->sendDone(); break;
        case 8: { QString _r = _t->generateFileName((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 9: _t->sendProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 10: { QString _r = _t->getLocalIp();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<quint32> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MulticastClient::*)(QString , QString , QString , QString , TransferType , bool );
            if (_t _q_method = &MulticastClient::fileReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MulticastClient::*)(int );
            if (_t _q_method = &MulticastClient::transferProgress; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *MulticastClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MulticastClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MulticastClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MulticastClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void MulticastClient::fileReceived(QString _t1, QString _t2, QString _t3, QString _t4, TransferType _t5, bool _t6)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MulticastClient::transferProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
