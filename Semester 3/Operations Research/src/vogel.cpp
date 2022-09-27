#include <iostream>
#include <numeric>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &outStream, const std::vector<T> v) {
	auto iterator = v.cbegin();
	auto end = v.cend();

	outStream << '[';
	if (iterator != end) {
		outStream << *iterator;
		iterator = std::next(iterator);
	}

	while (iterator != end) {
		outStream << ", " << *iterator;
		iterator = std::next(iterator);
	}

	return outStream << ']';
}

std::vector<int> demand = {800, 600, 1100};
std::vector<int> supply = {400, 400, 500, 400, 800};
std::vector<std::vector<int>> costs = {
	{5, 4, 8},
	{8, 7, 4},
	{6, 7, 6},
	{6, 6, 6},
	{3, 5, 4}
};

int rowCount = supply.size();
int columnCount = demand.size();

std::vector<bool> rowsDone(rowCount, false);
std::vector<bool> columnsDone(columnCount, false);
std::vector<std::vector<int>> result(rowCount, std::vector<int>(columnCount, 0));

std::vector<int> difference(int index, int length, bool isRow) {
	int min1 = INT_MAX;
	int min2 = INT_MAX;
	int minP = -1;

	for (int i = 0; i < length; i++) {
		if (isRow ? columnsDone[i] : rowsDone[i]) {
			continue;
		}

		int cost = isRow ? costs[index][i] : costs[i][index];
		if (cost < min1) {
			min2 = min1;
			min1 = cost;
			minP = i;
		}
		else if (cost < min2) {
			min2 = cost;
		}
	}

	return {min2 - min1, min1, minP};
}

std::vector<int> maxPenality(int length1, int length2, bool isRow) {
	int md = INT_MIN;
	int pc = -1;
	int pm = -1;
	int mc = -1;

	for (int i = 0; i < length1; ++i)
	{
		if (isRow ? rowsDone[i] : columnsDone[i]) {
			continue;
		}

		std::vector<int> res = difference(i, length2, isRow);
		if(res[0] > md) {
			md = res[0];
			pm = i;
			mc = res[1];
			pc = res[2];
		}
	}

	return isRow ? std::vector<int> {pm, pc, mc, md} : std::vector<int> {pc, pm, mc, md};
}

std::vector<int> nextCell() {
	auto res1 = maxPenality(rowCount, columnCount, true);
	auto res2 = maxPenality(columnCount, rowCount, false);

	if (res1[3] == res2[3]) {
		return res1[2] < res2[2] ? res1 : res2;
	}

	return res1[3] > res2[3] ? res2 : res1;
}

int main(int argc, char const *argv[]) {
	int supplyLeft = std::accumulate(supply.cbegin(), supply.cend(), 0, [](int a, int b) { return a + b; });
	int totalCost = 0;

	while (supplyLeft > 0) {
		auto cell = nextCell();
		int r = cell[0];
		int c = cell[1];

		int quantity = std::min(demand[c], supply[r]);

		demand[c] -= quantity;
		if (demand[c] == 0) {
			columnsDone[c] = true;
		}

		supply[r] -= quantity;
		if (supply[r] == 0) {
			rowsDone[r] = true;
		}

		result[r][c] = quantity;
		supplyLeft -= quantity;

		totalCost += quantity * costs[r][c];
	}

	for (auto &a : result)
	{
		std::cout << a << '\n';
	}

	std::cout << "Total cost:" << totalCost;

	return 0;
}
