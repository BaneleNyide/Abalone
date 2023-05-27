/*Lindokuhle Ntombela 221043610
 *Banele Nyide 221019384
 *Vuyo Sibiya 221060528
 *Nqobile Shinga 221055984
 *Sanelisiwe Xolo 220025744
 */
 /*
  * To change this license header, choose License Headers in Project Properties.
  * To change this template file, choose Tools | Templates
  * and open the template in the editor.
  */

#include "board.h"
#include <iomanip>
#include <fstream>

  //New parameter td for the tertiary direction and trd for traverse direction. Also dia if the traversal is  diagonal
string board::traverse(const cell* start, const int& pd, const int& sd, const int& td, const int& trd, bool&& dia) const {
	string ts;
	const cell* linep = start;
	while (linep != nullptr) {
		const cell* cellp = linep;
		while (cellp != nullptr) {
			ts += (cellp->getLocation() + ' ');

			//For skipping the null pointer at the center of the board when traversing
			if (trd == 0 && cellp == cells.at("E6"))
				cellp = cells.at("G6");
			else if (trd == 1 && cellp == cells.at("E5"))
				cellp = cells.at("G7");
			else if (trd == 2 && cellp == cells.at("F5"))
				cellp = cells.at("F7");
			else if (trd == 3 && cellp == cells.at("G5"))
				cellp = cells.at("E7");
			else if (trd == 4 && cellp == cells.at("G6"))
				cellp = cells.at("E6");
			else if (trd == 5 && cellp == cells.at("G7"))
				cellp = cells.at("E5");
			else if (trd == 6 && cellp == cells.at("F7"))
				cellp = cells.at("F5");
			else if (trd == 7 && cellp == cells.at("E7"))
				cellp = cells.at("G5");
			else
				cellp = cellp->getAdjacent(trd);
		}
		if (dia) {//Diagonal trarvesal
			if (linep->getAdjacent(pd) != nullptr)
				linep = linep->getAdjacent(pd);
			else
				linep = linep->getAdjacent(td);
		}
		else {//Horizontal and vertical trarvesals
			if (linep->getAdjacent(pd) != nullptr)
				linep = linep->getAdjacent(pd);
			else if (linep->getAdjacent(sd) != nullptr)
				linep = linep->getAdjacent(sd);
			else
				linep = linep->getAdjacent(td);
		}
	}
	return ts;
}
void setMarbles(cell* row, const int& z, const char& m) {
	for (int i = 0; i < z; i++) {
		row[i].setMarble(m);
	}
	if (z == 9) {
		char pos[6] = { 0,1,2,6,7,8 };
		for (int i = 0; i < 6; i++) {
			row[pos[i]].setMarble('+');
		}
	}
}
void connectVertical(cell* srow, cell* lrow, const int& rz, bool&& middle, bool&& space, int&& dir) { //This is new traverse that will traverse to direction 0 and 4(North and South)
	int sp = 0;//if connecting any row with row F then sp is set to 1 to shift the postion of F to 1 unit less than the latter row when we reach cell F7
	for (int i = 0; i < rz; i++) {
		if (middle) {
			if (space && i == 5) {
				sp = 1;
				continue;//Skip the space at the center
			}
			srow[i].setAdjacent(lrow + i - sp, dir);
			continue;
		}
		srow[i].setAdjacent(lrow + i + 1, dir);
	}
}
void connectHorizontal(cell* row, const int& rz, bool& space) {
	for (int i = 0; i < rz - 1; i++) {
		if (space && i == 4)
			continue; //Skip the space at the center
		row[i].setAdjacent(row + i + 1, 2);
	}
}

