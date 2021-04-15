#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>   
#include <sstream>
#include <set>
#include <iterator>

using namespace std;

class Point_of_Graf
{
public:
	Point_of_Graf(int a, unsigned short int b, int c, bool d, int e);
	int to_point;
	unsigned short int flow;
	int number_flow;
	bool is_revers;
	int reversed_number_graf;
};

Point_of_Graf::Point_of_Graf(int a, unsigned short int b, int c, bool d, int e)
{
	to_point = a;
	flow = b;
	number_flow = c;
	is_revers = d;
	reversed_number_graf = e;
}

class Point_of_Flow
{
public:
	Point_of_Flow(int a, int b, unsigned short int c, int d, int e);
	int number_flow;
	int from_point;
	int to_point;
	unsigned short int flow;
	int number_in_vector;
};

Point_of_Flow::Point_of_Flow(int a, int b, unsigned short int c, int d, int e)
{
	from_point = a;
	to_point = b;
	flow = c;
	number_flow = d;
	number_in_vector = e;
}

int main()
{
	int M, N, s, t, c, i, j;
	ifstream input;
	input.open("flow.in");
	string line;
	getline(input, line);
	istringstream istring_1(line);
	vector<string> string_1
	{
		istream_iterator<string>(istring_1), {}
	};
	M = stoi(string_1[0]);
	N = stoi(string_1[1]);
	getline(input, line);
	istringstream istring_2(line);
	vector<string> string_2
	{
		istream_iterator<string>(istring_2), {}
	};
	s = stoi(string_2[0]);
	t = stoi(string_2[1]);
	s--;
	t--;
	vector <vector<Point_of_Graf>> Graf(M);
	vector <int> MaxFlow(N, 0);
	int FlowIn = 0;
	for (int iter = 0; iter < N; iter++)
	{
		getline(input, line);
		istringstream istring(line);
		vector<string> next_string
		{
			istream_iterator<string>(istring), {}
		};
		i = stoi(next_string[0]);
		j = stoi(next_string[1]);
		c = stoi(next_string[2]);
		i--;
		j--;
		Graf[i].push_back(Point_of_Graf(j, c, iter, false, Graf[j].size()));
		Graf[j].push_back(Point_of_Graf(i, 0, iter, true, Graf[i].size() - 1));
	}
	input.close();
	bool end = false;
	while (true)
	{
		int now_point = s;
		int max_flow;
		int next_point_in_graf_vector = 0;
		int next_point = s;
		int min_flow_in_iter = 2000000;
		vector<bool> is_not_visit(M, true);
		is_not_visit[s] = false;
		vector<Point_of_Flow> points;
		while (true)
		{
			max_flow = 0;
			for (int iter = 0; iter < Graf[now_point].size(); iter++)
			{
				if (max_flow < Graf[now_point][iter].flow && is_not_visit[Graf[now_point][iter].to_point])
				{
					max_flow = Graf[now_point][iter].flow;
					next_point_in_graf_vector = iter;
					next_point = Graf[now_point][iter].to_point;
				}
			}
			if (now_point == s && s == next_point)
			{
				end = true;
				break;
			}
			else if (next_point == now_point)
			{
				next_point = now_point = points[points.size() - 1].from_point;
				if (points.size() != 1)
					min_flow_in_iter = points[points.size() - 1].flow;
				else
					min_flow_in_iter = 2000000;
				points.pop_back();
			}
			else
			{
				if (min_flow_in_iter > Graf[now_point][next_point_in_graf_vector].flow)
					min_flow_in_iter = Graf[now_point][next_point_in_graf_vector].flow;
				is_not_visit[next_point] = false;
				points.push_back(Point_of_Flow(now_point, next_point, min_flow_in_iter, Graf[now_point][next_point_in_graf_vector].number_flow, next_point_in_graf_vector));
				now_point = next_point;
				if (now_point == t)
					break;
			}
		}
		if (end)
			break;
		FlowIn += min_flow_in_iter;
		for (int iter = 0; iter < points.size(); iter++)
		{
			int from_point = points[iter].from_point;
			int to_point = points[iter].to_point;
			Graf[from_point][points[iter].number_in_vector].flow -= min_flow_in_iter;
			Graf[to_point][Graf[from_point][points[iter].number_in_vector].reversed_number_graf].flow += min_flow_in_iter;
			if (Graf[from_point][points[iter].number_in_vector].is_revers)
				MaxFlow[points[iter].number_flow] -= min_flow_in_iter;
			else
				MaxFlow[points[iter].number_flow] += min_flow_in_iter;
		}
	}
	ofstream ouput;
	ouput.open("flow.out");
	ouput << FlowIn;
	for (int iter = 0; iter < N; iter++)
	{
		ouput << "\n" << MaxFlow[iter];
	}
	ouput.close();
}