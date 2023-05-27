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

#include "agent.h"

movement::movement(const string& ll, const int& nn, const int& f, const int& m, const int& d, const int& mt, const bool& sm) :
	l(ll), n(nn), fd(f), md(m), d2e(d), mtype(mt), scoreMove(sm) {}
movement::movement(const movement& copy) : l(copy.l), n(copy.n), fd(copy.fd), md(copy.md), d2e(copy.d2e), mtype(copy.mtype), scoreMove(copy.scoreMove) {}

agent::agent(const char& c) : symbol(c) {
	for (int n = 1; n < 4; n++) {
		for (int fd = 0; fd < 8; fd++) {//all 8 directions
			if (n == 1) {//will create 8 moves for n=1
				vector<int> tv;//declared 120 times
				tv.push_back(n);
				tv.push_back(fd);
				tv.push_back(getAdjacentIndex(fd));
				pmd.push_back(tv);
			}
			else {//will create a total of 112 moves for n=2 and n=3
				for (int md = 0; md < 8; md++) {
					if (md != fd) {//moving towards own marbles (not possible)
						vector<int> tv;
						tv.push_back(n);
						tv.push_back(fd);
						tv.push_back(md);
						pmd.push_back(tv);
					}
				}
			}
		}
	}
}
vector<movement> agent::findMoves(const board& bd) const {
	vector<movement> mv;
	map<string, cell*> cells = bd.getCells();
	for (map<string, cell*>::iterator it = cells.begin(); it != cells.end(); it++) {
		string l = it->first;
		cell* cp = it->second;
		if (cp != nullptr && cp->getMarble() == symbol) {
			set<int> mdSet;//directions in which neighbours either have a space or an opponent marble[possible md  for a cell with location l
			for (int md = 0; md < 8; md++) {
				cell* np = cp->getAdjacent(md);//neighbour
				if (np != nullptr && np->getMarble() != symbol) {
					mdSet.insert(md);
				}
			}
			for (vector<int> tv : pmd) {
				int md = tv[2];
				if (mdSet.find(md) != mdSet.end()) {
					int mtype = 0;//default to in-line with no following marbles
					bool scoreMove = false;
					if (bd.validateMove(symbol, l, tv[0], tv[1], md, mtype, scoreMove))
						mv.push_back(movement(l, tv[0], tv[1], md, cp->getDistanceToEdge(md), mtype, scoreMove));
				}
			}
		}
	}
	return mv;
}

double score2(const movement& a) {
	double score = 1;
	if (a.scoreMove)
		score *= 2;
	return score;
}
double score(const movement& a) {
	double maxScore = 0;
	if (a.mtype < 0)
		maxScore = 0.6;
	else if (a.mtype > 0)
		maxScore = 0.2;
	else
		maxScore = 0.8;
	if (a.mtype > 0) {
		if (a.scoreMove)
			maxScore = 0;
		return maxScore * (1 + (1 - a.d2e) / 9.0);
	}
	else {
		return maxScore * (1 - abs(5 - a.d2e) / 5.0);
	}
}
distAgent::distAgent(const char& c) :agent(c) {}
randAgent::randAgent(const char& c) : agent(c) {}
//this agent plays random moves but if there is a score move they always play it
randScoreMoveAgent::randScoreMoveAgent(const char& c) : agent(c) {}

movement distAgent::move(movement const* const om, board bd) {
	vector<movement> mv = findMoves(bd);//RETURNS ALL VALID MOVES AN AGENT CAN MAKE
	sort(mv.begin(), mv.end(), moveTNDPriority());
	const int bm2c = 3;//consider only these top best moves.
	if (mv.size() == 0)
		throw string("Error! There are no available moves!");
	else if (mv.size() < bm2c)
		return mv[0];

	int i = rand() % bm2c;
	return mv[i];
}
movement randAgent::move(movement const* const om, board bd) {
	vector<movement> mv = findMoves(bd);
	const int bm2c = mv.size();
	if (mv.size() == 0) {
		throw string("Error! There are no available moves!");
	}
	else if (mv.size() == 1)
		return mv[0];
	int i = rand() % bm2c;
	return mv[i];
}
movement randScoreMoveAgent::move(movement const* const om, board bd) {
	vector<movement> mv = findMoves(bd);
	sort(mv.begin(), mv.end(), scoreMovePriority());
	const int bm2c = mv.size();
	if (mv.size() == 0) {
		throw string("Error! There are no available moves!");
	}
	if (mv[0].scoreMove || mv.size() == 1)//considers a scoremove if any
		return mv[0];
	int i = rand() % bm2c;
	return mv[i];
}
string randScoreMoveAgent::toString() {
	string s = "RandScoreMoveAgent   marble character: ";
	return s + symbol;
}
string distAgent::toString() {
	string s = "DistAgent   marble character: ";
	return s + symbol;
}
string randAgent::toString() {
	string s = "RandAgent   marble character: ";
	return s + symbol;
}