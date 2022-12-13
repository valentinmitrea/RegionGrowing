#include <images/Image.h>

#include <algorithm>
#include <queue>
#include <map>
#include <random>


Image::Image() : File()
{
}


Image::Image(int width, int height, unsigned char *data, bool grayscale) : File(), width(width), height(height), grayscale(grayscale)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			int index = i * 3 * width + j * 3;

			pixels.push_back(Pixel(i, j, data[index], data[index + 1], data[index + 2]));
		}
}


Image::~Image()
{
}


std::vector<Pixel> Image::getNeighbours(const Pixel &pixel)
{
	std::vector<Pixel> neighbours;

	/* compute the 4-connected neighbours of the current pixel */
	if (pixel.getX() > 0)
		neighbours.push_back(pixels[(pixel.getX() - 1) * width + pixel.getY()]);

	if (pixel.getX() < height - 1)
		neighbours.push_back(pixels[(pixel.getX() + 1) * width + pixel.getY()]);

	if (pixel.getY() > 0)
		neighbours.push_back(pixels[pixel.getX() * width + (pixel.getY() - 1)]);

	if (pixel.getY() < width - 1)
		neighbours.push_back(pixels[pixel.getX() * width + (pixel.getY() + 1)]);

	return neighbours;
}


void Image::segmentRegionGrowing(int threshold, bool randomSeeds)
{
	int label = 0;
	std::vector<int> seedPositions;

	if (randomSeeds)
	{
		for (std::size_t i = 0; i < pixels.size(); i++)
			seedPositions.push_back(i);

		/* shuffle seed positions so we get random starting seeds for the regions */
		std::random_shuffle(seedPositions.begin(), seedPositions.end());
	}
	else
	{
		std::vector<int> nrOfSimilarNeighbours;

		/* compute number of similar neighbours for each pixel in the image */
		for (std::size_t i = 0; i < pixels.size(); i++)
		{
			int nr = 0;

			for (Pixel neighbour : getNeighbours(pixels[i]))
				if (neighbour.isSimilar(pixels[i], threshold))
					nr++;

			nrOfSimilarNeighbours.push_back(nr);
		}

		/* compute seed positions based on the number of similar neighbours */
		for (int nr = 4; nr >= 0; nr--)
			for (std::size_t i = 0; i < pixels.size(); i++)
				if (nrOfSimilarNeighbours[i] == nr)
					seedPositions.push_back(i);
	}

	/* run the Region Growing algorithm using the seed starting positions calculated above */
	for (std::size_t i = 0; i < pixels.size(); i++)
		if (pixelLabels[seedPositions[i]] == -1)
		{
			Pixel seed = pixels[seedPositions[i]];
			std::queue<Pixel> pixelQueue;

			/* create a new region starting from the seed */
			pixelLabels[seedPositions[i]] = label;
			pixelQueue.push(seed);

			/* while the region is still growing */
			while (!pixelQueue.empty())
			{
				Pixel pixel = pixelQueue.front();
				pixelQueue.pop();

				/* analyze the 4-connected neighbours of the current pixel */
				for (Pixel neighbour : getNeighbours(pixel))
					if (pixelLabels[neighbour.getX() * width + neighbour.getY()] == -1 && neighbour.isSimilar(seed, threshold))
					{
						pixelLabels[neighbour.getX() * width + neighbour.getY()] = label;
						pixelQueue.push(neighbour);
					}
			}

			/* update label value */
			label++;
		}
}


void Image::segment(ImageAlgorithm algorithm, int threshold, bool randomSeeds, bool randomColors)
{
	std::map<int, std::tuple<unsigned char, unsigned char, unsigned char>> regionColors;

	/* reset pixel labels */
	pixelLabels.clear();
	for (std::size_t i = 0; i < pixels.size(); i++)
		pixelLabels.push_back(-1);

	if (algorithm == ImageAlgorithm::SEGMENTATION_REGION_GROWING)
		segmentRegionGrowing(threshold, randomSeeds);

	/* compute colors for the regions (either random colors or mean colors) */
	if (randomColors)
	{
		int nrRegions = -1;
		std::random_device random_device;
		std::mt19937 engine(random_device());
		std::uniform_int_distribution<int> dist(0, 255);

		/* find number of regions created by the segmentation algorithm (which is the highest label in the segmented image) */
		for (std::size_t i = 0; i < pixels.size(); i++)
			nrRegions = std::max(nrRegions, pixelLabels[i]);

		for (int i = 0; i < nrRegions; i++)
			regionColors.emplace(i, std::make_tuple(dist(engine), dist(engine), dist(engine)));
	}
	else
		for (std::size_t i = 0; i < pixels.size(); i++)
		{
			auto it = regionColors.find(pixelLabels[i]);

			if (it == regionColors.end())
				regionColors.emplace(pixelLabels[i], pixels[i].getColor());
			else
			{
				std::get<0>(it->second) = (std::get<0>(it->second) + pixels[i].getRed()) / 2;
				std::get<1>(it->second) = (std::get<1>(it->second) + pixels[i].getGreen()) / 2;
				std::get<2>(it->second) = (std::get<2>(it->second) + pixels[i].getBlue()) / 2;
			}
		}

	/* update the colors of the pixels */
	for (std::size_t i = 0; i < pixels.size(); i++)
		pixels[i].setColor(regionColors[pixelLabels[i]]);
}


int Image::getWidth() const
{
	return width;
}


int Image::getHeight() const
{
	return height;
}


const std::vector<Pixel>& Image::getPixels() const
{
	return pixels;
}


bool Image::isGrayscale() const
{
	return grayscale;
}
