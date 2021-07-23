#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>
#include "sendfile.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "客户端主线程：" << QThread::currentThread();

    ui->ip->setText("127.0.01");
    ui->port->setText("8989");
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

    // 创建线程对象
    QThread *t = new QThread;
    // 创建任务对象
    SendFile *worker = new SendFile;

    worker->moveToThread(t);
    connect(this, &MainWindow::sendFile,worker,&SendFile::sendFile);

    connect(this, &MainWindow::startConnect, worker, &SendFile::connectServer);
    // 处理主线程发送的信号
    connect(worker,&SendFile::connectOK, this,[=]()
    {
        QMessageBox::information(this, "连接服务器","已经成功连接了服务器，恭喜！");

    });
    connect(worker,&SendFile::gameover, this,[=]()
    {
        // 资源释放
        t->quit();
        t->wait();
        worker->deleteLater();
        t->deleteLater();

    });

    connect(worker, &SendFile::curPercent, ui->progressBar,&QProgressBar::setValue);
    t->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectServer_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    emit startConnect(port, ip);
}

void MainWindow::on_selFile_clicked()
{
    QString path = QFileDialog::getOpenFileName();
    if(path.isEmpty()){
        QMessageBox::warning(this, "打开文件","选择的文件路径不能为空！");
        return;
    }
    ui->filePath->setText(path);
}

void MainWindow::on_sendFile_clicked()
{
    emit sendFile(ui->filePath->text());
}
