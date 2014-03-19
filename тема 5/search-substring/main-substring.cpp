#include <iostream>
#include <string>
#include <vector>
using namespace std;
const int size_of_alphabet = 26;//a..z

//префикс функция
vector<int> prefix_function(const string &s)
{
	vector<int> result(s.length());
	result[0] = 0;

	for(size_t i = 1, k = 0; i < s.length(); i++) //по i перебираем всю строку
	{
	    //int k = result[i-1]; можно добавить для очевидности

		while ((k > 0) && (s[k] != s[i])) k = result[k-1];

		if (s[k] == s[i]) k++;

		result[i] = k;
	}
	return result;
}

vector<int> boyer_moore(const string &text, const string &s)
{
    int symbols[size_of_alphabet];
    int suffix[s.length()];
    int len = s.length();
    vector<int> result;
    for (int i = 0; i < size_of_alphabet; i++) symbols[i] = -1;
    for (int i = 0; i < len; i++) suffix[i] = 0;

    //заполним таблицу символов
    for (int i = len - 1 - 1; i >= 0; --i) //len - 1 - 1 смотрим с предпоследнего символа
        if (symbols[s[i]-'a'] < i) symbols[s[i]-'a'] = i;

    //заполним таблицу суффиксов
    vector<int> pf1 = prefix_function(s); //значение префикс функции для самой строки
    string temp = s;
    for (int i = 0; i < len/2; i++) swap(temp[i], temp[len-i-1]);
    vector<int> pf2 = prefix_function(temp); //значение префикс функции для перевернутой строки

    for (int j = 0; j < len; j++)
        suffix[j] = len-1 - pf1[len];

    for (int i = 0; i < len; i++)
    {
        int t = len-1 - pf2[i];
        suffix[t]  = min(suffix[t], i - pf2[i]);
    }
    for (int i = 0; i < len; i++) suffix[i]++; //приводим к натуральным числам

    //for (int i = 0; i < size_of_alphabet; i++) cout << symbols[i] << ' ';
    //cout << endl;

    //for (int i = 0; i < len; i++) cout << suffix[i] << ' ';
    //cout << endl;

    //return 0;
    //осуществим поиск
    for (int i = 0; i < (text.length() - len + 1); )
    {
        int j;
        for (j = len-1; (j >= 0) && (s[j] == text[i+j]); --j); //проходим по буквам строки
        //cout << i << ' ' << j << ' ' << ' ' << i+j << ' ' << text[i+j] << endl;
        if (j == -1)
        {
            //cout << "result: " << i << endl;
            result.push_back(i);
            i++;
        }
        else
        {
            int delta1 = j - symbols[(int)text[i+j]-'a']; //сдвиг по стоп-символу
            int delta2 = suffix[j]; //сдвиг по суффиксу
            //cout << j << ' ' << delta1 << ' ' << delta2 << endl;
            if (delta1>delta2) i += delta1; else i+=delta2;
        }
    }
    return result;
}

vector<int> rabin_karp(const string &text, const string &s)
{
    //будем использовать в качестве хеш-функции представление числа в недесятичной системе счисления
    //за основание возьмем размер алфавита
    vector<int> result;

    if (s.length() > text.length()) return result;

    int len = s.length();
    vector<long long> foundations(len); //основания системы счисления
    foundations[0] = 1;
    for (size_t i = 1; i < foundations.size(); i++)
        foundations[i] = foundations[i-1] * (size_of_alphabet+1);

    //вычислим хеш от строки s
    long long hash_s = 0;
    for (int i = 0; i < len; i++)
        hash_s += (s[i] - 'a'+1) * foundations[len-1-i];

    //cout << "hash_s:"<< hash_s << endl;
    //текущий хеш текста
    long long current_hash = 0;
    for (int i = 0; i < len; i++)
        current_hash += (text[i] - 'a'+1) * foundations[len-1-i];

    if (current_hash == hash_s) result.push_back(0);

    for (size_t i = len; i < text.length(); i++)
    {
        current_hash -= (text[i-len] - 'a'+1) * foundations[len-1]; //вычитаем старший член суммы
        current_hash *= foundations[1]; //увеличиваем степени
        current_hash += (text[i] - 'a'+1); //прибавляем новый в нулевой степени
        if (current_hash == hash_s) result.push_back(i-len+1);
    }

    return result;
}

vector<int> knuth_morris_pratt(const string &text, const string &s)
{
    vector<int> result;

    vector<int> pf = prefix_function(s + "#" + text);

    int len = s.length();

    for (size_t i = len; i < pf.size(); i++)
        if (pf[i] == len) result.push_back(i-2*len);
    return result;
}

//поиск подстроки с помощью конечного автомата
vector<int> finite_automaton(const string &text, string s)
{
    //cout << "###";
    ///////////////////////
    int len = s.length();
    int delta_function[len+1][size_of_alphabet];
    vector<int> pf(len+1);
    vector<int> result;

    s = s + "#";
    string temp = s;

    //DEBUG
    //cout << s << endl;

    for (int j = 0; j < size_of_alphabet; j++) delta_function[0][j] = 0; //первая строка нулевая
    delta_function[0][s[0]-'a'] = 1; //начальный символ строки всегда переводит хотя бы в первое состояние
    pf[0] = 0; //значение префикс функции пустой строки

	for (int i = 1, k = 0; i < len+1; i++) //по i перебираем всю строку
	{
        for (int j = 0; j < size_of_alphabet; j++) //перебираем алфавит
        {
            s[i] = j + 'a';
            k = pf[i-1];

            while ((k > 0) && (s[k] != s[i])) k = s[k-1];

            if (s[k] == s[i]) k++;

            if (temp[i] == s[i])
            {
                pf[i] = k; // в pf данные настоящей строки
                delta_function[i][j] = (i+1); //переход в следующее состояние
            }
            else
                delta_function[i][j] = k;
        }
        s[i] = temp[i]; //возвращаем исходную строку
        //cout << " " << pf[i] << " " << s[i];
        //cout << endl;
	}

    // DEBUG
    /*
    for (size_t i = 0; i < pf.size(); i++) cout << pf[i] << " ";
    cout << endl;

    for (int i = 0; i < len+1; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << delta_function[i][j];
        }
        cout << endl;
    }
    */
    //поиск подстроки в тексте
    int q = 0;
    for (size_t i = 0; i < text.length(); i++)
    {
        if (q == len) result.push_back(i-len);
        q = delta_function[q][(int)text[i]-'a'];
        //cout <<"q=" << q << " i=" << i << endl;
    }


    return result;
}

int main()
{
    vector<int> v;
    string s = "ababaca";
    string t = "abdfababacababacaqw";
    cout << t << endl;
    while (1)
    {
    //v = prefix_function(s);
    //for (size_t i = 0; i < v.size(); i++) cout << v[i] << " ";
    //cout << endl;


    //}
    //cout << "bbb";
    //cout << endl << endl;
    //return 0;
    cin >> s;
    v = finite_automaton(t,s);
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << " ";
    cout << endl;

    v = knuth_morris_pratt(t, s);
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << " ";
    cout << endl;

    v = rabin_karp(t, s);
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << " ";
    cout << endl;

    v = boyer_moore(t, s);
    for (size_t i = 0; i < v.size(); i++) cout << v[i] << " ";
    cout << endl;
    }

    return 0;
}
