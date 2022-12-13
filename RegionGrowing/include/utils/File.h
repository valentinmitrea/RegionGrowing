#pragma once

#include <vector>
#include <string>


class File
{

protected:
	std::size_t size;
	std::vector<unsigned char> data;

public:
	File();
	virtual ~File();

	virtual void read(const std::string &fileName);
	virtual void write(const std::string &fileName);

};
