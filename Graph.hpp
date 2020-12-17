#ifndef STL_GRAPH_GRAPH_H
#define STL_GRAPH_GRAPH_H

#include <iostream>

using namespace std;

template<typename T1, typename T2>
class Vertex;

template<typename T1, typename T2>
class Graph;

template<typename T1, typename T2>
class Edge {
    typedef T1 vertexValueType;
    typedef const T1 vertexConstValueType;
    typedef T1& vertexValueTypeReference;
    typedef T1* vertexValueTypePointer;
    typedef T2 edgeValueType;
    typedef const T2 edgeConstValueType;
    typedef T2& edgeValueTypeReference;
    typedef T2* edgeValueTypePointer;
    edgeValueTypePointer content = nullptr;
    Edge *nextEdge = nullptr;
    Edge *previousEdge = nullptr;
    Vertex<vertexValueType, edgeValueType> *src = nullptr;
    Vertex<vertexValueType, edgeValueType> *dest = nullptr;

    explicit Edge(edgeValueTypeReference object, Vertex<vertexValueType, edgeValueType> &src, Vertex<vertexValueType, edgeValueType> &dest) {
        content = &object;
        this->src = &src;
        this->dest = &dest;
    }

    Edge(edgeValueTypeReference object, Vertex<vertexValueType, edgeValueType> &src, Vertex<vertexValueType, edgeValueType> &dest, Edge &lastEdge) : Edge(object, src, dest) {
        previousEdge = &lastEdge;
        previousEdge->nextEdge = this;
    }

    friend class Vertex<vertexValueType, edgeValueType>;

    friend class Graph<vertexValueType, edgeValueType>;
public:

    int &getContent() const {
        return (*content);
    }

    Vertex<vertexValueType, edgeValueType> &getSrc() const {
        return (*src);
    }

    Vertex<vertexValueType, edgeValueType> &getDest() const {
        return (*dest);
    }
};

template<typename T1, typename T2>
class Vertex {
    typedef T1 vertexValueType;
    typedef const T1 vertexConstValueType;
    typedef T1& vertexValueTypeReference;
    typedef T1* vertexValueTypePointer;
    typedef T2 edgeValueType;
    typedef const T2 edgeConstValueType;
    typedef T2& edgeValueTypeReference;
    typedef T2* edgeValueTypePointer;
    vertexValueTypePointer content = nullptr;
    Vertex *nextVertex = nullptr;
    Vertex *previousVertex = nullptr;
    Edge<vertexValueType, edgeValueType> *firstEdge = nullptr;
    Edge<vertexValueType, edgeValueType> *lastEdge = nullptr;
    Graph<vertexValueType, edgeValueType> *pointerToContainer = nullptr;
    int edgeAmount = 0;

    explicit Vertex(vertexValueTypeReference object, Graph<vertexValueType, edgeValueType> &container) {
        content = &object;
        pointerToContainer = &container;
    }

    Vertex(vertexValueTypeReference object, Graph<vertexValueType, edgeValueType> &container, Vertex &lastVertex) {
        content = &object;
        pointerToContainer = &container;
        previousVertex = &lastVertex;
        previousVertex->nextVertex = this;
    }

    ~Vertex() {
        while (firstEdge != nullptr) {
            Edge<vertexValueType, edgeValueType> *temp = firstEdge;
            firstEdge = firstEdge->nextEdge;
            delete temp;
        }
    }

    friend class Graph<vertexValueType, edgeValueType>;
public:
    bool empty() {
        return !this->firstEdge;
    }

    const int &size() const {
        return edgeAmount;
    }

    int &getContent() const {
        return (*content);
    }

    void addEdge(edgeValueTypeReference object, Vertex &dest) {
        if (firstEdge == nullptr) {
            firstEdge = new Edge<vertexValueType, edgeValueType>(object, (*this), dest);
            lastEdge = firstEdge;
        } else {
            lastEdge = new Edge<vertexValueType, edgeValueType>(object, (*this), dest, (*firstEdge));
        }
        ++edgeAmount;
    }

