#ifndef ROSTER_ROSTER_H
#define ROSTER_ROSTER_H

#include <QString>

#include "GroupItem.h"

namespace Roster {

	class Roster : public GroupItem {
		public:
			Roster();
			~Roster();

			const QString& getName() const;
			void setName(const QString& name );


		private:
			QString name_;
	};

}

#endif
