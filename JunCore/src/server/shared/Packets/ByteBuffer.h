#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include <vector>
#include <Define.h>

class MessageBuffer;

class ByteBuffer
{
public:
	constexpr static size_t DEFAULT_SIZE = 0x1000;

	// constructor
	ByteBuffer() : _rpos(0), _wpos(0)
	{
		_storage.reserve(DEFAULT_SIZE);
	}

	ByteBuffer(size_t reserve) : _rpos(0), _wpos(0)
	{
		_storage.reserve(reserve);
	}

	ByteBuffer(ByteBuffer&& buf) noexcept : _rpos(buf._rpos), _wpos(buf._wpos), _storage(std::move(buf._storage))
	{
		buf._rpos = 0;
		buf._wpos = 0;
	}

	ByteBuffer(const ByteBuffer& right) = default;

	ByteBuffer(MessageBuffer&& buffer);

	ByteBuffer& operator=(ByteBuffer const& right)
	{
		if (this != &right)
		{
			_rpos = right._rpos;
			_wpos = right._wpos;
			_storage = right._storage;
		}

		return *this;
	}

	ByteBuffer& operator=(ByteBuffer&& right) noexcept
	{
		if (this != &right)
		{
			_rpos = right._rpos;
			right._rpos = 0;
			_wpos = right._wpos;
			right._wpos = 0;
			_storage = std::move(right._storage);
		}

		return *this;
	}

	virtual ~ByteBuffer() = default;

	void clear()
	{
		_storage.clear();
		_rpos = _wpos = 0;
	}

protected:
	size_t _rpos, _wpos;
	std::vector<uint8> _storage;
};

#endif