#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct pos { int x, y; };
struct range { int min, max; };
struct segment { int max, lazy; };

int n, k, power2;
vector<pos> ps;
vector<int> ypx, ymx;

int dist_p(int l, int r) {
	return abs((ps.at(ypx.at(l)).y + ps.at(ypx.at(l)).x) - (ps.at(ypx.at(r)).y + ps.at(ypx.at(r)).x));;
}

int dist_m(int l, int r) {
	return abs((ps.at(ymx.at(l)).y - ps.at(ymx.at(l)).x) - (ps.at(ymx.at(r)).y - ps.at(ymx.at(r)).x));
}

void range_update(vector<segment>& segment_tree, int from, int to, int dx, int node = 1, int node_min = 0, int node_max = n - 1) {
	if (segment_tree[node].lazy != 0) {
		segment_tree[node].max += segment_tree[node].lazy;
		if (node_min != node_max) {
			segment_tree[node * 2].lazy += segment_tree[node].lazy;
			segment_tree[node * 2 + 1].lazy += segment_tree[node].lazy;
		}
		segment_tree[node].lazy = 0;
	}

	if (to < node_min || from > node_max) {
		return;
	}
	else if (from <= node_min && node_max <= to) {
		segment_tree[node].max += dx;
		if (node_min != node_max) {
			segment_tree[node * 2].lazy += dx;
			segment_tree[node * 2 + 1].lazy += dx;
		}
	}
	else {
		int d = (node_min + node_max) / 2;
		
		range_update(segment_tree, from, to, dx, node * 2, node_min, d);
		range_update(segment_tree, from, to, dx, node * 2 + 1, d + 1, node_max);
		segment_tree[node].max = max(segment_tree[node * 2].max, segment_tree[node * 2 + 1].max);
	}
}

int range_max(vector<segment>& segment_tree) {
	return segment_tree[1].max;
}

bool possible(int max_dist) {
	vector<range> groups(n);
	int left = 0, right = 0;
	while (left < n) {
		while (right < n && dist_p(left, right) <= max_dist) {
			groups.at(ypx.at(right)).min = left;
			right++;
		}

		groups.at(ypx.at(left)).max = left;
		left++;
	}

	vector<segment> frequencies(2 * power2, { 0, 0 });
	left = 0, right = 0;
	while (right < n) {
		while (right < n && dist_m(left, right) <= max_dist) {
			range_update(frequencies, groups.at(ymx.at(right)).min, groups.at(ymx.at(right)).max, 1);
			right++;
		}

		if (range_max(frequencies) >= k) return true;

		range_update(frequencies, groups.at(ymx.at(left)).min, groups.at(ymx.at(left)).max, -1);
		left++;
	}

	return false;
}

int solve() {
	int index = 0;
	int jump = 1 << 30;
	while (jump > 0) {
		if (!possible(index + jump)) index += jump;
		jump /= 2;
	}

	return index + 1;
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

		power2 = n - 1;			// Calculates smallest power of 2 greater than n (needed for segment tree)
		power2 |= power2 >> 1;
		power2 |= power2 >> 2;
		power2 |= power2 >> 4;
		power2 |= power2 >> 8;
		power2 |= power2 >> 16;
		power2++;

		std::cout << i << " " << solve() << endl;
	}
}
