#ifndef ROSTER_ACCOUNT_H
#define ROSTER_ACCOUNT_H

#include <QString>
#include <QIcon>

#include "groupitem.h"
#include "globals.h"

namespace Roster {

	class Account : public GroupItem {
		public:
			Account();
			Account(const QString& name, const XMPP::Jid& jid);
			~Account();

			const QString& getName() const;
			const StatusType getStatus() const;
			const XMPP::Jid& getJid() const;

			void setName(const QString& name);
			void setStatus(const StatusType status);
			void setJid(const XMPP::Jid& jid);

		private:
			QString name_;
			XMPP::Jid jid_;
			StatusType status_;
	};

}

#endif
