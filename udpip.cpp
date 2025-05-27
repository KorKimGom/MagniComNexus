#include "udpip.h"

UdpIp::UdpIp(QObject* parent) : QObject(parent), m_socket(new QUdpSocket(this))
{
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpIp::receiveData);
}

UdpIp::~UdpIp()
{
    delete m_socket;
}

void UdpIp::startListening(quint16 port)
{
    if (m_socket->bind(QHostAddress::Any, port))
        emit newMessage("UDP Server is listening...");
    else
        emit newMessage("Failed to start UDP server.");
}

void UdpIp::receiveData()
{
    while (m_socket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(m_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        QString header = buffer.mid(0, 128);
        QString fileType = header.split(",")[0].split(":")[1];
        buffer = buffer.mid(128);

        if (fileType == "message")
            emit newMessage(QString("%1:%2 :: %3").arg(sender.toString()).arg(senderPort).arg(QString::fromUtf8(buffer)));
    }
}

void UdpIp::sendData(const QHostAddress& address, quint16 port, const QByteArray& data)
{
    m_socket->writeDatagram(data, address, port);
}

void UdpIp::sendMessage(const QHostAddress& address, quint16 port, const QString& message)
{
    QByteArray header;
    header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(message.size()).toUtf8());
    header.resize(128);

    QByteArray byteArray = message.toUtf8();
    byteArray.prepend(header);

    sendData(address, port, byteArray);
}

void UdpIp::sendAttachment(const QHostAddress& address, quint16 port, const QString& filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QFileInfo fileInfo(file.fileName());
        QString fileName(fileInfo.fileName());

        QByteArray header;
        header.prepend(QString("fileType:attachment,fileName:%1,fileSize:%2;").arg(fileName).arg(file.size()).toUtf8());
        header.resize(128);

        QByteArray byteArray = file.readAll();
        byteArray.prepend(header);

        sendData(address, port, byteArray);
    }
}
