#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <QPointF>
#include "define.h"
#include <QMessageBox>
#include <stdlib.h>
#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QTimer>
#include <QInputDialog>
#include <QDateTime>


const char toHex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ==========================================
    // 【UI 现代化美化：注入 QSS 样式表】
    // ==========================================
    QString qss = R"(
/* 1. 全局字体与基础背景：采用现代无衬线字体，极浅灰蓝背景色 */
        * {
            font-family: "Microsoft YaHei", "Segoe UI", sans-serif;
            font-size: 13px;
        }
        QMainWindow {
            background-color: #F0F2F5; /* 现代 Web 常用的大背景色 */
        }

        /* 2. 卡片化设计 (QGroupBox)：纯白背景，大圆角，去除突兀边框 */
        QGroupBox {
            background-color: #FFFFFF;
            border: 1px solid #E4E7ED;
            border-radius: 8px;
            margin-top: 24px; /* 给标题留出更多呼吸空间 */
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 5px;
            color: #1F2937; /* 深灰黑色，比纯黑更高级 */
            font-weight: bold;
            font-size: 14px;
            top: 2px;
            left: 10px;
        }

        /* 3. 按钮设计：主次分明，无边框，增加圆角半径 */
        QPushButton {
            background-color: #0066FF; /* 充满科技感的高亮蓝 */
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #3385FF;
        }
        QPushButton:pressed {
            background-color: #0052CC;
        }
        QPushButton:disabled {
            background-color: #A0CFFF;
        }
        /* 针对取消/次要操作的按钮（你可以后续通过 objectName 单独赋予背景色，这里统一样式） */
        QPushButton:checked {
            background-color: #FF4D4F; /* 警示红 */
        }

        /* 4. 沉浸式输入框：平时无边框浅灰底，聚焦时亮起 */
        QLineEdit, QComboBox, QSpinBox {
            background-color: #F3F4F6;
            border: 1px solid transparent; /* 隐藏默认边框 */
            border-radius: 6px;
            padding: 6px 10px;
            color: #374151;
        }
        QLineEdit:focus, QComboBox:focus, QPlainTextEdit:focus {
            background-color: #FFFFFF;
            border: 1px solid #0066FF; /* 聚焦时的主题色边框 */
        }
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
        QComboBox::down-arrow {
            image: none; /* 如果有自定义小箭头图标可以放这里，否则保持极简 */
        }

        /* 5. 文本区域：作为卡片显示 */
        QPlainTextEdit {
            background-color: #FFFFFF;
            border: 1px solid #E4E7ED;
            border-radius: 8px;
            padding: 8px;
            color: #1F2937;
        }

        /* 6. 图表视图区域无缝化 */
        QGraphicsView {
            background-color: #FFFFFF;
            border: 1px solid #E4E7ED;
            border-radius: 8px;
        }

        /* 7. 现代极简 Tab 标签页 */
        QTabWidget::pane {
            border: none;
            background: transparent;
        }
        QTabBar::tab {
            background: transparent;
            color: #6B7280;
            padding: 8px 16px;
            font-weight: bold;
            font-size: 14px;
            margin-right: 10px;
            border-bottom: 2px solid transparent; /* 预留底边框空间 */
        }
        QTabBar::tab:hover {
            color: #0066FF;
        }
        QTabBar::tab:selected {
            color: #0066FF;
            border-bottom: 2px solid #0066FF; /* 选中的下划线高亮 */
        }

        /* 8. 进度条：细长现代风格 */
        QProgressBar {
            border: none;
            background-color: #E5E7EB;
            border-radius: 4px;
            text-align: right;
            color: transparent; /* 隐藏中间的文字，现代UI进度条通常不在条内写字 */
            max-height: 8px; /* 变得纤细 */
        }
        QProgressBar::chunk {
            background-color: #10B981; /* 翡翠绿 */
            border-radius: 4px;
        }

        /* 9. 隐藏丑陋的系统滚动条，替换为圆润细线 */
        QScrollBar:vertical {
            border: none;
            background: transparent;
            width: 6px;
            margin: 0px 0px 0px 0px;
        }
        QScrollBar::handle:vertical {
            background: #D1D5DB;
            min-height: 20px;
            border-radius: 3px;
        }
        QScrollBar::handle:vertical:hover {
            background: #9CA3AF;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px; /* 隐藏上下箭头 */
        }
        QScrollBar:horizontal {
            border: none;
            background: transparent;
            height: 6px;
            margin: 0px 0px 0px 0px;
        }
        QScrollBar::handle:horizontal {
            background: #D1D5DB;
            min-width: 20px;
            border-radius: 3px;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px; /* 隐藏左右箭头 */
        }
    )";

    // 将上面定义的样式表应用到整个主窗口及其所有子控件上
    this->setStyleSheet(qss);
    // ==========================================

    statusBar()->showMessage(tr("就绪"));

    // ==== 手动设置默认值 ====
    ui->lEditIpAddr->setText("192.168.1.102");   // 本地IP默认值
    ui->lEditIpPort->setText("1234");           // 本地端口默认值
    ui->lEditUdpIP->setText("192.168.1.10");    // 目标IP默认值
    ui->lEditUdpPort->setText("1234");           // 目标端口默认值
    // ======================

    chartStartTime = QDateTime::currentDateTime();
    lineSeries = new QSplineSeries();
    axisX = new QValueAxis();
    axisY = new QValueAxis();
    MAX_X = 20;
    //MAX_Y = 11000;
    MAX_Y = 150000;
    Chart_Init();
    //设置程序的开启默认画面
    setUdpGuiExt();

    // //获取本机的IP地址,并初始化相应的控件属性和变量
    // QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // // use the first non-localhost IPv4 address
    // for(int i = 0; i < ipAddressesList.size(); ++i) {
    //     if(ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
    //         m_ip = ipAddressesList.at(i).toString();
    //         break;
    //     }
    // }
    // // 设置获取的IP到相应文本框
    // ui->lEditIpAddr->setText(m_ip);
    // ui->lEditUdpIP->setText(m_ip);

    // 设置UDP模式的默认IP和端口

    ui->SndProgressBar->setVisible(false);

    //初始化全局变量
    rmtServerIP = new QHostAddress();
    rcvDataCnt  = 0;
    sndDataCnt = 0;
    TcpClientLinkCnt = 0;
    //NetState = false;
    loopSending = false;
    CurIPPort = "";
    CurPath = "";
    curFile = 0;
    //connect(udpSocket,&QUdpSocket::readyRead,this,&MainWindow::DrawLine);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**********************************************************/
