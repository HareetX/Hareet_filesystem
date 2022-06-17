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
	return isLogin;
}


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
	
	cin.ignore();
	system("pause");
	system("cls");
}

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
	int d_index = dirGroup[cur_dir].find_file(name, DIR_MODE);
	if (d_index == -1) {
		return -1;
	}
	return find_dir(dirGroup[cur_dir].getDentry(d_index).getIndex());
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

int FileSystem::find_dir(int i_index)
{
	int dir_size = dirGroup.size();
	for (int i = 0; i < dir_size; i++) {
		if (i_index == dirGroup[i].getI_Index()) {
			return i;
		}
	}
	return -1;
}

int FileSystem::getDirTotalSize(int dir)
{
	int dirTotalSize = 0;
	for (int i = dirGroup[dir].getDentryNum() - 1; i >= 0; i--) {
		Dentry dentry = dirGroup[dir].getDentry(i);
		dirTotalSize += dentry.getF_Size();
		if (dentry.getMode() == DIR_MODE) {// ����Ŀ¼�ļ��ݹ�����Ŀ¼���ļ��ܴ�С
			dirTotalSize += getDirTotalSize(find_dir(dentry.getIndex()));
		}
	}
	return dirTotalSize;
}

void FileSystem::printUserPos()
{
	cout << userGroup[cur_user].getUsername() << "@" << Cur_Host_Name << ": " << getDirPos(cur_dir) << "$";
}

void FileSystem::df()
{
	disk.printBlocks();
}

void FileSystem::df_i()
{
	disk.printInodes();
}

void FileSystem::df_s()
{
	disk.printSuperblock();
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
	for (int i = 0; i < dirGroup[cur_dir].getDentryNum(); i++) {
		Dentry dentry = dirGroup[cur_dir].getDentry(i);
		cout << dentry.getName() << "\t";
		int mode = dentry.getMode();
		if (mode == DIR_MODE) {
			cout << "Ŀ¼�ļ�";
			cout << "\t" << dentry.getF_Size()  + getDirTotalSize(find_dir(dentry.getName().c_str())) << "B" << endl;
		}
		else if (mode == FILE_MODE) {
			cout << "һ���ļ�";
			cout << "\t" << dentry.getF_Size() << "B" << endl;
		}
		else if (mode == OTHER_MODE) {
			cout << "�����ļ�" << endl;
		}
	}
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
	// ����ָ��ǰĿ¼��Ŀ¼����ļ���С
	if (cur_dir > 0) {
		int parent_dir = dirGroup[cur_dir].getParentDir();
		dirGroup[parent_dir].setDentryFsize(dirGroup[cur_dir].getName().c_str(), dirGroup[cur_dir].getDirSize(), DIR_MODE);
	}

	// ��Inode�У�Ŀ¼�ļ���СҪ����
	// ����Superblock������λͼ������Inode�������ļ�Inode�ĸ��£��͵�ǰĿ¼��Ӧ��Inode�ĸ��£�
	disk.use_renew(b_index, i_index, FILE_MODE, 0, dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir].getDirSize());
	
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
	
	// ɾ���ļ���Ŀ¼��
	dirGroup[cur_dir].del_Dentry(d_index);
	
	// ����Superblock������λͼ������Inode�������ļ�Inode�ĸ��£��͵�ǰĿ¼��Ӧ��Inode�ĸ��£�
	disk.free_renew(i_index, dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir].getDirSize());
	
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

	if (cur_dir > 0) {
		int parent_dir = dirGroup[cur_dir].getParentDir();
		dirGroup[parent_dir].setDentryFsize(dirGroup[cur_dir].getName().c_str(), dirGroup[cur_dir].getDirSize(), DIR_MODE);
	}

	// ��Inode�У�Ŀ¼�ļ���СҪ����
	// ����Superblock������λͼ������Inode�������ļ�Inode�ĸ��£��͵�ǰĿ¼��Ӧ��Inode�ĸ��£�
	disk.use_renew(b_index, i_index, DIR_MODE, dir_size, dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir].getDirSize());

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
	disk.free_renew(i_index, dirGroup[cur_dir].getI_Index(), dirGroup[cur_dir].getDirSize());
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
	if (isLogin) {
		cout << "*****************************************************" << endl;
		cout << "*    help            - ��ʾ����                     *" << endl;
		cout << "*    ls              - ��ʾ��ǰĿ¼�嵥             *" << endl;
		cout << "*    cd Ŀ¼��       - ת��Ŀ¼                     *" << endl;
		cout << "*    touch �ļ���    - �ڸ�Ŀ¼�´����ļ�           *" << endl;
		cout << "*    mkdir Ŀ¼��    - ����Ŀ¼                     *" << endl;
		cout << "*    rm[ -f] �ļ���  - ɾ����Ŀ¼�µ��ļ�           *" << endl;
		cout << "*    rm -rf Ŀ¼��   - ɾ����Ŀ¼�µ�Ŀ¼           *" << endl;
		cout << "*    open �ļ���     - ���ļ����ɶ�д�ļ���       *" << endl;
		cout << "*    df              - ��ʾ�ļ�ϵͳ�Ĵ���ʹ�����   *" << endl;
		cout << "*    df -i           - ��ʾ�ļ�ϵͳ��i�ڵ�ʹ�����  *" << endl;
		cout << "*    df -s           - ��ʾ�ļ�ϵͳ�ĳ�������Ϣ     *" << endl;
		cout << "*    q               - �˳��ļ�ϵͳ                 *" << endl;
		cout << "*****************************************************" << endl;
	}
	return;
}

