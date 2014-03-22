#include <iostream>
#include <cstdlib>
#include <iomanip>

#include "RB-tree.h"

//экспериментальное подтверждение корректности:
//проверим, что св-ва обычного бинарного дерева выполняются на малых размерах,
//оценим на любых размерах отношение максимальной и минимальной ветвей - оно меньше 2
//-> ожидаемая сбалансированность достигнута

using namespace std;

int main()
{
	binary_tree<int> a;
    int t, max, min;

	cout << "auto add to a" << endl;
    for (int i = 0; i < 27; i++)
    {
    	//cin >> t;
        a.add_leaf(rand()%100);
        //a.print_tree();
        //a.deep(max, min);
        //cout << "max deep =" << max << "; min deep=" << min << ";" << endl;
        //a.add_leaf(t);
    }
    //a.clear_tree();
    //a = a;
    a.debug2();
	cout << "Graph tree a:" <<endl; a.print_tree(); a.debug();
    a.deep(max, min);
    cout << "max deep =" << max << "; min deep=" << min << ";" << endl;
    cout << "delete element in a" << endl;

    for (int i = 0; i < 10; i++)
    {
    	cin >> t;
    	a.del_leaf(t); //else a.rotate_leaf_right(t);
		//cout << "Graph tree a:\n"; a.print_tree();
		//a.del_leaf(rand()%100);
		a.deep(max, min);
        cout << "max deep =" << max << "; min deep=" << min << ";" << endl;
        //a.debug();
        a.debug2();
    }

	cout << "press any key" << endl;
    cin >> t;
	return 0;
}
