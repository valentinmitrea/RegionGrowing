#include <utils/Pixel.h>


Pixel::Pixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue) : x(x), y(y), red(red), green(green), blue(blue)
{
}


Pixel::Pixel(int x, int y, std::tuple<unsigned char, unsigned char, unsigned char> color) : Pixel(x, y, std::get<0>(color), std::get<1>(color), std::get<2>(color))
{
}


Pixel::~Pixel()
{
}


bool Pixel::isSimilar(const Pixel &pixel, int threshold) const
{
	return ((abs(this->red - pixel.red) + abs(this->green - pixel.green) + abs(this->blue - pixel.blue)) / 3) <= threshold;
}


int Pixel::getX() const
{
	return x;
}


void Pixel::setX(int x)
{
	this->x = x;
}


int Pixel::getY() const
{
	return y;
}


void Pixel::setY(int y)
{
	this->y = y;
}


unsigned char Pixel::getRed() const
{
	return red;
}


void Pixel::setRed(unsigned char red)
{
	this->red = red;
}


unsigned char Pixel::getGreen() const
{
	return green;
}


void Pixel::setGreen(unsigned char green)
{
	this->green = green;
}


unsigned char Pixel::getBlue() const
{
	return blue;
}


void Pixel::setBlue(unsigned char blue)
{
	this->blue = blue;
}


std::tuple<unsigned char, unsigned char, unsigned char> Pixel::getColor() const
{
	return std::make_tuple(red, green, blue);
}


void Pixel::setColor(std::tuple<unsigned char, unsigned char, unsigned char> color)
{
	this->red = std::get<0>(color);
	this->green = std::get<1>(color);
	this->blue = std::get<2>(color);
}
