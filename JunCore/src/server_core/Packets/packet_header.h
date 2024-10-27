#ifndef _PACKET_HEADER_H_
#define _PACKET_HEADER_H_

#pragma pack(push, 1)
//struct PacketHeader
//{
//	uint16_t packetId;      // 패킷 ID (패킷 종류 식별자)
//	uint32_t packetLength;   // 패킷 길이 (데이터의 길이)
//	uint16_t checksum;       // 간단한 무결성 체크를 위한 체크섬
//
//	// 기본 생성자
//	PacketHeader() : packetId(0), packetLength(0), checksum(0) {}
//
//	// 패킷 헤더를 생성하는 생성자
//	PacketHeader(uint16_t id, uint32_t length) : packetId(id), packetLength(length), checksum(0)
//	{
//		checksum = CalculateChecksum();
//	}
//
//	// 간단한 체크섬 계산 함수
//	uint16_t CalculateChecksum() const
//	{
//		uint16_t sum = packetId;
//		sum ^= static_cast<uint16_t>(packetLength & 0xFFFF);
//		sum ^= static_cast<uint16_t>((packetLength >> 16) & 0xFFFF);
//		return sum;
//	}
//
//	// 유효성 검사 함수
//	bool IsValid() const
//	{
//		return CalculateChecksum() == checksum;
//	}
//
//	// 패킷 헤더를 직렬화하여 바이트 배열에 복사
//	void Serialize(uint8_t* buffer) const
//	{
//		std::memcpy(buffer, this, sizeof(PacketHeader));
//	}
//
//	// 바이트 배열에서 패킷 헤더를 역직렬화
//	void Deserialize(const uint8_t* buffer)
//	{
//		std::memcpy(this, buffer, sizeof(PacketHeader));
//	}
//};
#pragma pack(pop)

#pragma pack(push, 1)
struct LanHeader {
	WORD len;
};

struct NetHeader {
	BYTE code;
	WORD len;
	BYTE randKey;
	BYTE checkSum;
};
#pragma pack(pop)

#define LAN_HEADER_SIZE  ((WORD)sizeof(LanHeader))
#define NET_HEADER_SIZE  ((WORD)sizeof(NetHeader))

#endif