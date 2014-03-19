#include <iostream>
#include <fstream>
#include <queue>
#define INFINITY 10000000
using namespace std;
int matrix[10][10];

void dijkstra(int **matrix, int count_of_nodes, int node, int *distance, int *way)
{
    //int mark[count_of_nodes];
    bool visit[count_of_nodes];
    int count_of_visited = 0; //число посещенных
    for (int i = 0; i < count_of_nodes; i++)
    {
        distance[i] = INFINITY; //расстояние до любой др.вершины = oo
        visit[i] = false; //ни одна вершина не посещена
        way[i] = INFINITY;
    }

    distance[node] = 0; //посетили стартовую

    //пока посетили не все вершины
    while (count_of_visited < count_of_nodes)
    {
        //нйдем непосещенную вершину с минимальным расстоянием
        int v, min_distance = INFINITY+1;
        for (int i = 0; i < count_of_nodes; i++)
            if ((visit[i] == false) && (distance[i] < min_distance))
            {
                min_distance = distance[i]; //запомнили минимальное расстояние
                v = i; //запомнили номер вершины
            }

        visit[v] = true; //посетили вершину
        count_of_visited++;

        //рассмотрим соседние, и попытаемся уменьшить расстояние в них
        for (int i = 0; i < count_of_nodes; i++)
        {   //если вершина не  посещена, путь ненулевой(т.е. существует), новое расстояние меньше старого
            if (visit[i] == false && matrix[v][i] && (distance[v] + matrix[v][i] < distance[i]))
            {
                distance[i] = distance[v] + matrix[v][i];
                way[i] = v;
            }

        }

        /*
        for (int i = 0; i < count_of_nodes; i++)
        if (distance[i] == INFINITY) cout << "I "; else cout << distance[i] << " ";

        cout << endl;

        for (int i = 0; i < count_of_nodes; i++)
            cout << visit[i] << " ";

        cout << endl;
        */
    }
}



int main()
{
    ifstream input ("input-graph-p1.txt");
    int t1, t2;
    int N;
    input >> N; //число вершин графа

    int **matrix;
    matrix = new int*[N];
    for (int i = 0; i < N; i++) matrix[i] = new int[N];

    int distance[N]; //расстояния
    int way[N];
    //считываем слова
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            input >> matrix[i][j];
            cout << matrix[i][j];
        }
        cout << endl;
    }
    cout << endl;
    input.close();

    //cin >> t1;
    t1 = 0;
    dijkstra(matrix, N, t1, distance, way); //ищем расстояния от нулевой вершины

    for (int i = 0; i < N; i++)
    if (distance[i] == INFINITY) cout << "I "; else cout << distance[i] << " ";

    cout << endl;

    for (int i = 0; i < N; i++)
    if (distance[i] == INFINITY) cout << "I "; else cout << distance[i] << " ";

    //for (int i = 0; i < N; i++) way[i] = 0;

    //cout << "Hello world!" << endl;
    return 0;
}
