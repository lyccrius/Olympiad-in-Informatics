#include <iostream>
#include <deque>
#include <set>

void promote() {
	std::ios::sync_with_stdio(0);
	std::cin.tie(0);
	std::cout.tie(0);
	return;
}

typedef std::deque<int> diq;
typedef std::set<int> sit;

const int maxN = 2e5;

int t;
int n, k;
int x, y;

namespace Lyccrius {
	struct Neuron {
		sit sociality;
		sit obsession;
	} neuron[maxN + 10];

	void clear() {
		for (int i = 1; i <= n; i++) neuron[i].sociality.clear();
		for (int i = 1; i <= n; i++) neuron[i].obsession.clear();
		return;
	}

	void addSociality(int tail, int head) {
		neuron[tail].sociality.insert(head);
		return;
	}

	void RemovingLeaves() {
		clear();
		std::cin >> n >> k;
		for (int i = 1; i < n; i++) {
			std::cin >> x >> y;
			addSociality(x, y);
			addSociality(y, x);
		}
		if (n == 2) {
			std::cout << 1 << '\n';
			return;
		}
		int ans = 0;
		int cnt;
		diq que;
		sit del;
		for (int u = 1; u <= n; u++) if (neuron[u].sociality.size() == 1) neuron[*neuron[u].sociality.begin()].obsession.insert(u);
		for (int u = 1; u <= n; u++) if (neuron[u].obsession.size() >= k) que.push_back(u);
		while (!que.empty()) {
			int u = que.front();
			que.pop_front();
			del.clear();
			ans += neuron[u].obsession.size() / k;
			cnt = neuron[u].obsession.size() / k * k;
			for (auto v : neuron[u].obsession) {
				if (cnt == 0) break;
				if (neuron[v].sociality.size() == 1) {
					cnt--;
					neuron[v].sociality.clear();
					neuron[v].obsession.clear();
					del.insert(v);
				}
			}
			for (auto v : del) {
				neuron[u].sociality.erase(v);
				neuron[u].obsession.erase(v);
			}
			if (neuron[u].sociality.size() == 1) {
				int v = *neuron[u].sociality.begin();
				neuron[v].obsession.insert(u);
				if (neuron[v].obsession.size() == k) {
					que.push_back(v);
				}
			}
		}
		std::cout << ans << '\n';
		return;
	}
}

int main() {
	promote();
	std::cin >> t;
	while (t--) Lyccrius::RemovingLeaves();
	return 0;
}