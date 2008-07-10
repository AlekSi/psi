#ifndef ROSTER_RESOURCE_H
#define ROSTER_RESOURCE_H

#include <QString>

#include "item.h"

namespace Roster {

	// single contact on roster
	class Resource : public Item {
		public:
			Resource(const QString& name, const int priority, const QString status = "");
			~Resource();

			const QString& getName() const;
			const QString& getStatus() const;
			const int getPriority() const;
			const QIcon& getIcon() const;

			void setName(const QString& name);
			void setStatus(const QString& status);
			void setPriority(const int priority);
			void setIcon(const QIcon& icon);

		private:
			QString name_;
			int priority_;
			QString status_;
			QIcon icon_;
	};
}

#endif
