#ifndef ROSTER_METACONTACT_H
#define ROSTER_METACONTACT_H

#include <QString>
#include <QIcon>

#include "groupitem.h"
#include "globals.h"

class PsiEvent;

namespace Roster {

	class Metacontact : public GroupItem {
		public:
			Metacontact(const QString& name);

			const QString& getName() const;
			const QString getStatusMessage() const;
			const QIcon& getAvatar() const;
			const StatusType getStatus() const;
			PsiEvent* getIncomingEvent() const;

			void setName(const QString& name);
			void setAvatar(const QIcon& avatar); 
			void setStatusMessage(const QString& statusMessage);
			void setStatus(const StatusType status);
			void setIncomingEvent(PsiEvent* event);

		private:
			QString name_;
			QString statusMessage_;
			QIcon avatar_;
			StatusType status_;
			PsiEvent* incomingEvent_;
	};
}

#endif
