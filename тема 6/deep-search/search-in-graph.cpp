#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

//поиск в ширину
vector<int> breadth_first_search(vector< vector<int> > &matrix, int node)
{
    int size = matrix.size();
    bool visit[size];
    queue<int> q;
    vector<int> result;
    for (int i = 0; i < size; i++) visit[i] = false;

    visit[node] = true;
    q.push(node);
    result.push_back(node);

    while (q.empty() == false)
    {
        node = q.front(); q.pop();

        for (int i = 0; i < size; i++)
            if (matrix[node][i] > 0 && visit[i] == false)
            {
                q.push(i);
                result.push_back(i);
                visit[i] = true;
            }
    }

    return result;
}

//поиск в глубину - можно было передавать только node, а все остальное сделать глобальными параметрами
void dfs(vector< vector<int> > &matrix, bool *visit, int node, vector<int> &result)
{
    result.push_back(node);
    visit[node] = true;
    for (size_t i = 0; i < matrix.size(); i++)
        if (matrix[node][i] > 0 && visit[i] == false)
            dfs(matrix, visit, i, result);
}

//поиск в глубину - интерфейс
vector<int> depth_first_search(vector< vector<int> > &matrix, int node)
{
    bool visit[matrix.size()];
    for (size_t i = 0; i < matrix.size(); i++) visit[i] = false;
    vector<int> result;

    dfs(matrix, visit, node, result);

    return result;
}

int main()
{
    ifstream input ("input-search.txt");
    int N,t;
    input >> N; //число вершин графа

    vector< vector<int> > matrix(N);
    vector<int> v;

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

    v = breadth_first_search(matrix, 0);
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << " ";
    cout << endl << endl;

    v = depth_first_search(matrix, 0);
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << " ";
    cout << endl << endl;

}
