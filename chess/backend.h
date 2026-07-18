#ifndef BACKEND_H
#define BACKEND_H
#include <string>
#include <sstream>
#include <iostream>

typedef int chesscolour;
const chesscolour RED=0;
const chesscolour BLUE=1;
const chesscolour GREY=2;

class chessboard {
private:
	struct piece {
		chesscolour colour; //红/黑
		int suit;  //花色
		int number; //数字, J=11, Q=12, K=13, 0=无牌
		/*
		colour=RED, suit = 1,  number = X: 红桃X
		colour=RED, suit=0, number = X: 方片X
		colour=BLUE, suit = 1,  number = X: 黑桃X
		colour=BLUE, suit=0, number = X: 草花X
		*/
	};
	piece board[10][10];
	int sum[2];
	chesscolour turn;

public:	
	int king_sum(chesscolour colour);
	std::string get_text(int x, int y);
	chesscolour get_colour(int pos_x, int pos_y);
	bool check(int x1, int y1, int x2, int y2);
	void move(int x1, int y1, int x2, int y2);
	chessboard(): turn(0), sum{2,2},
		board{
		{ {0,0,0}, {0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0} },
		{ {0,0,0}, {0,1,11}, {0,1,12}, {0,1,13}, {0,0,13}, {0,0,12}, {0,0,11} },
		{ {0,0,0}, {0,1,10}, { 0,1,9}, { 0,1,8}, { 0,0,8}, { 0,0,9}, { 0,0,10}},
		{ {0,0,0}, {0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0} },
		{ {0,0,0}, {0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0}, { 0,0,0} },
		{ {0,0,0}, {1,0,10}, { 1,0,9}, { 1,0,8}, { 1,1,8}, { 1,1,9}, {1,1,10} },
		{ {0,0,0}, {1,0,11}, { 1,0,12}, { 1,0,13}, { 1,1,13}, { 1,1,12}, {1,1,11} }
	} {}
};
#endif // !BACKEND_H