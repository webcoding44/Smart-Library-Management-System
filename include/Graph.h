    #ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>

class Graph {
private:
    std::map<std::string, std::set<std::string>> adjacencyList;
    std::map<std::string, std::map<std::string, double>> weights; // وزن یال‌ها

public:
    // اضافه کردن راس
    void addVertex(std::string vertex) {
        if (adjacencyList.find(vertex) == adjacencyList.end()) {
            adjacencyList[vertex] = std::set<std::string>();
        }
    }

    // اضافه کردن یال (غیر جهت‌دار با وزن)
    void addEdge(std::string v1, std::string v2, double weight = 1.0) {
        addVertex(v1);
        addVertex(v2);
        adjacencyList[v1].insert(v2);
        adjacencyList[v2].insert(v1);
        weights[v1][v2] = weight;
        weights[v2][v1] = weight;
    }

    // حذف یال
    void removeEdge(std::string v1, std::string v2) {
        if (adjacencyList.find(v1) != adjacencyList.end()) {
            adjacencyList[v1].erase(v2);
        }
        if (adjacencyList.find(v2) != adjacencyList.end()) {
            adjacencyList[v2].erase(v1);
        }
    }

    // حذف راس
    void removeVertex(std::string vertex) {
        if (adjacencyList.find(vertex) != adjacencyList.end()) {
            for (const auto& neighbor : adjacencyList[vertex]) {
                adjacencyList[neighbor].erase(vertex);
            }
            adjacencyList.erase(vertex);
        }
    }

    // همسایه‌ها
    std::set<std::string> getNeighbors(std::string vertex) const {
        if (adjacencyList.find(vertex) != adjacencyList.end()) {
            return adjacencyList.at(vertex);
        }
        return std::set<std::string>();
    }

    // BFS - Breadth First Search
    std::vector<std::string> bfs(std::string start) const {
        std::vector<std::string> result;
        std::set<std::string> visited;
        std::queue<std::string> q;
        
        if (adjacencyList.find(start) == adjacencyList.end()) {
            return result;
        }
        
        visited.insert(start);
        q.push(start);
        
        while (!q.empty()) {
            std::string current = q.front();
            q.pop();
            result.push_back(current);
            
            if (adjacencyList.find(current) != adjacencyList.end()) {
                for (const auto& neighbor : adjacencyList.at(current)) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        q.push(neighbor);
                    }
                }
            }
        }
        
        return result;
    }

    // DFS - Depth First Search
    std::vector<std::string> dfs(std::string start) const {
        std::vector<std::string> result;
        std::set<std::string> visited;
        
        if (adjacencyList.find(start) == adjacencyList.end()) {
            return result;
        }
        
        dfsHelper(start, visited, result);
        return result;
    }

    void dfsHelper(std::string vertex, std::set<std::string>& visited, 
                   std::vector<std::string>& result) const {
        visited.insert(vertex);
        result.push_back(vertex);
        
        if (adjacencyList.find(vertex) != adjacencyList.end()) {
            for (const auto& neighbor : adjacencyList.at(vertex)) {
                if (visited.find(neighbor) == visited.end()) {
                    dfsHelper(neighbor, visited, result);
                }
            }
        }
    }

    // پیداکردن مسیر کوتاه‌ترین با BFS
    std::vector<std::string> shortestPath(std::string start, std::string end) const {
        if (adjacencyList.find(start) == adjacencyList.end() || 
            adjacencyList.find(end) == adjacencyList.end()) {
            return std::vector<std::string>();
        }
        
        std::map<std::string, std::string> parent;
        std::set<std::string> visited;
        std::queue<std::string> q;
        
        visited.insert(start);
        q.push(start);
        parent[start] = "";
        
        while (!q.empty()) {
            std::string current = q.front();
            q.pop();
            
            if (current == end) {
                // Reconstruct path
                std::vector<std::string> path;
                std::string curr = end;
                while (curr != "") {
                    path.push_back(curr);
                    curr = parent[curr];
                }
                std::reverse(path.begin(), path.end());
                return path;
            }
            
            if (adjacencyList.find(current) != adjacencyList.end()) {
                for (const auto& neighbor : adjacencyList.at(current)) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        parent[neighbor] = current;
                        q.push(neighbor);
                    }
                }
            }
        }
        
        return std::vector<std::string>(); // No path found
    }

    // پیشنهاد کتاب‌های مرتبط
    std::vector<std::string> getRelatedBooks(std::string bookId, int maxResults = 5) const {
        std::vector<std::pair<std::string, double>> scored;
        
        if (adjacencyList.find(bookId) == adjacencyList.end()) {
            return std::vector<std::string>();
        }
        
        // BFS تا عمق 2
        std::set<std::string> visited;
        std::queue<std::pair<std::string, int>> q;
        
        visited.insert(bookId);
        q.push({bookId, 0});
        
        while (!q.empty()) {
            auto [current, depth] = q.front();
            q.pop();
            
            if (depth > 2) continue;
            
            if (adjacencyList.find(current) != adjacencyList.end()) {
                for (const auto& neighbor : adjacencyList.at(current)) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        double weight = 1.0;
                        if (weights.find(current) != weights.end() && 
                            weights.at(current).find(neighbor) != weights.at(current).end()) {
                            weight = weights.at(current).at(neighbor);
                        }
                        scored.push_back({neighbor, weight / (depth + 1)});
                        
                        if (depth < 2) {
                            q.push({neighbor, depth + 1});
                        }
                    }
                }
            }
        }
        
        // مرتب‌سازی بر اساس امتیاز
        std::sort(scored.begin(), scored.end(), 
            [](const auto& a, const auto& b) { return a.second > b.second; });
        
        std::vector<std::string> result;
        for (int i = 0; i < std::min(maxResults, (int)scored.size()); i++) {
            result.push_back(scored[i].first);
        }
        
        return result;
    }

    int getVertexCount() const { return adjacencyList.size(); }
    
    int getEdgeCount() const {
        int count = 0;
        for (const auto& pair : adjacencyList) {
            count += pair.second.size();
        }
        return count / 2; // غیر جهت‌دار
    }

    void display() const {
        for (const auto& pair : adjacencyList) {
            std::cout << pair.first << " -> ";
            for (const auto& neighbor : pair.second) {
                std::cout << neighbor << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif // GRAPH_H