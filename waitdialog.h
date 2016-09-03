#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>

namespace Ui
{
class WaitDialog;
}

class WaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitDialog(QWidget *parent = 0);
    ~WaitDialog();

public slots:
   void onBackAllowed();
   void onBackDisallowed();

protected:
   void closeEvent(QCloseEvent* event) override;

private:
    Ui::WaitDialog *ui;
    bool m_allowed;

signals:
    void backAllowed();
    void backDisallowed();
};

#endif // WAITDIALOG_H
