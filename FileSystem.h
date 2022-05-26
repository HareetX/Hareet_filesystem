#pragma once
#include "XX_filesystem.h"

class User
{
private:
    string username;
    string password;
public:
    User(string username, string password);
    void setUser(string username, string password);
    bool check(string username, string password);

    string getUsername();
};

class FileSystem
//    public ACI
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

    char Cur_Host_Name[110]; // 主机名
    vector<User> userGroup; // 用户组（本系统只有root用户，但考虑到未来扩展的情况，所以用vector）
    int cur_user; // 当前用户序号

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
    void writeDirGroup();


    /*
    * 用户与文件系统交互接口
    */
    // 格式化相关
    void needFormat(bool doFormat); // 根据传入参数，修改是否需要格式化的标志
    bool isFormat(); // 传出文件系统格式化需求

    // 登录接口
    bool is_Login(); // 返回系统的登录状态
    void Login();

    // 目录组相关 TEST
    int find_dir(const char* name); // 根据目录名，从目录组中查找匹配的目录，返回索引
    int find_file(const char* name, int mode); // 根据文件名和文件类型，从当前目录找到对应的目录项，返回索引
    bool check_fname(const char* name, int mode); // 根据文件名和文件类型，检查当前目录的文件是否有重名
    
    string getDirPos(int cur); // 打印目录位置
    void printUserPos(); // 登录后，打印用户所在目录


    void ls();// 显示该目录下的所有文件信息（ls）
    void cd(int cur); // 转到该目录（cd）
    void touch(const char* name); // 在该目录下创建文件（touch）
    void rm_f(const char* name); // 删除该目录下的某文件（rm_f）
    void mkdir(const char* name); // 在该目录下创建目录（mkdir）
    void rm_rf(const char* name); // 删除该目录下的某目录（rm_rf）
    void openfile(const char* name); // 打开一般文件（open）
    void help(); // 显示帮助
    bool cmd(string args); // 输入指令
};

