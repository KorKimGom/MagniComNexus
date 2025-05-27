#include "tcpip.h"

TcpIp::TcpIp(QObject* parent) : QObject(parent), m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &TcpIp::newConnection);
}

TcpIp::~TcpIp()
{
    stopServer();
}

void TcpIp::startServer(quint16 port)
{
    if (m_server->listen(QHostAddress::Any, port))
        emit newMessage("Server is listening...");
    else
        emit newMessage(QString("Unable to start server: %1").arg(m_server->errorString()));
}

void TcpIp::stopServer()
{
    foreach (QTcpSocket* socket, connection_set)
    {
        socket->close();
        socket->deleteLater();
    }
    connection_set.clear();
    m_server->close();
}

void TcpIp::newConnection()
{
    while (m_server->hasPendingConnections())
    {
        QTcpSocket* socket = m_server->nextPendingConnection();
        connection_set.insert(socket);
        setupConnections(socket);
        emit newMessage(QString("Client connected: %1").arg(socket->socketDescriptor()));
    }
}

void TcpIp::setupConnections(QTcpSocket* socket)
{
    connect(socket, &QTcpSocket::readyRead, this, [=]() { receiveData(socket); });
    connect(socket, &QTcpSocket::disconnected, this, [=]() { handleDisconnection(socket); });
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, [=](QAbstractSocket::SocketError error) { handleError(socket, error); });
}

void TcpIp::receiveData(QTcpSocket* socket)
{
    QByteArray buffer;
    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    if (!socketStream.commitTransaction())
        return;

    QString header = buffer.mid(0, 128);
    QString fileType = header.split(",")[0].split(":")[1];
    buffer = buffer.mid(128);

    if (fileType == "message")
        emit newMessage(QString("%1 :: %2").arg(socket->socketDescriptor()).arg(QString::fromStdString(buffer.toStdString())));
}

void TcpIp::handleDisconnection(QTcpSocket* socket)
{
    connection_set.remove(socket);
    socket->deleteLater();
    emit newMessage("Client disconnected");
}

void TcpIp::handleError(QTcpSocket* socket, QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::RemoteHostClosedError)
        return;

    emit newMessage(QString("Socket error: %1").arg(socket->errorString()));
}

void TcpIp::sendData(QTcpSocket* socket, const QByteArray& data)
{
    if (socket && socket->isOpen())
    {
        QDataStream socketStream(socket);
        socketStream.setVersion(QDataStream::Qt_5_15);
        socketStream << data;
    }
}

void TcpIp::sendMessage(QTcpSocket* socket, const QString& message)
{
    QByteArray header;
    header.prepend(QString("fileType:message,fileName:null,fileSize:%1;").arg(message.size()).toUtf8());
    header.resize(128);

    QByteArray byteArray = message.toUtf8();
    byteArray.prepend(header);

    sendData(socket, byteArray);
}

void TcpIp::sendAttachment(QTcpSocket* socket, const QString& filePath)
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

        sendData(socket, byteArray);
    }
}
