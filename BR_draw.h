
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <limits>
#include <string>
using namespace std;

void setColor(int color) {
    std::cout << "\033[1;" << color << "m"; 
}


void resetColor() {
    std::cout << "\033[0m"; 
}
class draw_matrix
{
public:
    int Len, Wth;
    char** Mat;


    draw_matrix() : Len(0), Wth(0), Mat(nullptr) {}

   
    draw_matrix(int _Len) : Len(_Len), Wth(_Len)
    {
        Mat = new char* [Len];
        for (int i = 0; i < Len; i++) {
            Mat[i] = new char[Wth];
            for (int j = 0; j < Wth; j++) {
               
                Mat[i][j] =' ';

            }
        }
    }


    draw_matrix(int _Len, int _Wth) : Len(_Len), Wth(_Wth)
    {
        Mat = new char* [Len];
        for (int i = 0; i < Len; i++) {
            Mat[i] = new char[Wth];
            for (int j = 0; j < Wth; j++) {
                Mat[i][j] =' ';
            }
        }
    }


    ~draw_matrix()
    {
        if (Mat) {
            for (int i = 0; i < Len; i++)
                delete[] Mat[i];
            delete[] Mat;
        }
    }

    void print()
    {
        for (int i = 0; i < Len; i++) {
            for (int j = 0; j < Wth; j++) {
                switch (Mat[i][j])
                {
                case('F'):
                    setColor(32);
                    break;
                case('A'):
                     setColor(31);
                     break;
                case('-'):
                      setColor(35);
                      break;
                case('*'):
                    setColor(33);
                    break;
                case('0'):
                    setColor(31);
                    break;
                default:
                    setColor(36);
                    break;
                      

                }
                cout <<" " <<Mat[i][j];
                resetColor();
            }
            cout << endl;
        }
    }

  
    void draw_point(int rad, int x, int y, char sym)
    {
        for (int i = max(0, x - rad); i <= min(Len - 1, x + rad); i++) {
            for (int j = max(0, y - rad); j <= min(Wth - 1, y + rad); j++) {
                Mat[i][j] = sym;
            }
        }
    }
    void draw_line(int x1, int y1, int x2, int y2, int rad, char sym)
    {
        int dx = abs(x2 - x1), dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
            draw_point(rad, x1, y1, sym); 

            if (x1 == x2 && y1 == y2) break;

            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x1 += sx; } 
            if (e2 < dx) { err += dx; y1 += sy; } 
        }
    }

	
};
