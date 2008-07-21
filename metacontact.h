#ifndef ROSTER_METACONTACT_H
#define ROSTER_METACONTACT_H

#include <QString>
#include <QIcon>

#include "groupitem.h"

namespace Roster {

	class Metacontact : public GroupItem {
		public:
			Metacontact(const QString& name);

			const QString& getName() const;
			const QString getStatus() const;
			const QIcon getIcon() const;
			const QIcon& getAvatar() const;

			void setName(const QString& name);
			void setAvatar(const QIcon& avatar); 
			void setStatus(const QString& status);
		private:
			QString name_;
			QString status_;
			QIcon avatar_;
	};
}

#endif
