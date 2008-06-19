#ifndef rosterlist_MODEL_H
#define rosterlist_MODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>

#include "Item.h"

namespace Roster {
	class RosterList;

	class Model : public QAbstractItemModel	{
		Q_OBJECT

		public:
			Model(RosterList* rosterlist);
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			QVariant data(const QModelIndex& index, int role) const;
			Qt::ItemFlags flags(const QModelIndex& index) const;
			QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
			int columnCount(const QModelIndex& index) const;
			QModelIndex parent(const QModelIndex& index) const;
			QModelIndex index(int row, int column, const QModelIndex& parent) const;
			QStringList mimeTypes() const;
			QMimeData* mimeData(const QModelIndexList &indexes) const;
			bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);
		
			Item* getItem(const QModelIndex& index) const;

		private:
			RosterList* rosterlist_;
	};
};

#endif
