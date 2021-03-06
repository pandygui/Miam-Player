#ifndef SETTINGSPRIVATE_H
#define SETTINGSPRIVATE_H

#include <QFileInfo>
#include <QPushButton>
#include <QSettings>
#include <QTranslator>
#include "plugininfo.h"

#include "miamcore_global.h"

/**
 * \brief		SettingsPrivate class contains all relevant pairs of (keys, values) used by Miam-Player.
 * \details		This class implements the Singleton pattern. Instead of using standard "this->value(QString)", lots of methods
 *				are built on-top of it. It keeps the code easy to read and some important enums are shared between plugins too.
 * \author      Matthieu Bachelier
 * \copyright   GNU General Public License v3
 */
class MIAMCORE_LIBRARY SettingsPrivate : public QSettings
{
	Q_OBJECT
private:
	/** The unique instance of this class. */
	static SettingsPrivate *settings;

	/** Private constructor. */
	SettingsPrivate(const QString &organization = "MmeMiamMiam",
			 const QString &application = "MiamPlayer");

	/** Store the size of each font used in the app. */
	QMap<QString, QVariant> fontPointSizeMap;

	/** Store the family of each font used in the app. */
	QMap<QString, QVariant> fontFamilyMap;

	Q_ENUMS(DragDropAction)
	Q_ENUMS(FontFamily)
	Q_ENUMS(InsertPolicy)
	Q_ENUMS(PlaylistDefaultAction)
	Q_ENUMS(LibrarySearchMode)

public:
	enum DragDropAction { DD_OpenPopup		= 0,
						  DD_AddToLibrary	= 1,
						  DD_AddToPlaylist	= 2};

	enum FontFamily { FF_Playlist	= 0,
					  FF_Library	= 1,
					  FF_Menu		= 2};

	enum InsertPolicy { IP_Artists			= 0,
						IP_Albums			= 1,
						IP_ArtistsAlbums	= 2,
						IP_Years			= 3};

	enum PlaylistDefaultAction { PL_AskUserForAction	= 0,
								 PL_SaveOnClose			= 1,
								 PL_DiscardOnClose		= 2};

	enum LibrarySearchMode { LSM_Filter			= 0,
							 LSM_HighlightOnly	= 1};

	QTranslator customTranslator, defaultQtTranslator;

	/** Singleton Pattern to easily use Settings everywhere in the app. */
	static SettingsPrivate* instance();

	/** Add an activated plugin to the application. */
	void addPlugin(const PluginInfo &plugin);

	/** Disable a previously registered plugin (so it still can be listed in options). */
	void disablePlugin(const QString &absFilePath);

	qreal bigCoverOpacity() const;

	/** Returns the actual size of media buttons. */
	int buttonsSize() const;

	/** Returns true if the background color in playlist is using alternatative colors. */
	bool colorsAlternateBG() const;

	bool copyTracksFromPlaylist() const;

	/** Returns the size of a cover. */
	int coverSize() const;

	QColor customColors(QPalette::ColorRole cr) const;

	/** Custom icons in Customize(Theme) */
	const QString customIcon(const QString &buttonName) const;

	QString defaultLocationFileExplorer() const;

	DragDropAction dragDropAction() const;

	/** Returns the font of the application. */
	QFont font(const FontFamily fontFamily);

	/** Sets the font of the application. */
	int fontSize(const FontFamily fontFamily);

	/** Custom icons in CustomizeTheme */
	bool hasCustomIcon(const QString &buttonName) const;

	InsertPolicy insertPolicy() const;

	/** Returns true if big and faded covers are displayed in the library when an album is expanded. */
	bool isBigCoverEnabled() const;

	/** Returns true if covers are displayed in the library. */
	bool isCoversEnabled() const;

	bool isCustomColors() const;

	bool isExtendedSearchVisible() const;

	/** Returns true if background process is active to keep library up-to-date. */
	bool isFileSystemMonitored() const;

	/** Returns the hierarchical order of the library tree view. */
	bool isLibraryFilteredByArticles() const;

	/** Returns true if the button in parameter is visible or not. */
	bool isMediaButtonVisible(const QString & buttonName) const;

	bool isPlaylistResizeColumns() const;

	/** Returns true if tabs should be displayed like rectangles. */
	bool isRectTabs() const;

	/** Returns true if the article should be displayed after artist's name. */
	bool isReorderArtistsArticle() const;

	/** Returns true if star outline must be displayed in the library. */
	bool isShowNeverScored() const;

