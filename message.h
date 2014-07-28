#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
class message
{
public:
    message(QString sender, QString subject, QString date, QString firstLine, QString messageLink, QString attachmentLink);
    QString getMessageLink();
    QString getAttachmentLink();
    QString getSubject();
    QString getSender();
    QString getFirstLine();
    QString getDate();

private:
    QString messageLink;
    QString subject;
    QString sender;
    QString date;
    QString firstLine;
    QString attachmentLink;
};

#endif // MESSAGE_H
