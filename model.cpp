#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>
#include <QDebug>
#include <QAction>
#include <QMimeData>
#include <QTextDocument>

#include "roster.h"
#include "rosterlist.h"
#include "model.h"
#include "groupitem.h"
#include "item.h"
#include "contact.h"
#include "group.h"
#include "resource.h"
#include "manager.h"

namespace Roster {
	Model::Model(RosterList* rosterlist) : rosterlist_(rosterlist), showAvatars_(true), showStatus_(true) {
	}

	QVariant Model::data(const QModelIndex &index, int role) const {
		if ( ! index.isValid() ) {
			return QVariant();
		}

		Item* item = static_cast<Item*>(index.internalPointer());

		if ( role == Qt::DisplayRole or role == Qt::EditRole ) { // name
			if ( Roster* roster = dynamic_cast<Roster*>(item) ) {
				return roster->getName();
			} else if ( Group* group = dynamic_cast<Group*>(item) ) {
				return group->getName();
			} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				return contact->getName();
			} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
				return QString("%1 (%2)").arg(resource->getName(), QString::number(resource->getPriority()));
			}
		} else if ( role == Qt::DecorationRole ) { // left icon
			if ( Group* group = dynamic_cast<Group*>(item) ) {
				if ( group->isOpen() ) {
					return QIcon("icons/groupopen.png");
				} else {
					return QIcon("icons/groupclose.png");
				}
			} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				return contact->getIcon();
			} else if ( Roster* roster = dynamic_cast<Roster*>(item) ) {
				return roster->getIcon();
			} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
				return resource->getIcon();
			}
		} else if ( role == ItemRole ) { // pointer to real item
			return QVariant::fromValue(item);
		} else if ( role == Qt::BackgroundRole ) { // background color
			if ( dynamic_cast<Roster*>(item) ) {
				return QBrush( QColor(150, 150, 150), Qt::SolidPattern ); // FIXME: colors from options
			} else if ( dynamic_cast<Group*>(item) ) {
				return QBrush( QColor(240, 240, 240), Qt::SolidPattern );
			}
		} else if ( role == Qt::ForegroundRole ) { // text color
			if ( dynamic_cast<Roster*>(item) ) {
				return QBrush( Qt::white, Qt::SolidPattern );
			} else if ( dynamic_cast<Group*>(item) ) {
				return QBrush( QColor(90, 90, 90), Qt::SolidPattern );
			} else if ( dynamic_cast<Resource*>(item) ) {
				return QBrush(Qt::gray);
			}
		} else if ( role == Qt::ToolTipRole ) { // tooltip
			return makeToolTip(index);
		} else if ( role == IdRole ) { // unique id
			return item->getId();
		} else if ( role == AvatarRole and showAvatars_ ) { // avatar image
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				return contact->getAvatar(); 
			}
		} else if ( role == StatusRole and showStatus_ ) { // status text
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				return contact->getStatus();
			} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
				return resource->getStatus();
			}
		} else if ( role == Qt::SizeHintRole ) { // size of item
			if ( !index.data(AvatarRole).value<QIcon>().isNull() ) {
				return QSize(1, 34);
			} else if ( !index.data(StatusRole).toString().isEmpty() ) {
				return QSize(1, 34);
			} else {
				return QSize(1, 20);
			}
		}

		return QVariant();
	}

	QVariant Model::makeToolTip(const QModelIndex& index) const {
		Item* item = index.data(ItemRole).value<Item*>();
		QString tip("<div style=\"white-space: pre\">");

		if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			tip += QString("%1 &lt;%2&gt;\n").arg(Qt::escape(contact->getName()), Qt::escape(contact->getJid()));

			foreach(Resource* resource, contact->getResources()) {
				tip += QString("<img src=\":icons/online.png\"> <b>%1</b> (%2)\n").arg(resource->getName(), QString::number(resource->getPriority()));

				if (! resource->getStatus().isEmpty()) {
					tip += "<u>Status message</u>\n";
					tip += Qt::escape(resource->getStatus()) + "\n";
				}
			}
		} else if ( Roster* roster = dynamic_cast<Roster*>(item) ) {
			tip += roster->getName();
		} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
			Contact* parent = dynamic_cast<Contact*>(item->getParent());
			tip += QString("%1 &lt;%2&gt;\n").arg(Qt::escape(parent->getName()), Qt::escape(parent->getJid()));
			tip += QString("<img src=\":icons/online.png\"> <b>%1</b> (%2)\n").arg(resource->getName(), QString::number(resource->getPriority()));

			if (! resource->getStatus().isEmpty()) {
				tip += "<u>Status message</u>\n";
				tip += Qt::escape(resource->getStatus()) + "\n";
			}
		} else {
			return QVariant();
		}

		tip += "</div>";
		return tip;
	}

	int Model::rowCount(const QModelIndex &parent) const {
		int num = 0;

		if (parent.isValid()) {
			Item* item = static_cast<Item*>(parent.internalPointer());
			if ( GroupItem* parentItem = dynamic_cast<GroupItem*>(item) ) {
				num = parentItem->getNbItems();
			} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				num = contact->getResources().size();
			}
		} else {
			num = rosterlist_->getNbItems();
		}

		return num;
	}

	QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const {
		Q_UNUSED(section);
		Q_UNUSED(orientation);
		Q_UNUSED(role);
		return QVariant();
	}

	Qt::ItemFlags Model::flags(const QModelIndex& index) const {
		Item* item = index.data(ItemRole).value<Item*>();

		if ( dynamic_cast<Contact*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		} else if ( dynamic_cast<Resource*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		} else if ( dynamic_cast<Group*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable;
		} else {
			return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
		}
	}

	int Model::columnCount(const QModelIndex& index) const {
		Q_UNUSED(index);
		return 1;
	}

	QModelIndex Model::index(int row, int column, const QModelIndex& parent) const {
		Item* item = static_cast<Item*>(parent.internalPointer());
		if ( parent.isValid() and dynamic_cast<Contact*>(item) ) {
			Contact* contact = dynamic_cast<Contact*>(item);
			if ( row < contact->getResources().size() ) {
				return createIndex(row, column, contact->getResources().at(row));
			}
		} else {
			GroupItem* parentItem;
			if (parent.isValid()) {
				parentItem = dynamic_cast<GroupItem*>(item);
			} 
			else {
				parentItem = rosterlist_;
			}

			if ( row < parentItem->getNbItems() ) {
				return createIndex(row, column, parentItem->getItem(row));
			}
		}

		return QModelIndex();
	}

	QModelIndex Model::parent(const QModelIndex& index) const {
		if (!index.isValid()) {
			return QModelIndex();
		}

		Item* item = static_cast<Item*>(index.internalPointer());
		Item* parent = item->getParent();

		if (parent == rosterlist_) {
			return QModelIndex();
		}
		else {
			GroupItem* grandparent = dynamic_cast<GroupItem*>(parent->getParent());
			return createIndex(grandparent->getIndexOf(parent), 0, parent);
		}
	}

	QMimeData* Model::mimeData(const QModelIndexList &indexes) const {
		QMimeData* mimeData = new QMimeData();

		QByteArray encodedText, encodedIds;
		QDataStream textStream(&encodedText, QIODevice::WriteOnly);
		QDataStream idStream(&encodedIds, QIODevice::WriteOnly);


		foreach (QModelIndex index, indexes) {
			if (index.isValid()) {
				Item* item = index.data(ItemRole).value<Item*>();
				idStream << index.data(IdRole).value<unsigned int>();
				if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
					textStream << contact->getName();
				}
			}
		}

		mimeData->setData("application/vnd.text.list", encodedText);
		mimeData->setData("application/x-psi-rosteritem", encodedIds);
		return mimeData;
	}

	bool Model::dropMimeData(const QMimeData* data,	Qt::DropAction action, int row, int column, const QModelIndex& parent) {
		Q_UNUSED(column);
		Q_UNUSED(row);

		if (action == Qt::IgnoreAction) {
			return true;
		}

		if (!data->hasFormat("application/x-psi-rosteritem") or !parent.isValid()) {
			return false;
		}

		/* get indexes for dragged items */
		QByteArray encodedData = data->data("application/x-psi-rosteritem");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

		QList<Item*> items;
		while (!stream.atEnd()) {
			unsigned int id;
			stream >> id;

			QModelIndexList indexList = match(index(0, 0, QModelIndex()), IdRole, id, 1, Qt::MatchWrap | Qt::MatchExactly | Qt::MatchRecursive);
			if ( indexList.isEmpty() ) {
				qDebug() << "Item not found on roster. Maybe it was deleted while dragging?";
				continue;
			}

			QModelIndex index = indexList.at(0);
			Item* item = index.data(ItemRole).value<Item*>();
			items.append(item);
		}
		
		Item* parentItem = parent.data(ItemRole).value<Item*>();

		if ( Contact* parentContact = dynamic_cast<Contact*>(parentItem) ) {
			qDebug() << items.size() << "item(s) has been dropped on contact" << parentContact->getName();
		} else if ( Group* parentGroup = dynamic_cast<Group*>(parentItem) ) {
			foreach(Item* item, items) {
				Contact* contact = dynamic_cast<Contact*>(item);
				if ( contact ) {
					if ( action == Qt::CopyAction ) {
						manager_->copyContact(contact, parentGroup);
					}
				}
			}
			qDebug() << "Following items has been dropped on group" << parentGroup->getName();
		} else if ( Roster* parentRoster = dynamic_cast<Roster*>(parentItem) ) {
			qDebug() << items.size() << "item(s) has been dropped on roster" << parentRoster->getName();
		}
		

		if ( action == Qt::CopyAction ) {
			qDebug() << "action was 'copy'";
		} else if ( action == Qt::MoveAction ) {
			qDebug() << "action was 'move'";
		}

		return true;
	}

	Qt::DropActions Model::supportedDropActions() const {
		return Qt::CopyAction | Qt::MoveAction;
	}

	QStringList Model::mimeTypes() const {
		QStringList types;
		types << "application/x-psi-rosteritem";
		return types;
	}

	void Model::setShowAvatars(bool showAvatars) {
		showAvatars_ = showAvatars;
		emit layoutChanged();
	}

	void Model::setShowStatus(bool showStatus) {
		showStatus_ = showStatus;
		emit layoutChanged();
	}

	void Model::setManager(Manager* manager) {
		manager_ = manager;
		connect(manager_, SIGNAL(itemUpdated(Item*)), SLOT(itemUpdated(Item*)));
		connect(manager_, SIGNAL(itemAdded(Item*)), SLOT(itemAdded(Item*)));
	}

	Manager* Model::getManager() const {
		return manager_;
	}

	QModelIndex Model::getIndex(const unsigned int id) const {
			QModelIndexList indexList = match(index(0, 0, QModelIndex()), IdRole, id, 1, Qt::MatchWrap | Qt::MatchExactly | Qt::MatchRecursive);
			if ( indexList.isEmpty() ) {
				return QModelIndex();
			}

			return indexList.at(0);
	}

	void Model::itemUpdated(Item* item) {
		QModelIndex index = getIndex(item->getId());
		if ( index.isValid() ) {
			emit dataChanged(index, index);
		}
	}

	void Model::itemAdded(Item* item) {
		Q_UNUSED(item);
		emit layoutChanged();
	}
}

