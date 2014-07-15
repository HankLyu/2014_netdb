#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;

struct Point{
	bool removable;
	double x,y;
};
struct Line{
	int id;
	vector<Point>lipoint;
};
struct grid_node{	//record this grid has which line and point
	map<int,int>line_id;	//line -> record this line id
	vector<Point> grid_point;	//point ->record this obstacle's x and y
};

int main(int argv,char *argc[]){
    int PointToRemove = atoi(argc[1]);
    FILE *LineInput,*PointInput,*LineOut;
    char input[10000];
    map<double, map<double, grid_node> > grid;	//record each has how many line and point
    vector<Point> obstacle_point;	//record Point from pointinput
    vector<Line> line,ans;
    Point ptmp;
    Line ltmp;
    LineInput=fopen(argc[2],"r");
    PointInput=fopen(argc[3],"r");
	LineOut=fopen(argc[4],"w");
	/*read data from file*/
	while(fscanf(LineInput,"%s",input)!=EOF){	//read line
		int id=atoi(input);
		while(fscanf(LineInput,"%s",input)!=EOF){	//ignore gml format and its text
			if(input[0]=='"' && input[1]=='>'){
				break;
			}
		}
		ltmp.id=id;
		ltmp.lipoint.clear();
		ptmp.removable=0;	//each line of first point
		ptmp.x=atof(input+2);	//read x
		for(int i=2;input[i]!='\0';i++)	//read y
			if(input[i]==','){
				ptmp.y=atof(input+i+1);
				break;
			}
		grid[(int)(ptmp.x/100)*100][(int)(ptmp.y/100)*100].line_id[id]=1;
		ltmp.lipoint.push_back(ptmp);
		ptmp.removable=1;

		while(fscanf(LineInput,"%s",input)!=EOF){	//other point in this line
			if(input[0]=='<')	break;	//end of the point
			ptmp.x=atof(input);
			for(int i=0;input[i]!='\0';i++)
				if(input[i]==','){
					ptmp.y=atof(input+i+1);
					break;
				}
			grid[(int)(ptmp.x/100)*100][(int)(ptmp.y/100)*100].line_id[id]=1;
			ltmp.lipoint.push_back(ptmp);
		}
		ltmp.lipoint[ltmp.lipoint.size()-1].removable=0;
		line.push_back(ltmp);	//put line to vector
	}
	while(fscanf(PointInput,"%s",input)!=EOF){	//read PointInput
		while(fscanf(LineInput,"%s",input)!=EOF){	//ignore gml format and its text
			if(input[0]=='"' && input[1]=='>'){
				break;
			}
		}
		ptmp.x=atof(input+2);	//read x
		for(int i=2;input[i]!='\0';i++)	//read y
			if(input[i]==','){
				ptmp.y=atof(input+i+1);
				break;
			}
		grid[(int)(ptmp.x/100)*100][(int)(ptmp.y/100)*100].grid_point.push_back(ptmp);
	}
    /*delete the point*/
    int line_size=line.size(),line_point_size;
    for(int i=0;i<line_size;i++){
    	int max_x,min_x,max_y,min_y,tmpx,tmpy;
    	line_point_size=line[i].lipoint.size();
    	max_x=min_x=(int)(line[i].lipoint[0].x/100)*100;
    	max_y=min_y=(int)(line[i].lipoint[0].y/100)*100;
    	int no_other=1;
    	for(int j=0;i<line_point_size;j++){
			tmpx=(int)(line[i].lipoint[j].x/100)*100;
			tmpy=(int)(line[i].lipoint[j].y/100)*100;
			if(tmpx>max_x){
				grid_node gtmp;
				for(int k=min_y;k<=max_y;k++){
					if(grid[tmpx][k].grid_point.size()>0 ||
						(grid[tmpx][k].line_id.size()>0 && grid[tmpx][k].line_id[line[j].id]==0) ){
						no_other=0;
					}
					if(no_other)	max_x=tmpx;
				}
			}else if(tmpx<min_x){
			}
			if(tmpy>max_y){
			}else if(tmpy<min_y){
			}
    	}
    }
    fclose(LineInput);
    fclose(PointInput);
    fclose(LineOut);
    return 0;
}
