#ifndef ROSTER_RESOURCE_H
#define ROSTER_RESOURCE_H

#include <QString>

#include "item.h"
#include "globals.h"

namespace Roster {

	// single contact on roster
	class Resource : public Item {
		public:
			Resource(const QString& name, const int priority, const StatusType status, const QString statusMessage = "");
			~Resource();

			const QString& getName() const;
			const QString& getStatusMessage() const;
			const int getPriority() const;
			const StatusType getStatus() const;
			const QString& getClientOS() const;
			const QString& getClientVersion() const;
			const QString& getClientName() const;
			const QString& getTune() const;

			void setName(const QString& name);
			void setStatusMessage(const QString& statusMessage);
			void setPriority(const int priority);
			void setStatus(const StatusType status);
			void setClientInfo(const QString& clientOS, const QString& clientVersion, const QString& clientName);
			void setTune(const QString& tune);

		private:
			QString name_;
			int priority_;
			StatusType status_;
			QString statusMessage_;
			QString tune_, clientVersion_, clientName_, clientOS_;
	};
}

#endif
