#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QStatusBar"
#include "QTableWidgetItem"
#include "QHeaderView"
#include "QBrush"
#include "QDoubleValidator"
#include "QDateTime"
#include "Qsettings"
#include "worker.h"
#include "logmanager.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    logManager =new LogManager(ui->textEditLog);
    this->setStyleSheet(
        "QMainWindow { background-color: white; color: black; }"
        "QWidget { background-color: white; color: black; }"
        "QPushButton:disabled { color: gray; background-color: #eeeeee; }"
        );
    setWindowTitle("Device Tool");
    resize(800,600);
    ui->labelStatus->setText("设备未连接");
    ui->labelStatus->setStyleSheet("color: red;");
    statusBar()->showMessage("设备未连接");
    ui->comboBoxBaudRate->addItem("9600");
    ui->comboBoxBaudRate->addItem("19200");
    ui->comboBoxBaudRate->addItem("38400");
    ui->comboBoxBaudRate->addItem("57600");
    ui->comboBoxBaudRate->addItem("115200");
    ui->comboBoxBaudRate->setCurrentText("115200");
    ui->textEditSerialReceive->setReadOnly(true);
    initTcpSocket();
    scanSerialPorts();
    loadSetting();
    //startWorkerTask();
    updateTcpUiState(false);
}
MainWindow::~MainWindow()
{
    saveSetting();
    delete ui;
}
void MainWindow::on_btnConnect_clicked(){
    QString ip=ui->lineEditIp->text().trimmed();
    QString portText=ui->lineEditPort->text().trimmed();
    if(ip.isEmpty()||portText.isEmpty()){
        QMessageBox::warning(this,"提示","请输入IP和端口.");
        return;
    }
    bool ok=false;
    quint16 port=portText.toUShort(&ok);
    if(!ok){
        QMessageBox::warning(this,"提示","端口格式不正确.");
        return;
    }
    socket->connectToHost(ip,port);
    logManager->tcp("正在连接到"+ip+":"+QString::number(port));
    statusBar()->showMessage("正在连接到......");
    ui->labelStatus->setText("连接中");
    ui->labelStatus->setStyleSheet("color: orange");
}
void MainWindow::on_btnDisconnect_clicked(){
    if(socket->state()==QAbstractSocket::ConnectedState){
        appendLog("请求用户断开连接");
        socket->disconnectFromHost();
    }
    else{
        appendLog("现在没有已经建立的TCP连接");
        ui->labelStatus->setText("设备未连接");
        ui->labelStatus->setStyleSheet("color: red");
        statusBar()->showMessage("设备未连接");
    }
}
void MainWindow::on_btnClearLog_clicked(){
    logManager->clear();
}
void MainWindow::initTcpSocket(){
    socket=new QTcpSocket(this);
    connect(socket,&QTcpSocket::connected,this,[this](){
        logManager->tcp("connected：TCP连接成功");
        ui->labelStatus->setText("设备已连接");
        ui->labelStatus->setStyleSheet("color:green;");
        statusBar()->showMessage("设备已连接");
        updateTcpUiState(true);

    });
    connect(socket,&QTcpSocket::disconnected,this,[this](){
        if(ui->labelStatus->text()=="TCP连接错误"){
            logManager->tcp("disconnected：连接失败后 socket 已关闭");
        }
        else{
            logManager->tcp("disconnected：TCP连接已断开");
            ui->labelStatus->setText("设备未连接");
            ui->labelStatus->setStyleSheet("color:red");
            statusBar()->showMessage("设备未连接");
        }
        updateTcpUiState(false);
    });
    connect(socket,&QTcpSocket::readyRead,this,[this]{
        QByteArray date=socket->readAll();
        QString text=QString::fromUtf8(date);
        logManager->recive("readyRead:收到数据:"+text);
    } );
    connect(socket,&QTcpSocket::errorOccurred,this,[this](){
        logManager->error("TCP错误:"+socket->errorString());
        ui->labelStatus->setText("TCP连接错误");
        ui->labelStatus->setStyleSheet("color:red");
        statusBar()->showMessage("TCP连接错误");
        updateTcpUiState(false);
    }


            );
}
void MainWindow::appendLog(const QString &message){
    logManager->info(message);
}
void MainWindow::scanSerialPorts()
{
    const QList<QSerialPortInfo> ports=QSerialPortInfo::availablePorts();
    appendLog("开始扫描端口");
    for(const QSerialPortInfo &info:ports){
        appendLog("发现串口:1"+info.portName()+"-"+info.description());
    }
    if(ports.isEmpty()){
        appendLog("未发现可用串口");
    }



}
void MainWindow::on_btnSendTcp_clicked(){
    if(socket->state()!=QAbstractSocket::ConnectedState){
        logManager->error("TCP发送失败：当前未连接");
        statusBar()->showMessage("TCP发送失败：当前未连接");
        return;
    }
    QString text=ui->lineEditTcpSend->text().trimmed();
    if(text.isEmpty()){
        appendLog("TCP发送失败：发送内容为空");
        statusBar()->showMessage("TCP发送失败：当前未连接");
        return;
    }
    QByteArray date=text.toUtf8();
    qint64 bytes=socket->write(date);
    logManager->send("TCP发送数据："+text+",字节数,"+QString::number(bytes));
    statusBar()->showMessage("TCP发送成功");
    ui->lineEditTcpSend->clear();
}
void MainWindow::updateTcpUiState(bool connected){
    ui->btnConnect->setEnabled(!connected);
    ui->btnDisconnect->setEnabled(connected);
    ui->btnSendTcp->setEnabled(connected);
}
void MainWindow::loadSetting(){
    QSettings settings("anllenge","DeviceTool");
    QString ip=settings.value("tcp/ip","127.0.0.1").toString();
    QString port=settings.value("tcp/port","8888").toString();
    QString baudRate=settings.value("serial/baudRate").toString();
    ui->lineEditIp->setText(ip);
    ui->lineEditPort->setText(port);
    ui->comboBoxBaudRate->setCurrentText(baudRate);
    appendLog("已保存当前配置");
}
void MainWindow::saveSetting(){
    QSettings settings("anllenge","DeviceTool");
    settings.setValue("tcp/ip",ui->lineEditIp->text().trimmed());
    settings.setValue("tcp/port",ui->lineEditPort->text().trimmed());
    settings.setValue("serial/baudRate",ui->comboBoxBaudRate->currentText());
    appendLog("已保存当前配置");
}
void MainWindow::startWorkerTask(){
    workerThread=new QThread(this);
    Worker *worker=new Worker;
    worker->moveToThread(workerThread);
    connect(workerThread,&QThread::started,worker,&Worker::doWork);
    connect(worker,&Worker::progress,this,[this](const QString message){
        appendLog(message);
    });
    connect(worker,&Worker::finished,workerThread,&QThread::quit);
    connect(worker,&Worker::finished,worker,&Worker::deleteLater);
    connect(workerThread,&QThread::finished,workerThread,&QThread::deleteLater);
    workerThread->start();

}