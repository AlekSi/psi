#include <QMenu>
#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QHeaderView>
#include <QMenuBar>
#include <QDropEvent>

#include "view.h"
#include "model.h"
#include "group.h"
#include "contact.h"
#include "roster.h"

namespace Roster {

	/* Look & feel, initializations, connects */
	View::View() {
		setContextMenuPolicy(Qt::CustomContextMenu);
		setDragEnabled(true);
		setAcceptDrops(true);
		setSelectionMode(ExtendedSelection);
		setHeaderHidden(true);
		setRootIsDecorated(false);
		setExpandsOnDoubleClick(false);
		setEditTriggers(QAbstractItemView::EditKeyPressed);

		setStyleSheet("QTreeView::branch { image: none; width: 0px } QTreeView { alternate-background-color: #D6EEFF; }"); // FIXME: externalize this
		setIndentation(5);
		//setAlternatingRowColors(true);

		initMenu();		

		/* view signals */
		connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(showContextMenu(const QPoint&)));
		connect(this, SIGNAL(activated(const QModelIndex&)), SLOT(doActivated(const QModelIndex&)));

		connect(this, SIGNAL(expanded(const QModelIndex&)), SLOT(itemExpanded(const QModelIndex&)));
		connect(this, SIGNAL(collapsed(const QModelIndex&)), SLOT(itemCollapsed(const QModelIndex&)));
	}

	/* initialize context menu actions */
	void View::initMenu() {
		/* group */
		sendMessageToGroupAct_ = new QAction(QIcon("icons/send.png"), tr("Send message to group"), this);
		connect(sendMessageToGroupAct_, SIGNAL(triggered()), this, SLOT(menuSendMessageToGroup()));
		renameGroupAct_ = new QAction(tr("Re&name"), this);
		connect(renameGroupAct_, SIGNAL(triggered()), this, SLOT(menuRename()));
		removeGroupAct_ = new QAction(QIcon("icons/remove.png"), tr("Remove group"), this);
		connect(removeGroupAct_, SIGNAL(triggered()), this, SLOT(menuRemoveGroup()));
		removeGroupAndContactsAct_ = new QAction(QIcon("icons/remove.png"),tr("Remove group and contacts"), this);
		connect(removeGroupAndContactsAct_, SIGNAL(triggered()), this, SLOT(menuRemoveGroupAndContacts()));


		/* contact */
		sendMessageAct_ = new QAction(QIcon("icons/send.png"), tr("Send &message"), this);
		connect(sendMessageAct_, SIGNAL(triggered()), this, SLOT(menuSendMessage()));
		historyAct_ = new QAction(QIcon("icons/history.png"), tr("&History"), this);
		connect(historyAct_, SIGNAL(triggered()), this, SLOT(menuHistory()));
		showResourcesAct_ = new QAction(tr("Show resources"), this);
		connect(showResourcesAct_, SIGNAL(triggered()), this, SLOT(menuShowResources()));
		hideResourcesAct_ = new QAction(tr("Hide resources"), this);
		connect(hideResourcesAct_, SIGNAL(triggered()), this, SLOT(menuHideResources()));
		renameContactAct_ = new QAction(tr("Re&name"), this);
		connect(renameContactAct_, SIGNAL(triggered()), this, SLOT(menuRename()));

		/* roster */
		xmlConsoleAct_ = new QAction(tr("&XML Console"), this);
		connect(xmlConsoleAct_, SIGNAL(triggered()), this, SLOT(menuXmlConsole()));
		goOnlineAct_ = new QAction(tr("Online"), this);
		connect(goOnlineAct_, SIGNAL(triggered()), this, SLOT(menuGoOnline()));
		goOfflineAct_ = new QAction(tr("Offline"), this);
		connect(goOfflineAct_, SIGNAL(triggered()), this, SLOT(menuGoOffline()));

		/* multiple contacts */
		sendToAllAct_ = new QAction(QIcon("icons/send.png"), tr("&Send to all"), this);
		connect(sendToAllAct_, SIGNAL(triggered()), this, SLOT(menuSendToAll()));
	}

	/* build and display context menu */
	void View::showContextMenu(const QPoint& position) {
		QModelIndex index = indexAt(position);
		if ( ! index.isValid() ) {
			return;
		}

		QMenu* menu = new QMenu();

		Q_ASSERT(index.data(Qt::UserRole).canConvert<Item*>());
		Item* item = index.data(Qt::UserRole).value<Item*>();

		if ( selectedIndexes().size() > 1 ) { // multiple items selected
			qDebug() << "Context menu opened for multiple contacts";

			menu->addAction(sendToAllAct_); 
		} else if ( Group* group = dynamic_cast<Group*>(item) ) { 
			qDebug() << "Context menu opened for group" << group->getName();
			// FIXME: disable removing / renaming if that's not user group (i.e. generic)
			menu->addAction(sendMessageToGroupAct_);
			menu->addAction(renameGroupAct_);
			menu->addSeparator();
			menu->addAction(removeGroupAct_);
			menu->addAction(removeGroupAndContactsAct_);
		} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) { 
			qDebug() << "Context menu opened for contact" << contact->getName();

			menu->addAction(sendMessageAct_);
			menu->addAction(historyAct_);
			if ( isExpanded(index) ) {
				menu->addAction(hideResourcesAct_);
			} else {
				menu->addAction(showResourcesAct_);
			}
			menu->addAction(renameContactAct_);
		} else if ( Roster* roster = dynamic_cast<Roster*>(item) ) {
			qDebug() << "Context menu opened for roster" << roster->getName();

			QMenu* statusMenu = new QMenu(tr("&Status"));
			statusMenu->addAction(goOnlineAct_);
			statusMenu->addAction(goOfflineAct_);
			menu->addMenu(statusMenu);

			menu->addAction(xmlConsoleAct_);
		}

		foreach(QAction* action, menu->actions()) {
			action->setData(QVariant::fromValue<Item*>(item));
		}
			
		menu->popup( this->mapToGlobal(position) );
	}

	/* trigger default action when user clicks on roster item */
	void View::doActivated(const QModelIndex& index) {
		Q_ASSERT(index.data(Qt::UserRole).canConvert<Item*>());
		Item* item = index.data(Qt::UserRole).value<Item*>();

		if ( dynamic_cast<Group*>(item) ) {
			Group* group = dynamic_cast<Group*>(item);
			qDebug() << "Default action triggered on group" << group->getName();
			setExpanded(index, !isExpanded(index));
		} else if ( dynamic_cast<Contact*>(item) ) {
			Contact* contact = dynamic_cast<Contact*>(item);
			qDebug() << "Default action triggered on contact" << contact->getName();
		} else if ( dynamic_cast<Roster*>(item) ) {
			setExpanded(index, !isExpanded(index));
			Roster* roster = dynamic_cast<Roster*>(item);
			qDebug() << "Default action triggered on roster" << roster->getName();
		}
	}

	/* slot triggered when user expands item */
	void View::itemExpanded(const QModelIndex& index) {
		Item* item = index.data(Qt::UserRole).value<Item*>();
		if ( Group* group = dynamic_cast<Group*>(item) ) {
			group->setOpen(true);
		}
	}

	/* slot triggered when user collapses item */
	void View::itemCollapsed(const QModelIndex& index) {
		Item* item = index.data(Qt::UserRole).value<Item*>();
		if ( Group* group = dynamic_cast<Group*>(item) ) {
			group->setOpen(false);
		}
	}

	/* menu action for (contact)->send message */
	void View::menuSendMessage() {
		QAction* action = static_cast<QAction*>(sender());
		Contact* contact = static_cast<Contact*>(action->data().value<Item*>());
		qDebug() << "send message to" << contact->getName();
	}

	/* menu action for (contact)->history */
	void View::menuHistory() {
		QAction* action = static_cast<QAction*>(sender());
		Contact* contact = static_cast<Contact*>(action->data().value<Item*>());
		qDebug() << "history for" << contact->getName();
	}

	/* menu action for (group)->send message to group */
	void View::menuSendMessageToGroup() {
		QAction* action = static_cast<QAction*>(sender());
		Group* group = static_cast<Group*>(action->data().value<Item*>());
		qDebug() << "send message to group" << group->getName();
	}

	/* menu action for (contact/group)->rename */
	void View::menuRename() {
		edit(senderItemIndex());
	}

	/* menu action for (roster)->status->online */
	void View::menuGoOnline() {
		QAction* action = static_cast<QAction*>(sender());
		Roster* roster = static_cast<Roster*>(action->data().value<Item*>());
		qDebug() << "go online on roster" << roster->getName();
	}

	/* menu action for (roster)->status->offline */
	void View::menuGoOffline() {
		QAction* action = static_cast<QAction*>(sender());
		Roster* roster = static_cast<Roster*>(action->data().value<Item*>());
		qDebug() << "go offline on roster" << roster->getName();
	}

	/* menu action for (roster)->xml console */
	void View::menuXmlConsole() {
		QAction* action = static_cast<QAction*>(sender());
		Roster* roster = static_cast<Roster*>(action->data().value<Item*>());
		qDebug() << "xml console on roster" << roster->getName();
	}

	/* menu action for (contacts)->send to all */
	void View::menuSendToAll() {
		qDebug() << "Send to all for following contacts";
		foreach(QModelIndex index, selectedIndexes()) {
			Item* item = index.data(Qt::UserRole).value<Item*>();
			Contact* contact = dynamic_cast<Contact*>(item);
			qDebug() << " + " << contact->getName();
		}
	}

	void View::menuShowResources() {
		QModelIndex index = senderItemIndex();
		expand(index);
	}

	void View::menuHideResources() {
		QModelIndex index = senderItemIndex();
		collapse(index);
	}

	void View::menuRemoveGroup() {
	}

	void View::menuRemoveGroupAndContacts() {
	}

	/* 
	 * returns QModelIndex of item on which context menu was called 
	 * it is NOT safe to call it if sender() is not QAction or if it doesn't contain Roster::Item* object inside
	 * returned index is not valid if item is already gone
	 */
	QModelIndex View::senderItemIndex() const {
		QAction* action = static_cast<QAction*>(sender());
		QModelIndex index;

		unsigned int id = action->data().value<Item*>()->getId();
		QModelIndexList indexList = model()->match(model()->index(0, 0, QModelIndex()), IdRole, id, 1, Qt::MatchWrap | Qt::MatchExactly | Qt::MatchRecursive);
		if ( ! indexList.isEmpty() ) {
			index = indexList.at(0);
		}
		return index;
	}
}

