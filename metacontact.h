#ifndef ROSTER_METACONTACT_H
#define ROSTER_METACONTACT_H

#include <QString>
#include <QIcon>

#include "groupitem.h"
#include "globals.h"

namespace Roster {

	class Metacontact : public GroupItem {
		public:
			Metacontact(const QString& name);

			const QString& getName() const;
			const QString getStatusMessage() const;
			const QIcon getIcon() const;
			const QIcon& getAvatar() const;
			const StatusType getStatus() const;

			void setName(const QString& name);
			void setAvatar(const QIcon& avatar); 
			void setStatusMessage(const QString& statusMessage);
			void setStatus(const StatusType status);
		private:
			QString name_;
			QString statusMessage_;
			QIcon avatar_;
			StatusType status_;
	};
}

#endif
