#include <iostream>
#include <fstream>
#include <list>

using namespace std;

struct Edge;
class City;
//class Graph;

struct Edge {
    City* node;
    unsigned short int distance;

    Edge(City* node, int weight) {
        this->node = node;
        this->distance = weight;
    }
};

class City {
    unsigned short int id;
    list<Edge*> edges;
    unsigned short int minimumDistance;
    list<City*> minimumDistanceFrom;
    bool minimumDistancePropagated = false;

public:
    City(int id) {
        this->id = id;
    }
    ~City() {
        auto iterator = edges.begin();
        Edge* n;
        while ((n = *iterator)) {
            delete n;
        }
    }
    void addEdge(Edge* edge) {
        edges.push_front(edge);
    }
    void print() const {
        cout << "City   id -> " << id << "   distance -> " << minimumDistance << '\n';
    }
    void calculateDistancesFromHere() {
        minimumDistance = 0;
        minimumDistanceFrom.push_front(this);

        calculateEdges();
    }
    void calculateDistanceFrom(City* from, int weight) {
        if(minimumDistanceFrom.empty()) {
            minimumDistance = weight;
            minimumDistanceFrom.push_front(from);
        } else if(minimumDistance == weight) {
            minimumDistanceFrom.push_front(from);
        } else if(weight < minimumDistance) {
            minimumDistance = weight;
            minimumDistanceFrom.clear();
            minimumDistanceFrom.push_front(from);
            minimumDistancePropagated = false;
        }

        calculateEdges();
    }
    void calculateEdges() {
        if (minimumDistancePropagated)
            return;
        minimumDistancePropagated = true;

        for (auto edge : edges) {
            edge->node->calculateDistanceFrom(this, minimumDistance + edge->distance);
        }
    }
};

//class Graph {
//    City* powarts;
//
//    Graph() {
//        powards = new City();
//    }
//};


unsigned short int N, P;
unsigned int M;

int main() {
    ifstream in("input0.txt");
    in >> N; // Cities number
    in >> M; // Edges number
    in >> P; // Powarts city id
    cout << "Cities: " << N << '\n';
    City **cities = (City**) calloc(N, sizeof(City*));

    unsigned int c1Id, c2Id, w;
    City *c1, *c2;
    for (unsigned int i = 0; i < M; i++) {
        in >> c1Id;
        in >> c2Id;
        in >> w;

        if(!cities[c1Id]) {
            c1 = new City(c1Id);
            cities[c1Id] = c1;
        } else
            c1 = cities[c1Id];
        if(!cities[c2Id]) {
            c2 = new City(c2Id);
            cities[c2Id] = c2;
        } else
            c2 = cities[c2Id];

        c1->addEdge(new Edge(c2, w));
        c2->addEdge(new Edge(c1, w));
    }

    cities[P]->calculateDistancesFromHere();

    for (unsigned short int i = 0; i < N; i++) {
        cities[i]->print();
    }

    ofstream out("output.txt");
    // TODO: Write output
    in.close();
    out.close();
    return 0;
}
