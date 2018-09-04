#ifndef METADATA_H
#define METADATA_H

class MetaData
{
public:
    MetaData(void *pRoot,void *pParent);
    ~MetaData();
    void *getRoot(){return fRoot;}
    void *getParent(){return fParent;}
private:
    void *fRoot;
    void *fParent;
};

#endif // METADATA_H
