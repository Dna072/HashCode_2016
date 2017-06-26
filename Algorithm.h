#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Pixel.h"
#include <fstream>
#include <string.h>
#include <sstream>

int R, C;
Pixel **pixelGrid;
list<Pixel> sqPixels;
vector<Pixel> toBeErased;

vector<string> commands;

struct Line{
	Vector2 from;
	Vector2 to;

	int n_pixels = 0;
	Vector2* coveredCells;

	Line(Vector2 _from, Vector2 _to, int np){
		from = _from;
		to = _to;
		n_pixels = np;
	}

	void setCells(){
		if (n_pixels > 0){
			coveredCells = new Vector2[n_pixels];
			int i = 0;

			//Check if cells are on the same row or same column
			if (from.x == to.x){
				
				for (int c = from.y; c <= to.y; c++){					
					coveredCells[i] = pixelGrid[from.x][c].getPosition();
					i++;
				}
			}
			else if (from.y == to.y){
				
				for (int r = from.x; r <= to.x; r++){
					coveredCells[i] = pixelGrid[r][from.y].getPosition();
					i++;
				}
			}


		}
	}

	//operator overloads to sort
	bool operator==(const Line &rhs) const{
		return (this->n_pixels == rhs.n_pixels);
	}

	//Invert the operators so the largest values come on top
	bool operator<(const Line & rhs) const{
		return (this->n_pixels > rhs.n_pixels);
	}

	bool operator>(const Line & rhs) const{
		return (this->n_pixels < rhs.n_pixels);
	}

	bool operator<=(const Line & rhs){
		return (this->n_pixels >= rhs.n_pixels);
	}

	bool operator>=(const Line & rhs){
		return (this->n_pixels <= rhs.n_pixels);
	}


	bool operator!=(const Line & rhs){
		return (this->n_pixels != rhs.n_pixels);
	}

};


//Initialize the grid
void initialize(int _R, int _C){
	R = _R;
	C = _C;
	pixelGrid = new Pixel*[_R];
	
	for (int i = 0; i < _R; i++){
		pixelGrid[i] = new Pixel[_C];
	}
}

//obsolete - use findLines
vector<Line> findHorizontalLines(){
	//Basically, we would start from the first 'on'(not painted) pixel and loop through the grid to get the last pixel in the row
	//If we find a painted cell in the row, we simply stop at the previous unpainted pixel, and store that as the last row
	//Suppose we made it to an 'off' pixel we also stop and assign the coordinates of the last 'on' pixel

	vector<Line> horizontalLines;
	Vector2 from;
	Vector2 to;
	

	for (int r = 0; r < R; r++){
		int np = 0;			//number of pixels covered
		
		from.x = -1;	to.x = -1;
		from.y = -1;	to.y = -1;
		
		for (int c = 0; c < C; c++){
			Pixel t = pixelGrid[r][c];

			if (from.x > 0 && !t.isOn()){
				break;
			}

			if (t.isOn() && !t.isPainted()){

				if (from.x < 0){
					from = t.getPosition();
				}

				to = t.getPosition();
				np++;
			}
			

		}
		
			if (from != to)
				horizontalLines.push_back(Line(from, to, np));
		
	}

	return horizontalLines;
}

//obsolete - use findLines
vector<Line> findVerticalLines(){
	vector<Line> vLines;
	Vector2 from;
	Vector2 to;
	

	for (int c = 0; c < C; c++){
		int np = 0;
		from.x = -1;	to.x = -1;
		from.y = -1;	to.y = -1;

		for (int r = 0; r < R; r++){
			Pixel t = pixelGrid[r][c];

			if (from.x > 0 && !t.isOn()){
				break;
			}

			if (t.isOn() && !t.isPainted()){
				if (from.y < 0){
					from = t.getPosition();
				}
				np++;
				to = t.getPosition();
			}
		}

		if (from != to)
			vLines.push_back(Line(from, to, np));

	}
	return vLines;
}

