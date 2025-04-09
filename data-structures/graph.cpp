#include "Graph.h"
#include <iostream>

// Vertex constructor implementation
Graph::Vertex::Vertex(int r, int c) : row(r), col(c) {
    // Initialize domain with all possible values 1-9
    for (int i = 1; i <= 9; i++) {
        domain.insert(i);
    }
}

// Graph constructor
Graph::Graph() : numVertices(0) {
    for (int i = 0; i < MAX_VERTICES; i++) {
        vertexExists[i] = false;
        vertices[i] = nullptr;
    }
}

// Graph destructor
Graph::~Graph() {
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (vertexExists[i]) {
            delete vertices[i];
        }
    }
}

// Convert row, col to a unique vertex ID
int Graph::getVertexId(int row, int col) const {
    return row * 9 + col;
}

// Add a vertex to the graph
void Graph::addVertex(int row, int col) {
    int id = getVertexId(row, col);
    if (!vertexExists[id]) {
        vertices[id] = new Vertex(row, col);
        vertexExists[id] = true;
        numVertices++;
    }
}

// Add an edge between two vertices
void Graph::addEdge(int row1, int col1, int row2, int col2) {
    int id1 = getVertexId(row1, col1);
    int id2 = getVertexId(row2, col2);
    
    // Don't add self-loops
    if (id1 == id2) return;
    
    if (vertexExists[id1] && vertexExists[id2]) {
        // Check if the edge already exists
        CustomVector<CustomPair<int, int>>& neighbors = vertices[id1]->neighbors;
        bool edgeExists = false;
        
        for (int i = 0; i < neighbors.size(); i++) {
            if (neighbors[i].first == row2 && neighbors[i].second == col2) {
                edgeExists = true;
                break;
            }
        }
        
        if (!edgeExists) {
            vertices[id1]->neighbors.push_back(CustomPair<int, int>(row2, col2));
        }
        
        // Add the reverse edge
        neighbors = vertices[id2]->neighbors;
        edgeExists = false;
        
        for (int i = 0; i < neighbors.size(); i++) {
            if (neighbors[i].first == row1 && neighbors[i].second == col1) {
                edgeExists = true;
                break;
            }
        }
        
        if (!edgeExists) {
            vertices[id2]->neighbors.push_back(CustomPair<int, int>(row1, col1));
        }
    }
}

// Get neighbors of a vertex
CustomVector<CustomPair<int, int>> Graph::getNeighbors(int row, int col) const {
    int id = getVertexId(row, col);
    if (vertexExists[id]) {
        return vertices[id]->neighbors;
    }
    return CustomVector<CustomPair<int, int>>();
}
void Graph::debugPrint() const {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int id = getVertexId(row, col);
            if (vertexExists[id]) {
                std::cout << "Cell (" << row << ", " << col << ") domain: { ";
                for (auto it = vertices[id]->domain.begin(); it != vertices[id]->domain.end(); ++it) {
                    std::cout << *it << " ";
                }
                std::cout << "} | Neighbors: ";
                for (int i = 0; i < vertices[id]->neighbors.size(); i++) {
                    std::cout << "(" << vertices[id]->neighbors[i].first << "," << vertices[id]->neighbors[i].second << ") ";
                }
                std::cout << "\n";
            }
        }
    }
}
void Graph::connectSudokuConstraints() {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            // Connect all other cells in same row
            for (int k = 0; k < 9; k++) {
                if (k != col) addEdge(row, col, row, k);
            }
            // Connect all other cells in same column
            for (int k = 0; k < 9; k++) {
                if (k != row) addEdge(row, col, k, col);
            }
            // Connect all other cells in the same 3x3 box
            int boxRowStart = (row / 3) * 3;
            int boxColStart = (col / 3) * 3;
            for (int r = boxRowStart; r < boxRowStart + 3; r++) {
                for (int c = boxColStart; c < boxColStart + 3; c++) {
                    if (r != row || c != col) {
                        addEdge(row, col, r, c);
                    }
                }
            }
        }
    }
}

// Set the domain of a vertex
bool Graph::setDomain(int row, int col, const unorderedSet& domain) {
    int id = getVertexId(row, col);
    if (vertexExists[id]) {
        vertices[id]->domain = domain;
        return true;
    }
    return false;
}

// Set a single value in the domain
bool Graph::setValue(int row, int col, int value) {
    int id = getVertexId(row, col);
    if (vertexExists[id]) {
        vertices[id]->domain.clear();
        vertices[id]->domain.insert(value);
        return true;
    }
    return false;
}


// Remove a value from the domain of a vertex
bool Graph::removeFromDomain(int row, int col, int value) {
    int id = getVertexId(row, col);
    if (vertexExists[id]) {
        vertices[id]->domain.remove(value);
        return true;
    }
    return false;
}

// Check if a vertex has a single value in its domain
bool Graph::isSingleValue(int row, int col) const {
    int id = getVertexId(row, col);
    if (vertexExists[id]) {
        return vertices[id]->domain.size() == 1;
    }
    return false;
}

// Get the single value in the domain if it has only one value
int Graph::getValue(int row, int col) const {
    int id = getVertexId(row, col);
    if (vertexExists[id] && vertices[id]->domain.size() == 1) {
        // Since we can't directly access the first element of our custom set,
        // we'll use the iterator to get the first element
        unorderedSet::Iterator it = vertices[id]->domain.begin();
        return *it;
    }
    return 0; // Return 0 if not single value or invalid
}

// Check if the domain is empty
bool Graph::isDomainEmpty(int row, int col) const {
    int id = getVertexId(row, col);
    if (vertexExists[id]) {
        return vertices[id]->domain.empty();
    }
    return true; // Consider invalid vertices as having empty domains
}