    void deleteEdge(Edge<vertexValueType, edgeValueType> &edge) {
        Edge<vertexValueType, edgeValueType> *temp = firstEdge;
        while (temp != &edge) temp = temp->nextEdge;
        temp->previousEdge = temp->nextEdge;
        temp->nextEdge->previousEdge = temp->previousEdge;
        if (temp == firstEdge) firstEdge = firstEdge->nextEdge;
        else if (temp == lastEdge) lastEdge = lastEdge->previousEdge;
        delete temp;
        --edgeAmount;
    }

    bool edgeExists(Edge<vertexValueType, edgeValueType> &edge) const {
        Edge<vertexValueType, edgeValueType> *temp = firstEdge;
        while (temp != nullptr) {
            if (&(*temp) == &edge) return true;
            temp = temp->nextEdge;
        }
        return false;
    }

    Edge<vertexValueType, edgeValueType> &operator[](int index) const {
        Edge<vertexValueType, edgeValueType> *temp = firstEdge;
        while (index != 0) {
            temp = temp->nextEdge;
            --index;
        }
        return (*temp);
    }

    class iterator {
        Edge<vertexValueType, edgeValueType> *pointer = nullptr;
        Vertex *pointerToContainer = nullptr;
    protected:
        explicit iterator(Edge<vertexValueType, edgeValueType> &edge) {
            pointer = &edge;
            if (pointer != nullptr) pointerToContainer = pointer->src;
        }

        explicit iterator(Vertex &vertex) {
            pointer = nullptr;
            pointerToContainer = &vertex;
        }

    public:
        iterator() = default;

        iterator &operator=(iterator it) {
            swap((*this).pointer, it.pointer);
            swap((*this).pointerToContainer, it.pointerToContainer);
            return *this;
        }

        virtual iterator &operator++() {
            if (pointer == nullptr) pointer = pointerToContainer->firstEdge;
            else pointer = pointer->nextEdge;
            return *this;
        }

        virtual iterator &operator--() {
            if (pointer == nullptr) pointer = pointerToContainer->lastEdge;
            else pointer = pointer->previousEdge;
            return *this;
        }

        Edge<vertexValueType, edgeValueType> *operator*() const {
            return pointer;
        }

        friend class Vertex;

        friend class Graph<vertexValueType, edgeValueType>;

        friend bool operator==(const Vertex::iterator &lhs, const Vertex::iterator &rhs) {
            if (lhs.pointer == rhs.pointer && lhs.pointerToContainer == rhs.pointerToContainer) return true;
            else return false;
        };

        friend bool operator!=(const Vertex::iterator &lhs, const Vertex::iterator &rhs) {return !operator==(lhs, rhs);}
    };

    class constIterator : public iterator {
    protected:
        explicit constIterator(Edge<vertexValueType, edgeValueType> &edge) : iterator(edge) {};

        explicit constIterator(Vertex &vertex) : iterator(vertex) {};

    public:
        constIterator() = default;

        const Edge<vertexValueType, edgeValueType> *operator*() const {
            return iterator::operator*();
        }

        friend class Vertex;

        friend class Graph<vertexValueType, edgeValueType>;
    };

    class reverseIterator : public iterator {
    protected:
        explicit reverseIterator(Edge<vertexValueType, edgeValueType> &edge) : iterator(edge) {};

        explicit reverseIterator(Vertex &vertex) : iterator(vertex) {};

    public:
        reverseIterator() = default;

        iterator &operator++() override {
            return iterator::operator--();
        }

        iterator &operator--() override {
            return iterator::operator++();
        }

        friend class Vertex;

        friend class Graph<vertexValueType, edgeValueType>;
    };

    iterator &begin() {
        return (*new iterator((*firstEdge)));
    }

    iterator &end() {
        return (*new iterator((*this)));
    }

    constIterator &cbegin() {
        return (*new constIterator((*firstEdge)));
    }

    constIterator &cend() {
        return (*new constIterator((*this)));
    }

    reverseIterator &rbegin() {
        return (*new reverseIterator((*lastEdge)));
    }