//Function for connection button
void MainWindow::on_pBtnNetCnnt_clicked(bool checked)
{
    if(checked) {     //切换到链接状态
        if(ui->cBoxNetType->currentIndex() == UDP_MODE) {
            //建立UDP链接
            udpSocket = new QUdpSocket(this);
            connect(udpSocket, SIGNAL(readyRead()), this, SLOT(udpDataReceived()));
            lhAddr.setAddress(ui->lEditIpAddr->text());
            lhPort = ui->lEditIpPort->text().toInt();
            rmtAddr.setAddress(ui->lEditUdpIP->text());
            rmtPort = ui->lEditUdpPort->text().toInt();
            bool result = udpSocket->bind(lhPort);
            if(!result)
            {
                ui->pBtnNetCnnt->setChecked(0);
                QMessageBox::information(this, tr("错误"), tr("UDP绑定端口失败!"));
                return;
            }
            ui->CurState->setText(tr("建立UDP连接成功"));

        } else if(ui->cBoxNetType->currentIndex() == TCP_SERVER_MODE) {
            //建立TCP服务器链接
            lhAddr.setAddress(ui->lEditIpAddr->text());
            lhPort = ui->lEditIpPort->text().toInt();

            if(! slotTryCreateTcpServer())
            {
                ui->pBtnNetCnnt->setChecked(0);
                QMessageBox::information(this, tr("错误"), tr("尝试建立服务器失败! 请确认网络状态和端口。"));
                return;
            }
            ui->CurState->setText(tr("建立TCP服务器成功"));
        } else if(ui->cBoxNetType->currentIndex() == TCP_CLIENT_MODE) {
            //建立TCP客户端
            QString ip = ui->lEditIpAddr->text();
            if(!rmtServerIP->setAddress(ip))
            {
                QMessageBox::information(this, tr("错误"), tr("TCP服务器IP设置失败!"));
                return;
            }
            tcpClientSocket = new QTcpSocket(this);
            connect(tcpClientSocket, SIGNAL(readyRead()), this, SLOT(tcpClientDataReceived()));
            tcpClientSocket->connectToHost(*rmtServerIP, ui->lEditIpPort->text().toInt());

            if(!tcpClientSocket->waitForConnected(2000)) {
                ui->lEditUdpPort->setText(QString::number(0, 10));
                ui->pBtnNetCnnt->setChecked(0);
                QMessageBox::information(this, tr("错误"), tr("尝试连接服务器失败! 请确认服务器状态。"));

                return;
            }

            ui->lEditUdpPort->setText(QString::number(tcpClientSocket->localPort(), 10));
            ui->CurState->setText(tr("连接TCP服务器成功"));
        }
        ui->pBtnNetCnnt->setText(tr("断开网络"));
        ui->pBtnSendData->setEnabled(true);
        //NetState = true;
    } else { //切换到断开状态
        if(ui->cBoxNetType->currentIndex() == UDP_MODE) {
            //断开UDP链接
            udpSocket->close();
            delete udpSocket;
        } else if(ui->cBoxNetType->currentIndex() == TCP_SERVER_MODE) {
            //断开TCP服务器链接
            slotDeleteTcpServer();
        } else if(ui->cBoxNetType->currentIndex() == TCP_CLIENT_MODE) {
            //断开TCP客户端链接
            tcpClientSocket->disconnectFromHost();
        }

        ui->pBtnNetCnnt->setText(tr("连接网络"));
        ui->pBtnSendData->setEnabled(false);
        ui->CurState->setText(tr(""));
        // NetState = false;
    }
}

/**********************************************************/
//funcions switch
void MainWindow::on_cBoxNetType_currentIndexChanged(int index)
{
    if(index == UDP_MODE) {
        setUdpGuiExt();
        ui->label_Port->setText(tr("本地端口"));
        ui->label_IP->setText(tr("本地IP地址"));
        ui->labelUdp->setText(tr("目标IP地址"));
        ui->labelUdp1->setText(tr("目标端口"));
        ui->lEditIpAddr->setText(m_ip);
    } else if(index == TCP_SERVER_MODE) {
        setTcpSvrGuiExt();
        // setTcpClientGuiExt();
        ui->label_Port->setText(tr("本地端口"));
        ui->label_IP->setText(tr("本地IP地址"));
        ui->lEditIpAddr->setText(m_ip);
    } else if(index == TCP_CLIENT_MODE) {
        setTcpClientGuiExt();
        ui->label_Port->setText(tr("服务器端口"));
        ui->label_IP->setText(tr("服务器IP地址"));
        ui->labelUdp->setText(tr("本地IP地址"));
        ui->labelUdp1->setText(tr("本地端口"));
        ui->lEditIpAddr->setText(tr("220.165.9.87"));
    }
}

/**********************************************************/
//set UDP mode
void MainWindow::setUdpGuiExt()
{
    ui->labelSpaceUdp->setVisible(true);
    ui->lEditUdpIP->setVisible(true);
    ui->labelUdp->setVisible(true);
    ui->labelUdp1->setVisible(true);
    ui->lEditUdpPort->setVisible(true);

    ui->labelClients->setVisible(false);
    ui->cBoxClients->setVisible(false);
    ui->labelSpaceClients->setVisible(false);
    ui->cBox_chatMode->setVisible(false);
    ui->cBox_echoMode->setVisible(false);
}

/**********************************************************/
//set TCP server
void MainWindow::setTcpSvrGuiExt()
{
    ui->labelSpaceUdp->setVisible(false);
    ui->lEditUdpIP->setVisible(false);
    ui->labelUdp->setVisible(false);
    ui->labelUdp1->setVisible(false);
    ui->lEditUdpPort->setVisible(false);

    ui->labelClients->setVisible(true);
    ui->cBoxClients->setVisible(true);
    ui->labelSpaceClients->setVisible(true);
    ui->cBox_chatMode->setVisible(true);
    ui->cBox_echoMode->setVisible(true);
}

