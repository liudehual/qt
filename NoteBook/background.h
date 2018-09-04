#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QObject>
#include <QString>
class QTextEdit;
class MainWindow;
class QPrinter;

class BackGround : public QObject
{
    Q_OBJECT
public:
    explicit BackGround(QObject *parent = 0);
    virtual ~BackGround(){}
    void setTextEdit(QTextEdit *tEdit);
    void setMainWindow(MainWindow *min);
signals:

public slots:
    /* File menu */
    void myOpenFile();
    void mySaveFile();
    void mySaveAsFile();
    void myCreateNewFile();
    void myPageSet();
    void myPrintPreview();
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

    /*tool */
    void print(QPrinter *printer);
private: /*桌面控件*/
     QTextEdit *edit;
     MainWindow *window;
     QString fileName;
};

#endif // BACKGROUND_H
