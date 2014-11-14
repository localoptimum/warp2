//Copyright (c) 2014, Phil Bentley
//All rights reserved.

//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:

//1. Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
//ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//The views and conclusions contained in the software and documentation are those
//of the authors and should not be interpreted as representing official policies,
//either expressed or implied, of the University of Uppsala or the European
//Spallation Source.

#include "messagedelegate.h"

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
            r = option.rect.adjusted(option.rect.width() -120, 0, 20, -30);
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
