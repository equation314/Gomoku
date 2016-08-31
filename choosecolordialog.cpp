#include "choosecolordialog.h"
#include "ui_choosecolordialog.h"

#include <QPainter>

ChooseColorDialog::ChooseColorDialog(const QString& username, Const::NetworkIdentity type, QTcpSocket* socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseColorDialog),
    m_username(username), m_type(type), m_socket(socket)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);

    drawIcon();
    this->setFixedHeight(this->sizeHint().height());
    if (type == Const::Client) ui->pushButton_start->hide();
    m_state[0] = m_state[1] = -1;

    connect(m_socket, &QTcpSocket::readyRead, this, &ChooseColorDialog::onReceivedData);
}

ChooseColorDialog::~ChooseColorDialog()
{
    delete ui;
}

void ChooseColorDialog::drawIcon()
{
    QPixmap pixmap(24, 24);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(12, 12), 10, 10);
    ui->pushButton_first->setIcon(pixmap);

    pixmap.fill(Qt::transparent);
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPoint(12, 12), 10, 10);
    ui->pushButton_second->setIcon(pixmap);
}

void ChooseColorDialog::changeInfo()
{
    qDebug()<<m_state[0]<<' '<<m_state[1];
    if (ui->pushButton_first->isChecked() && ui->pushButton_second->isChecked())
    {
        if (m_type == Const::Server)
        {
            ui->pushButton_start->setEnabled(true);
            ui->label_info->setText(tr("Press the start button to start"));
        }
        else
            ui->label_info->setText(tr("Waiting for the server to start..."));
    }
    else
    {
        ui->pushButton_start->setEnabled(false);
        if (m_state[0] != (int)m_type && m_state[1] != (int)m_type)
            ui->label_info->setText(tr("Please choose one color"));
        else
            ui->label_info->setText(tr("Waiting for the opponent to choose..."));
    }
}

void ChooseColorDialog::sendData(bool isOk)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("TYPE") << QString("ChooseColor");
    out << isOk << m_state[0] << m_state[1] << m_username;
    m_socket->write(array);
}

void ChooseColorDialog::onReceivedData()
{
    QDataStream in(m_socket);
    int state0, state1;
    bool isOk;
    QString head, type, opp_user;
    in >> head >> type;
    if (head != "TYPE" || type != "ChooseColor") return;

    in >> isOk >> state0 >> state1 >> opp_user;

    if (m_type == Const::Server && ((state0 != m_state[0] && state0 >= 0 && m_state[0] >= 0)
                                ||  (state1 != m_state[1] && state1 >= 0 && m_state[1] >= 0)))
        sendData(isOk);
    else
    {
        m_state[0] = state0;
        m_state[1] = state1;
        if (state0 >= 0)
        {
            ui->label_user0->setText(state0 == (int)m_type ? m_username : opp_user);
            ui->label_user0_arrow->setText("▼");
            ui->pushButton_first->setChecked(true);
        }
        else
        {
            ui->label_user0->clear();
            ui->label_user0_arrow->clear();
            ui->pushButton_first->setChecked(false);
        }
        if (state1 >= 0)
        {
            ui->label_user1->setText(state1 == (int)m_type ? m_username : opp_user);
            ui->label_user1_arrow->setText("▼");
            ui->pushButton_second->setChecked(true);
        }
        else
        {
            ui->label_user1->clear();
            ui->label_user1_arrow->clear();
            ui->pushButton_second->setChecked(false);
        }
        if (ui->label_user0->text() == opp_user)
        {
            ui->pushButton_first->setEnabled(false);
            ui->label_user0->setStyleSheet("color:rgb(180,180,180);");
            ui->label_user0_arrow->setStyleSheet("color:rgb(180,180,180);");
        }
        else
        {
            ui->pushButton_first->setEnabled(true);
            ui->label_user0->setStyleSheet("color:black;");
            ui->label_user0_arrow->setStyleSheet("color:black;");
        }
        if (ui->label_user1->text() == opp_user)
        {
            ui->pushButton_second->setEnabled(false);
            ui->label_user1->setStyleSheet("color:rgb(180,180,180);");
            ui->label_user1_arrow->setStyleSheet("color:rgb(180,180,180);");
        }
        else
        {
            ui->pushButton_second->setEnabled(true);
            ui->label_user1->setStyleSheet("color:black;");
            ui->label_user1_arrow->setStyleSheet("color:black;");
        }
    }
    changeInfo();
    if (isOk)
    {
        m_first_palyer = (Const::NetworkIdentity)m_state[0];
        this->accept();
    }
}

void ChooseColorDialog::on_pushButton_first_clicked(bool checked)
{
    if (checked)
    {
        m_state[0] = m_type;
        ui->label_user0->setText(m_username);
        ui->label_user0_arrow->setText("▼");
        if (m_state[1] == m_type)
        {
            m_state[1] = -1;
            ui->label_user1->clear();
            ui->label_user1_arrow->clear();
            ui->pushButton_second->setChecked(false);
        }
        if (ui->pushButton_second->isChecked() && m_type == Const::Server)
            ui->pushButton_start->setEnabled(true);
    }
    else
    {
        m_state[0] = -1;
        ui->label_user0->clear();
        ui->label_user0_arrow->clear();
        ui->pushButton_start->setEnabled(false);
    }
    changeInfo();
    sendData(false);
}

void ChooseColorDialog::on_pushButton_second_clicked(bool checked)
{
    if (checked)
    {
        m_state[1] = m_type;
        ui->label_user1->setText(m_username);
        ui->label_user1_arrow->setText("▼");
        if (m_state[0] == m_type)
        {
            m_state[0] = -1;
            ui->label_user0->clear();
            ui->label_user0_arrow->clear();
            ui->pushButton_first->setChecked(false);
        }
        if (ui->pushButton_second->isChecked() && m_type == Const::Server)
            ui->pushButton_start->setEnabled(true);
    }
    else
    {
        m_state[1] = -1;
        ui->label_user1->clear();
        ui->label_user1_arrow->clear();
        ui->pushButton_start->setEnabled(false);
    }
    changeInfo();
    sendData(false);
}

void ChooseColorDialog::on_pushButton_start_clicked()
{
    m_first_palyer = (Const::NetworkIdentity)m_state[0];
    sendData(true);
    this->accept();
}
