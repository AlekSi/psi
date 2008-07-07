#ifndef ROSTER_XMPPROSTERITEM_H
#define ROSTER_XMPPROSTERITEM_H

#include <QList>
#include <QString>

namespace Roster {

	class XMPPRosterItem {
		public:
			XMPPRosterItem();
			XMPPRosterItem(const QString& name, const QString& jid, const QList<QString>& groups);
			~XMPPRosterItem();

			const QString& getName() const;
			const QString& getJid() const;
			const QList<QString>& getGroups() const;
		private:
			QString name_;
			QString jid_;
			QList<QString> groups_;
	};

}

#endif
