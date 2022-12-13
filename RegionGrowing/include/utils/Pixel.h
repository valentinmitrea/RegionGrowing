#pragma once

#include <tuple>


class Pixel
{

	int x, y;
	unsigned char red, green, blue;

public:
	Pixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue);
	Pixel(int x, int y, std::tuple<unsigned char, unsigned char, unsigned char> color);
	~Pixel();

	bool isSimilar(const Pixel &pixel, int threshold) const;

	int getX() const;
	void setX(int x);

	int getY() const;
	void setY(int y);

	unsigned char getRed() const;
	void setRed(unsigned char red);

	unsigned char getGreen() const;
	void setGreen(unsigned char green);

	unsigned char getBlue() const;
	void setBlue(unsigned char blue);

	std::tuple<unsigned char, unsigned char, unsigned char> getColor() const;
	void setColor(std::tuple<unsigned char, unsigned char, unsigned char> color);

};
