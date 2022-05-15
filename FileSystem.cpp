#pragma once
#include "XX_filesystem.h"


FileSystem::FileSystem()
{
	dirGroup.clear();
	fr = NULL;
	fw = NULL;
	isLogin = false;
	doFormat = false;
	cur_dir = 0;
}

bool FileSystem::is_Login()
{
	//cout << isLogin;
	return isLogin;
}

void inUsername(char username[])	//�����û���
{
	
	cout << "username: " ;
	cin >> username;
}

void inPasswd(char passwd[])	//��������
{
	cout << "passwd: ";
	cin >> passwd;
	//cin.ignore();
	//int i = 0;
	//while (true)
	//{
	//	passwd[i] = getch();     //ֻ�ܽ���һ������ 
	//	if (passwd[i] == '\r')     //�س�����ʾ\r\n 
	//	{
	//		break;
	//	}

	//}
}

bool check(char username[], char passwd[]) //�˶��û���������
{	

	if ((strcmp(username, "root") == 0) & (strcmp(passwd, "root") == 0)) {

		return 1;
	}
	else
	{
		return 0;
	}
}


void FileSystem::Login()	//��¼����
{
	 //ֻ�ƻ���һ���û�Ϊroot
	cout << "���ļ�ϵͳֻ��root�û����������û���root�Լ�����root��¼:" << endl;
	char username[100] = { 0 };
	char passwd[100] = { 0 };
	inUsername(username);	//�����û���
	inPasswd(passwd);		//�����û�����
	
	if (check(username, passwd)) {	//�˶��û���������
		isLogin = true;
		
	}
	else {
		isLogin = false;
		
	}
}




//void FileSystem::f_read(FILE* fpr)
//{
//	fr = fpr;
//}
//
//void FileSystem::f_write(FILE* fpw)
//{
//	fw = fpw;
//}

bool FileSystem::readSysFile()
{
	if (fr == NULL) { // û���Զ���ʽ���ļ�
		cout << "�ļ�δ�򿪣����ȴ��ļ� ..." << endl;
		return false;
	}
	else { // �ļ��Ѵ�
		// �Ѵ����ļ�����disk
		disk.disk_read(fr);
	}
	return true;
}

bool FileSystem::writeSysFile()
{
	if (fw == NULL) { // û����д��ʽ���ļ�
		cout << "�ļ�δ�򿪣����ȴ��ļ� ..." << endl;
		return false;
	}
	else { // �ļ��Ѵ�
		// ��disk����д������ļ�
		disk.disk_write(fw);
	}
	return true;
}

bool FileSystem::openSysFile()
{
	// ����ʽ����������ļ�
	if ((fr = fopen(FILESYSTEMNAME, "rb")) == NULL) {
		// ��������ļ�������

		// ������������ļ�
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) {
			// ��������ļ�����ʧ��
			cout << "��������ļ�����ʧ��..." << endl;
			return false;
		}
		// �����ļ������ɹ������´��ļ�
		fr = fopen(FILESYSTEMNAME, "rb");

		doFormat = true; // �����´����ļ������ʽ��
	}
	else { // ��������ļ�����
		// �Ѵ����ļ����ݶ���disk
		readSysFile();
		// д��ʽ���ļ�
		if ((fw = fopen(FILESYSTEMNAME, "wb")) == NULL) {
			// д��ʽ���ļ�ʧ��
			cout << " ��������ļ���ʧ��..." << endl;
			return false;
		}
		// д��ʽ���ļ��ɹ�����disk����д�ش����ļ� ��д��ʽ���ļ�������ļ���
		writeSysFile();
	}
	return true;
}

void FileSystem::closeSysFile()
{
	fclose(fr);
	fclose(fw);
	fr = NULL;
	fw = NULL;
}

bool FileSystem::formatSysFile()
{
	if (doFormat) {
		disk.disk_format(); // ��ʽ��disk
		return writeSysFile(); // �Ѹ�ʽ����diskд������ļ�
	}
	return true;
}

