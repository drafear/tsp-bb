#include <bits/stdc++.h>

using namespace std;

int inf = 1e9;

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
