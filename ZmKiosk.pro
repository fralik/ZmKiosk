#-------------------------------------------------
#
# Project created by QtCreator 2012-06-06T16:42:35
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = ZmKiosk
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    appsettings.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    appsettings.h \
    settingsdialog.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
TRANSLATIONS    += translations/zmkiosk_ru.ts \
                    translations/zmkiosk_en.ts
OTHER_FILES += translations/zmkiosk_ru.qm \
                translations/zmkiosk_en.qm \
                translations/qt_ru.qm \
                translations/qt_ru.ts

bundle.files = translations/qt_ru.qm translations/zmkiosk_ru.qm translations/zmkiosk_en.qm
#bundle.path = C:/home/projects/ZmKiosk-build-desktop-Qt_4_8_1_for_Desktop_-_MSVC2010__Qt_SDK__Debug/debug/translations
bundle.path = $${DESTDIR}/translations
INSTALLS    += bundle
DEPLOYMENT += bundle