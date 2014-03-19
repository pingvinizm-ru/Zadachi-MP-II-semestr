#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <map>

#define DEBUG 0
using namespace std;

struct leaf //лист
{
    int right; //индекс правого
    int left; //индекс левого
};

struct letter
{
    char symbol; //символ
    int freq; //частота
};

struct small_tree
{
    int freq; //суммарная частота
    int root; //корневой индекс
};

bool operator>(const small_tree &a, const small_tree &b)
{
  return a.freq > b.freq;
}

void create_table(vector<leaf> &tree, map<char, string> &code, map<string, char> &decode, letter *al, int index, string s)
{
    if (tree[index].left != -1)
        create_table(tree, code, decode, al, tree[index].left, s+"0");
    if (tree[index].right != -1)
        create_table(tree, code, decode, al, tree[index].right, s+"1");

    if (tree[index].left == -1 && tree[index].right == -1) // мы в листе
    {
        code[al[index].symbol] = s;
        decode[s] = al[index].symbol;
        cout << al[index].symbol << " " << s << endl;
    }
}

string haffman(letter *alphabet, int size_of_alphabet, string text, bool code)
{
    priority_queue<small_tree, vector<small_tree>, greater<small_tree> > forest; //лес, используется очередь с приоритетами
    vector<leaf> tree(50);
    map<char, string> code_table;
    map<string, char> decode_table;
    int tree_index = 0; //для удобства: в векторе хранится эта информация
    string result;

    if (size_of_alphabet == 0) return "";
    //готовим лес и добавляем к дереву самый нижний уровень
    for (int i = 0; i < size_of_alphabet; i++)
    {
        small_tree t;
        leaf l;
        t.freq = alphabet[i].freq;
        t.root = i;
        forest.push(t);

        l.left = l.right = -1;
        tree[tree_index++] = l;
    }

    //построим дерево
    while (forest.size() > 1) //пока лес не пуст
    {
        small_tree t1, t2, new_t; //два деревца с наименьшем весом
        t1 = forest.top(); forest.pop(); //извлекли и удалили
        t2 = forest.top(); forest.pop();

        tree[tree_index].left  = t1.root;
        tree[tree_index].right = t2.root;

        new_t.freq = t1.freq + t2.freq;
        new_t.root = tree_index;
        forest.push(new_t);
        tree_index++;
    }
    if (DEBUG)
    {
    cout << "дерево:" << endl;
    for (int i = 0; i < tree_index; i++)
    {
        cout << tree[i].left << " " << tree[i].right << endl;
    }
    }
    //заполним таблицы кодирования/декодирования
    cout << "таблица кодов:" << endl;
    create_table(tree, code_table, decode_table, alphabet, tree_index-1, "");

    if (code) //кодируем
    {
        for (unsigned int i = 0; i < text.length(); i++)
            result+=code_table[text[i]];
    }
    else //декодируем
    {
        string t;
        for (unsigned int i = 0; i < text.length(); i++)
        {
            t += text[i];
            if (decode_table[t])
            {
                result += decode_table[t];
                t = "";
            }
        }
    }
    return result;
}

int main()
{
    ifstream input ("input-huffman.txt"); //файл с алфавитом: число букв, строки вида - буква-частота
    letter *alphabet; //алфавит
    int size_of_alphabet; //размер алфавита
    string text = "abcd";
    string code_text;
    input >> size_of_alphabet;
    alphabet = new letter[size_of_alphabet];

    cout << "алфавит и частоты" << endl;
    for (int i = 0; i < size_of_alphabet; i++)
    {
        char c;
        int v;
        small_tree t;
        input >> c >> v;
        cout << c << " " << v << endl;
        alphabet[i].symbol = c;
        alphabet[i].freq = t.freq = v;
    }
    cout << endl;

    code_text = haffman(alphabet, size_of_alphabet, text, true);

    cout << "text: " << text << endl;
    cout << "code: " << code_text << endl;

    text = haffman(alphabet, size_of_alphabet, code_text, false);
    cout << "text: " << text << endl;
    return 0;
}
