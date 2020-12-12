#include <iostream>
#include <fstream>
#include <list>
#include <queue>

using namespace std;

struct Edge;
struct CityQueue;
class City;

struct Edge {
    City* node;
    unsigned short distance;

    Edge(City* node, unsigned short weight) {
        this->node = node;
        this->distance = weight;
    }
};

struct CityQueue {
    City* from;
    queue<Edge*>* edgeQueue;

    CityQueue(City* f, list<Edge*> *edges) {
        from = f;
        edgeQueue = new queue<Edge*>();
        for(auto e : *edges) {
            edgeQueue->push(e);
        }
    }
    CityQueue(City* f, queue<Edge*> *queue) {
        from = f;
        edgeQueue = queue;
    }
    ~CityQueue() {
        delete edgeQueue;
    }
};

class City {
    unsigned short id;
    list<Edge*> edges;
    unsigned int minimumDistance;
    list<City*> minimumDistanceFrom;
    bool minimumDistancePropagated = false;
    unsigned short citiesDamagedSize;
    City *damagedFrom;
    bool foundDamagedCities = false;

public:
    City(unsigned short id) {
        this->id = id;
    }
    ~City() {
        auto iterator = edges.begin();
        Edge* e;
        while (iterator != edges.end()) {
            e = *iterator;
            iterator++;
            delete e;
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
        cout << "  damaged cities -> " << citiesDamagedSize << "\n";
    }
    void calculateDistancesFromHereIterative() {
        minimumDistance = 0;
        minimumDistanceFrom.push_front(this);

        auto queue = new list<CityQueue*>();
        queue->push_back(new CityQueue(this, &edges));

        CityQueue* cityQueue;
        City* fromCity;
        Edge* cityEdge;
        do {
            cityQueue = queue->front();
            queue->pop_front();
            fromCity = cityQueue->from;

            while (!cityQueue->edgeQueue->empty()) {
                cityEdge = cityQueue->edgeQueue->front();
                cityQueue->edgeQueue->pop();

                auto cq = cityEdge->node->
                        calculateDistanceFromIterative(fromCity,
                                                       fromCity->minimumDistance + cityEdge->distance);
                if(cq)
                    queue->push_back(cq);
            }
            delete cityQueue;
        } while (!queue->empty());

        delete queue;
    }
    void calculateDistancesFromHere() {
        minimumDistance = 0;
        minimumDistanceFrom.push_front(this);

        calculateEdges();
    }
    CityQueue* calculateDistanceFromIterative(City* from, unsigned int weight) {
        if(minimumDistanceFrom.empty()) {
            minimumDistance = weight;
            minimumDistanceFrom.push_front(from);
        } else if(minimumDistance == weight) {
            if(find(minimumDistanceFrom.begin(), minimumDistanceFrom.end(), from) == minimumDistanceFrom.end())
                minimumDistanceFrom.push_front(from);
            return NULL;
        } else if(weight < minimumDistance) {
            minimumDistance = weight;
            minimumDistanceFrom.clear();
            minimumDistanceFrom.push_front(from);
        } else
            return NULL;

        if(++edges.begin() == edges.end())
            return NULL; // If it's a leaf return null

        return new CityQueue(this, &edges);

//        auto edgesToCalculate = new list<Edge*>();
//
//        for (auto c : minimumDistanceFrom) {
//        }
//
//        CityQueue* toReturn = NULL;
//        if(!edgesToCalculate->empty())
//            toReturn = new CityQueue(this, edgesToCalculate);
//        delete edgesToCalculate;
//        return toReturn;
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
    City* calculateCitiesDamageAndGetMaxFromHere(){
        foundDamagedCities = true;
        City **maxCity = new City*();
        *maxCity = (*edges.begin())->node;

        for(auto edge : edges){
            edge->node->calculateCitiesDamage(maxCity, edge->node);
        }

        citiesDamagedSize = 0;

        City* maxCityToReturn = *maxCity;
        delete maxCity;
        return maxCityToReturn;
    }
    void calculateCitiesDamage(City **maxCity, City *cityDamaged){
        if(foundDamagedCities)
            return;
        foundDamagedCities = true;

        if(id == 1 || id == 3 || id == 4 || id == 5)
            cout << "";

        damagedFrom = cityDamaged;
        citiesDamagedSize = 1;

        for(auto e : edges) {
            bool ok = true;
            bool isThisCity = false;

            for (auto cityFrom : e->node->minimumDistanceFrom) {
                if(!isThisCity && cityFrom == this)
                    isThisCity = true;
                if(cityFrom->damagedFrom != cityDamaged)
                    ok = false;
            }
            if(!isThisCity)
                continue;

            if(ok){
                e->node->calculateCitiesDamage(maxCity, cityDamaged);
                citiesDamagedSize += e->node->citiesDamagedSize;
            } else
                e->node->calculateCitiesDamage(maxCity, e->node);
        }

        if(citiesDamagedSize > (*maxCity)->citiesDamagedSize)
            *maxCity = this;
    }

    unsigned short getCitiesDamagedSize(){
        return citiesDamagedSize;
    };
    unsigned short getId() const{
        return id;
    };
};



int main() {
    unsigned short int N, P;
    unsigned int M;
    ifstream in("input19.txt");
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
    cout << "Added all edges\n";

    cities[P]->calculateDistancesFromHereIterative();
    cout << "Distances calculated\n";

    City* maxDamagedCity = cities[P]->calculateCitiesDamageAndGetMaxFromHere();
    cout << "Damages calculated and max city got\n";

//    for (unsigned short int i = 0; i < N; i++) {
//        cities[i]->print();
//    }
//
//    cout << "Max damage city: " << maxDamagedCity->getCitiesDamaged().size();
//    maxDamagedCity->print();

    ofstream out("output.txt");
    out << maxDamagedCity->getCitiesDamagedSize() << '\n';
//    for(auto c : maxDamagedCity->getCitiesDamaged()) {
//        out << c->getId() << '\n';
//    }
    in.close();
    out.close();

    for (unsigned short i = 0; i < N; i++) {
        delete cities[i];
    }
    delete [] cities;
    return 0;
}
