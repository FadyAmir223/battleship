#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <string>
using namespace std;

void display_set();
void display_game();
void set_AI();
void target_AI();
void set_player();
void target_player();
void initlization();
bool win();
void after_winning();
void view();
void close_around_sink();
void color_exchange();

struct coordinate {
	int x1, y1, x2, y2;
};

const int n = 10;
int i, j, k;
string c;

//set AI
int X1, Y1, X2, Y2;						//coordinates set between
bool set_before[n][n];						//brevent set on or around ship
bool set_up, set_down, set_right, set_left;			//direction options
bool no_set_up, no_set_down, no_set_right, no_set_left;		//directions excluded
bool wrong_start, done, taken_before;

//target AI
int x, y;							//coordinate targeted
int go_up, go_down, go_right, go_left;				//direction to continue when hit
bool hit_before_AI[n][n];					//prevent hit place twice
bool hit, right_direction;					//random -> target
bool up, down, rightt, leftt;					//direction to try
bool no_up, no_down, no_right, no_left;				//direction to exclude
bool entered_direction;

//set player
bool wrong_input;
char x1_ch, x2_ch;

//target player
bool rep_P1, rep_P2, rep_AI;					//if one hit he hits again
bool hit_before_human[n][n];

//Game
int choice, p1_p2;						//define whose turn to play and diplay
int sink_P1, sink_P2, sink_AI;					//number of sinked parts
char oceanP1[n][n], oceanP2[n][n], oceanAI[n][n];		//display ships place & hitted places
char targetP1[n][n], targetP2[n][n];				//dislpay X O only
bool sb_P1[n][n], sb_P2[n][n], sb_AI[n][n];			//set before (ocean)
bool hb_P1[n][n], hb_P2[n][n], hb_AI[n][n];			//hit before (target)

//know ship sinked
int R_AI, B_AI, U_AI, S_AI, D_AI;
int R_P1, B_P1, U_P1, S_P1, D_P1;
int R_P2, B_P2, U_P2, S_P2, D_P2;
//save coordinates, close around the ship
coordinate s_AI[5], s_P1[5], s_P2[5];

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int main() {
	do {
		initlization();
		view();
		if (choice == 1) {
			set_AI();
			set_player();
			do {
				display_game();
				if(!rep_AI) target_player();
				if (sink_AI == 17) break;
				if (!rep_P1) target_AI();
			} while (win());
		}
		
		if (choice == 2) {
			set_player();
			set_player();
			do {
				display_game();
				target_player();
				if(!rep_P1 && !rep_P2) p1_p2++;
			} while (win());
		}
		display_game();
		after_winning();
	} while (true);

	return 0;
}

void display_set() {
	system("CLS");
	//first
	SetConsoleTextAttribute(hConsole, 0x08);
	cout << " ";
	for (i = 0; i < n; i++)
		cout << " " << i % 10;
	cout << endl << " ";
	bool c194 = false;
	cout << char(218);
	for (i = 0; i < n; i++) {
		if (c194) cout << char(194);
		cout << char(196);
		c194 = true;
	}
	cout << char(191) << endl;
	//middle
	bool hidden_ij = false, c197 = false;
	for (i = 0; i < n; i++) {
		//1st
		if (hidden_ij) {
			cout << " " << char(195);
			for (int j = 0; j < n; j++) {
				if (c197)
					cout << char(197);
				cout << char(196);
				c197 = true;
			}
			c197 = false;
			cout << char(180) << endl;
		}
		//2nd
		cout << char(65 + i) << char(179);;
		for (j = 0; j < n; j++) {
			if (p1_p2 % 2 == 0) {
				if (oceanP1[i][j] == 'R') SetConsoleTextAttribute(hConsole, 0x09);
				if (oceanP1[i][j] == 'B') SetConsoleTextAttribute(hConsole, 0x0d);
				if (oceanP1[i][j] == 'U') SetConsoleTextAttribute(hConsole, 0x05);
				if (oceanP1[i][j] == 'S') SetConsoleTextAttribute(hConsole, 0x0b);
				if (oceanP1[i][j] == 'D') SetConsoleTextAttribute(hConsole, 0x0e);
			}
			if (p1_p2 % 2 == 1) {
				if (oceanP2[i][j] == 'R') SetConsoleTextAttribute(hConsole, 0x09);
				if (oceanP2[i][j] == 'B') SetConsoleTextAttribute(hConsole, 0x0d);
				if (oceanP2[i][j] == 'U') SetConsoleTextAttribute(hConsole, 0x05);
				if (oceanP2[i][j] == 'S') SetConsoleTextAttribute(hConsole, 0x0b);
				if (oceanP2[i][j] == 'D') SetConsoleTextAttribute(hConsole, 0x0e);
			}
			//ocean grid
			if (choice == 1) cout << oceanP1[i][j];
			if (choice == 2) {
				if (p1_p2 % 2 == 0) cout << oceanP1[i][j];
				if (p1_p2 % 2 == 1) cout << oceanP2[i][j];
			}
			SetConsoleTextAttribute(hConsole, 0x08);
			cout << char(179);
		}
		cout << endl;
		hidden_ij = true;
	}
	//final
	bool c193 = false;
	cout << " " << char(192);
	for (i = 0; i < n; i++) {
		if (c193) cout << char(193);
		cout << char(196);
		c193 = true;
	}
	cout << char(217) << endl;
}

