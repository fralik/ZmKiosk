#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "appsettings.h"

SettingsDialog::SettingsDialog(AppSettings* settings, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , _appSettings(*settings)
{
    ui->setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::showDialog()
{
    ui->edHost->setText(_appSettings.host());
    if (_appSettings.host().isEmpty())
        ui->edHost->setText("http://");

    ui->edUsername->setText(_appSettings.user());
    ui->edLockable->setChecked(_appSettings.isLockable());

    show();
}

void SettingsDialog::changeEvent(QEvent* e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsDialog::dialogAccepted()
{
    _appSettings.setHost(ui->edHost->text());

    _appSettings.setUser(ui->edUsername->text());

    _appSettings.setPassword(ui->edPassword->text());
    ui->edPassword->clear();

    _appSettings.setUnlockPassword(ui->edAuthPassword->text());
    ui->edAuthPassword->clear();

    _appSettings.setLockable(ui->edLockable->isChecked());

    emit settingsChanged();
}
