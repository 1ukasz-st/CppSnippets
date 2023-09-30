/* Time complexity: O(nPointsInside + nVerts*log2(nVerts)) */
#include<bits/stdc++.h>

using namespace std;

void polar_sort(vector<pair<int,int>> &polygon){ // polar sort for a group of points 
    int ref_ind = 0;
    for(int i = 1;i<polygon.size();++i){                                // find leftmost vertex (from now on, it's the reference point, from witch the sorting begins)
        if(polygon[i].first < polygon[ref_ind].first){
            ref_ind = i;
        }
    }
    swap(polygon[0],polygon[ref_ind]);
    auto [ref_x,ref_y] = polygon[0];
    sort(polygon.begin()+1,polygon.end(),[ref_x,ref_y](pair<int,int> a, pair<int,int> b){ 
        long long ax = a.first - ref_x, ay = a.second - ref_y, bx = b.first - ref_x, by = b.second - ref_y;
        long long d = ay*bx - by*ax; 
        if(d==0){  // in case a , b and reference point are colinear
          return ax*ax + ay*ay < bx*bx + by*by;
        } 
        return d>0;
    });
}

vector<pair<int,int>> getPoints(vector<pair<int,int>> polygon){ // Returns all grid points (integer coords) inside a convex polygon. OK for some non-convex too but not all
    polar_sort(polygon); 
    vector<vector<int>> edges; // an edge is here an array {x1,y1,x2,y2}
    vector<pair<int,int>> points;
    for(int u=0;u<polygon.size();++u){
        int v = (u+1)%polygon.size();
        vector<int> edge({polygon[u].first,polygon[u].second,polygon[v].first,polygon[v].second});
        if(edge[0]==edge[2]){    // skip vertical lines 
            continue;
        }
        if(polygon[u].first>polygon[v].first){   // make it so that the first vertex in an edge is always the leftmost one
            swap(edge[0],edge[2]);
            swap(edge[1],edge[3]);
        }
        edges.push_back(edge);
    }
    sort(edges.begin(),edges.end(),[](vector<int> a, vector<int> b){
        if(a[0] == b[0]){
            if(a[2]==b[2]){
                return make_pair(a[1],a[3]) < make_pair(b[1],b[3]);
            }
            return a[2] < b[2];
        }
        return a[0] < b[0];
    }); // sort by x coordinate of first (leftmost) vertex of the edge, and then by x coordinate of second vertex, then by y coords.
    reverse(edges.begin(),edges.end()); // reverse vector so that the last element is the leftmost (for easier line sweeping algorithm)
    
    auto getY = [](vector<int> seg, int x){  // given a 2D segment and an X coordinate, return the point on this segment that has this X coord.
            double x1 = seg[0];
            double y1 = seg[1];
            double x2 = seg[2];
            double y2 = seg[3];
            return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
        };
    vector<int> edge_a({-1e9,-1e9,-1e9,-1e9}), edge_b({-1e9,-1e9,-1e9,-1e9});
    for(int x = polygon[0].first;;++x){ // line sweep algorithm
        while(x>edge_a[2]){ // remove outdated edges
            if(edges.empty()){
                return points;
            }
            edge_a = edges.back();
            edges.pop_back();
        }
        while(x>edge_b[2]){ // remove outdated edges
            if(edges.empty()){
                return points;
            }
            edge_b = edges.back();
            edges.pop_back();
        }
        double y1 = getY(edge_a,x);
        double y2 = getY(edge_b,x);
        if(y2<y1){ swap(y1,y2); }
        for(int y=ceil(y1);y<=floor(y2);++y){ 
            points.push_back({x,y});
        }
    }
    return points;
}

int main(){

    vector<pair<int,int>> test_poly({{-4,-4},{-4,4},{4,4},{4,-4}});//({{2,2},{0,-1},{3,5},{-4,-2},{-4,4},{1,6},{-5,1}});
    polar_sort(test_poly);
    for(auto [x,y] : test_poly){
        cout<<x<<" "<<y<<endl;  
    }cout<<endl;
    vector<pair<int,int>> pts = getPoints(test_poly);
    for(auto [x,y] : pts){
        cout<<x<<","<<y<<endl;
    }
    return 0;
}