void display_game() {
	system("CLS");
	if (choice == 2) {
		if (p1_p2 % 2 == 0) SetConsoleTextAttribute(hConsole, 0x08);
		if (p1_p2 % 2 == 1) SetConsoleTextAttribute(hConsole, 0x07);
		cout << "\tPlayer 1\t";
		if (p1_p2 % 2 == 0) SetConsoleTextAttribute(hConsole, 0x07);
		if (p1_p2 % 2 == 1) SetConsoleTextAttribute(hConsole, 0x08);
		cout << "\tPlayer 2 \n";
	}
	//first
	SetConsoleTextAttribute(hConsole, 0x08);
	for (k = 0; k < 2; k++) {
		color_exchange();
		if (k == 0) cout << " ";
		if (k == 1) cout << "   ";
		for (i = 0; i < n; i++) cout << " " << i % 10;
	}
	cout << endl << " ";
	for (k = 0; k < 2; k++) {
		color_exchange();
		if (k == 1) cout << "  ";
		bool c194 = false;
		cout << char(218);
		for (i = 0; i < n; i++) {
			if (c194) cout << char(194);
			cout << char(196);
			c194 = true;
		}
		cout << char(191);
	}
	cout << endl;
	//middle
	bool hidden_ij = false, c197 = false;
	for (i = 0; i < n; i++) {
		//1st
		for (k = 0; k < 2; k++) {
			color_exchange();
			if (hidden_ij) {
				if (k == 1) cout << " ";
				cout << " " << char(195);
				for (j = 0; j < n; j++) {
					if (c197) cout << char(197);
					cout << char(196);
					c197 = true;
				}
				c197 = false;
				cout << char(180);
			}
		}
		if (hidden_ij) cout << endl;
		//2nd
		for (k = 0; k < 2; k++) {
			color_exchange();
			if (k == 1) cout << " ";
			cout << char(65 + i) << char(179);
			for (j = 0; j < n; j++) {
				if (k == 0) {
					//color
					if (choice == 1) {
						if (oceanP1[i][j] == 'X') SetConsoleTextAttribute(hConsole, 0x0c);
						if (oceanP1[i][j] == 'O') SetConsoleTextAttribute(hConsole, 0x02);
						if (oceanP1[i][j] == 'R') SetConsoleTextAttribute(hConsole, 0x09);
						if (oceanP1[i][j] == 'B') SetConsoleTextAttribute(hConsole, 0x0d);
						if (oceanP1[i][j] == 'U') SetConsoleTextAttribute(hConsole, 0x05);
						if (oceanP1[i][j] == 'S') SetConsoleTextAttribute(hConsole, 0x0b);
						if (oceanP1[i][j] == 'D') SetConsoleTextAttribute(hConsole, 0x0e);
					}
					if (choice == 2) {
						if (oceanP1[i][j] == 'X') SetConsoleTextAttribute(hConsole, 0x0c);
						if (oceanP1[i][j] == 'O') SetConsoleTextAttribute(hConsole, 0x02);
					}
				}
				if (k == 1) {
					if (choice == 1) {
						if (oceanAI[i][j] == 'X') SetConsoleTextAttribute(hConsole, 0x0c);
						if (oceanAI[i][j] == 'O') SetConsoleTextAttribute(hConsole, 0x02);
					}
					if (choice == 2) {
						if (oceanP2[i][j] == 'X') SetConsoleTextAttribute(hConsole, 0x0c);
						if (oceanP2[i][j] == 'O') SetConsoleTextAttribute(hConsole, 0x02);
					}
				}
				//view
				//ocean grid
				if (k == 0) {
					if (choice == 1) cout << oceanP1[i][j];
					if (choice == 2) cout << targetP2[i][j];
				}
				//target grid
				if (k == 1) {
					if (choice == 1) cout << targetP1[i][j];
					if (choice == 2) cout << targetP1[i][j];
				}
				SetConsoleTextAttribute(hConsole, 0x08);
				color_exchange();
				cout << char(179);
			}
			hidden_ij = true;
		}
		cout << endl;
	}
	//final
	cout << " ";
	for (k = 0; k < 2; k++) {
		color_exchange();
		if (k == 1) cout << "  ";
		bool c193 = false;
		cout << char(192);
		for (i = 0; i < n; i++) {
			if (c193) cout << char(193);
			cout << char(196);
			c193 = true;
		}
		cout << char(217);
	}
	cout << endl;
}

