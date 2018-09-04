#ifndef CORE_H
#define CORE_H

#include <QThread>
#include "metadata.h"
#include "global.h"
class Core : public QThread,public MetaData
{
    Q_OBJECT
public:
    explicit Core(QObject *parent);
    explicit Core();
    int initFFmpegLib();
    virtual ~Core();
    void run();
signals:

public slots:

private:

};

#endif // CORE_H
