#ifndef SENDPROGRESSDIALOG_H
#define SENDPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class SendProgressDialog;
}

class SendProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendProgressDialog(QWidget *parent = 0);

    void setTotalProgress(int prog);
    void setAssembleFilesProgress(int prog);
    void setUploadMessageProgress(int prog);
    void setUploadAttachmentProgress(int prog);

    ~SendProgressDialog();

public slots:

private:
    Ui::SendProgressDialog *ui;

    void tactilepause();
};

#endif // SENDPROGRESSDIALOG_H