/**********************************************************/
void MainWindow::setTcpClientGuiExt()
{
    ui->labelSpaceUdp->setVisible(true);
    ui->lEditUdpIP->setVisible(true);
    ui->labelUdp->setVisible(true);
    ui->labelUdp1->setVisible(true);
    ui->lEditUdpPort->setVisible(true);

    ui->labelClients->setVisible(false);
    ui->cBoxClients->setVisible(false);
    ui->labelSpaceClients->setVisible(false);
    ui->cBox_chatMode->setVisible(false);
    ui->cBox_echoMode->setVisible(false);
}

/**********************************************************/
//send data
void MainWindow::on_pBtnSendData_clicked()
{
    if(ui->cBoxStartSndFile->checkState()) {
        on_pBtnResetCnt_clicked();
        ui->pBtnSendData->setText(tr("正在发送"));
        ui->pBtnSendData->setEnabled(false);
        insertDateTimeInRcvDisp();
        ui->ReceiveTextEdit->appendPlainText(tr("开始发送..."));
        toSendFile();
        insertDateTimeInRcvDisp();
        ui->ReceiveTextEdit->appendPlainText(tr("发送完成！"));
        float hasSnd = sndDataCnt;
        hasSnd = hasSnd / 1024 / 1024;
        QString hasSndSz = QString("%1").arg(hasSnd);
        ui->ReceiveTextEdit->appendPlainText(tr("共发送数据：") + hasSndSz + "MB");

        ui->pBtnSendData->setText(tr("发送"));
        ui->pBtnSendData->setEnabled(true);
        return;
    }

    if(ui->tEditSendText->toPlainText().size() == 0) {
        QMessageBox::information(this, tr("提示"), tr("发送区为空，请输入内容。"));
        return;  //如果发送区为空则直接跳出
    }

    if(ui->cBoxLoopSnd->checkState())
    {
        if(!loopSending) {
            ui->pBtnSendData->setText(tr("停止发送"));
            timer->start();
            loopSending = true;
        } else {
            timer->stop();
            ui->pBtnSendData->setText(tr("发送"));
            loopSending = false;
        }
    } else {
        toSendData();
        if(ui->cBox_AntoClearSnd->checkState()) {
            ui->tEditSendText->clear();
        }
    }
}

/**********************************************************/
//send data
void MainWindow::toSendData()
{
    QByteArray datagram;

    if(ui->cBox_SndHexDisp->checkState()) {
        QStringList hexStr = ui->tEditSendText->toPlainText().split(" ", Qt::SkipEmptyParts);
        int hexSize = hexStr.size();
        for(int i = 0; i < hexSize; i++) {
            QString hexSubStr = hexStr.at(i);
            datagram.append(ConvertHexStr(hexSubStr));
        }
        datagram.resize(hexSize);
    } else {
        datagram = ui->tEditSendText->toPlainText().toLocal8Bit();
    }

    if(datagram.size() == 0)
        return;

    if(ui->cBoxNetType->currentIndex() == UDP_MODE) {
        udpSocket->writeDatagram(datagram.data(), datagram.size(), rmtAddr, rmtPort);
    } else if(ui->cBoxNetType->currentIndex() == TCP_SERVER_MODE) {
        int idx = ui->cBoxClients->currentIndex() ;
        if(idx == 0) {
            emit sendDataToClient((char *)datagram.data(), datagram.size(), 0, 0);
        } else {
            emit sendDataToClient((char *)datagram.data(), datagram.size(), tcpClientSocketDescriptorList.at(idx), 0);
        }
    } else if(ui->cBoxNetType->currentIndex() == TCP_CLIENT_MODE) {
        tcpClientSocket->write(datagram.data(), datagram.size());
    }

    sndDataCnt += datagram.size();
    ui->lEdit_SndCnt->setText(QString::number(sndDataCnt, 10));
}

