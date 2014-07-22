/*
程式架構：
1.read line
2.read obstacle point
3.刪點
	3.1取這條線的第一個點
	3.2確認此格是否有其他object
	3.3判斷線的下一點是否有超過目前區域的範圍
	3.4若超過，則對於新增的區域看有否其他object,若有其他object 則跳3.6
		//超過y 則判斷 (min_x,new_y)~(max_x,new_y)
	3.5若沒有超過，或是新增區域沒有其他object 則跳回3.3
	3.6刪點
	3.7換下一條線回3.1
若要執行程式，須設定argv
./main 100 lines_out.txt points_out.txt out.txt
//刪除點下限，線，障礙點，輸出檔
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

#define grid_size 1000

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

int main(int argc,char *argv[]){
    int PointToRemove = atoi(argv[1]);
    FILE *LineInput,*PointInput,*LineOut;
    char input[10000];
    map<double, map<double, grid_node> > grid;	//record each has how many line and point
    vector<Point> obstacle_point;	//record Point from pointinput
    vector<Line> line,ans;
    Point ptmp;
    Line ltmp;
    LineInput=fopen(argv[2],"r");
    PointInput=fopen(argv[3],"r");
	LineOut=fopen(argv[4],"w");
	/*
	read data from file
	because the gml format is fixed, do not care what the text write
	for each point, let grid to record this grid has which point and line
	*/
	while(fscanf(LineInput,"%s",input)!=EOF){	//read line
		int id=atoi(input);	//get the id
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
		int tmpx,tmpy;
		tmpx=((int)(ptmp.x/grid_size))*grid_size;	//calculate this point should be put
		tmpy=((int)(ptmp.y/grid_size))*grid_size;	//into which grid
		if(ptmp.x<0)	tmpx-=grid_size;			//ex(1003.123,231.11)->(1000,0)
		if(ptmp.y<0)	tmpy-=grid_size;			//ex(0.312,-12.11)->(0,-1000)
		grid[tmpx][tmpy].line_id[id]=1;
		ltmp.lipoint.push_back(ptmp);
		ptmp.removable=1;

		while(fscanf(LineInput,"%s",input)!=EOF){	//other point in this line
			if(input[0]=='<')	break;		//end of the point
			ptmp.x=atof(input);
			for(int i=0;input[i]!='\0';i++)
				if(input[i]==','){
					ptmp.y=atof(input+i+1);
					break;
				}
			tmpx=((int)(ptmp.x/grid_size))*grid_size;	//calculate this point should be put
			tmpy=((int)(ptmp.y/grid_size))*grid_size;	//into which grid
			if(ptmp.x<0)	tmpx-=grid_size;
			if(ptmp.y<0)	tmpy-=grid_size;
			grid[tmpx][tmpy].line_id[id]=1;
			ltmp.lipoint.push_back(ptmp);
		}
		ltmp.lipoint[ltmp.lipoint.size()-1].removable=0;
		line.push_back(ltmp);	//put line to vector
	}
	while(fscanf(PointInput,"%s",input)!=EOF){	//read PointInput
		while(fscanf(PointInput,"%s",input)!=EOF){	//ignore gml format and its text
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
		//add this obstacle to this grid
		int tmpx,tmpy;
		tmpx=((int)(ptmp.x/grid_size))*grid_size;	//calculate this point should be put
		tmpy=((int)(ptmp.y/grid_size))*grid_size;	//into which grid
		if(ptmp.x<0)	tmpx-=grid_size;
		if(ptmp.y<0)	tmpy-=grid_size;
		grid[tmpx][tmpy].grid_point.push_back(ptmp);
		while(fscanf(PointInput,"%s",input)!=EOF){	//read until to the gml end
			if(input[0]=='<')	break;
		}
	}
    /*delete the point*/
    int line_size=line.size(),line_point_size;
    int remove_num=0;
    for(int i=0;i<line_size;i++){
    	int max_x,min_x,max_y,min_y,tmpx,tmpy;
    	line_point_size=line[i].lipoint.size();
    	max_x=min_x=(int)(line[i].lipoint[0].x/grid_size)*grid_size;	//calculate which is start grid
    	max_y=min_y=(int)(line[i].lipoint[0].y/grid_size)*grid_size;
    	if(line[i].lipoint[0].x<0)	max_x=min_x=min_x-grid_size;
    	if(line[i].lipoint[0].y<0)	max_y=min_y=min_y-grid_size;
    	int no_other=1,start=0;
    	ltmp.lipoint.clear();	//clear ltmp
    	ltmp.id=line[i].id;
    	for(int j=0;j<line_point_size;j++){
			tmpx=(int)(line[i].lipoint[j].x/grid_size)*grid_size;	//get the current point is in which grid
			tmpy=(int)(line[i].lipoint[j].y/grid_size)*grid_size;
			if(line[i].lipoint[j].x<0)	tmpx-=grid_size;
			if(line[i].lipoint[j].y<0)	tmpy-=grid_size;
			if(tmpx>max_x){	//check the new area whether has other object that may cause line and line cross
				for(int k=min_y;k<=max_y;k+=grid_size){
					if(grid[tmpx][k].grid_point.size()>0	//if new grid has obstacle point
						|| (grid[tmpx][k].line_id.size()==1 && grid[tmpx][k].line_id[line[i].id]==0)
						|| grid[tmpx][k].line_id.size()>1){	//has another line
						no_other=0;
						break;
					}
					if(no_other)	max_x=tmpx;	//if new area just has no other object, expend the area
				}
			}else if(tmpx<min_x){
				for(int k=min_y;k<=max_y;k+=grid_size){
					if(grid[tmpx][k].grid_point.size()>0
						|| (grid[tmpx][k].line_id.size()==1 && grid[tmpx][k].line_id[line[i].id]==0)
						|| grid[tmpx][k].line_id.size()>1){
						no_other=0;
						break;
					}
					if(no_other)	min_x=tmpx;	//if new area just has no other object, expend the area
				}
			}
			if(tmpy>max_y){
				for(int k=min_x;k<=max_x;k+=grid_size){
					if(grid[k][tmpy].grid_point.size()>0
						||(grid[k][tmpy].line_id.size()==1 && grid[k][tmpy].line_id[line[i].id]==0)
						|| grid[k][tmpy].line_id.size()>1){
						no_other=0;
						break;
					}
					if(no_other)	max_y=tmpy;	//if new area just has no other object, expend the area
				}
			}else if(tmpy<min_y){
				for(int k=min_x;k<=max_x;k+=grid_size){
					if(grid[k][tmpy].grid_point.size()>0
						|| (grid[k][tmpy].line_id.size()==1 && grid[k][tmpy].line_id[line[i].id]==0)
						|| grid[k][tmpy].line_id.size()>1){
						no_other=0;
						break;
					}
					if(no_other)	min_y=tmpy;	//if new area just has no other object, expend the area
				}
			}
			if(min_x==max_x && min_y==max_y){	//the area just has "one" grid
				if(grid[min_x][min_y].grid_point.size()>0
					|| (grid[min_x][min_y].line_id.size()==1 && grid[min_x][min_y].line_id[line[i].id]==0)
					|| grid[min_x][min_y].line_id.size()>1)
					no_other=0;
			}
			if(!no_other){	//do delete
				if(start==j){	//if this grid have obstacle point or another line
					ptmp=line[i].lipoint[start];
					ltmp.lipoint.push_back(ptmp);
					start++;
					max_x=min_x=(int)(line[i].lipoint[start].x/grid_size)*grid_size;	//calculate which is start grid
					max_y=min_y=(int)(line[i].lipoint[start].y/grid_size)*grid_size;
				}else if(start==j-1){	//if this grid just have one point in this line
					ptmp=line[i].lipoint[start];
					ltmp.lipoint.push_back(ptmp);
					start=j;
					j--;
					max_x=min_x=(int)(line[i].lipoint[start].x/grid_size)*grid_size;	//calculate which is start grid
					max_y=min_y=(int)(line[i].lipoint[start].y/grid_size)*grid_size;
				}else{	//if this big area have another line or obstacle point
					remove_num+=j-start-2;
					ptmp=line[i].lipoint[start];
					ltmp.lipoint.push_back(ptmp);
					ptmp=line[i].lipoint[j-1];
					ltmp.lipoint.push_back(ptmp);
					start=j;
					j--;
					max_x=min_x=(int)(line[i].lipoint[start].x/grid_size)*grid_size;	//calculate which is start grid
					max_y=min_y=(int)(line[i].lipoint[start].y/grid_size)*grid_size;
				}
				no_other=1;
			}else if(no_other && j==line_point_size-1){	//if the current point to
				ptmp=line[i].lipoint[start];			//the end of line have no other obstacle
				ltmp.lipoint.push_back(ptmp);
				ptmp=line[i].lipoint[j];
				ltmp.lipoint.push_back(ptmp);
				remove_num+=j-start-1;
			}
    	}//for(int j=0;j<line_point_size;j++)
    	ans.push_back(ltmp);
    }
    printf("the num of remove point is %d\n",remove_num);
    /*print ans */
    line_size=ans.size();
    for(int i=0;i<line_size;i++){
    	line_point_size=ans[i].lipoint.size();
		fprintf(LineOut,"%d:<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\">",ans[i].id);
		fprintf(LineOut,"<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">");
		for(int j=0;j<line_point_size;j++){
			fprintf(LineOut,"%.7lf,%.7lf ",ans[i].lipoint[j].x,ans[i].lipoint[j].y);
		}
		fprintf(LineOut,"</gml:coordinates></gml:LineString>\n");
    }
    fclose(LineInput);
    fclose(PointInput);
    fclose(LineOut);
    return 0;
}
