#pragma once
#include <memory>
#include <mutex>
#include <Windows.h>
#include <winnt.h>
#include <exception>
#include "PacketHeader.h"

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
	inline ~PacketBuffer();

private:
	// 할당 직렬화 버퍼
	char* begin_;
	char* end_;
	const int size_;

	// 외부 read/write (직렬화, 역직렬화)
	char* payload_pos_;
	char* write_pos_;

	// 관리
	const int header_len_;
	bool encrypted_;

private:
	// 패킷 초기화
	void Initialization();

	// 헤더 셋팅
	void SetNetHeader(BYTE protocolCode, BYTE privateKey);

	// 패킷 복호화
	bool DecryptPacket(char* encryptPacket, BYTE privateKey);

	// 헤더 포함 패킷 시작 주소 반환 (payLoadPos - headerLen)
	char* GetPacketPos();

	// 패킷 사이즈 반환 (패킷 헤더 사이즈 포함)
	inline int GetPacketSize();

	// 체크섬 반환
	BYTE GetChecksum();

public:
	// 할당, 반환
	static PacketBuffer* Alloc();
	static void Free(PacketBuffer* instance);
	static void Free(PacketBuffer* instance, bool* isReleased);

	// Empty & Full
	inline bool Empty() const;
	inline bool Full() const;

	// Getter
	inline int GetFreeSize()const;
	inline int GetPayloadSize() const;

	// move pos
	inline void MoveWp(int size) { write_pos_ += size; }
	inline void MoveRp(int size) { payload_pos_ += size; }

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


PacketBuffer::PacketBuffer()
	: header_len_(sizeof(PacketHeader)), size_(header_len_ + MAX_PAYLOAD_LEN)
{
	begin_	= (char*)malloc(size_);
	end_	= begin_ + size_;

	payload_pos_ = begin_ + header_len_;
	write_pos_	 = begin_ + header_len_;
}


inline PacketBuffer::~PacketBuffer() {
	free((char*)begin_);
}


void PacketBuffer::Initialization() {
	payload_pos_ = begin_ + header_len_;
	write_pos_ = begin_ + header_len_;
	encrypted_ = false;
}


inline int PacketBuffer::GetPacketSize() {
	return (write_pos_ - payload_pos_) + header_len_;
}


inline char* PacketBuffer::GetPacketPos() {
	return (payload_pos_ - header_len_);
}


inline bool PacketBuffer::Empty() const {
	if (write_pos_ <= payload_pos_) return true;
	return false;
}


bool PacketBuffer::Full() const {
	if (write_pos_ + 1 == end_) return true;
	return false;
}


inline int PacketBuffer::GetFreeSize() const {
	return end_ - write_pos_;
}


inline int PacketBuffer::GetPayloadSize() const {
	return write_pos_ - payload_pos_;
}


void PacketBuffer::SetNetHeader(BYTE protocol_code, BYTE private_key) 
{
	// 중복 암호화 하지 않기 위함 (이미 암호화 된 패킷)
	if (encrypted_) return;
	encrypted_ = true;

	PacketHeader netHeader;
	netHeader.code = protocol_code;
	netHeader.len = GetPayloadSize();
	netHeader.randKey = (rand() & 0xFF);
	netHeader.checkSum = GetChecksum();
	memmove(GetPacketPos(), &netHeader, header_len_);

	char* encryptPos = payload_pos_ - 1;	// 암호화'될' 주소
	short encrypt_len = netHeader.len + 1;	// 암호화될 길이
	BYTE RK = netHeader.randKey;			// 랜덤 키
	BYTE K = private_key;					// 고정 키
	BYTE P = 0, E = 0;						// 암호화 변수

	// 암호화
	for (int i = 0; i < encrypt_len; i++, encryptPos++) {
		P = (*encryptPos) ^ (P + RK + (i + 1));
		E = P ^ (E + K + (i + 1));
		*((BYTE*)encryptPos) = E;
	}
}

