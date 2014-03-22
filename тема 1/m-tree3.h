///Кирюхин Виталий, ККС-1-11///

#include <iostream>
#include <string>

using namespace std;

struct leaf
{
    unsigned int num; //число слов
    char c;  //хранящаяся буква
    struct leaf *prev; //ссылка на предыдущий уровень
    struct leaf *right; //ссылка на следующий элемент текущего уровня
    struct leaf *next; //ссылка на следующий уровень
};

class m_tree
{
    private:
        struct leaf *root; //корень дерева - пустая строка с условной длиной -1

        //выделение памяти на лист с возвратом адреса выделенного места
        struct leaf* create_new_leaf(char x, unsigned int n) //выделение памяти под новый лист
        {
        	struct leaf *place;
        	try
        	{
				place = new struct leaf; //выделение памяти
				//заполнение полей согласно аргументам
				place->c = x;
				place->num = n;
				//указатели нулевые - лист "висит в воздухе"
				place->prev = 0;
				place->right = 0;
				place->next = 0;
            }
            catch(bad_alloc err) //отлавливаем ошибку выделения памяти
            {
            	cerr << "bab_alloc" << endl;
            	return 0; //возвращаем 0 - место не выделено
            }
            return place; //возвращаем выделенный под лист адрес
        }

        //добавление листа к дереву
        void add(struct leaf *current, int len, string s)
        {
            //рассмотрим три случая: буква строки =, <, > буквы в листе
            if (len == -1 || current->c == s[len]) //либо мы в корне, который всегда подходит, либо нашли нужный
            {
                if ((s.length()-1) == len) //прошли по всем буквам строки
                {
                    current->num++;
                    return;
                }
                if (current->next == 0) //следующий уровень еще не создан
                {
                    current->next = create_new_leaf(s[len+1], 0);//создали лист со следующей буквой строки
                    current->next->prev = current; //предыдущим нового листа является текущий лист
                }
                add(current->next, len+1, s);//рассматриваем следующий уровень
            }
            else
            if (s[len] < current->c) //случай вставки в самое левое место уровня
            {
                struct leaf *newleaf = create_new_leaf(s[len], 0);
                newleaf->right = current; //текущий правее нового листа
                newleaf->prev = current->prev; //и новый и текущий на одном уровне -> имеют одинакового предыдущего
                newleaf->prev->next = newleaf; //предок теперь указывает на новый, самый левый лист
                add(newleaf, len, s); //переходим в новый лист
            }
            else //if (s[len] > current->c)
            {
                if (current->right == 0 || (s[len] < current->right->c))
                {
                    //необходимость вставки в уровень возникает если следующего листа нет,
                    //или его значение больше нужной буквы в строке
                    struct leaf *temp = current->right;
                    current->right = create_new_leaf(s[len], 0);
                    current->right->prev = current->prev; //у них одинаковый предыдущий, т.к. они на одном уровне
                    current->right->right = temp; //правее нового листа лист, который был правым раньше
                }
                add(current->right, len, s); //переходим в новый лист
            }
        }

        //удаление листа из дерева (для простоты значение num в листе не учитывается)
        bool del(struct leaf *current, int len, string s)
        {
            /*
                мы удаляем лист из памяти тогда, когда у него нет потомков (next = 0) и число в листе <= 1,
                    число больше 1 - уменьшаем его;
                если у листа есть потомки и значение в листе > 0, уменьшаем значение на 1,
                    если равно 0 - ничего не деnextлаем;
            */

            //удаляемый элемент либо самый левый в уровне, либо нет
            /* блок написан плохо! используется дублирование кода!*/
            if ((s.length()-1) == len) //прошли по всей строке
            {
                if (current->c == s[len]) //мы в самом левом элементе уровня
                {
                    if (current->next == 0) //нет потомков
                    {
                        current->prev->next = current->right;
                        delete current;
                        return true;
                    }
                    else
                        return false;
                }
                if (current->right && current->right->c == s[len]) //рассмотрим элемент правее, он не самый левый
                {
                    if (current->right->next == 0) //нет потомков
                    {
                        struct leaf *temp = current->right;
                        current->right = current->right->right;
                        delete temp;
                        return true;
                    }
                    else
                        return false;

                }
            }

            if (len == -1 || current->c == s[len]) //если мы в корне или нашли нужную букву
            {
                return del(current->next, len+1, s); //смотрим следующий уровень
            }
            else
            if (s[len] < current->c) //дальше значения только возрастают -> нужного нет вообще
                return false;
            else //if (s[len] > current->c)
                return del(current->right, len, s);//смотрим уровень дальше

        }