//AI
void set_AI() {
	srand(time(NULL));
	//random start point
	for (k = 0; k < 5; k++) {
		do {
			do {
				X1 = rand() % n;
				Y1 = rand() % n;
			} while (sb_AI[X1][Y1]);
			done = true;
			no_set_up = true, no_set_down = true, no_set_right = true, no_set_left = true;
			//random direction
			do {
				int set_direction = rand() % 4;
				if (k == 0) {
					if (set_direction == 0 && no_set_up) X2 = X1 - 4, Y2 = Y1;
					if (set_direction == 1 && no_set_down) X2 = X1 + 4, Y2 = Y1;
					if (set_direction == 2 && no_set_right) X2 = X1, Y2 = Y1 + 4;
					if (set_direction == 3 && no_set_left) X2 = X1, Y2 = Y1 - 4;
				}
				if (k == 1) {
					if (set_direction == 0 && no_set_up) X2 = X1 - 3, Y2 = Y1;
					if (set_direction == 1 && no_set_down) X2 = X1 + 3, Y2 = Y1;
					if (set_direction == 2 && no_set_right) X2 = X1, Y2 = Y1 + 3;
					if (set_direction == 3 && no_set_left) X2 = X1, Y2 = Y1 - 3;
				}
				if (k == 2) {
					if (set_direction == 0 && no_set_up) X2 = X1 - 2, Y2 = Y1;
					if (set_direction == 1 && no_set_down) X2 = X1 + 2, Y2 = Y1;
					if (set_direction == 2 && no_set_right) X2 = X1, Y2 = Y1 + 2;
					if (set_direction == 3 && no_set_left) X2 = X1, Y2 = Y1 - 2;
				}
				if (k == 3) {
					if (set_direction == 0 && no_set_up) X2 = X1 - 2, Y2 = Y1;
					if (set_direction == 1 && no_set_down) X2 = X1 + 2, Y2 = Y1;
					if (set_direction == 2 && no_set_right) X2 = X1, Y2 = Y1 + 2;
					if (set_direction == 3 && no_set_left) X2 = X1, Y2 = Y1 - 2;
				}
				if (k == 4) {
					if (set_direction == 0 && no_set_up) X2 = X1 - 1, Y2 = Y1;
					if (set_direction == 1 && no_set_down) X2 = X1 + 1, Y2 = Y1;
					if (set_direction == 2 && no_set_right) X2 = X1, Y2 = Y1 + 1;
					if (set_direction == 3 && no_set_left) X2 = X1, Y2 = Y1 - 1;
				}
				//for now computer entered 2 coordinates
				//confirm
				taken_before = false;
				if (X2 >= 0 && Y2 >= 0 && X2 < n - 1 && Y2 < n - 1) {
					if (X1 > X2) swap(X1, X2); //x2 > x1
					if (Y1 > Y2) swap(Y1, Y2); //y2 > y1
					for (i = X1; i <= X2; i++)
						for (j = Y1; j <= Y2; j++)
							if (sb_AI[i][j]) taken_before = true;
					if (!taken_before) done = false, wrong_start = false;
				}
				if (set_direction == 0 && done) no_set_up = false;
				if (set_direction == 1 && done) no_set_down = false;
				if (set_direction == 2 && done) no_set_right = false;
				if (set_direction == 3 && done) no_set_left = false;

				if (!no_set_up && !no_set_down && !no_set_right && !no_set_left) done = false, wrong_start = true;
				if (taken_before) done = false, wrong_start = true;
			} while (done);
		} while (wrong_start);
		//set ship
		if (X1 == X2) {
			if (Y2 < Y1) swap(Y2, Y1); //y1 > y2
			for (i = Y1; i <= Y2; i++) {
				if (k == 0) oceanAI[X1][i] = 'R';
				if (k == 1) oceanAI[X1][i] = 'B';
				if (k == 2) oceanAI[X1][i] = 'U';
				if (k == 3) oceanAI[X1][i] = 'S';
				if (k == 4) oceanAI[X1][i] = 'D';
			}
		}
		if (Y1 == Y2) {
			if (X2 < X1) swap(X2, X1); //x1 > x2
			for (i = X1; i <= X2; i++) {
				if (k == 0) oceanAI[i][Y1] = 'R';
				if (k == 1) oceanAI[i][Y1] = 'B';
				if (k == 2) oceanAI[i][Y1] = 'U';
				if (k == 3) oceanAI[i][Y1] = 'S';
				if (k == 4) oceanAI[i][Y1] = 'D';
			}
		}
		//colse around ship
		if (X1 > X2) swap(X1, X2); //x2 > x1
		if (Y1 > Y2) swap(Y1, Y2); //y2 > y1
		for (i = X1 - 1; i <= X2 + 1; i++)
			for (j = Y1 - 1; j <= Y2 + 1; j++)
				if (i >= 0 && j >= 0 && i < n && j < n) sb_AI[i][j] = true;
		//save coordinates
		s_AI[k].x1 = X1;
		s_AI[k].y1 = Y1;
		s_AI[k].x2 = X2;
		s_AI[k].y2 = Y2;
	}
}

