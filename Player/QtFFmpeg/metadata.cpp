#include "metadata.h"

MetaData::MetaData(void *parent,void *root)
{
    fParent=parent;
    fRoot=root;
}
MetaData::~MetaData()
{

}
void *MetaData::getParent()
{
    return fParent;
}

void *MetaData::getRoot()
{
    return fRoot;
}
