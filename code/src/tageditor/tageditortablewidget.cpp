#include "tageditortablewidget.h"

#include "filehelper.h"
#include "library/libraryitem.h"
#include "nofocusitemdelegate.h"
#include "settings.h"

#include <QScrollBar>
#include <phonon>

#include <QtDebug>

using namespace Phonon;

TagEditorTableWidget::TagEditorTableWidget(QWidget *parent) :
	QTableWidget(parent)
{
	Settings *settings = Settings::getInstance();
	this->setStyleSheet(settings->styleSheet(this));
	this->horizontalScrollBar()->setStyleSheet(settings->styleSheet(horizontalScrollBar()));
	this->verticalScrollBar()->setStyleSheet(settings->styleSheet(verticalScrollBar()));
	this->setItemDelegate(new NoFocusItemDelegate(this));
}

void TagEditorTableWidget::init()
{
	// Always keep the same number of columns with this taglist
	QStringList keys = (QStringList() << "FILENAME" << "ABSPATH" << "TITLE" << "ARTIST" << "ARTISTALBUM");
	keys << "ALBUM" << "TRACKNUMBER" << "DISC" << "DATE" << "GENRE" << "COMMENT";
	for (int column = 0; column < this->columnCount(); column++) {
		QTableWidgetItem *header = this->horizontalHeaderItem(column);
		if (!header) {
			header = new QTableWidgetItem(0);
			this->setHorizontalHeaderItem(column, header);
		}
		header->setData(KEY, keys.at(column));
	}
}

void TagEditorTableWidget::updateColumnData(int column, QString text)
{
	foreach (QModelIndex index, selectionModel()->selectedRows(column)) {
		QTableWidgetItem *item = itemFromIndex(index);
		item->setText(text);
		item->setData(MODIFIED, true);
	}
}

void TagEditorTableWidget::resetTable()
{
	this->setSortingEnabled(false);

	QMapIterator<QString, QPersistentModelIndex> it(indexes);
	int row = 0;
	while (it.hasNext()) {
		it.next();
		QFileInfo fileInfo(it.key());
		MediaSource source(fileInfo.absoluteFilePath());
		if (source.type() != MediaSource::Invalid) {
			TagLib::FileRef f(source.fileName().toLocal8Bit().data());

			FileHelper fh(f, it.value().data(LibraryItem::SUFFIX).toInt());

			// Reload info
			int column = 1;
			QTableWidgetItem *title = this->item(row, ++column);
			QTableWidgetItem *artist = this->item(row, ++column);
			QTableWidgetItem *artistAlbum = this->item(row, ++column);
			QTableWidgetItem *album = this->item(row, ++column);
			QTableWidgetItem *trackNumber = this->item(row, ++column);
			//QTableWidgetItem *disc = this->item(row, ++column);
			QTableWidgetItem *year = this->item(row, ++column);
			QTableWidgetItem *genre = this->item(row, ++column);
			QTableWidgetItem *comment = this->item(row, ++column);

			title->setText(f.tag()->title().toCString());
			artist->setText(f.tag()->artist().toCString());
			artistAlbum->setText(fh.artistAlbum().toCString());
			album->setText(f.tag()->album().toCString());
			trackNumber->setText(QString::number(f.tag()->track()));
			//disc->setText(discNumber.toCString());
			year->setText(QString::number(f.tag()->year()));
			genre->setText(f.tag()->genre().toCString());
			comment->setText(f.tag()->comment().toCString());
		}
		row++;
	}
	this->setSortingEnabled(true);
	this->sortItems(0);
	this->sortItems(1);
}

void TagEditorTableWidget::addItemFromLibrary(const QPersistentModelIndex &index)
{
	QString path = Settings::getInstance()->musicLocations().at(index.data(LibraryItem::IDX_TO_ABS_PATH).toInt()).toString();
	QString name = index.data(LibraryItem::REL_PATH_TO_MEDIA).toString();
	QFileInfo fileInfo(path + name);
	MediaSource source(fileInfo.absoluteFilePath());

	if (source.type() != MediaSource::Invalid) {
		TagLib::FileRef f(source.fileName().toLocal8Bit().data());
		indexes.insert(fileInfo.absoluteFilePath(), index);

		FileHelper fh(f, index.data(LibraryItem::SUFFIX).toInt());

		// The first two columns are not editable
		// It may changes in the future for the first one (the filename)
		QTableWidgetItem *fileName = new QTableWidgetItem(fileInfo.fileName());
		fileName->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		fileName->setData(LibraryItem::SUFFIX, fh.type());
		QTableWidgetItem *absPath = new QTableWidgetItem(fileInfo.absolutePath());
		absPath->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

		QTableWidgetItem *title = new QTableWidgetItem(f.tag()->title().toCString());
		QTableWidgetItem *artist = new QTableWidgetItem(f.tag()->artist().toCString());
		QTableWidgetItem *artistAlbum = new QTableWidgetItem(fh.artistAlbum().toCString());
		QTableWidgetItem *album = new QTableWidgetItem(f.tag()->album().toCString());
		/// FIXME: is there a way to extract String = "01" instead of int = 1 ?
		QTableWidgetItem *trackNumber = new QTableWidgetItem(QString::number(f.tag()->track()));
		//QTableWidgetItem *disc = new QTableWidgetItem(discNumber.toCString());
		QTableWidgetItem *disc = new QTableWidgetItem("");
		QTableWidgetItem *year = new QTableWidgetItem(QString::number(f.tag()->year()));
		QTableWidgetItem *genre = new QTableWidgetItem(f.tag()->genre().toCString());
		QTableWidgetItem *comment = new QTableWidgetItem(f.tag()->comment().toCString());

		QList<QTableWidgetItem*> items;
		items << fileName << absPath << title << artist << artistAlbum << album << trackNumber << disc << year << genre << comment;

		// Create a new row with right data
		int row = rowCount();
		this->insertRow(row);
		for (int column = 0; column < items.size(); column++) {
			this->setItem(row, column, items.at(column));
		}
	}
}

/** Redefined. */
void TagEditorTableWidget::clear()
{
	while (rowCount() > 0) {
		this->removeRow(0);
	}
	indexes.clear();
	this->setSortingEnabled(false);
}