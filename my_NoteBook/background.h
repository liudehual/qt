#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QObject>
class QTextEdit;

class BackGround : public QObject
{
    Q_OBJECT
public:
    explicit BackGround(QObject *parent = 0);
    virtual ~BackGround(){}
    void setTextEdit(QTextEdit *tEdit);
signals:

public:
    /* File menu */
    void myOpenFile();
    void mySaveFile();
    void mySaveAsFile();
    void myCreateNewFile();
    void myPrint();

    /* Edit menu */
    void myRevoke();
    void myCut();
    void myCopy();
    void myPaste();
    void myDelete();
    void myFind();
    void myFindNext();
    void myReplace();
    void myGoto();
    void mySelectAll();
    void myDatatime();
    void myAboutFile();

    /*Format menu*/
    void myAutoWrap();
    void myFront();

    /* Check menu */
    void myStatus();


private:
     QTextEdit *edit;
};

#endif // BACKGROUND_H
