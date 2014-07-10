#ifndef KEYLISTEDITOR_H
#define KEYLISTEDITOR_H

#include <QDialog>

namespace Ui {
class KeyListEditor;
}

class KeyListEditor : public QDialog
{
    Q_OBJECT

public:
    explicit KeyListEditor(QWidget *parent = 0);
    ~KeyListEditor();

private slots:

    void on_exportPublicKeyButton_clicked();

    void on_doneButton_clicked();

    void on_newPrivateIDButton_clicked();

    void on_importContactButton_clicked();

private:
    Ui::KeyListEditor *ui;
};

#endif // KEYLISTEDITOR_H
