# qwx_learn_webserver

1、我们封装了errif函数用来简单的处理错误，由于每个模块都可能使用到所以我们将其放到util文件中

2、socket套接字的使用步骤大致分为：
    2.1、创建套接字，socket
    2.2、绑定端口和ip，bind
    2.3、将文件描述符变成监听状态、listen
    2.4、阻塞等待客户端连接，accpet
    我们将这部分封装成一个Socket类，每个套接字fd都分别对应一个Socket实例化对象，通过对象内部的方法来调用底层socket函数

3、我们将ip、端口号等信息封装成一个InetAddress类为每一份ip、端口号实例化一个对象，来简化使用struct sockaddr_in与struct sockaddr这两个结构体

4、epoll的使用大致分为：
    4.1、创建epoll树，epoll_create
    4.2、创建epoll_event对象，将其与文件描述符绑定，设置监听的事件
    4.3、将epoll_event对象上epoll树
    4.4、创建epoll_event数组用来接收发生变化的文件描述符绑定的epoll_event对象，启动epoll_wait对epoll数进行监听
    4.5、对返回的文件描述符进行对应事件的操作
    4.6、对文件描述符绑定的epoll_event对象进行修改、上树、下数操作
    我们将1~4、6步骤封装到Epoll对象中，通过Epoll对象来调用epoll底层的函数
    我们将5封装成一个Channel对象，为每一个fd都绑定Channel对象，通过调用Channel的方法实现对epoll树的操作，通过设置不同的回调函数实现不同的逻辑

4、 抽象出一个Server类，通过Server提供全部的网络服务
    4.1、将对Epoll的操作封装成一个EventLoop类，通过Server来调用并管理其生命周期
    4.2、将接收套接字功能封装成一个Accept类，通过Server来调用并管理其生命周期
    4.3、将建立连接功能封装成一个Connection类，通过Server来调用并管理生命周期

    