    reverseIterator &rend() {
        return (*new reverseIterator((*this)));
    }
};

template<typename T1, typename T2>
class Graph {
protected:
    typedef T1 vertexValueType;
    typedef const T1 vertexConstValueType;
    typedef T1& vertexValueTypeReference;
    typedef T1* vertexValueTypePointer;
    typedef T2 edgeValueType;
    typedef const T2 edgeConstValueType;
    typedef T2& edgeValueTypeReference;
    typedef T2* edgeValueTypePointer;
    Vertex<vertexValueType, edgeValueType> *firstVertex = nullptr;
    Vertex<vertexValueType, edgeValueType> *lastVertex = nullptr;
    int vertexAmount = 0;
public:
    Graph() = default;

    Graph(Graph const &graph) {
        firstVertex = graph.firstVertex;
        lastVertex = graph.lastVertex;
        vertexAmount = graph.vertexAmount;
    }

    ~Graph() {
        this->clear();
    }

    bool empty() {
        return !this->firstVertex;
    }

    void clear() {
        while (firstVertex != nullptr) {
            Vertex<vertexValueType, edgeValueType> *temp = firstVertex;
            firstVertex = firstVertex->nextVertex;
            delete temp;
        }
        vertexAmount = 0;
    }

    const int& size() const {
        return vertexAmount;
    }

    int getVertexIndex(const Vertex<vertexValueType, edgeValueType> &vertex) const {
        Vertex<vertexValueType, edgeValueType> *temp = firstVertex;
        int index = 0;
        while (&(*temp) != &vertex) temp = temp->nextVertex;
        return index;
    }

    Graph &operator=(Graph rhs) {
        swap(this->firstVertex, rhs.firstVertex);
        swap(this->lastVertex, rhs.lastVertex);
        swap(this->vertexAmount, rhs.vertexAmount);
        return *this;
    }

    void addVertex(vertexValueTypeReference object) {
        if (firstVertex == nullptr) {
            firstVertex = new Vertex<vertexValueType, edgeValueType>(object, (*this));
            lastVertex = firstVertex;
        } else {
            lastVertex = new Vertex<vertexValueType, edgeValueType>(object, (*this), (*firstVertex));
        }
        ++vertexAmount;
    }

    void deleteVertex(Vertex<vertexValueType, edgeValueType> &vertex) {
        Vertex<vertexValueType, edgeValueType> *temp = firstVertex;
        while (temp != &vertex) temp = temp->nextVertex;
        temp->previousVertex = temp->nextVertex;
        temp->nextVertex->previousVertex = temp->previousVertex;
        if (temp == firstVertex) firstVertex = firstVertex->nextVertex;
        else if (temp == lastVertex) lastVertex = lastVertex->previousVertex;
        delete temp;
        --vertexAmount;
    }

    bool vertexExists(Vertex<vertexValueType, edgeValueType> &vertex) const {
        Vertex<vertexValueType, edgeValueType> *temp = firstVertex;
        while (firstVertex != nullptr) {
            if (&(*temp) == &vertex) return true;
            temp = temp->nextVertex;
        }
        return false;
    }

    Vertex<vertexValueType, edgeValueType> &operator[](int index) const {
        Vertex<vertexValueType, edgeValueType> *temp = firstVertex;
        while (index != 0) {
            temp = temp->nextVertex;
            --index;
        }
        return (*temp);
    }

    friend std::ostream &operator<<(ostream &os, Graph<vertexValueType, edgeValueType> &graph) {
        auto vertexIterator = graph.begin();
        int index = 0;
        auto edgeIterator = graph[index].begin();
        while ((*vertexIterator) != nullptr) {
            os << index + 1 << ": ";
            edgeIterator = graph[index].begin();
            if ((*edgeIterator) != nullptr) {
                os << graph.getVertexIndex((*edgeIterator)->getDest()) + 1;
                ++edgeIterator;
            }
            while ((*edgeIterator) != nullptr) {
                os << ", " << graph.getVertexIndex((*edgeIterator)->getDest()) + 1;
                ++edgeIterator;
            }
            os << endl;
            ++vertexIterator;
            ++index;
        }
        return os;
    }

