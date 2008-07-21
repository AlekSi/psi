#ifndef ROSTER_RESOURCE_H
#define ROSTER_RESOURCE_H

#include <QString>

#include "item.h"
#include "globals.h"

namespace Roster {
	class XMPPResource;

	// single contact on roster
	class Resource : public Item {
		public:
			Resource(const QString& name, const int priority, const ShowType show, const QString status = "");
			Resource(const XMPPResource* xresource);
			~Resource();

			const QString& getName() const;
			const QString& getStatus() const;
			const int getPriority() const;
			const QIcon getIcon() const;
			const ShowType getShow() const;

			void setName(const QString& name);
			void setStatus(const QString& status);
			void setPriority(const int priority);

		private:
			QString name_;
			int priority_;
			ShowType show_;
			QString status_;
	};
}

#endif
