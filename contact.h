#ifndef ROSTER_CONTACT_H
#define ROSTER_CONTACT_H

#include <QString>

#include "item.h"

namespace Roster {

	// single contact on roster
	class Contact : public Item {
		public:
			Contact(const QString& name, const QString& jid);
			~Contact();

			const QString& getName() const;
			const QString& getJid() const;
			const QIcon& getIcon() const;
			const QIcon& getAvatar() const;

			void setName(const QString& name);
			void setJid(const QString& jid);
			void setIcon(const QIcon& icon);
			void setAvatar(const QIcon& avatar);

		private:
			QString name_;
			QString jid_;
			QIcon icon_;
			QIcon avatar_;
	};
}

#endif
