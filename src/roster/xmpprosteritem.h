#ifndef ROSTER_XMPPROSTERITEM_H
#define ROSTER_XMPPROSTERITEM_H

#include <QList>
#include <QString>
#include <QMap>

#include "globals.h"

class UserListItem;

namespace Roster {
	class XMPPResource;

	class XMPPRosterItem {
		public:
			XMPPRosterItem();
			XMPPRosterItem(const QString& name, const XMPP::Jid& jid, const QList<QString>& groups);
			XMPPRosterItem(const UserListItem* item);
			~XMPPRosterItem();

			const QString& getName() const;
			const XMPP::Jid& getJid() const;
			const QList<QString> getGroups() const;
			const QList<XMPPResource*> getResources() const;

			void setResource(XMPPResource* resource);
		private:
			QString name_;
			XMPP::Jid jid_;
			QList<QString> groups_;
			QMap<QString, XMPPResource*> resources_;
	};

}

#endif
