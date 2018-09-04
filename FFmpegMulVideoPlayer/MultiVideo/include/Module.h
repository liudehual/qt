#ifndef MODULE_H
#define MODULE_H

/// 调用者回调函数(视频)
typedef void (*videoFrameHandler)(void *arg,char *buf,int width,int height);

/// 模块类
class Module
{
public:
  ~Module();

  /// 创建模块对象
  static Module *createNewModule();

  /// 初始化系统环境
  static int initOS();

  /// 初始化模块
  int initModule(void *arg,videoFrameHandler handler);

  /// 调用iniModule前，请先调用该函数，并写入sps,pps帧，用户解码库初始化
  int addVideoFrame(char *buf,int len);

  /// 设置视频的宽高
  int setWidthAndHeight(int w,int h);

  /// 设置视频的帧率
  int setFrameRate(int rate);

  /// 启动模块
  int startModule();

  /// 关闭模块
  int closeModule();

  /// 测试
  int setFileName(char *fileName);
protected:
   Module();
private:
  void *fVideoDecodeThread; /// 解码线程引用s
};

#endif // MODULE_H
