#pragma once
#include <iostream>
using namespace std;

// �궨��
#define FILESYSTEMNAME "XX_filesystem.sys" // ��������ļ�����

const int Sum_Size = 0;

char buffer[10 * 1024 * 1024] = { 0 };

FILE* fr; // �����ļ��Ķ�ָ��
FILE* fw; // �����ļ���дָ��

bool Format(); // ��ʽ����������ļ�
void Initial(); // ��ʼ����������ļ