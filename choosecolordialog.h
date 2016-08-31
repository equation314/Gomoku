#ifndef CHOOSECOLORDIALOG_H
#define CHOOSECOLORDIALOG_H

#include "const.h"

#include <QDialog>
#include <QTcpSocket>

namespace Ui
{
class ChooseColorDialog;
}

class ChooseColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseColorDialog(const QString& username, Const::NetworkIdentity type, QTcpSocket* socket, QWidget *parent = 0);
    ~ChooseColorDialog();

    Const::NetworkIdentity FirstPlayer() const { return m_first_palyer; }

private:
    Ui::ChooseColorDialog *ui;
    QString m_username;
    Const::NetworkIdentity m_type, m_first_palyer;
    QTcpSocket* m_socket;
    int m_state[2];

    void drawIcon();
    void changeInfo();
    void sendData(bool isOk);

private slots:
    void on_pushButton_first_clicked(bool checked);
    void on_pushButton_second_clicked(bool checked);

    void onReceivedData();
    void on_pushButton_start_clicked();
};

#endif // CHOOSECOLORDIALOG_H
