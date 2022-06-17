#pragma once
#include "XX_filesystem.h"

class User
{
private:
    string username; // 用户名
    string password; // 密码
public:
    User(string username, string password);
    void setUser(string username, string password); // 根据传入的用户名和密码，设置该用户的用户名和密码
    bool check(string username, string password); // 检查传入的用户名和密码是否正确，若正确，返回true，否则返回false

    string getUsername(); // 返回该用户的用户名
};

class FileSystem
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

    /*
    * 虚拟磁盘文件存取控制接口
    */
    bool readSysFile(); // 把磁盘文件数据读入disk，若读取成功，返回true，否则返回false
    bool writeSysFile(); // 把disk数据写入磁盘文件，若写入成功，返回true，否则返回false
    
    // 打开磁盘文件：若文件存在，则正常打开；若文件不存在，创建磁盘文件再打开；若打开成功，返回true，否则返回false
    bool openSysFile();
    // 关闭磁盘文件，释放句柄
    void closeSysFile();

    bool formatSysFile(); // 格式化磁盘文件，若格式化成功，返回true，否则返回false

    /*
    * 目录组与系统磁盘交互接口
    */
    void readDirGroup();
    void writeDirGroup();


    /*
    * 用户与文件系统交互接口
    */
    // 格式化相关
    void needFormat(bool doFormat); // 根据传入参数，修改是否需要格式化的标志doFormat
    bool isFormat(); // 返回文件系统格式化需求doFormat

    // 登录接口
    bool is_Login(); // 返回系统的登录状态isLogin
    void Login(); // 登录文件系统

    // 目录组相关 TEST
    int find_dir(const char* name); // 根据目录名，从目录组中查找匹配的目录，返回索引
    int find_dir(int i_index); // 根据inode索引，从目录组中查找匹配的目录，返回索引
    int find_file(const char* name, int mode); // 根据文件名和文件类型，从当前目录找到对应的目录项，返回索引
    bool check_fname(const char* name, int mode); // 根据文件名和文件类型，检查当前目录的文件是否有重名

    int getDirTotalSize(int dir); // 根据传入的目录序号，递归计算目录下的文件的总大小
    
    string getDirPos(int cur); // 根据传入的当前目录序号，返回当前目录路径
    void printUserPos(); // 登录后，模仿Linux终端，打印“用户名@主机名: 当前目录路径$”
  
    bool cmd(string args); // 根据传入的指令，执行指令操作，若输入q，则函数返回false，否则返回true

    void ls();// 显示该目录下的所有文件信息（ls）
    void cd(int cur); // 根据传入的目录序号，转到该目录（cd）
    void touch(const char* name); // 根据传入的文件名，在该目录下创建对应文件（touch）
    void rm_f(const char* name); // 根据传入的文件名，删除该目录下的对应文件（rm_f）
    void mkdir(const char* name); // 根据传入的目录名，在该目录下创建对应目录（mkdir）
    void rm_rf(const char* name); // 根据传入的目录名，删除该目录下的对应目录（rm_rf）
    void openfile(const char* name); // 根据传入的文件名，打开一般文件（open）
    void df(); // 显示文件系统的磁盘使用情况
    void df_i(); // 显示文件系统的i节点使用情况
    void df_s(); // 显示文件系统的超级块使用情况
    void help(); // 显示帮助
  
};