#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

#include "const.h"

namespace Ui
{
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

    Const::NetworkIdentity Type() const { return m_type; }
    QString Ip() const { return m_ip; }
    int Port() const { return m_port; }
    QString Username() const { return m_user; }

private slots:
    void on_lineEdit_0_textChanged(const QString &arg1);
    void on_lineEdit_1_textChanged(const QString &arg1);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_radioButton_server_clicked();
    void on_radioButton_client_clicked();
    void on_pushButton_create_clicked();

private:
    Ui::ConnectDialog *ui;
    Const::NetworkIdentity m_type;
    QString m_local_ip, m_ip, m_user;
    int m_port;

};

#endif // CONNECTDIALOG_H
