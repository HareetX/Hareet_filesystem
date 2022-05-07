#pragma once
#include "XX_filesystem.h"

class FileSystem :
    public ACI
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
    void writeDirGroup(); // TEST


    /*
    * �û����ļ�ϵͳ�����ӿ�
    */
    // ��ʽ�����
    void needFormat(bool doFormat); // ���ݴ���������޸��Ƿ���Ҫ��ʽ���ı�־
    bool isFormat(); // �����ļ�ϵͳ��ʽ������

    // Ŀ¼����� TODO
    void ls(); // ��ʾ��Ŀ¼�µ������ļ���Ϣ��ls��
    void cd(int cur); // ת����Ŀ¼��cd��
    void touch(const char* name); // �ڸ�Ŀ¼�´����ļ���touch��
    void rm_f();// ɾ����Ŀ¼�µ�ĳ�ļ���rm_f��
    void mkdir(const char* name);// �ڸ�Ŀ¼�´���Ŀ¼��mkdir��
    void rm_rf();// ɾ����Ŀ¼�µ�ĳĿ¼��rm_rf��
};