bool FileSystem::cmd(string args)	//�������������
{
	//û����������
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
	else if (res[0] == "rm") {// Ĭ��Ϊɾ���ļ�����
		if (res.size() <= 1) {
			cout << "δ����Ҫɾ�����ļ���" << endl;
			/*cout << "rm[ -f] - ɾ����Ŀ¼�µ��ļ�" << endl;
			cout << "rm -rf  - ɾ����Ŀ¼�µ�Ŀ¼" << endl;*/
		}
		else if (res[1][0] == '-') {
			if (res[1] == "-f") { // ɾ����Ŀ¼�µ�ĳ�ļ���rm_f��
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
				cout << "rm[ -f] - ɾ����Ŀ¼�µ��ļ�" << endl;
				cout << "rm -rf  - ɾ����Ŀ¼�µ�Ŀ¼" << endl;
			}
		}
		else {
			/*cout << "rm[ -f] - ɾ����Ŀ¼�µ��ļ�" << endl;
			cout << "rm -rf  - ɾ����Ŀ¼�µ�Ŀ¼" << endl;*/
			rm_f(res[1].c_str());
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
	// ��ʾ�ļ�ϵͳ�Ĵ��̻�i�ڵ�ʹ�����
	else if (res[0] == "df") {
		if (res.size() <= 1) { // ��ʾ�ļ�ϵͳ�Ĵ���ʹ�����
			df();
		}
		else if (res[1] == "-i") { // ��ʾ�ļ�ϵͳ��i�ڵ�ʹ�����
			df_i();
		}
		else if (res[1] == "-s") { // ��ʾ�ļ�ϵͳ�ĳ�����ʹ�����
			df_s();
		}
		else {
			cout << "df      - ��ʾ�ļ�ϵͳ�Ĵ���ʹ�����" << endl;
			cout << "df -i   - ��ʾ�ļ�ϵͳ��i�ڵ�ʹ�����" << endl;
			cout << "df -s   - ��ʾ�ļ�ϵͳ�ĳ�����ʹ�����" << endl;
		}
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
		dirGroup[cur_dir].setDentryFsize(name, disk.file_size(i_index), FILE_MODE);
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
