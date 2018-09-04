#include "flvtag.h"
#include <stdio.h>
FLVTag::FLVTag():
                fTagIndex(-1),
                fTagBuf(NULL)
{

}
FLVTag::~FLVTag()
{
    delete [] fTagBuf;
}
