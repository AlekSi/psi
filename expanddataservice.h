#ifndef ROSTER_EXPANDDATASERVICE_H
#define ROSTER_EXPANDDATASERVICE_H

#include <QString>
#include <QMap>

namespace Roster {

	class ExpandDataService {
		public:
			const bool isContactExpanded(const QString& jid, const QString& groupName) const;
			void setContactExpanded(const QString& jid, const QString& groupName, bool expanded);

			const bool isGroupExpanded(const QString& groupName) const;
			void setGroupExpanded(const QString& groupName, bool expanded);

		private:
			QMap<QPair<QString, QString>, bool> contacts_;
			QMap<QString, bool> groups_;
	};

}

#endif
