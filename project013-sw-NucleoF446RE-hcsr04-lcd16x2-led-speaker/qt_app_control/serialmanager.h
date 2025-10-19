#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(float lowerLimit READ lowerLimit NOTIFY limitsUpdated)
    Q_PROPERTY(float upperLimit READ upperLimit NOTIFY limitsUpdated)

public:
    explicit SerialManager(QObject *parent = nullptr);

    Q_INVOKABLE QStringList availablePorts();
    Q_INVOKABLE void connectPort(const QString &portName);
    Q_INVOKABLE void disconnectPort();
    Q_INVOKABLE void requestLimits();     // 0x10 Get Limits

    bool isConnected() const { return m_serial.isOpen(); }
    float lowerLimit() const { return m_lowerLimit; }
    float upperLimit() const { return m_upperLimit; }

signals:
    void connectedChanged(bool connected);
    void limitsUpdated();

private slots:
    void onReadyRead();

private:
    void parseFrame(const QByteArray &frame);
    quint16 crc16(const QByteArray &data);

    QSerialPort m_serial;
    QByteArray  m_buffer;
    float m_lowerLimit = 0.0f;
    float m_upperLimit = 0.0f;
};

#endif
