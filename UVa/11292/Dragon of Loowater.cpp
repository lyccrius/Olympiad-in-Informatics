#include <bits/stdc++.h>

using namespace std;

const int maxn = 20000 + 10;

int n, m;
int dragon[maxn], knight[maxn];

int main() {
  while (scanf("%d%d", &n, &m) == 2 && n && m) {
    for (int i = 1; i <= n; i++) {
        cin >> dragon[i];
    }

    for (int i = 1; i <= m; i++) {
        cin >> knight[i];
    }

    sort(dragon + 1, dragon + n + 1);
    sort(knight + 1, knight + m + 1);

    int cur = 1;
    int cost = 0;

    for (int i = 1; i <= m; i++) {
      if (knight[i] >= dragon[cur]) {
        cost += knight[i];

        if (++cur > n) {
            break;
        }
      }
    }

    if (cur <= n) {
        cout << "Loowater is doomed!" << endl;
    } else {
        cout << cost << endl;
    }
  }

  return 0;
}