void target_AI() {
	//random hit
	srand(time(0));
	if (!hit) {
		//computer input
		do {
			x = rand() % n;
			y = rand() % n;
		} while (hb_AI[x][y]);
		hb_AI[x][y] = true;
		//hit
		if (oceanP1[x][y] == 'R' || oceanP1[x][y] == 'B' || oceanP1[x][y] == 'U'
			|| oceanP1[x][y] == 'S' || oceanP1[x][y] == 'D') {
			hit = true;
			if (oceanP1[x][y] == 'R') R_P1--;
			if (oceanP1[x][y] == 'B') B_P1--;
			if (oceanP1[x][y] == 'U') U_P1--;
			if (oceanP1[x][y] == 'S') S_P1--;
			if (oceanP1[x][y] == 'D') D_P1--;
			oceanP1[x][y] = 'O';
			sink_P1++;
			rep_AI = true;
		}
		//miss
		else {
			oceanP1[x][y] = 'X';
			rep_AI = false;
		}
	}

	//target
	else {
		entered_direction = true;
		if (!right_direction) {
			//random direction
			up = false, down = false, rightt = false, leftt = false;
			do {
				int direction = rand() % 4;
				if (direction == 0 && no_up && !hb_AI[x - go_up][y] && x > 0)
					up = true, entered_direction = false;
				if (direction == 1 && no_down && !hb_AI[x + go_down][y] && x < 9)
					down = true, entered_direction = false;
				if (direction == 2 && no_right && !hb_AI[x][y + go_right] && y < 9)
					rightt = true, entered_direction = false;
				if (direction == 3 && no_left && !hb_AI[x][y - go_left] && y > 0)
					leftt = true, entered_direction = false;

			} while (entered_direction);
		}
		//UP
		if (x > 0 && up && !hb_AI[x - go_up][y]) {
			//hit
			if (oceanP1[x - go_up][y] == 'R' || oceanP1[x - go_up][y] == 'B' || oceanP1[x - go_up][y] == 'U'
				|| oceanP1[x - go_up][y] == 'S' || oceanP1[x - go_up][y] == 'D') {
				if (oceanP1[x - go_up][y] == 'R') R_P1--;
				if (oceanP1[x - go_up][y] == 'B') B_P1--;
				if (oceanP1[x - go_up][y] == 'U') U_P1--;
				if (oceanP1[x - go_up][y] == 'S') S_P1--;
				if (oceanP1[x - go_up][y] == 'D') D_P1--;
				oceanP1[x - go_up][y] = 'O';
				sink_P1++;
				if (hb_AI[x - go_up - 1][y] || x - go_up - 1 < 0) { up = false, down = true; }
				right_direction = true, no_right = false, no_left = false;
				rep_AI = true;
			}
			//miss
			else {
				no_up = false, right_direction = false, rep_AI = false;
				if (x >= go_up) oceanP1[x - go_up][y] = 'X';
			}
			hb_AI[x - go_up][y] = true;
			go_up++;
		}
		//DOWN
		else if (x < n - go_down && down && !hb_AI[x + go_down][y]) {
			//hit
			if (oceanP1[x + go_down][y] == 'R' || oceanP1[x + go_down][y] == 'B' || oceanP1[x + go_down][y] == 'U'
				|| oceanP1[x + go_down][y] == 'S' || oceanP1[x + go_down][y] == 'D') {
				if (oceanP1[x + go_down][y] == 'R') R_P1--;
				if (oceanP1[x + go_down][y] == 'B') B_P1--;
				if (oceanP1[x + go_down][y] == 'U') U_P1--;
				if (oceanP1[x + go_down][y] == 'S') S_P1--;
				if (oceanP1[x + go_down][y] == 'D') D_P1--;
				oceanP1[x + go_down][y] = 'O';
				sink_P1++;
				if (hb_AI[x + go_down + 1][y] || x + go_down + 1 > n - 1) { down = false, up = true; }
				right_direction = true, no_right = false, no_left = false;
				rep_AI = true;
			}
			//miss
			else {
				no_down = false, right_direction = false, rep_AI = false;
				if (x >= -go_down) oceanP1[x + go_down][y] = 'X';
			}
			hb_AI[x + go_down][y] = true;
			go_down++;
		}
		//RIGHT
		else if (y < n - go_right && rightt && !hb_AI[x][y + go_right]) {
			//hit
			if (oceanP1[x][y + go_right] == 'R' || oceanP1[x][y + go_right] == 'B' || oceanP1[x][y + go_right] == 'U'
				|| oceanP1[x][y + go_right] == 'S' || oceanP1[x][y + go_right] == 'D') {
				if (oceanP1[x][y + go_right] == 'R') R_P1--;
				if (oceanP1[x][y + go_right] == 'B') B_P1--;
				if (oceanP1[x][y + go_right] == 'U') U_P1--;
				if (oceanP1[x][y + go_right] == 'S') S_P1--;
				if (oceanP1[x][y + go_right] == 'D') D_P1--;
				oceanP1[x][y + go_right] = 'O';
				sink_P1++;
				if (hb_AI[x][y + go_right + 1] || y + go_right + 1>n - 1) { rightt = false, leftt = true; }
				right_direction = true, no_up = false, no_down = false;
				rep_AI = true;
			}
			//miss
			else {
				no_right = false, right_direction = false, rep_AI = false;
				if (y >= -go_right) oceanP1[x][y + go_right] = 'X';
			}
			hb_AI[x][y + go_right] = true;
			go_right++;
		}
		//LEFT
		else if (y > 0 && leftt && !hb_AI[x][y - go_left]) {
			//hit
			if (oceanP1[x][y - go_left] == 'R' || oceanP1[x][y - go_left] == 'B' || oceanP1[x][y - go_left] == 'U'
				|| oceanP1[x][y - go_left] == 'S' || oceanP1[x][y - go_left] == 'D') {
				if (oceanP1[x][y - go_left] == 'R') R_P1--;
				if (oceanP1[x][y - go_left] == 'B') B_P1--;
				if (oceanP1[x][y - go_left] == 'U') U_P1--;
				if (oceanP1[x][y - go_left] == 'S') S_P1--;
				if (oceanP1[x][y - go_left] == 'D') D_P1--;
				oceanP1[x][y - go_left] = 'O';
				sink_P1++;
				if (hb_AI[x][y - go_left - 1] || y - go_left - 1<0) { leftt = false, rightt = true; }
				right_direction = true, no_up = false, no_down = false;
				rep_AI = true;
			}
			//miss
			else {
				no_left = false, right_direction = false, rep_AI = false;
				if (y >= go_left) oceanP1[x][y - go_left] = 'X';
			}
			hb_AI[x][y - go_left] = true;
			go_left++;
		}
		//sinked
		if (R_P1 == 0 || B_P1 == 0 || U_P1 == 0 || S_P1 == 0 || D_P1 == 0) {
			close_around_sink();
			go_up = 1, go_down = 1, go_right = 1, go_left = 1;
			up = false, down = false, rightt = false, leftt = false;
			no_up = true, no_down = true, no_right = true, no_left = true;
			hit = false, right_direction = false, rep_AI = false;
		}
	}
}

