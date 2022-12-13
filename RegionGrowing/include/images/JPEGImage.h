#pragma once

#include "Image.h"


class JPEGImage : public Image
{

public:
	JPEGImage();
	~JPEGImage();

	virtual void read(const std::string &fileName);
	virtual void write(const std::string &fileName);

};
