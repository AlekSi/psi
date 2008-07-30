#ifndef ROSTER_XMPPROSTERITEM_H
#define ROSTER_XMPPROSTERITEM_H

#include <QList>
#include <QString>
#include <QMap>

namespace Roster {
	class XMPPResource;

	class XMPPRosterItem {
		public:
			XMPPRosterItem();
			XMPPRosterItem(const QString& name, const QString& jid, const QList<QString>& groups);
			~XMPPRosterItem();

			const QString& getName() const;
			const QString& getJid() const;
			const QList<QString>& getGroups() const;
			const QList<XMPPResource*> getResources() const;

			void setResource(XMPPResource* resource);
		private:
			QString name_;
			QString jid_;
			QList<QString> groups_;
			QMap<QString, XMPPResource*> resources_;
	};

}

#endif