void FileSystem::readDirGroup()
{
	Dentry dentry;
	Directory dir = disk.dir_read(0);
	dir.setName("root");
	dir.setI_Index(0);
	dir.setParentDir(NULL);
	dirGroup.push_back(dir);
	int j = 0;
	while (1) {
		for (int i = 0; i < dirGroup[j].getDentryNum(); i++) {
			dentry = dirGroup[j].getDentry(i);
			if (dentry.getMode() == DIR_MODE) {
				int i_index = dentry.getIndex();
				dir = disk.dir_read(i_index);
				dir.setName(dentry.getName().c_str());
				dir.setI_Index(i_index);
				dir.setParentDir(j);
				dirGroup.push_back(dir);
			}
		}
		j++;
		if (j == dirGroup.size()) {
			break;
		}
	}
}

void FileSystem::writeDirGroup()
{
	Dentry dentry;
	disk.dir_write(0, dirGroup[0]);
	int k;
	int d_num;
	int dir_size = dirGroup.size();
	int d_index;
	for (int j = 0; j < dir_size; j++) {
		d_num = dirGroup[j].getDentryNum();
		for (int i = 0; i < d_num; i++) {
			dentry = dirGroup[j].getDentry(i);
			d_index = dentry.getIndex();
			if (dentry.getMode() == DIR_MODE) {
				for (k = 0; k < dir_size; k++) {
					if (d_index == dirGroup[k].getI_Index()) {
						break;
					}
				}
				disk.dir_write(d_index, dirGroup[k]);
			}
		}
	}
}


void FileSystem::ls()
{
	dirGroup[cur_dir].printDir();
}

void FileSystem::cd(int cur)
{
	if (cur == cur_dir) {
		cout << "���ڸ�Ŀ¼��" << endl;
	}
	else if (cur < dirGroup.size() && cur >= 0) {
		cur_dir = cur;
	}
	else {
		cout << "û���ҵ���Ŀ¼" << endl;
	}
}

void FileSystem::touch(const char* name)
{
	// ����Inode��Block
	int i_index = disk.d_ialloc();
	int b_index = disk.d_balloc();
	if (i_index == -1 || b_index == -1) {
		cout << "���̿ռ䲻�����޷������ļ�" << endl;
		return;
	}	
	
	// �ڵ�ǰĿ¼�����Ӧ��Ŀ¼��
	Dentry f_dentry(i_index, FILE_MODE, 0, name);
	dirGroup[cur_dir].add_Dentry(f_dentry);

	// ��Inode�У�Ŀ¼�ļ���СҪ����
	// ����Superblock������λͼ������Inode�������ļ�Inode�ĸ��£��͵�ǰĿ¼��Ӧ��Inode�ĸ��£�
	disk.use_renew(b_index, i_index, FILE_MODE, 0, dirGroup[cur_dir]);
	
	// ����ǰĿ¼д�����ݿ�
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
}

void FileSystem::rm_f(int i_index)
{
	// �ڵ�ǰĿ¼ɾ����Ӧ��Ŀ¼��
	int i;
	int d_num = dirGroup[cur_dir].getDentryNum();
	for (i = 0; i < d_num; i++) {
		if (dirGroup[cur_dir].getDentry(i).getIndex() == i_index) {
			break;
		}
	}
	if (i == d_num) {
		cout << "δ�ҵ���Ӧ�ļ�" << endl;
		return;
	}
	dirGroup[cur_dir].del_Dentry(i);
	
	// ����Superblock������λͼ������Inode�������ļ�Inode�ĸ��£��͵�ǰĿ¼��Ӧ��Inode�ĸ��£�
	disk.free_renew(i_index, dirGroup[cur_dir]);
	
	// ����ǰĿ¼д�����ݿ�
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
}


void FileSystem::mkdir(const char* name)
{
	// ����Inode��Block
	int i_index = disk.d_ialloc();
	int b_index = disk.d_balloc();
	if (i_index == -1 || b_index == -1) {
		cout << "���̿ռ䲻�����޷������ļ�" << endl;
		return;
	}

	// �ڵ�ǰĿ¼�����Ӧ��Ŀ¼��
	Directory dir;
	dir.setName(name);
	dir.setI_Index(i_index);
	dir.setParentDir(cur_dir);

	dirGroup.push_back(dir);

	int dir_size = dir.getDirSize();

	Dentry dir_dentry(i_index, DIR_MODE, dir_size, name);
	dirGroup[cur_dir].add_Dentry(dir_dentry);

	// ��Inode�У�Ŀ¼�ļ���СҪ����
	// ����Superblock������λͼ������Inode�������ļ�Inode�ĸ��£��͵�ǰĿ¼��Ӧ��Inode�ĸ��£�
	disk.use_renew(b_index, i_index, FILE_MODE, dir_size, dirGroup[cur_dir]);

	// ����ǰĿ¼д�����ݿ�
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
	disk.dir_write(i_index, dir);
}

