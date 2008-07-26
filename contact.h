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
			const QIcon getIcon() const;
			const QIcon& getAvatar() const;
			const QString getStatusMessage() const;

			void setName(const QString& name);
			void setJid(const QString& jid);
			void setIcon(const QIcon& icon);
			void setAvatar(const QIcon& avatar);

			void addResource(Resource* resource);

		private:
			QString name_;
			QString jid_;
			ShowType show_;
			QIcon avatar_;
	};
}

#endif
