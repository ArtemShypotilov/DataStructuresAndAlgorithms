#define _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include <iterator> 
#include <map> 
#include <string>
#include <vector>
#include <algorithm>

#define MAXN 100
#define MAX_L 31


std::map<std::string, int> countries_id;
std::vector<std::string> id_to_country;

std::vector<int> adjacent[MAXN];

std::vector<std::string> cut_points;

bool used[MAXN];
int timer, tin[MAXN], fup[MAXN];



void dfs(int v, int p = -1) {
	used[v] = true;
	tin[v] = fup[v] = timer++;
	int children = 0;
	for (size_t i = 0; i < adjacent[v].size(); ++i) {
		int to = adjacent[v][i];
		if (to == p)  continue;
		if (used[to])
			fup[v] = std::min(fup[v], tin[to]);
		else {
			dfs(to, v);
			fup[v] = std::min(fup[v], fup[to]);
			if (fup[to] >= tin[v] && p != -1)
				cut_points.push_back(id_to_country[v]);
			++children;
		}
	}
	if (p == -1 && children > 1)
		cut_points.push_back(id_to_country[v]);
}


int main()
{
	int N;
	int TC = 1;
	setbuf(stdout, NULL);
	freopen("sample.txt", "r", stdin);

	while (1)
	{
		countries_id.clear();
		id_to_country.clear();

		std::cin >> N;

		if (N == 0)
			return 0;

		for (int i = 0; i < N; i++) {
			std::string str;
			std::cin >> str;
			countries_id.insert(std::pair<std::string, int>(str, i));
			id_to_country.push_back(str);

		}
		std::cin >> N;
		for (int i = 0; i < N; i++) {
			std::string str1;
			std::string str2;
			std::cin >> str1;
			std::cin >> str2;
			adjacent[countries_id.find(str1)->second].push_back(countries_id.find(str2)->second);
			adjacent[countries_id.find(str2)->second].push_back(countries_id.find(str1)->second);
		}

		timer = 0;
		for (unsigned int i = 0; i < countries_id.size(); ++i)
			used[i] = false;

		dfs(0);

		std::cout << "City map #" << TC <<": "<< cut_points.size() << " camera(s) found" << std::endl;
		std::sort(cut_points.begin(), cut_points.end());
		cut_points.erase(std::unique(cut_points.begin(), cut_points.end()), cut_points.end());
		for (unsigned int i = 0; i < cut_points.size(); i++)
			std::cout << cut_points[i] << std::endl;
		std::cout << std::endl;

		cut_points.clear();
		for (int i = 0; i < N; i++) {
			adjacent[i].clear();
		}

		TC++;

	}





	return 0;
}
