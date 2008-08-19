#include <QDebug>

#include "viewactionsservice.h"
#include "psiaccount.h"
#include "rosteractionsservice.h"
#include "contact.h"
#include "account.h"
#include "resource.h"
#include "conferencebookmark.h"
#include "group.h"
#include "metacontact.h"
#include "groupitem.h"
#include "notinlist.h"

namespace Roster {

	ViewActionsService::ViewActionsService() {
	}

	ViewActionsService::~ViewActionsService() {
	}

	void ViewActionsService::registerAccount(const QString& acname, PsiAccount* acc) {
		services_.insert(acname, acc);
	}

	void ViewActionsService::unregisterAccount(const QString& acname) {
		services_.remove(acname);
	}

	void ViewActionsService::sendMessage(Contact* contact) {
		services_[contact->getAccountName()]->actionSendMessage(contact->getJid());
	}

	void ViewActionsService::sendMessage(Group* group) {
		QList<XMPP::Jid> jids;
		foreach(Item* item, group->getItems()) {
			if ( Contact* contact= dynamic_cast<Contact*>(item) ) {
				jids.append(contact->getJid());
			} else if ( Metacontact* metacontact = dynamic_cast<Metacontact*>(item) ) {
				if ( metacontact->getNbItems() > 0 ) {
					jids.append(static_cast<Contact*>(metacontact->getItems().at(0))->getJid());
				}
			}
		}

		if ( group->getAccountName().isEmpty() ) {
			foreach(RosterActionsService* srv, services_) {
				if ( srv->isAvailable() ) {
					services_[group->getAccountName()]->actionSendMessage(jids);
					break;
				}
			}
		} else {
			services_[group->getAccountName()]->actionSendMessage(jids);
		}
	}

	void ViewActionsService::openChat(Contact* contact) {
		services_[contact->getAccountName()]->actionOpenChat(contact->getJid());
	}

	void ViewActionsService::sendFile(Contact* contact) {
		services_[contact->getAccountName()]->actionSendFile(contact->getJid());
	}

	void ViewActionsService::removeContact(Contact* contact) {
		services_[contact->getAccountName()]->actionRemove(contact->getJid());
	}

	void ViewActionsService::showHistory(Contact* contact) {
		services_[contact->getAccountName()]->actionHistory(contact->getJid());
	}

	void ViewActionsService::userInfo(Contact* contact) {
		services_[contact->getAccountName()]->actionInfo(contact->getJid());
	}

	void ViewActionsService::openWhiteboard(Contact* contact) {
		services_[contact->getAccountName()]->actionOpenWhiteboard(contact->getJid());
	}

	void ViewActionsService::resendAuthTo(Contact* contact) {
		services_[contact->getAccountName()]->actionAuth(contact->getJid());
	}

	void ViewActionsService::rerequestAuthFrom(Contact* contact) {
		services_[contact->getAccountName()]->actionAuthRequest(contact->getJid());
	}

	void ViewActionsService::removeAuthFrom(Contact* contact) {
		services_[contact->getAccountName()]->actionAuthRemove(contact->getJid());
	}

	void ViewActionsService::assignAvatar(Contact* contact, const QString& file) {
		services_[contact->getAccountName()]->actionSetManualAvatar(contact->getJid(), file);
	}

	void ViewActionsService::clearAvatar(Contact* contact) {
		services_[contact->getAccountName()]->actionUnsetManualAvatar(contact->getJid());
	}

	void ViewActionsService::changeStatus(Account* account, const StatusType status) {
		services_[account->getAccountName()]->changeStatus(status);
	}

	void ViewActionsService::newBlankMessage(Account* account) {
		XMPP::Jid jid;
		services_[account->getAccountName()]->actionSendMessage(jid);
	}

	void ViewActionsService::serviceDiscovery(Account* account) {
		services_[account->getAccountName()]->actionDisco(account->getJid(), "");
	}

	void ViewActionsService::mood(Account* account) {
		services_[account->getAccountName()]->actionSetMood();
	}

	void ViewActionsService::setAvatar(Account* account) {
		services_[account->getAccountName()]->actionSetAvatar();
	}

	void ViewActionsService::unsetAvatar(Account* account) {
		services_[account->getAccountName()]->actionUnsetAvatar();
	}

	void ViewActionsService::modifyAccount(Account* account) {
		services_[account->getAccountName()]->modify();
	}

	void ViewActionsService::addContact(Account* account) {
		services_[account->getAccountName()]->openAddUserDlg();
	}

	void ViewActionsService::xmlConsole(Account* account) {
		services_[account->getAccountName()]->showXmlConsole();
	}

	void ViewActionsService::openChat(Resource* resource) {
		XMPP::Jid jid = static_cast<Contact*>(resource->getParent())->getJid().withResource(resource->getName());
		services_[resource->getAccountName()]->actionOpenChatSpecific(jid);
	}

	void ViewActionsService::sendMessage(Resource* resource) {
		XMPP::Jid jid = static_cast<Contact*>(resource->getParent())->getJid().withResource(resource->getName());
		services_[resource->getAccountName()]->actionSendMessage(jid);
	}

