#ifndef UDPIP_H
#define UDPIP_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>

class UdpIp : public QObject
{
    Q_OBJECT

public:
    explicit UdpIp(QObject* parent = nullptr);
    ~UdpIp();

    void startListening(quint16 port);
    void sendMessage(const QHostAddress& address, quint16 port, const QString& message);
    void sendAttachment(const QHostAddress& address, quint16 port, const QString& filePath);

signals:
    void newMessage(const QString& message);

private slots:
    void receiveData();

private:
    QUdpSocket* m_socket;
    void sendData(const QHostAddress& address, quint16 port, const QByteArray& data);
};

#endif // UDPIP_H