void connectDiagonal(cell* lrow, cell* srow, const int& f, const int& sz, const bool& top, int&& dir, bool&& middle) {
	int sp = 0;

	for (int i = 0; i < sz; i++) {
		if (top) {
			lrow[i + f].setAdjacent(srow + i, dir);
		}
		else {//connects in diagonal 1 and diagonal 7 and also daigonal 3 and diagonal 5 at the same time
			int d2 = 7;
			if (dir == 3) d2 = 5;
			if (middle && i == 4) sp = 1;//skip the space at the of the board by 1
			if (middle && i > 4)
				lrow[i].setAdjacent(srow + i + 2, dir);//after skipping ensure to correct the mismatches of connections.
			else
				lrow[i].setAdjacent(srow + i + 1, dir);
			lrow[i + f].setAdjacent(srow + i + sp, d2);
		}
	}
}
void board::mapCells(cell* row, const char& r, const int& z) {
	int f;
	if (r >= 'A'&& r < 'D')
		f = 4 - (r - 'A');

	else if (r > 'H'&& r <= 'K')
		f = 1 + (r - 'H');

	else if (r >= 'D' && r <= 'H')
		f = 1;
	else return;
	for (int i = 0; i < z; i++) {
		stringstream ss;
		ss << r;
		if (r == 'F' && i + f >= 6) {
			ss << (i + f + 1);
		}
		else {
			ss << (i + f);
		}
		string lc = ss.str();
		row[i].setLocation(lc);
		cells[lc] = row + i;
	}
}
void board::construct(int&& size, bool&&esc, char& r, int& i, char&& marble) {// short function fo pushing rows,connecting horizonta,mapping cells and setting marbles
	rows.push_back(new cell[size]);                                        // All for Connecting horizontal marbles
	connectHorizontal(rows[i], size, esc);
	if (esc)
		mapCells(rows[i], r, size);
	else
		mapCells(rows[i], r, size);
	setMarbles(rows[i], size, marble);

}
board::board() :woc(0), boc(0) {
	char r = 'K';
	for (int i = 0; i < 11; i++) {
		if (i < 4) {
			if (i < 3)
				construct((5 + 2 * i), false, r, i, 'O');//use the contruct for connecting the horizontal marbles
			else
				construct((5 + 2 * i), false, r, i, '+');
		}
		else if (i >= 4 && i <= 6) {
			if (i == 5)
				construct(10, true, r, i, '+');
			else
				construct(11, false, r, i, '+');
		}
		else {
			if (i >= 7) {
				if (i > 7)
					construct((25 - 2 * i), false, r, i, '@');
				else
					construct((25 - 2 * i), false, r, i, '+');
			}
		}
		r--;
	}
	//if not middle 
	for (int i = 1; i < 4; i++)
		connectVertical(rows[i - 1], rows[i], 3 + (2 * i), false, false, 4);

	//middle part
	connectVertical(rows[3], rows[4], 11, true, false, 4);
	connectVertical(rows[4], rows[5], 11, true, true, 4);
	connectVertical(rows[6], rows[5], 11, true, true, 0);
	connectVertical(rows[7], rows[6], 11, true, false, 0);
	for (int i = 10; i > 7; i--)
		connectVertical(rows[i], rows[i - 1], 25 - (2 * i), false, false, 0);
	//top
	for (int i = 1; i < 4; i++) {
		connectDiagonal(rows[i], rows[i - 1], 0, 3 + (2 * i), true, 1, false);
		connectDiagonal(rows[i], rows[i - 1], 2, 3 + (2 * i), true, 7, false);
	}
	//middle
	connectDiagonal(rows[4], rows[3], 1, 10, false, 1, false);//connecting G and H
	connectDiagonal(rows[5], rows[4], 1, 9, false, 1, true);//connecting G and F
	connectDiagonal(rows[5], rows[6], 1, 9, false, 3, true);//connecting F abd E
	connectDiagonal(rows[7], rows[6], 1, 10, false, 1, false);//connecting D and E
	for (int i = 10; i > 7; i--) {
		connectDiagonal(rows[i], rows[i - 1], 0, 25 - (2 * i), true, 7, false);
		connectDiagonal(rows[i - 1], rows[i], 2, 25 - (2 * i), true, 5, false);
	}
	cell* linep = rows[0];//start at K4
	while (linep != nullptr) {
		cell* cellp = linep;
		while (cellp != nullptr) {
			for (int i = 0; i < 8; i++) {
				cellp->setDistanceToEdge(i);
			}
			if (cellp == cells["F5"]) {
				cellp = cells["F7"];
				continue;
			}
			else
				cellp = cellp->getAdjacent(2);
		}
		if (linep->getAdjacent(5) != nullptr)
			linep = linep->getAdjacent(5);
		else if (linep->getAdjacent(4) != nullptr)
			linep = linep->getAdjacent(4);
		else
			linep = linep->getAdjacent(3);
	}
}
board::board(const board& copy) : board() {//constructor delegation - C++11
	woc = copy.woc;
	boc = copy.boc;

	map<string, cell*>::const_iterator it = copy.cells.begin();
	while (it != copy.cells.end()) {
		(cells[it->first])->setMarble((*(it->second)).getMarble());
		it++;
	}
}
map<string, cell*> board::getCells() const {
	return cells;
}
board::~board() {
	for (int i = 0; i < 11; i++) {
		delete[] rows[i];
		rows[i] = nullptr;
	}
	cells.clear();
}

