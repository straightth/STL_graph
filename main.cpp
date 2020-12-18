#include "gtest/gtest.h"
#include "Graph.hpp"

class TestGraph : public testing::Test {
protected:
    Graph<int, int> graph;
    TestGraph() = default;
};

TEST_F(TestGraph, TestVertexAndVertexIterator) { //Test vertex methods
    int firstValue = 3;
    ASSERT_EQ(0, graph.size());
    ASSERT_EQ(true, graph.empty());
    graph.addVertex(firstValue);
    ASSERT_EQ(false, graph.empty());
    auto j = graph.begin();
    ASSERT_EQ(firstValue, (*j)->getContent());
    ASSERT_EQ(firstValue, graph[0].getContent());
    ASSERT_EQ((*j), &graph[0]);
    int secondValue = 4;
    graph.addVertex(secondValue);
    ASSERT_EQ(2, graph.size());
    ++j;
    ASSERT_EQ(secondValue, (*j)->getContent());
    ASSERT_EQ(secondValue, graph[1].getContent());
    ASSERT_EQ((*j), &graph[1]);
    --j;
    ASSERT_EQ(firstValue, (*j)->getContent());
    ASSERT_EQ(firstValue, graph[0].getContent());
    ASSERT_EQ((*j), &graph[0]);
    ++++j;
    ASSERT_EQ(j, graph.end());
    --j;
    graph.deleteVertex(graph[0]);
    ASSERT_EQ(1, graph.size());
    ASSERT_EQ(secondValue, (*j)->getContent());
    ASSERT_EQ(secondValue, graph[0].getContent());
    ASSERT_EQ((*j), &graph[0]);
    int thirdValue = 5;
    graph.addVertex(thirdValue);
    ASSERT_EQ(2, graph.size());
    ++j;
    ASSERT_EQ(thirdValue, (*j)->getContent());
    ASSERT_EQ(thirdValue, graph[1].getContent());
    ASSERT_EQ((*j), &graph[1]);
    graph.deleteVertex(graph[0]);
    ASSERT_EQ(1, graph.size());
    --j;
    ASSERT_EQ(thirdValue, (*j)->getContent());
    ASSERT_EQ(thirdValue, graph[0].getContent());
    ASSERT_EQ((*j), &graph[0]);
    auto i = graph.begin();
    ASSERT_EQ((*j), (*i));
    i = graph.end();
    --i;
    ASSERT_EQ((*j), (*i));
    ASSERT_EQ(0, graph.getVertexIndex(graph[0]));
    ASSERT_EQ(true, graph.vertexExists(graph[0]));
    Graph<int, int> tempGraph = graph;
    ASSERT_EQ(true, tempGraph == graph);
    ASSERT_EQ(false, graph.empty());
    graph.clear();
    ASSERT_EQ(true, graph.empty());
    ASSERT_EQ(0, graph.size());
}

TEST_F(TestGraph, TestEdgeAndEdgeIterator) {
    int firstValue = 1;
    graph.addVertex(firstValue);
    ASSERT_EQ(0, graph[0].size());
    ASSERT_EQ(true, graph[0].empty());
    graph[0].addEdge(firstValue, graph[1]);
    ASSERT_EQ(false, graph[0].empty());
    auto j = graph[0].begin();
    ASSERT_EQ(firstValue, (*j)->getContent());
    ASSERT_EQ(firstValue, graph[0][0].getContent());
    ASSERT_EQ((*j), &graph[0][0]);
    int secondValue = 2;
    graph[0].addEdge(secondValue, graph[1]);
    ASSERT_EQ(2, graph[0].size());
    ++j;
    ASSERT_EQ(secondValue, (*j)->getContent());
    ASSERT_EQ(secondValue, graph[0][1].getContent());
    ASSERT_EQ((*j), &graph[0][1]);
    --j;
    ASSERT_EQ(firstValue, (*j)->getContent());
    ASSERT_EQ(firstValue, graph[0][0].getContent());
    ASSERT_EQ((*j), &graph[0][0]);
    ++++j;
    ASSERT_EQ(j, graph[0].end());
    --j;
    graph[0].deleteEdge(graph[0][0]);
    ASSERT_EQ(1, graph[0].size());
    ASSERT_EQ(secondValue, (*j)->getContent());
    ASSERT_EQ(secondValue, graph[0][0].getContent());
    ASSERT_EQ((*j), &graph[0][0]);
    int thirdValue = 5;
    graph[0].addEdge(thirdValue, graph[1]);
    ASSERT_EQ(2, graph[0].size());
    ++j;
    ASSERT_EQ(thirdValue, (*j)->getContent());
    ASSERT_EQ(thirdValue, graph[0][1].getContent());
    ASSERT_EQ((*j), &graph[0][1]);
    graph[0].deleteEdge(graph[0][0]);
    ASSERT_EQ(1, graph[0].size());
    --j;
    ASSERT_EQ(thirdValue, (*j)->getContent());
    ASSERT_EQ(thirdValue, graph[0][0].getContent());
    ASSERT_EQ((*j), &graph[0][0]);
    auto i = graph[0].begin();
    ASSERT_EQ((*j), (*i));
    i = graph[0].end();
    --i;
    ASSERT_EQ((*j), (*i));
    ASSERT_EQ(true, graph[0].edgeExists(graph[0][0]));
}