list<Line> findLines(bool singleCell){
	list<Line> lines;
	Vector2 from;
	Vector2 to;

	for (int r = 0; r < R; r++){
		int np = 0;			//number of pixels covered

		from.x = -1;	to.x = -1;
		from.y = -1;	to.y = -1;

		for (int c = 0; c < C; c++){
			Pixel t = pixelGrid[r][c];

			if (from.x >= 0 && !t.isOn()){
				break;
			}

			if (t.isOn() && !t.isPainted()){

				if (from.x < 0){
					from = t.getPosition();
				}

				to = t.getPosition();
				np++;
			}
		}

		if (from != to){
			np = (to.y - from.y) + 1;
			lines.push_back(Line(from, to, np));
		}
		else if (singleCell && (from == to)){
			if ((from.x >= 0 && from.y >= 0) && (to.x >= 0 && to.y >= 0))
				lines.push_back(Line(from, to, np));
		}
	}

	for (int c = 0; c < C; c++){
		int np = 0;
		from.x = -1;	to.x = -1;
		from.y = -1;	to.y = -1;

		for (int r = 0; r < R; r++){
			Pixel t = pixelGrid[r][c];

			if (from.x >= 0 && !t.isOn()){
				break;
			}

			if (t.isOn() && !t.isPainted()){
				if (from.y < 0){
					from = t.getPosition();
				}
				np++;
				to = t.getPosition();
			}
		}

		if (from != to){
			np = (to.x - from.x) + 1;
			lines.push_back(Line(from, to, np));
		}
	}


	return lines;
}

//Test the hLines and vLines
void testLines(bool singleCell){
	list<Line> lines = findLines(singleCell);
	
	int i = 0;
	cout <<"\nLines" << endl;
	
	for (Line l : lines){
		printf("Lines %d: from (%d,%d) to (%d,%d)\n",l.n_pixels , l.from.x, l.from.y, l.to.x, l.to.y);
		i++;
	}

}

void PAINT_SQUARE(Pixel** _canvas){
	Pixel sq_p = sqPixels.front();
	sqPixels.pop_front();

	//Paint sq_pixel neighbors on canvas
	for (int r = -sq_p.sqRadius; r <= sq_p.sqRadius; r++){
		for (int c = -sq_p.sqRadius; c <= sq_p.sqRadius; c++){
			_canvas[sq_p.getPosition().x + r][sq_p.getPosition().y + c].setOn(true);
			pixelGrid[sq_p.getPosition().x + r][sq_p.getPosition().y + c].setPainted(true);
		}
	}

	printf("PAINT_SQUARE %d %d %d\n", sq_p.getPosition().x, sq_p.getPosition().y, sq_p.sqRadius);
	stringstream ss;
	ss << "PAINT_SQUARE " << sq_p.getPosition().x << ' ' << sq_p.getPosition().y << ' ' << sq_p.sqRadius << endl;
	string cmd = ss.str();
	commands.push_back(cmd);

	//Clear the sqPixel list
	sqPixels.clear();

	//Update the sqPixel list and sort it again
	for (int r = 0; r < R; r++){
		for (int c = 0; c < C; c++){
			pixelGrid[r][c].setNeighbours(pixelGrid, R, C, sqPixels);
		}
	}
	sqPixels.sort();
	
}


void PAINT_LINE(Pixel** _canvas, bool singleCell){
	//vertical lines available
	list<Line> lines = findLines(singleCell);
	
	while (!lines.empty()){
		lines.sort();
		lines.front().setCells();

		//paint the lines
		for (int i = 0; i < lines.front().n_pixels; i++){
			_canvas[lines.front().coveredCells[i].x][lines.front().coveredCells[i].y].setOn(true);
			pixelGrid[lines.front().coveredCells[i].x][lines.front().coveredCells[i].y].setPainted(true);
		}

		printf("PAINT_LINE %d %d %d %d\n", lines.front().from.x, lines.front().from.y, lines.front().to.x, lines.front().to.y);
		
		stringstream ss;
		ss << "PAINT_LINE " << lines.front().from.x << ' ' << lines.front().from.y << ' ' << lines.front().to.x << ' ' << lines.front().to.y << endl;
		string cmd = ss.str();
		commands.push_back(cmd);
		
		lines = findLines(singleCell);
	}

}


