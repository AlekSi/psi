#include <QModelIndex>
#include <QPainter>
#include <QDebug>
#include <QFontMetrics>
#include <QStyledItemDelegate>

#include <QLineEdit>

#include "delegate.h"
#include "item.h"
#include "contact.h"
#include "roster.h"
#include "group.h"
#include "model.h"

/* FILE TODO:
 * + add more magic numbers
 * + get colors from options file
 */

namespace Roster {
	void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
		painter->save();
		painter->setClipping(false);

		if ( option.state & QStyle::State_Selected ) {
			painter->fillRect(option.rect, option.palette.highlight());
			painter->setPen(option.palette.highlightedText().color());
		} else {
			if ( index.data(Qt::BackgroundRole).canConvert<QColor>() ) {
				painter->fillRect(option.rect, index.data(Qt::BackgroundRole).value<QColor>());
			}
			painter->setPen(index.data(Qt::ForegroundRole).value<QColor>());
		}

		QRect rect(option.rect);

		/* icon */
		QRect iconRect(QPoint(rect.left(), rect.top()), QSize(20, rect.height()));
		index.data(Qt::DecorationRole).value<QIcon>().paint(painter, iconRect, Qt::AlignVCenter | Qt::AlignHCenter);
		rect.adjust(20, 0, 0, 0);

		/* avatar (if present) */
		if ( ! index.data(AvatarRole).value<QIcon>().isNull() ) {
			QRect avatarRect(QPoint(rect.right()-32, rect.top()), QSize(32, 34));
			index.data(AvatarRole).value<QIcon>().paint(painter, avatarRect, Qt::AlignVCenter | Qt::AlignHCenter);
			rect.adjust(0, 0, -32, 0);
		}

		/* name */
		QRect textRect(rect);
		if ( ! index.data(StatusRole).toString().isEmpty() ) {
			textRect.setHeight(16);
			rect.adjust(0, 16, 0, 0);
		}

		QFontMetrics fm = painter->fontMetrics();
		QString name = fm.elidedText(index.data(Qt::DisplayRole).toString(), Qt::ElideRight, textRect.width());
		painter->drawText(textRect, Qt::AlignVCenter, name);

		/* status message */
		if ( ! index.data(StatusRole).toString().isEmpty() ) {
			QFont statusFont;
			statusFont.setItalic(true);
			statusFont.setPointSize(8);

			QFontMetrics statusFm(statusFont);
			QRect statusRect(rect);
			QString status = statusFm.elidedText(index.data(StatusRole).toString(), Qt::ElideRight, statusRect.width());
			painter->setFont(statusFont);
			painter->drawText(statusRect, Qt::AlignVCenter, status);
		}

		painter->restore();
	}

	/* 
	 * Returns QRect on which item's name (Qt::DisplayRole) is drawn
	 * Used to correctly place inline editor
	 * Not nice, but hey, it works ;-)
	 */
	QRect Delegate::nameRect(const QStyleOptionViewItem& option, const QModelIndex& index) const {
		QRect rect(option.rect);
		rect.adjust(20, 0, 0, 0);

		if ( ! index.data(AvatarRole).value<QIcon>().isNull() ) {
			rect.adjust(0, 0, -32, 0);
		}

		if ( ! index.data(StatusRole).toString().isEmpty() ) {
			rect.setHeight(16);
		}

		return rect;
	}

	QWidget* Delegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
		Q_UNUSED(option);
		Q_UNUSED(index)

		QLineEdit* editor = new QLineEdit(parent);
		return editor;
	}

	void Delegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
		static_cast<QLineEdit*>(editor)->setText(index.data(Qt::EditRole).toString());
	}

	void Delegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const {
		editor->setGeometry(nameRect(option, index));
	}

	void Delegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
		Q_UNUSED(model);

		qDebug() << "Renaming" << index.data().toString() << "to" << static_cast<QLineEdit*>(editor)->text();
	}
}

