#pragma once
#include "XX_filesystem.h"

class FileSystem :
    public ACI
{
private:
    // �ļ�ϵͳ��ر���
    Disk disk;

    FILE* fr; // �����ļ��Ķ�ָ��
    FILE* fw; // �����ļ���дָ��

    bool isLogin; // �Ƿ��ѵ�¼
    bool doFormat; // �Ƿ���Ҫ��ʽ��

public:
    FileSystem();

    // ������
    void f_read(FILE* fpr);
    void f_write(FILE* fpw);

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
    * �û����ļ�ϵͳ�����ӿ�
    */
    // ��ʽ�����
    void needFormat(bool doFormat); // ���ݴ���������޸��Ƿ���Ҫ��ʽ���ı�־
    bool isFormat(); // �����ļ�ϵͳ��ʽ������
};
