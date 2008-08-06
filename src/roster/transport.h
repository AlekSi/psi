#ifndef ROSTER_TRANSPORT_H
#define ROSTER_TRANSPORT_H

#include "groupitem.h"
#include "globals.h"

namespace Roster {
	class Resource;
	
	class Transport : public GroupItem {
		public:
			Transport(const QString& name, const XMPP::Jid& jid);
			~Transport();

			const QString& getName() const;
			const XMPP::Jid& getJid() const;
			const StatusType getStatus() const;
//			const QIcon& getAvatar() const;
//			const QString getStatusMessage() const;

			void setName(const QString& name);
			void setJid(const XMPP::Jid& jid);
			void setStatus(const StatusType status);
//			void setAvatar(const QIcon& avatar);
//			void setStatusMessage(const QString& statusMessage);

			Resource* findResource(const QString& name) const;

		private:
			QString name_;
			XMPP::Jid jid_;
//			QIcon avatar_;
//			QString statusMessage_;
			StatusType status_;
	};
}

#endif
