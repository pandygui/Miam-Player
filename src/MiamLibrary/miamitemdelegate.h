#ifndef MIAMITEMDELEGATE_H
#define MIAMITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QSortFilterProxyModel>
#include <QTimer>
#include "albumitem.h"
#include "artistitem.h"
#include "libraryitemmodel.h"
#include "trackitem.h"

#include "miamlibrary_global.hpp"

/**
 * \brief		The MiamItemDelegate class is the base class for drawing music informations in a QTreeView or a QListView
 * \details		This class has been made pure virtual class, because we cannot render Albums or Tracks without knowing the underlying
 *				context. But we can group here
 * \author      Matthieu Bachelier
 * \copyright   GNU General Public License v3
 */
class MIAMLIBRARY_LIBRARY MiamItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
protected:
	static qreal _iconOpacity;

	QStandardItemModel *_libraryModel;
	QSortFilterProxyModel *_proxy;
	bool _showCovers;

	/** This timer is used to animate album cover when one is scrolling.
	 * It improves reactivity of the UI by temporarily disabling painting events.
	 * When covers are becoming visible once again, they are redisplayed with a nice fading effect. */
	QTimer *_timer;

	int _coverSize;

public:
	explicit MiamItemDelegate(QSortFilterProxyModel *proxy);

protected:
	virtual void drawAlbum(QPainter *painter, QStyleOptionViewItem &option, AlbumItem *item) const = 0;

	virtual void drawArtist(QPainter *painter, QStyleOptionViewItem &option, ArtistItem *item) const = 0;

	void drawLetter(QPainter *painter, QStyleOptionViewItem &option, SeparatorItem *item) const;

	virtual void drawTrack(QPainter *painter, QStyleOptionViewItem &option, TrackItem *track) const;

	void paintRect(QPainter *painter, const QStyleOptionViewItem &option) const;

	void paintText(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rectText, const QString &text, const QStandardItem *item) const;
};

#endif // MIAMITEMDELEGATE_H
