#include <iostream>
#include <fstream>
#include <list>

using namespace std;


//class Graph


int N;

int main() {
    ifstream in("input.txt");
    in >> N;
    cout << N << '\n';
//    auto l = new list<int>();

    for (int i = 0; i < N; i++) {
        // TODO: Add to graph
    }

    ofstream out("output.txt");
    // TODO: Write output
    in.close();
    out.close();
    return 0;
}
