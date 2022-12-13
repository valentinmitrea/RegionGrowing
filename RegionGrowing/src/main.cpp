#include <images/BMPImage.h>
#include <images/JPEGImage.h>

#include <iostream>


void test(Image &image, std::string inFileName, std::string outFileName)
{
	image.read(inFileName);
	std::cout << "Image: " << inFileName << std::endl;
	std::cout << "Size: " << image.getWidth() << " " << image.getHeight() << std::endl << std::endl;
	image.segment(ImageAlgorithm::SEGMENTATION_REGION_GROWING, 25, true, false);
	image.write(outFileName);
}


int main()
{
	BMPImage imageBMP1, imageBMP2;
	JPEGImage imageJPEG1, imageJPEG2, imageJPEG3;

	test(imageBMP1, "images/rectangles.bmp", "images/rectangles_segmented.bmp");
	test(imageBMP2, "images/circles.bmp", "images/circles_segmented.bmp");
	test(imageJPEG1, "images/rectangles.jpg", "images/rectangles_segmented.jpg");
	test(imageJPEG2, "images/circles.jpg", "images/circles_segmented.jpg");
	test(imageJPEG3, "images/lena.jpg", "images/lena_segmented.jpg");

	return 0;
}