        //удаление листа из дерева - реализация через ссылку на адрес
        bool del_ref(struct leaf *&current, unsigned int len, string s)
        {
            /*
                мы удаляем лист из памяти тогда, когда у него нет потомков (next = 0) и число в листе <= 1,
                    число больше 1 - уменьшаем его;
                если у листа есть потомки и значение в листе > 0, уменьшаем значение на 1,
                    если равно 0 - ничего не делаем;
            */

            if (current == 0)
                return false;
            else
                if (s[len] == current->c) //нашли нужный
                {
                    if (s.length()-1 <= len) //прошли всю строку
                    {
                        //рассмотрим возможные ситуации
                        if ((current->num) > 1) //значение в листе больше 1 -> уменьшаем
                        {
                            (current->num)--;
                            return true;
                        }
                        if ((current->next) == 0) //нет потомков и значение <= 1 -> удаляем
                        {
                            struct leaf *temp = current;
                            current = current->right;
                            delete temp;
                            return true;
                        }
                        //третий случай: потомки есть и значение либо 0, либо 1 -> обнуляем
                        current->num = 0;
                        return true;
                    }
                    else //смотрим следующий уровень
                        return del_ref(current->next, len+1, s);
                }
                else
                    if (s[len] < current->c)
                        return false;
                    else //будем просматривать текущий уровень дальше
                        return del_ref(current->right, len, s);
        }


        //удаление листа из дерева - реализация через дополнительный аргумент рекурсивной функции
        bool del_prev(struct leaf* before_current, struct leaf* current, unsigned int len, string s)
        {
            /*
                мы удаляем лист из памяти тогда, когда у него нет потомков (next = 0) и число в листе <= 1,
                    число больше 1 - уменьшаем его;
                если у листа есть потомки и значение в листе > 0, уменьшаем значение на 1,
                    если равно 0 - ничего не делаем;
            */

            if (current == 0)
                return false;
            else
                if (s[len] == current->c) //нашли нужный
                {
                    if ((s.length()-1) == len) //прошли всю строку
                    {
                        //рассмотрим возможные ситуации
                        if ((current->num) > 1) //значение в листе больше 1 - уменьшаем
                        {
                            (current->num)--;
                            return true;
                        }
                        if ((current->next) == 0) //нет потомков и значение <= 1 - удаляем
                        {
                            //предыдущий либо выше уровнем, либо на таком же
                            if (before_current == current->prev)
                                before_current->next = current->right;
                            else
                                before_current->right = current->right;
                            delete current;
                            return true;
                        }
                        //третий случай: потомки есть и значение либо 0, либо 1 - обнуляем
                        current->num = 0;
                        return true;
                    }
                    else //смотрим следующий уровень
                        return del_prev(current, current->next, len+1, s);
                }
                else
                    if (s[len] < current->c)
                        return false;
                    else //будем просматривать текущий уровень дальше
                        return del_prev(current, current->right, len, s);
        }

        //поиск листа в дереве
        bool search (struct leaf* current, int len, string s)
        {
            if (current == 0) return false;

            if (len == -1 || current->c == s[len]) //если мы в корне или нашли нужную букву
            {
                if ((s.length()-1) == len) //прошли по всей строке
                    return true;

                return search(current->next, len+1, s); //смотрим следующий уровень
            }
            else
            if (s[len] < current->c)
                return false;
            else //if (s[len] > current->c)
                return search(current->right, len, s);//смотрим уровень дальше
        }

        //очистка памяти, выделенной под дерево
        void clear_tree(struct leaf *current)
        {
            if (current == 0) return; //нечего удалять
            clear_tree(current->next); //удаляем следующий уровень
            clear_tree(current->right); //удаляем текущий уровень
            delete current; //освобождаем память, занятую под текущий элемент
        }

        //копирование дерева из аргумента в текущее
        void copy_tree(struct leaf *parent, struct leaf *source)
        {
        	//parent - исходное дерево
            //source - источник копирования

            if (root == 0) //корень еще не скопирован
            {
                root = parent = create_new_leaf(source->c, source->num);
            }
            if (source->next) //существует следующий уровень
            {
                parent->next = create_new_leaf(source->next->c, source->next->num);
                copy_tree(parent->next, source->next);
            }
            if (source->right) //существует продолжение текущего
            {
                parent->right = create_new_leaf(source->right->c, source->right->num);
                copy_tree(parent->right, source->right);
            }

        }

        void print_tree(struct leaf *current, string s)
        {
            if (current == 0) return;

            string t = s + current->c; //добавили новый символ
            cout << t
                 << " число:" << current->num
                 << " потомки:" << (bool) current->next << endl;
            print_tree(current->right, s); //передаем без нового символа
            print_tree(current->next, t); //добавляем новый символ
        }

    public:
        m_tree() //конструктор
        {
            root = create_new_leaf(0, 0); //создаем пустой корень
        }

        ~m_tree() //деструктор
        {
            clear_tree(root);
        }

        m_tree (const m_tree &x) //конструктор копирования
        {
            root = 0; //изначально дерево пустое
        	copy_tree(root, x.root); //запуск копирования
        }

        m_tree operator= (const m_tree &x) //оператор присваивания
        {
            if (this == &x) return *this; //защита от самокопирования
            clear_tree(root); //освобождение памяти занятой текущим деревом
            root = 0;
            copy_tree(root, x.root); //запуск копирования переданного в текущее
            return *this;//возвращение значения текущего
        }

        //интерфейсы
        void add(string s)
        {
            add(root, -1, s);
        }

        bool del(string s)
        {
            //return del(root, -1, s);
            //return del_prev(root, root->next, 0, s);
            return del_ref(root->next, 0, s);
        }

        bool search(string s)
        {
            return search(root->next, 0, s);
        }

        void print_tree() //вывод - отладочная функция
        {
            print_tree(root->next, "");
        }
};
