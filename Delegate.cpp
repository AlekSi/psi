#include <QModelIndex>
#include <QPainter>
#include <QDebug>

#include "Delegate.h"
#include "Item.h"
#include "Contact.h"
#include "Roster.h"
#include "Group.h"

namespace Roster {
		void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
			Q_ASSERT(index.data().canConvert<Item*>());

			painter->save();

			if ( option.state & QStyle::State_Selected ) {
				painter->fillRect(option.rect, option.palette.highlight());
			}

			Item* item = index.data().value<Item*>(); 

			if ( dynamic_cast<Contact*>(item) ) {
				Contact* contact = dynamic_cast<Contact*>(item);
				painter->drawText(option.rect, Qt::AlignVCenter, contact->getName());
			} else if ( dynamic_cast<Group*>(item) ) {
				Group* group = dynamic_cast<Group*>(item);
				painter->drawText(option.rect, Qt::AlignVCenter | Qt::AlignHCenter, group->getName());
			} else if ( dynamic_cast<Roster*>(item) ) {
				Roster* roster = dynamic_cast<Roster*>(item);
				painter->drawText(option.rect, Qt::AlignVCenter, roster->getName());
			}

			painter->restore();
		}

		QSize Delegate::sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const {
			Q_UNUSED(option);
			Q_UNUSED(index);

			return QSize(1, 16);
		}
}
