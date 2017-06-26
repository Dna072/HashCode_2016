#include "Pixel.h"

//Vector methods
void Vector2::initialize(){
	x = -1;
	y = -1;
}

bool Vector2::operator==(const Vector2& rhs){
		return (this->x == rhs.x && this->y == rhs.y);
}

bool Vector2::operator!=(const Vector2& rhs){
	return (!(*this == rhs));
}

Pixel::Pixel(bool _on){
	on = _on;
	isSquare = false;
	sqRadius = 0;
	neighbors = NULL;
	scanned = false;
	//position.initialize();
}

Pixel::Pixel(int r, int c, bool _on){
	on = _on;
	isSquare = false;
	sqRadius = 0;
	position.x = r;
	position.y = c;
	neighbors = NULL;
	scanned = false;
}

void Pixel::turnOn(bool _on){
	on = _on;
}

bool Pixel::isOn(){
	return on;
}

bool Pixel::isPainted(){
	return painted;
}

void Pixel::setPainted(bool _paint){
	painted = _paint;
}

Vector2 Pixel::getPosition(){
	return position;
}

void Pixel::setNeighbours(Pixel** pixelGrid, int _R, int _C, list<Pixel> & squarePixels){
	//returns the pixel surronding a cell to make a square
	bool stop = !this->isOn();
	int radius = 1;
	//Loop if the Pixel is 'on'
	while (!stop){
		
		vector<Pixel> temp;
		
		for (int r = -radius; r <= radius; r++){
			for (int c = -radius; c <= radius; c++){
				Vector2 a;
				a.x = this->position.x + r;
				a.y = this->position.y + c;

				if ((a.x < _R && a.x >= 0) && (a.y < _C && a.y >= 0)){
					Pixel p = pixelGrid[a.x][a.y];
					if (p.on && !p.painted){
						temp.push_back(p);
					}
else
					break;

				}
			}
		}

		if (temp.size() == (2 * radius + 1) * (2 * radius + 1)){	//ie. we've found a square
			neighbors = new Pixel[(2 * radius + 1) * (2 * radius + 1)];
			int i = 0;
			
			for (Pixel p : temp){
				neighbors[i] = p;
				i++;
			}

			//This pixel is a center of a square
			this->isSquare = true;
			this->sqRadius = radius;
			squarePixels.push_back(*this);

			//printf("Cell pos: (%d, %d) Radius: %d\n", this->position.x, this->position.y, this->sqRadius);


			radius++;
		}
		else{
			this->sqRadius = 0;
			stop = true;
		}
		
		
	}
}

void Pixel::setIncompleteSquareNeighbours(Pixel** pixelGrid, int _R, int _C, list<Pixel> & squarePixels, vector<Pixel> & toBeErased){
	//returns the pixel surronding a cell to make a square
	bool stop = !this->isOn();
	int radius = 1;
	int erasableCount = 0;
	int tolerance = 1;
	//Loop if the Pixel is 'on'
	while (!stop){

		vector<Pixel> temp;
		vector<Pixel> erasable;

		for (int r = -radius; r <= radius; r++){
			for (int c = -radius; c <= radius; c++){
				Vector2 a;
				a.x = this->position.x + r;
				a.y = this->position.y + c;

				if ((a.x < _R && a.x >= 0) && (a.y < _C && a.y >= 0)){
					Pixel p = pixelGrid[a.x][a.y];
					if (p.on && !p.painted){
						temp.push_back(p);
					}
					else{
						if ((erasableCount < tolerance) && !p.scanned){
							erasableCount++;
							erasable.push_back(p);
							temp.push_back(p);
							pixelGrid[p.getPosition().x][p.getPosition().y].scanned = true;
						}
						else{
							erasable.clear();
							break;
						}
					}
				}
			}
		}

		if (temp.size() == (2 * radius + 1) * (2 * radius + 1)){	//ie. we've found a square
			neighbors = new Pixel[(2 * radius + 1) * (2 * radius + 1)];
			int i = 0;

			for (Pixel p : temp){
				neighbors[i] = p;
				i++;
			}

			for (Pixel p : erasable){
				toBeErased.push_back(p);
			}

			//This pixel is a center of a square
			this->isSquare = true;
			this->sqRadius = radius;
			squarePixels.push_back(*this);

			printf("Cell pos: (%d, %d) Radius: %d\n", this->position.x, this->position.y, this->sqRadius);


			radius++;
			tolerance = radius + 1;
		}
		else{
			this->sqRadius = 0;
			stop = true;
		}

	}


}

void Pixel::clearNeighbors(){
	delete[] neighbors;
}

void Pixel::setOn(bool _on){
	on = _on;
}

/* List methods, inverted so that the largest values come on top*/
bool Pixel::operator<(const Pixel& rhs){
	return (this->sqRadius > rhs.sqRadius);
}

bool Pixel::operator==(const Pixel& rhs){
	return (this->sqRadius == rhs.sqRadius);
}

bool Pixel::operator>(const Pixel& rhs){
	return (this->sqRadius < rhs.sqRadius);
}