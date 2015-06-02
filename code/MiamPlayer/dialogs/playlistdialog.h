#ifndef PLAYLISTDIALOG_H
#define PLAYLISTDIALOG_H

#include <QDialog>

#include "ui_playlistdialog.h"

#include "../playlists/tabplaylist.h"

#include "model/sqldatabase.h"
#include <QStackedLayout>
#include <QStandardItem>

/**
 * \brief The PlaylistDialog class can save, load and export playlists in m3u format.
 */
class PlaylistDialog : public QDialog, public Ui::PlaylistDialog
{
	Q_OBJECT

private:
	QLabel *_labelEmptyPreview;

	/** Display an icon when the Preview Area is empty. */
	QStackedLayout *_stackLayout;

	/** Volatile models in the Dialog to separate which playlists were save or not. */
	QStandardItemModel *_unsavedPlaylistModel, *_savedPlaylistModel;

	QList<Playlist*> _playlists;
	QMap<QStandardItem*, Playlist*> _unsaved;

	Q_ENUMS(PlaylistRoles)

public:
	enum PlaylistRoles { PlaylistObjectPointer	= Qt::UserRole + 1,
						 PlaylistID				= Qt::UserRole + 2};

	explicit PlaylistDialog(QWidget *parent = NULL);

	inline void setPlaylists(const QList<Playlist*> &playlists) { _playlists = playlists; }

	/** Add drag & drop processing. */
	virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
	void clearPreview(bool aboutToInsertItems = true);

	/** Remove all special characters for Windows, Unix, OSX. */
	static QString convertNameToValidFileName(QString &name);

public slots:
	/** Redefined: clean preview area, populate once again lists. */
	void open();

	//void updatePlaylists2(const QList<Playlist*> playlists);

private slots:
	/** Delete from the file system every selected playlists. Cannot be canceled. */
	void deleteSavedPlaylists();

	void dropAutoSavePlaylists(const QModelIndex &, int start, int);

	/** Export one playlist at a time. */
	void exportSelectedPlaylist();

	/** Load every saved playlists. */
	void loadSelectedPlaylists();

	void populatePreviewFromSaved(const QItemSelection &, const QItemSelection &);

	void populatePreviewFromUnsaved(const QItemSelection &, const QItemSelection &);

	/** Update saved playlists when one is adding a new one. Also used at startup. */
	void updatePlaylists();


signals:
	void aboutToLoadPlaylist(uint playlistId);

	void aboutToRenamePlaylist(Playlist *playlist);

	void aboutToRemoveTabs(const QList<PlaylistDAO> &playlists);
};

#endif // PLAYLISTDIALOG_H