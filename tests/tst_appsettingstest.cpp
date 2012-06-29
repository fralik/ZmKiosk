#include <QtCore/QString>
#include <QtTest/QtTest>

#include "../appsettings.h"

class AppSettingsTest : public QObject
{
    Q_OBJECT
    
public:
    AppSettingsTest();
    
private Q_SLOTS:
    void hostSaving_data();
    void hostSaving();
};

AppSettingsTest::AppSettingsTest()
{
}

void AppSettingsTest::hostSaving_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("host_1") << "testhost.com/zm" << "http://testhost.com/zm/";
    QTest::newRow("host_2") << "http://testhost.com/zm" << "http://testhost.com/zm/";
    QTest::newRow("host_3") << "http://testhost.com/zm/" << "http://testhost.com/zm/";
}

void AppSettingsTest::hostSaving()
{
    AppSettings appSettings;
    QFETCH(QString, input);
    QFETCH(QString, output);

    appSettings.setHost(input);
    QString res = appSettings.host();
    QVERIFY(res == output);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("FralikEntertainment");
    QCoreApplication::setApplicationName("ZmKiosk");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

    AppSettingsTest tc;
    QTest::qExec(&tc, argc, argv);

    return 0;
}


#include "tst_appsettingstest.moc"
