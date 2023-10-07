/* Time complexity: O(nPointsInside + nVerts*log2(nVerts)) */
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <fstream>

void polar_sort(std::vector<std::pair<int, int>> &polygon) {
    int ref_ind = 0;
    for (int i = 1; i < polygon.size(); ++i) {
        if (polygon[i].first < polygon[ref_ind].first) {
            ref_ind = i;
        }
    }
    std::swap(polygon[0], polygon[ref_ind]);
    auto [ref_x, ref_y] = polygon[0];
    std::sort(polygon.begin() + 1, polygon.end(), [ref_x, ref_y](std::pair<int, int> a, std::pair<int, int> b) {
        long long ax = a.first - ref_x, ay = a.second - ref_y, bx = b.first - ref_x, by = b.second - ref_y;
        long long d = ay * bx - by * ax;
        if (d == 0) {  // in case a , b, and reference point are colinear
            return ax * ax + ay * ay < bx * bx + by * by;
        }
        return d > 0;
    });
}

std::vector<std::pair<int, int>> getPolygonGridPoints(std::vector<std::pair<int, int>> polygon) {
    const int ninf = -1000000000; // Constant for negative infinity. Only relevant in this function.
    polar_sort(polygon);
    std::vector<std::vector<int>> edges; // an edge is here an array {x1, y1, x2, y2}
    std::vector<std::pair<int, int>> points; // result
    for (int u = 0; u < polygon.size(); ++u) {
        int v = (u + 1) % polygon.size();
        std::vector<int> edge({polygon[u].first, polygon[u].second, polygon[v].first, polygon[v].second});
        if (edge[0] == edge[2]) {    // skip vertical lines
            continue;
        }
        if (polygon[u].first > polygon[v].first) {   // make it so that the first vertex in an edge is always the leftmost one
            std::swap(edge[0], edge[2]);
            std::swap(edge[1], edge[3]);
        }
        edges.push_back(edge);
    }
    std::sort(edges.begin(), edges.end(), [](std::vector<int> a, std::vector<int> b) {
        if (a[0] == b[0]) {
            if (a[2] == b[2]) {
                return std::make_pair(a[1], a[3]) < std::make_pair(b[1], b[3]);
            }
            return a[2] < b[2];
        }
        return a[0] < b[0];
    }); // sort by x coordinate of first (leftmost) vertex of the edge, and then by x coordinate of second vertex, then by y coords. X coordinate matters most in this algorithm.
    std::reverse(edges.begin(), edges.end()); // reverse vector so that the last element is the leftmost (for easier line sweeping algorithm)

    auto getY = [](std::vector<int> seg, int x) {  // given a 2D segment and an X coordinate, return the Y coord. of the only point that has this X coord AND lies on this segment.
        double x1 = seg[0];
        double y1 = seg[1];
        double x2 = seg[2];
        double y2 = seg[3];
        return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
    };
    std::vector<int> edge_a({ninf, ninf, ninf, ninf}), edge_b({ninf, ninf, ninf, ninf});
    for (int x = polygon[0].first;; ++x) { // line sweep algorithm. For each reasonable X coordinate, we find all grid points that have this X coord. and lie inside the polygon
        while (x > edge_a[2]) { // remove outdated edges and find current edge A
            if (edges.empty()) {
                return points;
            }
            edge_a = edges.back();
            edges.pop_back();
        }
        while (x > edge_b[2]) { // remove outdated edges and find current edge B
            if (edges.empty()) {
                return points;
            }
            edge_b = edges.back();
            edges.pop_back();
        }
        double y1 = getY(edge_a, x); // the Y coord. of the only point that lies on edge A and has the X coord. equal to current x
        double y2 = getY(edge_b, x); // same here but for edge B
        if (y2 < y1) {
            std::swap(y1, y2);
        }
        for (int y = std::ceil(y1); y <= std::floor(y2); ++y) { // Since polygon is convex, all of its points with current X are between points (X,y1) and (X,y2)
            points.push_back({x, y});
        }
    }
    return points;
}

int main() {

    std::ofstream out("PolygonPoints/res.txt");

    std::vector<std::pair<int, int>> test_poly({{-250,-166},{0,0},{86,0}});
    polar_sort(test_poly);
    out<<test_poly.size()<<std::endl;
    for (auto [x, y] : test_poly) {
        out << x << " " << y << std::endl;
    }
    std::vector<std::pair<int, int>> pts = getPolygonGridPoints(test_poly);
    for (auto [x, y] : pts) {
        out << x << " " << y << std::endl;
    }
    return 0;
}
