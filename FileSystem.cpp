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
	userGroup.clear();

	memset(Cur_Host_Name, 0, sizeof(Cur_Host_Name));
	DWORD k = 100;
	GetComputerName(Cur_Host_Name, &k);
	userGroup.push_back(User("root", "root"));
	cur_user = -1; // ϵͳһ��ʼδ��¼
}

bool FileSystem::is_Login()
{
	//cout << isLogin;
	return isLogin;
}

//void inUsername(char username[])	//�����û���
//{
//	
//	cout << "username: " ;
//	cin >> username;
//}

//void inPasswd(char passwd[])	//��������
//{
//	cout << "passwd: ";
//	cin >> passwd;
//	//cin.ignore();
//	//int i = 0;
//	//while (true)
//	//{
//	//	passwd[i] = getch();     //ֻ�ܽ���һ������ 
//	//	if (passwd[i] == '\r')     //�س�����ʾ\r\n 
//	//	{
//	//		break;
//	//	}
//
//	//}
//}

//bool check(char username[], char passwd[]) //�˶��û���������
//{	
//
//	if ((strcmp(username, "root") == 0) && (strcmp(passwd, "root") == 0)) {
//
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}


void FileSystem::Login()	//��¼����
{
	 //ֻ�ƻ���һ���û�Ϊroot
	cout << "���ļ�ϵͳֻ��root�û����������û���root�Լ�����root��¼:" << endl;
	char username[100] = { 0 };
	char passwd[100] = { 0 };
	//inUsername(username);	
	// �����û���
	cout << "username: ";
	cin >> username;
	//inPasswd(passwd);		
	// �����û�����
	cout << "passwd: ";
	cin >> passwd;

	//�˶��û���������
	int usersize = userGroup.size();
	for (int i = 0; i < usersize; i++) {
		if (userGroup[i].check(username, passwd)) {
			isLogin = true;
			cur_user = i;
			break;
		}
		else {
			isLogin = false;
		}
	}
	if (!isLogin) {
		cout << "�û������������" << endl;
	}
	/*if (check(username, passwd)) {	
		isLogin = true;
		
	}
	else {
		isLogin = false;
		
	}*/
	cin.ignore();
	system("pause");
	system("cls");
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


int FileSystem::find_dir(const char* name)
{
	int dir_size = dirGroup.size();
	for (int i = 0; i < dir_size; i++) {
		if (name == dirGroup[i].getName()) {
			return i;
		}
	}
	return -1;
}

bool FileSystem::check_fname(const char* name, int mode)
{
	if (dirGroup[cur_dir].find_file(name, mode) == -1) { 
		// û�ҵ��ļ���˵��û������
		return false;
	}
	// �ҵ����ļ���˵��������
	return true;
}

int FileSystem::find_file(const char* name, int mode)
{
	return dirGroup[cur_dir].find_file(name, mode);
}

void FileSystem::printUserPos()
{
	cout << userGroup[cur_user].getUsername() << "@" << Cur_Host_Name << ": " << getDirPos(cur_dir) << "$";
}

string FileSystem::getDirPos(int cur)
{
	if (cur == 0) {
		return dirGroup[cur].getName();
	}
	else {
		return getDirPos(dirGroup[cur].getParentDir()) + "/" + dirGroup[cur].getName();
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
	// ����Ƿ��������ļ�
	if (check_fname(name, FILE_MODE)) {
		cout << "�ļ����ظ���������ļ���" << endl;
		return;
	}
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

void FileSystem::rm_f(const char* name)
{
	// �ڵ�ǰĿ¼ɾ����Ӧ��Ŀ¼��
	int d_index = find_file(name, FILE_MODE);
	if (d_index == -1) {
		cout << "δ�ҵ��ļ�" << endl;
		return;
	}

	// ��¼ɾ���ļ���inode����
	int i_index = dirGroup[cur_dir].getDentry(d_index).getIndex();
	
	dirGroup[cur_dir].del_Dentry(d_index);
	
	// ����Superblock������λͼ������Inode�������ļ�Inode�ĸ��£��͵�ǰĿ¼��Ӧ��Inode�ĸ��£�
	disk.free_renew(i_index, dirGroup[cur_dir]);
	
	// ����ǰĿ¼д�����ݿ�
	disk.dir_write(dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir]);
}


void FileSystem::mkdir(const char* name)
{
	// ����Ƿ�������Ŀ¼
	if (check_fname(name, DIR_MODE)) {
		cout << "Ŀ¼���ظ��������Ŀ¼��" << endl;
		return;
	}

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

void FileSystem::rm_rf(const char* name)
{
	// ע�⣺ɾ��Ŀ¼Ҫ�Ѹ�Ŀ¼�������ļ���ɾ���������õ��ݹ麯����
	// �ڵ�ǰĿ¼�ҵ���Ӧ��Ŀ¼��
	int d_index = find_file(name, DIR_MODE);
	if (d_index == -1) {
		cout << "δ�ҵ�Ŀ¼" << endl;
		return;
	}
	int cur = cur_dir;
	
	/*int d_num = dirGroup[cur_dir].getDentryNum();
	for (i = 0; i < d_num; i++) {
		if (dirGroup[cur_dir].getDentry(i).getIndex() == i_index) {
			break;
		}
	}
	if (i == d_num) {
		cout << "δ�ҵ���Ӧ�ļ�" << endl;
		return;
	}*/
	

	// �ҵ�Ҫɾ����Ŀ¼
	/*for (dir = 0; dir < dirGroup.size(); dir++) {
		if (dirGroup[dir].getI_Index() == i_index) {
			break;
		}
	}*/
	int dir = find_dir(name);
	if (d_index == -1) {
		cout << "δ�ҵ�Ŀ¼" << endl;
		return;
	}
	// ��¼ɾ��Ŀ¼��inode����
	int i_index = dirGroup[dir].getI_Index();
	
	cd(dir);
	Dentry dentry;
	for (int i = 0; i < dirGroup[cur_dir].getDentryNum(); i++) {
		dentry = dirGroup[cur_dir].getDentry(i);
		if (dentry.getMode() == FILE_MODE) {
			rm_f(dentry.getName().c_str());
		}
		else if (dentry.getMode() == DIR_MODE) {
			rm_rf(dentry.getName().c_str());
		}
	}
	cd(cur);

	// ɾ����ӦĿ¼���Ŀ¼
	dirGroup[cur_dir].del_Dentry(d_index);
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
	cout << "***************************************" << endl;
	cout << "*    ls     - ��ʾ��ǰĿ¼�嵥        *" << endl;
	cout << "*    cd     - ת��Ŀ¼                *" << endl;
	cout << "*    touch  - �ڸ�Ŀ¼�´����ļ�      *" << endl;
	cout << "*    mkdir  - ����Ŀ¼                *" << endl;
	cout << "*    rm -f  - ɾ����Ŀ¼�µ��ļ�      *" << endl;
	cout << "*    rm -rf - ɾ����Ŀ¼�µ�Ŀ¼      *" << endl;
	cout << "*    open   - ���ļ����ɶ�д�ļ���  *" << endl;
	cout << "*    q      - �˳��ļ�ϵͳ            *" << endl;
	cout << "***************************************" << endl;
	return;
}

bool FileSystem::cmd(string args)	//�������������
{
	if (args == "") {
		return true;
	}
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
		if (res.size() <= 1) { // δ����ת��Ŀ¼��
			cout << "δ����ת��Ŀ¼��" << endl;
			return true;
		}
		else if (res[1] == ".") {
			return true;
		}
		else if (res[1] == "..") {
			inp2 = dirGroup[cur_dir].getParentDir();
		}
		else {
			inp2 = find_dir(res[1].c_str()); // �ҵ���Ӧ��Ŀ¼
		}
		cd(inp2);
	}
	// �ڸ�Ŀ¼�´����ļ���touch��
	else if (res[0] == "touch") {
		if (res.size() <= 1) { // δ�����ļ���
			cout << "δ�����ļ���" << endl;
			return true;
		}
		touch(res[1].c_str());
	}
	// �ڸ�Ŀ¼�´���Ŀ¼��mkdir��
	else if (res[0] == "mkdir") {
		if (res.size() <= 1) { // δ����Ŀ¼��
			cout << "δ����Ŀ¼��" << endl;
			return true;
		}
		mkdir(res[1].c_str());
	}
	// ɾ����Ŀ¼�µ��ļ���rm��
	else if (res[0] == "rm") {
		if (res.size() <= 1 || (res[1] != "-f" && res[1] != "-rf")) {
			cout << "rm -f  - ɾ����Ŀ¼�µ��ļ�" << endl;
			cout << "rm -rf - ɾ����Ŀ¼�µ�Ŀ¼" << endl;
		}
		else if (res[1] == "-f") { // ɾ����Ŀ¼�µ�ĳ�ļ���rm_f��
			if (res.size() <= 2) {
				cout << "δ����Ҫɾ�����ļ���" << endl;
			}
			else {
				rm_f(res[2].c_str());
			}
		}
		else if (res[1] == "-rf") { // ɾ����Ŀ¼�µ�ĳĿ¼��rm_rf��
			if (res.size() <= 2) {
				cout << "δ����Ҫɾ����Ŀ¼��" << endl;
			}
			else {
				rm_rf(res[2].c_str());
			}
		}
		else {
			cout << "rm -f  - ɾ����Ŀ¼�µ��ļ�" << endl;
			cout << "rm -rf - ɾ����Ŀ¼�µ�Ŀ¼" << endl;
		}
	}
	// ��һ���ļ�
	else if (res[0] == "open") {
		if (res.size() <= 1) { // δ�����ļ���
			cout << "δ�����ļ���" << endl;
			return true;
		}
		openfile(res[1].c_str());
	}
	// ��ʾ����
	else if (res[0] == "help") {
		help();
	}
	// �˳�ϵͳ
	else if (res[0] == "q") {
		return false;
	}
	else {
		cout << "��Ǹ��û�и�����" << endl;
	}
	
	return true;
}

void FileSystem::openfile(const char* name)
{
	// �ҵ���Ҫ�򿪵��ļ�
	int d_index = find_file(name, FILE_MODE);
	if (d_index == -1) {
		cout << "δ�ҵ��ļ�" << endl;
		return;
	}
	int i_index = dirGroup[cur_dir].getDentry(d_index).getIndex();
	cout << name << "�Ѵ�..." << endl;
	// �����ı�����
	char* context = disk.file_read(i_index);
	cout << context << endl;
	delete[] context;

	// �û����������ı�����
	cout << "����д���ı����ݣ����� q �˳��༭����" << endl;
	string text = "\0";
	while (1) {
		getline(cin, text);
		if (text == "q") {
			cout << "�ļ��ѹر�..." << endl;
			break;
		}
		text = "\n" + text;
		if (!disk.file_write(i_index, text.c_str())) {
			cout << "�ļ��ѹر�..." << endl;
			break;
		}
		dirGroup[cur_dir].setDentryFsize(name, disk.file_size(i_index));
	}
}

User::User(string username, string password)
{
	this->username = username;
	this->password = password;
}

void User::setUser(string username, string password)
{
	this->username = username;
	this->password = password;
}

bool User::check(string username, string password)
{
	if (username == this->username && password == this->password) {
		return true;
	}
	return false;
}

string User::getUsername()
{
	return username;
}