// 암호패킷을 this에 복호화

bool PacketBuffer::DecryptPacket(char* encryptPacket, BYTE privateKey) {
	// 복호화 변수
	const BYTE RK = ((PacketHeader*)encryptPacket)->randKey;
	const BYTE K = privateKey;
	BYTE P = 0, LP = 0, LE = 0;

	// 복호화 준비
	char* const packetPos = GetPacketPos();						// this 패킷 주소
	char* decryptPos = packetPos + (header_len_ - 1);			// this 복호화'될' 주소 (복호화 부 : checksum + payload)
	char* encryptPos = encryptPacket + (header_len_ - 1);		// 암호화 부
	const short encryptLen = ((PacketHeader*)encryptPacket)->len + 1;	// 암호화 부 길이 (checksum + payload)
	memmove(packetPos, encryptPacket, header_len_ - 1);			// 암호화 되어있지 않은 부분 복사

	// 복호화
	for (int i = 0; i < encryptLen; ++i) { // 복호화
		// BYTE 단위 복호화
		P = (*encryptPos) ^ (LE + K + (i + 1));
		*((BYTE*)decryptPos) = P ^ (LP + RK + (i + 1));

		// 다음 루프 준비
		LE = *encryptPos;
		LP = P;
		encryptPos++;
		decryptPos++;
	}
	MoveWp(encryptLen - 1); // 복호화 중 복사된 페이로드 부 만큼 move

	// 암호패킷의 신뢰성 결과 반환
	if (((PacketHeader*)packetPos)->checkSum == GetChecksum()) {
		return true;
	}
	return false;
}


BYTE PacketBuffer::GetChecksum() {
	WORD len = GetPayloadSize();

	DWORD checkSum = 0;
	char* cpy_readPos = payload_pos_;
	for (int i = 0; i < len; i++) {
		checkSum += *cpy_readPos;
		cpy_readPos++;
	}
	return (BYTE)(checkSum & 0xFF);
}

///////////////////////////////
//	operator <<
///////////////////////////////


PacketBuffer& PacketBuffer::operator<<(const char& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const unsigned char& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const int& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const unsigned int& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const long& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const unsigned long& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const short& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const unsigned short& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const float& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const double& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const long long& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator<<(const unsigned long long& data) {
	if (write_pos_ + sizeof(data) <= end_) {
		memmove(write_pos_, &data, sizeof(data));
		write_pos_ += sizeof(data);
	}
	else {
		throw PacketException(PUT_ERROR);
	}
	return *this;
}

///////////////////////////////
//	operator >>
///////////////////////////////


PacketBuffer& PacketBuffer::operator>>(char& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(unsigned char& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(int& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(unsigned int& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(long& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(unsigned long& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(short& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(unsigned short& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(float& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}

	return *this;
}


PacketBuffer& PacketBuffer::operator>>(double& data) {
	if (payload_pos_ + sizeof(data) <= `) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(long long& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}


PacketBuffer& PacketBuffer::operator>>(unsigned long long& data) {
	if (payload_pos_ + sizeof(data) <= write_pos_) {
		memmove(&data, payload_pos_, sizeof(data));
		payload_pos_ += sizeof(data);
	}
	else {
		throw PacketException(GET_ERROR);
	}
	return *this;
}

///////////////////////////////
//	PUT, GET
///////////////////////////////


void PacketBuffer::PutData(const char* src, int size) {
	if (write_pos_ + size <= end_) {
		memmove(write_pos_, src, size);
		write_pos_ += size;
	}
	else {
		throw PacketException(PUT_ERROR);
	}
}


void PacketBuffer::GetData(char* dst, int size) {
	if (payload_pos_ + size <= write_pos_) {
		memmove(dst, payload_pos_, size);
		payload_pos_ += size;
	}
	else {
		throw PacketException(GET_ERROR);
	}
}