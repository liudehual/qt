#ifndef METADATA_H
#define METADATA_H

#include "global.h"

class MetaData
{
public:
    MetaData(void *qRoot,void *qParent);
    virtual ~MetaData();
    /// 获取父对象
    void *getParent(){return fParent;}
    /// 获取根对象
    void *getRoot(){return fRoot;}
    /// 清除
    virtual void clean();
private:
    void *fRoot;
    void *fParent;
};

#endif // METADATA_H
