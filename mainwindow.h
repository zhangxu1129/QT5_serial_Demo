#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort> //串口
#include <QSerialPortInfo> //串口信息
#include <QDebug> //打印信息

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /*-------------funtions----------------------*/
    void serial_init();
private slots:

    void on_pushButton_OpenPort_clicked();

    void on_pushButton_SendData_clicked();

    void readdata();
private:
    Ui::MainWindow *ui;
    /*-----------variable----------------------*/
    //实例化对象
    QSerialPort *Serial;
};
#endif // MAINWINDOW_H
