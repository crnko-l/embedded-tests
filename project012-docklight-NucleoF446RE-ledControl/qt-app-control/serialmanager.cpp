#include "serialmanager.h"
#include <QDebug>

SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialManager::readSerial);
}

QStringList SerialManager::availablePorts()
{
    QStringList ports;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
        ports << info.portName();
    return ports;
}

void SerialManager::connectPort(const QString &portName)
{
    if (m_serial.isOpen())
        m_serial.close();

    m_serial.setPortName(portName);
    m_serial.setBaudRate(QSerialPort::Baud115200);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if (m_serial.open(QIODevice::ReadWrite))
        emit connectedChanged();
    else
        qWarning() << "Failed to open port:" << m_serial.errorString();
}

void SerialManager::disconnectPort()
{
    if (m_serial.isOpen()) {
        m_serial.close();
        emit connectedChanged();
    }
}

void SerialManager::sendCommand(const QString &cmd)
{
    if (!m_serial.isOpen()) return;
    QString message = cmd + "\n";
    m_serial.write(message.toUtf8());
}

bool SerialManager::isConnected() const
{
    return m_serial.isOpen();
}

QString SerialManager::lastMessage() const
{
    return m_lastMessage;
}

void SerialManager::readSerial()
{
    QByteArray data = m_serial.readAll();
    m_lastMessage = QString::fromUtf8(data);
    emit messageReceived(m_lastMessage);
}
