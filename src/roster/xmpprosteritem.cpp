#include "xmpprosteritem.h"
#include "xmppresource.h"
#include "userlist.h"

namespace Roster {

	XMPPRosterItem::XMPPRosterItem() {
	}

	XMPPRosterItem::XMPPRosterItem(const QString& name, const QString& jid, const QList<QString>& groups) : name_(name), jid_(jid), groups_(groups) {
	}

	XMPPRosterItem::XMPPRosterItem(const UserListItem* item) {
		if ( item->name().isEmpty() ) {
			name_ = item->jid().full();
		} else {
			name_ = item->name();
		}
		jid_ = item->jid().full();
		groups_ = item->groups();
	}

	XMPPRosterItem::~XMPPRosterItem() {
	}

	const QString& XMPPRosterItem::getName() const {
		return name_;
	}

	const QString& XMPPRosterItem::getJid() const {
		return jid_;
	}

	const QList<QString> XMPPRosterItem::getGroups() const {
		if ( groups_.isEmpty() ) {
			QList<QString> ret;
			ret << "General";
			return ret;	
		}
		return groups_;
	}

	const QList<XMPPResource*> XMPPRosterItem::getResources() const {
		return resources_.values();
	}

	void XMPPRosterItem::setResource(XMPPResource* resource) {
		resources_.insert(resource->getName(), resource);
	}
}
