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
    // �ļ�ϵͳ��ر���
    Disk disk; // ϵͳ�ڵĴ�����Ϣ
    vector<Directory> dirGroup; // Ŀ¼��

    FILE* fr; // �����ļ��Ķ�ָ��
    FILE* fw; // �����ļ���дָ��

    bool isLogin; // �Ƿ��ѵ�¼
    bool doFormat; // �Ƿ���Ҫ��ʽ��

    int cur_dir; // ��ǰĿ¼���

    char Cur_Host_Name[110]; // ������
    vector<User> userGroup; // �û��飨��ϵͳֻ��root�û��������ǵ�δ����չ�������������vector��
    int cur_user; // ��ǰ�û����

public:
    FileSystem();

    //// ������
    //void f_read(FILE* fpr);
    //void f_write(FILE* fpw);

    /*
    * ��������ļ���ȡ���ƽӿ�
    */
    bool readSysFile(); // �Ѵ����ļ����ݶ���disk
    bool writeSysFile(); // ��disk����д������ļ�
    
    // �򿪴����ļ������ļ����ڣ��������򿪣����ļ������ڣ����������ļ��ٴ�
    bool openSysFile();
    // �رմ����ļ����ͷž��
    void closeSysFile();

    bool formatSysFile(); // ��ʽ�������ļ�

    /*
    * Ŀ¼����ϵͳ���̽����ӿ�
    */
    void readDirGroup();
    void writeDirGroup();


    /*
    * �û����ļ�ϵͳ�����ӿ�
    */
    // ��ʽ�����
    void needFormat(bool doFormat); // ���ݴ���������޸��Ƿ���Ҫ��ʽ���ı�־
    bool isFormat(); // �����ļ�ϵͳ��ʽ������

    // ��¼�ӿ�
    bool is_Login(); // ����ϵͳ�ĵ�¼״̬
    void Login();

    // Ŀ¼����� TEST
    int find_dir(const char* name); // ����Ŀ¼������Ŀ¼���в���ƥ���Ŀ¼����������
    int find_file(const char* name, int mode); // �����ļ������ļ����ͣ��ӵ�ǰĿ¼�ҵ���Ӧ��Ŀ¼���������
    bool check_fname(const char* name, int mode); // �����ļ������ļ����ͣ���鵱ǰĿ¼���ļ��Ƿ�������
    
    string getDirPos(int cur); // ��ӡĿ¼λ��
    void printUserPos(); // ��¼�󣬴�ӡ�û�����Ŀ¼


    void ls();// ��ʾ��Ŀ¼�µ������ļ���Ϣ��ls��
    void cd(int cur); // ת����Ŀ¼��cd��
    void touch(const char* name); // �ڸ�Ŀ¼�´����ļ���touch��
    void rm_f(const char* name); // ɾ����Ŀ¼�µ�ĳ�ļ���rm_f��
    void mkdir(const char* name); // �ڸ�Ŀ¼�´���Ŀ¼��mkdir��
    void rm_rf(const char* name); // ɾ����Ŀ¼�µ�ĳĿ¼��rm_rf��
    void openfile(const char* name); // ��һ���ļ���open��
    void help(); // ��ʾ����
    bool cmd(string args); // ����ָ��
};

