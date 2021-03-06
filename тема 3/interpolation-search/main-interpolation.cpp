#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

template <typename element>
int interpolation_search(element *array, element x, int n, int &count_of_operations)
//упорядоченный массив, искомый элемент, число элементов, число операций
{
    int index = -1;
    int left = 0;
    int right = n - 1;


    while (array[left] <= x && x <= array[right]) //т.е. x принадлежит [left, right]
    {
        count_of_operations +=2; //два сравнения
        index = left + (x - array[left]) * (right - left) / (array[right] - array[left]);

        if (array[index] == x)
            return index;
        count_of_operations++; //сравнение
        if (array[index] < x)
            left = index + 1; //рассматриваем интервал от index+1 до right
        else //array[index] > x
            right = index - 1; //рассматриваем интервал от left до index-1
        count_of_operations++; //сравнение
    }

    return -1; //не нашли нужного
}

int main()
{
    ifstream input ("input-interpolation.txt");
    int N, t, op, index;
    int *data; //упорядоченный массив

    input >> N;
    data = new int[N];
    for (int i = 0; i < N; i++)
    {
        input >> t;
        data[i] = t;
        cout << i << ") " << t << endl;
    }
    input.close();
    cout << "search..." << endl;
    for (int i = 0; i < 10; i++)
    {
        cin >> t;
        op = 0;

        index = interpolation_search(data, t , N, op);
        cout << "result = " << index << " op = " << op << endl;

    }

    return 0;
}
