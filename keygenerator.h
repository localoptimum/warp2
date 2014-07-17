#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QObject>

#include <QProcess>

class keyGenerator : public QObject
{
    Q_OBJECT
public:
    explicit keyGenerator(QObject *parent = 0);

    int createKeys(QString fullusername, QString simpleusername, QString password);

private:
    QProcess gpg;


signals:
    void keyGenerated();
public slots:
        void readSlot();
        void errorSlot();

};

#endif // KEYGENERATOR_H