void ProgramPrototype(){
	//First create our canvas
	Pixel** canvas;

	//Initialize our canvas grid with R's and C's
	canvas = new Pixel*[R];
	for (int i = 0; i < R; i++){
		canvas[i] = new Pixel[C];
	}

	//Assign pixels to grid cells on the canvas
	for (int r = 0; r < R; r++){
		for (int c = 0; c < C; c++){
			canvas[r][c] = Pixel(r, c);
		}
	}
	
	for (int r = 0; r < R; r++){
		for (int c = 0; c < C; c++){
			pixelGrid[r][c].setNeighbours(pixelGrid, R, C, sqPixels);
		}
	}
	sqPixels.sort();


	//Now it's time to paint
	//We first check for squares we can paint, the largest squares considered first
	if (!sqPixels.empty()){
		for (int i = 0; i < sqPixels.size(); i++){
			PAINT_SQUARE(canvas);
		}
	}

	//Clear the unnecessary allocations
	for (int r = 0; r < R; r++){
		for (int c = 0; c < C; c++){
			pixelGrid[r][c].clearNeighbors();
		}
	}


	//Now we paint lines
	PAINT_LINE(canvas, false);

	//Check for any left cells
	PAINT_LINE(canvas, true);
	//testLines(true);

	printf("\n\n%d\n", commands.size());

	ofstream output("output.txt");
	output << commands.size()<<endl;

	for (int i = 0; i < commands.size(); i++){
		cout << commands[i];
		output << commands[i];
	}


	//lets print out the image we've drawn
	ofstream image("image.txt");
	for (int r = 0; r < R; r++){
		for (int c = 0; c < C; c++){
			if (canvas[r][c].isOn())
				image << '#';
			else
				image << '.';
		}
		image << endl;
	}
}


void test(){
	initialize(7, 6);

	pixelGrid[0][0] = Pixel(0, 0);	pixelGrid[0][1] = Pixel(0, 1, false); pixelGrid[0][2] = Pixel(0, 2, false); pixelGrid[0][3] = Pixel(0, 3, false); pixelGrid[0][4] = Pixel(0, 4, false); pixelGrid[0][5] = Pixel(0, 5, false);
	pixelGrid[1][0] = Pixel(1, 0);	pixelGrid[1][1] = Pixel(1, 1, false); pixelGrid[1][2] = Pixel(1, 2, true); pixelGrid[1][3] = Pixel(1, 3, true); pixelGrid[1][4] = Pixel(1, 4, false); pixelGrid[1][5] = Pixel(1, 5, false);
	pixelGrid[2][0] = Pixel(2, 0);	pixelGrid[2][1] = Pixel(2, 1, true); pixelGrid[2][2] = Pixel(2, 2, true); pixelGrid[2][3] = Pixel(2, 3, true); pixelGrid[2][4] = Pixel(2, 4, true); pixelGrid[2][5] = Pixel(2, 5, false);
	pixelGrid[3][0] = Pixel(3, 0);	pixelGrid[3][1] = Pixel(3, 1, true); pixelGrid[3][2] = Pixel(3, 2, true); pixelGrid[3][3] = Pixel(3, 3, true); pixelGrid[3][4] = Pixel(3, 4, true); pixelGrid[3][5] = Pixel(3, 5, false);
	pixelGrid[4][0] = Pixel(4, 0);	pixelGrid[4][1] = Pixel(4, 1, true); pixelGrid[4][2] = Pixel(4, 2, true); pixelGrid[4][3] = Pixel(4, 3, true); pixelGrid[4][4] = Pixel(4, 4, true); pixelGrid[4][5] = Pixel(4, 5, false);
	pixelGrid[5][0] = Pixel(5, 0);	pixelGrid[5][1] = Pixel(5, 1, true); pixelGrid[5][2] = Pixel(5, 2, false); pixelGrid[5][3] = Pixel(5, 3, false); pixelGrid[5][4] = Pixel(5, 4, true); pixelGrid[5][5] = Pixel(5, 5, false);
	pixelGrid[6][0] = Pixel(6, 0);	pixelGrid[6][1] = Pixel(6, 1, true); pixelGrid[6][2] = Pixel(6, 2, false); pixelGrid[6][3] = Pixel(6, 3, false); pixelGrid[6][4] = Pixel(6, 4, true); pixelGrid[4][5] = Pixel(6, 5, false);

	for (int r = 0; r < R; r++){
		for (int c = 0; c < C; c++){
			pixelGrid[r][c].setIncompleteSquareNeighbours(pixelGrid, R, C, sqPixels, toBeErased);
		}
	}

	//print the erasables
	cout << "Erasables" << endl;
	for (Pixel p : toBeErased){
		printf("(%d, %d)\n", p.getPosition().x, p.getPosition().y);
	}

	sqPixels.sort();
	testLines(false);
	//cout << endl << "Program Prototype" << endl;

	//ProgramPrototype();
}

void readFile(string file){
	ifstream data(file);
	char p;	//representing a pixel

	data >> R >> C;	//get R and C from file
	
	//initialize pixel grid
	initialize(R, C);

	for (int r = 0; r < R; r++){
		for (int c = 0; c < C; c++){
			data >> p;
			bool _on = (p == '#') ? true : false;
			pixelGrid[r][c] = Pixel(r, c, _on);
		}
		
	}
	//testLines(false);

	ProgramPrototype();
}
#endif