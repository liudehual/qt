#ifndef METADATA_H
#define METADATA_H
#include <stdio.h>
class MetaData
{
public:
    MetaData(void *parent=NULL,void *root=NULL);
    virtual ~MetaData();
    void *getParent();
    void *getRoot();
private:
    void *fRoot;
    void *fParent;
};

#endif // METADATA_H
