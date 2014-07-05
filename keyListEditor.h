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
    void on_toolButton_3_clicked();

    void on_toolButton_clicked();

private:
    Ui::KeyListEditor *ui;
};

#endif // KEYLISTEDITOR_H