	/** Returns true if stars are visible and active. */
	bool isStarDelegates() const;

	/** Returns true if a user has modified one of defaults theme. */
	bool isButtonThemeCustomized() const;

	/** Returns true if the volume value in percent is always visible in the upper left corner of the widget. */
	bool isVolumeBarTextAlwaysVisible() const;

	/** Returns the language of the application. */
	QString language();

	/** Returns the last active playlist header state. */
	QByteArray lastActivePlaylistGeometry() const;

	/** Returns the last playlists that were opened when player was closed. */
	QList<uint> lastPlaylistSession() const;

	QStringList libraryFilteredByArticles() const;

	LibrarySearchMode librarySearchMode() const;

	/** Returns all music locations. */
	QStringList musicLocations() const;

	int tabsOverlappingLength() const;

	/// PlayBack options
	qint64 playbackSeekTime() const;

	/** Default action to execute when one is closing a playlist. */
	PlaylistDefaultAction playbackDefaultActionForClose() const;

	/** Automatically save all playlists before exit. */
	bool playbackKeepPlaylists() const;

	/** Automatically restore all saved playlists at startup. */
	bool playbackRestorePlaylistsAtStartup() const;

	QMap<QString, PluginInfo> plugins() const;

	void setCustomColorRole(QPalette::ColorRole cr, const QColor &color);

	/** Custom icons in CustomizeTheme */
	void setCustomIcon(const QString &buttonName, const QString &iconPath);

	/** Sets the language of the application. */
	bool setLanguage(const QString &lang);

	/** Sets the last playlists that were opened when player is about to close. */
	void setLastPlaylistSession(const QList<uint> &ids);

	void setMusicLocations(const QStringList &locations);

	void setShortcut(const QString &objectName, const QKeySequence &keySequence);

	QKeySequence shortcut(const QString &objectName) const;

	QMap<QString, QVariant> shortcuts() const;

	int volumeBarHideAfter() const;

private:
	bool initLanguage(const QString &lang);

	void initShortcuts();

public:
	void setDefaultLocationFileExplorer(const QString &location);

	/** Define the hierarchical order of the library tree view. */
	void setInsertPolicy(InsertPolicy ip);

public slots:
	void addMusicLocations(const QList<QDir> &dirs);

	void setBigCoverOpacity(int v);

	void setBigCovers(bool b);

	/** Sets a new button size. */
	void setButtonsSize(const int &s);

	/// Colors
	void setColorsAlternateBG(bool b);

	void setCopyTracksFromPlaylist(bool b);

	void setCovers(bool b);
	void setCoverSize(int s);

	void setCustomColors(bool b);

	/** Sets if stars are visible and active. */
	void setDelegates(const bool &value);

	void setDragDropAction(DragDropAction action);

	void setExtendedSearchVisible(bool b);

	/** Sets the font of a part of the application. */
	void setFont(const FontFamily &fontFamily, const QFont &font);

	/** Sets the font size of a part of the application. */
	void setFontPointSize(const FontFamily &fontFamily, int i);

	void setIsLibraryFilteredByArticles(bool b);

	/** Save the last active playlist header state. */
	void setLastActivePlaylistGeometry(const QByteArray &);

	void setLibraryFilteredByArticles(const QStringList &tagList);

	/** Sets if the button in parameter is visible or not. */
	void setMediaButtonVisible(const QString & buttonName, const bool &value);

	/** Sets if MiamPlayer should launch background process to keep library up-to-date. */
	void setMonitorFileSystem(bool b);

	/// PlayBack options
	void setPlaybackSeekTime(int t);
	void setPlaybackCloseAction(PlaylistDefaultAction action);
	void setPlaybackKeepPlaylists(bool b);
	void setPlaybackRestorePlaylistsAtStartup(bool b);

	void setReorderArtistsArticle(bool b);

	void setSearchAndExcludeLibrary(bool b);
	void setShowNeverScored(bool b);

	void setTabsOverlappingLength(int l);

	void setTabsRect(bool b);

	void setButtonThemeCustomized(bool b);

	void setVolumeBarHideAfter(int seconds);
	void setVolumeBarTextAlwaysVisible(bool b);

signals:
	void fontHasChanged(FontFamily, const QFont &font);

	void librarySearchModeHasChanged();

	/** Signal sent whether the music locations have changed or not. */
	void musicLocationsHaveChanged(const QStringList &oldLocations, const QStringList &newLocations);
};

Q_DECLARE_METATYPE(QPalette::ColorRole)

#endif // SETTINGSPRIVATE_H
