#include <QDebug>

#include "psidataservice.h"
#include "xmpprosteritem.h"
#include "xmppresource.h"
#include "psiaccount.h"
#include "userlist.h"

namespace Roster {

	PsiDataService::PsiDataService(PsiAccount* acc) : acc_(acc) {
		connect(acc_, SIGNAL(updateContact(const UserListItem&)), SLOT(updatedContact(const UserListItem&)));
		connect(acc_, SIGNAL(updatedAccount()), SLOT(updatedAccount()));
	}

	PsiDataService::~PsiDataService() {
	}

	const QList<XMPPRosterItem*> PsiDataService::getRosterItems() const {
		// FIXME: where to delete those?
		QList<XMPPRosterItem*> list;

		for ( uint i = 0; i < acc_->userList()->count(); i++ ) {
			UserListItem* item = acc_->userList()->at(i);


			XMPPRosterItem* xitem = buildRosterItem(item);

			for ( uint j = 0; j < item->userResourceList().count(); j++ ) {
				UserResource res = item->userResourceList().at(j);
				XMPPResource* xres = buildResource(res);
				xitem->setResource(xres);
			}

			list.append(xitem);
		}

		return list;
	}

	const XMPPRosterItem* PsiDataService::getSelf() const {
		UserListItem item = acc_->self();
		XMPPRosterItem* xitem = buildRosterItem(&item);

		for ( uint j = 0; j < item.userResourceList().count(); j++ ) {
			UserResource res = item.userResourceList().at(j);
			XMPPResource* xres = buildResource(res);
			xitem->setResource(xres);
		}

		return xitem;
	}

	const QIcon PsiDataService::getAvatar(const XMPP::Jid& jid) const {
		UserListItem* item = acc_->find(jid);
		if ( item ) {
			return QIcon(item->avatar());
		}
		
		return QIcon();
	}

	void PsiDataService::updatedContact(const UserListItem& item) {
		qDebug() << "updated contact" << item.jid().full();
		XMPPRosterItem* xitem = buildRosterItem(&item);

		for ( uint j = 0; j < item.userResourceList().count(); j++ ) {
			UserResource res = item.userResourceList().at(j);
			XMPPResource* xres = buildResource(res);
			xitem->setResource(xres);
		}

		if ( item.isSelf() ) {
			emit selfUpdated(xitem, acc_->jid().full());
		} else {
			emit itemUpdated(xitem, acc_->jid().full());
		}
	}

	const bool PsiDataService::isTransport(const XMPP::Jid& jid) const {
		UserListItem* item = acc_->find(jid);

		if ( item ) {
			return item->isTransport();
		}

		return false;
	}

	XMPPRosterItem* PsiDataService::buildRosterItem(const UserListItem* item) const {
		XMPPRosterItem* xitem = new XMPPRosterItem;

		if ( item->name().isEmpty() ) {
			xitem->setName(item->jid().full());
		} else {
			xitem->setName(item->name());
		}
		xitem->setJid(item->jid().full());
		xitem->setGroups(item->groups());

		return xitem;
	}

	XMPPResource* PsiDataService::buildResource(const UserResource& res) const {
		XMPPResource* xresource = new XMPPResource(res.name(), res.priority(), res.status().type(), res.status().status());
		return xresource;
	}

	void PsiDataService::updatedAccount() {
		emit accountUpdated(acc_->jid().full());
	}
}
