## ccnet

## 概述
* 整个库是对一个epoll的一个封装
* EventLoop是整个库的核心，对应的架构是 loop peer thread。
* EventLoop收集EPollPoller所产生的事件，通过Channel分发事件给TcpConnection，TcpConnection进行读写到Buffer缓冲区，然后通过回调给TcpServer。用户通过绑定与TcpServer的回调函数即可收到数据报。

## 编译
下载
```
git clone https://github.com/diycat1024/ccnet
```
创建build目录
```
mkdir -p ccnet/build && cd ccnet/build
```
编译
```
cmake ..
make -j4
```
编译客户端
```
g++ -o ../bin/client client.cc -std=c++11
```

运行
```
../bin/server
```
```
../bin/client
```