/**********************************************************/
void MainWindow::toSendFile()
{
    if(curFile == 0)
        return;
    char buf[1024];
    int rdLen = 0;
    ui->SndProgressBar->setMaximum(curFile->bytesAvailable());
    ui->SndProgressBar->setVisible(true);
    ui->SndProgressBar->setValue(0);
    if(ui->cBoxNetType->currentIndex() == UDP_MODE) {   //UDP 模式

        while(!curFile->atEnd())
        {
            rdLen = curFile->read(buf, 1024);
            udpSocket->writeDatagram(buf, rdLen, rmtAddr, rmtPort);
            sndDataCnt += rdLen;
            ui->lEdit_SndCnt->setText(QString::number(sndDataCnt, 10));
            ui->SndProgressBar->setValue(ui->SndProgressBar->value() + rdLen);
            msDelay(1);
        }
    } else if(ui->cBoxNetType->currentIndex() == TCP_SERVER_MODE) { //TCP服务器模式
        int idx = ui->cBoxClients->currentIndex() ;
        if(idx == 0) {
            while(!curFile->atEnd())
            {
                //msDelay(2);
                rdLen = curFile->read(buf, 1024);
                emit sendDataToClient(buf, rdLen, 0, 0);

                sndDataCnt += rdLen;
                ui->lEdit_SndCnt->setText(QString::number(sndDataCnt, 10));
                ui->SndProgressBar->setValue(ui->SndProgressBar->value() + rdLen);
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
        } else {
            while(!curFile->atEnd())
            {
                // msDelay(2);
                rdLen = curFile->read(buf, 1024);
                emit sendDataToClient(buf, rdLen, tcpClientSocketDescriptorList.at(idx), 0);
                sndDataCnt += rdLen;
                ui->lEdit_SndCnt->setText(QString::number(sndDataCnt, 10));
                ui->SndProgressBar->setValue(ui->SndProgressBar->value() + rdLen);
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
        }
    } else if(ui->cBoxNetType->currentIndex() == TCP_CLIENT_MODE) {
        while(!curFile->atEnd())
        {
            rdLen = curFile->read(buf, 1024);
            tcpClientSocket->write(buf, rdLen);
            //更新发送数据计数器
            sndDataCnt += rdLen;
            ui->lEdit_SndCnt->setText(QString::number(sndDataCnt, 10));
            ui->SndProgressBar->setValue(ui->SndProgressBar->value() + rdLen);
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        }
    }

    ui->SndProgressBar->setVisible(false);
}
/**********************************************************/
//udp data received
void MainWindow::udpDataReceived()
{
    QHostAddress address;
    quint16 port;
    QString tmpIPPort = "";

    static QList<double> frequencyQueue; // 存储频率值的队列
    //static QList<uint32_t> combinedValueQueue; // 静态队列存储最近10次原始值
    const int QUEUE_MAX_LENGTH = 10000;           // 队列最大长度


    double frequency = 0.0; // 用于存储计算出的值

    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &address, &port);

        // ==== 2025年6月18日新增：处理十六进制数据并进行计算 ==== 和fpga板子对应比如我这个是1mhz时钟7字节
        if(datagram.size() >= 7) // 确保有足够的数据
        {
            // 提取后3个字节（索引4、5、6）
            uint8_t byte4 = static_cast<uint8_t>(datagram[4]);
            uint8_t byte5 = static_cast<uint8_t>(datagram[5]);
            uint8_t byte6 = static_cast<uint8_t>(datagram[6]);

            // 合并为24位整数
            uint32_t rawValue = (byte4 << 16) | (byte5 << 8) | byte6;

            // === 修改点：只取低18位 ===
            uint32_t maskedValue = rawValue & 0x3FFFF; // 提取低18位

            // 将有符号18位数转换为32位有符号整数（符号扩展）
            int32_t signedValue;
            // 检查最高位（第17位）是否为1（负数）
            if(maskedValue & 0x20000) {
                // 负数的符号扩展：设置高14位为1
                signedValue = static_cast<int32_t>(maskedValue | 0xFFFFC000);
            } else {
                // 正数直接使用
                signedValue = static_cast<int32_t>(maskedValue);
            }
            // === 修改结束 ===

            // // 将有符号24位数转换为32位有符号整数（符号扩展）
            // int32_t signedValue;
            // // 检查最高位（第23位）是否为1（负数）
            // if(rawValue & 0x00800000) {
            //     // 负数的符号扩展：设置高8位为1
            //     signedValue = static_cast<int32_t>(rawValue | 0xFF000000);
            // } else {
            //     // 正数直接使用
            //     signedValue = static_cast<int32_t>(rawValue);
            // }



            // ===== 队列平均功能 =====

            // ===== 频率值队列平均功能 =====
            // 1. 计算当前数据点的频率值
            double currentFrequency = (static_cast<double>(signedValue) * 25000000.0) / pow(2,21)/2/3.1415926;

            // 2. 将当前频率加入队列
            frequencyQueue.append(currentFrequency);

            // 3. 如果队列超过最大长度，移除最旧的值
            while(frequencyQueue.size() > QUEUE_MAX_LENGTH) {
                frequencyQueue.removeFirst();
            }

            // 4. 计算队列平均值
            double averageFrequency = 0.0;
            if(!frequencyQueue.isEmpty()) {
                double sum = 0.0;
                for(int i = 0; i < frequencyQueue.size(); i++) {
                    sum += frequencyQueue.at(i);
                }
                averageFrequency = sum / frequencyQueue.size();
            }

            // 5. 使用平均值
            frequency = averageFrequency; // 保存频率值

            // 格式化为字符串（保留2位小数）
            QString resultStr = QString("当前值: %1 Hz, 平均值: %2 Hz (基于最近%3次平均)")
                                    .arg(currentFrequency, 0, 'f', 2)
                                    .arg(averageFrequency, 0, 'f', 2)
                                    .arg(frequencyQueue.size())+QString("原始数据如下: ");
            //画图
            updateChart(frequency);

            // 格式化为字符串（保留2位小数）

            // QString resultStr = QString("计算结果: %1 (基于最近%2次平均)").arg(result, 0, 'f', 2).arg(combinedValueQueue.size())+QString("原始数据如下: ");

            // 在接收区显示计算结果（带换行）
            ui->ReceiveTextEdit->appendPlainText(resultStr);
        }
        // ==== 新增代码结束 ====


        if(ui->StartRcvFile->checkState()) {
            char *buf;
            buf = datagram.data();
            if(curFile != 0) {
                curFile->write(buf, datagram.size());
            }
        } else {
            if(!ui->cBox_PauseShowRcv->checkState()) {
                tmpIPPort = address.toString() + ":" + QString::number(port, 10);
                QString rcvMsg  = QString::fromUtf8(datagram, datagram.size());
                if(CurIPPort != tmpIPPort) {
                    CurIPPort = tmpIPPort;
                    if(ui->ReceiveTextEdit->toPlainText().size() != 0) {
                        ui->ReceiveTextEdit->insertPlainText("\n");
                    }
                    ui->ReceiveTextEdit->insertPlainText(tr("【数据来自") + CurIPPort + tr("】\n"));
                }
                //显示数据接收时间
                if(ui->cBox_ShowRcvTime->checkState())    insertDateTimeInRcvDisp();

                //输出数据到接收显示区
                if(!ui->cBox_RcvHexDisp->checkState()) {             //显示字符串
                    ui->ReceiveTextEdit->insertPlainText(rcvMsg);
                } else {                                              //显示十六进制字符串
                    for(int i = 0; i < datagram.size(); i++) {
                        char ch = datagram.at(i);
                        QString tmpStr = "";
                        tmpStr.append(toHex[(ch & 0xf0) / 16]);
                        tmpStr.append(toHex[ch & 0x0f]);
                        tmpStr.append(" ");
                        ui->ReceiveTextEdit->insertPlainText(tmpStr);
                    }
                }
            }
        }
        rcvDataCnt += datagram.size();
        ui->lEdit_RcvCnt->setText(QString::number(rcvDataCnt, 10));

    }
    //图表有关


}



/**********************************************************/
//clear the receive editor
void MainWindow::on_pBtnClearRcvDisp_clicked()
{
    ui->ReceiveTextEdit->clear();
}

/**********************************************************/
//reset the conters
void MainWindow::on_pBtnResetCnt_clicked()
{
    sndDataCnt = 0;
    rcvDataCnt = 0;

    ui->lEdit_RcvCnt->setText(QString::number(0, 10));
    ui->lEdit_SndCnt->setText(QString::number(0, 10));
}

