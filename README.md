# Sigspactial 2014

[http://mypages.iit.edu/~xzhang22/GISCUP2014/index.php](http://mypages.iit.edu/~xzhang22/GISCUP2014/index.php)

## Document

### C++ get answer

Compile

    g++ -o Simplify main.cpp

Execute

    ./Simplify < PointToRemove > < LineInputFilePath > < PointInputFilePath > < OutputFilePath >

Execute example

    #sample 01
    ./Simplify 100 sample_01/lines_out.txt sample_01/points_out.txt answer.txt

    #sample 02
    ./Simplify 100 sample_02/lines_out.txt sample_02/points_out.txt answer.txt

    #sample 03
    ./Simplify 100 sample_03/lines_out.txt sample_03/points_out.txt answer.txt

### Python draw picture
First you have to install the python2.7 and pip

install require package

    pip intall -r requirements.txt

install require package (alternative)

    sudo apt-get install python-matplotlib

start drawing

    python draw.py

## Note
line.txt point.txt 是我自己測的小測資

目前設定上grid_size 為1000
已處理好初始刪點，只剩格子內的刪除


目前碰到一個怪問題，若把gird_size (格子大小) 寫大一點如一千，
則可以有效判斷此格子是否有其他的object，如果grid_size 寫10，則無法判斷

2014/7/25
新增只有輸出點的部分而沒有gml 格式
anstext.txt 為只輸出答案點
pointtext.txt 為只輸出障礙點的部分

2014/7/22
目前已可處理線與線之間的判斷
即如此格子若有其他的物體，可判斷出來

2014/7/21
更正點放格子的方式：
	因為座標有負的關係，所以此點的歸類需減一個grid_size
	Ex: (-1.23,0) grid_size=10
	則此點需放 gird[-10][0].grid_point
