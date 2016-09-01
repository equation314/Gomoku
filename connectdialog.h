#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

#include "const.h"
#include "dialpaddialog.h"

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

    Const::HostType Type() const { return m_type; }
    QString Ip() const { return m_ip; }
    int Port() const { return m_port; }
    QString Username() const { return m_username; }

private slots:
    void onDialPadNumClicked(int n);

    void on_lineEdit_0_textChanged(const QString &arg1);
    void on_lineEdit_1_textChanged(const QString &arg1);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_radioButton_server_clicked();
    void on_radioButton_client_clicked();
    void on_pushButton_create_clicked();
    void on_pushButton_dial_pad_clicked();


private:
    Ui::ConnectDialog *ui;
    Const::HostType m_type;
    QString m_ip, m_username;
    int m_port;
    DialPadDialog* m_dialog;
};

#endif // CONNECTDIALOG_H
