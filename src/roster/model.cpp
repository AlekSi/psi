#include <QAbstractItemModel>
#include <QVariant>
#include <QModelIndex>
#include <QDebug>
#include <QAction>
#include <QMimeData>
#include <QTextDocument>

#include "account.h"
#include "roster.h"
#include "model.h"
#include "groupitem.h"
#include "item.h"
#include "contact.h"
#include "group.h"
#include "resource.h"
#include "manager.h"
#include "rosterbuilder.h"
#include "metacontact.h"
#include "statusiconprovider.h"
#include "iconset.h"
#include "transport.h"
#include "self.h"
#include "psioptions.h"
#include "viewactionsservice.h"
#include "notinlist.h"

namespace Roster {

	Model::Model(Roster* root) : root_(root), showAvatars_(false), showStatusMessages_(false), statusIconProvider_(NULL) {
	}

	QVariant Model::data(const QModelIndex &index, int role) const {
		if ( ! index.isValid() ) {
			return QVariant();
		}

		Item* item = static_cast<Item*>(index.internalPointer());
		Q_ASSERT(item);

		if ( role == Qt::DisplayRole or role == Qt::EditRole ) { // name
			if ( Account* account = dynamic_cast<Account*>(item) ) {
				return account->getName();
			} else if ( Group* group = dynamic_cast<Group*>(item) ) {
				return group->getName();
			} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				return contact->getName();
			} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
				return QString("%1 (%2)").arg(resource->getName(), QString::number(resource->getPriority()));
			} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
				return metacontact->getName();
			}
		} else if ( role == Qt::DecorationRole ) { // left icon
			return decorationRole(item);
		} else if ( role == ItemRole ) { // pointer to real item
			return QVariant::fromValue(item);
		} else if ( role == Qt::BackgroundRole ) { // background color
			return backgroundRole(item);
		} else if ( role == Qt::ForegroundRole ) { // text color
			return foregroundRole(item);
		} else if ( role == Qt::ToolTipRole ) { // tooltip
			return makeToolTip(index);
		} else if ( role == IdRole ) { // unique id
			return item->getId();
		} else if ( role == AvatarRole and showAvatars_ ) { // avatar image
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				return contact->getAvatar(); 
			} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
				return metacontact->getAvatar();
			}
		} else if ( role == StatusMessageRole and showStatusMessages_ ) { // statusMessage text
			if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
				return contact->getStatusMessage();
			} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
				return resource->getStatusMessage();
			} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
				return metacontact->getStatusMessage();
			}
		} else if ( role == Qt::SizeHintRole ) { // size of item
			if ( !index.data(AvatarRole).value<QIcon>().isNull() ) {
				return QSize(1, 34);
			} else if ( !index.data(StatusMessageRole).toString().isEmpty() ) {
				return QSize(1, 34);
			} else {
				return QSize(1, 20);
			}
		}

		return QVariant();
	}

	QVariant Model::backgroundRole(Item* item) const {
		if ( dynamic_cast<Account*>(item) ) {
			return QBrush(PsiOptions::instance()->getOption("options.ui.look.colors.contactlist.profile.header-background").value<QColor>());
		} else if ( dynamic_cast<Group*>(item) ) {
			return QBrush(PsiOptions::instance()->getOption("options.ui.look.colors.contactlist.grouping.header-background").value<QColor>());
		}

		return QVariant();
	}

	QVariant Model::decorationRole(Item* item) const {
		if ( Group* group = dynamic_cast<Group*>(item) ) {
			return statusIconProvider_->getIconForGroup(group->isExpanded());
		} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			if ( contact->getIncomingEvent() ) {
				return statusIconProvider_->getIconForEvent(contact->getIncomingEvent());
			}
			return statusIconProvider_->getIconForStatus(contact->getStatus());
		} else if ( Account* account = dynamic_cast<Account*>(item) ) {
			return statusIconProvider_->getIconForStatus(account->getStatus());
		} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
			return statusIconProvider_->getIconForStatus(resource->getStatus());
		} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
			if ( metacontact->getIncomingEvent() ) {
				return statusIconProvider_->getIconForEvent(metacontact->getIncomingEvent());
			}
			return statusIconProvider_->getIconForStatus(metacontact->getStatus());
		}

		return QVariant();
	}

	QVariant Model::foregroundRole(Item* item) const {
		QBrush br(Qt::black, Qt::SolidPattern);

		if ( dynamic_cast<Account*>(item) ) {
			br.setColor(PsiOptions::instance()->getOption("options.ui.look.colors.contactlist.profile.header-foreground").value<QColor>());
		} else if ( dynamic_cast<Group*>(item) ) {
			br.setColor(PsiOptions::instance()->getOption("options.ui.look.colors.contactlist.grouping.header-foreground").value<QColor>());
		} else if ( dynamic_cast<Resource*>(item) ) {
			br.setColor(Qt::gray); // FIXME: new option
		} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			StatusType status = contact->getStatus();

			if ( status == STATUS_ONLINE or status == STATUS_CHAT or status == STATUS_INVISIBLE ) {
				br.setColor(PsiOptions::instance()->getOption("options.ui.look.colors.contactlist.status.online").value<QColor>());
			} else if ( status == STATUS_AWAY or status == STATUS_XA ) {
				br.setColor(PsiOptions::instance()->getOption("options.ui.look.colors.contactlist.status.away").value<QColor>());
			} else if ( status == STATUS_DND ) {
				br.setColor(PsiOptions::instance()->getOption("options.ui.look.colors.contactlist.status.do-not-disturb").value<QColor>());
			} else if ( status == STATUS_OFFLINE ) {
				br.setColor(PsiOptions::instance()->getOption("options.ui.look.colors.contactlist.status.offline").value<QColor>());
			}
		}

		return br;
	}

	QVariant Model::makeToolTip(const QModelIndex& index) const {
		Item* item = index.data(ItemRole).value<Item*>();
		QString tip("<div style=\"white-space: pre\">");

		if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			tip += QString("%1 &lt;%2&gt;\n").arg(Qt::escape(contact->getName()), Qt::escape(contact->getJid().full()));

			foreach(Item* subitem, contact->getItems()) {
				Resource* resource = dynamic_cast<Resource*>(subitem);
				tip += QString("<icon name=\"%3\"> <b>%1</b> (%2)\n")
					.arg(resource->getName(), QString::number(resource->getPriority()), statusToText(resource->getStatus()));

				if (! resource->getStatusMessage().isEmpty()) {
					tip += "<u>StatusMessage message</u>\n";
					tip += Qt::escape(resource->getStatusMessage()) + "\n";
				}
			}

			/* if we're using joined accounts, put info on which account is this contact on */
			if ( contact->getParent()->getAccountName().isEmpty() ) {
				tip += QString("\n(Account: %1)\n").arg(Qt::escape(contact->getAccountName()));
			}
		} else if ( Account* account = dynamic_cast<Account*>(item) ) {
			tip += account->getName();
		} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
			Contact* parent = dynamic_cast<Contact*>(item->getParent());
			tip += QString("%1 &lt;%2&gt;\n").arg(Qt::escape(parent->getName()), Qt::escape(parent->getJid().full()));
			tip += QString("<icon name=\"%3\"> <b>%1</b> (%2)\n")
				.arg(resource->getName(), QString::number(resource->getPriority()), statusToText(resource->getStatus()));

			if (! resource->getStatusMessage().isEmpty()) {
				tip += "<u>StatusMessage message</u>\n";
				tip += Qt::escape(resource->getStatusMessage()) + "\n";
			}
		} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
			tip += Qt::escape(metacontact->getName()) + "\n";

			foreach(Item* item, metacontact->getItems()) {
				Contact* contact = static_cast<Contact*>(item);	
				tip += QString("%1 &lt;%2&gt;\n").arg(Qt::escape(contact->getName()), Qt::escape(contact->getJid().full()));
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
			if ( GroupItem* groupItem = dynamic_cast<GroupItem*>(item) ) {
				num = groupItem->getNbItems();
			}
		} else {
			num = root_->getNbItems();
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

		if ( dynamic_cast<Transport*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
		} else if ( dynamic_cast<NotInList*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		} else if ( dynamic_cast<Self*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		} else if ( dynamic_cast<Contact*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		} else if ( dynamic_cast<Resource*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		} else if ( dynamic_cast<Group*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;
		} else if ( dynamic_cast<Metacontact*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		} else if ( dynamic_cast<Account*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
		} else {
			return Qt::ItemIsEnabled;
		}
	}

	int Model::columnCount(const QModelIndex& index) const {
		Q_UNUSED(index);
		return 1;
	}

	QModelIndex Model::index(int row, int column, const QModelIndex& parent) const {
		GroupItem* parentItem;
		if ( parent.isValid() ) {
			parentItem = static_cast<GroupItem*>(parent.internalPointer());
		} else {
			parentItem = root_;
		}

		if ( row < parentItem->getNbItems() ) {
			return createIndex(row, column, parentItem->getItem(row));
		}

		return QModelIndex();
	}

	QModelIndex Model::parent(const QModelIndex& index) const {
		if (!index.isValid()) {
			return QModelIndex();
		}

		Item* item = static_cast<Item*>(index.internalPointer());
		Q_ASSERT(item);
		Item* parent = item->getParent();

		if (parent == root_) {
			return QModelIndex();
		}
		else {
			GroupItem* grandparent = parent->getParent();
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

			QModelIndexList indexList = match(index(0, 0, QModelIndex()), IdRole, id, 1, 
					Qt::MatchWrap | Qt::MatchExactly | Qt::MatchRecursive);
			if ( indexList.isEmpty() ) {
				qDebug() << "Item not found on roster. Maybe it was deleted while dragging?";
				continue;
			}

			QModelIndex index = indexList.at(0);
			Item* item = index.data(ItemRole).value<Item*>();
			items.append(item);
		}
		
		Item* parentItem = parent.data(ItemRole).value<Item*>();

		if ( Group* parentGroup = dynamic_cast<Group*>(parentItem) ) {
			// we're moving items to group
			foreach(Item* item, items) {
				if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
					if ( action == Qt::MoveAction ) { // move contact
						actionsService_->moveToGroup(contact, parentGroup->getGroupPath());
					} else if ( action == Qt::CopyAction ) { // copy contact
						actionsService_->copyToGroup(contact, parentGroup->getGroupPath());
					}
				} else if ( Group* group = dynamic_cast<Group*>(item) ) {
					if ( action == Qt::MoveAction ) {
						actionsService_->moveGroup(group, parentGroup);
					}
				}
			}
		} else if ( Account* parentAccount = dynamic_cast<Account*>(parentItem) ) {
			foreach(Item* item, items) {
				if ( Group* group = dynamic_cast<Group*>(item) ) {
					if ( action == Qt::MoveAction ) {
						actionsService_->moveGroup(group, parentAccount);
					}
				}
			}
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

	void Model::setShowStatusMessages(bool showStatusMessages) {
		showStatusMessages_ = showStatusMessages;
		emit layoutChanged();
	}

	void Model::setManager(Manager* manager) {
		manager_ = manager;
		connect(manager_, SIGNAL(itemUpdated(Item*)), SLOT(itemUpdated(Item*)));
		connect(manager_, SIGNAL(itemAdded(Item*)), SLOT(itemAdded(Item*)));
		connect(manager_, SIGNAL(itemToBeAdded(GroupItem*, int)), SLOT(itemToBeAdded(GroupItem*, int)));
		connect(manager_, SIGNAL(itemRemoved(Item*)), SLOT(itemRemoved(Item*)));
		connect(manager_, SIGNAL(itemToBeRemoved(Item*)), SLOT(itemToBeRemoved(Item*)));
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
			if ( GroupItem* groupItem = dynamic_cast<GroupItem*>(item) ) {
				if ( groupItem->isExpanded() ) {
					emit expand(index);
				} else {
					emit collapse(index);
				}
			}

			emit dataChanged(index, index);
		}
	}

	void Model::itemAdded(Item* item) {
		endInsertRows();

		QModelIndex index = getIndex(item->getId());
		if ( index.isValid() ) {
			if ( GroupItem* groupItem = dynamic_cast<GroupItem*>(item) ) {
				if ( groupItem->isExpanded() ) {
					emit expand(index);
				} else {
					emit collapse(index);
				}
			}

			emit layoutChanged();
		}
	}

	void Model::itemToBeAdded(GroupItem* parent, int row) {
		QModelIndex index = getIndex(parent->getId());
		beginInsertRows(index, row, row);
	}

	void Model::itemToBeRemoved(Item* item) {
		QModelIndex index = getIndex(item->getId());
		beginRemoveRows(index.parent(), index.row(), index.row());
	}

	void Model::itemRemoved(Item* item) {
		Q_UNUSED(item);
		endRemoveRows();
	}

	bool Model::setData(const QModelIndex& index, const QVariant& value, int role) {
		Q_UNUSED(role);

		Item* item = index.data(ItemRole).value<Item*>();

		if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			actionsService_->rename(contact, value.toString());	
			return true;
		} else if ( Group* group = dynamic_cast<Group*>(item) ) {
			actionsService_->rename(group, value.toString());
			return true;
		}

		return false;
	}

	void Model::iconsChanged() {
		emit layoutChanged();
	}

	void Model::setStatusIconProvider(StatusIconProvider* statusIconProvider) {
		if ( statusIconProvider_ ) {
			disconnect(statusIconProvider_, SIGNAL(updated()), this, SLOT(iconsChanged()));
		}
		statusIconProvider_ = statusIconProvider;
		connect(statusIconProvider_, SIGNAL(updated()), SLOT(iconsChanged()));
	}

	void Model::setViewActionsService(ViewActionsService* actionsService) {
		actionsService_ = actionsService;
	}

}

