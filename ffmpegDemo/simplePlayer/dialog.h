#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QImage>
class DecodeThread;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:

    void displayVideo(QImage img);


private:
    Ui::Dialog *ui;
    DecodeThread *dt;
};

#endif // DIALOG_H
