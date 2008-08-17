#ifndef ROSTER_VIEWDATASERVICE_H
#define ROSTER_VIEWDATASERVICE_H

class PsiAccount;
class ConferenceBookmark;

//namespace Roster {
	
	class ViewDataService {
		public:
			virtual bool isAvailable() const = 0;
			virtual bool hasPep() const = 0;
			virtual QList<ConferenceBookmark> conferences() = 0;
			virtual QStringList groupchats() const = 0;
			virtual QStringList groups() const = 0;
	};

//}

#endif
