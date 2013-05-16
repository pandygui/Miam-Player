#ifndef PLAYBACKMODEWIDGETFACTORY_H
#define PLAYBACKMODEWIDGETFACTORY_H

#include "mediabutton.h"
#include "tabplaylist.h"
#include "playbackmodewidget.h"

/// Factory or helper?
class PlaybackModeWidgetFactory : public QObject
{
	Q_OBJECT
private:
	Q_ENUMS(Edge)

	MediaButton *_playbackModeButton;

	QList<PlaybackModeWidget*> _popups;

public:
	PlaybackModeWidgetFactory(QWidget *parent, MediaButton *playbackModeButton, TabPlaylist *tabPlaylists);

	enum Edge { UNDEFINED	= -1,
				TOP			= 0,
				RIGHT		= 1,
				BOTTOM		= 2,
				LEFT		= 3
			   };

	void move();

private:
	Edge _previousEdge;

public slots:
	void togglePlaybackModes();
};

#endif // PLAYBACKMODEWIDGETFACTORY_H