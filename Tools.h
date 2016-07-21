#ifndef TOOLS_H
#define TOOLS_H

/***********************************************************/
/*  2820U Algorithms & Data Structures - Display Tools     */
/***********************************************************/
/* Filename: tools.cpp                                     */
/* Author: Robert Burden								   */
/***********************************************************/

#include <iostream> 
#include <ctime> 
#include <windows.h> 
#include <vector>
#include <string>

using namespace std;

//SET Console Window Size
void setConsoleSize(void) {
	system("mode con lines=80"); //Set Console Height
	system("mode con cols=140"); //Set Console Width 
}

//Pause the code Execution
void pause (bool show) {
	if (show) system("PAUSE");  //Loud pause (prints a message)
	else system("PAUSE > nil"); //Silent pause (no message outputted)
}

//Print text with a specific colour
void coutc(int color, int output) { 
	HANDLE handle= GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute( handle, color); 
	cout<< output; 
} 
void coutc(int color, char* output) { 
	HANDLE handle= GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute( handle, color); 
	cout<< output; 
} 
template <typename T> void coutc(int color, T output) { 
	HANDLE handle= GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute( handle, color); 
	cout<< output; 
} 

//Manually Set colour for future text
void setColor(int color) { 
	HANDLE handle= GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute( handle, color); 
} 

//Move Cursor to specific point 
void gotoxy ( short x, short y ) { 
	COORD coord = {x, y}; 
	SetConsoleCursorPosition ( GetStdHandle ( STD_OUTPUT_HANDLE ), coord ); 
}

/*
 * Move Cursor to specific point and print the string
 *
 * @param (short x) X Coord to move to
 * @param (short y) Y Coord to move to
 * @param (string print) the string to display starting from the x,y coords moving right
 *
*/
void gotoxy ( short x, short y, string print) { 
	COORD coord = {x, y}; 
	SetConsoleCursorPosition ( GetStdHandle ( STD_OUTPUT_HANDLE ), coord );
	cout << print;
}

/*
 * Draw a double bordered box
 * 
 * @param (int x1) Top left X Coord
 * @param (int y1) Top left Y Coord 
 * @param (int x2) Bottom right X Coord
 * @param (int y2) Bottom right Y Coord 
 * @param (char fill) Background of the box for blank use ' '
 * @param (int fill_colour) Colour of the background
 *
*/ 
 void drawBox (int x1, int y1, int x2, int y2, char fill, int fill_color) {
	y2++;
	for (int y = 0; y <= y2; y++) {
		gotoxy(x1,y1+y);

		if (y == 0) cout << char(201);
		else if (y == y2) cout << char(200);
		else cout << char(186);
		
		for (int i = 1; i <= x2; i++) {
			if (y > 0 && y < y2) {
				coutc(fill_color, fill);
			} else {
				cout << char(205);
			}
		}
		setColor(7);

		if (y == 0) cout << char(187) << endl;
		else if (y == y2) cout << char(188) << endl;
		else cout << char(186) << endl;
	}
}

#endif