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

vector<pair<int,int>> getPoints(vector<pair<int,int>> polygon){ // Returns all grid points (intered coords) inside a convex polygon. OK for some non-convex too but not all
    polar_sort(polygon); 
    vector<pair<pair<int,int>,pair<int,int>>> edges;
    for(int u=0;u<polygon.size();++u){
        int v = (u+1)%polygon.size();
        if(polygon[u].first<=polygon[v].first){   // make it so that the firts vertex in an edge is always the leftmost one
            edges.push_back({polygon[u],polygon[v]});
        }else{
            edges.push_back({polygon[v],polygon[u]});
        }
    }
    sort(edges.begin(),edges.end()); // sort by x coordinate of first (leftmost) vertex of the edge. this also happens to be std's default sorting
    reverse(edges.begin(),edges.end()); // reverse vector so that the last element is the leftmost (for easier line sweeping algorithm)
    pair<pair<int,int>,pair<int,int>> edge_a,edge_b;
    edge_a = edges.back();
    edges.pop_back();
    edge_b = edges.back();
    edges.pop_back();
    vector<pair<int,int>> points;
    auto getY = [](pair<pair<int,int>,pair<int,int>> seg, int x){  // given a 2D segment and an X coordinate, return the point on this segment that has this X coord.
            double x1 = seg.first.first;
            double y1 = seg.first.second;
            double x2 = seg.second.first;
            double y2 = seg.second.second;
            return y1 + (x - x1) * (y2 - y1) / (x2 - x1);
        };
    for(int x = polygon[0].first;;++x){ // line sweep algorithm
        bool stop = false;
        while(x>edge_a.second.first){ // remove outdated edges
            if(edges.empty()){
                stop = true;
                break;
            }
            edge_a = edges.back();
            edges.pop_back();
        }
        while(x>edge_b.second.first){ // remove outdated edges
            if(edges.empty()){
                stop = true;
                break;
            }
            edge_b = edges.back();
            edges.pop_back();
        }
        if(stop){break;}
        bool was_vert = false;
        while(edge_a.first.first == edge_a.second.first){ // handle vertical edges
            was_vert=true;
            int y1 = min(edge_a.first.second,edge_a.second.second), y2 =  max(edge_a.first.second,edge_a.second.second);
            for(int y=y1;y<=y2;++y){
                points.push_back({x,y});
            }
            if(edges.empty()){
                edge_a = {{-1,-1},{-1,-1}};
                break;
            }
            edge_a = edges.back();
            edges.pop_back();
        }
        while(edge_b.first.first == edge_b.second.first){ // handle vertical edges
            was_vert=true;
            int y1 = min(edge_b.first.second,edge_b.second.second), y2 =  max(edge_b.first.second,edge_b.second.second);
            for(int y=y1;y<=y2;++y){
                points.push_back({x,y});
            }
            if(edges.empty()){
                edge_b = {{-1,-1},{-1,-1}};
                break;
            }
            edge_b = edges.back();
            edges.pop_back();
        }
        if(was_vert){ // if vertcal edges were handles in this turn, go to the next turn
            continue;
        }
        if(edge_b.first.first == -1){ // stop if no more edges
            break;
        }
        double y1 = getY(edge_a,x);
        double y2 = getY(edge_b,x);
        if(y2<y1){ swap(y1,y2); }
        for(int y=ceil(y1);y<=floor(y2);++y){ // handle normal edges (ironically, most important part took the least code...)
            points.push_back({x,y});
        }
    }
    return points;
}

int main(){

    vector<pair<int,int>> test_poly({{-4,-4},{-4,4},{2,4},{-1,-2},{-2,6}});//({{2,2},{0,-1},{3,5},{-4,-2},{-4,4},{1,6},{-5,1}});
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
