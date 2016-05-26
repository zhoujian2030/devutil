Solution:
--------
1. reactor线程从epoll中收到某个socket的event后不会把该socket从epoll中删除，直接调用socket的event handler处理event，该event handler
不能做耗时操作，应该马上消费该event，比如对于TCP server socket，应该调用Socket::accept()接收新连接请求，然后创建新连接的TcpSocket交给
worker线程处理返回，reactor线程之后可以继续以相同方式处理其他socket event，全部处理完后重新再poll epoll的新event

2. TcpServer注册为TcpServerSocket的listener，reactor线程在收到TcpServerSocket的event后调用其handler去accept新TCP连接并创建TcpSocket，
然后调用listener的handleAcceptResult方法把新的连接作为TcpAcceptTask交给worker thread处理。选取worker thread的策略是根据该TCP连接的remote ip
和port的网络字节value相加再除worker thread总数得到余数作为获取worker thread的index，这样可以保证同一个worker处理相同连接（remote ip和port）
的多次请求.
每一个worker都会调用其mapping的唯一TcpServerWorker处理该worker负责的TCP连接，也就是说一个TcpServerWorker instance只会被一个worker thread调用，
该类的代码不会出现多个worker线程冲突的问题

3. worker thread在执行TcpAcceptTask时，有两种模式，一是同步模式，worker线程自己或者另起一个连接处理线程直接调用TcpSocket的recv阻塞接受对端
的消息，此时该worker线程（或者改连接处理线程）不能再处理其他新的连接请求task，直到当前连接被释放；二是异步模式，worker线程执行的TcpAcceptTask
不会阻塞在recv上，而是把该TcpSocket注册到epoll的EPOLLIN事件，由reactor线程接收epoll event后再调用该TcpSocket的event handler接收处理消息。

TBD
---
1. server socket和其他新连接的的socket都是注册到reactor thread pool中的某个thread，但是server socket的重要性应该远远高于其他连接socket，如果刚好
负责监听server socket所在epoll的reactor thread crash掉则会导致无法接受新连接服务。可否考虑另起一个独立进程listen server socket？


Test:
----


TODO:
-----

>> add TcpSocket and TcpServerSocket to support TCP server component

>> add TcpClientSocket to support TCP client

>> consider how to delete Socket and SocketEventHandler object when connection is closed

>> 
