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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QListWidgetItem>
#include "messageeditor.h"
#include "keyListEditor.h"
#include "message.h" //TESTING

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getHeaderHash(void);
    QString getMessageHash(void);
    QString getAttachHash(void);
    QString getTimeStamp(void);
    QString getClearTextHash(void);

    void setHeaderHash(QString hash);
    void setMessageHash(QString hash);
    void setAttachHash(QString hash);
    void setTimeStamp(QString hash);
    void setClearTextHash(QString hash);

private slots:
    void on_mainCreateMessageButton_clicked();

    void on_mainContactsButton_clicked();

    void on_mainGetNewMessagesButton_clicked();

    void inboxListFinishedSlot(QNetworkReply* reply);

    void headerDownloadFinishedSlot(QNetworkReply* reply);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_replyButton_clicked();



private:
    Ui::MainWindow *ui;
    MessageEditor *messageEditor;
    KeyListEditor *keyListEditor;
    QNetworkAccessManager* netmanager;

    QStringList newMsgHashes;
    QString gpgPath;

    QString rootPath;

    QString replyTo;

    QString headerHash;
    QString messageHash;
    QString attachHash;
    QString timestamp;
    QString timestampFileName;
    QString userPassword;
    QString clearTextHash;



    QList<message> messages;

    void downloadHeader(QString headerHash);
    void downloadMessage(QString msgHash);


    QString decryptHeader(QString headerHash);
    QString decryptMessage(QString messageHash);

    void updateTimeStamp(void);


    void addMessageToInbox(QString from, QString subject, QString date, QString firstLine, QString messageLink, QString attachmentLink);
    void addMessageToGui(message m);


    void downloadAttachment(QString attachHash);

    void firstLoadMessages();

};

#endif // MAINWINDOW_H
