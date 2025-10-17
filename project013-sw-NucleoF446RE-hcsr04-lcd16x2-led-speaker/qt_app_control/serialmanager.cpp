#include "serialmanager.h"
#include <QDebug>

SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialManager::onReadyRead);
}

QStringList SerialManager::availablePorts()
{
    QStringList list;
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts())
        list << info.portName();
    return list;
}

void SerialManager::connectPort(const QString &portName)
{
    if (m_serial.isOpen())
        m_serial.close();

    m_serial.setPortName(portName);
    m_serial.setBaudRate(115200);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);

    if (m_serial.open(QIODevice::ReadWrite))
        emit connectedChanged(true);
    else
        qWarning() << "Failed to open" << portName << ":" << m_serial.errorString();
}

void SerialManager::disconnectPort()
{
    if (m_serial.isOpen()) {
        m_serial.close();
        emit connectedChanged(false);
    }
}

void SerialManager::requestLimits()
{
    if (!m_serial.isOpen()) return;
    QByteArray cmd;
    cmd.append(0x7E);
    cmd.append(0x01);
    cmd.append(0x7E);
    m_serial.write(cmd);
}

void SerialManager::requestDistance()
{
    // Optional — can be used if firmware sends continuous distance
}

void SerialManager::onReadyRead()
{
    m_buffer.append(m_serial.readAll());

    // Parse any complete 0x7E ... 0x7E frames
    while (true) {
        int start = m_buffer.indexOf('\x7E');
        if (start == -1) break;
        int end = m_buffer.indexOf('\x7E', start + 1);
        if (end == -1) break;

        QByteArray frame = m_buffer.mid(start, end - start + 1);
        parseFrame(frame);
        m_buffer.remove(0, end + 1);
    }
}

void SerialManager::parseFrame(const QByteArray &frame)
{
    if (frame.size() < 12) return; // 0x7E + 8-byte payload + 2-byte CRC + 0x7E

    QByteArray payload = frame.mid(1, 8);
    QByteArray crcBytes = frame.mid(9, 2);
    quint16 receivedCrc = static_cast<quint8>(crcBytes[0]) | (static_cast<quint8>(crcBytes[1]) << 8);
    quint16 calcCrc = crc16(payload);

    if (receivedCrc == calcCrc) {
        memcpy(&m_lowerLimit, payload.data(), 4);
        memcpy(&m_upperLimit, payload.data() + 4, 4);
        emit limitsUpdated();
    } else {
        qWarning() << "CRC mismatch";
    }
}

quint16 SerialManager::crc16(const QByteArray &data)
{
    quint16 crc = 0xFFFF;
    for (uint8_t byte : data) {
        crc ^= byte;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001)
                crc = (crc >> 1) ^ 0x8408;
            else
                crc >>= 1;
        }
    }
    return ~crc;
}
