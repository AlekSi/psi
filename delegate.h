#ifndef ROSTER_DELEGATE_H
#define ROSTER_DELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>

namespace Roster {
	class Delegate : public QStyledItemDelegate {
		Q_OBJECT

		public:
			Delegate(QWidget *parent = 0) : QStyledItemDelegate(parent), showAvatars_(true), showStatus_(true) {}

			void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
			QSize sizeHint (const QStyleOptionViewItem& option, const QModelIndex& index) const;

		public slots:
			void setShowAvatars(bool showAvatars);
			void setShowStatus(bool shotStatus);


		private:
			bool showAvatars_, showStatus_;
	};
}

#endif
