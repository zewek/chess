#include "backend.h"
#include <cmath>

inline static bool same(int num1, int num2) { //比较num1和num2的权重是否相同
	if (num1==num2) return true;
	if (num1>10 and num2>10) return true;
	return false;
}

chesscolour chessboard::get_colour(int pos_x, int pos_y) {
	if (board[pos_x][pos_y].number==0) return GREY;
	else return board[pos_x][pos_y].colour;
}

std::string chessboard::get_text(int x, int y) {
	if (board[x][y].number==0) return " "; //特殊考虑空格
	std::stringstream ans;
	std::string number;
	if (board[x][y].number<=10) number=std::to_string(board[x][y].number);
	else if (board[x][y].number==11) number="J";
	else if (board[x][y].number==12) number="Q";
	else if (board[x][y].number==13) number="K";
	else throw std::runtime_error("get_type:Invalid number");

	if (board[x][y].colour==RED) {
		if (board[x][y].suit==1) ans<<"♥\n "<<number;
		else ans<<"♦\n "<<number;
	} else if (board[x][y].colour==BLUE) {
		if (board[x][y].suit==1) ans<<"♠\n "<<number;
		else ans<<"♣\n "<<number;
	} else throw std::runtime_error("get_type:Invalid colour");
	return ans.str();
}

bool chessboard::check(int x1, int y1, int x2, int y2) {
	int num1=board[x1][y1].number;
	int num2=board[x2][y2].number;
	//空牌
	if (num1==0) return false; 
	//没轮到自己
	if (board[x1][y1].colour!=turn) return false;
	//自己不能吃自己
	if (board[x1][y1].colour==board[x2][y2].colour and board[x2][y2].number!=0) return false; 
	//两牌权重相同, 且不可吃
	if (same(num1, num2) and board[x1][y1].suit<board[x2][y2].suit) return false;
	
	if (board[x1][y1].number<=10) { //数字牌
		if (!((x1==x2 and std::abs(y1-y2)==1) or (y1==y2 and std::abs(x1-x2)==1))) return false;
		if (num1<num2) //已考虑num2=0
			if (num1!=1 or (num1==1 and num2<=10)) return false;
		return true;
	} else if (num1==11) { //J
		if (num2==1) return false;
		if ((x1==x2 and std::abs(y1-y2)==1) or (y1==y2 and std::abs(x1-x2)==1)) return true;
		else if (x1==x2 and y1-y2==2 and board[x1][y2+1].number!=0 and board[x2][y2].number!=0) return true;
		else if (x1==x2 and y2-y1==2 and board[x1][y1+1].number!=0 and board[x2][y2].number!=0) return true;
		else if (x1-x2==2 and y1==y2 and board[x2+1][y1].number!=0 and board[x2][y2].number!=0) return true;
		else if (x2-x1==2 and y1==y2 and board[x1+1][y1].number!=0 and board[x2][y2].number!=0) return true;
		else return false;
	} else if (num1==12) { //Q
		if (std::abs(x1-x2)==1 and std::abs(y1-y2)==1 and num2!=1) return true;
		else return false;
	} else if (num1==13) { //K
		if (!((x1==x2 and std::abs(y1-y2)==1) or (y1==y2 and std::abs(x1-x2)==1))) return false;
		if (num2==1) return false;
		return true;
	} else throw("check:Invalid number");
}

void chessboard::move(int x1, int y1, int x2, int y2) {
	turn=!turn;
	if (same(board[x1][y1].number, board[x2][y2].number)) {
		board[x1][y1].number=0;
		board[x2][y2].number=0;
		if (board[x2][y2].number==13) sum[board[x2][y2].colour]--;
	} else if (board[x1][y1].number==1 or board[x1][y1].number>10) {
		if (board[x2][y2].number==13) sum[board[x2][y2].colour]--;
		board[x2][y2].colour=board[x1][y1].colour;
		board[x2][y2].number=board[x1][y1].number;
		board[x2][y2].suit=board[x1][y1].suit;
		board[x1][y1].number=0;
		
	} else if (board[x1][y1].number<=10) {
		board[x2][y2].colour=board[x1][y1].colour;
		board[x2][y2].number=board[x1][y1].number-board[x2][y2].number;
		board[x2][y2].suit=board[x1][y1].suit;
		board[x1][y1].number=0;
	} else throw std::runtime_error("move:unable to move the chess piece");
}

int chessboard::king_sum(chesscolour colour) {
	return sum[colour];
}