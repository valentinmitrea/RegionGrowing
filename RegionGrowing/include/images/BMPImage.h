#pragma once

#include "Image.h"


class BMPImage : public Image
{

	unsigned char header[54];

public:
	BMPImage();
	~BMPImage();

	virtual void read(const std::string &fileName);
	virtual void write(const std::string &fileName);

};
