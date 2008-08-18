#ifndef ROSTER_ROSTERACTIONSSERVICE_H
#define ROSTER_ROSTERACTIONSSERVICE_H

#include <QString>

namespace XMPP {
	class Jid;
}

using XMPP::Jid; // PsiAccount uses Jid instead of XMPP::Jid

class PsiAccount;
class ConferenceBookmark;

//namespace Roster {

	class RosterActionsService {

		public:
			virtual void actionSendMessage(const XMPP::Jid& jid) = 0;
			virtual void actionOpenChat(const XMPP::Jid& jid) = 0;
			virtual void actionOpenChatSpecific(const XMPP::Jid& jid) = 0;
			virtual void actionSendFile(const XMPP::Jid& jid) = 0;
			virtual void actionRemove(const XMPP::Jid& jid) = 0;
			virtual void actionHistory(const XMPP::Jid& jid) = 0;
			virtual void actionOpenWhiteboard(const XMPP::Jid& jid) = 0;
			virtual void actionAuth(const XMPP::Jid& jid) = 0;
			virtual void actionAuthRequest(const XMPP::Jid& jid) = 0;
			virtual void actionAuthRemove(const XMPP::Jid& jid) = 0;
			virtual void actionSetMood() = 0;
			virtual void actionSetAvatar() = 0;
			virtual void actionUnsetAvatar() = 0;
			virtual void actionDisco(const XMPP::Jid& jid, const QString& node) = 0;
			virtual void actionSetManualAvatar(const XMPP::Jid& jid, const QString& file) = 0;
			virtual void actionUnsetManualAvatar(const XMPP::Jid& jid) = 0;
			virtual void actionRecvEvent(const XMPP::Jid& jid) = 0;
			virtual void actionManageBookmarks() = 0;
			virtual void actionJoin(const ConferenceBookmark& bookmark, bool connectImmediately) = 0;
			virtual void actionInvite(const XMPP::Jid& jid, const QString& groupchat) = 0;
			virtual void actionAssignKey(const XMPP::Jid& jid) = 0;
			virtual void actionUnassignKey(const XMPP::Jid& jid) = 0;
			virtual void actionGroupAdd(const XMPP::Jid& jid, const QString& group) = 0;
			virtual void actionGroupRemove(const XMPP::Jid& jid, const QString& group) = 0;
			virtual void actionRename(const XMPP::Jid& jid, const QString &) = 0;
			virtual void actionGroupRename(const QString& oldname, const QString& newname) = 0;
			virtual void actionSendMessage(const QList<XMPP::Jid>& jids) = 0;
			virtual void actionAdd(const XMPP::Jid& jid) = 0;
			
			virtual void actionGroupDelete(const QString& group) = 0;
			virtual void actionGroupDeleteAll(const QString& group) = 0;

			// FIXME: change this here and in PsiAccount
			virtual void openAddUserDlg() = 0;
			virtual void modify() = 0;
			virtual void changeStatus(int) = 0;
			virtual void showXmlConsole() = 0;
			virtual void actionExecuteCommand(const XMPP::Jid& jid, const QString& node = QString()) = 0;
			virtual void actionInfo(const XMPP::Jid& jid, bool showStatusInfo = true) = 0;
	};

//}

#endif
