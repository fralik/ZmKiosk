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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _locked(true)
{
    ui->setupUi(this);
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
    ui->webView->load(QUrl(_appSettings.host()));

    _validityTimer.setSingleShot(true);
    _validityTimer.setInterval(300000); // 5 minutes
    connect(&_validityTimer, SIGNAL(timeout()), this, SLOT(_resetAuth()));

    ui->webView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_checkCredentials()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Unlock"), tr("Password:"),
                                         QLineEdit::Password, QString(), &ok);
    if (ok && !text.isEmpty())
    {
        if (text == _appSettings.unlockPassword())
        {
            _locked = false;
            ToggleFullView();
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

            QString cycleUrl = _appSettings.host() + "index.php?view=cycle_locked&group=0";
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
        _locked = false;
    }
    else
    {
        setWindowState(Qt::WindowFullScreen);
        _locked = true;
    }
}

void MainWindow::_resetAuth()
{
    _locked = true;
    ToggleFullView();
}

void MainWindow::on_btnRestart_clicked()
{
    if (QMessageBox::question(this, QString::fromLocal8Bit("Перезагрузка компьютера"), QString::fromLocal8Bit("Уверены?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        system("sudo /sbin/reboot");
    }
}
