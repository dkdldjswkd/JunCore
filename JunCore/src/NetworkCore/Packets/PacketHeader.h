#ifndef _PACKET_HEADER_H_
#define _PACKET_HEADER_H_

#pragma pack(push, 1)
struct PacketHeader {
	BYTE code;
	WORD len;
	BYTE randKey;
	BYTE checkSum;
};
#pragma pack(pop)

#define NET_HEADER_SIZE  ((WORD)sizeof(PacketHeader))

#endif