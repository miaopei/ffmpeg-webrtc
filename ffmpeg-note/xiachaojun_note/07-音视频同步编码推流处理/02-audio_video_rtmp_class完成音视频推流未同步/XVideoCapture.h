#pragma once
#include "XDataThread.h"
class XVideoCapture:public XDataThread
{
public:
	int width = 0;
	int height = 0;
	int fps = 0;
	static XVideoCapture *Get(unsigned char index = 0);
	virtual bool Init(int camIndex = 0) = 0;
	virtual bool Init(const char *url) = 0;
	virtual void Stop() = 0;

	virtual ~XVideoCapture();
protected:
	XVideoCapture();
};

