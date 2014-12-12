#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <stack>


/*
  the program uses recursion way to perform bfs (breadth first search)
  in order to search the way from start point to end point.

*/

using namespace std;

const unsigned int maze_vertical_size = 5;
const unsigned int maze_horizen_size = 5;

typedef vector< vector<int> >  maze_t;

typedef std::pair<int,int>   point_t;

typedef list<point_t>       maze_path;

typedef list<point_t>       point_list;

enum color_e {
    white = 0, // the point has never been traversed.
    grey = 1,  // the point is traversed.
};
    
typedef struct _tagnodeAccessInfo {
    color_e       c;
}nodeAccessInfo_t;


typedef vector< vector<nodeAccessInfo_t> > traversed_map;

traversed_map traverseMap(maze_vertical_size, vector<nodeAccessInfo_t>(maze_horizen_size));

bool bfs(const maze_t &maze, const point_t &cPoint, const point_t &ePoint, maze_path &path, bool startPoint);

int main(int argc, char *argv[])
{
    std::cout << "hello world" << std::endl;
    maze_t maze(maze_vertical_size,vector<int>(maze_horizen_size));
    
    for(int i = 0;i<maze_vertical_size;i++)
    {
        for(int j=0;j<maze_horizen_size;j++)
        {
            maze[i][j] = 0;
            traverseMap[i][j].c = white;
        }
    }

    // set wall to maze.
    maze[0][1] = 1;
    maze[1][1] = 1;
    maze[1][3] = 1;
    maze[3][1] = 1;
    maze[3][2] = 1;
    maze[3][3] = 1;

    //uncomment the following code to build a inaccessable maze. 
    //maze[3][4] = 1;
    //maze[4][3] = 1;
    
    point_t sPoint(0,0);
    point_t ePoint(maze_vertical_size-1,maze_horizen_size-1);
   
    maze_path path;
    bool foundPath = bfs(maze,sPoint,ePoint,path,true);

    printf ("result:%d\n", foundPath);
    if (foundPath) {
        maze_path::reverse_iterator iter = path.rbegin();
        for(;iter != path.rend();++iter) {
            cout << "-->" << "point(" << iter->first << "," << iter->second << ")" << std::endl;
        }

    }
    return 0;
}

// this funciont doesn't consider the previous step, so the caller will remove the duplicated one from return value.
point_list bfs_available_next_points(const point_t &cPoint, const maze_t &maze)
{
    point_list points;

    static const point_t offset[4] = {point_t(0,1), point_t(1,0), point_t(0,-1), point_t(-1,0)};

    for (int i = 0;i < 4; ++i) {
        point_t tmpPoint = point_t(cPoint.first +offset[i].first, cPoint.second +offset[i].second);

        if (tmpPoint.first < 0 || tmpPoint.second < 0) {
            continue;
        }

        if (tmpPoint.first >= maze_horizen_size) {
            continue;
        }

        if (tmpPoint.second >= maze_vertical_size) {
            continue;
        }

        if (maze[tmpPoint.first][tmpPoint.second] == 0) {
            points.push_back(tmpPoint);
        }
    }

    return points;
}


bool bfs(const maze_t &maze, const point_t &cPoint, const point_t &ePoint, maze_path &path, bool startPoint = false)
{
    // set the start point as traversed.
    if (true == startPoint) {
        traverseMap[cPoint.first][cPoint.second].c = grey;
    }

    point_list around_points = bfs_available_next_points(cPoint, maze);

    queue<point_t> pendingPoints;

    point_list::iterator iter = around_points.begin();
    for(;iter != around_points.end();++iter) {
        //printf ("around point(%d,%d)\n", iter->first, iter->second);

        if (*iter == ePoint) {
            //printf ("we found the path to end point(%d,%d)\n", ePoint.first, ePoint.second);
            path.push_back(cPoint);
            return true;
        }

        // color the un-traverse nodes.
        if (traverseMap[iter->first][iter->second].c == white) {
            traverseMap[iter->first][iter->second].c = grey;
            printf ("point(%d,%d) parent is point(%d,%d)\n", 
                    iter->first, iter->second,
                    cPoint.first, cPoint.second);
            pendingPoints.push(*iter);
        }
    }

    while(!pendingPoints.empty()) {
        point_t pt = pendingPoints.front();
        pendingPoints.pop();
        //printf ("queue pulling point(%d,%d)\n", pt.first, pt.second);
        if (bfs(maze, pt, ePoint, path)) {
            path.push_back(cPoint);
            return true;
        }
    }
  
    return false;
}

