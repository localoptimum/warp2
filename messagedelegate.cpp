#include "MessageDelegate.h"

    messagedelegate::messagedelegate(QObject *parent)
    {

    }

    void messagedelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
            QRect r = option.rect;

            QPen fontPen(QColor::fromRgb(51,51,51), 1, Qt::SolidLine);
            QString read = index.data(Qt::UserRole+3).toString();

            if(option.state & QStyle::State_Selected){
                painter->setBrush(QColor::fromRgb(208,231,255));
            }else if(read.toStdString().compare("read")!=0){
                painter->setBrush(QColor::fromRgb(230,243,255));
            }else{
                //BACKGROUND ALTERNATING COLORS
                painter->setBrush( (index.row() % 2) ? Qt::white : QColor(253,253,253) );
            }
            painter->drawRect(r);
            painter->setPen(fontPen);

            //Get sender, subject, content, and date
            QString sender = index.data(Qt::DisplayRole).toString();
            QString subject = index.data(Qt::UserRole).toString();
            QString content = index.data(Qt::UserRole+1).toString();
            QString date = index.data(Qt::UserRole+2).toString();


            //SENDER
            r = option.rect.adjusted(10, 0, 20, -30);
            painter->setFont( QFont( "Lucida Grande", 12, QFont::Bold ) );
            painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, sender, &r);

            //DATE
            r = option.rect.adjusted(option.rect.width() -60, 0, 20, -30);
            painter->setFont( QFont( "Lucida Grande", 9, QFont::Normal ) );
            painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignBottom|Qt::AlignLeft, date, &r);

            //SUBJECT
            r = option.rect.adjusted(10, 20, -10, 0);
            painter->setFont( QFont( "Lucida Grande", 10, QFont::Normal ) );
            painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, subject, &r);

            //CONTENT
            r = option.rect.adjusted(10, 32, -10, 0);
            painter->setFont( QFont( "Lucida Grande", 9, QFont::Light) );
            painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, content, &r);
    }


    QSize messagedelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
        return QSize(200, 50);
    }

    messagedelegate::~messagedelegate()
    {

    }
