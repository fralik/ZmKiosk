#include <QtGui/QApplication>
#include <QtCore/QSettings>
//#include <QTranslator>
//#include <QLocale>
#include <QTextCodec>

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

    //QTranslator translator;
    //translator.load("zmkiosk_" + QLocale::system().name(), ":/translations");
    //a.installTranslator(());

    MainWindow w;
    w.show();
    
    return a.exec();
}
