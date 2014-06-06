#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_messageeditor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mainCreateMessageButton_clicked()
{
    messageEditor = new MessageEditor(this);
    messageEditor -> show();
}
