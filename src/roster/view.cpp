#include <QMenu>
#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QHeaderView>
#include <QMenuBar>
#include <QDropEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>

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
#include "viewactionsservice.h"
#include "psioptions.h"
#include "pgputil.h"
#include "self.h"
#include "viewdataservice.h"
#include "conferencebookmark.h"
#include "notinlist.h"

#include "psiiconset.h"

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
		setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

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

	void View::setViewActionsService(ViewActionsService* actionsService) {
		actionsService_ = actionsService;
	}

	/* initialize context menu actions */
	void View::initMenu() {
		struct {
			const char* name;
			const QString text;
			const char* slot;
			const char* icon;
		} actionlist[] = {
			{"sendMessage", tr("Send &message"), SLOT(menuSendMessage()), "psi/sendMessage"},
			{"executeCommand", tr("E&xecute command"), SLOT(menuExecuteCommand()), ""},
			{"openChat", tr("Open &chat window"), SLOT(menuOpenChat()), "psi/start-chat"},
			{"sendFile", tr("Send &file"), SLOT(menuSendFile()), "psi/upload"},
			{"removeContact", tr("Rem&ove"), SLOT(menuRemoveContact()), "psi/remove"},
			{"history", tr("&History"), SLOT(menuHistory()), "psi/history"},
			{"hideResources", tr("Hide resources"), SLOT(menuHideResources()), ""},
			{"showResources", tr("Show resources"), SLOT(menuShowResources()), ""},
			{"userInfo", tr("User &info"), SLOT(menuUserInfo()), "psi/vCard"},
			{"whiteboard", tr("Open a &whiteboard"), SLOT(menuOpenWhiteboard()), "psi/whiteboard"},
			{"rename", tr("Re&name"), SLOT(menuRename()), ""},
			{"resendAuthTo", tr("Resend authorization to"), SLOT(menuResendAuthTo()), ""},
			{"rerequestAuthFrom", tr("Rerequest authorization from"), SLOT(menuRerequestAuthFrom()), ""},
			{"removeAuthFrom", tr("Remove authorization from"), SLOT(menuRemoveAuthFrom()), ""},
			{"assignAvatar", tr("&Assign custom picture"), SLOT(menuAssignAvatar()), ""},
			{"clearAvatar", tr("&Clear custom picture"), SLOT(menuClearAvatar()), ""},
			{"goOnline", tr("Online"), SLOT(menuChangeStatus()), "status/online"},
			{"goOffline", tr("Offline"), SLOT(menuChangeStatus()), "status/offline"},
			{"goXA", tr("Not available"), SLOT(menuChangeStatus()), "status/xa"},
			{"goAway", tr("Away"), SLOT(menuChangeStatus()), "status/away"},
			{"goDND", tr("Do not disturb"), SLOT(menuChangeStatus()), "status/dnd"},
			{"goChat", tr("Free for chat"), SLOT(menuChangeStatus()), "status/chat"},
			{"goInvisible", tr("Invisible"), SLOT(menuChangeStatus()), "status/invisible"},
			{"mood", tr("Mood"), SLOT(menuMood()), ""},
			{"setAvatarAccount", tr("Set avatar"), SLOT(menuSetAvatarAccount()), ""},
			{"unsetAvatarAccount", tr("Unset avatar"), SLOT(menuUnsetAvatarAccount()), ""},
			{"addContact", tr("&Add a contact"), SLOT(menuAddContact()), "psi/addContact"},
			{"serviceDiscovery", tr("Service &Discovery"), SLOT(menuServiceDiscovery()), "psi/disco"},
			{"xmlConsole", tr("&XML Console"), SLOT(menuXmlConsole()), "psi/xml"},
			{"modifyAccount", tr("&Modify Account..."), SLOT(menuModifyAccount()), "psi/account"},
			{"newBlankMessage", tr("New &blank message"), SLOT(menuNewBlankMessage()), "psi/sendMessage"},
			{"onlineUsers", tr("Online users"), SLOT(menuOnlineUsers()), "psi/disco"},
			{"sendServerMessage", tr("Send server message"), SLOT(menuSendServerMessage()), "psi/sendMessage"},
			{"setMOTD", tr("Set MOTD"), SLOT(menuSetMOTD()), ""},
			{"updateMOTD", tr("Update MOTD"), SLOT(menuUpdateMOTD()), ""},
			{"deleteMOTD", tr("Delete MOTD"), SLOT(menuDeleteMOTD()), "psi/remove"},
			{"manageBookmarks", tr("Manage..."), SLOT(menuManageBookmarks()), ""},
			{"recvEvent", tr("&Receive incoming event"), SLOT(menuRecvEvent()), ""},
			{"assignKey", tr("Assign Open&PGP key"), SLOT(menuAssignKey()), "psi/gpg-yes"},
			{"unassignKey", tr("Unassign Open&PGP key"), SLOT(menuUnassignKey()), "psi/gpg-no"},
			{"addGroup", tr("&Create new..."), SLOT(menuAddGroup()), ""},
			{"sendToGroup", tr("Send message to group"), SLOT(menuSendMessage()), "psi/sendMessage"},
			{"removeGroup", tr("Remove group"), SLOT(menuRemoveGroup()), "psi/remove"},
			{"removeGroupAll", tr("Remove group and contacts"), SLOT(menuRemoveGroupAndContacts()), "psi/remove"},
			{"addAuthorize", tr("Add/Authorize to contact list"), SLOT(menuAddAuthorize()), "psi/addContact"},

			{"", tr(""), SLOT(menu()), ""}
		};

		for ( int i = 0; ! QString(actionlist[i].name).isEmpty(); i++ ) {
			menuActions_[actionlist[i].name] = new QAction(tr(actionlist[i].text), this);
			if ( ! QString(actionlist[i].icon).isEmpty() ) {
				// FIXME: use proxy for icon factory
				menuActions_[actionlist[i].name]->setIcon(IconsetFactory::iconPixmap(actionlist[i].icon));
			}
			connect(menuActions_[actionlist[i].name], SIGNAL(triggered()), this, actionlist[i].slot);
		}
	}

	// FIXME: magic strings sucks
	bool View::isSpecial(Group* group) const {
		return ( group->getGroupPath() == tr("General") or
				group->getGroupPath() == tr("Agents/Transports") or
				group->getGroupPath() == tr("Hidden") or
				group->getGroupPath() == tr("Always visible") or
				group->getGroupPath() == tr("Not in list") );
	}

	/* build and display context menu */
	void View::showContextMenu(const QPoint& position) {
		// FIXME: memory leaking here with undeleted actions and menus!
		QModelIndex index = indexAt(position);
		if ( ! index.isValid() ) {
			return;
		}

		QMenu* menu = new QMenu();

		Q_ASSERT(index.data(Qt::UserRole).canConvert<Item*>());
		Item* item = index.data(Qt::UserRole).value<Item*>();
		ViewDataService* dataService = item->getAccountName().isEmpty() ? NULL : getDataService(item);

		if ( selectedIndexes().size() > 1 ) { // multiple items selected
			qDebug() << "Context menu opened for multiple contacts";
		} else if ( Group* group = dynamic_cast<Group*>(item) ) { 
			bool available = false;
			if ( group->getAccountName().isEmpty() ) { // joined accounts
				foreach(ViewDataService* ds, dataServices_.values()) {
					if ( ds->isAvailable() ) {
						available = true;
						break;
					}
				}
			} else {
				available = dataService->isAvailable();
			}

			menu->addAction(menuActions_["sendToGroup"]);
			menu->addAction(menuActions_["rename"]);
			menuActions_["rename"]->setEnabled( (!isSpecial(group)) and available );
			menu->addSeparator();
			menu->addAction(menuActions_["removeGroup"]);
			menu->addAction(menuActions_["removeGroupAll"]);
		} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) { 
			bool isSelf = dynamic_cast<Self*>(item);
			bool isTransport = dynamic_cast<Transport*>(item);
			bool notInList = dynamic_cast<NotInList*>(item);

			if ( notInList and ! PsiOptions::instance()->getOption("options.ui.contactlist.lockdown-roster").toBool() ) {
				menu->addAction(menuActions_["addAuthorize"]);
				menuActions_["addAuthorize"]->setEnabled(dataService->isAvailable());
				menu->addSeparator();
			}
			if ( contact->getIncomingEvent() ) {
				menu->addAction(menuActions_["recvEvent"]);
				menu->addSeparator();
			}
			if ( PsiOptions::instance()->getOption("options.ui.message.enabled").toBool() ) {
				menu->addAction(menuActions_["sendMessage"]);
			}
			menu->addAction(menuActions_["openChat"]);
			menu->addAction(menuActions_["whiteboard"]);
			if ( PsiOptions::instance()->getOption("options.ui.menu.contact.active-chats").toBool() ) {
				// Missing action: active chats
			}
			menu->addSeparator();

			if ( ! isTransport ) {
				menu->addAction(menuActions_["sendFile"]);
				// Missing action: voice call	
				
				QMenu* inviteMenu = new QMenu(tr("Invite"));

				foreach(QString groupchat, dataService->groupchats()) {
					QAction* action = inviteMenu->addAction(groupchat, this, SLOT(menuInvite()));
					action->setData(item->getId());
					action->setProperty("name", groupchat);
				}

				menu->addMenu(inviteMenu);
				inviteMenu->setEnabled(!inviteMenu->isEmpty());
				menu->addSeparator();
			}

			if ( ! PsiOptions::instance()->getOption("options.ui.contactlist.lockdown-roster").toBool() and ! isSelf and ! notInList ) {
				menu->addAction(menuActions_["rename"]);
				menuActions_["rename"]->setEnabled(true);
				if ( isTransport ) {
					// Missing action: log in
					// Missing action: log out
				} else {
					// move to group
					QMenu* groupMenu = new QMenu(tr("&Group"));
					QAction* noneAction = groupMenu->addAction("&None", this, SLOT(menuMoveToNone()));
					noneAction->setData(item->getId());
					if ( contact->getGroupPath() == tr("General") ) {
						noneAction->setCheckable(true);
						noneAction->setChecked(true);
					}
					groupMenu->addSeparator();

					QStringList groups = dataService->groups();
					qSort(groups);
					groups.removeOne(tr("Hidden"));
					groups.removeOne(tr("Always visible"));
					groups.removeOne(tr("General"));
					groups.removeOne(tr("Not in list"));
					groups.removeOne(tr("Agents/Transports"));
					foreach(QString group, groups) {
						QAction* action = groupMenu->addAction(group, this, SLOT(menuMoveToGroup()));
						action->setData(item->getId());
						if ( contact->getGroupPath() == group ) {
							action->setCheckable(true);
							action->setChecked(true);
						}
					}
					groupMenu->addSeparator();
					groupMenu->addAction(tr("Hidden"), this, SLOT(menuMoveToGroup()))->setData(item->getId());
					groupMenu->addAction(tr("Always visible"), this, SLOT(menuMoveToGroup()))->setData(item->getId());
					groupMenu->addSeparator();
					groupMenu->addAction(menuActions_["addGroup"]);
					menu->addMenu(groupMenu);
				}

				if ( ! notInList ) {
					QMenu* authMenu = new QMenu(tr("Authorization"), menu); // FIXME: missing icon
					authMenu->addAction(menuActions_["resendAuthTo"]);
					authMenu->addAction(menuActions_["rerequestAuthFrom"]);
					authMenu->addAction(menuActions_["removeAuthFrom"]);
					menu->addMenu(authMenu);
				}

				menu->addAction(menuActions_["removeContact"]);
				menu->addSeparator();
			}

			if ( isExpanded(index) ) {
				menu->addAction(menuActions_["hideResources"]);
			} else {
				menu->addAction(menuActions_["showResources"]);
			}
			menu->addSeparator();

			if ( PsiOptions::instance()->getOption("options.ui.menu.contact.custom-picture").toBool() ) {
				QMenu* pictureMenu = new QMenu(tr("&Picture"), menu);
				pictureMenu->addAction(menuActions_["assignAvatar"]);
				pictureMenu->addAction(menuActions_["clearAvatar"]);
				menuActions_["clearAvatar"]->setEnabled(contact->hasManualAvatar());
				menu->addMenu(pictureMenu);
			}

			if ( PGPUtil::instance().pgpAvailable() and 
					PsiOptions::instance()->getOption("options.ui.menu.contact.custom-pgp-key").toBool() ) {
				if ( contact->hasPGPKey() ) {
					menu->addAction(menuActions_["unassignKey"]);
				} else {
					menu->addAction(menuActions_["assignKey"]);
				}
			}

			menu->addAction(menuActions_["userInfo"]);
			menu->addAction(menuActions_["history"]);
		} else if ( dynamic_cast<Account*>(item) ) {
			QMenu* statusMenu = new QMenu(tr("&Status"));
			statusMenu->addAction(menuActions_["goOnline"]);
			if ( PsiOptions::instance()->getOption("options.ui.menu.status.chat").toBool() ) {
				statusMenu->addAction(menuActions_["goChat"]);
			}
			statusMenu->addSeparator();
			statusMenu->addAction(menuActions_["goAway"]);
			if ( PsiOptions::instance()->getOption("options.ui.menu.status.xa").toBool() ) {
				statusMenu->addAction(menuActions_["goXA"]);
			}
			statusMenu->addAction(menuActions_["goDND"]);
			statusMenu->addSeparator();
			if ( PsiOptions::instance()->getOption("options.ui.menu.status.invisible").toBool() ) {
				statusMenu->addSeparator();
				statusMenu->addAction(menuActions_["goInvisible"]);
			}
			statusMenu->addAction(menuActions_["goOffline"]);
			menu->addMenu(statusMenu);

			menu->addAction(menuActions_["mood"]);
			menuActions_["mood"]->setEnabled(dataService->hasPep());

			QMenu* avatarMenu = new QMenu(tr("Avatar"));
			avatarMenu->addAction(menuActions_["setAvatarAccount"]);
			avatarMenu->addAction(menuActions_["unsetAvatarAccount"]);
			menu->addMenu(avatarMenu);
			avatarMenu->setEnabled(dataService->hasPep());

			QMenu* bookmarkMenu = new QMenu(tr("Bookmarks"));
			bookmarkMenu->addAction(menuActions_["manageBookmarks"]);
			bookmarkMenu->addSeparator();

			bookmarkActions_.clear();

			foreach(ConferenceBookmark c, dataService->conferences()) {
				QAction* action = new QAction( QString("Join %1").arg(c.name()), bookmarkMenu );
				action->setData(item->getId());
				connect(action, SIGNAL(triggered()), SLOT(menuJoinConference()));
				bookmarkActions_.insert(action, c);
				bookmarkMenu->addAction(action);
			}
			menu->addMenu(bookmarkMenu);
			bookmarkMenu->setEnabled(dataService->isAvailable());

			menu->addAction(menuActions_["addContact"]);
			menu->addAction(menuActions_["serviceDiscovery"]);
			menu->addAction(menuActions_["newBlankMessage"]);
			menu->addSeparator();
			menu->addAction(menuActions_["xmlConsole"]);
			menu->addSeparator();
			menu->addAction(menuActions_["modifyAccount"]);

			if ( PsiOptions::instance()->getOption("options.ui.menu.account.admin").toBool() ) {
				menu->addSeparator();
				QMenu* adminMenu = new QMenu(tr("&Admin"));
				adminMenu->addAction(menuActions_["onlineUsers"]);
				adminMenu->addAction(menuActions_["sendServerMessage"]);
				adminMenu->addSeparator();
				adminMenu->addAction(menuActions_["setMOTD"]);
				adminMenu->addAction(menuActions_["updateMOTD"]);
				adminMenu->addAction(menuActions_["deleteMOTD"]);
				menu->addMenu(adminMenu);
				adminMenu->setEnabled(dataService->isAvailable());
			}

		} else if ( dynamic_cast<Resource*>(item) ) {
			menu->addAction(menuActions_["openChat"]);
			menu->addAction(menuActions_["sendMessage"]);
			menu->addAction(menuActions_["whiteboard"]);
			menu->addAction(menuActions_["executeCommand"]);
		}

		foreach(QAction* action, menuActions_.values()) {
			action->setData(item->getId());
		}
			
		menu->popup( this->mapToGlobal(position) );
	}

	/* trigger default action when user clicks on account item */
	void View::doActivated(const QModelIndex& index) {
		Q_ASSERT(index.data(Qt::UserRole).canConvert<Item*>());
		Item* item = index.data(Qt::UserRole).value<Item*>();

		if ( dynamic_cast<Group*>(item) ) {
			setExpanded(index, !isExpanded(index));
		} else if ( Contact* contact = dynamic_cast<Contact*>(item) ) {
			if ( contact->getIncomingEvent() ) {
				actionsService_->recvEvent(contact);
			} else if ( PsiOptions::instance()->getOption("options.messages.default-outgoing-message-type").toString() == "message") {
				actionsService_->sendMessage(contact);
			} else {
				actionsService_->openChat(contact);
			}
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
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->sendMessage(contact);
		} else if ( Resource* resource = getActionItem<Resource*>() ) {
			actionsService_->sendMessage(resource);
		} else if ( Group* group = getActionItem<Group*>() ) {
			actionsService_->sendMessage(group);
		}
	}

	void View::menuExecuteCommand() {
		if ( Resource* resource = getActionItem<Resource*>() ) {
			actionsService_->executeCommand(resource);
		}
	}

	void View::menuUserInfo() {
		if ( getActionItem<Contact*>() ) {
			actionsService_->userInfo(getActionItem<Contact*>());
		}
	}

	/* menu action for (contact)->history */
	void View::menuHistory() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->showHistory(contact);
		}
	}

	/* menu action for (contact/group)->rename */
	void View::menuRename() {
		edit(senderItemIndex());
	}

	/* menu action for (account)->xml console */
	void View::menuXmlConsole() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->xmlConsole(account);
		}
	}

	void View::menuShowResources() {
		QModelIndex index = senderItemIndex();
		if ( index.isValid() ) {
			expand(index);
		}
	}

	void View::menuHideResources() {
		QModelIndex index = senderItemIndex();
		if ( index.isValid() ) {
			collapse(index);
		}
	}

	void View::menuRemoveContact() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->removeContact(contact);
		}
	}

	void View::menuSendFile() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->sendFile(contact);
		}
	}

	void View::menuChangeStatus() {
		QAction* action = static_cast<QAction*>(sender());

		if ( Account* account = getActionItem<Account*>() ) {
			StatusType status = STATUS_OFFLINE;
			if ( action == menuActions_["goOnline"] ) {
				status = STATUS_ONLINE;
			} else if ( action == menuActions_["goChat"] ) {
				status = STATUS_CHAT;
			} else if ( action == menuActions_["goOffline"] ) {
				status = STATUS_OFFLINE;
			} else if ( action == menuActions_["goAway"] ) {
				status = STATUS_AWAY;
			} else if ( action == menuActions_["goXA"] ) {
				status = STATUS_XA;
			} else if ( action == menuActions_["goDND"] ) {
				status = STATUS_DND;
			} else if ( action == menuActions_["goInvisible"] ) {
				status = STATUS_INVISIBLE;
			}

			actionsService_->changeStatus(account, status);
		}
	}

	void View::menuOpenChat() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->openChat(contact);
		} else if ( Resource* resource = getActionItem<Resource*>() ) {
			actionsService_->openChat(resource);
		}
	}

	void View::menuOpenWhiteboard() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->openWhiteboard(contact);
		} else if ( Resource* resource = getActionItem<Resource*>() ) {
			actionsService_->openWhiteboard(resource);
		}
	}

	void View::menuResendAuthTo() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->resendAuthTo(contact);
			QMessageBox::information(this, tr("Authorize"), tr("Sent authorization to <b>%1</b>.").arg(contact->getName()));
		}
	}

	void View::menuRerequestAuthFrom() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->rerequestAuthFrom(contact);
			QMessageBox::information(this, tr("Authorize"), tr("Rerequested authorization from <b>%1</b>.").arg(contact->getName()));
		}
	}

	void View::menuRemoveAuthFrom() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			int n = QMessageBox::information(this, tr("Remove"), 
					tr("Are you sure you want to remove authorization from <b>%1</b>?").arg(contact->getName()),
					tr("&Yes"), tr("&No"));

			if ( n == 0 ) {
				actionsService_->removeAuthFrom(contact);
			}
		}
	}

	void View::menuClearAvatar() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->clearAvatar(contact);
		}
	}

	void View::menuRecvEvent() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->recvEvent(contact);
		}
	}

	void View::menuAssignAvatar() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			QString file = QFileDialog::getOpenFileName(this, tr("Choose an image"), "", tr("All files (*.png *.jpg *.gif)"));
			if ( ! file.isNull() ) {
				actionsService_->assignAvatar(contact, file);
			}
		}
	}

	void View::menuMood() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->mood(account);
		}
	}

	void View::menuSetAvatarAccount() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->setAvatar(account);
		}
	}

	void View::menuUnsetAvatarAccount() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->unsetAvatar(account);
		}
	}

	void View::menuAddContact() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->addContact(account);
		}
	}

	void View::menuServiceDiscovery() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->serviceDiscovery(account);
		}
	}

	void View::menuModifyAccount() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->modifyAccount(account);
		}
	}

	void View::menuNewBlankMessage() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->newBlankMessage(account);
		}
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

	/* copy-paste from contactview.cpp */
	void View::keyPressEvent(QKeyEvent* event) {
		int key = event->key();
		if ( key == Qt::Key_Home   || key == Qt::Key_End      ||
			 key == Qt::Key_PageUp || key == Qt::Key_PageDown ||
			 key == Qt::Key_Up     || key == Qt::Key_Down     ||
			 key == Qt::Key_Left   || key == Qt::Key_Right    ||
			 key == Qt::Key_Enter  || key == Qt::Key_Return) {
			QTreeView::keyPressEvent(event);
#ifdef Q_WS_MAC
		} else if (event->modifiers() == Qt::ControlModifier) {
			QTreeView::keyPressEvent(event);
#endif 
		} else {
			QString text = event->text().lower();
			if (text.isEmpty()) {
				QTreeView::keyPressEvent(event);
			}
			else {
				bool printable = true;
				foreach (QChar ch, text) {
					if (!ch.isPrint()) {
						QTreeView::keyPressEvent(event);
						printable = false;
						break;
					}
				}
				if (printable) {
					emit searchInput(text);
				}
			}
		}
	}

	/* 
	 * returns QModelIndex of item on which context menu was called 
	 * it is NOT safe to call it if sender() is not QAction or if it doesn't contain Roster::Item* object inside
	 * returned index is not valid if item is already gone
	 */
	QModelIndex View::senderItemIndex() const {
		QAction* action = static_cast<QAction*>(sender());
		QModelIndex index;

		unsigned int id = action->data().toInt();
		QModelIndexList indexList = model()->match(model()->index(0, 0, QModelIndex()), IdRole, id, 1, Qt::MatchWrap | Qt::MatchExactly | Qt::MatchRecursive);
		if ( ! indexList.isEmpty() ) {
			index = indexList.at(0);
		}
		return index;
	}

	template<typename T> T View::getActionItem() {
		// FIXME: this should return 0 when pointer is not valid anymore
		QAction* action = static_cast<QAction*>(sender());
		QModelIndexList indexList = model()->match(model()->index(0, 0, QModelIndex()), IdRole, action->data().toInt(), 1, 
				Qt::MatchWrap | Qt::MatchExactly | Qt::MatchRecursive);

		if ( ! indexList.isEmpty() ) {
			QModelIndex index = indexList.at(0);
			if ( index.isValid() ) {
				Item* item = index.data(ItemRole).value<Item*>();
				return dynamic_cast<T>(item);
			}
		}

		return NULL;
	}

	void View::registerAccount(const QString& acname, ViewDataService* ds) {
		dataServices_.insert(acname, ds);
	}

	void View::unregisterAccount(const QString& acname) {
		dataServices_.remove(acname);
	}

	ViewDataService* View::getDataService(Item* item) {
		return dataServices_[item->getAccountName()];
	}

	void View::menuOnlineUsers() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->onlineUsers(account);
		}
	}

	void View::menuSendServerMessage() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->sendServerMessage(account);
		}
	}

	void View::menuSetMOTD() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->setMOTD(account);
		}
	}

	void View::menuUpdateMOTD() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->updateMOTD(account);
		}
	}

	void View::menuDeleteMOTD() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->deleteMOTD(account);
		}
	}

	void View::menuManageBookmarks() {
		if ( Account* account = getActionItem<Account*>() ) {
			actionsService_->manageBookmarks(account);
		}
	}

	void View::menuJoinConference() {
		QAction* action = static_cast<QAction*>(sender());

		if ( Account* account = getActionItem<Account*>() ) {
			QMap<QAction*, ConferenceBookmark>::iterator it = bookmarkActions_.find(action); 
			if ( it != bookmarkActions_.end() ) {
				actionsService_->joinConference(account, it.value());
			}
		}
	}

	void View::menuInvite() {
		QAction* action = static_cast<QAction*>(sender());
		QString name = action->property("name").toString();

		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->invite(contact, name);
			QMessageBox::information(this, tr("Invitation"), tr("Sent groupchat invitation to <b>%1</b>.").arg(name));
		}
	}

	void View::menuAssignKey() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->assignKey(contact);
		}
	}

	void View::menuUnassignKey() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->unassignKey(contact);
		}
	}

	void View::menuMoveToGroup() {
		QAction* action = static_cast<QAction*>(sender());

		if ( Contact* contact = getActionItem<Contact*>() ) {
			if ( action->text() == tr("Hidden") ) {
				actionsService_->moveToGroup(contact, "Hidden");
			} else if ( action->text() == tr("Always visible") ) {
				actionsService_->moveToGroup(contact, "Always visible");
			} else {
				actionsService_->moveToGroup(contact, action->text());
			}
		}
	}

	void View::menuMoveToNone() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			actionsService_->moveToNone(contact);
		}
	}

	void View::menuAddGroup() {
		if ( Contact* contact = getActionItem<Contact*>() ) {
			bool ok = false;
			QString group = QInputDialog::getText(tr("Create New Group"), tr("Enter the new Group name:"), 
					QLineEdit::Normal, QString::null, &ok, this);
			if ( ! ok or group.isEmpty() ) {
				return;
			}
	
			actionsService_->moveToGroup(contact, group);	
		}
	}

	void View::menuRemoveGroup() {
		if ( Group* group = getActionItem<Group*>() ) {
			int n = QMessageBox::information(this, tr("Remove Group"),
					tr("This will cause all contacts in this group to be disassociated with it.\n\nProceed?"), tr("&Yes"), tr("&No"));
			if ( n == 0 ) {
				actionsService_->remove(group);
			}
		}
	}

	void View::menuRemoveGroupAndContacts() {
		if ( Group* group = getActionItem<Group*>() ) {
			int n = QMessageBox::information(this, tr("Remove Group and Contacts"),
					tr("WARNING!  This will remove all contacts associated with this group!\n\nProceed?"), tr("&Yes"), tr("&No"));
			if ( n == 0 ) {
				actionsService_->removeAll(group);
			}
		}
	}

	void View::menuAddAuthorize() {
		if ( NotInList* nil = getActionItem<NotInList*>() ) {
			actionsService_->addAuthorize(nil);
			QMessageBox::information(this, tr("Add"), tr("Added/Authorized <b>%1</b> to the contact list.").arg(nil->getName()));
		}
	}

}

