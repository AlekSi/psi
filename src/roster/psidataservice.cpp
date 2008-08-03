#include "psidataservice.h"
#include "xmpprosteritem.h"
#include "xmppresource.h"
#include "psiaccount.h"
#include "userlist.h"

namespace Roster {

	PsiDataService::PsiDataService(PsiAccount* acc) : acc_(acc) {
		connect(acc_, SIGNAL(updateContact(const UserListItem&)), SLOT(updatedContact(const UserListItem&)));
	}

	PsiDataService::~PsiDataService() {
	}

	const QList<XMPPRosterItem*> PsiDataService::getRosterItems() const {
		// FIXME: where to delete those?
		QList<XMPPRosterItem*> list;

		for ( uint i = 0; i < acc_->userList()->count(); i++ ) {
			UserListItem* item = acc_->userList()->at(i);

			if ( ! item->isTransport() ) {
				XMPPRosterItem* xitem = new XMPPRosterItem(item);

				for ( uint j = 0; j < item->userResourceList().count(); j++ ) {
					UserResource res = item->userResourceList().at(j);
					XMPPResource* xres = new XMPPResource(res);
					xitem->setResource(xres);
				}

				list.append(xitem);
			}
		}

		return list;
	}

	const QIcon PsiDataService::getAvatar(const QString& jid) const {
		return QIcon();
	}

	void PsiDataService::updatedContact(const UserListItem& item) {
		if ( ! item.inList() ) {
			return;
		}

		XMPPRosterItem* xitem = new XMPPRosterItem(&item);

		for ( uint j = 0; j < item.userResourceList().count(); j++ ) {
			UserResource res = item.userResourceList().at(j);
			XMPPResource* xres = new XMPPResource(res);
			xitem->setResource(xres);
		}

		emit itemUpdated(xitem, acc_->jid().full());
	}
}
