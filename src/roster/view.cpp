#include <QMenu>
#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QHeaderView>
#include <QMenuBar>
#include <QDropEvent>
#include <QFileDialog>
#include <QMessageBox>

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
		} else if ( Group* group = dynamic_cast<Group*>(item) ) { 
			qDebug() << "Context menu opened for group" << group->getName();
		} else if ( dynamic_cast<Contact*>(item) ) { 
			bool isSelf = dynamic_cast<Self*>(item);
			bool isTransport = dynamic_cast<Transport*>(item);

			// Missing action: recieve event
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
				// Missing action: invite to chat
				menu->addSeparator();
			}

			if ( ! PsiOptions::instance()->getOption("options.ui.contactlist.lockdown-roster").toBool() and ! isSelf ) {
				menu->addAction(menuActions_["rename"]);
				if ( isTransport ) {
					// Missing action: log in
					// Missing action: log out
				} else {
					// Missing action: move to group
				}

				QMenu* authMenu = new QMenu(tr("Authorization"), menu); // FIXME: missing icon
				authMenu->addAction(menuActions_["resendAuthTo"]);
				authMenu->addAction(menuActions_["rerequestAuthFrom"]);
				authMenu->addAction(menuActions_["removeAuthFrom"]);
				menu->addMenu(authMenu);

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
				pictureMenu->addAction(menuActions_["clearAvatar"]); // FIXME: disable if not avatar assigned
				menu->addMenu(pictureMenu);
			}

			if ( PGPUtil::instance().pgpAvailable() and 
					PsiOptions::instance()->getOption("options.ui.menu.contact.custom-pgp-key").toBool() ) {
				// Missing action: gpg yes/no 	
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

			QMenu* avatarMenu = new QMenu(tr("Avatar"));
			avatarMenu->addAction(menuActions_["setAvatarAccount"]);
			avatarMenu->addAction(menuActions_["unsetAvatarAccount"]);
			menu->addMenu(avatarMenu);

			// Missing action: bookmarks

			menu->addAction(menuActions_["addContact"]);
			menu->addAction(menuActions_["serviceDiscovery"]);
			menu->addAction(menuActions_["newBlankMessage"]);
			menu->addSeparator();
			menu->addAction(menuActions_["xmlConsole"]);
			menu->addSeparator();
			menu->addAction(menuActions_["modifyAccount"]);
			menu->addSeparator();

			// Missing action: admin
		} else if ( dynamic_cast<Resource*>(item) ) {
			menu->addAction(menuActions_["openChat"]);
			menu->addAction(menuActions_["sendMessage"]);
			menu->addAction(menuActions_["whiteboard"]);
			menu->addAction(menuActions_["executeCommand"]);
		}

		foreach(QAction* action, menuActions_.values()) {
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
			actionsService_->openChat(contact);
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
			actionsService_->unsetAvatar(account);
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

		unsigned int id = action->data().value<Item*>()->getId();
		QModelIndexList indexList = model()->match(model()->index(0, 0, QModelIndex()), IdRole, id, 1, Qt::MatchWrap | Qt::MatchExactly | Qt::MatchRecursive);
		if ( ! indexList.isEmpty() ) {
			index = indexList.at(0);
		}
		return index;
	}

	template<typename T> T View::getActionItem() {
		QAction* action = static_cast<QAction*>(sender());
		return dynamic_cast<T>(action->data().value<Item*>());
	}

}

