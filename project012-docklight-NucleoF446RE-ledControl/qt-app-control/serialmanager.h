#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(QString lastMessage READ lastMessage NOTIFY messageReceived)

public:
    explicit SerialManager(QObject *parent = nullptr);

    Q_INVOKABLE QStringList availablePorts();
    Q_INVOKABLE void connectPort(const QString &portName);
    Q_INVOKABLE void disconnectPort();
    Q_INVOKABLE void sendCommand(const QString &cmd);

    bool isConnected() const;
    QString lastMessage() const;

signals:
    void connectedChanged();
    void messageReceived(const QString &message);

private slots:
    void readSerial();

private:
    QSerialPort m_serial;
    QString m_lastMessage;
};

#endif // SERIALMANAGER_H
