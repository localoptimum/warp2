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
