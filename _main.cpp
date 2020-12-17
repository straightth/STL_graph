#include <iostream>

#include "Graph.hpp"

class Paper {
public:
    int thickness = 4;
};

int main() {
    Graph<int, int> container;
    int paper = 5;
    int _paper = 4;
    int word = 3;
    int _word = 2;
    container.addVertex(paper);
    container.addVertex(_paper);
    cout << container[1].getContent() << endl;
    cout << container[0].getContent() << endl;
    Graph<int, int>::iterator it = container.begin();
    while ((*it) != nullptr) {
        cout << (*it)->getContent() << endl;
        ++it;
    }
    container[0].addEdge(word,container[1]);
    container[0].addEdge(_word, container[1]);
    cout << container[0][0].getContent() << endl;
    cout << container[0][1].getContent() << endl;
    cout << container[0][0].getDest().getContent();
    cout << "\n\n\n";
    cout << (container == container);
    container.begin();
    return 0;
}