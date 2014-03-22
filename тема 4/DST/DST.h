//Кирюхин В. ККС-1-11
//дерево цифрового поиска
#include <iostream>
#include <string>
using namespace std;

struct leaf
{
	string data;
	struct leaf *prev;
    struct leaf *left;
    struct leaf *right;
};

class DST
{
    private:
    	leaf *root; //корень дерева
        leaf *nil;

        //выделение памяти под новый лист
        leaf* create_new_leaf(string x)
        {
        	leaf *place;
        	try
        	{
                place = new leaf;
                place->data = x;
                place->prev = 0;
                place->left = nil;
                place->right = nil;
            }
            catch(bad_alloc err)
            {
            	cerr << "bab_alloc" << endl;
            	return 0;
            }
            return place;
        }
        bool bit(string &x, int bit)
        {
            bool result = true;
            int t = (1 << (7 - bit % 8));
            result = (x[bit / 8] & t);
            return result;
        }
        //вывод дерева на печать
        void print_tree(leaf *current, int deep = 0)
        {
        	if (current == nil) return;

            print_tree(current->left, deep + 1);

            for (int i = 0; i < deep; ++i) cout << "     ";
                cout << current->data << endl;

            print_tree(current->right, deep + 1);
        }

        //отладочная функция - список листов с адресами и данными
        void debug(leaf *current)
        {
        	if (current == nil) return;

            debug(current->left);

            cout << current->data << " "
                 << setw(10) << current << " "
                 << setw(10) << current->prev << " "
                 << setw(10) << current->left << " "
                 << setw(10) << current->right << " ";
            cout << endl;

            debug(current->right);
        }

        //очистка дерева
        void clear_tree(leaf *current)
        {
        	if (current == nil) return;//если дерево уже пустое -> выходим
            clear_tree(current->left); //запускаем очистку левой ветви
            clear_tree(current->right); //запускаем очистку правой ветви

            //к этому моменту левая и правая ветви пусты
            delete current;
            //если мы удалили все дерево, и находимся в корне
            //устанавливаем root = 0, показывая, что дерево пусто
        	if (current == root) root = 0;
        }


        //копирование дерева из аргумента в текущее
        void copy_tree(leaf *current, leaf *source)
        {
            if (source) //копируемое дерево не пустое
            {
            	if (root == 0)
            	{
                	root = current = create_new_leaf(source->data);
            	}
            	if (source->left)
            	{
            		current->left = create_new_leaf(source->left->data);
                	copy_tree(current->left, source->left);
            	}
            	if (source->right)
            	{
            		current->right = create_new_leaf(source->right->data);
                	copy_tree(current->right, source->right);
            	}
            }
        }

        //получение адреса элемента
        leaf* get_addr_of_leaf(leaf *current, string x, int deep = 0)
        {
            if (current == nil) return 0;//
            if (current->data == x) return current; //нашли адрес нужного элемента

            if (bit(x, deep))
                return get_addr_of_leaf(current->left, x, deep+1);
            else //x > data
                return get_addr_of_leaf(current->right, x, deep+1);
        };

        //добавление элемента в дерево без учета свойств
        void add_leaf(leaf *parent, leaf *&current, string x, int deep = 0)
        {
            if (current == nil) //осуществляем вставку
            {
                //создали новый красный лист
                current = create_new_leaf(x);
                current->prev = parent; //установили связь с предыдущим уровнем
                return;
            }
            //переходим в соответствующую ветвь
            if (bit(x, deep))
                add_leaf(current, current->left, x, deep+1);
            else
                add_leaf(current, current->right, x, deep+1);
        }

        //левый поворот листа
        bool rotate_leaf_left(leaf *current)
        {
            if (current == nil) return false;
            leaf *parent = current->prev;
            leaf *child = current->right;
            if (child == nil) return false;

            current->right = child->left;
            child->left->prev = current;

            child->left = current;
            current->prev = child;

            if (parent) //родитель существует
                if (parent->left == current) //текущий - левый сын
                    parent->left = child;
                else //текущий - правый сын
                    parent->right = child;
            else
                root = child; //мы поворачиваем корень

            child->prev = parent;//устанавливаем связь ребенка текущего с родителем текущего
            return true;
        }

        //правый поворот листа
        bool rotate_leaf_right(leaf *current)
        {
            if (current == nil) return false;
            struct leaf *parent = current->prev;
            struct leaf *child = current->left;
            if (child == nil) return false;

            current->left = child->right;
            child->right->prev = current;

            child->right = current;
            current->prev = child;

            if (parent) //родитель существует
                if (parent->right == current) //текущий - правый сын
                    parent->right = child;
                else //текущий - левый сын
                    parent->left = child;
            else
                root = child; //мы поворачиваем корень

            child->prev = parent;//устанавливаем связь ребенка текущего с родителем текущего
            return true;
        }

        void deep(leaf *current, int &max, int &min, int d)
        {
                if (current == nil)
                {
                    if (d > max) max = d;
                    if (d < min) min = d;
                    return;
                }
                deep(current->left, max, min, d+1);
                deep(current->right, max, min, d+1);

        }


    public:

    	DST()
        {
            nil = create_new_leaf("");
            root = nil;
        }

        ~DST()
        {
        	clear_tree();
        	delete nil;
        }

        DST (const DST &x)
        {
            root = 0;
        	copy_tree(root, x.root);
        }

        DST operator= (const DST &x)
        {
            if (this == &x) return *this;
            clear_tree();
            if (x.root) copy_tree(root, x.root);

            return *this;
        }

        void clear_tree()
        {
        	clear_tree(root);
        }

        void add_leaf(string x)
        {
        	add_leaf(0, root, x);
        }

        void print_tree(int type_of_print = 1)
        {
            print_tree(root, type_of_print);
            cout << endl;
        }

        void debug()
        {
            debug(root);
            cout << endl;
        }


        void print_root()
        {
            cout << "root = " << root << endl;
        }

        //существует ли такой элемент в дереве
        bool is_exist(string x)
        {
            return get_addr_of_leaf(root, x);
        }

        //левый поворот элемента
        bool rotate_leaf_left(string x)
        {
            return rotate_leaf_left(get_addr_of_leaf(root, x));
        }

        //правый поворот элемента
        bool rotate_leaf_right(string x)
        {
            return rotate_leaf_right(get_addr_of_leaf(root, x));
        }

        void deep(int &max, int &min)
        {
            min = 2100000000;
            max = 0;
            deep(root, max, min, 0);
        }


};
