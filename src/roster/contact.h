#ifndef ROSTER_CONTACT_H
#define ROSTER_CONTACT_H

#include <QString>
#include <QIcon>

#include "groupitem.h"
#include "globals.h"

class PsiEvent;

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
			PsiEvent* getIncomingEvent() const;

			void setName(const QString& name);
			void setJid(const XMPP::Jid& jid);
			void setAvatar(const QIcon& avatar);
			void setStatusMessage(const QString& statusMessage);
			void setStatus(const StatusType status);
			void setIncomingEvent(PsiEvent* event);

		private:
			QString name_;
			XMPP::Jid jid_;
			QIcon avatar_;
			QString statusMessage_;
			StatusType status_;
			PsiEvent* incomingEvent_;
	};
}

#endif
