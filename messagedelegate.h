#ifndef MESSAGEDELEGATE_H
#define MESSAGEDELEGATE_H

#include <QPainter>
#include <QAbstractItemDelegate>

class messagedelegate : public QAbstractItemDelegate
{
    public:
        messagedelegate(QObject *parent = 0);

        void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
        QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;

        virtual ~messagedelegate();

};

#endif // MESSAGEDELEGATE_H
