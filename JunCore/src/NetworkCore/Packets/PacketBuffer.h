#pragma once
#include <memory>
#include <mutex>
#include <Windows.h>
#include <winnt.h>
#include <exception>

#define MAX_PAYLOAD_LEN 8000

using BYTE = unsigned char;

#pragma pack(push, 1)
struct PacketHeader {
	BYTE code;
	WORD len;
	BYTE randKey;
	BYTE checkSum;
};
#pragma pack(pop)

#define HEADER_SIZE sizeof(PacketHeader)

struct PacketException : public std::exception {
public:
	PacketException(const char* error_code, bool type) : exception(error_code), errorType(type) {}
	PacketException(bool type) : errorType(type) {}

public:
	bool errorType;
#define GET_ERROR 0 // >> Error
#define PUT_ERROR 1 // << Error
};

class PacketBuffer {
public:
	PacketBuffer();
	~PacketBuffer();

private:
	// 할당 직렬화 버퍼
	char* begin_;
	char* end_;
	int size_;

	// 외부 read/write (직렬화, 역직렬화)
	char* payload_pos_;
	char* write_pos_;

	// 관리
	bool encrypted_;

private:
	// 패킷 복호화
	bool DecryptPacket(char* encryptPacket, BYTE privateKey);

	// 체크섬 반환
	BYTE GetChecksum();

public:
	// 패킷 초기화
	void Initialization();

	// 헤더 셋팅
	void SetHeader();

	// Empty & Full
	bool Empty() const;
	bool Full() const;

	// Getter
	char* GetPacketPos() const;
	char* GetWritePos() const;
	int GetFreeSize()const;
	int GetPacketSize() const;
	int GetPayloadSize() const;

	// move pos
	void MoveWp(int size) { write_pos_ += size; }
	void MoveRp(int size) { payload_pos_ += size; }

	// 직렬화
	PacketBuffer& operator <<(const char& data);
	PacketBuffer& operator <<(const unsigned char& data);
	PacketBuffer& operator <<(const int& data);
	PacketBuffer& operator <<(const unsigned int& data);
	PacketBuffer& operator <<(const long& data);
	PacketBuffer& operator <<(const unsigned long& data);
	PacketBuffer& operator <<(const short& data);
	PacketBuffer& operator <<(const unsigned short& data);
	PacketBuffer& operator <<(const float& data);
	PacketBuffer& operator <<(const double& data);
	PacketBuffer& operator <<(const long long& data);
	PacketBuffer& operator <<(const unsigned long long& data);
	void PutData(const char* src, int size);

	// 역직렬화
	PacketBuffer& operator >>(char& data);
	PacketBuffer& operator >>(unsigned char& data);
	PacketBuffer& operator >>(int& data);
	PacketBuffer& operator >>(unsigned int& data);
	PacketBuffer& operator >>(long& data);
	PacketBuffer& operator >>(unsigned long& data);
	PacketBuffer& operator >>(short& data);
	PacketBuffer& operator >>(unsigned short& data);
	PacketBuffer& operator >>(float& data);
	PacketBuffer& operator >>(double& data);
	PacketBuffer& operator >>(long long& data);
	PacketBuffer& operator >>(unsigned long long& data);
	void GetData(char* dst, int size);
};
using PacketBufferPtr = std::shared_ptr<PacketBuffer>;