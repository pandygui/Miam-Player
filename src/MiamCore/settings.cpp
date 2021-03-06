#include "settings.h"

#include <QDateTime>
#include <QFile>
#include <QApplication>
#include <QGuiApplication>
#include <QHeaderView>
#include <QScrollBar>
#include <QTabWidget>

#include <QtDebug>

Settings* Settings::settings = nullptr;

/** Private constructor. */
Settings::Settings(const QString &organization, const QString &application)
	: QSettings(IniFormat, UserScope, organization, application)
{}

/** Singleton pattern to be able to easily use settings everywhere in the app. */
Settings* Settings::instance()
{
	if (settings == nullptr) {
		settings = new Settings;
	}
	return settings;
}

QString Settings::lastActiveView() const
{
	return value("lastActiveView", "actionViewPlaylists").toString();
}

/** Returns the actual theme name. */
QString Settings::theme() const
{
	return value("theme", "oxygen").toString();
}

/** Returns volume from the slider. */
qreal Settings::volume() const
{
	return value("volume", 0.9).toReal();
}

/// Slots

/** Sets the last view activated by the user. Used when reopening the player. */
void Settings::setLastActiveView(const QString &viewName)
{
	setValue("lastActiveView", viewName);
}

void Settings::setThemeName(const QString &theme)
{
	setValue("theme", theme.toLower());
	emit themeHasChanged();
}


void Settings::setVolume(qreal v)
{
	setValue("volume", v);
}