	void ViewActionsService::openWhiteboard(Resource* resource) {
		XMPP::Jid jid = static_cast<Contact*>(resource->getParent())->getJid().withResource(resource->getName());
		services_[resource->getAccountName()]->actionOpenWhiteboard(jid);
	}

	void ViewActionsService::executeCommand(Resource* resource) {
		XMPP::Jid jid = static_cast<Contact*>(resource->getParent())->getJid().withResource(resource->getName());
		services_[resource->getAccountName()]->actionExecuteCommand(jid);
	}

	void ViewActionsService::recvEvent(Contact* contact) {
		services_[contact->getAccountName()]->actionRecvEvent(contact->getJid());
	}

	void ViewActionsService::setMOTD(Account* account) {
		XMPP::Jid jid = account->getJid().host() + "/announce/motd";
		services_[account->getAccountName()]->actionSendMessage(jid);
	}

	void ViewActionsService::updateMOTD(Account* account) {
		XMPP::Jid jid = account->getJid().host() + "/announce/motd/update";
		services_[account->getAccountName()]->actionSendMessage(jid);
	}

	void ViewActionsService::deleteMOTD(Account* account) {
		XMPP::Jid jid = account->getJid().host() + "/announce/motd/delete";
		services_[account->getAccountName()]->actionSendMessage(jid);
	}

	void ViewActionsService::onlineUsers(Account* account) {
		XMPP::Jid jid = account->getJid().host() + "/admin";
		services_[account->getAccountName()]->actionDisco(jid, "");
	}

	void ViewActionsService::sendServerMessage(Account* account) {
		XMPP::Jid jid = account->getJid().host() + "/announce/online";
		services_[account->getAccountName()]->actionSendMessage(jid);
	}

	void ViewActionsService::manageBookmarks(Account* account) {
		services_[account->getAccountName()]->actionManageBookmarks();
	}

	void ViewActionsService::joinConference(Account* account, ConferenceBookmark c) {
		services_[account->getAccountName()]->actionJoin(c, true);
	}

	void ViewActionsService::invite(Contact* contact, QString groupchat) {
		services_[contact->getAccountName()]->actionInvite(contact->getJid(), groupchat);
	}

	void ViewActionsService::assignKey(Contact* contact) {
		services_[contact->getAccountName()]->actionAssignKey(contact->getJid());
	}

	void ViewActionsService::unassignKey(Contact* contact) {
		services_[contact->getAccountName()]->actionUnassignKey(contact->getJid());
	}

	void ViewActionsService::moveToGroup(Contact* contact, const QString& target) {
		services_[contact->getAccountName()]->actionGroupRemove(contact->getJid(), contact->getGroupPath());
		services_[contact->getAccountName()]->actionGroupAdd(contact->getJid(), target);
	}

	void ViewActionsService::copyToGroup(Contact* contact, const QString& target) {
		services_[contact->getAccountName()]->actionGroupAdd(contact->getJid(), target);
	}

	void ViewActionsService::moveToNone(Contact* contact) {
		services_[contact->getAccountName()]->actionGroupRemove(contact->getJid(), contact->getGroupPath());
	}

	void ViewActionsService::rename(Contact* contact, const QString& name) {
		services_[contact->getAccountName()]->actionRename(contact->getJid(), name);
	}

	void ViewActionsService::rename(Group* group, const QString& name) {
		if ( group->getAccountName().isEmpty() ) {
			foreach(RosterActionsService* srv, services_) {
				srv->actionGroupRename(group->getGroupPath(), name);
			}
		} else {
			services_[group->getAccountName()]->actionGroupRename(group->getGroupPath(), name);
		}
	}

	void ViewActionsService::remove(Group* group) {
		if ( group->getAccountName().isEmpty() ) {
			foreach(RosterActionsService* srv, services_) {
				if ( srv->isAvailable() ) {
					srv->actionGroupDelete(group->getGroupPath());
				}
			}
		} else {
			services_[group->getAccountName()]->actionGroupDelete(group->getGroupPath());
		}
	}

	void ViewActionsService::removeAll(Group* group) {
		if ( group->getAccountName().isEmpty() ) {
			foreach(RosterActionsService* srv, services_) {
				if ( srv->isAvailable() ) {
					srv->actionGroupDeleteAll(group->getGroupPath());
				}
			}
		} else {
			services_[group->getAccountName()]->actionGroupDeleteAll(group->getGroupPath());
		}
	}

	void ViewActionsService::moveGroup(Group* group, GroupItem* parent) {
		QString target = parent->getGroupPath();
		if ( ! target.isEmpty() ) {
			target.append(SEPARATOR);
		}
		target.append(group->getName());
		rename(group, target);
	}

	void ViewActionsService::addAuthorize(NotInList* nil) {
		services_[nil->getAccountName()]->actionAdd(nil->getJid());
		services_[nil->getAccountName()]->actionAuth(nil->getJid());
	}

}
