#ifndef ROSTER_ACCOUNT_H
#define ROSTER_ACCOUNT_H

#include <QString>
#include <QIcon>

#include "groupitem.h"

namespace Roster {

	class Account : public GroupItem {
		public:
			Account();
			Account(const QString& name);
			~Account();

			const QString& getName() const;
			const QIcon& getIcon() const;

			void setName(const QString& name);
			void setIcon(const QIcon& icon);

		private:
			QString name_;
			QIcon icon_;
	};

}

#endif