/**********************************************************/
//UDP port changed PRC
void MainWindow::on_lEditUdpPort_textChanged(QString text)
{
    rmtPort = text.toInt();
}

/**********************************************************/
//UDP IP  Adderess changed PRC
void MainWindow::on_lEditUdpIP_textChanged(QString text)
{
    rmtAddr.setAddress(text);
}

/**********************************************************/
// TCP Client data received
void MainWindow::tcpClientDataReceived()
{
    while(tcpClientSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(tcpClientSocket->bytesAvailable());
        tcpClientSocket->read(datagram.data(), datagram.size());
        if(ui->StartRcvFile->checkState()) {
            char *buf;
            buf = datagram.data();
            if(curFile != 0) {
                curFile->write(buf, datagram.size());
            }
        } else {
            if(!ui->cBox_PauseShowRcv->checkState()) {
                QString rcvMsg  = QString::fromUtf8(datagram, datagram.size());
                QString tmpIPPort = ui->lEditIpAddr->text() + ":" + ui->lEditIpPort->text();
                if(CurIPPort != tmpIPPort) {
                    CurIPPort = tmpIPPort;
                    if(ui->ReceiveTextEdit->toPlainText().size() != 0) {
                        ui->ReceiveTextEdit->insertPlainText("\n");
                    }
                    ui->ReceiveTextEdit->insertPlainText(tr("【数据来自") + CurIPPort + tr("】\n"));
                }
                //显示数据接收时间
                if(ui->cBox_ShowRcvTime->checkState())   insertDateTimeInRcvDisp();
                //输出数据到接收显示区
                if(!ui->cBox_RcvHexDisp->checkState()) {           //显示字符串
                    ui->ReceiveTextEdit->insertPlainText(rcvMsg);
                } else {                                            //显示十六进制字符串
                    for(int i = 0; i < datagram.size(); i++) {
                        char ch = datagram.at(i);
                        QString tmpStr = "";
                        tmpStr.append(toHex[(ch & 0xf0) / 16]);
                        tmpStr.append(toHex[ch & 0x0f]);
                        tmpStr.append(" ");
                        ui->ReceiveTextEdit->insertPlainText(tmpStr);
                    }
                }
            }
        }

        rcvDataCnt += datagram.size();
        ui->lEdit_RcvCnt->setText(QString::number(rcvDataCnt, 10));
    }
}

void MainWindow::insertDateTimeInRcvDisp()
{
    int year, month, day;
    QDateTime::currentDateTime().date().getDate(&year, &month, &day);
    QString date = QString::number(year, 10) + "-" + QString::number(month, 10) + "-" + QString::number(day, 10);
    ui->ReceiveTextEdit->appendPlainText(tr("【") + date + tr(" ") + QDateTime::currentDateTime().time().toString() + tr("】"));
}

/**********************************************************/
bool MainWindow::slotTryCreateTcpServer()
{
    mtcpServer = new TcpServer(this);

    if(! mtcpServer->listen(lhAddr, lhPort))
    {
        return false;
        QMessageBox::information(this, tr("错误"), tr("尝试建立服务器失败! 请确认网络状态和端口。"));
    }

    connect(mtcpServer, SIGNAL(updateTcpServer(char*, int, int)), this, SLOT(tcpServerDataReceived(char*, int, int)));
    connect(this, SIGNAL(sendDataToClient(char*, int, int, int)), mtcpServer, SLOT(sendDataToClient(char*, int, int, int)));
    connect(mtcpServer, SIGNAL(addClientLink(QString, int)), this, SLOT(addClientLink(QString, int)));
    connect(mtcpServer, SIGNAL(removeClientLink(QString, int)), this, SLOT(removeClientLink(QString, int)));

    return true;
}

/**********************************************************/
void MainWindow::slotDeleteTcpServer()
{
    //disconnect(mtcpServer,SIGNAL(updateTcpServer(char*,int,int)),this,SLOT(tcpServerDataReceived(char*,int,int)));
    mtcpServer->disconnect();
    mtcpServer->close();
    delete  mtcpServer;
}

/**********************************************************/
void MainWindow::tcpServerDataReceived(char *msg, int length, int socketDescriptorEx)
{
    if(ui->StartRcvFile->checkState()) {    //保存接收数据到文件
        if(curFile != 0) {
            curFile->write(msg, length);
        }
    } else {                                                 //显示接收数据
        if(!ui->cBox_PauseShowRcv->checkState()) {
            int idx = tcpClientSocketDescriptorList.indexOf(socketDescriptorEx);
            QString tmpIPPort = ui->cBoxClients->itemText(idx);
            if(CurIPPort != tmpIPPort) {
                CurIPPort = tmpIPPort;
                if(ui->ReceiveTextEdit->toPlainText().size() != 0) {
                    ui->ReceiveTextEdit->insertPlainText("\n");
                }
                ui->ReceiveTextEdit->insertPlainText(tr("【数据来自") + CurIPPort + tr("】\n"));
            }
            //显示数据接收时间
            if(ui->cBox_ShowRcvTime->checkState())  insertDateTimeInRcvDisp();

            //输出数据到接收显示区
            if(!ui->cBox_RcvHexDisp->checkState()) { //显示字符串
                ui->ReceiveTextEdit->insertPlainText(msg);
            } else {                                                           //显示十六进制字符串
                for(int i = 0; i < length; i++) {
                    char ch = *(msg + i);
                    QString tmpStr = "";
                    tmpStr.append(toHex[(ch & 0xf0) / 16]);
                    tmpStr.append(toHex[ch & 0x0f]);
                    tmpStr.append(" ");
                    ui->ReceiveTextEdit->insertPlainText(tmpStr);
                }
            }
        }
    }
    //群聊功能控制，转发数据
    if(ui->cBox_chatMode->checkState()) {
        if(ui->cBox_echoMode->checkState()) {
            emit sendDataToClient(msg, length, 0, 0);
        } else {
            emit sendDataToClient(msg, length, 0, socketDescriptorEx);
        }
    }

    //数据接收计数器更新
    rcvDataCnt += length;
    ui->lEdit_RcvCnt->setText(QString::number(rcvDataCnt, 10));
}

