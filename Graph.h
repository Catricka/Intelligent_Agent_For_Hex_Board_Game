
#ifndef GRAPH_H
#define	GRAPH_H
#include <map>
#include <list>
#include <queue>
#include <iostream>
#include <limits>
#include <fstream>
#include <string>

static const short NIL = -1;
static const double INF = std::numeric_limits< double >::infinity();
typedef std::deque<std::pair<double*, int> > PDeque;

class Graph {
public:

    enum Colour {
        FIRST, RED, GREEN, BLUE, LAST
    };

    Graph() {
        // default constructor that does not do anything
    };
    Graph(int size, int distance, double density);
    Graph(std::ifstream &file);
    Graph(const Graph& G);

    void add_arc(int source, int dest, double weight = 1);
    void add_edge(int source, int dest, double weight = 1, Colour c = RED);
    void set_vertex(int v, double dist = INF, int parent = NIL);
    void set_all_vertex(double dist = INF, int parent = NIL);
    void set_dist(int vertex, double dist);
    void set_parent(int vertex, int parent);
    void set_edge_list();

    void build_random_graph(int size, int distance, double density);
    void erase();

    double get_dist(int vertex);
    double get_weight(int source, int dest);
    int get_parent(int vertex);
    int get_vertex_count();
    int get_edge_count();
    std::vector<int>* get_edges(int i);
    bool edge_exists(int source, int dest);

    void dijkstra_run(int source);
    std::list<int> dijkstra_get_path(int dest);

    void prim_run(std::map<Colour, bool>);
    void prim_run();
    std::list<std::pair<int, int> > prim_get_path();
    int prim_get_dist();

    void print_prop();
    void print_adj_list();
    friend std::ostream& operator<<(std::ostream &out, Graph &cGraph);

    PDeque make_queue();
private:
    // An edge has a target, its weight and optionally a colour.

    struct edge {
        int dest;
        double weight;
        Colour c;

        edge(int dest, double weight, Colour c = RED)
        : dest(dest), weight(weight), c(c) {
        }
    };

    // Node Properties like estimated distance and parent

    typedef struct propo {
        double dist;
        int parent;
        bool inTree; // needed for Prim's only
    } property;

    // Comparator for pairs

    struct Pcomp {

        bool operator()(std::pair<double*, int> const& lhs, std::pair<double*, int> const& rhs) {
            return *(lhs.first) > *(rhs.first);
        }
    };

    typedef std::list<edge> EdList;
    typedef std::map<int, EdList > AdjMap;
    typedef std::map<int, property> PropMap;

    std::vector<std::vector<int> > edgeList;
    static const std::string clabel[5];
    AdjMap adj;
    PropMap prop;
};

#endif	/* GRAPH_H */
