#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QTcpSocket>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();
    void on_btnClearLog_clicked();
    void on_btnSendTcp_clicked();

private:
    QTcpSocket *socket;
    QSerialPort *serialPort;
    QThread *workerThread=nullptr;
private:
    Ui::MainWindow *ui;
private:
    void initTcpSocket();
    void appendLog(const QString &message);
    void scanSerialPorts();
    void updateTcpUiState(bool connected);
    void loadSetting();
    void saveSetting();
    void startWorkerTask();
};

#endif // MAINWINDOW_H
