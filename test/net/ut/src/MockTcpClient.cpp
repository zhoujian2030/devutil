#include "MockTcpClient.h"

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
    return (m_tcpSocket->connect() == 0);
}

int MockTcpClient::send(string data) {
    m_sendBuffer = new DataBuffer(data);
    int result = m_tcpSocket->send(m_sendBuffer->getStartOfDataPointer(), m_sendBuffer->getLength());
    delete m_sendBuffer;
    return result;
}
