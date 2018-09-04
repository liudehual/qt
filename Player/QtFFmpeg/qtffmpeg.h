#ifndef QTFFMPEG_H
#define QTFFMPEG_H

#include <QMainWindow>
#include "global.h"
#include "core.h"
namespace Ui {
class QtFFmpeg;
}

class QtFFmpeg : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtFFmpeg(QWidget *parent = 0);
    ~QtFFmpeg();

private:
    Ui::QtFFmpeg *ui;
    Core *fCore;
};

#endif // QTFFMPEG_H
