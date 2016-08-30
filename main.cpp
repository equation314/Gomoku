#include "mainwindow.h"
#include "connectdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<Pieces::PiecesColor>("Pieces::PiecesType");

    ConnectDialog dialog;
    if (dialog.exec() != QDialog::Accepted) return 0;

    MainWindow w(dialog.Type(), dialog.Ip(), dialog.Port());
    w.show();

    return a.exec();
}
