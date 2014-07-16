#ifndef IDCREATOR_H
#define IDCREATOR_H

#include <QDialog>

namespace Ui {
class IDCreator;
}

class IDCreator : public QDialog
{
    Q_OBJECT

public:
    explicit IDCreator(QWidget *parent = 0);
    ~IDCreator();
signals:
    void updateKeys(); //Hanna
private slots:

    void on_IDgenerateOK_clicked();
    void on_IDgenerateCancel_clicked();

    void keyGenerated(); //Hanna

private:
    Ui::IDCreator *ui;
};

#endif // IDCREATOR_H
