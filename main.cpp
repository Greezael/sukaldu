#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLocale eslocale("es");
    QLocale::setDefault(eslocale);

    QString translationsPath(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QLocale locale = QLocale();

    QTranslator qtTranslator;
    if (qtTranslator.load(locale, "qt", "_", translationsPath))
        app.installTranslator(&qtTranslator);

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load(locale, "qtbase", "_", translationsPath))
        app.installTranslator(&qtBaseTranslator);

    QTranslator translator;
    if (QLocale().language() == QLocale::Spanish)
        if (translator.load("sukaldu_es", "translations"))
            app.installTranslator(&translator);

    if (!createConnection(&app))
        return false;

    MainWindow w;
    w.show();

    return app.exec();
}
