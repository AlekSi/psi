#ifndef ROSTER_XMPPRESOURCE_H
#define ROSTER_XMPPRESOURCE_H

#include <QString>

namespace Roster {

	enum ShowType {
		STATUS_ONLINE,
		STATUS_AWAY,
		STATUS_CHAT,
		STATUS_DND,
		STATUS_XA,
		STATUS_OFFLINE
	};


	class XMPPResource {
		public:
			XMPPResource(const QString& name, const int priority, const ShowType show = STATUS_ONLINE, const QString status = "");
			~XMPPResource();

			const QString& getName() const;
			const QString& getStatus() const;
			const int getPriority() const;
			const ShowType getShow() const;

			void setName(const QString& name);
			void setStatus(const QString& status);
			void setPriority(const int priority);
			void setShow(const ShowType show);

		private:
			QString name_;
			int priority_;
			ShowType show_;
			QString status_;
	};
}

#endif
