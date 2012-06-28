#include "appsettings.h"

AppSettings::AppSettings()
{
    //QString host = _settings.value("host").toString();
    //qDebug() << "got host" << host;
}

QString AppSettings::host() const
{
    QString host = _settings.value("host").toString();
    if (host.at(host.length() - 1) != QChar('/'))
        host += "/";

    return host;
}

QString AppSettings::user() const
{
    return _settings.value("user").toString();
}

void AppSettings::setUser(const QString& user)
{
    _settings.setValue("user", user);
}

QString AppSettings::password() const
{
    QString pass = _decode(_settings.value("password").toString());
    return pass;
}

void AppSettings::setPassword(const QString& pass)
{
    QString encodedPass = _encode(pass);
    _settings.setValue("password", encodedPass);
}

QString AppSettings::unlockPassword()
{
    return _decode(_settings.value("ath").toString());
}

void AppSettings::setUnlockPassword(const QString& pass)
{
    _settings.setValue("ath", _encode(pass));
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

