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

namespace Roster {
	Model::Model(RosterList* rosterlist) : rosterlist_(rosterlist), showAvatars_(true), showStatus_(true) {
	}

	QVariant Model::data(const QModelIndex &index, int role) const {
		if ( ! index.isValid() ) {
			return QVariant();
		}

		Item* item = static_cast<Item*>(index.internalPointer());

		if ( role == Qt::DisplayRole ) { // name
			if ( dynamic_cast<Roster*>(item) ) {
				return dynamic_cast<Roster*>(item)->getName();
			} else if ( dynamic_cast<Group*>(item) ) {
				return dynamic_cast<Group*>(item)->getName();
			} else if ( dynamic_cast<Contact*>(item) ) {
				return dynamic_cast<Contact*>(item)->getName();
			} else if ( dynamic_cast<Resource*>(item) ) {
				return QString("%1 (%2)").arg(dynamic_cast<Resource*>(item)->getName(), QString::number(dynamic_cast<Resource*>(item)->getPriority()));
			}
		} else if ( role == Qt::DecorationRole ) { // left icon
			if ( dynamic_cast<Group*>(item) ) {
				if ( dynamic_cast<Group*>(item)->isOpen() ) {
					return QIcon("icons/groupopen.png");
				} else {
					return QIcon("icons/groupclose.png");
				}
			} else if ( dynamic_cast<Contact*>(item) ) {
				return dynamic_cast<Contact*>(item)->getIcon();
			} else if ( dynamic_cast<Roster*>(item) ) {
				return dynamic_cast<Roster*>(item)->getIcon();
			} else if ( dynamic_cast<Resource*>(item) ) {
				return dynamic_cast<Resource*>(item)->getIcon();
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
			if ( dynamic_cast<Contact*>(item) ) {
				return dynamic_cast<Contact*>(item)->getAvatar(); 
			}
		} else if ( role == StatusRole and showStatus_ ) { // status text
			if ( dynamic_cast<Contact*>(item) ) {
				return dynamic_cast<Contact*>(item)->getStatus();
			} else if ( dynamic_cast<Resource*>(item) ) {
				return dynamic_cast<Resource*>(item)->getStatus();
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
		QString tip;

		if ( dynamic_cast<Contact*>(item) ) {
			Contact* contact = dynamic_cast<Contact*>(item);
			tip += "<div style=\"white-space: pre\">";

			tip += QString("%1 &lt;%2&gt;\n").arg(Qt::escape(contact->getName()), Qt::escape(contact->getJid()));
			tip += QString("<img src=\":icons/online.png\"> <b>%1</b> (%2)\n").arg("resource", "5");
			
			if (! contact->getStatus().isEmpty()) {
				tip += "<u>Status message</u>\n";
				tip += Qt::escape(contact->getStatus()) + "\n";
			}

			tip += "</div>";
		} else if ( dynamic_cast<Roster*>(item) ) {
			Roster* roster = dynamic_cast<Roster*>(item);
			tip += roster->getName();
		} else {
			return QVariant();
		}

		return tip;
	}

	int Model::rowCount(const QModelIndex &parent) const {
		GroupItem* parentItem;

		if (parent.isValid()) {
			Item* item = static_cast<Item*>(parent.internalPointer());
			parentItem = dynamic_cast<GroupItem*>(item);
		}
		else {
			parentItem = rosterlist_;
		}

		return (parentItem ? parentItem->getNbItems() : 0);
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
			return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsSelectable;
		} else if ( dynamic_cast<Resource*>(item) ) {
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
		} else {
			return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
		}
	}

	int Model::columnCount(const QModelIndex& index) const {
		Q_UNUSED(index);
		return 1;
	}

	QModelIndex Model::index(int row, int column, const QModelIndex& parent) const {
		GroupItem* parentItem;
		if (parent.isValid()) {
			parentItem = static_cast<GroupItem*>(parent.internalPointer());
		} 
		else {
			parentItem = rosterlist_;
		}

		if ( row < parentItem->getNbItems() ) {
			Item* item = parentItem->getItem(row);
			return (item ? createIndex(row, column, item) : QModelIndex());
		}
		else {
			return QModelIndex();
		}
	}

	QModelIndex Model::parent(const QModelIndex& index) const {
		if (!index.isValid()) {
			return QModelIndex();
		}

		Item* item = static_cast<Item*>(index.internalPointer());
		GroupItem* parent = item->getParent();

		if (parent == rosterlist_) {
			return QModelIndex();
		}
		else {
			GroupItem* grandparent = parent->getParent();
			return createIndex(grandparent->getIndexOf(parent), 0, parent);
		}
	}

	QMimeData* Model::mimeData(const QModelIndexList &indexes) const {
		//QMimeData* mimeData = QAbstractItemModel::mimeData(indexes);
		QMimeData* mimeData = new QMimeData();

		QByteArray encodedText, encodedIds;
		QDataStream textStream(&encodedText, QIODevice::WriteOnly);
		QDataStream idStream(&encodedIds, QIODevice::WriteOnly);


		foreach (QModelIndex index, indexes) {
			if (index.isValid()) {
				Item* item = index.data(ItemRole).value<Item*>();
				idStream << index.data(IdRole).value<unsigned int>();
				if ( dynamic_cast<Contact*>(item) ) {
					textStream << dynamic_cast<Contact*>(item)->getName();
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

		Item* parentItem = parent.data(ItemRole).value<Item*>();

		if ( dynamic_cast<Contact*>(parentItem) ) {
			qDebug() << "Following items has been dropped on contact" << dynamic_cast<Contact*>(parentItem)->getName();
		} else if ( dynamic_cast<Group*>(parentItem) ) {
			qDebug() << "Following items has been dropped on group" << dynamic_cast<Group*>(parentItem)->getName();
		} else if ( dynamic_cast<Roster*>(parentItem) ) {
			qDebug() << "Following items has been dropped on roster" << dynamic_cast<Roster*>(parentItem)->getName();
		}
		
		QByteArray encodedData = data->data("application/x-psi-rosteritem");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

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
			if ( dynamic_cast<Contact*>(item) ) {
				qDebug() << "+ contact" << dynamic_cast<Contact*>(item)->getName();
			} else if ( dynamic_cast<Group*>(item) ) {
				qDebug() << "+ group" << dynamic_cast<Group*>(item)->getName();
			} 
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

}

