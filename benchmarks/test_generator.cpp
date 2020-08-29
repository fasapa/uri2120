#include <random>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using std::cin, std::cout;

int main(int argc, char *argv[]) {
  int n;
  cin >> n;

  cout << n << "\n";

  std::default_random_engine rng;
  std::uniform_int_distribution<int> dist(1, 1'000'000'000);

  bool order = (argc == 2);

  std::set<int> keys;
  for (int i = 0, x; i < n; i++) {
    x = dist(rng);
    while (keys.find(x) != keys.end())
      x = dist(rng);

    if (!order)
      cout << x << " ";
    keys.insert(x);
  }
  if (order) {
    for (const auto &x : keys) {
      cout << x << " ";
    }
  }
  cout << "\n";

  std::vector<int> queries;
  for (int i = 2; i <= n; i++) {
    queries.push_back(i);
  }
  std::shuffle(queries.begin(), queries.end(), rng);

  // numero de perguntas (queries)
  cout << n-1 << "\n";
  for (const auto &x : queries) {
    cout << x << " ";
  }
  cout << "\n";
  return 0;
}
