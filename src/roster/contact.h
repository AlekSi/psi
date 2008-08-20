#ifndef ROSTER_CONTACT_H
#define ROSTER_CONTACT_H

#include <QString>
#include <QIcon>

#include "groupitem.h"
#include "globals.h"
#include "xmpp_rosteritem.h"

namespace Roster {
	// single contact on roster
	class Contact : public GroupItem {
		public:
			Contact(const QString& name, const XMPP::Jid& jid);
			Contact(const Contact& c);
			~Contact();

			const QString& getName() const;
			const XMPP::Jid& getJid() const;
			const QIcon& getAvatar() const;
			const QString getStatusMessage() const;
			const StatusType getStatus() const;
			const EventType getIncomingEvent() const;
			const bool hasPGPKey() const;
			const bool hasManualAvatar() const;
			const XMPP::Subscription getSubscription() const;

			void setName(const QString& name);
			void setJid(const XMPP::Jid& jid);
			void setAvatar(const QIcon& avatar);
			void setStatusMessage(const QString& statusMessage);
			void setStatus(const StatusType status);
			void setIncomingEvent(const EventType event);
			void setHasPGPKey(const bool h);
			void setHasManualAvatar(const bool h);
			void setSubscription(const XMPP::Subscription& subscription);

		private:
			QString name_;
			XMPP::Jid jid_;
			QIcon avatar_;
			QString statusMessage_;
			StatusType status_;
			EventType incomingEvent_;
			bool hasPGPKey_, hasManualAvatar_;
			XMPP::Subscription subscription_;
	};
}

#endif
