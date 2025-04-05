#include "BR_draw.h"
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
using namespace std;

class nod {
public:
    int x, y;
    int* adj;
    int adj_size;

    nod() : x(0), y(0), adj(nullptr), adj_size(0) {}

    void set_coords(int _x, int _y) {
        x = _x;
        y = _y;
    }

    void add_adj(int index) {
        int* new_adj = new int[adj_size + 1];
        for (int i = 0; i < adj_size; i++) {
            new_adj[i] = adj[i];
        }
        new_adj[adj_size] = index;
        delete[] adj;
        adj = new_adj;
        adj_size++;
    }

    ~nod() {
        delete[] adj;
    }
};

class Graph {
public:
    nod* Noduri;
    int size;

    Graph() : Noduri(nullptr), size(0) {}

    ~Graph() {
        delete[] Noduri;
    }

    void read_from_file(const string& filename) {
        ifstream in(filename);
        if (!in) {
            cerr << "NU a putut fi gasit fiserul\n";
            return;
        }

        string line;
        string section;
        int index = 0;
        map<string, int> name_to_index;
        vector<pair<int, int>> edge_list;

        while (getline(in, line)) {
            if (line.empty()) continue;
            if (line == "NODES" || line == "EDGES") {
                section = line;
                continue;
            }

            if (section == "NODES") {
                string name;
                int x, y;
                istringstream iss(line);
                iss >> name >> x >> y;
                name_to_index[name] = index++;
            }
        }

        
        size = index;
        Noduri = new nod[size];

        in.clear();
        in.seekg(0); // rewind file

        section = "";
        while (getline(in, line)) {
            if (line.empty()) continue;
            if (line == "NODES" || line == "EDGES") {
                section = line;
                continue;
            }

            if (section == "NODES") {
                string name;
                int x, y;
                istringstream iss(line);
                iss >> name >> x >> y;
                int i = name_to_index[name];
                Noduri[i].set_coords(x, y);
            }

            if (section == "EDGES") {
                string from, to;
                istringstream iss(line);
                iss >> from >> to;
                int a = name_to_index[from];
                int b = name_to_index[to];
                Noduri[a].add_adj(b);
                Noduri[b].add_adj(a); // undirected
            }
        }

        in.close();
    }

    void draw(draw_matrix& mat) {
  
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < Noduri[i].adj_size; j++) {
                int neighbor = Noduri[i].adj[j];
                if (i < neighbor) {
                    mat.draw_line(Noduri[i].x, Noduri[i].y,
                        Noduri[neighbor].x, Noduri[neighbor].y,
                        0, '-');
                }
            }
        }

        
        for (int i = 0; i < size; i++) {
            mat.draw_point(1, Noduri[i].x, Noduri[i].y, 'O');
        }
    }
};

int main() {
    draw_matrix tabla(100, 100);

    Graph G;
    G.read_from_file("Data.txt");

    G.draw(tabla);

    tabla.print();

    return 0;
}