//human
void set_player() {
	for (k = 0; k < 5; k++) {
		display_set();
		done = true;
		do {
			if (k == 0) cout << "ship R (5) \n";
			if (k == 1) cout << "ship B (4) \n";
			if (k == 2) cout << "ship U (3) \n";
			if (k == 3) cout << "ship S (3) \n";
			if (k == 4) cout << "ship D (2) \n";

			wrong_input = false, taken_before = false;
			cout << "Enter point 1: ";
			cin >> x1_ch >> Y1;
			if (cin.fail()) {
				wrong_input = true;
				cin.clear();
				cin >> c;
			}
			cout << "Enter point 2: ";
			cin >> x2_ch >> Y2;
			if (cin.fail()) {
				wrong_input = true;
				cin.clear();
				cin >> c;
			}
			for (i = 0; i < n; i++) if (x1_ch == char(65 + i) || x1_ch == char(97 + i))X1 = i;
			for (i = 0; i < n; i++) if (x2_ch == char(65 + i) || x2_ch == char(97 + i))X2 = i;
			//confirm
			//ship place
			if (X1 >= 0 && Y1 >= 0 && X1 < n && Y1 < n
				&& X2 >= 0 && Y2 >= 0 && X2 < n && Y2 < n) {
				if (X1 > X2) swap(X1, X2); //x2 > x1
				if (Y1 > Y2) swap(Y1, Y2); //y2 > y1
				for (i = X1; i <= X2; i++)
					for (j = Y1; j <= Y2; j++) {
						if (sb_P1[i][j] && p1_p2 % 2 == 0) taken_before = true;
						if (sb_P2[i][j] && p1_p2 % 2 == 1) taken_before = true;
					}
				if (!taken_before) done = false;
			}
			//ship length
			if (k == 0 && (!(X1 == X2 || Y1 == Y2) || !(X2 - X1 == 4 || Y2 - Y1 == 4))) done = true;
			if (k == 1 && (!(X1 == X2 || Y1 == Y2) || !(X2 - X1 == 3 || Y2 - Y1 == 3))) done = true;
			if (k == 2 && (!(X1 == X2 || Y1 == Y2) || !(X2 - X1 == 2 || Y2 - Y1 == 2))) done = true;
			if (k == 3 && (!(X1 == X2 || Y1 == Y2) || !(X2 - X1 == 2 || Y2 - Y1 == 2))) done = true;
			if (k == 4 && (!(X1 == X2 || Y1 == Y2) || !(X2 - X1 == 1 || Y2 - Y1 == 1))) done = true;
			//X axis (char)
			if (x1_ch < char(65) || (x1_ch > char(65 + n - 1) && x1_ch < char(97)) || x1_ch > char(97 + n - 1) ||
				x2_ch < char(65) || (x2_ch > char(65 + n - 1) && x2_ch < char(97)) || x2_ch > char(97 + n - 1))
				done = true;
			//Y axis (int)
			if (wrong_input || Y1 < 0 || Y1 > n - 1 || Y2 < 0 || Y2 > n - 1) done = true;
			if (done) {
				display_set();
				cout << "Invalid Coordinate, Please Enter Again \n";
			}
		} while (done);
		//set ship
		if (X1 == X2) {
			if (Y2 < Y1) swap(Y2, Y1); //y1 > y2
			for (i = Y1; i <= Y2; i++) {
				if (p1_p2 % 2 == 0) {
					if (k == 0) oceanP1[X1][i] = 'R';
					if (k == 1) oceanP1[X1][i] = 'B';
					if (k == 2) oceanP1[X1][i] = 'U';
					if (k == 3) oceanP1[X1][i] = 'S';
					if (k == 4) oceanP1[X1][i] = 'D';
				}
				if (p1_p2 % 2 == 1) {
					if (k == 0) oceanP2[X1][i] = 'R';
					if (k == 1) oceanP2[X1][i] = 'B';
					if (k == 2) oceanP2[X1][i] = 'U';
					if (k == 3) oceanP2[X1][i] = 'S';
					if (k == 4) oceanP2[X1][i] = 'D';
				}
			}
		}
		if (Y1 == Y2) {
			if (X2 < X1) swap(X2, X1); //x1 > x2
			for (i = X1; i <= X2; i++) {
				if (p1_p2 % 2 == 0) {
					if (k == 0) oceanP1[i][Y1] = 'R';
					if (k == 1) oceanP1[i][Y1] = 'B';
					if (k == 2) oceanP1[i][Y1] = 'U';
					if (k == 3) oceanP1[i][Y1] = 'S';
					if (k == 4) oceanP1[i][Y1] = 'D';
				}
				if (p1_p2 % 2 == 1) {
					if (k == 0) oceanP2[i][Y1] = 'R';
					if (k == 1) oceanP2[i][Y1] = 'B';
					if (k == 2) oceanP2[i][Y1] = 'U';
					if (k == 3) oceanP2[i][Y1] = 'S';
					if (k == 4) oceanP2[i][Y1] = 'D';
				}
			}
		}
		//colse around ship
		if (X1 > X2) swap(X1, X2); //x2 > x1
		if (Y1 > Y2) swap(Y1, Y2); //y2 > y1
		for (i = X1 - 1; i <= X2 + 1; i++)
			for (j = Y1 - 1; j <= Y2 + 1; j++) {
				if (i >= 0 && j >= 0 && i < n && j < n && p1_p2 % 2 == 0) sb_P1[i][j] = true;
				if (i >= 0 && j >= 0 && i < n && j < n && p1_p2 % 2 == 1) sb_P2[i][j] = true;
			}
		//save coordinates
		if (p1_p2 % 2 == 0) {
			s_P1[k].x1 = X1;
			s_P1[k].y1 = Y1;
			s_P1[k].x2 = X2;
			s_P1[k].y2 = Y2;
		}
		if (p1_p2 % 2 == 1) {
			s_P2[k].x1 = X1;
			s_P2[k].y1 = Y1;
			s_P2[k].x2 = X2;
			s_P2[k].y2 = Y2;
		}
	}
	display_set();
	if (choice == 2 && p1_p2 == 0) {
		cout << "Get The Other Player \n";
		system("pause");
	}
	if (choice == 2) p1_p2++;
}

