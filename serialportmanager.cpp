#include "serialportmanager.h"
SerialPortManager::SerialPortManager(QObject *parent):QObject(parent),m_serialPort(new QSerialPort(this)){
    connect(m_serialPort,&QSerialPort::readyRead,this,[this](){
        QByteArray date =m_serialPort->readAll();
        QString text =QString::fromUtf8(date);
        emit dataReceived(text);
    });
    connect(m_serialPort,&QSerialPort::errorOccurred,this,[this](QSerialPort::SerialPortError error){
        if(error==QSerialPort::NoError){
            return;
        }
        emit errorOccurred(m_serialPort->errorString());
    });
}
QStringList  SerialPortManager::availablePorts()const{
    QStringList names;
    const QList<QSerialPortInfo> ports=QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info :ports){
        names.append(info.portName());
    }
    return names;
}
bool SerialPortManager::openPort(const QString &portName,int baudRate){
    if(m_serialPort->isOpen()){
        m_serialPort->close();
    }
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if(!m_serialPort->open(QIODevice::ReadWrite)){
        emit errorOccurred(m_serialPort->errorString());
        return false;
    }
    emit opened();
    return true;
}
void SerialPortManager::closePort(){
    if(m_serialPort->isOpen()){
        m_serialPort->close();
        emit closed();
    }
}
void SerialPortManager::sendText(const QString &text){
    if(!m_serialPort->isOpen()){
        emit errorOccurred("串口未打开");
        return;
    }
    QByteArray data=text.toUtf8();
    qint64 bytes=m_serialPort->write(data);
    if(bytes==-1){
        emit errorOccurred(m_serialPort->errorString());
        return;
    }
    emit dataSent(text,bytes);

}
bool SerialPortManager::isOpen() const{
    return m_serialPort->isOpen();
}