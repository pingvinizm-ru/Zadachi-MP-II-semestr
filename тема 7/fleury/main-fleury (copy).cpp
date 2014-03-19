#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
/////////////////////////////////////////////////////////////
const int INF = 10005000;
const int BRIDGE = 2;
int N;
int num = 0;
vector<int> visit(8,-1);
vector<int> value(8,INF);
vector< pair<int,int> > result;

void bridges(vector< vector<int> > &m, int prev, int current)
{
    visit[current] = value[current] = num++;
    for (size_t i = 0; i < m.size(); i++)
    {
        if (m[current][i] > 0 && (int)i != prev)
        {
            if (visit[i] == -1)
            {
                bridges(m, current, i);
                if (value[i] < value[current]) value[current] = value[i];
                if (value[i] > visit[current])
                {
                    result.push_back(pair<int,int>(current,i));
                    m[current][i] = m[i][current] = BRIDGE;
                    cout << current+1 << " " << i+1 << endl;
                }
            }
            else
            {
                if (visit[i] < value[current])
                    value[current] = visit[i];
            }
        }
    }
}

void search_bridges(vector< vector<int> > &m)
{
    for (size_t i = 0; i < m.size(); i++)
        if (visit[i] == -1)
            bridges(m, -1, i);
}
///////////////////////////////////////////////////////////////////////

void fleury(vector< vector<int> > &matrix, vector< pair<int,int> > &result, int k, int current)
{
    if (k == 0) return; //прошли все ребра

    for (size_t i = 0; i < matrix.size(); i++)
    {
        if (matrix[current][i] > 0)
        {
            matrix[current][i] = matrix[i][current] = 0;
            result.push_back(pair<int,int>(current,i));
            cout << current+1 << " " << i+1 << endl;
            fleury(matrix,result,k-1,i);
        }
    }
}

void fleury_interface(vector< vector<int> > &matrix, vector< pair<int,int> > &result, int k)
{
    search_bridges(matrix);
    fleury(matrix, result, k , 0);
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

    fleury_interface(matrix, result, E);



    return 0;
}