void target_player() {
	do {
		if (choice == 2) {
			SetConsoleTextAttribute(hConsole, 0x07);
			if (p1_p2 % 2 == 0) cout << "Player 1 to play: \n";
			if (p1_p2 % 2 == 1) cout << "Player 2 to play: \n";
			SetConsoleTextAttribute(hConsole, 0x08);
		}
		cout << "Enter point: ";
		cin >> x1_ch >> Y1;
		wrong_input = false;
		if (cin.fail()) {
			wrong_input = true;
			cin.clear();
			cin >> c;
		}
		for (i = 0; i < n; i++) if (x1_ch == char(65 + i) || x1_ch == char(97 + i)) X1 = i;
		//confirm X axis (char)
		if (x1_ch < char(65) || (x1_ch > char(65 + n - 1) && x1_ch < char(97)) || x1_ch > char(97 + n - 1))
			wrong_input = true;
		//confirm Y axis (int)
		if (Y1 < 0 || Y1 > n - 1) wrong_input = true;
		if ((hb_P1[X1][Y1] && p1_p2 % 2 == 0) || (hb_P2[X1][Y1] && p1_p2 % 2 == 1) || wrong_input) {
			display_game();
			SetConsoleTextAttribute(hConsole, 0x08);
			cout << "Invalid Coordinate, Please Enter Again \n";
		}
	} while ((hb_P1[X1][Y1] && p1_p2 % 2 == 0) || (hb_P2[X1][Y1] && p1_p2 % 2 == 1) || wrong_input);

	if (p1_p2 % 2 == 0) hb_P1[X1][Y1] = true;
	if (p1_p2 % 2 == 1) hb_P2[X1][Y1] = true;

	if (choice == 1) {
		if (oceanAI[X1][Y1] == char(0)) {
			oceanAI[X1][Y1] = 'X', targetP1[X1][Y1] = 'X';
			rep_P1 = false;
		}
		else {
			if (oceanAI[X1][Y1] == 'R') R_AI--;
			if (oceanAI[X1][Y1] == 'B') B_AI--;
			if (oceanAI[X1][Y1] == 'U') U_AI--;
			if (oceanAI[X1][Y1] == 'S') S_AI--;
			if (oceanAI[X1][Y1] == 'D') D_AI--;
			oceanAI[X1][Y1] = 'O', targetP1[X1][Y1] = 'O';
			rep_P1 = true;
			sink_AI++;
		}
	}

	if (choice == 2) {
		if (p1_p2 % 2 == 0) {
			if (oceanP2[X1][Y1] == char(0)) {
				oceanP2[X1][Y1] = 'X', targetP1[X1][Y1] = 'X';
				rep_P1 = false;
			}
			else {
				if (oceanP2[X1][Y1] == 'R') R_P2--;
				if (oceanP2[X1][Y1] == 'B') B_P2--;
				if (oceanP2[X1][Y1] == 'U') U_P2--;
				if (oceanP2[X1][Y1] == 'S') S_P2--;
				if (oceanP2[X1][Y1] == 'D') D_P2--;
				oceanP2[X1][Y1] = 'O', targetP1[X1][Y1] = 'O';
				rep_P1 = true;
				sink_P2++;
			}
		}
		if (p1_p2 % 2 == 1) {
			if (oceanP1[X1][Y1] == char(0)) {
				oceanP1[X1][Y1] = 'X', targetP2[X1][Y1] = 'X';
				rep_P2 = false;
			}
			else {
				if (oceanP1[X1][Y1] == 'R') R_P1--;
				if (oceanP1[X1][Y1] == 'B') B_P1--;
				if (oceanP1[X1][Y1] == 'U') U_P1--;
				if (oceanP1[X1][Y1] == 'S') S_P1--;
				if (oceanP1[X1][Y1] == 'D') D_P1--;
				oceanP1[X1][Y1] = 'O', targetP2[X1][Y1] = 'O';
				rep_P2 = true;
				sink_P1++;
			}
		}

	}
	if (((R_AI == 0 || B_AI == 0 || U_AI == 0 || S_AI == 0 || D_AI == 0) && choice == 1) ||
		((R_P1 == 0 || B_P1 == 0 || U_P1 == 0 || S_P1 == 0 || D_P1 == 0) && choice == 2) ||
		((R_P2 == 0 || B_P2 == 0 || U_P2 == 0 || S_P2 == 0 || D_P2 == 0) && choice == 2))
		close_around_sink();
}

void initlization() {
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			oceanP1[i][j] = char(0), oceanP2[i][j] = char(0), oceanAI[i][j] = char(0);
			targetP1[i][j] = char(0), targetP2[i][j] = char(0);
			sb_P1[i][j] = false, sb_P2[i][j] = false, sb_AI[i][j] = false;
			hb_P1[i][j] = false, hb_P2[i][j] = false, hb_AI[i][j] = false;
		}
	no_up = true, no_down = true, no_right = true, no_left = true;
	rep_P1 = false, rep_P2 = false, rep_AI = false;
	hit = false, right_direction = false;
	go_up = 1, go_down = 1, go_right = 1, go_left = 1;
	R_AI = 5, B_AI = 4, U_AI = 3, S_AI = 3, D_AI = 2;
	R_P1 = 5, B_P1 = 4, U_P1 = 3, S_P1 = 3, D_P1 = 2;
	R_P2 = 5, B_P2 = 4, U_P2 = 3, S_P2 = 3, D_P2 = 2;
	sink_P1 = 0, sink_P2 = 0, sink_AI = 0;
	choice = -1, p1_p2 = 0;
}

