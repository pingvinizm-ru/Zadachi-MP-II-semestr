#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

const int INF = 100000000;

struct edge //ребро
{
    int weight; //вес ребра
    int node1; //начальная и конечная вершины
    int node2; //
};
bool operator < (const edge &a, const edge &b)
{
  return a.weight > b.weight;
}

//O(V^2)
vector<edge> prim(vector< vector<int> > &matrix)
{
    vector<edge> result;
    vector<int> parent(matrix.size(), -1);
    vector<int> dist(matrix.size(), INF);

    int node = 0;
    dist[node] = -1; //включили нулевую вершину в остов - исключили из очереди

    for (size_t k = 0; k < matrix.size()-1; k++)
    {
        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (dist[i] !=-1 && matrix[node][i] > 0 && matrix[node][i] < dist[i])
            {
                dist[i] = matrix[node][i];
                parent[i] = node;
            }
        }

        int min_weight = INF;
        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (dist[i] != -1 && dist[i] < min_weight)
            {
                node = i;
                min_weight = dist[i];
            }
        }
        dist[node] = -1;

        edge t;
        t.weight = matrix[parent[node]][node];
        t.node1 = parent[node];
        t.node2 = node;
        result.push_back(t);
    }
    return result;
}



vector<edge> kruskal(vector< vector<int> > &matrix)
{
    priority_queue<edge, vector<edge>, less<edge> > edges; //список ребер графа
    vector<edge> result; //список ребер остовного дерева
    vector<int> sets; //вектор соответствия между вершинами и множествами

    //построим список ребер графа
    for (size_t i = 0; i < matrix.size(); i++)
        for (size_t j = i; j < matrix.size(); j++)
            if (matrix[i][j] > 0)
            {
                edge t;
                t.weight = matrix[i][j];
                t.node1 = i;
                t.node2 = j;
                edges.push(t);
            }

    for (size_t i = 0; i < matrix.size(); i++)
        sets.push_back(i); //каждая вершина в своем множестве

    size_t count = matrix.size();
    while (count > 1)
    {
        edge t;
        t = edges.top(); edges.pop(); //извлекли ребро
        if (sets[t.node1] != sets[t.node2]) //вершины из разных множеств
        {
            //объединяем множества
            int old_set = sets[t.node1];
            for (size_t i = 0; i < matrix.size(); i++)
                if (sets[i] == old_set) sets[i] = sets[t.node2];

            result.push_back(t);
            count--;
        }
    }

    return result;
}

int main()
{
    ifstream input ("input-span.txt"); //пример из википедии
    int N,t;
    input >> N; //число вершин графа

    vector< vector<int> > matrix(N);
    vector<edge> v;
    vector< pair<int,int> > edges; //ребра графа

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            input >> t;
            matrix[i].push_back(t);
            cout << matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
    input.close();

    v = kruskal(matrix);
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << "вес:" << v[i].weight << "; из вершины " << v[i].node1 << " в вершину " << v[i].node2 << endl;
    }

    cout << endl << endl;
    v = prim(matrix);
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << "вес:" << v[i].weight << "; из вершины " << v[i].node1 << " в вершину " << v[i].node2 << endl;
    }

    return 0;
}