board::operator std::string()  const {
	stringstream ss;
	char r = 'K';
	for (int i = 1; i < 12; i++) {
		if (i < 5) {
			ss << r << setw(11 - 2 * i);
			cell* cellp = rows[i - 1];
			for (int j = 0; j < 3 + (2 * i); j++) {
				ss << cellp[j].getMarble() << " ";
			}
			ss << endl;
		}
		else if (i >= 5 && i < 8) {
			ss << r << setw(3);
			cell* cellp = rows[i - 1];
			if (i == 6) {//f
				for (int j = 0; j < 10; j++) {
					if (j == 5)
						ss << "  " << cellp[j].getMarble() << " ";
					else
						ss << cellp[j].getMarble() << " ";
				}
				ss << endl; r--;
				continue;
			}
			for (int j = 0; j < 11; j++) {
				ss << cellp[j].getMarble() << " ";
			}
			ss << endl;
		}
		else {
			ss << r << setw(2 * i - 13);
			cell* cellp = rows[i - 1];
			for (int j = 0; j < 27 - (2 * i); j++) {
				ss << cellp[j].getMarble() << " ";
			}
			ss << endl;
		}
		r--;
	}
	ss << setw(4);
	for (int numbers = 1; numbers < 12; numbers++)
		ss << numbers << " ";
	ss << endl << endl;//The scoreboard prints the marbles that have been pushed out 
	ss << "______________________" << endl;
	ss << "|    Score Board      |" << endl;
	ss << "|_____________________|" << endl;
	for (int j = 0; j < 3; j++) {
		ss << "|";
		if (j == 0) {
			for (int i = 0; i < boc; i++) {
				ss << " @ ";
			}
			ss << setw(22 - boc * 3) << "|" << endl;
		}
		if (j == 1) {
			for (int i = 0; i < woc; i++) {
				ss << " O ";
			}
			ss << setw(22 - woc * 3) << "|" << endl;
		}
		if (j > 1)
			ss << setw(22) << "|" << endl;

	}
	ss << "|_____________________|" << endl;
	ss << endl << endl;
	return ss.str();
}
string board::Winner()const {
	string s;
	s += "\n|*******************************|\n";
	if (boc < woc)
		s += "|   The Winner is Player '@'    |\n";
	else if (boc > woc)
		s += "|   The Winner is Player 'O'    |\n";
	else
		s += "|        It's A Tie!             |\n";
	s += "*********************************\n";
	return s;
}
string board::traverseDiagonal() const {
	map<string, cell*>::const_iterator it = cells.find(string("D1"));
	cell* start = it->second;
	return traverse(start, 0, 1, 2, 3, true);
}
string board::traverseHorizontal() const {
	map<string, cell*>::const_iterator it = cells.find(string("A4"));
	cell* start = it->second;
	return traverse(start, 7, 0, 1, 2, false);
}
string board::traverseVertical() const {//This is new traverse that will traverse to direction 0 and 4(North and South)
	map<string, cell*>::const_iterator it = cells.find(string("D1"));
	cell* start = it->second;
	return traverse(start, 3, 2, 1, 0, false);
}
bool board::validateMove(const char& m, const string& l, const int& n, const int& fd, const int& md, int& mtype, bool& scoreMove) const {
	if (!cell::validateLocation(l))//confirm existence of location
		return false;
	else {
		cell* cp = cells.at(l);//initialize to leading cell
		const char marble = cp->getMarble();
		if (marble != m)//trying to move a space or a marble that is not yours
			return false;
		for (int i = 1; i < n; i++) {
			cp = cp->getAdjacent(fd);
			if (cp == nullptr || marble != cp->getMarble())//confirm availability and type of claimed resources
				return false;
		}
		if (fd == getAdjacentIndex(md)) {//in-line move
			cp = cells.at(l);//initialize to leading cell
			mtype = 0;//how many opponent marbles are in front of the leading marble i.e in the move direction
			for (int i = 0; i < n; i++) {
				cp = cp->getAdjacent(md);
				if (cp != nullptr) {//confirm availability of required resources
					if (marble == cp->getMarble())//met own marble instead of space
						return false;
					if ('+' != cp->getMarble())//if it belongs to opponent
						mtype++;//count opponent marbles
					else
						break;
				}
				else {
					if (mtype > 0)//opponent's marble will be pushed off the board
						scoreMove = true;
					else//if trying to push your own marble off the board
						return false;
					break;
				}
			}
			if (mtype >= n)//should only push fewer than n of the opponent's marbles
				return false;
		}
		else {//broad-side move
			mtype = -1;
			if (n <= 1)//broad-side move must use at least 2 marbles
				return false;
			cp = cells.at(l);
			for (int i = 0; i < n; i++) {
				cell* dp = cp->getAdjacent(md);//destination pointer
				if (dp != nullptr && '+' == dp->getMarble())//is there an empty space to move to?
					cp = cp->getAdjacent(fd);
				else
					return false;
			}
		}
		return true;
	}
}