bool win() {
	if (sink_P1 == 17 || sink_P2 == 17 || sink_AI == 17) return 0;
	else return 1;
}

void after_winning() {
	SetConsoleTextAttribute(hConsole, 0x09);
	if (sink_P1 == 17) {
		if (choice == 1) cout << "computer Won \n";
		if (choice == 2) cout << "Player 2 Won \n";
	}
	if (sink_AI == 17) cout << "You Won \n";
	if (sink_P2 == 17) cout << "Player 1 Won \n";
	cout << "Do You Want To Play Again? \n\n";
	cout << "\t" << char(201); for (i = 0; i < 7; i++) cout << char(205);	cout << char(187) << endl;
	cout << "\t" << char(186) << " Y / N " << char(186) << endl;
	cout << "\t" << char(200); for (i = 0; i < 7; i++) cout << char(205);	cout << char(188) << endl;
	char new_game;
	cin >> new_game;
	while (new_game != 'Y' && new_game != 'y' && new_game != 'N' && new_game != 'n') {
		display_game();
		cout << "Invalid value..Please enter again \n";
		cin >> new_game;
	}
	if (new_game == 'N' || new_game == 'n') exit(0);
}

void view() {
	system("CLS");
	SetConsoleTextAttribute(hConsole, 0x09);
	cout << "1, 2, or 3";
	cout << "\n\n\n\n";
	cout << "\t\t\t" << char(201); for (i = 0; i < 11; i++) cout << char(205);	cout << char(187) << endl;
	cout << "\t\t\t" << char(186) << " 1 Player  " << char(186) << endl;
	cout << "\t\t\t" << char(200); for (i = 0; i < 11; i++) cout << char(205);	cout << char(188) << endl;
	cout << "\t\t\t" << char(201); for (i = 0; i < 11; i++) cout << char(205);	cout << char(187) << endl;
	cout << "\t\t\t" << char(186) << " 2 Players " << char(186) << endl;
	cout << "\t\t\t" << char(200); for (i = 0; i < 11; i++) cout << char(205);	cout << char(188) << endl;
	cout << "\t\t\t" << char(201); for (i = 0; i < 11; i++) cout << char(205);	cout << char(187) << endl;
	cout << "\t\t\t" << char(186) << "   Close   " << char(186) << endl;
	cout << "\t\t\t" << char(200); for (i = 0; i < 11; i++) cout << char(205);	cout << char(188) << endl;

	if (choice != 1 && choice != 2 && choice != 3 && choice != -1) cout << "Invalid value..Please enter again: \n";
	cin >> choice;
	if (cin.fail()) {
		choice = 0;
		cin.clear();
		cin >> c;
	}
	if (choice != 1 && choice != 2 && choice != 3) view();
	if (choice == 3) exit(0);
}

