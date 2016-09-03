#include "mainwindow.h"
#include "connectdialog.h"

#include <QTranslator>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator trans1, trans2;
    trans1.load(":/trans/trans/zh_CN.qm");
    trans2.load(":/trans/trans/qt_zh_CN.qm");
    qApp->installTranslator(&trans1);
    qApp->installTranslator(&trans2);

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
        w.show();

        if (a.exec() != 233) break;
    }
    return 0;
}
