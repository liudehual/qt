#ifndef FLVTAG_H
#define FLVTAG_H
class FLVTagHeader
{
public:
    unsigned char fTagType;
    unsigned int fTagLen;
    unsigned int fTimeStamp;
    unsigned int fStreamId;
};
class FLVTag
{
public:
    FLVTag();
    ~FLVTag();


    FLVTagHeader fTagHeader;
    unsigned char *fTagBuf;

    /* 自定义扩展信息 */
    float fTagIndex;

};

#endif // FLVTAG_H
