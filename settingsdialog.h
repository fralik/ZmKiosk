#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
//#include <appsettings.h>

namespace Ui {
class SettingsDialog;
}

class QEvent;
class AppSettings;

class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    SettingsDialog(AppSettings* settings, QWidget* parent = 0);
    ~SettingsDialog();

    void showDialog();

signals:
    void settingsChanged();
    
protected:
    void changeEvent(QEvent* e);

private slots:
    void dialogAccepted();

private:
    Ui::SettingsDialog *ui;
    AppSettings& _appSettings;
};

#endif // SETTINGSDIALOG_H
