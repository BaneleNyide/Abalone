/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   main.cpp
  * Author: MSCS
  *
  * Created on 12 March 2020, 8:38 AM
  */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include "agent.h"

int main(int argc, char** argv) {
	time_t now = time(NULL);
	srand(now);
	cout<< "Seed: " << now << endl;
	board abalone;
	string state(abalone);//string representing current state of the board
	vector<agent*>agnt1 = { new randAgent('O'),new distAgent('O'),new randScoreMoveAgent('O') };
	vector<agent*>agnt2 = { new randAgent('@'),new distAgent('@'),new randScoreMoveAgent('@') };
	agent* w = agnt1[rand() % 3];
	agent* b = agnt2[rand() % 3];
	cout << w->toString() << "   v.s   " << b->toString() << endl;
	cout << "Initial State:" << endl << state;
	movement* pom = nullptr;//previous opponent's move
	movement* mym = nullptr;//player's chosen move
	char c = 'O';
	for (int i = 0; i < 200 && abalone.inPlay(); i++) {
		try {
			if (c == 'O') {
				movement mm = w->move(pom, abalone);//1
				mym = new movement(mm);
				if (pom)
					delete pom;
				pom = new movement(mm);
			}
			else {
				movement mm = b->move(pom, abalone);
				mym = new movement(mm);
				if (pom)
					delete pom;
				pom = new movement(mm);
			}
		}
		catch (const string& s) {
			cout << s;
			return 1;
		}
		bool valid = abalone.executeMove(c, mym->l, mym->n, mym->fd, mym->md);
		cout << "Move " << i + 1 << ": " << c << "," << mym->l << "," << mym->n << "," << mym->fd << "," << mym->md << endl;
		if (valid) {
			string state(abalone);
			cout << "Next State:" << endl << state;
		}
		else {
			cout << "Invalid move!!" << endl;
		}
		if (c == 'O')
			c = '@';
		else
			c = 'O';
		delete mym;	
		SLP(0.5);
	}
	cout << abalone.Winner() << endl;
	if (pom)
		delete pom;
	
	for (int i = 0; i < 3; i++) {
		delete agnt1[i];
		delete agnt2[i];
		agnt1[i] = nullptr;
		agnt2[i] = nullptr;
	}
	int p;
	cin >> p;
	return 0;
}

