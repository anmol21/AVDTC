#include <bits/stdc++.h>

#define pb push_back
#define F first
#define S second

using namespace std;



struct node {
  int col = -1;
  set<int> adj_edges, adj_nodes;
  set<int> inc_colors;

};

struct edge {
  int col = -1;
  int u,v; // The two vertices on which this edge is incident

};

vector<node> nodes;
vector<edge> edges;

bool check_node_violation(int idx) {
  // Adjacent Nodes should not have the same color
  for (auto it:nodes[idx].adj_nodes) {
    //cerr << idx << " " << nodes[idx].col << " " << it << " " << nodes[it].col << endl;
    if (nodes[idx].col!=-1 && nodes[idx].col==nodes[it].col)
      return true;
  }
  // Node and Adjacent Edge should not have the same color
  for (auto it:nodes[idx].adj_edges) {
    //cerr << "YO " <<  idx << " " << nodes[idx].col << " " << it << " " << nodes[it].col << endl;
    if (nodes[idx].col!=-1 && nodes[idx].col==edges[it].col)
      return true;
  }
  set<int> edge_cols;
  for (auto it:nodes[idx].adj_edges) {
    if(edges[it].col !=-1) {
      if (edge_cols.count(edges[it].col))
        return true;
      else
       edge_cols.insert(edges[it].col);
    }
  }
  // Checking AVD condition

  if (nodes[idx].inc_colors.size() == nodes[idx].adj_edges.size() + 1) {
    for (auto it:nodes[idx].adj_nodes) {
      if (nodes[it].inc_colors.size() == nodes[it].adj_edges.size() + 1) {
        if (nodes[idx].inc_colors == nodes[it].inc_colors)
          return true; // AVD condition violated
      }
    }
  }
  return false;
}

bool check_edge_violation(int idx) {
  return check_node_violation(edges[idx].u)|check_node_violation(edges[idx].v);
}





vector<vector<int>> g;
vector< pair<int,int>> order; /* order in which the vertices are being colored
                                 F indicates v/e, S indicates idx */

int n,m,k;

map< pair<int,int>, int > edge_to_idx;


long long cnt = 0;

void print_solution() {
  if (cnt>=1)
    return;
  cnt++;
  // for (auto it:nodes[1].inc_colors)
  //    cerr << it << " ";
  // cerr << endl;
  // for (auto it:nodes[10].inc_colors)
  //    cerr << it << " ";
  cerr << endl;
  cout << "***************************Coloring " << cnt << "***********************************************" << endl;
  cout << "*****************************************************************************" << endl;
  cout << "Vertex Colors: " << endl;
  for (int i=1; i<=n; i++)
    cout << nodes[i].col << " ";
  cout << endl;
  cout << "Edge Colors: " << endl;
  for (int i=1; i<=m; i++) {
    cout << "(" << edges[i].u << "," << edges[i].v << ") is" << edges[i].col << endl;
  }
  cout << "*****************************************************************************" << endl;
  cout << "*****************************************************************************" << endl;
}

void color_algo(int pos) {
  // cout << pos << endl;
  if (pos >= order.size()) {
    print_solution();
    return;
  }
  int cat = order[pos].F, idx = order[pos].S;
  for (int col=1; col<=k; col++) {
    if (cat == 1) {
      nodes[idx].col = col;
      if (!check_node_violation(idx)) {
        nodes[idx].inc_colors.insert(col);
        color_algo(pos+1);
        nodes[idx].inc_colors.erase(col);
      }
      nodes[idx].col = -1;
    } else {
      int u = edges[idx].u, v = edges[idx].v;
      edges[idx].col = col;
      if(!check_edge_violation(idx)) {
        nodes[u].inc_colors.insert(col);
        nodes[v].inc_colors.insert(col);
        if(!check_edge_violation(idx)) {
            color_algo(pos+1);
        }
        nodes[u].inc_colors.erase(col);
        nodes[v].inc_colors.erase(col);
      }
      edges[idx].col = -1;
    }
  }
}

int main() {
  cout << "Enter number of vertices in your graph: ";
  cin >> n;
  cout << "Enter number of edges in your graph: ";
  cin >> m;
  cout << "Enter number of colors: ";
  cin >> k;
  cout << "Enter all edges in your graph: " << endl;
  nodes.resize(n+1); // Keeping Things 1 Based
  g.resize(n+1);
  edges.resize(m+1); // Keeping Things 1 Based
  for (int i=1; i<=m; i++) {
    int u,v;
    cin >> u >> v;
    edges[i].u = u;
    edges[i].v = v;
    nodes[u].adj_nodes.insert(v);
    nodes[u].adj_edges.insert(i);
    nodes[v].adj_nodes.insert(u);
    nodes[v].adj_edges.insert(i);
    edge_to_idx[{u,v}] = i;
    edge_to_idx[{v,u}] = i;
    g[u].pb(v);
    g[v].pb(u);
  }
  vector<int> vis(n+1,0),vis_edge(m+1,0);
  order.pb({1,1});
  vis[1] = 1;
  queue<int> Q;
  Q.push(1);
  while(!Q.empty()) {
    int x = Q.front();
    Q.pop();
    for (auto it:g[x]) {
      if(vis[it] == 1) {
        if (vis_edge[edge_to_idx[{it,x}]] == 0) {
          order.pb({2,edge_to_idx[{it,x}]});
          vis_edge[edge_to_idx[{it,x}]] = 1;
        }
      } else {
        order.pb({2,edge_to_idx[{it,x}]});
        vis_edge[edge_to_idx[{it,x}]] = 1;
        order.pb({1,it});
        vis[it] = 1;
        Q.push(it);
      }
    }
  }
  //sort(order.begin(), order.end());
  color_algo(0);
  return 0;
}
