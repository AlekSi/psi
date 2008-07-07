#include "xmpprosteritem.h"

namespace Roster {

	XMPPRosterItem::XMPPRosterItem() {
	}

	XMPPRosterItem::XMPPRosterItem(const QString& name, const QString& jid, const QList<QString>& groups) : name_(name), jid_(jid), groups_(groups) {
	}

	XMPPRosterItem::~XMPPRosterItem() {
	}

	const QString& XMPPRosterItem::getName() const {
		return name_;
	}

	const QString& XMPPRosterItem::getJid() const {
		return jid_;
	}

	const QList<QString>& XMPPRosterItem::getGroups() const {
		return groups_;
	}
}
