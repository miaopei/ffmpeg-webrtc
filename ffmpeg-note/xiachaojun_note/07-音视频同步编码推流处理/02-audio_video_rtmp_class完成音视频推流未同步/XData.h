#pragma once
class XData
{
public:
	char *data = 0;
	int size = 0;
	void Drop();
	XData();
	//创建空间，并复制data内容
	XData(char *data,int size);
	virtual ~XData();
};
