#pragma once
#include "Hareet_filesystem.h"

class User
{
private:
    string username; // �û���
    string password; // ����
public:
    User(string username, string password);
    void setUser(string username, string password); // ���ݴ�����û��������룬���ø��û����û���������
    bool check(string username, string password); // ��鴫����û����������Ƿ���ȷ������ȷ������true�����򷵻�false

    string getUsername(); // ���ظ��û����û���
};

class FileSystem
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

    /*
    * ��������ļ���ȡ���ƽӿ�
    */
    bool readSysFile(); // �Ѵ����ļ����ݶ���disk������ȡ�ɹ�������true�����򷵻�false
    bool writeSysFile(); // ��disk����д������ļ�����д��ɹ�������true�����򷵻�false
    
    // �򿪴����ļ������ļ����ڣ��������򿪣����ļ������ڣ����������ļ��ٴ򿪣����򿪳ɹ�������true�����򷵻�false
    bool openSysFile();
    // �رմ����ļ����ͷž��
    void closeSysFile();

    bool formatSysFile(); // ��ʽ�������ļ�������ʽ���ɹ�������true�����򷵻�false

    /*
    * Ŀ¼����ϵͳ���̽����ӿ�
    */
    void readDirGroup();
    void writeDirGroup();


    /*
    * �û����ļ�ϵͳ�����ӿ�
    */
    // ��ʽ�����
    void needFormat(bool doFormat); // ���ݴ���������޸��Ƿ���Ҫ��ʽ���ı�־doFormat
    bool isFormat(); // �����ļ�ϵͳ��ʽ������doFormat

    // ��¼�ӿ�
    bool is_Login(); // ����ϵͳ�ĵ�¼״̬isLogin
    void Login(); // ��¼�ļ�ϵͳ

    // Ŀ¼����� TEST
    int find_dir(const char* name); // ����Ŀ¼������Ŀ¼���в���ƥ���Ŀ¼����������
    int find_dir(int i_index); // ����inode��������Ŀ¼���в���ƥ���Ŀ¼����������
    int find_file(const char* name, int mode); // �����ļ������ļ����ͣ��ӵ�ǰĿ¼�ҵ���Ӧ��Ŀ¼���������
    bool check_fname(const char* name, int mode); // �����ļ������ļ����ͣ���鵱ǰĿ¼���ļ��Ƿ�������

    int getDirTotalSize(int dir); // ���ݴ����Ŀ¼��ţ��ݹ����Ŀ¼�µ��ļ����ܴ�С
    
    string getDirPos(int cur); // ���ݴ���ĵ�ǰĿ¼��ţ����ص�ǰĿ¼·��
    void printUserPos(); // ��¼��ģ��Linux�նˣ���ӡ���û���@������: ��ǰĿ¼·��$��
  
    bool cmd(string args); // ���ݴ����ָ�ִ��ָ�������������q����������false�����򷵻�true

    void ls();// ��ʾ��Ŀ¼�µ������ļ���Ϣ��ls��
    void cd(int cur); // ���ݴ����Ŀ¼��ţ�ת����Ŀ¼��cd��
    void touch(const char* name); // ���ݴ�����ļ������ڸ�Ŀ¼�´�����Ӧ�ļ���touch��
    void rm_f(const char* name); // ���ݴ�����ļ�����ɾ����Ŀ¼�µĶ�Ӧ�ļ���rm_f��
    void mkdir(const char* name); // ���ݴ����Ŀ¼�����ڸ�Ŀ¼�´�����ӦĿ¼��mkdir��
    void rm_rf(const char* name); // ���ݴ����Ŀ¼����ɾ����Ŀ¼�µĶ�ӦĿ¼��rm_rf��
    void openfile(const char* name); // ���ݴ�����ļ�������һ���ļ���open��
    void df(); // ��ʾ�ļ�ϵͳ�Ĵ���ʹ�����
    void df_i(); // ��ʾ�ļ�ϵͳ��i�ڵ�ʹ�����
    void df_s(); // ��ʾ�ļ�ϵͳ�ĳ�����ʹ�����
    void help(); // ��ʾ����
  
};

