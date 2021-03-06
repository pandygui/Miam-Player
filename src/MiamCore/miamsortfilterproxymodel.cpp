#include "miamsortfilterproxymodel.h"
#include "settingsprivate.h"

#include <functional>
#include <QSet>
#include <QStandardItem>

#include <QtDebug>

MiamSortFilterProxyModel::MiamSortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
{
	this->setSortCaseSensitivity(Qt::CaseInsensitive);
	this->setSortRole(Miam::DF_NormalizedString);
	this->setDynamicSortFilter(false);
	this->sort(0, Qt::AscendingOrder);
}

void MiamSortFilterProxyModel::findMusic(const QString &text)
{
	if (SettingsPrivate::instance()->librarySearchMode() == SettingsPrivate::LSM_Filter) {
		this->filterLibrary(text);
	} else {
		this->highlightMatchingText(text);
	}
}

bool MiamSortFilterProxyModel::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const
{

	/*QModelIndex item = sourceModel()->index(0, sourceColumn, sourceParent);
	if (!item.isValid()) {
		return false;
	} else {
		qDebug() << Q_FUNC_INFO << item;
	}*/
	return QSortFilterProxyModel::filterAcceptsColumn(sourceColumn, sourceParent);
}

/*bool LibraryFilterProxyModel::hasAcceptedChildren(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex item = sourceModel()->index(sourceRow, 0, sourceParent);
	if (!item.isValid()) {
		return false;
	}

	// Check if there are children
	int childCount = item.model()->rowCount(item);
	if (childCount == 0) {
		return false;
	}

	for (int i = 0; i < childCount; ++i) {
		if (filterAcceptsRowItself(i, item)) {
			return true;
		}
		// Recursive call
		if (hasAcceptedChildren(i, item)) {
			return true;
		}
	}
	return false;
}*/

void MiamSortFilterProxyModel::filterLibrary(const QString &filter)
{
	if (filter.isEmpty()) {
		this->setFilterRole(Qt::DisplayRole);
		this->setFilterRegExp(QRegExp());
		this->sort(0, this->sortOrder());
	} else {
		bool needToSortAgain = false;
		if (this->filterRegExp().pattern().size() < filter.size() && filter.size() > 1) {
			needToSortAgain = true;
		}
		if (filter.contains(QRegExp("^(\\*){1,5}$"))) {
			// Convert stars into [1-5], ..., [5-5] regular expression
			this->setFilterRole(Miam::DF_Rating);
			this->setFilterRegExp(QRegExp("[" + QString::number(filter.size()) + "-5]", Qt::CaseInsensitive, QRegExp::RegExp));
		} else {
			this->setFilterRole(Qt::DisplayRole);
			this->setFilterRegExp(QRegExp(filter, Qt::CaseInsensitive, QRegExp::FixedString));
		}
		if (needToSortAgain) {
			this->sort(0, this->sortOrder());
		}
	}
}

/** Highlight items in the Tree when one has activated this option in settings. */
void MiamSortFilterProxyModel::highlightMatchingText(const QString &text)
{
	// Clear highlight on every call
	std::function<void(QStandardItem *item)> recursiveClearHighlight;
	recursiveClearHighlight = [&recursiveClearHighlight] (QStandardItem *item) -> void {
		if (item->hasChildren()) {
			item->setData(false, Miam::DF_Highlighted);
			for (int i = 0; i < item->rowCount(); i++) {
				recursiveClearHighlight(item->child(i, 0));
			}
		} else {
			item->setData(false, Miam::DF_Highlighted);
		}
	};

	QStandardItemModel *_libraryModel = qobject_cast<QStandardItemModel*>(this->sourceModel());
	for (int i = 0; i < _libraryModel->rowCount(); i++) {
		recursiveClearHighlight(_libraryModel->item(i, 0));
	}

	// Adapt filter if one is typing '*'
	QString filter;
	Qt::MatchFlags flags;
	if (text.contains(QRegExp("^(\\*){1,5}$"))) {
		this->setFilterRole(Miam::DF_Rating);
		filter = "[" + QString::number(text.size()) + "-5]";
		flags = Qt::MatchRecursive | Qt::MatchRegExp;
	} else {
		this->setFilterRole(Qt::DisplayRole);
		filter = text;
		flags = Qt::MatchRecursive | Qt::MatchContains;
	}

	// Mark items with a bold font
	QSet<QChar> lettersToHighlight;
	if (!text.isEmpty()) {
		QModelIndexList indexes = _libraryModel->match(_libraryModel->index(0, 0, QModelIndex()), this->filterRole(), filter, -1, flags);
		QList<QStandardItem*> items;
		for (int i = 0; i < indexes.size(); ++i) {
			items.append(_libraryModel->itemFromIndex(indexes.at(i)));
		}
		for (QStandardItem *item : items) {
			item->setData(true, Miam::DF_Highlighted);
			QStandardItem *parent = item->parent();
			// For every item marked, mark also the top level item
			while (parent != nullptr) {
				parent->setData(true, Miam::DF_Highlighted);
				if (parent->parent() == nullptr) {
					lettersToHighlight << parent->data(Miam::DF_NormalizedString).toString().toUpper().at(0);
				}
				parent = parent->parent();
			}
		}
	}
	qDebug() << Q_FUNC_INFO << lettersToHighlight;
	emit aboutToHighlightLetters(lettersToHighlight);
}
