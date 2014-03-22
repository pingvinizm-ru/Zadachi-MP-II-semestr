#include <iostream>
#include "m-tree3.h"
using namespace std;

int main()
{
    m_tree a, b, c;
    string t;
    cout << "добавление в дерево 'a':" << endl;
    for (int i = 0; i < 3; i++)
    {
        cin >> t;
        a.add(t);
        cout << endl;
        a.print_tree();
        cout << endl;
    }

    cout << "b = c = a = a" << endl;
    b = c = a = a;
    cout << "b:" << endl;
    b.print_tree();
    cout << endl;

    cout << "удаление из дерева 'a':" << endl;
    for (int i = 0; i < 5; i++)
    {
        cin >> t;
        cout << a.del(t);
        cout << endl;
        a.print_tree();
        cout << endl;
    }

    cout << "поиск в дереве 'a':" << endl;
    for (int i = 0; i < 5; i++)
    {
        cin >> t;
        cout << a.search(t);
        cout << endl;
    }



    cout << "Тестирование завершено" << endl;
}
