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
			Resource(const QString& name, const int priority, const StatusType status, const QString statusMessage = "");
			Resource(const XMPPResource* xresource);
			~Resource();

			const QString& getName() const;
			const QString& getStatusMessage() const;
			const int getPriority() const;
			const StatusType getStatus() const;

			void setName(const QString& name);
			void setStatusMessage(const QString& statusMessage);
			void setPriority(const int priority);
			void setStatus(const StatusType status);

		private:
			QString name_;
			int priority_;
			StatusType status_;
			QString statusMessage_;
	};
}

#endif
