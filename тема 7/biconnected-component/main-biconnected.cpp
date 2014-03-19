#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

void search_cutnode(int, int);

/////////////////////////////////////////////////////////////////////
//можно объединить в класс и сделать методы статическими
const int INF = 100000000;

int N;
int num = 0;
int n_component = 0;
vector< vector<int> > matrix;
vector<int> visit(8,-1);
vector<int> value(8,INF);
vector<pair<int,int> > stack2;
set<int> cutnode;
vector< set<int> > component;
//интерфейс
set<int> sc(vector< vector<int> > &M, vector< set<int> > &C)
{
    matrix = M;
    component = C;

    search_cutnode(-1,0);
    //cout << "----" << endl;
    //for (vector<pair<int,int> >::iterator z = stack2.begin(); z != stack2.end(); z++)
    //                  cout << "   " << (*z).first+1 << " " << (*z).second+1  << endl;
    //занесем последнюю двусвязную компоненту графа
    while (stack2.empty() == false)
    {
        pair<int,int> t = stack2.back();
        stack2.pop_back();
                        //
        component[n_component].insert(t.first);
        component[n_component].insert(t.second);
    }
    n_component++;

    C = component;
    return cutnode;
}

//некий синтез алгоритма поиска точек сочленения и двусвязных компонент
void search_cutnode(int prev, int current)
{
    int children = 0; //число детей - для учета случая корня
    visit[current] = value[current] = num++;

    for (int i = 0; i < N; i++) //переберем все возможные пути
    {
        if (matrix[current][i] > 0 && i != prev) //путь существует и не является обратным
        {
            if (visit[i] == -1) //вершина не посещена
            {
                stack2.push_back(pair<int,int>(current,i)); //занесем ребро в стек (т.е. мы переходим по нему)
                search_cutnode(current, i); //запускаем поиск из новой точки
                if (value[i] < value[current]) value[current] = value[i];
                if (value[i] >= visit[current] && prev != -1) //текущая вершина - точка сочленения
                {
                    cutnode.insert(current); //добавляем в множество

                    //отладка
                    //cout << "   c=" << current+1 << "i="<< i+1<< endl;
                    //for (vector<pair<int,int> >::iterator z = stack2.begin(); z != stack2.end(); z++)
                    //   cout << "   " << (*z).first+1 << " " << (*z).second+1  << endl;

                    //извлекаем из стека и добавляем в текущую компоненту
                    pair<int,int> t;
                    do
                    {
                        t = stack2.back();
                        stack2.pop_back();
                        //
                        component[n_component].insert(t.first);
                        component[n_component].insert(t.second);
                    }
                    while (t != pair<int,int>(current,i));

                    n_component++; //увеличиваем чило компонент
                }
                children++;
            }
            else //вершина посещена
            {
                if (visit[i] < visit[current] && i != prev)
                {
                    if (visit[i] < value[current])
                        value[current] = visit[i];
                }
            }
        }
    }

    if (prev == -1 && children > 1) //случай, когда "корень" - точка сочленения
    {
        cutnode.insert(current);
        //for (vector<pair<int,int> >::iterator z = stack2.begin(); z != stack2.end(); z++)
        //              cout << "   " << (*z).first+1 << " " << (*z).second+1  << endl;
        //извлекаем из стека и добавляем в текущую компоненту
        pair<int,int> t;
        do
        {
            t = stack2.back();
            stack2.pop_back();
            //
            component[n_component].insert(t.first);
            component[n_component].insert(t.second);
        }
        while (t.first != current);

        n_component++;
    }
}


/////////////////////////////////////////////////////////////////////
int main()
{
    ifstream input ("input-biconnected.txt");
    int t;
    input >> N; //число вершин графа
    vector< vector<int> > M(N);
    vector<int> q;
    set<int> c;
    vector< set<int> > bi_component(N);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            input >> t;
            M[i].push_back(t);
            cout << M[i][j];
        }
        cout << endl;
    }
    cout << endl;
    input.close();

    c = sc(M, bi_component);


    //отладка поиска точек сочленения
    /*
    for (size_t i = 0; i < matrix.size(); i++)
        cout << i+1 << " ";

    cout << endl;

    for (size_t i = 0; i < matrix.size(); i++)
        if (value[i] < INF) cout << visit[i] << " "; else cout << "N ";

    cout << endl;

    for (size_t i = 0; i < matrix.size(); i++)
        if (value[i] < INF) cout << value[i] << " "; else cout << "I ";

    cout << endl;
    */
    cout << "Точки сочленения:" << endl;
    for (set<int>::iterator i = c.begin(); i != c.end(); i++)
    {
        cout << (*i)+1 << endl;
    }

    cout << "Двусвязные компоненты:" << endl;
    for (int i = 0; i < n_component; i++)
    {
        for (set<int>::iterator j = bi_component[i].begin(); j != bi_component[i].end(); j++)
        {
            cout << (*j)+1 << " ";
        }
        cout << endl;

    }
    return 0;
}
