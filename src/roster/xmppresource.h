#ifndef ROSTER_XMPPRESOURCE_H
#define ROSTER_XMPPRESOURCE_H

#include <QString>

#include "globals.h"

class UserResource;

namespace Roster {

	class XMPPResource {
		public:
			XMPPResource(const QString& name, const int priority, const StatusType status = STATUS_OFFLINE, const QString statusMessage = "");
			XMPPResource(const UserResource& item);
			~XMPPResource();

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
