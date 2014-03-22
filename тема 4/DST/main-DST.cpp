#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include "DST.h"

using namespace std;
//быстрая сортировка на 3 участка, усиленная сортировкой вставками
template <typename type>
void quick_sort_flag_ins(type array[], int left, int right)
{
	const int small = 16;
	int l = left, r = right, l_flag = left, r_flag = right;
	type middle = array[rand() % (right - left) + left] ; //среднее значение

	if (right - left <= small)
	{
		insert_sort(array, left, right);
		return;
	}

	while (1)
	{
		while (array[l] > middle) l++;
		while (array[r] < middle) r--;
		if (l >= r) break;

		swap(array[l], array[r]);

		if (array[l] == middle)
		{
			swap(array[l_flag], array[l]);
			l_flag++;
		}

		if (array[r] == middle)
		{
			swap(array[r_flag], array[r]);
			r_flag--;
		}

		l++;
		r--;
	}

	--l; ++r;
	for (int i = left; i < l_flag; ++i, --l) swap(array[i], array[l]);
	for (int i = right; i > r_flag; --i, ++r) swap(array[i], array[r]);

	if (left < l) quick_sort_flag_ins(array, left,  l);
	if (r < right) quick_sort_flag_ins(array, r, right);
}

//сортировка вставками
template <typename type>
void insert_sort(type array[], int left, int right)
{
	int i, j;
	for (i = left + 1; i <= right; ++i)
	{
		type temp = array[i];
		for (j = i - 1; j >= left && array[j] < temp; --j)
			array[j + 1] = array[j];

		array[j + 1] = temp;
	}
}

int main()
{
	DST a;
    //int max, min;

    ifstream input ("input-DST.txt");
    int N, k;
    string s;
    pair<int, string> *data; //список слов в формате: частота - слово

    input >> N;
    data = new pair<int, string>[N];
    //считываем слова
    for (int i = 0; i < N; i++)
    {
        input >> s;
        input >> k;
        data[i].second = s;
        data[i].first = k;
        cout << k << " " << s << " " << endl;
    }
    cout << endl;
    input.close();
    //сортируем по частоте
    quick_sort_flag_ins(data,0,N-1);


    for (int i = 0; i < N; i++)
    {
        cout << data[i].second << " " << data[i].first << " " << endl;
    }
    //добавляем в дерево
    for (int i = 0; i < N; i++)
    {
        a.add_leaf(data[i].second);

    }
    a.print_tree();

    cout << "Поиск:" << endl;
    for (int i = 0; i < 10; i++)
    {
        cin >> s;
        cout << a.is_exist(s) << endl;
    }
    //a.clear_tree();
    //a = a;

	//cout << "Graph tree a:" <<endl; a.print_tree();
    //a.deep(max, min);
    //cout << "max deep =" << max << "; min deep=" << min << ";" << endl;
    //cout << "delete element in a" << endl;
	//cout << "press any key" << endl;
    //cin >> t;
	return 0;
}
