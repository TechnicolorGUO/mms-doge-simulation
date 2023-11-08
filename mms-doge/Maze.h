#ifndef Maze_h
#define Maze_h
#define MAX_LEN 16

#include <string>
#include "Dir.h"
#include "BitVector256.h"

class Maze {
public:
	BitVector256 wall0;  // 上墙
	BitVector256 wall1;  //右墙
	BitVector256 wall2;  //下墙
	BitVector256 wall3;  //左墙
	BitVector256 isVisited;

	void setWall(unsigned x, unsigned y, int8_t dir)
	{
	switch (dir) {
	case 0:
		wall0.set(x, y);
		break;
	case 1:
		wall1.set(x, y);
		break;
	case 2:
		wall2.set(x, y);
		break;
	case 3:
		wall3.set(x, y);
		break;}
	};
	bool canMove(int cur0,int cur1,int tar0, int tar1)
	{
		return 
		(cur0 == tar0 && tar1 - cur1== +1 &&  !wall0.get(cur0, cur1))
		||
		(cur0 == tar0 && tar1 - cur1== -1 &&  !wall2.get(cur0, cur1))
		||
		(cur1 == tar1 && tar0 - cur0== +1 &&  !wall1.get(cur0, cur1))
		||
		(cur1 == tar1 && tar0 - cur0== -1 &&  !wall3.get(cur0, cur1));
	}

Maze(){
    wall0.clearAll();
    wall1.clearAll();
    wall2.clearAll();
    wall3.clearAll();
	isVisited.clearAll();
    // for (int i = 0; i < MAX_LEN; i++)
    // {
    //     wall0.set(15, i);
    //     wall1.set(i, 15);
    //     wall2.set(0, i);
    //     wall3.set(i, 0);
    // }
}

};

#endif