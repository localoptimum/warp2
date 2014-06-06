#include "messageeditor.h"
#include "ui_messageeditor.h"

#include <QFileDialog>

MessageEditor::MessageEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageEditor)
{
    ui->setupUi(this);
}

MessageEditor::~MessageEditor()
{
    delete ui;
}

void MessageEditor::on_messageWindowCancelButton_clicked()
{
     QWidget::close();
}

void MessageEditor::on_attachmentFileBrowseButton_clicked()
{

    attachmentFileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "",
                                                     tr("Files (*.*)"));

    ui->attachmentFileBox->setText(attachmentFileName);
}
