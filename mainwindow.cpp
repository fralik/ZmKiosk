#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QUrl>
#include <QWebFrame>
#include <QWebElement>
#include <QKeyEvent>
#include <QMessageBox>

#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _locked(true)
    , _settingsDialog(NULL)
{
    ui->setupUi(this);
    setGeometry(50, 50, 800, 600);
    setWindowState(Qt::WindowFullScreen);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavaEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(_loadFinished(bool)));

    if (_appSettings.isPresent())
    {
        ui->webView->load(QUrl(_appSettings.host()));
        ui->webView->show();

        if (_appSettings.isLockable())
        {
            ui->btnSettings->hide();
        }
    }
    else
    {
        _locked = false;
        on_btnSettings_clicked();
    }

    _validityTimer.setSingleShot(true);
    _validityTimer.setInterval(300000); // 5 minutes
    connect(&_validityTimer, SIGNAL(timeout()), this, SLOT(_resetAuth()));

    ui->webView->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_checkCredentials()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("#unlock_title"), tr("#pwd_prompt"),
                                         QLineEdit::Password, QString(), &ok);
    if (ok && !text.isEmpty())
    {
        if (text == _appSettings.unlockPassword())
        {
            _locked = false;
            ui->btnSettings->show();

            ToggleFullView();
            if (_appSettings.isLockable())
                _validityTimer.start();
        }
    }
}

void MainWindow::_loadFinished(bool ok)
{
    if (ok)
    {
        QString indexUrl = _appSettings.host();
        indexUrl += "index.php";

        QString pageUrl = ui->webView->url().toString();
        QWebPage* page = ui->webView->page();
        QWebFrame* frame = page->currentFrame();
        QWebElement root = frame->documentElement();
        if (!root.findFirst("#loginForm").isNull())
        {
            QWebElementCollection allInputs = root.findAll("input");
            foreach (QWebElement inputElement, allInputs)
            {
                if (inputElement.hasAttribute("name"))
                {
                    QString name = inputElement.attribute("name");
                    if (name == "username")
                    {
                        inputElement.setAttribute("value", _appSettings.user());
                    }
                    else if (name == "password")
                    {
                        inputElement.setAttribute("value", _appSettings.password());
                    }
                }
            }

            root.findFirst("input[type=submit]").evaluateJavaScript("this.click()");
        }
        else if (pageUrl == indexUrl)
        {
            disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(_loadFinished(bool)));

            QString cycleUrl = _appSettings.host() + "index.php?view=montage_locked&group=0";
            ui->webView->load(cycleUrl);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
//    int dwModifier = event->modifiers();
//    if (event->key() == Qt::Key_F11 && dwModifier == Qt::NoModifier)
//    {
//        ToggleFullView();
//        return;
//    }
//    else
//    {
        if (!_locked)
        {
            return QMainWindow::keyPressEvent(event);
        }

        _checkCredentials();
//    }
}

void MainWindow::ToggleFullView()
{
    if (windowState() == Qt::WindowFullScreen)
    {
        setWindowState(Qt::WindowNoState);
    }
    else
    {
        setWindowState(Qt::WindowFullScreen);
    }
}

void MainWindow::_resetAuth()
{
    _locked = true;
    ToggleFullView();
    ui->btnSettings->hide();
}

void MainWindow::_settingsChanged()
{
    if (_appSettings.isPresent())
    {
        qDebug() << "Settings changed, going to load" << _appSettings.host();
        ui->webView->load(QUrl(_appSettings.host()));
        ui->webView->show();

        _locked = _appSettings.isLockable();
        ui->btnSettings->setVisible(!_locked);
    }
    else
    {
        on_btnSettings_clicked();
    }
}

void MainWindow::on_btnRestart_clicked()
{
    if (QMessageBox::question(this, tr("#reboot"), tr("#reboot_confirmation_question"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        system("sudo /sbin/reboot");
    }
}

void MainWindow::on_btnSettings_clicked()
{
    if (!_settingsDialog)
    {
        _settingsDialog = new SettingsDialog(&_appSettings, this);
        connect(_settingsDialog, SIGNAL(settingsChanged()), this, SLOT(_settingsChanged()));
    }

    _settingsDialog->showDialog();
}
