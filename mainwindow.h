#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
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

private:
    Ui::MainWindow *ui;
    MessageEditor *messageEditor;
    KeyListEditor *keyListEditor;
    QNetworkAccessManager* netmanager;
};

#endif // MAINWINDOW_H
