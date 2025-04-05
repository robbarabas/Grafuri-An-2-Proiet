#include "BR_draw.h"

int main() {
   
    draw_matrix A(100,200);
    A.draw_line(3, 3, 50, 50, 1, 'e');
    A.draw_line(30, 10, 50, 50, 1, 'f');
    A.draw_line(50, 50, 100, 100, 2, '/');
    A.draw_line(50, 0, 50, 200, 1, 'X');
    A.print();
    return 0;
}
