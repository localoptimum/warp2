#include "message.h"
#include <QString>

message::message(QString sender, QString subject, QString date, QString firstLine, QString messageLink, QString attachmentLink){
    this->sender = sender;
    this->subject = subject;
    this->firstLine = firstLine;
    this->date = date;
    this->messageLink = messageLink;
    this->attachmentLink = attachmentLink;
}
QString message::getMessageLink(){
    return messageLink;
}
QString message::getAttachmentLink(){
    return attachmentLink;
}
QString message::getFirstLine(){
    return firstLine;
}

QString message::getSender(){
    return sender;
}
QString message::getSubject(){
    return subject;
}
QString message::getDate(){
    return date;
}
