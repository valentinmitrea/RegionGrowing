#include <images/BMPImage.h>


BMPImage::BMPImage() : Image()
{
}


BMPImage::~BMPImage()
{
}


void BMPImage::read(const std::string &fileName)
{
	Image::read(fileName);

	/* parse the header of the BMP image */
	for (int i = 0; i < 54; i++)
		header[i] = data[i];

	width = header[18] | header[19] << 8 | header[20] << 16 | header[21] << 24;
	height = header[22] | header[23] << 8 | header[24] << 16 | header[25] << 24;

	/* parse the pixels of the BMP image */
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int index = 54 + i * 3 * width + j * 3;

			pixels.push_back(Pixel(i, j, data[index], data[index + 1], data[index + 2]));
		}
}


void BMPImage::write(const std::string &fileName)
{
	/* update the header of the BMP image */
	header[18] = width & 0xFF;
	header[19] = (width >> 8) & 0xFF;
	header[20] = (width >> 16) & 0xFF;
	header[21] = (width >> 24) & 0xFF;
	header[22] = height & 0xFF;
	header[23] = (height >> 8) & 0xFF;
	header[24] = (height >> 16) & 0xFF;
	header[25] = (height >> 24) & 0xFF;

	for (int i = 0; i < 54; i++)
		data[i] = header[i];

	/* update the pixels of the BMP image */
	for (int i = 0; i < width * height; i++)
	{
		int index = 54 + i * 3;

		data[index] = pixels[i].getRed();
		data[index + 1] = pixels[i].getGreen();
		data[index + 2] = pixels[i].getBlue();
	}

	Image::write(fileName);
}