/**********************************************************/
void MainWindow::addClientLink(QString clientAddrPort, int socketDescriptor)
{
    if(TcpClientLinkCnt == 0) {
        tcpClientSocketDescriptorList.clear();
        tcpClientSocketDescriptorList.append(0);
        ui->cBoxClients->addItem(tr("全部连接"));
    }
    TcpClientLinkCnt++;
    tcpClientSocketDescriptorList.append(socketDescriptor);
    ui->cBoxClients->addItem(clientAddrPort);
}

/**********************************************************/
void MainWindow::removeClientLink(QString clientAddrPort, int socketDescriptor)
{
    if(socketDescriptor != -1)
        return;
    if(TcpClientLinkCnt <= 1) {
        tcpClientSocketDescriptorList.clear();
        ui->cBoxClients->clear();
    } else {
        TcpClientLinkCnt--;
        int idx = ui->cBoxClients->findText(clientAddrPort);
        ui->cBoxClients->removeItem(idx);
        tcpClientSocketDescriptorList.removeAt(idx);
    }
}

/***********************************************/
//保存接收区的文本文件的内容到文件
void MainWindow::on_pBtnSaveRcvData_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("保存接收区内容到文本文件"), tr(""), tr("文本文件(*.txt)"));

    QFile saveFile(path);
    if(saveFile.open(QFile::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&saveFile);
        QString  str = ui->ReceiveTextEdit->toPlainText();
        out << str;
    }
    saveFile.close();
}

/***********************************************/
//开启定时自动发送功能
void MainWindow::on_cBoxLoopSnd_toggled(bool checked)
{
    if(checked) {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(toSendData()));
        int msInterval = ui->lEdit_Interval_ms->text().toInt();
        if(msInterval > 0) {
            timer->setInterval(ui->lEdit_Interval_ms->text().toInt());
        } else {
            ui->cBoxLoopSnd->setChecked(false);
            delete timer;
        }
    } else {
        timer->stop();
        delete timer;
        ui->pBtnSendData->setEnabled(true);
    }
}

/***********************************************/
//修改发送间隔时间
void MainWindow::on_lEdit_Interval_ms_editingFinished()
{
    int msInterval = ui->lEdit_Interval_ms->text().toInt();
    if(msInterval > 0) {
        timer->setInterval(ui->lEdit_Interval_ms->text().toInt());
    }
}

/***********************************************/
//清空发送区
void MainWindow::on_pBtnClearSndDisp_clicked()
{
    ui->tEditSendText->clear();
}

/***********************************************/
//载入文本文件到发送区
void MainWindow::on_pBtnLoadSndData_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("载入文本文件到发送区"));
    QFile file(path);
    QByteArray str;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, tr("错误"), tr("打开文件失败。"));
        return ;
    } else {
        while(!file.atEnd()) {
            str = file.readLine();
            ui->tEditSendText->insertPlainText(str);
        }
    }
    file.close();
}

/***********************************************/
//接收数据转存到文件
void MainWindow::on_StartRcvFile_clicked(bool checked)
{
    if(checked) {
        QFileDialog *qfd = new QFileDialog(this);
        qfd->setViewMode(QFileDialog::List);
        qfd->setFileMode(QFileDialog::AnyFile);
        qfd->setWindowTitle(tr("建立接收文件"));
        // qfd->setFilter(tr("所有文件(*.*)"));         //qt4.8
        qfd->setNameFilter(tr("所有文件(*.*)"));  //qt5

        if(qfd->exec() == QDialog::Accepted)     //如果成功的执行
        {
            QStringList slist = qfd->selectedFiles();
            CurPath = slist[0];
            curFile = new QFile(CurPath);

            if(!curFile->open(QFile::WriteOnly | QIODevice::Truncate)) {
                //打开文件失败
                ui->StartRcvFile->setChecked(false);
                return;
            }
            ui->ReceiveTextEdit->setPlainText(tr("接收数据保存到文件：\n") + CurPath + "\n");
            on_pBtnResetCnt_clicked();
        } else {
            ui->StartRcvFile->setChecked(false);
            return;
        }
    } else {
        ui->ReceiveTextEdit->clear();
        if(curFile) curFile->close();
        if(curFile) delete curFile;
    }
}

/***********************************************/
//发送数据源为文件
void MainWindow::on_cBoxStartSndFile_clicked(bool checked)
{
    if(checked) {
        QFileDialog *qfd = new QFileDialog(this);
        qfd->setViewMode(QFileDialog::List);
        qfd->setFileMode(QFileDialog::AnyFile);
        qfd->setWindowTitle(tr("选择发送文件"));
        // qfd->setFilter(tr("所有文件(*.*)"));            //qt4.8
        qfd->setNameFilter(tr("所有文件(*.*)"));   //qt5

        if(qfd->exec() == QDialog::Accepted)     //如果成功的执行
        {
            QStringList slist = qfd->selectedFiles();
            CurPath = slist[0];
            curFile = new QFile(CurPath);

            if(!curFile->open(QFile::ReadOnly | QIODevice::Truncate)) {
                //打开文件失败
                ui->cBoxStartSndFile->setChecked(false);
                return;
            }
            ui->ReceiveTextEdit->setPlainText(tr("从文件发送数据：\n") + CurPath + tr("\n"));
        } else {
            ui->cBoxStartSndFile->setChecked(false);
            return;
        }
    } else {
        ui->ReceiveTextEdit->clear();
        if(curFile) curFile->close();
        if(curFile) delete curFile;
    }
}

