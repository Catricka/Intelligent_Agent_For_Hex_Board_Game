
#include "Graph.h"
#include <ctime>
#include <cstdlib>

using namespace std;

/* Useful for printing the Enums as strings */
const string Graph::clabel[5] = {"First", "Red", "Green", "Blue", "Last"};

/* Constructor allows you to specify size, distance and density */
Graph::Graph(int size, int distance, double density) {
    if (size != 0) {
        build_random_graph(size, distance, density);
    }
}

/* Constructor that takes a file with the first line being the number of nodes
   and the remaining lines being the source, destination, weight and colour of each edge
   delimited by a space.
 */
Graph::Graph(ifstream &file) {
    int source, dest, weight;
    Colour colour;
    string col_s;
    if (file.is_open()) {
        file.ignore(); //Number of nodes, not needed
        while (file.good()) {
            file >> source >> dest >> weight >> col_s;

            if (col_s.compare("red") == 0) {
                colour = RED;
            } else if (col_s.compare("green") == 0) {
                colour = GREEN;
            } else if (col_s.compare("blue") == 0) {
                colour = BLUE;
            }

            add_edge(source, dest, weight, colour);
        }
    }
}

/* Copy constructor */
Graph::Graph(const Graph& G) {
    for (AdjMap::const_iterator i = G.adj.begin(); i != G.adj.end(); ++i) {
        for (EdList::const_iterator j = i->second.begin(); j != i->second.end(); ++j) {
            adj[i->first].push_back(edge(j->dest, j->weight));
        }
    }
    for (PropMap::const_iterator i = G.prop.begin(); i != G.prop.end(); i++) {
        prop[i->first].dist = i->second.dist;
        prop[i->first].parent = i->second.parent;
    }
}

/* Adds a one-directional edge between source and dest with a defined weight */
void Graph::add_arc(int source, int dest, double weight) {
    adj[source].push_back(edge(dest, weight));
    prop[source].dist = INF;
    prop[source].parent = NIL;
    prop[dest].dist = INF;
    prop[dest].parent = NIL;
}

/* Adds a undirected edge between source and dest with a defined weight */
void Graph::add_edge(int source, int dest, double weight, Colour c) {
    adj[source].push_back(edge(dest, weight, c));
    adj[dest].push_back(edge(source, weight, c));
    prop[source].dist = INF;
    prop[source].parent = NIL;
    prop[dest].dist = INF;
    prop[dest].parent = NIL;
}

/* Returns the distance of a given vertex */
double Graph::get_dist(int vertex) {
    return prop[vertex].dist;
}

/* Returns the weight of a given vertex*/
double Graph::get_weight(int source, int dest) {
    for (EdList::iterator i = adj[source].begin(); i != adj[source].end(); ++i) {
        if (i->dest == dest) {
            return i->weight;
        }
    }
}

/* Returns the parent of a given vertex*/
int Graph::get_parent(int vertex) {
    return prop[vertex].parent;
}

vector<int>* Graph::get_edges(int i) {
    return &edgeList[i];
}

/* Sets the parent of a given vertex */
void Graph::set_parent(int vertex, int parent) {
    prop[vertex].parent = parent;
}

/* Sets the distance of a given vertex */
void Graph::set_dist(int vertex, double dist) {
    prop[vertex].dist = dist;
}

/* Sets the parent and distance of a given vertex */
void Graph::set_vertex(int v, double dist, int parent) {
    prop[v].dist = dist;
    prop[v].parent = parent;
}

/* Sets all the vertices in the Graph to the given distance and parent and resets
   inTree value */
void Graph::set_all_vertex(double dist, int parent) {
    for (PropMap::iterator i = prop.begin(); i != prop.end(); ++i) {
        i->second.dist = dist;
        i->second.parent = parent;
        i->second.inTree = false;
    }
}

/* Returns the number of vertices in the Graph*/
int Graph::get_vertex_count() {
    return prop.size();
}

/* Returns the number of edges in the Graph*/
int Graph::get_edge_count() {
    int count = 0;
    for (AdjMap::iterator i = adj.begin(); i != adj.end(); ++i) {
        count += i->second.size();
    }
    return count / 2;
}

/* Returns true if the edge exists in the Graph */
bool Graph::edge_exists(int source, int dest) {
    for (EdList::iterator i = adj[source].begin(); i != adj[source].end(); ++i) {
        if (i->dest == dest) {
            return true;
        }
    }
    return false;
}

/* Builds a Graph with the specified size, maximum distance between edges and density */
void Graph::build_random_graph(int size, int distance, double density) {
    srand(time(NULL));
    for (int i = 1; i < size; i++) {
        for (int j = i + 1; j <= size; j++) {
            if ((rand() % 100) <= (density * 100)) {
                add_edge(i, j, rand() % distance + 1);
            }
        }
    }
}

/* Erases all Nodes in the graph */
void Graph::erase() {
    adj.erase(adj.begin(), adj.end());
    prop.erase(prop.begin(), prop.end());
}

/* Makes a special queue composed of a pair of vertices and their distance */
PDeque Graph::make_queue() {
    PDeque Q;
    for (AdjMap::iterator i = adj.begin(); i != adj.end(); ++i) {
        Q.push_back(make_pair(&(prop[i->first].dist), i->first));
    }
    return Q;
}

