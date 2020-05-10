#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

#define CONFIGURATION_FILE "configurations.conf"

class Settings
{
public:
    Settings(QString settingsFileName);
    Settings();
    ~Settings();

private:
    QSettings *settings;

};

#endif // SETTINGS_H
