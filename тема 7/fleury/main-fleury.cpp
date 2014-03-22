#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
/////////////////////////////////////////////////////////////
const int INF = 100500;
void fleury_interface(vector< vector<int> > &, vector< pair<int,int> > &);
void fleury(vector< vector<int> >&, vector< pair<int,int> > &, int, int);
bool edge_is_available(vector< vector<int> > &, int, int);
/*
Для большей эффективности граф можно было бы хранить не как матрицу смжности, а как
массив списоков смежных вершин, что ускоряло бы перебор возможных ребер, но замедляло удаление.
*/
///////////////////////////////////////////////////////////////////////
int dfs(vector< vector<int> > &matrix,  vector<bool> &visit, int current)
{
    visit[current] = true;
    int count = 1;

    for (size_t i = 0; i < matrix.size(); i++)
        if (matrix[current][i] > 0 && visit[i] == false)
            count += dfs(matrix, visit, i);

    return count;
}

void fleury(vector< vector<int> > &matrix, vector< pair<int,int> > &result, int current)
{
    for (size_t i = 0; i < matrix.size(); i++)
    {
        if (matrix[current][i] > 0 && edge_is_available(matrix, current, i))
        {
            matrix[current][i] = matrix[i][current] = 0; //удалим ребро
            result.push_back(pair<int,int>(current,i));
            cout << current << " " << i << endl;
            fleury(matrix,result,i);
        }
    }
}
//доступно ли ребро для перехода
bool edge_is_available(vector< vector<int> > &matrix, int current, int next)
{
    //подсчитаем число смежных вершин
    int count = 0;
    for (size_t i = 0; i < matrix.size(); i++)
        if (matrix[current][i] > 0)count++;

    if (count == 1) return true; //путь единственный - идем по нему

    vector<bool> visit(matrix.size(), false);
    int before_remove = dfs(matrix, visit, current); //подсчитаем число доступных вершин

    matrix[current][next] = matrix[next][current] = 0; //удалим ребро

    visit.assign(matrix.size(), false);
    int after_remove = dfs(matrix, visit, current); //подсчитаем число доступных вершин после удаления

    matrix[current][next] = matrix[next][current] = 1; //вернем ребро

    if (before_remove > after_remove)
        return false;
    else
        return true;
}

void fleury_interface(vector< vector<int> > &matrix, vector< pair<int,int> > &result)
{
    fleury(matrix, result, 3);
}

int main()
{
    ifstream input("input-fleury.txt");
    int V, E; //число вершин ребер графа
    input >> V >> E;
    vector< vector<int> > matrix(V);
    vector< pair<int,int> > result;

    //заполним матрицу смежности нулями
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            matrix[i].push_back(0);

    //занесем данные из списка ребер
    for (int i = 0; i < E; i++)
    {
        int t1, t2;
        input >> t1 >> t2;
        matrix[t1][t2] = matrix[t2][t1] = 1;
    }

    for (int i = 0; i < V; i++)
    {
        int t = 0;
        for (int j = 0; j < V; j++)
            t+=matrix[i][j];

        if (t % 2)
        {
            cerr << "Граф не является эйлеровым" << endl;
            return 1;
        }
    }

    fleury_interface(matrix, result);

    return 0;
}
