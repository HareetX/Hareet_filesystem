#pragma once
#include "XX_filesystem.h"

class FileSystem :
    public ACI
{
private:
    // 文件系统相关变量
    Disk disk; // 系统内的磁盘信息
    vector<Directory> dirGroup; // 目录组

    FILE* fr; // 虚拟文件的读指针
    FILE* fw; // 虚拟文件的写指针

    bool isLogin; // 是否已登录
    bool doFormat; // 是否需要格式化

    int cur_dir; // 当前目录序号

public:
    FileSystem();

    //// 调试用
    //void f_read(FILE* fpr);
    //void f_write(FILE* fpw);

    /*
    * 虚拟磁盘文件存取控制接口
    */
    bool readSysFile(); // 把磁盘文件数据读入disk
    bool writeSysFile(); // 把disk数据写入磁盘文件

    // 打开磁盘文件：若文件存在，则正常打开；若文件不存在，创建磁盘文件再打开
    bool openSysFile();
    // 关闭磁盘文件，释放句柄
    void closeSysFile();

    bool formatSysFile(); // 格式化磁盘文件

    /*
    * 目录组与系统磁盘交互接口
    */
    void readDirGroup();
    void writeDirGroup(); // TEST


    /*
    * 用户与文件系统交互接口
    */
    // 格式化相关
    void needFormat(bool doFormat); // 根据传入参数，修改是否需要格式化的标志
    bool isFormat(); // 传出文件系统格式化需求

    // 目录组相关 TODO
    void ls(); // 显示该目录下的所有文件信息（ls）
    void cd(int cur); // 转到该目录（cd）
    void touch(const char* name); // 在该目录下创建文件（touch）
    void rm_f();// 删除该目录下的某文件（rm_f）
    void mkdir(const char* name);// 在该目录下创建目录（mkdir）
    void rm_rf();// 删除该目录下的某目录（rm_rf）
};

