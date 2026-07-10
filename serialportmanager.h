#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QIODevice>
class SerialPortManager :public QObject{
    Q_OBJECT
public:
    explicit SerialPortManager(QObject *parent=nullptr);
    QStringList availablePorts() const;
    bool openPort(const QString &portName,int baudRate);
    void closePort();
    void sendText(const QString &text);
    bool isOpen() const;
signals:
    void opened();
    void closed();
    void dataReceived(const QString &text);
    void errorOccurred(const QString &message);
    void dataSent(const QString &text,qint64 bytes);
private:
    QSerialPort *m_serialPort;
};


#endif // SERIALPORTMANAGER_H
