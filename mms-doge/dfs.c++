#include "Maze.h" 
#include "Mouse.h"
#include <iostream>
#include <string>
#include <stack>
#include "API.h"
using namespace std;
Mouse mouse;
Maze maz;
stack<int*> locStack;
stack<int> dirStack;
string lo,lo1, mouse_x, mouse_y;

void view_stack(stack<int*> s){

}

void log(const std::string& text) {
    std::cerr << text << std::endl;
}

    void dfsMapMaze() {
        
        int curLoc[] = {mouse.x, mouse.y};
        mouse_x = to_string(mouse.x);
        mouse_y = to_string(mouse.y);

        int* nextLoc;
        log("\ndfs setup fin");
        if (!maz.isVisited.get(curLoc[0],curLoc[1])) {

            log("curr not vis");
            maz.isVisited.set(curLoc[0],curLoc[1]);
            mouse.updateWall(maz);
            lo = to_string(maz.wall0.get(curLoc[0],curLoc[1]));
            lo1 = to_string(maz.wall1.get(curLoc[0],curLoc[1]));
            log(mouse_x);
            log(mouse_y);
            log(lo);
            log(lo1);
            log(to_string(API::wallFront()));

            


            if ((curLoc[1]!=15)&&(!maz.wall0.get(curLoc[0],curLoc[1]))&& (!maz.isVisited.get(curLoc[0],curLoc[1]+1))) {
                locStack.push(new int[2]{curLoc[0],curLoc[1] + 1});
                log("aaa");
            }

            if ((curLoc[0]!=15) && (!maz.wall1.get(curLoc[0],curLoc[1]))&& (!maz.isVisited.get(curLoc[0]+1,curLoc[1]))) {
                locStack.push(new int[2]{curLoc[0]+1,curLoc[1]});
                log("bbb");
            }

            if ((curLoc[1])&&(!maz.wall2.get(curLoc[0],curLoc[1]))&& (!maz.isVisited.get(curLoc[0],curLoc[1]-1))) {
                locStack.push(new int[2]{curLoc[0],curLoc[1] - 1});
                log("ccc");
            }

            if ((curLoc[0])&&(!maz.wall3.get(curLoc[0],curLoc[1]))&& (!maz.isVisited.get(curLoc[0]-1,curLoc[1]))) {
                locStack.push(new int[2]{curLoc[0]-1,curLoc[1]});
                log("ddd");
            }

        }

        while (true) {
            log("0");
            if (locStack.empty()) {
                log("empty");
                if (!(curLoc[0] == 0 && curLoc[1] == 0)) {
                    log("not start pos");
                    switch (((dirStack.top() + 2) % 4)-mouse.heading )
		{
		case 0:
			break;
		case 1:
			 mouse.turn(1);
			break;
		case 2:
			 mouse.turn(2);
			break;
		case 3:
			 mouse.turn(-1);
			break;
		case -1:
			 mouse.turn(-1);
			break;
		case -2:
			 mouse.turn(2);
			break;
		case -3:
			 mouse.turn(1);
			break;
		default:
			break;
		}

         dirStack.pop();
                    mouse.moveForward();
                    dfsMapMaze();
                }
                return;
            }
            nextLoc = locStack.top();locStack.pop();
            log(to_string(nextLoc[0]));
            log(to_string(nextLoc[1]));
            if (!maz.isVisited.get(nextLoc[0],nextLoc[1])) {
                log("not visit");
                break;
            }
        }

        if (maz.canMove(curLoc[0], curLoc[1], nextLoc[0], nextLoc[1])) {
            log("movable");
            mouse.turnToward(nextLoc[0], nextLoc[1]);
            dirStack.push(mouse.heading);
            mouse.moveForward();
        } else {
            log("not movable");
            locStack.push(new int[2] {nextLoc[0],nextLoc[1]});
           
            
            switch (((dirStack.top() + 2) % 4)-mouse.heading )
		{
		case 0:
			break;
		case 1:
			 mouse.turn(1);
			break;
		case 2:
			 mouse.turn(2);
			break;
		case 3:
			 mouse.turn(-1);
			break;
		case -1:
			 mouse.turn(-1);
			break;
		case -2:
			 mouse.turn(2);
			break;
		case -3:
			 mouse.turn(1);
			break;
		default:
			break;
		}

         dirStack.pop();
            log(to_string(mouse.heading));
            mouse.moveForward();
        }
        log("next loop");
        dfsMapMaze();
    }


int main(int argc, char* argv[]) 
{   log("Running...");
    API::setColor(0, 0, 'G');
    API::setText(0, 0, "abc");
    dfsMapMaze();
}