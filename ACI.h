#pragma once
#include "XX_filesystem.h"

// ��������ļ���ȡ���ƽӿڣ�Access Control Interface��
class ACI
{
public:
	virtual bool readSysFile() = 0;
	virtual bool writeSysFile() = 0;
	virtual bool openSysFile() = 0;
	virtual void closeSysFile() = 0;
};
