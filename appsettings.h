#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>

/**
  * Wrapper for application settings
  */
class AppSettings
{
public:
    AppSettings();

    QString host() const;
    void setHost(const QString& host);

    QString user() const;
    void setUser(const QString& user);

    QString password() const;
    void setPassword(const QString& pass);

    QString unlockPassword() const;
    void setUnlockPassword(const QString& pass);

    /**
      * Lockable status. If TRUE the app can be locked/unlocked
      * and it will be locked after some time of inactivity.
      * If FALSE the app will not be locked.
      */
    bool isLockable() const;
    void setLockable(bool lockable);

    bool isPresent() const;

private:
    QString _encode(const QString& str) const;
    QString _decode(const QString& str) const;

    QSettings _settings;
    static const int _key = 47;
};

#endif // APPSETTINGS_H
