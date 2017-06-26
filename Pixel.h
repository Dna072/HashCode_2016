#ifndef PIXEL_H
#define PIXEL_H

#include "vector"
#include <iostream>
#include "List"
using namespace std;


struct Vector2{		//represents the coordinated of the pixel
	int x;
	int y;
	void initialize();

	bool operator==(const Vector2& rhs);

	bool operator!=(const Vector2& rhs);
};


class Pixel{
private:
	bool on = false;
	Vector2 position;
	bool painted = false;		//stores the state of whether we've painted the pixel or not


public:
	bool isSquare;			    //true if pixel is the center of a square
	int sqRadius = 0;			//the radius of the square it covers, default = 0
	bool scanned = false;		//this is simply to check if the algorithm has already scanned the pixel

	Pixel(bool _on = false);

	Pixel(int r, int c, bool _on = false);

	Pixel* neighbors;
	
	void turnOn(bool _on);
	
	//If this pixel is the center of a square, this finds its neigbours
	void setNeighbours(Pixel** pixelGrid, int _R, int _C, list<Pixel> & sqPixels);

	void setIncompleteSquareNeighbours(Pixel** pixelGrid, int _R, int _C, list<Pixel> & sqPixels,vector<Pixel> & toBeErased);

	bool isOn();

	Vector2 getPosition();

	bool isPainted();

	void setPainted(bool _paint);

	void setOn(bool _on);

	void clearNeighbors();

	//Methods for sorting according to the radius of squarePixels
	bool operator==(const Pixel & rhs);

	bool operator>(const Pixel & rhs);

	bool operator<(const Pixel & rhs);
};

#endif