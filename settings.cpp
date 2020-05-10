#include "settings.h"

Settings::Settings(QString settingsFileName) {
    settings = new QSettings(settingsFileName, QSettings::IniFormat);

}

Settings::Settings() {
    settings = new QSettings(CONFIGURATION_FILE, QSettings::IniFormat);

}

Settings::~Settings(){
    delete settings;
}
