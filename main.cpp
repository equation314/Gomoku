#include "mainwindow.h"
#include "connectdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<Pieces::PiecesColor>("Pieces::PiecesColor");

    for (;;)
    {
        ConnectDialog dialog;
        if (dialog.exec() != QDialog::Accepted)
        {
            dialog.deleteLater();
            return 0;
        }

        MainWindow w(dialog.Username(), dialog.Type(), dialog.Ip(), dialog.Port());
        //w.show();

        if (a.exec() != 233) break;
    }
    return 0;
}
