#ifndef MESSAGEEDITOR_H
#define MESSAGEEDITOR_H

#include <QDialog>

namespace Ui {
class MessageEditor;
}

class MessageEditor : public QDialog
{
    Q_OBJECT

public:
    explicit MessageEditor(QWidget *parent = 0);
    ~MessageEditor();

private slots:
    void on_messageWindowCancelButton_clicked();
    void on_attachmentFileBrowseButton_clicked();


private:
    Ui::MessageEditor *ui;
    QString attachmentFileName;
};

#endif // MESSAGEEDITOR_H
