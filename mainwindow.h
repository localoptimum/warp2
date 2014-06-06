#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "messageeditor.h"

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

private:
    Ui::MainWindow *ui;
    MessageEditor *messageEditor;
};

#endif // MAINWINDOW_H
