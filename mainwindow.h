#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "appsettings.h"

namespace Ui {
class MainWindow;
}

class QInputDialog;
class QKeyEvent;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

protected slots:
    void _checkCredentials();
    void _loadFinished(bool ok);
    void _resetAuth();
    void _settingsChanged();

protected:
    void keyPressEvent(QKeyEvent* event);
    void ToggleFullView();
    
private slots:
    void on_btnRestart_clicked();

    void on_btnSettings_clicked();

private:
    Ui::MainWindow *ui;
    bool _locked;
    AppSettings _appSettings;
    QTimer _validityTimer;
    SettingsDialog* _settingsDialog;
};

#endif // MAINWINDOW_H
