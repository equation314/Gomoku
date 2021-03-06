#include "connectdialog.h"
#include "ui_connectdialog.h"

#include <QTcpServer>
#include <QMessageBox>
#include <QTranslator>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    m_type(Const::Server),
    m_dialog(nullptr)
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



void ConnectDialog::onDialPadNumClicked(int n)
{
    QLineEdit* lineEdit = static_cast<QLineEdit*>(this->focusWidget());
    if (lineEdit && lineEdit->inherits("QLineEdit"))
    {
        if (n >= 0)
            lineEdit->insert(QString::number(n));
        else if (n == -1)
            lineEdit->backspace();
        else if (n == -2)
            lineEdit->clear();
        this->activateWindow();
    }
}

void ConnectDialog::on_lineEdit_0_textChanged(const QString &arg1)
{
    if (arg1.toInt() > 25)
    {
        ui->lineEdit_1->setFocus();
        ui->lineEdit_1->selectAll();
    }
}

void ConnectDialog::on_lineEdit_1_textChanged(const QString &arg1)
{
    if (arg1.toInt() > 25)
    {
        ui->lineEdit_2->setFocus();
        ui->lineEdit_2->selectAll();
    }
}

void ConnectDialog::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (arg1.toInt() > 25)
    {
        ui->lineEdit_3->setFocus();
        ui->lineEdit_3->selectAll();
    }
}

void ConnectDialog::on_lineEdit_3_textChanged(const QString &arg1)
{
    if (arg1.toInt() > 25)
    {
        ui->lineEdit_port->setFocus();
        ui->lineEdit_port->selectAll();
    }
}

void ConnectDialog::on_radioButton_server_clicked()
{
    m_type = Const::Server;
    this->setWindowTitle(tr("Create Server"));
    ui->label->setText(tr("Local &IP:"));
    ui->pushButton_create->setText(tr("&Create"));

    QStringList ip = Const::GetLocalIp().split('.');
    if (ip.size() != 4)
    {
        QMessageBox::critical(this, tr("Invalid IP Address"), tr("Cannot get valid IP address.\nPlease check the network connection."));
        qApp->quit();
        return;
    }
    ui->lineEdit_0->setText(ip[0]);
    ui->lineEdit_1->setText(ip[1]);
    ui->lineEdit_2->setText(ip[2]);
    ui->lineEdit_3->setText(ip[3]);

    ui->lineEdit_port->setFocus();
    ui->lineEdit_port->selectAll();
}

void ConnectDialog::on_radioButton_client_clicked()
{
    m_type = Const::Client;
    this->setWindowTitle(tr("Connect to Server"));
    ui->label->setText(tr("Server &IP:"));
    ui->pushButton_create->setText(tr("&Connect"));

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
            QMessageBox::critical(this, tr("Create Server Failed"), QString(tr("Cannot create the server at %1:%2")).arg(m_ip).arg(m_port));
            ui->lineEdit_port->setFocus();
            return;
        }
    }
    if (m_dialog) m_dialog->hide();
    this->accept();
}

void ConnectDialog::on_pushButton_dial_pad_clicked()
{
    ui->pushButton_dial_pad->setEnabled(false);
    m_dialog = new DialPadDialog(this);
    connect(m_dialog, &DialPadDialog::numClicked, this, &ConnectDialog::onDialPadNumClicked);
    connect(m_dialog, &DialPadDialog::rejected, this, [this]()
    {
        ui->pushButton_dial_pad->setEnabled(true);
        m_dialog->deleteLater();
    });
    m_dialog->show();
}
