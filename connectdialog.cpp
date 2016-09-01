#include "connectdialog.h"
#include "ui_connectdialog.h"

#include <QDebug>
#include <QTcpServer>
#include <QMessageBox>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    m_type(Const::Server)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    QValidator* validator = new QIntValidator(0, 255, this);
    ui->lineEdit_0->setValidator(validator);
    ui->lineEdit_1->setValidator(validator);
    ui->lineEdit_2->setValidator(validator);
    ui->lineEdit_3->setValidator(validator);
    ui->lineEdit_port->setText("2333");
    ui->lineEdit_port->setValidator(new QIntValidator(0, 65535, this));
    this->setFixedSize(this->sizeHint());

    on_radioButton_server_clicked();
    ui->lineEdit_user->setFocus();
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::on_lineEdit_0_textChanged(const QString &arg1)
{
    if (!ui->lineEdit_0->isReadOnly() && arg1.toInt() > 25)
    {
        ui->lineEdit_1->setFocus();
        ui->lineEdit_1->selectAll();
    }
}

void ConnectDialog::on_lineEdit_1_textChanged(const QString &arg1)
{
    if (!ui->lineEdit_0->isReadOnly() && arg1.toInt() > 25)
    {
        ui->lineEdit_2->setFocus();
        ui->lineEdit_2->selectAll();
    }
}

void ConnectDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (!ui->lineEdit_0->isReadOnly() && arg1.toInt() > 25)
    {
        ui->lineEdit_3->setFocus();
        ui->lineEdit_3->selectAll();
    }
}

void ConnectDialog::on_lineEdit_3_textChanged(const QString &arg1)
{
    if (!ui->lineEdit_0->isReadOnly() && arg1.toInt() > 25)
    {
        ui->lineEdit_port->setFocus();
        ui->lineEdit_port->selectAll();
    }
}

void ConnectDialog::on_radioButton_server_clicked()
{
    m_type = Const::Server;
    this->setWindowTitle(tr("Create Server"));
    ui->label->setText(tr("Host &IP:"));
    ui->pushButton_create->setText(tr("&Create"));

    QStringList ip = Const::GetLocalIp().split('.');
    ui->lineEdit_0->setText(ip[0]);
    ui->lineEdit_1->setText(ip[1]);
    ui->lineEdit_2->setText(ip[2]);
    ui->lineEdit_3->setText(ip[3]);
    ui->lineEdit_0->setReadOnly(true);
    ui->lineEdit_1->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);

    ui->lineEdit_port->setFocus();
    ui->lineEdit_port->selectAll();
}

void ConnectDialog::on_radioButton_client_clicked()
{
    m_type = Const::Client;
    this->setWindowTitle(tr("Connect to Server"));
    ui->label->setText(tr("Server &IP:"));
    ui->pushButton_create->setText(tr("&Connect"));
    ui->lineEdit_0->setReadOnly(false);
    ui->lineEdit_1->setReadOnly(false);
    ui->lineEdit_2->setReadOnly(false);
    ui->lineEdit_3->setReadOnly(false);

    ui->lineEdit_0->setFocus();
    ui->lineEdit_0->selectAll();
}

void ConnectDialog::on_pushButton_create_clicked()
{
    if (ui->lineEdit_user->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Username Requested"), tr("Please input your username!"));
        ui->lineEdit_user->setFocus();
        return;
    }
    m_ip = QString("%1.%2.%3.%4").arg(ui->lineEdit_0->text())
                                 .arg(ui->lineEdit_1->text())
                                 .arg(ui->lineEdit_2->text())
                                 .arg(ui->lineEdit_3->text());
    m_port = ui->lineEdit_port->text().toInt();
    m_username = ui->lineEdit_user->text();
    if (m_type == Const::Server)
    {
        QTcpServer server;
        if (!server.listen(QHostAddress(m_ip), m_port))
        {
            QMessageBox::critical(this, tr("Create Server Failed"), QString(tr("Cannot listen the port %1.")).arg(m_port));
            ui->lineEdit_port->setFocus();
            return;
        }
    }
    this->accept();
}
