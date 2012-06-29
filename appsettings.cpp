#include "appsettings.h"

AppSettings::AppSettings()
{
    //QString host = _settings.value("host").toString();
    //qDebug() << "got host" << host;
}

QString AppSettings::host() const
{
    QString host = _settings.value("host").toString();
    if (!host.isEmpty() && (host.at(host.length() - 1) != QChar('/')))
        host += "/";

    return host;
}

void AppSettings::setHost(const QString& host)
{
    if (host.isEmpty())
        return;

    QString actualHost = host;
    if (!actualHost.contains("http://") && !actualHost.contains("https://"))
    {
        actualHost = "http://" + actualHost;
    }
    _settings.setValue("host", actualHost);
}

QString AppSettings::user() const
{
    return _settings.value("user").toString();
}

void AppSettings::setUser(const QString& user)
{
    if (user.isEmpty())
        return;

    _settings.setValue("user", user);
}

QString AppSettings::password() const
{
    return _decode(_settings.value("password").toString());
}

void AppSettings::setPassword(const QString& pass)
{
    if (pass.isEmpty())
        return;

    QString encodedPass = _encode(pass);
    _settings.setValue("password", encodedPass);
}

QString AppSettings::unlockPassword() const
{
    return _decode(_settings.value("ath").toString());
}

void AppSettings::setUnlockPassword(const QString& pass)
{
    if (pass.isEmpty())
        return;

    _settings.setValue("ath", _encode(pass));
}

bool AppSettings::isLockable() const
{
    return _settings.value("lockable", true).toBool();
}

void AppSettings::setLockable(bool lockable)
{
    _settings.setValue("lockable", lockable);
}

bool AppSettings::isPresent() const
{
    bool hostIsEmpty = host().isEmpty();
    bool userIsEmpty = user().isEmpty();
    bool passwordIsEmpty = password().isEmpty();
    return !(hostIsEmpty || userIsEmpty || passwordIsEmpty);
}

QString AppSettings::_encode(const QString& str) const
{
    QByteArray arr(str.toUtf8());
    for (int i = 0; i < arr.size(); ++i)
    {
        arr[i] = arr[i] ^ _key;
    }

    return QString::fromAscii(arr.toBase64());
}

QString AppSettings::_decode(const QString& str) const
{
    QByteArray arr = QByteArray::fromBase64(str.toAscii());
    for(int i = 0; i < arr.size(); ++i)
    {
        arr[i] = arr[i] ^ _key;
    }

    return QString::fromUtf8(arr);
}