    friend inline bool operator==(Graph<vertexValueType, edgeValueType> &lhs, Graph<vertexValueType, edgeValueType> &rhs) {
#define vertexesHasEdges !(*lhsVertexIterator)->empty() && !(*rhsVertexIterator)->empty()
#define iteratorsIsNotPointsToEnd lhsEdgeIterator != (*lhsVertexIterator)->end() && rhsEdgeIterator != (*rhsVertexIterator)->end()
        if (lhs.size() != rhs.size()) return false;
        auto lhsVertexIterator = lhs.begin();
        auto rhsVertexIterator = rhs.begin();
        auto lhsEdgeIterator = (*lhsVertexIterator)->begin();
        auto rhsEdgeIterator = (*rhsVertexIterator)->begin();
        while (lhsVertexIterator != lhs.end() && rhsVertexIterator != rhs.end()) {
            lhsEdgeIterator = (*lhsVertexIterator)->begin();
            rhsEdgeIterator = (*rhsVertexIterator)->begin();
            if ((*lhsVertexIterator)->size() != (*rhsVertexIterator)->size()) return false;
            while (vertexesHasEdges && iteratorsIsNotPointsToEnd) {
                if (!((*lhsEdgeIterator)->getContent() == (*lhsEdgeIterator)->getContent()
                      && &(*lhsEdgeIterator)->getDest() == &(*rhsEdgeIterator)->getDest()))
                    return false;
                ++lhsEdgeIterator;
                ++rhsEdgeIterator;
            }
            ++lhsVertexIterator;
            ++rhsVertexIterator;
        }
        return true;
#undef vertexesHasEdges
#undef iteratorsIsNotPointsToEnd
        return true;
    }

    friend inline bool operator!=(const Graph &lhs, const Graph &rhs) {return !operator==(lhs, rhs);};

    class iterator {
        Vertex<vertexValueType, edgeValueType> *pointer = nullptr;
        Graph *pointerToContainer = nullptr;
    protected:
        explicit iterator(Vertex<vertexValueType, edgeValueType> &vertex) {
            pointer = &vertex;
            if (pointer != nullptr) pointerToContainer = vertex.pointerToContainer;
        }

        explicit iterator(Graph &container) {
            pointer = nullptr;
            pointerToContainer = &container;
        }

    public:
        iterator() = default;

        iterator &operator=(iterator it) {
            swap((*this).pointer, it.pointer);
            swap((*this).pointerToContainer, it.pointerToContainer);
            return *this;
        }

        virtual iterator &operator++() {
            if (pointer == nullptr) pointer = pointerToContainer->firstVertex;
            else pointer = pointer->nextVertex;
            return *this;
        }

        virtual iterator &operator--() {
            if (pointer == nullptr) pointer = pointerToContainer->lastVertex;
            else pointer = pointer->previousVertex;
            return *this;
        }

        Vertex<vertexValueType, edgeValueType> *operator*() const {
            return pointer;
        }

        friend class Graph;

        friend bool operator==(const Graph::iterator &lhs, const Graph::iterator &rhs) {
            if (lhs.pointer == rhs.pointer && lhs.pointerToContainer == rhs.pointerToContainer) return true;
            else return false;
        }

        friend bool operator!=(const Graph::iterator &lhs, const Graph::iterator &rhs) {return !operator==(lhs, rhs);}
    };

    class constIterator : public iterator {
    protected:
        explicit constIterator(Vertex<vertexValueType, edgeValueType> &vertex) : iterator(vertex){};

        explicit constIterator(Graph &container) : iterator(container){};

    public:
        constIterator() = default;

        const Vertex<vertexValueType, edgeValueType> *operator*() const {
            return iterator::operator*();
        }

        friend class Graph;
    };

    class reverseIterator : public iterator {
    protected:
        explicit reverseIterator(Vertex<vertexValueType, edgeValueType> &vertex) : iterator(vertex){};

        explicit reverseIterator(Graph &container) : iterator(container){};

