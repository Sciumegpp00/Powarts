#include <iostream>
#include <fstream>
#include <list>

using namespace std;

struct Distance;
struct Edge;
class City;
//class Graph;

struct Edge {
    City* node;
    int weight;

    Edge(City* node, int weight) {
        this->node = node;
        this->weight = weight;
    }
};

class City {
    int id;
    list<Edge*> edges;
    int distance;
    list<City*> distanceFrom;
    bool distancePropagated = false;

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
        cout << "City   id -> " << id << "   distance -> " << distance << '\n';
    }
    void calculateDistancesFromHere() {
        distance = 0;
        distanceFrom.push_front(this);

        calculateEdges();
    }
    void calculateDistanceFrom(City* from, int weight) {
        if(distanceFrom.empty()) {
            distance = weight;
            distanceFrom.push_front(from);
        } else if(distance == weight) {
            distanceFrom.push_front(from);
        } else if(weight < distance) {
            distance = weight;
            distanceFrom.clear();
            distanceFrom.push_front(from);
        }

        calculateEdges();
    }
    void calculateEdges() {
        if (distancePropagated)
            return;
        distancePropagated = true;

        for (auto edge : edges) {
            edge->node->calculateDistanceFrom(this, distance + edge->weight);
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


int N, M, P;

int main() {
    ifstream in("input.txt");
    in >> N; // Cities number
    in >> M; // Edges number
    in >> P; // Powarts city id
    cout << "Cities: " << N << '\n';
    City **cities = (City**) calloc(N, sizeof(City*));

    int c1Id, c2Id, w;
    City *c1, *c2;
    for (int i = 0; i < M; i++) {
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

    for (int i = 0; i < N; i++) {
        cities[i]->print();
    }

    ofstream out("output.txt");
    // TODO: Write output
    in.close();
    out.close();
    return 0;
}