void FileSystem::rm_rf(int i_index)
{
	// ע�⣺ɾ��Ŀ¼Ҫ�Ѹ�Ŀ¼�������ļ���ɾ���������õ��ݹ麯����
	// �ڵ�ǰĿ¼�ҵ���Ӧ��Ŀ¼��
	int i;
	int cur = cur_dir;
	int dir;
	int d_num = dirGroup[cur_dir].getDentryNum();
	for (i = 0; i < d_num; i++) {
		if (dirGroup[cur_dir].getDentry(i).getIndex() == i_index) {
			break;
		}
	}
	if (i == d_num) {
		cout << "δ�ҵ���Ӧ�ļ�" << endl;
		return;
	}
	

	// �ҵ�Ҫɾ����Ŀ¼
	for (dir = 0; dir < dirGroup.size(); dir++) {
		if (dirGroup[dir].getI_Index() == i_index) {
			break;
		}
	}
	cd(dir);
	Dentry dentry;
	for (int i = 0; i < dirGroup[cur_dir].getDentryNum(); i++) {
		dentry = dirGroup[cur_dir].getDentry(i);
		if (dentry.getMode() == FILE_MODE) {
			rm_f(dentry.getIndex());
		}
		else if (dentry.getMode() == DIR_MODE) {
			rm_rf(dentry.getIndex());
		}
	}
	cd(cur);

	// ɾ����ӦĿ¼���Ŀ¼
	dirGroup[cur_dir].del_Dentry(i);
	dirGroup.erase(dirGroup.begin() + dir);

	// ����Superblock������λͼ������Inode�������ļ�Inode�ĸ��£��͵�ǰĿ¼��Ӧ��Inode�ĸ��£�
	disk.free_renew(i_index, dirGroup[cur_dir]);
	// ����ǰĿ¼д�����ݿ�
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
}

void FileSystem::needFormat(bool doFormat)
{
	this->doFormat = doFormat;
}

bool FileSystem::isFormat()
{
	return doFormat;
}

void FileSystem::help()	//��ʾ���������嵥 
{
	cout << "ls - ��ʾ��ǰĿ¼�嵥" << endl;
	cout << "cd - ����Ŀ¼" << endl;
	cout << "touch - �ڸ�Ŀ¼�´����ļ�" << endl;
	cout << "rm_f - ɾ����Ŀ¼�µ�ĳ�ļ�" << endl;
	cout << "mkdir - ����Ŀ¼" << endl;
	cout << "rm_rf - ɾ����Ŀ¼�µ�ĳĿ¼" << endl;
	return;
}

void FileSystem::cmd(string args)	//�������������
{
	//���ڴ�ŷָ����ַ���
	vector<string> res;
	//���ָ���ַ��������пո� 
	string inp = args;
	string result;
	//���ַ�������input�� 
	stringstream input(inp);
	//���������result�У�������res�� 
	while (input >> result)
		res.push_back(result);
	
	int inp2;

	if (res[0] == "ls") {
		ls();	
	}
	// ת����Ŀ¼��cd��
	else if (res[0] == "cd") {
		inp2 = stoi(res[1]); // ���ַ���ת��Ϊ����
		cd(inp2);
	}
	// �ڸ�Ŀ¼�´����ļ���touch��
	else if (res[0] == "touch") {
		touch(res[1].c_str());
	}
	// ɾ����Ŀ¼�µ�ĳ�ļ���rm_f��
	else if (res[0] == "rm_f") {
		inp2 = stoi(res[1]); // ���ַ���ת��Ϊ����
		rm_f(inp2);
	}
	// �ڸ�Ŀ¼�´���Ŀ¼��mkdir��
	else if (res[0] == "mkdir") {
		mkdir(res[1].c_str());
	}
	// ɾ����Ŀ¼�µ�ĳĿ¼��rm_rf��
	else if (res[0] == "rm_rf") {
		inp2 = stoi(res[1]); // ���ַ���ת��Ϊ����
		rm_rf(inp2);
	}
	// ��ʾ���� ,û������
	else if (res[0] == "help") {
		help();
	}
	else {
		cout << "��Ǹ��û�и�����" << endl;
	}
	
	return;
}