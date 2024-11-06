#pragma once
#include "PacketBuffer.h"

PacketBuffer::PacketBuffer()
	: size_(HEADER_SIZE + MAX_PAYLOAD_LEN), encrypted_(false)
{
	begin_	= (char*)malloc(size_);
	end_	= begin_ + size_;

	payload_pos_ = begin_ + HEADER_SIZE;
	write_pos_	 = begin_ + HEADER_SIZE;
}

PacketBuffer::~PacketBuffer()
{
	free((char*)begin_);
}

void PacketBuffer::Initialization()
{
	payload_pos_ = begin_ + HEADER_SIZE;
	write_pos_ = begin_ + HEADER_SIZE;
	encrypted_ = false;
}

char* PacketBuffer::GetPacketPos() const 
{
	return begin_;
}

bool PacketBuffer::Empty() const {
	if (write_pos_ <= payload_pos_) return true;
	return false;
}


bool PacketBuffer::Full() const {
	if (write_pos_ + 1 == end_) return true;
	return false;
}


int PacketBuffer::GetFreeSize() const {
	return end_ - write_pos_;
}

int PacketBuffer::GetPacketSize() const {
	return write_pos_ - begin_;
}

int PacketBuffer::GetPayloadSize() const {
	return write_pos_ - payload_pos_;
}

char* PacketBuffer::GetWritePos() const
{
	return write_pos_;
}

// todo : 작업 중!!
void PacketBuffer::SetHeader()
{
	// 중복 암호화 하지 않기 위함 (이미 암호화 된 패킷)
	if (encrypted_) return;
	encrypted_ = true;

	PacketHeader *_header = (PacketHeader*)GetPacketPos();
	//netHeader.code = protocol_code;
	_header->len = GetPayloadSize();
	//netHeader->randKey = (rand() & 0xFF);
	_header->checkSum = GetChecksum();
	//memmove(GetPacketPos(), &netHeader, HEADER_SIZE); // 

	// todo : 암호화!!

	//char* encryptPos = payload_pos_ - 1;	// 암호화'될' 주소
	//short encrypt_len = _header.len + 1;	// 암호화될 길이
	//BYTE RK = _header.randKey;			// 랜덤 키
	//BYTE K = private_key;					// 고정 키
	//BYTE P = 0, E = 0;						// 암호화 변수

	//// 암호화
	//for (int i = 0; i < encrypt_len; i++, encryptPos++) {
	//	P = (*encryptPos) ^ (P + RK + (i + 1));
	//	E = P ^ (E + K + (i + 1));
	//	*((BYTE*)encryptPos) = E;
	//}
}

// 암호패킷을 this에 복호화

bool PacketBuffer::DecryptPacket(char* encryptPacket, BYTE privateKey) {
	// 복호화 변수
	const BYTE RK = ((PacketHeader*)encryptPacket)->randKey;
	const BYTE K = privateKey;
	BYTE P = 0, LP = 0, LE = 0;

	// 복호화 준비
	char* const packetPos = GetPacketPos();						// this 패킷 주소
	char* decryptPos = packetPos + (HEADER_SIZE - 1);			// this 복호화'될' 주소 (복호화 부 : checksum + payload)
	char* encryptPos = encryptPacket + (HEADER_SIZE - 1);		// 암호화 부
	const short encryptLen = ((PacketHeader*)encryptPacket)->len + 1;	// 암호화 부 길이 (checksum + payload)
	memmove(packetPos, encryptPacket, HEADER_SIZE - 1);			// 암호화 되어있지 않은 부분 복사

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
	if (payload_pos_ + sizeof(data) <= write_pos_) {
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