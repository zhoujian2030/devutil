/*
 * DataBuffer.h
 *
 *  Created on: Apr 08, 2016
 *      Author: z.j
 */

#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <assert.h>
#include <string>

namespace net {
    class DataBuffer {
    public:
        DataBuffer();
        DataBuffer(int bufferLength);
        DataBuffer(const DataBuffer& theReceiveBuffer);
        virtual ~DataBuffer();

        void reset();

        // only increase the m_endOfDataPointer to point
        // to the end of valid data inserted in m_buffer
        int increaseDataLength(int numberOfBytes);

        int getSize() const;
        int getLength() const;

        char* getEndOfDataPointer() const;

        char* getStartOfDataPointer() const;

        // Only for test
        std::string getData() const;

        static const int InitialBufferSize;

    private:
        char* m_buffer;
        unsigned int m_bufferSize;

        char* m_endOfDataPointer;
    };

    // --------------------------------------------
    inline void DataBuffer::reset() {
        m_endOfDataPointer = m_buffer;
    }

    // --------------------------------------------
    inline int DataBuffer::increaseDataLength(int numberOfBytes) {
        assert((m_endOfDataPointer + numberOfBytes) <= (m_buffer + m_bufferSize));

        m_endOfDataPointer += numberOfBytes;

        return getLength();
    }

    // --------------------------------------------
    inline int DataBuffer::getSize() const {
        return m_bufferSize;
    }

    // --------------------------------------------
    inline int DataBuffer::getLength() const {
        return (m_endOfDataPointer - m_buffer);
    }

    // --------------------------------------------
    inline char* DataBuffer::getEndOfDataPointer() const {
        return m_endOfDataPointer;
    }

    // --------------------------------------------
    inline char* DataBuffer::getStartOfDataPointer() const {
        return m_buffer;
    }

    // --------------------------------------------
    inline std::string DataBuffer::getData() const {
        std::string dataStr;
        dataStr.append(m_buffer, getLength());

        return dataStr;
    }
}

#endif