#ifndef MESSAGEEDITOR_H
#define MESSAGEEDITOR_H

#include <QDialog>
#include "sendprogressdialog.h"
#include "messagesender.h"

namespace Ui {
class MessageEditor;
}

class MessageEditor : public QDialog
{
    Q_OBJECT

public:
    explicit MessageEditor(QWidget *parent = 0);
    ~MessageEditor();

private slots:
    void on_messageWindowCancelButton_clicked();
    void on_attachmentFileBrowseButton_clicked();
    void on_messageWindowSendButton_clicked();


private:
    Ui::MessageEditor *ui;

    SendProgressDialog* sendProgress;
    messageSender* myMessageSender;

    QString attachmentFileName;
    QString headerFileName;
    QString messageFileName;

    QString myEmailAddress;

    void assembleHeader();
    void assembleMessage();
    void assembleAttachment();

    void uploadMessage();
};

#endif // MESSAGEEDITOR_H
