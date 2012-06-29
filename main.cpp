#include <QtGui/QApplication>
#include <QtCore/QSettings>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("FralikEntertainment");
    QCoreApplication::setApplicationName("ZmKiosk");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    AppSettings settings;
    for (int i = 1; i < argc; ++i)
    {
        QString arg = argv[i];
        if (arg == "--pass" || arg == "-p")
        {
            // encode password and store it in the settings
            QString pass = argv[i+1];
            settings.setPassword(pass);

            return 0;
        }
        else if (arg == "--auth" || arg == "-a")
        {
            // encode unlock password
            QString pass = argv[i+1];
            settings.setUnlockPassword(pass);

            return 0;
        }
    }

    //QString locale = QLocale::system().name();
    QString locale = "ru_RU";
    QTranslator qtTranslator;
    QString qtFilename = "qt_" + locale;
    QString appTranslationPath = a.applicationDirPath() + "/translations";
    if (!qtTranslator.load(qtFilename, appTranslationPath))
    {
        qDebug() << "Failed to load translation" << qtFilename << "from " << appTranslationPath;
        if (!qtTranslator.load(qtFilename, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            qDebug() << "Failed to load translation" << qtFilename << "from " << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
        }
    }
    a.installTranslator(&qtTranslator);

    QTranslator translator;
    QString appFilename = "zmkiosk_" + locale;
    if (!translator.load(appFilename, appTranslationPath))
    {
        qDebug() << "Failed to load translation" << appFilename << "from " << appTranslationPath;
    }
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    
    return a.exec();
}
