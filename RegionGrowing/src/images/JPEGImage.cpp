#include <images/JPEGImage.h>

#include <jpeglib.h>


JPEGImage::JPEGImage() : Image()
{
}


JPEGImage::~JPEGImage()
{
}


void JPEGImage::read(const std::string &fileName)
{
	FILE *in;
	struct jpeg_decompress_struct jpeg;
	struct jpeg_error_mgr error;
	std::vector<unsigned char> buffer;
	JSAMPROW row;

	/* initialize JPEG decompress struct */
	jpeg.err = jpeg_std_error(&error);
	jpeg_create_decompress(&jpeg);

	/* open JPEG image and read the header */
	fopen_s(&in, fileName.c_str(), "rb");
	jpeg_stdio_src(&jpeg, in);
	jpeg_read_header(&jpeg, TRUE);

	/* start JPEG decompression */
	jpeg_start_decompress(&jpeg);

	/* parse the header of the JPEG image */
	width = jpeg.output_width;
	height = jpeg.output_height;
	buffer.resize(jpeg.output_components * width * height);
	grayscale = jpeg.output_components == 1 ? true : false;

	/* parse the pixels of the JPEG image */
	while (jpeg.output_scanline < jpeg.output_height)
	{
		row = &buffer[0] + jpeg.output_scanline * jpeg.output_components * width;
		jpeg_read_scanlines(&jpeg, &row, 1);
	}

	if (grayscale)
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				pixels.push_back(Pixel(i, j, buffer[i * width + j], buffer[i * width + j], buffer[i * width + j]));
	else
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				int index = i * 3 * width + j * 3;

				pixels.push_back(Pixel(i, j, buffer[index], buffer[index + 1], buffer[index + 2]));
			}

	/* finish JPEG decompression */
	jpeg_finish_decompress(&jpeg);
	jpeg_destroy_decompress(&jpeg);

	fclose(in);
}


void JPEGImage::write(const std::string &fileName)
{
	FILE *out;
	struct jpeg_compress_struct jpeg;
	struct jpeg_error_mgr error;
	std::vector<unsigned char> buffer;
	JSAMPROW row;

	/* initialize JPEG compress struct */
	jpeg.err = jpeg_std_error(&error);
	jpeg_create_compress(&jpeg);

	/* open destination JPEG image */
	fopen_s(&out, fileName.c_str(), "wb");
	jpeg_stdio_dest(&jpeg, out);

	/* update the header of the JPEG image */
	jpeg.image_width = width;
	jpeg.image_height = height;
	jpeg.input_components = grayscale ? 1 : 3;
	jpeg.in_color_space = grayscale ? JCS_GRAYSCALE : JCS_RGB;
	jpeg_set_defaults(&jpeg);
	jpeg_set_quality(&jpeg, 100, TRUE);

	/* start JPEG compression */
	jpeg_start_compress(&jpeg, TRUE);

	/* update the pixels of the JPEG image */
	if (grayscale)
		for (std::size_t i = 0; i < pixels.size(); i++)
			buffer.push_back(pixels[i].getRed());
	else
		for (std::size_t i = 0; i < pixels.size(); i++)
		{
			buffer.push_back(pixels[i].getRed());
			buffer.push_back(pixels[i].getGreen());
			buffer.push_back(pixels[i].getBlue());
		}

	while (jpeg.next_scanline < jpeg.image_height)
	{
		row = &buffer[0] + jpeg.next_scanline * jpeg.input_components * width;
		jpeg_write_scanlines(&jpeg, &row, 1);
	}

	/* finish JPEG compression */
	jpeg_finish_compress(&jpeg);
	jpeg_destroy_compress(&jpeg);

	fclose(out);
}
