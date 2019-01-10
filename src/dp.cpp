#include <bits/stdc++.h>

using namespace std;

int inf = 1e9;

class TSP {
public:
  struct Edge {
    int to, cost;
  };
private:
  int n;
  vector<vector<Edge>> G;
public:
  void add_edge(int from, int to, int cost) {
    assert(0 <= from && from < n);
    assert(0 <= to && to < n);
    G[from].push_back({to, cost});
  }
  TSP(int size) : n(size), G(size) {}
  int solve() {
    assert(2 <= n && n <= 30);
    vector<vector<int>> dp(1<<n-1, vector<int>(n-1, inf));
    for (auto&& e : G[0]) {
      if (e.to == 0) continue;
      dp[1<<e.to-1][e.to-1] = e.cost;
    }
    for (int i = 0; i < (1<<n-1); ++i) {
      for (int j = 0; j < n-1; ++j) {
        if (dp[i][j] == inf) continue;
        for (auto&& e : G[j+1]) {
          if (e.to == 0) continue;
          if (i>>e.to-1 & 1) continue;
          int ni = i|1<<e.to-1;
          dp[ni][e.to-1] = min(dp[ni][e.to-1], dp[i][j]+e.cost);
        }
      }
    }
    int ans = inf;
    for (auto&& e : G[0]) {
      ans = min(ans, dp[(1<<n-1)-1][e.to-1] + e.cost);
    }
    return ans;
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
