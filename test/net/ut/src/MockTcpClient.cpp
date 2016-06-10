#include "MockTcpClient.h"
#include "IoException.h"

using namespace net;
using namespace std;
using namespace cm;

// ------------------------------------
MockTcpClient::MockTcpClient(string ip, unsigned short port) {
    m_tcpSocket = new TcpSocket(ip, port);
}

// ------------------------------------
MockTcpClient::~MockTcpClient() {
    delete m_tcpSocket;
}

// ------------------------------------
bool MockTcpClient::connect() {
    try {
        m_tcpSocket->connect();
        return true;
    } catch (IoException& e) {
        return false;
    }
}

int MockTcpClient::send(string data) {
    m_sendBuffer = new DataBuffer(data);
    const char* buffer = m_sendBuffer->getStartOfDataPointer();
    int result = m_tcpSocket->send(buffer, m_sendBuffer->getLength());
    delete m_sendBuffer;
    return result;
}
