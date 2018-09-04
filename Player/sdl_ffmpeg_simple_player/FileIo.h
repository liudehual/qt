
#include "Information.h"
#define INPUTFLVFILENAME     "..\\FLV_STREAM\\22.flv"

extern FILE * pVideo_Audio_Flv_File;

FILE * OpenFile(char * FileName,char * OpenMode);                        //打开文件
void   CloseFile(FILE * pFile);                                          //关闭文件
int    ReadFile(FILE * pFile ,unsigned char * Buffer,int BufferSize);    //读取操作
int    WriteFile(FILE * pFile ,char * Buffer,int BufferSize);            //文件写操作
