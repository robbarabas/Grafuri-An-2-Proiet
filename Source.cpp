
#include "BR_draw.h"

using namespace std;
draw_matrix canvas(100, 100);

enum NodeType { ANT, FOOD };

class nod {
public:
    int x, y;
    NodeType type;
    int* adj;
    int adj_size;

    nod() : x(0), y(0), adj(nullptr), adj_size(0), type(FOOD) {}

    void set(int _x, int _y, NodeType _type) {
        x = _x; y = _y; type = _type;
    }

    void add_adj(int index) {
        for (int i = 0; i < adj_size; i++)
            if (adj[i] == index) return;
        int* new_adj = new int[adj_size + 1];
        for (int i = 0; i < adj_size; i++) new_adj[i] = adj[i];
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
    map<string, int> name_to_index;
    vector<string> index_to_name;

    Graph() : Noduri(nullptr), size(0) {}

    ~Graph() {
        delete[] Noduri;
    }

    void read_from_file(const string& filename) {
        ifstream in(filename);
        string line, section;
        int index = 0;
        vector<string> names;
        vector<int> xs, ys;
        vector<NodeType> types;
        vector<pair<string, string>> edges;

        while (getline(in, line)) {
            if (line.empty()) continue;
            if (line == "NODES" || line == "EDGES") {
                section = line;
                continue;
            }

            if (section == "NODES") {
                string name, type_str;
                int x, y;
                istringstream iss(line);
                iss >> name >> x >> y >> type_str;
                NodeType type = (type_str == "ANT") ? ANT : FOOD;
                name_to_index[name] = index++;
                index_to_name.push_back(name);
                names.push_back(name);
                xs.push_back(x);
                ys.push_back(y);
                types.push_back(type);
            }
            else if (section == "EDGES") {
                string from, to;
                istringstream iss(line);
                iss >> from >> to;
                edges.emplace_back(from, to);
            }
        }

        size = index;
        Noduri = new nod[size];

        for (int i = 0; i < size; ++i) {
            Noduri[i].set(xs[i], ys[i], types[i]);
        }

        for (int i = 0; i < edges.size(); ++i) {
            int a = name_to_index[edges[i].first];
            int b = name_to_index[edges[i].second];
            if (Noduri[a].type == ANT && Noduri[b].type == ANT) continue;
            Noduri[a].add_adj(b);
            Noduri[b].add_adj(a);
        }
    }

    void draw(draw_matrix& mat) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < Noduri[i].adj_size; j++) {
                int neighbor = Noduri[i].adj[j];
                if (i < neighbor)
                    mat.draw_line(Noduri[i].x, Noduri[i].y,
                        Noduri[neighbor].x, Noduri[neighbor].y, 0, '-');
            }
        }
        for (int i = 0; i < size; i++) {
            char symbol = (Noduri[i].type == ANT) ? 'A' : 'F';
            mat.draw_point(1, Noduri[i].x, Noduri[i].y, symbol);
        }
    }

    void check_connectivity(draw_matrix& canvas) {
        vector<bool> visited(size+1, false);
        queue<int> q;
        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int curr = q.front(); q.pop();
            for (int i = 0; i < Noduri[curr].adj_size; i++) {
                int neighbor = Noduri[curr].adj[i];
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        bool connected = true;
        for (int i = 0; i < size; ++i) {
            if (!visited[i]) {
                connected = false;
                break;
            }
        }

        cout << (connected ? "Graful este conex.\n" : "Graful NU este conex.\n");
        canvas.print(); 
    }


   

    void dijkstra(int src) {
        vector<int> dist(size, INT_MAX);
        dist[src] = 0;
        set<pair<int, int>> pq;
        pq.insert({ 0, src });

        while (!pq.empty()) {
            int node = pq.begin()->second;
            pq.erase(pq.begin());
            for (int i = 0; i < Noduri[node].adj_size; i++) {
                int to = Noduri[node].adj[i];
                if (dist[node] + 1 < dist[to]) {
                    pq.erase({ dist[to], to });
                    dist[to] = dist[node] + 1;
                    pq.insert({ dist[to], to });
                }
            }
        }

        cout << "Distantele de la nodul sursa:\n";
        for (int i = 0; i < size; i++) {
            cout << index_to_name[i] << ": ";
            if (dist[i] == INT_MAX) cout << "INF";
            else cout << dist[i];
            cout << endl;
            canvas.draw_point(1,Noduri[i].x, Noduri[i].y,(char)dist[i]+'0');
            
        }
    }
    void kruskal() {
        vector<tuple<int, int, int>> edges;

        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < Noduri[i].adj_size; j++) {
                int v = Noduri[i].adj[j];
                if (i < v) { 
                    edges.push_back(make_tuple(1, i, v));
                }
            }
        }

        sort(edges.begin(), edges.end()); 

       
        vector<int> parent(size), rank(size, 0);
        for (int i = 0; i < size; ++i)
            parent[i] = i;

      
        auto find = [&](int x) {
            while (x != parent[x])
                x = parent[x] = parent[parent[x]];
            return x;
            };

      
        auto union_sets = [&](int a, int b) {
            a = find(a);
            b = find(b);
            if (a != b) {
                if (rank[a] < rank[b])
                    swap(a, b);
                parent[b] = a;
                if (rank[a] == rank[b])
                    rank[a]++;
                return true;
            }
            return false;
            };

        cout << "Muchiile arborelui partial minim (Kruskal):\n";
        for (auto& e : edges) {
            int u = get<1>(e), v = get<2>(e);
            if (union_sets(u, v)) {
                cout << index_to_name[u] << " - " << index_to_name[v] << "\n";
               canvas.draw_line(Noduri[u].x, Noduri[u].y,Noduri[v].x,Noduri[v].y,0,'*');
            }
        }
        for (int i = 0; i < size; i++) {
            char symbol = (Noduri[i].type == ANT) ? 'A' : 'F';
            canvas.draw_point(1, Noduri[i].x, Noduri[i].y, symbol);
        }
    }


    void matching() {
        set<int> matched;
        cout << "Cuplaje gasite:\n";
        for (int i = 0; i < size; i++) {
            if (matched.count(i)) continue;
            for (int j = 0; j < Noduri[i].adj_size; j++) {
                int v = Noduri[i].adj[j];
                if (!matched.count(v)) {
                    cout << index_to_name[i] << " - " << index_to_name[v] << endl;
                    matched.insert(i);
                    matched.insert(v);
                    
                            cout << index_to_name[i] << " - " << index_to_name[v] << "\n";
                            canvas.draw_line(Noduri[i].x, Noduri[i].y, Noduri[v].x, Noduri[v].y, 0, '@');
                    
                         
                }

            }

        }
        for (int i = 0; i < size; i++) {
            char symbol = (Noduri[i].type == ANT) ? 'A' : 'F';
            canvas.draw_point(1, Noduri[i].x, Noduri[i].y, symbol);
        }
    }
};

int main() {
 
    Graph G;
    G.read_from_file("Data.txt");
    G.draw(canvas); 

    int choice;
    while (true) {
        cout << "\n--- MENIU ---\n";
        cout << "1. Conexitate\n";
        cout << "2. Cel mai scurt drum (de la un nod sursa)\n";
        cout << "3. Arbore partial minim\n";
        cout << "4. Cuplaje in graf\n";
        cout << "Alege optiunea: ";
        cin >> choice;

        switch (choice) {
        case 1:
            G.check_connectivity(canvas);
            break;
        case 2: {
            string name;
            cout << "Introdu numele nodului sursa: ";
            cin >> name;
            if (G.name_to_index.count(name)) {
                G.dijkstra(G.name_to_index[name]);
            }
            else {
                cout << "Nod inexistent.\n";
            }
            canvas.print();
            break;
        }
        case 3:
            G.kruskal();
            canvas.print();
            break;
        case 4:
            G.matching();
            canvas.print();
            break;
        case 5:
            return 0;
        default:
            cout << "Optiune invalida.\n";
        }
    }

    return 0;
}
