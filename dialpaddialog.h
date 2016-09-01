#ifndef DIALPADDIALOG_H
#define DIALPADDIALOG_H

#include <QDialog>

namespace Ui {
class DialPadDialog;
}

class DialPadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DialPadDialog(QWidget *parent = 0);
    ~DialPadDialog();

private:
    Ui::DialPadDialog *ui;
    QPushButton* m_num[10];
    QPushButton *m_backspace, *m_clear;

signals:
    void numClicked(int n);
};

#endif // DIALPADDIALOG_H
