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
	class StatusIconProvider;
	class GroupItem;
	class ViewActionsService;
	class Resource;

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
			void setStatusIconProvider(StatusIconProvider* statusIconProvider);
			void setViewActionsService(ViewActionsService* actionsService);

		public slots:
			void setShowAvatars(bool showAvatars);
			void setShowStatusMessages(bool showStatusMessages);

			void itemUpdated(Item* item);
			void itemAdded(Item* item);
			void itemToBeAdded(GroupItem* parent, int row);
			void itemRemoved(Item* item);
			void itemToBeRemoved(Item* item);

			void iconsChanged();

		signals:
			void expand(const QModelIndex& index);
			void collapse(const QModelIndex& index);
			void ensureVisible(const QModelIndex& index);

		private:
			QVariant makeToolTip(const QModelIndex& index) const;
			const QString doResourceTip(Resource* resource) const;
			QVariant foregroundRole(Item* item) const;
			QVariant backgroundRole(Item* item) const;
			QVariant decorationRole(Item* item) const;

			Roster* root_;
			Manager* manager_;
			bool showAvatars_, showStatusMessages_;
			StatusIconProvider* statusIconProvider_;
			ViewActionsService* actionsService_;
	};
};

#endif
