#ifndef ROSTER_CONTACT_H
#define ROSTER_CONTACT_H

#include <QString>

#include "Item.h"

namespace Roster {

	// single contact on roster
	class Contact : public Item {
		public:
			Contact(const QString& name, const QString& jid);
			~Contact();

			const QString& getName() const;
			const QString& getJid() const;

			void setName(const QString& name);
			void setJid(const QString& jid);

		private:
			QString name_;
			QString jid_;
	};
}

#endif
