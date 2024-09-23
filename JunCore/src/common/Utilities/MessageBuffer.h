#ifndef __MESSAGEBUFFER_H_
#define __MESSAGEBUFFER_H_

#include "Define.h"
#include <vector>

struct MessageBuffer
{
	using storage   = std::vector<uint8>;
	using size_type = std::vector<uint8>::size_type;

public:
	MessageBuffer() : _wpos(0), _rpos(0), _storage()
	{
		_storage.resize(4096);
	}

	std::vector<uint8>&& Move()
	{
		_wpos = 0;
		_rpos = 0;
		return std::move(_storage);
	}

	void Resize(size_type bytes)
	{
		_storage.resize(bytes);
	}

private:
	size_type _wpos;
	size_type _rpos;
	storage _storage;
};

#endif