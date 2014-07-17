#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <string>
#define MAXINPUT 10000
#define GRIDSIZE 100

using namespace std;

struct Point{
	bool removable;
	double x,y;
};
struct Line{
	int id;
	vector<Point> lipoint;
};
struct grid_node{				//record the lines and points in this grid
	map<int,int> line_id;		//line -> record this line id
	vector<Point> grid_point;	//point ->record this obstacle's x and y
};

// Argument: 1.pointToRemove 2.LineInput gml file 3.PointInput gml file 4.LineOutput gml file
int main(int argv,char *argc[]){
	
	int PointToRemove = atoi(argc[1]); // the point need to removed for porgram stop
    FILE *LineInput, *PointInput, *LineOut;
    char input[MAXINPUT];
    map<double, map<double, grid_node> > grid;	//record each has how many line and point
    vector<Point> obstacle_point;	//record Point from pointinput
    vector<Line> line,ans;
    Point ptmp;
    Line ltmp;
   
    LineInput=fopen(argc[2],"r");
    PointInput=fopen(argc[3],"r");
	LineOut=fopen(argc[4],"w");
	/*	read data from file
		because the gml format is fixed, do not care what the text write
		for each point, let grid to record this grid has which point and line
	*/

	while(fscanf(LineInput, "%s", input) != EOF){	//read line

		int id = atoi(input);	//get the id	
		while(fscanf(LineInput, "%s", input) != EOF){	//ignore gml format and its text
			if(input[0] == '"' && input[1] == '>'){
				break;
			}
		}

		ltmp.id = id;
		ltmp.lipoint.clear();
		ptmp.removable = 0;	//each line of first point
		ptmp.x = atof(input + 2);	//read x
		for(int i=2; input[i] != '\0'; i++){	//read y
			if(input[i]==','){
				ptmp.y=atof(input+i+1);
				break;
			}
		}

		grid[ (int)(ptmp.x / GRIDSIZE ) * GRIDSIZE ][ (int)(ptmp.y / GRIDSIZE ) * GRIDSIZE ].line_id[id] = 1;
		ltmp.lipoint.push_back(ptmp);
		ptmp.removable = 1;

		while(fscanf(LineInput, "%s", input) != EOF){	//other point in this line
		
			if(input[0] == '<'){	
					break;		//end of the point
			}
		
			ptmp.x = atof(input);
			for(int i=0; input[i] != '\0'; i++){
				if(input[i] == ','){
					ptmp.y = atof(input+i+1);
					break;
				}
			}
			grid[(int)(ptmp.x / GRIDSIZE ) * GRIDSIZE ][(int)(ptmp.y / GRIDSIZE ) * GRIDSIZE ].line_id[id] = 1;
			ltmp.lipoint.push_back(ptmp);
		}

		ltmp.lipoint[ltmp.lipoint.size() - 1].removable = 0;
		line.push_back(ltmp);	//put line to vector
	}
	while(fscanf(PointInput, "%s", input) != EOF){	//read PointInput
		while(fscanf(LineInput,"%s",input) != EOF){	//ignore gml format and its text
			if(input[0] == '"' && input[1] == '>'){
				break;
			}
		}

		ptmp.x = atof(input+2);	//read x
		for(int i=2; input[i] != '\0'; i++)	//read y
			if(input[i] == ','){
				ptmp.y = atof(input+i+1);
				break;
			}

		//add this obstacle to this grid
		grid[ (int)(ptmp.x / GRIDSIZE ) * GRIDSIZE ][ (int)(ptmp.y / GRIDSIZE ) * GRIDSIZE ].grid_point.push_back(ptmp);
		while(fscanf(LineInput,"%s",input) != EOF){	//read until to the gml end
			if(input[0] == '<'){	
					break;
			}
		}
	}

    /*delete the point*/
    int line_size=line.size(), line_point_size;
    
    for(int i=0; i < line_size; i++){
    	int max_x, min_x, max_y, min_y, tmpx, tmpy;

    	line_point_size=line[i].lipoint.size();
    	max_x = min_x = (int)(line[i].lipoint[0].x / GRIDSIZE) * GRIDSIZE;	//calculate which is start grid
    	max_y = min_y = (int)(line[i].lipoint[0].y / GRIDSIZE) * GRIDSIZE;
    	
    	int no_other = 1;
    	
    	for(int j = 0; i < line_point_size; j++){
			tmpx= (int)(line[i].lipoint[j].x / GRIDSIZE) * GRIDSIZE;	//get the current point is in which grid
			tmpy= (int)(line[i].lipoint[j].y / GRIDSIZE) * GRIDSIZE;
			
			if(tmpx > max_x){	//check the new area whether has other object that may cause line and line cross
				grid_node gtmp;
				for(int k = min_y; k <= max_y; k++){
					if(grid[tmpx][k].grid_point.size() > 0 ||
						( grid[tmpx][k].line_id.size() > 0 && grid[tmpx][k].line_id[line[j].id] == 0) ){
						no_other = 0;
					}
					if(no_other){
							max_x=tmpx;
					}
				}
			}else if(tmpx < min_x){
			}
			if(tmpy > max_y){
			}else if(tmpy < min_y){
			}
    	}
    }
    
    fclose(LineInput);
    fclose(PointInput);
    fclose(LineOut);
    return 0;
}
