#include "sendprogressdialog.h"
#include "ui_sendprogressdialog.h"

#include <QTime>

SendProgressDialog::SendProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendProgressDialog)
{
    ui->setupUi(this);
}


void SendProgressDialog::setTotalProgress(int prog)
{
    ui->sendTotalProgressBar->setValue(prog);
    tactilepause();
}

void SendProgressDialog::setAssembleFilesProgress(int prog)
{
    ui->sendAssembleFilesProgressBar->setValue(prog);
    tactilepause();
}

void SendProgressDialog::setUploadMessageProgress(int prog)
{
    ui->sendUploadMessageProgressBar->setValue(prog);
    tactilepause();
}

void SendProgressDialog::setUploadHeaderProgress(int prog)
{
    ui->sendUploadHeaderProgressBar->setValue(prog);
    tactilepause();
}

void SendProgressDialog::setUploadAttachmentProgress(int prog)
{
    ui->sendUploadAttachmentsProgressBar->setValue(prog);
    tactilepause();
}


SendProgressDialog::~SendProgressDialog()
{
    delete ui;
}

void SendProgressDialog::tactilepause()
{

        QTime dieTime = QTime::currentTime().addMSecs( 100 );
        while( QTime::currentTime() < dieTime )
        {
            QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        }

}