    public:
        reverseIterator() = default;

        iterator &operator++() override {
            return iterator::operator--();
        }

        iterator &operator--() override {
            return iterator::operator++();
        }

        friend class Graph;
    };

    class edgeIterator : public Vertex<vertexValueType, edgeValueType>::iterator {
    protected:
        explicit edgeIterator(Edge<vertexValueType, edgeValueType> &edge) : Vertex<vertexValueType, edgeValueType>::iterator(edge){};

        explicit edgeIterator(Vertex<vertexValueType, edgeValueType> &vertex) : Vertex<vertexValueType, edgeValueType>::iterator(vertex){};

    public:
        edgeIterator() = default;

#define _namespace Vertex<vertexValueType, edgeValueType>::iterator
        typename Vertex<vertexValueType, edgeValueType>::iterator &operator++() override {
            if (_namespace::pointer == nullptr) _namespace::pointer = _namespace::pointerToContainer->firstEdge;
            else if (_namespace::pointer->nextEdge == nullptr) _namespace::pointer = _namespace::pointer->src->nextVertex->firstEdge;
            else _namespace::pointer = _namespace::pointer->nextEdge;
            return *this;
        }

        typename Vertex<vertexValueType, edgeValueType>::iterator &operator--() override {
            if (_namespace::pointer == nullptr) _namespace::pointer = _namespace::pointerToContainer->lastEdge;
            else if (_namespace::pointer->previousEdge == nullptr) _namespace::pointer = _namespace::pointer->src->previousVertex->lastEdge;
            else _namespace::pointer = _namespace::pointer->previousEdge;
            return *this;
        }
#undef _namespace
        friend class Graph;
    };

    class constEdgeIterator : public edgeIterator {
    protected:
        explicit constEdgeIterator(Edge<vertexValueType, edgeValueType> &edge) : edgeIterator(edge){};

        explicit constEdgeIterator(Vertex<vertexValueType, edgeValueType> &vertex) : edgeIterator(vertex){};

    public:
        constEdgeIterator() = default;

        const Edge<vertexValueType, edgeValueType> *operator*() const {
            return Vertex<vertexValueType, edgeValueType>::iterator::operator*();
        }

        friend class Graph;
    };

    class reverseEdgeIterator : public edgeIterator {
    protected:
        explicit reverseEdgeIterator(Edge<vertexValueType, edgeValueType> &edge) : edgeIterator(edge){};

        explicit reverseEdgeIterator(Vertex<vertexValueType, edgeValueType> &vertex) : edgeIterator(vertex){};

    public:
        reverseEdgeIterator() = default;

        iterator &operator++() override {
            return edgeIterator::operator--();
        }

        iterator &operator--() override {
            return edgeIterator::operator++();
        }

        friend class Graph;
    };

    iterator &begin() {
        return (*new iterator((*firstVertex)));
    }

    iterator &end() {
        return (*new iterator((*this)));
    }

    constIterator &cbegin() {
        return (*new constIterator((*firstVertex)));
    }

    constIterator &cend() {
        return (*new constIterator((*this)));
    }

    reverseIterator &rbegin() {
        return (*new reverseIterator((*lastVertex)));
    }

    reverseIterator &rend() {
        return (*new reverseIterator((*this)));
    }

    edgeIterator &edgeBegin() {
        return (*new edgeIterator((*firstVertex->firstEdge)));
    }

    edgeIterator &edgeEnd() {
        return (*new edgeIterator((*lastVertex)));
    }

    constEdgeIterator &edgeConstBegin() {
        return (*new constEdgeIterator((*firstVertex->firstEdge)));
    }

    constEdgeIterator &edgeConstEnd() {
        return (*new constEdgeIterator((*lastVertex)));
    }

    reverseEdgeIterator &edgeReverseBegin() {
        return (*new reverseEdgeIterator((*lastVertex->lastEdge)));
    }

    reverseEdgeIterator &edgeReverseEnd() {
        return (*new reverseEdgeIterator((*lastVertex)));
    }
};

#endif //STL_GRAPH_GRAPH_H