/***********************************************/
//发送区以十六进制与字符串间切换
void MainWindow::on_cBox_SndHexDisp_clicked(bool checked)
{
    QByteArray datagram;
    if(checked) {
        if(ui->tEditSendText->toPlainText().length() != 0) {
            datagram = ui->tEditSendText->toPlainText().toLocal8Bit();
            ui->tEditSendText->clear();
            for(int i = 0; i < datagram.size(); i++) {
                char ch = datagram.at(i);
                QString tmpStr = QString::number(ch, 16);
                ui->tEditSendText->insertPlainText(tmpStr + " ");
            }
        }
    } else {
        if(ui->tEditSendText->toPlainText().length() != 0) {
            QStringList hexStr = ui->tEditSendText->toPlainText().split(" ", Qt::SkipEmptyParts);
            int hexSize = hexStr.size();
            qDebug() << QString::number(hexSize, 10);
            for(int i = 0; i < hexSize; i++) {
                QString hexSubStr = hexStr.at(i);
                datagram.append(ConvertHexStr(hexSubStr));
            }
            ui->tEditSendText->clear();
            QString msg = datagram.data();
            ui->tEditSendText->setPlainText(msg);
        }
    }
}

/***********************************************/
//转化十六进制中的字符到ASCII
char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch - 0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch - 'A' + 10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    else return (-1);
}

/***********************************************/
//转化十六进制字符到ASCII字母
char MainWindow::ConvertHexStr(QString hexSubStr)
{
    char ch = 0;
    if(hexSubStr.length() == 2) {
        // ch =  ConvertHexChar(hexSubStr.at(0).toAscii())*16+ ConvertHexChar(hexSubStr.at(1).toAscii()); //qt4.8
        ch =  ConvertHexChar(hexSubStr.at(0).toLatin1()) * 16 + ConvertHexChar(hexSubStr.at(1).toLatin1());
    } else if(hexSubStr.length() == 1) {
        //ch =  ConvertHexChar(hexSubStr.at(0).toAscii());  //qt4.8
        ch =  ConvertHexChar(hexSubStr.at(0).toLatin1());
    }
    return ch;
}

