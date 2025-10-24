#pragma once
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ is_connected NOTIFY connected_changed)
    Q_PROPERTY(float lower_limit READ lower_limit NOTIFY limits_updated)
    Q_PROPERTY(float upper_limit READ upper_limit NOTIFY limits_updated)
    Q_PROPERTY(float scaling_factor READ scaling_factor NOTIFY scaling_updated)
    Q_PROPERTY(float distance READ distance NOTIFY distance_updated)

public:
    explicit SerialManager(QObject *parent = nullptr);

    Q_INVOKABLE QStringList available_ports();
    Q_INVOKABLE void connect_port(const QString &port_name);
    Q_INVOKABLE void disconnect_port();

    Q_INVOKABLE void get_limits();
    Q_INVOKABLE void set_lower_limit(float value);
    Q_INVOKABLE void set_upper_limit(float value);
    Q_INVOKABLE void get_scaling_factor();
    Q_INVOKABLE void get_distance();

    bool is_connected() const { return m_serial.isOpen(); }
    float lower_limit() const { return m_lower_limit; }
    float upper_limit() const { return m_upper_limit; }
    float scaling_factor() const { return m_scaling_factor; }
    float distance() const { return m_distance; }

signals:
    void connected_changed(bool connected);
    void limits_updated();
    void scaling_updated();
    void distance_updated();

private slots:
    void on_ready_read();

private:
    void send_frame(const QByteArray &payload);
    void parse_frame(const QByteArray &frame);
    quint16 crc16(const QByteArray &data);

    QSerialPort m_serial;
    QByteArray m_buffer;

    float m_lower_limit = 0.0f;
    float m_upper_limit = 0.0f;
    float m_scaling_factor = 0.0f;
    float m_distance = 0.0f;
};
