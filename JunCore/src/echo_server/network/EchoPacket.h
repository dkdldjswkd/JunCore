#ifndef TRINITYCORE_WORLDPACKET_H
#define TRINITYCORE_WORLDPACKET_H

#include "Common.h"
//#include "Opcodes.h"
#include <Packets/ByteBuffer.h>
//#include "Duration.h"

class EchoPacket : public ByteBuffer
{
public:
    // just container for later use
    EchoPacket() : ByteBuffer(0), m_opcode(0 /*NULL_OPCODE*/)
    {
    }

    EchoPacket(uint16 opcode, size_t res = 200) : ByteBuffer(res),
        m_opcode(opcode) { }

    EchoPacket(EchoPacket&& packet) : ByteBuffer(std::move(packet)), m_opcode(packet.m_opcode)
    {
    }

    EchoPacket(EchoPacket&& packet, TimePoint receivedTime) : ByteBuffer(std::move(packet)), m_opcode(packet.m_opcode), m_receivedTime(receivedTime)
    {
    }

    EchoPacket(EchoPacket const& right) : ByteBuffer(right), m_opcode(right.m_opcode)
    {
    }

    EchoPacket& operator=(EchoPacket const& right)
    {
        if (this != &right)
        {
            m_opcode = right.m_opcode;
            ByteBuffer::operator=(right);
        }

        return *this;
    }

    EchoPacket& operator=(EchoPacket&& right)
    {
        if (this != &right)
        {
            m_opcode = right.m_opcode;
            ByteBuffer::operator=(std::move(right));
        }

        return *this;
    }

    EchoPacket(uint16 opcode, MessageBuffer&& buffer) : ByteBuffer(std::move(buffer)), m_opcode(opcode) { }

    void Initialize(uint16 opcode, size_t newres = 200)
    {
        clear();
        _storage.reserve(newres);
        m_opcode = opcode;
    }

    uint16 GetOpcode() const { return m_opcode; }
    void SetOpcode(uint16 opcode) { m_opcode = opcode; }

    TimePoint GetReceivedTime() const { return m_receivedTime; }

protected:
    uint16 m_opcode;
    TimePoint m_receivedTime; // only set for a specific set of opcodes, for performance reasons.
};

#endif