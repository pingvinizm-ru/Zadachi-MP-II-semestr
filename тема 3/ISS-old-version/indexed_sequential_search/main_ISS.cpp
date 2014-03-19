//Кирюхин В. ККС-1-11
//Индексно-последовательный поиск
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

template <typename element>
struct record
{
    element data; //данные
    unsigned int key; //ключ-идентификатор
};

struct key_record
{
    unsigned int key; //ключ
    unsigned int index; //индекс в массиве, соответствующий ключу
};

template <typename element>
class array_for_ISS
{
    record<element> *array; //массив данных и ключей
    int n; //число записей с данными
    key_record *table;
    int m; //число записей в таблице ключей
    int p; //сколько записей с данными соответствуют записи с индексами

    public:
    //при создании явно указывается число записей и массив, содержащий их.
    //по условию задачи МАССИВ УПОРЯДОЧЕН
    array_for_ISS(int count, record<element> *data)
    {
        n = count;
        p = sqrt(n); //оптимальный размер шага в таблице индексов
        m = n/p; //число записей в таблице индекса

        //cout << n << " " << m << " " << p << endl;
        array = new record<element>[n];
        table = new key_record[m];

        for (int i = 0, j = 0; i < n; i++) //перебираем все элементы
        {
            array[i] = data[i]; //копируем данные
            if (((i+1) % p == 0)) //каждый p-й ключ заносится в таблицу
            {
                table[j].key = array[i].key;
                table[j].index = i;
                j++;//
                //cout << "key = " << array[i].key << "  j = " << j << endl;
            }
        }
    }

    element search(unsigned int key, int &count_of_operations)
    {
        int i, j;
        int low_index, hight_index;
        //найдем группу
        count_of_operations+=1; //i = 0;
        for(i = 0; (i < m) && (table[i].key <= key); i++) count_of_operations+=3;

        //определим верхние и нижние индексы
        if (i == 0) low_index = 0; else low_index = table[i-1].index;
        if (i == m) hight_index = n; else hight_index = table[i].index-1;
        count_of_operations += 4; //два раза сравнили, два присвоили

        count_of_operations+=1; //j = low_index
        for (j = low_index; j <= hight_index; j++)
        {
            count_of_operations+=3;
            if (array[j].key == key) return array[j].data;
        }


        throw 0; //элемента нет в таблице

    }

    void copy(const array_for_ISS &x)
    {
        n = x.n; m = x.m; p = x.p;

        array = new record<element>[n];
        table = new key_record[m];

        for (int i = 0; i < n; i++)
            array[i] = x.array[i];

        for (int i = 0; i < m; i++)
            table[i] = x.table[i];
    }

    ~array_for_ISS()
    {
        delete[] array;
        delete[] table;
    }

    //конструктор копирования
    array_for_ISS (const array_for_ISS &x)
    {
        	copy(x);
    }

    //оператор присваивания
    array_for_ISS operator= (const array_for_ISS &x)
    {
        	if (this == &x) return *this;
        	delete[]array; //высвобождение памяти
        	delete[]table;
            copy(x);
            return *this;
    }

};

int main()
{
    //т.к. условие ввода не оговорено в тех.задании, то
    //первая строка входного файла "input.txt" содержит число записей
    //далее идут строки формата ключ-пробел-данные
    //для примера данные - произвольная строка
    ifstream input ("input-ISS.txt");
    int N, k, t, op;
    string s;
    record<string> *data;

    input >> N;
    data = new record<string>[N];
    for (int i = 0; i < N; i++)
    {
        input >> k;
        input >> s;
        data[i].data = s;
        data[i].key = k;
        cout << k << " " << s << " " << endl;
    }
    input.close();
    array_for_ISS<string> A1(N , data);

    for (int i = 0; i < 10; i++)
    {
        cin >> t;
        op = 0;
        try
        {
            s = A1.search(t, op);
            cout << "result = " << s << " op = " << op << endl;
        }
        catch (...)
        {
            cout << "err" << endl;
        }
    }

    return 0;
}
