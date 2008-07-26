#ifndef ROSTER_CONTACT_H
#define ROSTER_CONTACT_H

#include <QString>
#include <QIcon>

#include "groupitem.h"
#include "globals.h"

namespace Roster {
	class Resource;
	
	// single contact on roster
	class Contact : public GroupItem {
		public:
			Contact(const QString& name, const QString& jid);
			Contact(const Contact& c);
			~Contact();

			const QString& getName() const;
			const QString& getJid() const;
			const QIcon& getAvatar() const;
			const QString getStatusMessage() const;
			void setStatus(const StatusType status);

			void setName(const QString& name);
			void setJid(const QString& jid);
			void setAvatar(const QIcon& avatar);
			void setStatusMessage(const QString& statusMessage);
			const StatusType getStatus() const;

			void addResource(Resource* resource);

		private:
			QString name_;
			QString jid_;
			QIcon avatar_;
			QString statusMessage_;
			StatusType status_;
	};
}

#endif
