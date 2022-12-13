#pragma once

#include <utils/File.h>
#include <utils/Pixel.h>

#include <vector>
#include <string>


enum class ImageAlgorithm
{
	SEGMENTATION_REGION_GROWING
};


class Image : public File
{

protected:
	int width, height;
	std::vector<Pixel> pixels;
	bool grayscale;

	std::vector<int> pixelLabels;

	Image();

public:
	Image(int width, int height, unsigned char *data, bool grayscale);
	virtual ~Image();

private:
	std::vector<Pixel> getNeighbours(const Pixel &pixel);
	void segmentRegionGrowing(int threshold, bool randomSeeds);

public:
	void segment(ImageAlgorithm algorithm, int threshold = 10, bool randomSeeds = false, bool randomColors = false);

	int getWidth() const;
	int getHeight() const;
	const std::vector<Pixel>& getPixels() const;
	bool isGrayscale() const;

};
