all:TinyWindow

TinyWindow: ./
	g++ -std=c++11 -DSO -w -g -fpermissive -o ./GameOfLife ./GameOfLife.cpp -I../dependencies/ -I../dependencies/glm/glm -I../dependencies/tinywindow/include -I../dependencies/tinyextender/include/ -I../dependencies/tinyshaders/include -I../include/ -lGL -lX11 2> errors.txt


