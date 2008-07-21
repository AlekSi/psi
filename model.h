#ifndef ROSTER_MODEL_H
#define ROSTER_MODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>

#include "globals.h"

namespace Roster {
	class Item;
	class Roster;
	class Manager;
	class RosterBuilder;

	class Model : public QAbstractItemModel	{
		Q_OBJECT

		public:
			Model(Roster* root);
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			QVariant data(const QModelIndex& index, int role) const;
			Qt::ItemFlags flags(const QModelIndex& index) const;
			QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
			int columnCount(const QModelIndex& index) const;
			QModelIndex parent(const QModelIndex& index) const;
			QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
			QMimeData* mimeData(const QModelIndexList &indexes) const;
			bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent);
			Qt::DropActions supportedDropActions() const;
			QStringList mimeTypes() const;
			bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

			Item* getItem(const QModelIndex& index) const;
			QModelIndex getIndex(const unsigned int id) const;
			void setManager(Manager* manager);
			Manager* getManager() const;
			void setRosterBuilder(RosterBuilder* rb);

		public slots:
			void setShowAvatars(bool showAvatars);
			void setShowStatus(bool showStatus);

			void itemUpdated(Item* item);
			void itemAdded(Item* item);
			void itemRemoved(Item* item);
			void itemToBeRemoved(Item* item);

		signals:
			void expand(const QModelIndex& index);
			void collapse(const QModelIndex& index);

		private:
			QVariant makeToolTip(const QModelIndex& index) const;

			Roster* root_;
			Manager* manager_;
			RosterBuilder* rb_;
			bool showAvatars_, showStatus_;
	};
};

#endif
