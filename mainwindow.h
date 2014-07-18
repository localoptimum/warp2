#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QListWidgetItem>
#include "messageeditor.h"
#include "keyListEditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_mainCreateMessageButton_clicked();

    void on_mainContactsButton_clicked();

    void on_mainGetNewMessagesButton_clicked();

    void finishedSlot(QNetworkReply* reply);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_replyButton_clicked();

private:
    Ui::MainWindow *ui;
    MessageEditor *messageEditor;
    KeyListEditor *keyListEditor;
    QNetworkAccessManager* netmanager;

    QStringList newMsgHashes;


    void downloadHeader(QString headerHash);

    void decryptHeader(QString headerHash);

    void downloadMessage(QString msgHash);

    void downloadAttachment(QString attachHash);

    void loadMessages();//H

};

#endif // MAINWINDOW_H
