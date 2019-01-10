#include <bits/stdc++.h>

using namespace std;

int inf = 1e9;

class MinCostFlow {
  using P = pair<int, int>;
  vector<int> dist, h, prevV, prevE;
public:
  struct Edge {
    int to, cap, cost, rev;
  };
  vector<vector<Edge>> G;
  MinCostFlow(int n) : G(n), dist(n), h(n, 0), prevV(n), prevE(n) {}
  void add(int from, int to, int cap, int cost) {
    if (from < 0 || to < 0) return;
    G[from].push_back({to, cap, cost, (int)G[to].size()});
    G[to].push_back({from, 0, -cost, (int)G[from].size()-1});
  }
  int min_cost_flow(int s, int t, int f, bool is_ford_first = false) {
    int res = 0;
    while (f > 0) {
      dist.assign(G.size(), inf);
      dist[s] = 0;
      if (is_ford_first) {
        while (1) {
          bool is_update = false;
          for (int v = 0; v < G.size(); ++v) {
            if (dist[v] == inf) continue;
            for (int i = 0; i < G[v].size(); ++i) {
              const Edge& e = G[v][i];
              if (e.cap > 0 && dist[v] + e.cost < dist[e.to]) {
                dist[e.to] = dist[v] + e.cost;
                prevV[e.to] = v;
                prevE[e.to] = i;
                is_update = true;
              }
            }
          }
          if (!is_update) break;
        }
        is_ford_first = false;
      }
      else {
        priority_queue<P, vector<P>, greater<P>> Q;
        Q.push({0, s});
        while ( !Q.empty() ) {
          P p = Q.top(); Q.pop();
          int v = p.second;
          if (p.first > dist[v]) continue;
          for (int i = 0; i < G[v].size(); ++i) {
            Edge& e = G[v][i];
            if (e.cap > 0 && dist[v]+e.cost+h[v]-h[e.to] < dist[e.to]) {
              dist[e.to] = dist[v]+e.cost+h[v]-h[e.to];
              prevV[e.to] = v;
              prevE[e.to] = i;
              Q.push({dist[e.to], e.to});
            }
          }
        }
      }
      for (int i = 0; i < G.size(); ++i) {
        h[i] += dist[i];
      }
      if (dist[t] == inf) {
        return -res;
      }
      int d = f;
      for (int v = t; v != s; v = prevV[v]) {
        d = min(d, G[prevV[v]][prevE[v]].cap);
      }
      f -= d;
      res += d * h[t];
      for (int v = t; v != s; v = prevV[v]) {
        Edge& e = G[prevV[v]][prevE[v]];
        e.cap -= d;
        G[e.to][e.rev].cap += d;
      }
    }
    return res;
  }
};

