#ifndef ROSTER_ROSTER_H
#define ROSTER_ROSTER_H

#include <QString>
#include <QIcon>

#include "groupitem.h"

namespace Roster {

	class Roster : public GroupItem {
		public:
			Roster();
			~Roster();

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
