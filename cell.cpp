/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
 /*
 1. Vuyo Sibiya
 2. Banele Nyide
 3. Nqobile Shinga
 4. Sanelisiwe Xolo
 5. Lindokuhle Ntombela
 */
#include "cell.h"
#include <iostream>
int getAdjacentIndex(const int& i) { // returns opposite direction
	if (i < 4)
		return i + 4;
	else
		return i - 4;
}
char cell::getMarble()const {
	return marble;
}
void cell::setMarble(const char& m) {
	if (m == 'O' || m == '@' || m == '+') {
		marble = m;
	}
}
cell* cell::getAdjacent(const int& i)const {
	return adjacent.at(i);
}
void cell::setAdjacent(cell* a, const int& i) {
	if (adjacent.at(i) == nullptr) {
		adjacent.at(i) = a;
		a->setAdjacent(this, getAdjacentIndex(i));
	}
}
string cell::getLocation() const {
	return location;
}

bool cell::validateLocation(const string& l) {
	if ((l.size() < 1 || l.size() > 3) || l == "F6")
		return false;
	int lowerB = 1, upperB = 11;//default lower and upper bounds 
	if (l[0] >= 'A' && l[0] <= 'K') {
		if (l[0] >= 'A' && l[0] < 'D') {
			//y = -x + 69; y = x - 57
			lowerB = 69 - l[0];
			upperB = l[0] - 57;
		}
		else if (l[0] >= 'I' && l[0] <= 'K') {
			//y = x - 71;  y = -x + 83
			lowerB = l[0] - 71;
			upperB = 83 - l[0];
		}
		if (stoi(l.substr(1)) >= lowerB && stoi(l.substr(1)) <= upperB)
			return true;
	}

	return false;
}
void cell::setLocation(const string& l) {
	if (validateLocation(l))
		location = l;
	else {
		throw "Invalid Location!";
	}
}

cell::cell() {
	for (int i = 0; i < 8; i++) {
		adjacent.push_back(nullptr);
		distanceToEdge.push_back(0);
	}
}

void cell::setDistanceToEdge(const int& i) {
	if (i > 7 || i < 0)
		return;//invalid direction
	if (distanceToEdge[i] == 0) {
		cell* cp = this;
		cell* np = adjacent[i];//neighbour in the same direction
		cell* nq = adjacent[getAdjacentIndex(i)];//neighbour in the opposite direction
		if (np != nullptr && np->getDistanceToEdge(i) != 0) {
			distanceToEdge[i] = np->getDistanceToEdge(i) + 1;
		}
		else if (nq != nullptr && nq->getDistanceToEdge(i) != 0) {
			distanceToEdge[i] = nq->getDistanceToEdge(i) - 1;
		}
		else
			while (cp != nullptr) {
				cp = cp->adjacent[i];
				distanceToEdge[i]++;
			}
	}
}

int cell::getDistanceToEdge(const int& i) const {
	return distanceToEdge.at(i);
}
