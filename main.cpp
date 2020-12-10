#include <iostream>
#include <fstream>
#include <list>

using namespace std;

struct Edge;
class City;

struct Edge {
    City* node;
    unsigned short distance;

    Edge(City* node, unsigned short weight) {
        this->node = node;
        this->distance = weight;
    }
};

struct City {
    unsigned short id;
    list<Edge*> edges;
    unsigned int minimumDistance;
    list<City*> minimumDistanceFrom;
    bool minimumDistancePropagated = false;
    list<City*> citiesDamaged;
    bool foundDamagedCities = false;

    City(unsigned short id) {
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
        cout << "City   id -> " << id << "   distance -> " << minimumDistance << "   from ";
        for(auto city : minimumDistanceFrom){
            cout << city->id << " ";
        }
        cout << "  damaged cities -> " << citiesDamaged.size() << "\n";
    }

    void calculateDistanceFrom(City* from, unsigned int weight) {
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

    void calculateCitiesDamageFromHere(){
        foundDamagedCities = true;

        for(auto edge : edges){
            edge->node->calculateCitiesDamage();
        }

        citiesDamaged.clear();
    }
    list<City*> calculateCitiesDamage(){
        if(foundDamagedCities)
            return citiesDamaged;
        foundDamagedCities = true;

        citiesDamaged.push_front(this);
        for(auto e : edges){
            for(auto city : e->node->minimumDistanceFrom){
                if(city == this && e->node->minimumDistanceFrom.size() == 1){
                    citiesDamaged.merge(e->node->calculateCitiesDamage());
                }
            }
        }
        return citiesDamaged;
    }

    list<City*> getCitiesDamaged(){
        return citiesDamaged;
    };

    unsigned short getId() const{
        return id;
    };
};


void calculateDistance(City **cities, unsigned short p) {
    auto powarts = cities[p];

    powarts->minimumDistance = 0;
    powarts->minimumDistanceFrom.push_front(powarts);
    powarts->minimumDistancePropagated = true;

    auto c = powarts->edges.begin();
    while (true) {
    }
}


int main() {
    unsigned short int N, P;
    unsigned int M;
    ifstream in("input.txt");
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
    cities[P]->calculateCitiesDamageFromHere();

    City* maxDamagedCity = cities[0];
    for (unsigned short int i = 0; i < N; i++) {
//        cities[i]->print();

        if(cities[i]->getCitiesDamaged().size() > maxDamagedCity->getCitiesDamaged().size()){
            maxDamagedCity = cities[i];
        }
    }

    cout << "Max damage city: " << maxDamagedCity->getCitiesDamaged().size();
//    maxDamagedCity->print();

    ofstream out("output.txt");
    out << maxDamagedCity->getCitiesDamaged().size() << '\n';
    for(auto c : maxDamagedCity->getCitiesDamaged()) {
        out << c->getId() << '\n';
    }
    in.close();
    out.close();
    return 0;
}