/* Prints the Graph Node's properties */
void Graph::print_prop() {
    for (PropMap::iterator i = prop.begin(); i != prop.end(); ++i) {
        cout << i->first << ": distance: " << prop[i->first].dist << ", parent: " << prop[i->first].parent << endl;
    }
    cout << endl;
}

/* Runs the Dijkstra algorithm on a specified source
 * Pre: no negative weights
 */
void Graph::dijkstra_run(int source) {
    set_all_vertex();
    set_vertex(source, 0, NIL);

    PDeque Q = make_queue();
    make_heap(Q.begin(), Q.end(), Pcomp());

    while (!Q.empty()) {
        int x = Q.front().second;
        Q.pop_front();
        for (EdList::iterator j = adj[x].begin(); j != adj[x].end(); ++j) {
            double newDist = get_dist(x) + j->weight;
            if (get_dist(j->dest) > newDist) {
                set_vertex(j->dest, newDist, x);
                make_heap(Q.begin(), Q.end(), Pcomp());
            }
        }
    }
}

/* Returns a list consisting of an optimal path from source
 * (specified in run()) to dest.
 * Pre: run() has to be run before this. */
list<int> Graph::dijkstra_get_path(int dest) {
    if (get_parent(dest) == NIL) {
        cout << "Dijkstra::run() needs to be run first!" << endl;
    }

    list<int> S;
    while (get_parent(dest) != NIL) {
        S.push_front(dest);
        dest = get_parent(dest);
    }
    S.push_front(dest);
    return S;
}

/* Runs Prim's algorithm on edges of all colours.
 */
void Graph::prim_run() {
    map<Colour, bool> c;
    for (int i = FIRST + 1; i < LAST; i++) {
        c[(Colour) i] = true;
    }
    return prim_run(c);
}

/* Runs Prim's algorithm on the Graph and takes the colours that are allowed as
 * a Map of colours.
 */
void Graph::prim_run(map<Colour, bool> c) {
    int r = adj.begin()->first;

    set_all_vertex();
    set_vertex(r, 0, NIL);

    PDeque Q = make_queue();
    make_heap(Q.begin(), Q.end(), Pcomp());

    while (!Q.empty()) {
        int u = Q.front().second;
        Q.pop_front();
        prop[u].inTree = true;
        for (EdList::iterator v = adj[u].begin(); v != adj[u].end(); ++v) {
            double newDist = v->weight;
            if (!(prop[v->dest].inTree) && get_dist(v->dest) > newDist && c.count(v->c) != 0) {
                set_vertex(v->dest, newDist, u);
                make_heap(Q.begin(), Q.end(), Pcomp());
            }
        }
        make_heap(Q.begin(), Q.end(), Pcomp());
    }
}

/* Returns a list of pairs that represent the different edges belonging to the
   MST created by Prim's.
 */
list<pair<int, int> > Graph::prim_get_path() {
    list<pair<int, int> > L;
    for (PropMap::iterator i = prop.begin(); i != prop.end(); ++i) {
        if (i->second.parent == NIL) continue;
        L.push_back(make_pair(i->second.parent, i->first));
    }
    return L;
}

/* Returns the total distance of the MST created by Prim's */
int Graph::prim_get_dist() {
    int dist = 0;
    for (PropMap::iterator i = prop.begin(); i != prop.end(); ++i) {
        dist += i->second.dist;
    }
    return dist;
}

void Graph::print_adj_list() {
    for (Graph::AdjMap::iterator i = adj.begin(); i != adj.end(); ++i) {
        cout << i->first << ": ";
        for (Graph::EdList::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            cout << j->dest << ", ";
        }
        cout << endl;
    }
}

void Graph::set_edge_list() {
    edgeList.resize(adj.size()+1);
    for (AdjMap::iterator i = adj.begin(); i != adj.end(); i++) {
        edgeList[i->first].resize(i->second.size()+1);
        for (Graph::EdList::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            edgeList[i->first].push_back(j->dest);
        }
    }
}

/* Overloaded output stream function for printing the Graph */
ostream& operator<<(ostream &out, Graph & g) {
    for (Graph::AdjMap::iterator i = g.adj.begin(); i != g.adj.end(); ++i) {
        for (Graph::EdList::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            out << "(" << i->first << ", " << j->dest << ")" << " weight: " << j->weight << " colour: " << Graph::clabel[j->c] << endl;
        }
    }
    return out;
}

/* Overloaded ostream for list<int> > */
ostream& operator<<(ostream &out, std::list<int> &S) {
    if (S.size() < 2) {
        out << "No Path found!";
        return out;
    }
    for (std::list<int>::iterator i = S.begin(); i != S.end(); i++) {
        if (i != S.begin()) {
            out << ", ";
        }
        out << *i;
    }
    return out;
}

/* Overloaded ostream for list<pair<int, int> > */
ostream& operator<<(ostream &out, std::list<std::pair<int, int> > &S) {
    if (S.size() < 2) {
        out << "No Path found!";
        return out;
    }
    for (std::list<std::pair<int, int> >::iterator i = S.begin(); i != S.end(); i++) {
        if (i != S.begin()) {
            out << ", ";
        }
        out << "(" << i->first << "," << i->second << ")";
    }
    return out;
}