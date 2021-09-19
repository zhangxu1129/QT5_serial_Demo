#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*----------------user---------------------------*/
    serial_init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*----------------------------------------------------------------
 *         funtions
 * --------------------------------------------------------------*/
//串口初始化操作
void MainWindow::serial_init()
{
    //将所有可以选择的串口加入到串口选项
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    //遍历列表获取所有端口
    foreach(QSerialPortInfo info,infos)
    {
        //定义串口对象获取数据
          QSerialPort serial;
        //设置串口名称
        serial.setPort(info);
        //测试是否能够打开
        if(serial.open(QIODevice::ReadWrite)) {
            //直接将串口加入到选项
            ui->comboBox_PortName->addItem(serial.portName());
            //关闭串口
            serial.close();
        }
        else {
            //提示端口被占用
            ui->comboBox_PortName->addItem(serial.portName()+"(被占用)");
        }
    }
    ui->pushButton_SendData->setEnabled(false);//关闭使能按钮
}
/*---------------------------------------------------------------
 *                            slots
 * ------------------------------------------------------------*/
//打开串口
void MainWindow::on_pushButton_OpenPort_clicked()
{
    if(ui->pushButton_OpenPort->text() == "打开串口") {
        Serial = new QSerialPort(this);//初始化
        //设置串口名
        Serial->setPortName(ui->comboBox_PortName->currentText());
        //打开串口
        if(!Serial->open(QIODevice::ReadWrite)) {
            qDebug()<<"打开失败";
            //删除串口
            Serial->deleteLater();
            return;
        }
        //设置波特率
        switch(ui->comboBox_BaudRate->currentIndex()) {
            case 0:Serial->setBaudRate(QSerialPort::Baud2400);break;
            case 1:Serial->setBaudRate(QSerialPort::Baud4800);break;
            case 2:Serial->setBaudRate(QSerialPort::Baud9600);break;
            case 3:Serial->setBaudRate(QSerialPort::Baud115200);break;
        }
        //设置数据位
        switch(ui->comboBox_DataBits->currentIndex()) {
            case 0:Serial->setDataBits(QSerialPort::Data5);break;
            case 1:Serial->setDataBits(QSerialPort::Data6);break;
            case 2:Serial->setDataBits(QSerialPort::Data6);break;
            case 3:Serial->setDataBits(QSerialPort::Data7);break;
        }
        //设置校验位
        switch(ui->comboBox_Parity->currentIndex()) {
            case 0:Serial->setParity(QSerialPort::NoParity);break;
            case 1:Serial->setParity(QSerialPort::OddParity);break;
            case 2:Serial->setParity(QSerialPort::EvenParity);break;
        }
        //设置停止位
        switch(ui->comboBox_StopBits->currentIndex()) {
            case 0:Serial->setStopBits(QSerialPort::OneStop);break;
            case 1:Serial->setStopBits(QSerialPort::OneAndHalfStop);break;
            case 2:Serial->setStopBits(QSerialPort::TwoStop);break;
        }
        //关闭流控
        Serial->setFlowControl(QSerialPort::NoFlowControl);
        //关闭选择使能
        ui->comboBox_PortName->setEditable(false);
        ui->comboBox_BaudRate->setEditable(false);
        ui->comboBox_DataBits->setEditable(false);
        ui->comboBox_Parity->setEditable(false);
        ui->comboBox_StopBits->setEditable(false);

        ui->pushButton_OpenPort->setText("关闭串口");
        ui->pushButton_SendData->setEnabled(true);

        //连接信号
        connect(Serial,&QSerialPort::readyRead,this,&MainWindow::readdata);
    }
    else {
        //关闭串口
        Serial->clear();
        Serial->close();
        Serial->deleteLater();
        //开启选择使能
        ui->comboBox_PortName->setEditable(true);
        ui->comboBox_BaudRate->setEditable(true);
        ui->comboBox_DataBits->setEditable(true);
        ui->comboBox_Parity->setEditable(true);
        ui->comboBox_StopBits->setEditable(true);

        ui->pushButton_OpenPort->setText("打开串口");
        ui->pushButton_SendData->setEnabled(false);
    }
}
//发送数据
void MainWindow::on_pushButton_SendData_clicked()
{
    //获取发送内容
    QByteArray data = ui->textEdit_SendData->toPlainText().toUtf8();
    Serial->write(data);
}
//读取数据
void MainWindow::readdata()
{
   //接收所有内容
    QByteArray Data = Serial->readAll();
    //确认收到数据
    if(!Data.isEmpty()) {
        QString str = Data;
        ui->textEdit_ReceiveData->append(str);
        Data.clear();
    }
}
