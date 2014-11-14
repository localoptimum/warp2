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

    void updateHeaderProgress(int value);
    void updateMessageProgress(int value);
    void updateAttachmentProgress(int value);
    void updateTotalProgress(int value);


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

    QString messageComposeToken;

    QString myEmailAddress;

    QString gpgPath;
    QString shaPath;

    void assembleHeader();
    void assembleMessage();
    void assembleAttachment();

    void uploadMessage();
};

#endif // MESSAGEEDITOR_H
