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
			Account(const QString& name);
			~Account();

			const QString& getName() const;
			const StatusType getStatus() const;

			void setName(const QString& name);
			void setStatus(const StatusType status);

		private:
			QString name_;
			StatusType status_;
	};

}

#endif