/***********************************************/
//毫秒级延时
void MainWindow::msDelay(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

void MainWindow::on_actionEnglish_triggered()
{
    translator.load(":/language/English.qm");
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

void MainWindow::on_actionChinese_triggered()
{
    translator.load(":/language/Chinese.qm");
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

// void MainWindow::Chart_Init()
// {
//     chart = new QChart();

//     lineSeries = new QSplineSeries();
//     lineSeries->setPen(QPen(Qt::blue, 2)); // 设置线条样式

//     lineSeries->setName("时间-结果曲线");
//     //把曲线添加到QChart的实例chart中
//     chart->addSeries(lineSeries);
//     //声明并初始化x轴，y轴
//     axisX->setMin(0);
//     axisX->setMax(MAX_X);
//     axisX->setTitleText("时间");
//     axisX->setTickCount(11);
//     axisX->setGridLineVisible(true); //设置网格不显示
//     axisY->setMin(0);
//     axisY->setMax(MAX_Y);
//     axisY->setTitleText("计算结果");
//     axisY->setTickCount(11);
//     axisY->setGridLineVisible(true);
//     chart->addAxis(axisX,Qt::AlignBottom);
//     chart->addAxis(axisY,Qt::AlignLeft);
//     //把曲线关联到坐标轴
//     lineSeries->attachAxis(axisX);
//     lineSeries->attachAxis(axisY);
//     //把chart显示到窗口上
//     ui->graphicsView0->setChart(chart);
//     //设置抗锯齿
//     ui->graphicsView0->setRenderHint(QPainter::Antialiasing);
// }

void MainWindow::Chart_Init()
{
    chart = new QChart();

    lineSeries = new QSplineSeries();
    // 【优化】：将曲线颜色改为更具科技感的青蓝色，并加粗线条(线宽2)
    lineSeries->setPen(QPen(QColor(46, 134, 193), 2));
    lineSeries->setName("时间-频率波动曲线");

    //把曲线添加到QChart的实例chart中
    chart->addSeries(lineSeries);

    //声明并初始化x轴，y轴
    axisX->setMin(0);
    axisX->setMax(MAX_X);
    axisX->setTitleText("时间 (s)"); // 【优化】：补充单位，符合工程严谨性
    axisX->setTickCount(11);

    // 【优化】：将原本突兀的黑色实线网格改为浅灰色虚线，防止网格喧宾夺主
    axisX->setGridLineVisible(true);
    axisX->setGridLinePen(QPen(QColor(220, 220, 220), 1, Qt::DashLine));

    axisY->setMin(0);
    axisY->setMax(MAX_Y);
    axisY->setTitleText("实时频率 (Hz)"); // 【优化】：补充单位
    axisY->setTickCount(11);
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(QPen(QColor(220, 220, 220), 1, Qt::DashLine));

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);

    //把曲线关联到坐标轴
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);

    // 【优化】：消除图表周围不必要的巨大留白，让波形显示区域最大化
    chart->setMargins(QMargins(10, 10, 10, 10));

    //把chart显示到窗口上
    ui->graphicsView0->setChart(chart);
    // 让图表背景完全融入 QGraphicsView 卡片中
    chart->setBackgroundVisible(false);
    // 隐藏图表的外部轮廓线
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    //设置抗锯齿，让曲线更平滑
    ui->graphicsView0->setRenderHint(QPainter::Antialiasing);
}


void MainWindow::DrawLine()
{
    //version1
    // static int count =0;
    // if(count > MAX_X)
    // {
    //     //当曲线上最早的点超出范围时，删除它
    //     lineSeries->removePoints(0,lineSeries->count()-MAX_X);
    //     //更新x轴的范围
    //     chart->axisX()->setMin(count-MAX_X);
    //     chart->axisX()->setMax(count);
    // }
    // // lineSeries->append(QPointF(count,rand()%100));
    // double frequency = string.toDouble();
    // qDebug()<<QDateTime::currentDateTime()<<":"<<frequency;
    // frequencyDataList.append(frequency);
    // lineSeries->append(count,frequency);
    // count++;

    //version2
    // static qint64 startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // double relativeTime = (currentTime - startTime) / 1000.0;  // 转换为秒，精确到0.000秒
    // // 获取频率数据，这里假设string变量已经正确赋值为频率的字符串表示形式，你可能需要根据实际情况调整获取方式
    // double frequency = string.toDouble();
    // frequencyDataList.append(frequency);
    // timeDataList.append(relativeTime);
    // qDebug() << QDateTime::currentDateTime() << ":" << frequency;
    // if (lineSeries->count() > MAX_X) {
    //     // 当曲线上最早的点超出范围时，删除它
    //     lineSeries->removePoints(0, lineSeries->count() - MAX_X);
    // }
    // lineSeries->append(relativeTime, frequency);
    // // 更新图表的x轴范围，确保能显示最新的数据范围
    // chart->axisX()->setMin(relativeTime - MAX_X);
    // chart->axisX()->setMax(relativeTime);

    // // 触发图表重绘，以显示更新后的数据
    // chart->update();

    //Version3

    // static qint64 startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    // double relativeTime = (currentTime - startTime) / 1000.0;  // 转换为秒，精确到0.000秒
    // // 获取频率数据，这里假设string变量已经正确赋值为频率的字符串表示形式，你可能需要根据实际情况调整获取方式
    // double frequency = string.toDouble();
    // frequencyDataList.append(frequency);
    // timeDataList.append(currentTime); // 存储当前时间戳
    // qDebug() << QDateTime::fromMSecsSinceEpoch(currentTime) << ":" << frequency;
    // if (lineSeries->count() > MAX_X) {
    //     // 当曲线上最早的点超出范围时，删除它
    //     lineSeries->removePoints(0, lineSeries->count() - MAX_X);
    // }
    // lineSeries->append(relativeTime, frequency);
    // // 更新图表的x轴范围，确保能显示最新的数据范围
    // chart->axisX()->setMin(relativeTime - MAX_X);
    // chart->axisX()->setMax(relativeTime);

    // // 触发图表重绘，以显示更新后的数据
    // chart->update();
    // 检查是否有新的UDP数据到达
    // if (udpSocket && udpSocket->hasPendingDatagrams()) {
    //     QByteArray datagram;
    //     datagram.resize(udpSocket->pendingDatagramSize());
    //     QHostAddress sender;
    //     quint16 senderPort;

    //     udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

    //     // 尝试解析频率数据
    //     QString dataString = QString::fromUtf8(datagram).trimmed();
    //     bool ok;
    //     double frequency = dataString.toDouble(&ok);

    //     if (ok) {
    //         updateChart(frequency);
    //     }
    // }
}

// void MainWindow::updateChart(double frequency)
// {
//     // 计算当前时间相对于图表起始时间的偏移(秒)
//     double elapsedSeconds = chartStartTime.msecsTo(QDateTime::currentDateTime()) / 1000.0;

//     // 添加新数据点
//     lineSeries->append(elapsedSeconds, frequency);

//     // 删除超出时间窗口的旧数据
//     while (!lineSeries->points().isEmpty()) {
//         QPointF point = lineSeries->points().first();
//         if (point.x() < elapsedSeconds - TIME_WINDOW) {
//             lineSeries->remove(0);
//         }
//         else {
//             break;
//         }
//     }

//     // 动态调整Y轴范围
//     // double minY = axisY->min();
//     // double maxY = axisY->max();

//     // if (frequency < minY) minY = frequency - 1000;
//     // if (frequency > maxY) maxY = frequency + 1000;

//     // axisY->setMin(minY);
//     // axisY->setMax(maxY);

//     // 更新X轴范围，保持时间窗口滚动
//     axisX->setMin(qMax(0.0, elapsedSeconds - TIME_WINDOW));
//     axisX->setMax(elapsedSeconds);

//     // 更新频率数据列表用于统计
//     frequencyDataList.append(frequency);
//     if (frequencyDataList.size() > 1000) {
//         frequencyDataList.removeFirst();
//     }

//     // 更新状态栏显示
//     statusBar()->showMessage(tr("最新计算值: %1  ").arg(frequency, 0, 'f', 2));
// }

void MainWindow::updateChart(double frequency)
{
    // 计算当前时间相对于图表起始时间的偏移(秒)
    double elapsedSeconds = chartStartTime.msecsTo(QDateTime::currentDateTime()) / 1000.0;

    // 添加新数据点
    lineSeries->append(elapsedSeconds, frequency);

    // 删除超出时间窗口(TIME_WINDOW)的旧数据，形成滚动效果
    while (!lineSeries->points().isEmpty()) {
        QPointF point = lineSeries->points().first();
        if (point.x() < elapsedSeconds - TIME_WINDOW) {
            lineSeries->remove(0);
        } else {
            break;
        }
    }

    // ==========================================
    // 【核心优化】：Y轴动态自适应量程 (Auto-scaling)
    // ==========================================
    if (lineSeries->points().size() > 0) {
        // 先用当前窗口里的第一个点的 Y 值作为基准
        double minY = lineSeries->points().first().y();
        double maxY = minY;

        // 遍历屏幕上（当前队列中）的所有数据点，找出真实的最高点和最低点
        for (const QPointF &p : lineSeries->points()) {
            if (p.y() < minY) minY = p.y();
            if (p.y() > maxY) maxY = p.y();
        }

        // 计算当前窗口内频率的最大落差
        double diff = maxY - minY;

        // 视觉裕量：在最高点和最低点之外，再额外留出 20% 的空间，防止曲线贴边
        double margin = diff * 0.20;

        // 如果数据完全没变化(极差为0)，设定一个最小的默认裕量
        // 考虑到你们测量的是亚赫兹级别，最小裕量设为 0.5 Hz
        if (margin < 0.5) {
            margin = 0.5;
        }

        // 动态设置 Y 轴的显示范围
        axisY->setMin(minY - margin);
        axisY->setMax(maxY + margin);
    }
    // ==========================================

    // 更新X轴范围，保持时间窗口滚动
    axisX->setMin(qMax(0.0, elapsedSeconds - TIME_WINDOW));
    axisX->setMax(elapsedSeconds);

    // 更新频率数据列表用于统计 (你原有的滑动平均逻辑)
    frequencyDataList.append(frequency);
    if (frequencyDataList.size() > 1000) {
        frequencyDataList.removeFirst();
    }

    // 更新状态栏显示
    statusBar()->showMessage(tr("最新计算值: %1 Hz").arg(frequency, 0, 'f', 2));
}



// 添加平均值计算函数
double MainWindow::calculateAverage()
{
    if (frequencyDataList.isEmpty()) return 0.0;

    double sum = 0.0;
    for (double value : frequencyDataList) {
        sum += value;
    }
    return sum / frequencyDataList.size();
}
