#ifndef ROSTER_DELEGATE_H
#define ROSTER_DELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>

namespace Roster {
	class Delegate : public QStyledItemDelegate {
		Q_OBJECT

		public:
			Delegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

			void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
			void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
			QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
			void setEditorData(QWidget* editor, const QModelIndex& index) const;
		
		private:
			QRect nameRect(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	};
}

#endif
