#include <QDebug>

#include "psidataservice.h"
#include "psiaccount.h"
#include "userlist.h"
#include "avatars.h"

namespace Roster {

	PsiDataService::PsiDataService(PsiAccount* acc) : acc_(acc) {
		connect(acc_, SIGNAL(updateContact(const UserListItem&)), SLOT(updatedContact(const UserListItem&)));
		connect(acc_, SIGNAL(updatedAccount()), SLOT(updatedAccount()));
		connect(acc_, SIGNAL(removedGroup(const UserListItem*, const QString&)), SLOT(removedGroup(const UserListItem*, const QString&)));
		connect(acc_, SIGNAL(removedContact(const UserListItem*)), SLOT(removedContact(const UserListItem*)));
	}

	PsiDataService::~PsiDataService() {
	}

	const bool PsiDataService::isEnabled() const {
		return acc_->enabled();
	}

	const QList<UserListItem*> PsiDataService::getRosterItems() const {
		QList<UserListItem*> list;

		for ( uint i = 0; i < acc_->userList()->count(); i++ ) {
			list.append(acc_->userList()->at(i));
		}

		return list;
	}

	const UserListItem* PsiDataService::getSelf() const {
		return acc_->self();
	}

	const QIcon PsiDataService::getAvatar(const XMPP::Jid& jid) const {
		UserListItem* item = acc_->find(jid);
		if ( item ) {
			return QIcon(item->avatar());
		}
		
		return QIcon();
	}

	void PsiDataService::updatedContact(const UserListItem& item) {
		if ( item.isSelf() ) {
			emit selfUpdated(&item, acc_->jid().full());
		} else {
			emit itemUpdated(&item, acc_->jid().full());
		}
	}

	const bool PsiDataService::isTransport(const XMPP::Jid& jid) const {
		UserListItem* item = acc_->find(jid);

		if ( item ) {
			return item->isTransport();
		}

		return false;
	}

	void PsiDataService::updatedAccount() {
		emit accountUpdated(acc_->jid().full());
	}

	const StatusType PsiDataService::getStatus() const {
		return acc_->status().type();
	}

	const XMPP::Jid PsiDataService::getJid() const {
		return acc_->jid();
	}

	PsiEvent* PsiDataService::getIncomingEvent(const XMPP::Jid& jid) const {
		return acc_->eventQueue()->peek(jid);
	}

	const bool PsiDataService::hasManualAvatar(const XMPP::Jid& jid) const {
		return acc_->avatarFactory()->hasManualAvatar(jid);
	}

	void PsiDataService::removedGroup(const UserListItem* item, const QString& group) {
		emit groupRemoved(item, group, acc_->jid().full());
	}

	void PsiDataService::removedContact(const UserListItem* item) {
		emit itemRemoved(item, acc_->jid().full());
	}

}
