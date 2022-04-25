#pragma once
#include "XX_filesystem.h"

// 虚拟磁盘文件存取控制接口（Access Control Interface）
class ACI
{
public:
	virtual bool readSysFile() = 0;
	virtual bool writeSysFile() = 0;
	virtual bool openSysFile() = 0;
	virtual void closeSysFile() = 0;
};
