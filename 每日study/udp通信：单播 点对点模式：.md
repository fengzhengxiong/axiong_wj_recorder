udp通信：单播 点对点模式：

实现方式：QUdpSocket      QT5

主要函数：

```
发送端主要发送函数：

m_udpSocket->bind(local_port）;  //绑定本地ip和本地端口号

m_udpSocket->writeDatagram(str.toUtf8(), QHostAddress(peer_ip), peer_port);
//向目标ip和目标端口号发送数据
//udpSocket->writeDatagram(QNetworkDatagram(send_data,address,port));  //udp发送原函数

发送端主要接收函数：

在构造函数中：
connect(m_udpSocket, &QUdpSocket::readyRead, this, &udp_1_::on_readyRead);     
// 当有可读数据时发出readyRead信号

void udp_1_::on_readyRead()
{
    QNetworkDatagram datagram = m_udpSocket->receiveDatagram();  
    // 读取数据，这里需要添加qnetworkdatagram.h头文件
    ui->textEdit->append(datagram.data());                      // 显示读取到的数据
}
```