bool board::executeMove(const char& m, const string& l, const int& n, const int& fd, const int& md) {
	int moveType = 0;//default to in-line with no following marbles
	bool scoreMove = false;
	bool valid = validateMove(m, l, n, fd, md, moveType, scoreMove);
	if (valid) {
		if (moveType == -1) {//broad-side
			if (cells.find(l) == cells.end()) return false;
			cell* cp = cells.at(l);
			for (int i = 0; i < n; i++) {
				if (cp == nullptr) return false;
				cell* dp = cp->getAdjacent(md);//destination pointer
				if (dp == nullptr) return false;
				dp->setMarble(cp->getMarble());
				cp->setMarble('+');
				cp = cp->getAdjacent(fd);
			}
		}
		else {//in-line
			cell* cp = cells.at(l), *rearp = cp;
			const char marble = cp->getMarble();
			cell* frontp = cp->getAdjacent(md);
			for (int i = 1; i < n; i++) {
				rearp = rearp->getAdjacent(fd);
			}
			if (scoreMove) {
				frontp->setMarble(rearp->getMarble());
				rearp->setMarble('+');
				if (marble == 'O')
					boc++;
				if (marble == '@')
					woc++;
			}
			else {
				cell* tipp = frontp;
				for (int i = 0; i < moveType; i++) {
					tipp = tipp->getAdjacent(md);//looking for empty space
				}
				if (moveType != 0)//tipp is already equal to frontp
					tipp->setMarble(frontp->getMarble());
				frontp->setMarble(rearp->getMarble());
				rearp->setMarble('+');
			}
		}
	}
	return valid;
}
bool board::inPlay() const {
	if ((woc < 6) && (boc < 6))
		return true;
	return false;
}
