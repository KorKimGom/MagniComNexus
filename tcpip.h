#ifndef TCPIP_H
#define TCPIP_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSet>
#include <QDataStream>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QHostAddress>

class TcpIp : public QObject
{
    Q_OBJECT

public:
    explicit TcpIp(QObject* parent = nullptr);
    ~TcpIp();

    void startServer(quint16 port);
    void stopServer();
    void sendMessage(QTcpSocket* socket, const QString& message);
    void sendAttachment(QTcpSocket* socket, const QString& filePath);

signals:
    void newMessage(const QString& message);

private slots:
    void newConnection();
    void receiveData(QTcpSocket* socket);
    void handleDisconnection(QTcpSocket* socket);
    void handleError(QTcpSocket* socket, QAbstractSocket::SocketError error);

private:
    QTcpServer* m_server;
    QSet<QTcpSocket*> connection_set;

    void setupConnections(QTcpSocket* socket);
    void sendData(QTcpSocket* socket, const QByteArray& data);
};

#endif // TCPIP_H
