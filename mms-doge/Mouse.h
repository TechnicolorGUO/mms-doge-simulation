#ifndef MOUSE_H
#define MOUSE_H

#include "Maze.h"
#include "API.h"
class Mouse {

public:
	int x;  // X-coordinate (0 to 15)
	int y;  // Y-coordinate (0 to 15)
    int heading;


    // Member functions
    void moveForward(){
		switch(heading)
		{
			case 0:
			y++;
			break;
			case 1:
			x++;
			break;
			case 2:
			y--;
			break;
			case 3:
			x--;
			break;
		}
		API::moveForward(1);
		API::setColor(x, y, 'G');
    	API::setText(x, y, "vi");
	};
	bool isWallL() {
		return API::wallLeft();
	};
    bool isWallR() {
		return API::wallRight();

	};
    bool isWallF() {
		return API::wallFront();

	};
	void turn(int n){
		switch (n)
		{
		case 0:
			break;
		case 1:
		heading +=1;
		heading %=4;
		API::turnRight();
			break;
		case 2:
		heading +=2;
		heading %=4;
		API::turnRight();
		API::turnRight();
			break;
		case -1:
		heading +=3;
		heading%=4;
		API::turnLeft();
			break;
		default:
			break;
		}
	};
	void turnToward(int tar_x, int tar_y) {
		int n, target_heading;
		if (x == tar_x) {
			if(tar_y == y+1) target_heading = 0;
			else target_heading = 2;
			}

		else if (y == tar_y) {
			if(tar_x == x+1) target_heading = 1;
			else target_heading = 3;
			}

		n = target_heading - heading;
		switch (n)
		{
		case 0:
			break;
		case 1:
			turn(1);
			break;
		case 2:
			turn(2);
			break;
		case 3:
			turn(-1);
			break;
		case -1:
			turn(-1);
			break;
		case -2:
			turn(2);
			break;
		case -3:
			turn(1);
			break;
		default:
			break;
		}
		heading = target_heading;
	}

 	void updateWall(Maze& maz)
	{
		maz.setWall(x, y, (isWallF()?heading:4));
        maz.setWall(x, y, (isWallL()?((heading+3)%4):4));
        maz.setWall(x, y, (isWallR()?((heading+1)%4):4));
	}  
	Mouse(){
		heading=0;
		x =0;
		y=0;
	};  // Constructor 
};

#endif // MOUSE_H