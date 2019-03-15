#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

struct pos { int x, y; };

int n, k;
vector<pos> ps;
vector<int> ypx, ymx;

int dist(int l, int r) {
	return abs(ps.at(l).x - ps.at(r).x) + abs(ps.at(l).y - ps.at(r).y);
}

int dist_p(int l, int r) {
	return abs((ps.at(ypx.at(l)).y + ps.at(ypx.at(l)).x) - (ps.at(ypx.at(r)).y + ps.at(ypx.at(r)).x));;
}

int dist_m(int l, int r) {
	return abs((ps.at(ymx.at(l)).y - ps.at(ymx.at(l)).x) - (ps.at(ymx.at(r)).y - ps.at(ymx.at(r)).x));
}

bool possible(int max_dist) {
	for (int a = 0; a < n; a++) {
		vector<bool> group(n, false);
		for (int b = a; b < n && dist_p(a, b) <= max_dist; b++) {
			group.at(ypx.at(b)) = true;
		}

		int max = 0, count = 0;
		int left = 0, right = 0;
		while (right < n) {
			while (right < n && dist_m(left, right) <= max_dist) {
				count += group.at(ymx.at(right));
				right++;
			}

			max = std::max(max, count);
				
			count -= group.at(ymx.at(left));
			left++;
		}

		if (max >= k) return true;
	}

	return false;
}

int solve() {
	set<int> edges_set;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			edges_set.insert(dist(i, j));
		}
	}

	vector<int> edges(edges_set.begin(), edges_set.end());

	int index = -1;
	int jump = n * n;
	while (jump > 0) {		// Binary search on length of edges
		while (index + jump < edges.size() && !possible(edges.at(index + jump))) {
			index += jump;
		}
		jump /= 2;
	}

	return edges.at(index + 1);
}

int main() {
	int nr;
	cin >> nr;

	for (int i = 1; i <= nr; i++) {
		cin >> n >> k;

		ps = vector<pos>(n);
		for (int j = 0; j < n; j++) cin >> ps.at(j).x >> ps.at(j).y;
		
		ypx.clear();
		ymx.clear();
		for (int j = 0; j < n; j++) ypx.push_back(j);
		for (int j = 0; j < n; j++) ymx.push_back(j);

		sort(ypx.begin(), ypx.end(), [](int l, int r) {
			return ps.at(l).y + ps.at(l).x < ps.at(r).y + ps.at(r).x;
		});

		sort(ymx.begin(), ymx.end(), [](int l, int r) {
			return ps.at(l).y - ps.at(l).x < ps.at(r).y - ps.at(r).x;
		});

		cout << i << " " << solve() << endl;
	}
}
