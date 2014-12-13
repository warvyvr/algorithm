#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <stack>


/*
  the program uses non-recursion way to perform bfs (breadth first search)
  in order to search the way from start point to end point.
  TODO:remove parent from nodeAccessInfo structure,
  the parent information build in stack in manner of list/map will save memory.

*/

using namespace std;

const unsigned int maze_vertical_size = 2005;
const unsigned int maze_horizen_size = 2005;

typedef vector< vector<int> >  maze_t;

typedef std::pair<int,int>   point_t;

typedef list<point_t>       maze_path;

typedef list<point_t>       point_list;

typedef vector<point_t>     parent_list;

enum color_e {
    white = 0, // the point has never been traversed.
    grey = 1,  // the point is traversed.
};
    
typedef struct _tagnodeAccessInfo {
    color_e       c;
    point_t       parent;
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
        cout << "total step(s):" << path.size() << std::endl;
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
    parent_list plist;
    queue<point_t> pendingPoints;
    point_t pt;
    point_list around_points;

    // set the start point as traversed.
    if (true == startPoint) {
        traverseMap[cPoint.first][cPoint.second].c = grey;
        traverseMap[cPoint.first][cPoint.second].parent = point_t(-1,-1);
    }

    plist.push_back (cPoint);
    pendingPoints.push(cPoint);

    while (!pendingPoints.empty())
    {
        pt = pendingPoints.front();
        pendingPoints.pop();
    
        around_points = bfs_available_next_points(pt, maze);
        point_list::iterator iter = around_points.begin();
    
        for(;iter != around_points.end();++iter) {

            if (*iter == ePoint) {
                // when found the ePoint, build the path information.
                traverseMap[iter->first][iter->second].c = grey;
                traverseMap[iter->first][iter->second].parent = pt;
                point_t &parent = traverseMap[iter->first][iter->second].parent;
                while (parent != point_t(-1,-1)) {
                    path.push_back(parent);
                    parent = traverseMap[parent.first][parent.second].parent;
                }

                return true;
            }

            // color the un-traverse nodes.
            if (traverseMap[iter->first][iter->second].c == white) {
                traverseMap[iter->first][iter->second].c = grey;
                traverseMap[iter->first][iter->second].parent = pt;
                //printf ("point(%d,%d) parent is point(%d,%d)\n", 
                //        iter->first, iter->second,
                //        pt.first, pt.second);
                pendingPoints.push(*iter);
            }
        }
    }

    return false;
}

