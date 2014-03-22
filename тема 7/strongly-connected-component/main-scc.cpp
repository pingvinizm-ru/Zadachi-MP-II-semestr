#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

//поиск в глубину
void dfs(vector< vector<int> > &matrix, vector<bool> &visit, int node, vector<int> &result)
{
    visit[node] = true;
    for (size_t i = 0; i < matrix.size(); i++)
        if (matrix[node][i] > 0 && visit[i] == false)
            dfs(matrix, visit, i, result);

    result.push_back(node);
}

vector< set<int> > search_scc(vector< vector<int> > &matrix)
{
    vector<bool> visit(matrix.size(),false); //индикатор посещения вершин
    vector< vector<int> > m = matrix;
    vector<int> v;
    vector<int> priority;
    vector< set<int> > result;
    set<int> c;
    //выполним поиск в глубину - зададим приоритет посещения вершин графа
    for (size_t i = 0; i < m.size(); i++)
        if (visit[i] == false)
            dfs(m, visit, i, priority);

    //создадим новый граф с помощью обращения дуг
    for (size_t i = 0; i < m.size(); i++)
        for (size_t j = i; j < m.size(); j++)
            swap(m[i][j], m[j][i]);

    int node;
    visit.assign(m.size(), false); //ни одна вершина не посещена

    for (size_t i = 0; i < m.size(); i++)
    {
        node = priority.back(); priority.pop_back(); //выбираем вершину с наибольшим приоритетом
        if (visit[node] == false) //еще не посещали её
        {
            dfs(m, visit, node, v); //запускаем поиск

            for (size_t i = 0; i < v.size(); i++) c.insert(v[i]); //занесем в множество
            result.push_back(c); //добавим в результат
            c.clear();
            v.clear();
        }
    }

    return result;
}

int main()
{
    ifstream input ("input-scc.txt");
    int N,t;
    input >> N; //число вершин графа
    vector< vector<int> > matrix(N);
    vector<int> v;
    vector< set<int> > scc;
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

    scc = search_scc(matrix);

    for (size_t i = 0; i < scc.size(); i++)
    {
        cout << i+1 << ":";
        for (set<int>::iterator j = scc[i].begin(); j != scc[i].end(); j++)
        {
            cout << (*j)+1 << " ";
        }
        cout << endl;
    }
    return 0;
}