class TSP {
private:
  int n;
  using P = pair<int, int>;
  vector<map<int, int>> G;
  vector<set<P>> S;
public:
  void add_edge(int from, int to, int cost) {
    assert(0 <= from && from < n);
    assert(0 <= to && to < n);
    G[from][to] = cost;
    S[from].insert(P(cost, to));
  }
  TSP(int size) : n(size), G(size), S(size) {}
  int dfs(int v, int cnt, int cost, int ub) {
    if (cnt == n-1) {
      return G[v].count(0) > 0 ? min(ub, cost + G[v][0]) : ub;
    }
    // bounding
    if (ub < inf) {
      int w = 0;
      for (int i = 0; i < n; ++i) {
        if (S[i].size() > 0) {
          int use = i == 0 || i == v ? 1 : 2;
          if (S[i].size() < use) return ub;
          auto it = S[i].begin();
          w += it->first;
          if (use == 2) {
            ++it;
            w += it->first;
          }
        }
      }
      // lb: cost + w/2
      if (2 * cost + w >= 2 * ub) return ub;
      vector<int> left;
      for (int i = 1; i < n; ++i) {
        if (i == v) continue;
        left.push_back(i);
      }
      int V = left.size();
      vector<int> right = left;
      if (left.size() % 2 == 0) {
        right.push_back(v);
      }
      else {
        left.push_back(v);
      }
      left.push_back(0);
      int s = left.size() + right.size(), t = s+1;
      MinCostFlow mcf(t+1);
      for (auto&& i : left) mcf.add(s, i, 1, 0);
      for (auto&& i : right) mcf.add(left.size()+i, t, 1, 0);
      for (auto&& i : left) for (auto&& j : right) {
        if (G[i].count(j) == 0) continue;
        int c = G[i][j];
        if (S[i].count(P(c, j)) == 0) continue;
        mcf.add(i, left.size()+j, 1, c);
      }
      int flow = mcf.min_cost_flow(s, t, (left.size()+right.size()) * 2 - 2);
      if (cost + flow >= ub) return ub;
    }
    // branching
    set<P> edges(move(S[v]));
    S[v] = set<P>();
    int res = inf, invalid_count = 0;
    for (auto&& p : edges) {
      int c, to; tie(c, to) = p;
      S[to].erase(P(c, v));
      if (to == 0 && S[to].size() == 0) return ub;
      if (to != 0 && S[to].size() == 1) {
        ++invalid_count;
      }
    }
    if (invalid_count >= 2) return ub;
    for (auto&& p : edges) {
      int c, to; tie(c, to) = p;
      if (to == 0) continue;
      if (invalid_count == 0 || S[to].size() == 1) {
        ub = dfs(to, cnt+1, cost+c, ub);
      }
    }
    for (auto&& p : edges) {
      int c, to; tie(c, to) = p;
      S[to].insert(P(c, v));
    }
    S[v] = move(edges);
    return ub;
  }
  int solve() {
    assert(n >= 2);
    for (int i = 0; i < n; ++i) {
      if (G[i].size() < 2) return inf;
    }
    int res = inf;
    set<P> edges = S[0];
    for (auto&& p : edges) {
      int c, to; tie(c, to) = p;
      S[0].erase(P(c, to));
      S[to].erase(P(c, 0));
      res = dfs(to, 1, c, res);
      S[0].insert(P(c, to));
      S[to].insert(P(c, 0));
    }
    return res;
  }
};

void show_help(const string& cmd) {
  cout << "usage: " << cmd << " <input>" << endl;
}

TSP input(int argc, char* argv[]) {
  if (argc < 2) {
    show_help(argv[0]);
    exit(1);
  }
  const string arg(argv[1]);
  if (arg == "--help" || arg == "-h") {
    show_help(argv[0]);
    exit(0);
  }
  ifstream ifs(arg);
  int n = 0;
  TSP tsp(0);
  string type;
  while (ifs >> type && type != "EOF") {
    if (type == "DIMENSION:") {
      ifs >> n;
      tsp = TSP(n);
    }
    else if (type == "EDGE_WEIGHT_TYPE:") {
      string value; ifs >> value;
      assert(value == "EXPLICIT");
    }
    else if (type == "EDGE_WEIGHT_FORMAT:") {
      string value; ifs >> value;
      assert(value == "FULL_MATRIX");
    }
    else if (type == "EDGE_WEIGHT_SECTION:") {
      string line;
      getline(ifs, line);
      for (int i = 0; i < n; ++i) {
        assert(getline(ifs, line));
        stringstream ss(line);
        for (int j = 0; j < n; ++j) {
          int cost;
          assert(ss >> cost);
          assert(0 <= cost && cost <= 32767);
          if (cost == 32767) continue;
          tsp.add_edge(i, j, cost);
        }
      }
    }
    else {
      cerr << "Invalid Input: " << type << endl;
      exit(1);
    }
  }
  return tsp;
}

int main(int argc, char* argv[]) {
  TSP tsp = input(argc, argv);
  cout << tsp.solve() << endl;
}
