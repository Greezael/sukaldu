#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QInputDialog>
#include <QSettings>


#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSettings settings("Sukaldu-dev", "Sukaldu");

    QString localeString = QLocale::languageToString(QLocale().language());

    if (settings.value("locale").isNull())
    {
        QStringList items;
        QStringList codes;
        items << "Castellano" << "English";
        codes << "es" << "en";

        bool ok;
        QString item = QInputDialog::getItem(nullptr, "QInputDialog::getItem()",
                                             "Language", items, 0, false, &ok);

        if (ok && !item.isEmpty())
        {
            for (int i = 0; i < items.length(); i++)
            {
                if (item.compare(items.at(i)) == 0)
                {
                    localeString = codes.at(i);
                    break;
                }
            }
        }

        settings.setValue("locale", localeString);
    }
    else
    {
        localeString = settings.value("locale").toString();
    }

    QString translationsPath(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QLocale locale = QLocale(localeString);

    QTranslator qtTranslator;
    if (qtTranslator.load(locale, "qt", "_", translationsPath))
        app.installTranslator(&qtTranslator);

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load(locale, "qtbase", "_", translationsPath))
        app.installTranslator(&qtBaseTranslator);

    QTranslator translator;
    if (locale.language() == QLocale::Spanish)
        if (translator.load("sukaldu_es", "translations"))
            app.installTranslator(&translator);

    if (settings.value("currency").isNull() ||
            settings.value("currencyId").isNull())
    {
        settings.setValue("currency", "€");
        settings.setValue("currencyId", 1);
    }

    if (!createConnection(&app))
        return false;

    MainWindow w;
    w.show();

    return app.exec();
}
