Solution:
--------
1. reactor线程从epoll中收到某个socket的event后不会把该socket从epoll中删除，直接调用socket的event handler处理event，该event handler
不能做耗时操作，应该马上消费该event，比如对于TCP server socket，应该调用Socket::accept()接收新连接请求，然后创建新连接的TcpSocket交给
worker线程处理返回，reactor线程之后可以继续以相同方式处理其他socket event，全部处理完后重新再poll epoll的新event

2. TcpServer注册为TcpServerSocket的listener，reactor线程在收到TcpServerSocket的event后调用其handler去accept新TCP连接并创建TcpSocket，
然后调用listener的handleAcceptResult方法把新的连接作为new task交给worker thread处理。选取worker thread的策略是根据该TCP连接的remote ip
和port的网络字节value相加再除worker thread总数得到余数作为获取worker thread的index，这样可以保证同一个worker处理相同连接（remote ip和port）
的多次请求


Test:
----


TODO:
-----

>> add TcpSocket and TcpServerSocket to support TCP server component

>> add TcpClientSocket to support TCP client

>> consider how to delete Socket and SocketEventHandler object when connection is closed

>> 
