#include "xmpprosteritem.h"
#include "xmppresource.h"

namespace Roster {

	XMPPRosterItem::XMPPRosterItem() {
	}

	XMPPRosterItem::XMPPRosterItem(const QString& name, const XMPP::Jid& jid, const QList<QString>& groups) : name_(name), jid_(jid), groups_(groups) {
	}

	XMPPRosterItem::~XMPPRosterItem() {
	}

	const QString& XMPPRosterItem::getName() const {
		return name_;
	}

	const XMPP::Jid& XMPPRosterItem::getJid() const {
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

	void XMPPRosterItem::setName(const QString& name) {
		name_ = name;
	}

	void XMPPRosterItem::setJid(const XMPP::Jid& jid) {
		jid_ = jid;
	}

	void XMPPRosterItem::setGroups(const QList<QString>& groups) {
		groups_ = groups;
	}

}
