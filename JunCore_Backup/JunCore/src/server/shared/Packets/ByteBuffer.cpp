#include "ByteBuffer.h"
#include <Utilities/MessageBuffer.h>

ByteBuffer::ByteBuffer(MessageBuffer&& buffer) : _rpos(0), _wpos(0), _storage(buffer.Move())
{
}