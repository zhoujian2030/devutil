Solution:
--------
1. reactor线程从epoll中收到某个socket的event后不会把该socket event从epoll中删除，直接调用socket的event handler处理event，该event handler不能做耗时操作，应该马上消费该event，比如对于TCP server socket，应该调用Socket::accept()接收新连接请求，然后创建新连接的TcpSocket交给worker线程处理返回，reactor线程之后可以继续以相同方式处理其他socket event，全部处理完后重新再poll epoll的新event


Test:
----


TODO:
-----

>> add TcpSocket and TcpServerSocket to support TCP server component

>> add TcpClientSocket to support TCP client

>> consider how to delete Socket and SocketEventHandler object when connection is closed

>> 
