#ifndef ROSTER_DELEGATE_H
#define ROSTER_DELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>

namespace Roster {
	class Delegate : public QItemDelegate {
		Q_OBJECT

		public:
			Delegate(QWidget *parent = 0) : QItemDelegate(parent) {}

			void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
			QSize sizeHint (const QStyleOptionViewItem& option, const QModelIndex& index) const;
	};
}

#endif
