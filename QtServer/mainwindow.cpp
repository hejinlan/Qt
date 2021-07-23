#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->port->setText("8899");

    //  创建监听的服务器对象
    m_s = new QTcpServer(this);

    connect(m_s, &QTcpServer::newConnection, this,[=](){
         m_tcp = m_s->nextPendingConnection();

         m_status->setPixmap(QPixmap(":/connect.png").scaled(25,25));
        // 检测是否可以接受数据
        connect( m_tcp, &QTcpSocket::readyRead, this, [=]()
        {
            QByteArray data = m_tcp->readAll();
            ui->record->append("客户端say:"+data);
        });

        connect(m_tcp, &QTcpSocket::disconnected, this, [=]()
        {
            m_tcp->close();
            m_tcp->deleteLater();
            m_status->setPixmap(QPixmap(":/disconnect.png").scaled(25,25));

        });
    });

    // 状态栏
    m_status = new QLabel;
    m_status->setPixmap(QPixmap(":/disconnect.png").scaled(25,25));
    ui->statusbar->addWidget(new QLabel("连接状态："));
    ui->statusbar->addWidget(m_status);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any, port);
    ui->setListen->setDisabled(true);
}

void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->toPlainText();
    m_tcp->write(msg.toUtf8());
    ui->record->append("服务端say:"+msg);

}
