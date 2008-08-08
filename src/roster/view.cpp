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
#include "account.h"
#include "resource.h"
#include "manager.h"
#include "viewstatemanager.h"
#include "metacontact.h"
#include "psitooltip.h"
#include "transport.h"

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

	void View::setManager(Manager* manager) {
		manager_ = manager;
	}

	void View::setViewStateManager(ViewStateManager* vsm) {
		vsm_ = vsm;
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
		openChatAct_ = new QAction(QIcon("icons/start-chat.png"), tr("Open &chat window"), this);
		connect(openChatAct_, SIGNAL(triggered()), this, SLOT(menuOpenChat()));
		sendFileAct_ = new QAction(QIcon("icons/upload.png"), tr("Send file"), this);
		connect(sendFileAct_, SIGNAL(triggered()), this, SLOT(menuSendFile()));
		removeContactAct_ = new QAction(QIcon("icons/remove.png"), tr("Rem&ove"), this);
		connect(removeContactAct_, SIGNAL(triggered()), this, SLOT(menuRemoveContact()));

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

		/* account */
		xmlConsoleAct_ = new QAction(tr("&XML Console"), this);
		connect(xmlConsoleAct_, SIGNAL(triggered()), this, SLOT(menuXmlConsole()));
		goOnlineAct_ = new QAction(tr("Online"), this);
		connect(goOnlineAct_, SIGNAL(triggered()), this, SLOT(menuGoOnline()));
		goOfflineAct_ = new QAction(tr("Offline"), this);
		connect(goOfflineAct_, SIGNAL(triggered()), this, SLOT(menuGoOffline()));

		/* resource */
		sendMessageToResourceAct_ = new QAction(QIcon("icons/send.png"), tr("Send &message"), this);
		connect(sendMessageToResourceAct_, SIGNAL(triggered()), this, SLOT(menuSendMessageToResource()));
		openChatToResourceAct_ = new QAction(QIcon("icons/start-chat.png"), tr("Open &chat window"), this);
		connect(openChatToResourceAct_, SIGNAL(triggered()), this, SLOT(menuOpenChatToResource()));
		sendFileToResourceAct_ = new QAction(QIcon("icons/upload.png"), tr("Send file"), this);
		connect(sendFileToResourceAct_, SIGNAL(triggered()), this, SLOT(menuSendFileToResource()));

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
			menu->addAction(openChatAct_);
			menu->addSeparator();
			menu->addAction(sendFileAct_);
			menu->addSeparator();
			menu->addAction(renameContactAct_);
			menu->addAction(removeContactAct_);
			if ( isExpanded(index) ) {
				menu->addAction(hideResourcesAct_);
			} else {
				menu->addAction(showResourcesAct_);
			}
			menu->addSeparator();
			menu->addAction(historyAct_);
		} else if ( Account* account = dynamic_cast<Account*>(item) ) {
			qDebug() << "Context menu opened for account" << account->getName();

			QMenu* statusMenu = new QMenu(tr("&Status"));
			statusMenu->addAction(goOnlineAct_);
			statusMenu->addAction(goOfflineAct_);
			menu->addMenu(statusMenu);

			menu->addAction(xmlConsoleAct_);
		} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
			qDebug() << "Context menu opened for resource " << resource->getName();

			menu->addAction(sendMessageToResourceAct_);
			menu->addAction(openChatToResourceAct_);
			menu->addAction(sendFileToResourceAct_);
		} else if ( Transport* transport = dynamic_cast<Transport*>(item) ) {
			Q_UNUSED(transport);
			if ( isExpanded(index) ) {
				menu->addAction(hideResourcesAct_);
			} else {
				menu->addAction(showResourcesAct_);
			}
		}

		foreach(QAction* action, menu->actions()) {
			action->setData(QVariant::fromValue<Item*>(item));
		}
			
		menu->popup( this->mapToGlobal(position) );
	}

	/* trigger default action when user clicks on account item */
	void View::doActivated(const QModelIndex& index) {
		Q_ASSERT(index.data(Qt::UserRole).canConvert<Item*>());
		Item* item = index.data(Qt::UserRole).value<Item*>();

		if ( Group* group = dynamic_cast<Group*>(item) ) {
			qDebug() << "Default action triggered on group" << group->getName();
			setExpanded(index, !isExpanded(index));
		} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			qDebug() << "Default action triggered on contact" << contact->getName();
		} else if ( Account* account = dynamic_cast<Account*>(item) ) {
			setExpanded(index, !isExpanded(index));
			qDebug() << "Default action triggered on account" << account->getName();
		} else if ( Resource* resource = dynamic_cast<Resource*>(item) ) {
			qDebug() << "Default action triggered on resource" << resource->getName();
		} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
			qDebug() << "Default action triggered on metacontact" << metacontact->getName();
		}
	}

	void View::expandWithManager(const QModelIndex& index, bool expanded) {
		Item* item = index.data(Qt::UserRole).value<Item*>();
		if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			vsm_->setContactExpanded(contact, expanded);
		} else if ( Group* group = dynamic_cast<Group*>(item) ) {
			vsm_->setGroupExpanded(group, expanded);
		} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
			vsm_->setMetacontactExpanded(metacontact, expanded);
		} else if ( Account* account = dynamic_cast<Account*>(item) ) {
			vsm_->setAccountExpanded(account, expanded);
		}
	}

	/* slot triggered when user expands item */
	void View::itemExpanded(const QModelIndex& index) {
		Item* item = index.data(ItemRole).value<Item*>();
		GroupItem* groupItem = dynamic_cast<GroupItem*>(item);

		/* do not call if this item was already expanded */
		if ( groupItem and ! groupItem->isExpanded() ) {
			expandWithManager(index, true);
		};
	}

	/* slot triggered when user collapses item */
	void View::itemCollapsed(const QModelIndex& index) {
		Item* item = index.data(ItemRole).value<Item*>();
		GroupItem* groupItem = dynamic_cast<GroupItem*>(item);

		/* do not call if this item was already expanded */
		if ( groupItem and groupItem->isExpanded() ) {
			expandWithManager(index, false);
		};
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

	/* menu action for (account)->status->online */
	void View::menuGoOnline() {
		QAction* action = static_cast<QAction*>(sender());
		Account* account = static_cast<Account*>(action->data().value<Item*>());
		qDebug() << "go online on account" << account->getName();
	}

	/* menu action for (account)->status->offline */
	void View::menuGoOffline() {
		QAction* action = static_cast<QAction*>(sender());
		Account* account = static_cast<Account*>(action->data().value<Item*>());
		qDebug() << "go offline on account" << account->getName();
	}

	/* menu action for (account)->xml console */
	void View::menuXmlConsole() {
		QAction* action = static_cast<QAction*>(sender());
		Account* account = static_cast<Account*>(action->data().value<Item*>());
		qDebug() << "xml console on account" << account->getName();
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

	void View::menuSendFileToResource() {
	}

	void View::menuOpenChatToResource() {
	}

	void View::menuSendMessageToResource() {
	}

	void View::menuRemoveGroup() {
	}

	void View::menuRemoveContact() {
		qDebug() << "removing contact";
		Item* item = senderItemIndex().data(ItemRole).value<Item*>();
		qDebug() << "blah blah";
		if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			manager_->removeContact(contact);
		}
		qDebug() << "aaa";
	}

	void View::menuSendFile() {
	}

	void View::menuOpenChat() {
	}

	void View::menuRemoveGroupAndContacts() {
	}

	bool View::viewportEvent(QEvent* event) {
		if ( event->type() == QEvent::ToolTip ) {
			QHelpEvent* he = static_cast<QHelpEvent*>(event);

			QModelIndex index = indexAt(he->pos());
			if ( index.isValid() ) {
				PsiToolTip::showText(he->globalPos(), index.data(Qt::ToolTipRole).toString(), viewport());
			}

			event->setAccepted(true);
			return true;
		}

		return QTreeView::viewportEvent(event);
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

