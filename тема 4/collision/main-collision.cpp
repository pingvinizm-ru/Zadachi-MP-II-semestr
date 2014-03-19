#include <iostream>
#include <list>
using namespace std;

class database
{
    int module;
    list<int> *array;

    void copy_database(const database &x)
    {
        module = x.module;
        array = new list<int>[module];
        for (int i = 0; i < module; i++)
            array[i] = x.array[i];
    }

    int hash(int key)
    {
        return key % module;
    }

public:
    database(int m = 10)
    {
        module = m;
        array = new list<int>[module];
    }

    ~database()
    {
        delete[] array;
    }

    database (const database &db)
    {
        copy_database(db);
    }

    database operator= (const database &db)
    {
        if (this == &db) return *this;
        delete [] array;
        copy_database(db);
        return *this;
    }

    void add(int x)
    {
        int h = hash(x);
        list<int>::iterator i;

        for (i = array[h].begin(); i != array[h].end(); i++)
        {
            if ((*i) == x) return;//без дублирующих элементов

            if ((*i) > x)
            {
                array[h].insert(i, x);
                return;
            }
        }
        array[h].push_back(x); //вставка на первое место или в конец списка
    }

    void del(int x)
    {
        int h = hash(x);
        list<int>::iterator i;
        array[h].remove(x);
    }

    void print()
    {
        for(int j = 0; j < module; j++)
        {
            cout << j << ":";
            list<int>::iterator i;
            for (i = array[j].begin(); i != array[j].end(); i++) cout << (*i) << " ";
            cout << endl;
        }
    }

};


int main()
{
    database a;
    int t;

    cout << "Добавление:" << endl;

    for (int i = 0; i < 10; i++)
    {
        cin >> t;
        a.add(t);
        a.print();
    }

    cout << "Удаление:" << endl;
    for (int i = 0; i < 10; i++)
    {
        cin >> t;
        a.del(t);
        a.print();
    }
    return 0;
}