void close_around_sink() {
	if (choice == 1) {
		//AI vs P1
		if (R_P1 == 0) {
			R_P1 = -1;
			for (i = s_P1[0].x1 - 1; i <= s_P1[0].x2 + 1; i++)
				for (j = s_P1[0].y1 - 1; j <= s_P1[0].y2 + 1; j++)
					if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						oceanP1[i][j] = 'X', hb_AI[i][j] = true;
		}
		if (B_P1 == 0) {
			B_P1 = -1;
			for (i = s_P1[1].x1 - 1; i <= s_P1[1].x2 + 1; i++)
				for (j = s_P1[1].y1 - 1; j <= s_P1[1].y2 + 1; j++)
					if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						oceanP1[i][j] = 'X', hb_AI[i][j] = true;
		}
		if (U_P1 == 0) {
			U_P1 = -1;
			for (i = s_P1[2].x1 - 1; i <= s_P1[2].x2 + 1; i++)
				for (j = s_P1[2].y1 - 1; j <= s_P1[2].y2 + 1; j++)
					if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						oceanP1[i][j] = 'X', hb_AI[i][j] = true;
		}
		if (S_P1 == 0) {
			S_P1 = -1;
			for (i = s_P1[3].x1 - 1; i <= s_P1[3].x2 + 1; i++)
				for (j = s_P1[3].y1 - 1; j <= s_P1[3].y2 + 1; j++)
					if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						oceanP1[i][j] = 'X', hb_AI[i][j] = true;
		}
		if (D_P1 == 0) {
			D_P1 = -1;
			for (i = s_P1[4].x1 - 1; i <= s_P1[4].x2 + 1; i++)
				for (j = s_P1[4].y1 - 1; j <= s_P1[4].y2 + 1; j++)
					if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						oceanP1[i][j] = 'X', hb_AI[i][j] = true;
		}

		//P1 vs AI
		if (R_AI == 0) {
			R_AI = -1;
			for (i = s_AI[0].x1 - 1; i <= s_AI[0].x2 + 1; i++)
				for (j = s_AI[0].y1 - 1; j <= s_AI[0].y2 + 1; j++)
					if (oceanAI[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanAI[i][j] = 'X', hb_P1[i][j] = true;
		}
		if (B_AI == 0) {
			B_AI = -1;
			for (i = s_AI[1].x1 - 1; i <= s_AI[1].x2 + 1; i++)
				for (j = s_AI[1].y1 - 1; j <= s_AI[1].y2 + 1; j++)
					if (oceanAI[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanAI[i][j] = 'X', hb_P1[i][j] = true;
		}
		if (U_AI == 0) {
			U_AI = -1;
			for (i = s_AI[2].x1 - 1; i <= s_AI[2].x2 + 1; i++)
				for (j = s_AI[2].y1 - 1; j <= s_AI[2].y2 + 1; j++)
					if (oceanAI[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanAI[i][j] = 'X', hb_P1[i][j] = true;
		}
		if (S_AI == 0) {
			S_AI = -1;
			for (i = s_AI[3].x1 - 1; i <= s_AI[3].x2 + 1; i++)
				for (j = s_AI[3].y1 - 1; j <= s_AI[3].y2 + 1; j++)
					if (oceanAI[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanAI[i][j] = 'X', hb_P1[i][j] = true;
		}
		if (D_AI == 0) {
			D_AI = -1;
			for (i = s_AI[4].x1 - 1; i <= s_AI[4].x2 + 1; i++)
				for (j = s_AI[4].y1 - 1; j <= s_AI[4].y2 + 1; j++)
					if (oceanAI[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanAI[i][j] = 'X', hb_P1[i][j] = true;
		}
	}

	if (choice == 2) {
		//P1 vs P2
		if (R_P2 == 0) {
			R_P2 = -1;
			for (i = s_P2[0].x1 - 1; i <= s_P2[0].x2 + 1; i++)
				for (j = s_P2[0].y1 - 1; j <= s_P2[0].y2 + 1; j++)
					if (oceanP2[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanP2[i][j] = 'X', hb_P1[i][j] = true;
		}
		if (B_P2 == 0) {
			B_P2 = -1;
			for (i = s_P2[1].x1 - 1; i <= s_P2[1].x2 + 1; i++)
				for (j = s_P2[1].y1 - 1; j <= s_P2[1].y2 + 1; j++)
					if (oceanP2[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanP2[i][j] = 'X', hb_P1[i][j] = true;
		}
		if (U_P2 == 0) {
			U_P2 = -1;
			for (i = s_P2[2].x1 - 1; i <= s_P2[2].x2 + 1; i++)
				for (j = s_P2[2].y1 - 1; j <= s_P2[2].y2 + 1; j++)
					if (oceanP2[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanP2[i][j] = 'X', hb_P1[i][j] = true;
		}
		if (S_P2 == 0) {
			S_P2 = -1;
			for (i = s_P2[3].x1 - 1; i <= s_P2[3].x2 + 1; i++)
				for (int j = s_P2[3].y1 - 1; j <= s_P2[3].y2 + 1; j++)
					if (oceanP2[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanP2[i][j] = 'X', hb_P1[i][j] = true;
		}
		if (D_P2 == 0) {
			D_P2 = -1;
			for (i = s_P2[4].x1 - 1; i <= s_P2[4].x2 + 1; i++)
				for (j = s_P2[4].y1 - 1; j <= s_P2[4].y2 + 1; j++)
					if (oceanP2[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
						targetP1[i][j] = 'X', oceanP2[i][j] = 'X', hb_P1[i][j] = true;
		}
	}
	//P2 vs P1
	if (R_P1 == 0) {
		R_P1 = -1;
		for (i = s_P1[0].x1 - 1; i <= s_P1[0].x2 + 1; i++)
			for (j = s_P1[0].y1 - 1; j <= s_P1[0].y2 + 1; j++)
				if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
					targetP2[i][j] = 'X', oceanP1[i][j] = 'X', hb_P2[i][j] = true;
	}
	if (B_P1 == 0) {
		B_P1 = -1;
		for (i = s_P1[1].x1 - 1; i <= s_P1[1].x2 + 1; i++)
			for (j = s_P1[1].y1 - 1; j <= s_P1[1].y2 + 1; j++)
				if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
					targetP2[i][j] = 'X', oceanP1[i][j] = 'X', hb_P2[i][j] = true;
	}
	if (U_P1 == 0) {
		U_P1 = -1;
		for (i = s_P1[2].x1 - 1; i <= s_P1[2].x2 + 1; i++)
			for (j = s_P1[2].y1 - 1; j <= s_P1[2].y2 + 1; j++)
				if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
					targetP2[i][j] = 'X', oceanP1[i][j] = 'X', hb_P2[i][j] = true;
	}
	if (S_P1 == 0) {
		S_P1 = -1;
		for (i = s_P1[3].x1 - 1; i <= s_P1[3].x2 + 1; i++)
			for (j = s_P1[3].y1 - 1; j <= s_P1[3].y2 + 1; j++)
				if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
					targetP2[i][j] = 'X', oceanP1[i][j] = 'X', hb_P2[i][j] = true;
	}
	if (D_P1 == 0) {
		D_P1 = -1;
		for (i = s_P1[4].x1 - 1; i <= s_P1[4].x2 + 1; i++)
			for (j = s_P1[4].y1 - 1; j <= s_P1[4].y2 + 1; j++)
				if (oceanP1[i][j] == char(0) && i >= 0 && j >= 0 && i < n && j < n)
					targetP2[i][j] = 'X', oceanP1[i][j] = 'X', hb_P2[i][j] = true;
	}
}

void color_exchange() {
	if (choice == 2) {
		if (p1_p2 % 2 == 0 && k == 0) SetConsoleTextAttribute(hConsole, 0x08);
		if (p1_p2 % 2 == 0 && k == 1) SetConsoleTextAttribute(hConsole, 0x07);
		if (p1_p2 % 2 == 1 && k == 0) SetConsoleTextAttribute(hConsole, 0x07);
		if (p1_p2 % 2 == 1 && k == 1) SetConsoleTextAttribute(hConsole, 0x08);
	}
}
