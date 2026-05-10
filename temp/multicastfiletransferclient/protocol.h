#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>

#pragma pack(push,1)
struct PacketHeader
{
    quint8  type;
    quint32 fileId;
    quint32 seq;
    quint32 total;
    quint32 size;
    quint32 crc;
    quint32 targetCount;
    quint32 serverIp;
    quint8  overwrite;

    quint32 pathSize;

    // 🔥 yeni metadata alanları
    quint32 altNameSize;
    quint32 userNameSize;
    quint32 fileTypeSize;
};
#pragma pack(pop)

enum PacketType
{
    META = 1,
    DATA = 2,
    END  = 3,
    ACK  = 4,
    DONE = 5
};

#endif
