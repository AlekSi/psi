#ifndef rosterlist_MODEL_H
#define rosterlist_MODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>

#include "item.h"

namespace Roster {
	class RosterList;
	class Manager;

	enum Roles {
		ItemRole = Qt::UserRole,
		IdRole = Qt::UserRole + 1,
		AvatarRole = Qt::UserRole + 2,
		StatusRole = Qt::UserRole + 3
	};

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

		public slots:
			void setShowAvatars(bool showAvatars);
			void setShowStatus(bool showStatus);

			void itemUpdated(Item* item);
			void itemAdded(Item* item);
			void itemRemoved(Item* item);
			void itemToBeRemoved(Item* item);

		private:
			QVariant makeToolTip(const QModelIndex& index) const;

			RosterList* rosterlist_;
			Manager* manager_;
			bool showAvatars_, showStatus_;
	};
};

#endif
