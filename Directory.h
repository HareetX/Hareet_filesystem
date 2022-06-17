#pragma once

class Dentry
{
private:
	
	int dentry_size; // ��Ŀ¼��ĳ���
	int i_index; // �ļ�inode����
	int f_mode; // �ļ�����
	int f_size; // �ļ���С
	string f_name; // �ļ���

public:
	Dentry(int index, int mode, int size, const char* str);
	Dentry();

	// ����Ŀ¼�����ԵĽӿ�
	void setIndex(int index); // ����Ŀ¼���Ӧ���ļ���inode����
	void setMode(int mode); // ����Ŀ¼���Ӧ���ļ����ļ�����
	void setF_Size(int size); // ����Ŀ¼���Ӧ���ļ����ļ���С
	void setName(const char* str); // ����Ŀ¼���Ӧ���ļ����ļ���
	void renewSize(); // Ŀ¼���С�Ǹ����������Լ���õ��ģ������������������


	// ���Ŀ¼�����ԵĽӿ�
	int getIndex(); // ����Ŀ¼��ָ����ļ���inode����
	int getMode(); // ����Ŀ¼��ָ����ļ����ļ�����
	int getF_Size(); // ����Ŀ¼��ָ����ļ����ļ���С
	string getName(); // ����Ŀ¼��ָ����ļ����ļ���
	int getSize(); // ����Ŀ¼��dentry�Ĵ�С
};

class Directory
{
private:
	string dir_name; // Ŀ¼��
	int i_index; // ��Ŀ¼�ļ���Ӧ��Inode����
	size_t dentry_num; // ����Ŀ¼��ĸ���
	int parent_dir; // �ϼ�Ŀ¼
	vector<Dentry> dentryGroup; // Ŀ¼����

public:
	Directory();

	// ����Ŀ¼���ԵĽӿ�
	void setName(const char* str); // ����Ŀ¼��
	void setI_Index(int index); // ���ø�Ŀ¼�ļ���Ӧ��Inode����
	void renewDentryNum(); // ���¸�Ŀ¼����Ŀ¼��ĸ���
	void setParentDir(int dir); // ���ø�Ŀ¼���ϼ�Ŀ¼��Ŀ¼���е����

	// ���ݴ�����ļ������ļ���С���ļ����ͣ�����Ŀ¼������Ŀ¼��ָ����ļ����ļ���С
	void setDentryFsize(const char* name, int fsize, int mode);
	// ���Ŀ¼
	void dir_clear();
	
	// ���Ŀ¼���ԵĽӿ�
	string getName(); // ����Ŀ¼��
	int getI_Index();// ���ظ�Ŀ¼�ļ���Ӧ��Inode����
	size_t getDentryNum(); // ���ظ�Ŀ¼����Ŀ¼��ĸ���
	int getParentDir(); // ���ظ�Ŀ¼���ϼ�Ŀ¼��Ŀ¼���е����

	int getDirSize(); // ���ظ�Ŀ¼���ܴ�С

	// ���ص�No��Ŀ¼��
	Dentry getDentry(int No);
	
	// �ڵ�ǰĿ¼�����ݴ�����ļ������ҵ��ļ�����������
	int find_file(const char* name, int mode);

	// ����һ��Ŀ¼����������Ŀ¼��
	void add_Dentry(Dentry dentry);
	
	// ���ݴ����Ŀ¼����ţ�ɾ����ӦĿ¼��
	void del_Dentry(int No);

	// ��ӡĿ¼��Ϣ
	void printDir();

};

