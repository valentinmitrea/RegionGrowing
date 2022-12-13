#include <utils/File.h>

#include <fstream>


File::File()
{
}


File::~File()
{
}


void File::read(const std::string &fileName)
{
	std::ifstream in(fileName, std::ios::binary);

	/* compute file length */
	in.seekg(0, std::ios_base::end);
	size = static_cast<std::size_t>(in.tellg());
	in.seekg(0, std::ios_base::beg);

	/* read the data from the file */
	data.resize(size);
	in.read((char*)&data[0], size);
}


void File::write(const std::string &fileName)
{
	std::ofstream out(fileName, std::ios::binary);

	/* write the data to the file */
	out.write((char*)&data[0], data.size());
}
