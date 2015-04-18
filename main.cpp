#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

#include "connection.h"

int main(int argc, char *argv[])
{
//    QLocale locale("es_ES");
//    QLocale::setDefault(locale);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale().name(),
               QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    QTranslator translator;

    if (QLocale().language() == QLocale::Spanish)
    {
        translator.load("sukaldu_es", "translations");
    }

    QApplication app(argc, argv);
    app.installTranslator(&qtTranslator);
    app.installTranslator(&translator);

    if (!createConnection(&app))
        return false;

    MainWindow w;
    w.show();

    return app.exec();
}
