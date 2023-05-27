/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   cell.h
  * Author: MSCS
  *
  * Created on 12 March 2020, 8:39 AM
  */

  /*
 1. Vuyo Sibiya (221060528)
 2. Banele Nyide (221019384)
 3. Nqobile Shinga (221055984)
 4. Sanelisiwe Xolo (220025744)
 5. Lindokuhle Ntombela (221043610)
 */



#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;
int getAdjacentIndex(const int& i);
class cell {
	// 8 directions: north-northWest clockwise
	char marble;
	string location;
	vector<int> distanceToEdge; //how far is the boundary from this cell.

public:
	vector<cell*> adjacent;
	char getMarble() const;
	void setMarble(const char&);//set marble to either O, @ or +.
	cell* getAdjacent(const int&) const;
	void setAdjacent(cell*, const int&);//only set each neighbour once.
	string getLocation() const;
	static bool validateLocation(const string& l);
	void setLocation(const string& l);
	void setDistanceToEdge(const int&);
	int getDistanceToEdge(const int&) const;
	cell();//set all 8 neighbours to null.
};

#endif /* CELL_H */