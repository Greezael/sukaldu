#include "mainwindow.h"
#include <QApplication>


#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    createConnection();

    return a.exec();
}
