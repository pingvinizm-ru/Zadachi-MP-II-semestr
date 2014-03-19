//Кирюхин В. ККС-1-11
//Красно-черное дерево
#include <iostream>
#define NIL 0
#define RED 1
#define BLACK 2
using namespace std;

template <typename element>
struct leaf
{
	element data;
	char color;
	struct leaf<element> *prev;
    struct leaf<element> *left;
    struct leaf<element> *right;
};

template <typename element>
class binary_tree
{
    private:
    	struct leaf<element> *root; //корень дерева
        struct leaf<element> *nil;

        //выделение памяти под новый лист
        struct leaf<element>* create_new_leaf(element x, char c)
        {
        	struct leaf<element> *place;
        	try
        	{
                place = new struct leaf<element>;
                place->data = x;
                place->color = c;
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

        //вывод дерева на печать
        void print_tree(struct leaf<element> *current, int deep = 0)
        {
        	if (current == nil) return;

            if (current->left) print_tree(current->left, deep + 1);

            for (int i = 0; i < deep; ++i) cout << "   ";
                cout << current->data << endl;

            if (current->right) print_tree(current->right, deep + 1);
        }

        //отлаадочная функция - список листов с адресами и данными
        void debug(struct leaf<element> *current)
        {
        	if (current == nil) return;

            if (current->left) debug(current->left);

            cout << current->data << " "
                 << setw(10) << current << " "
                 << setw(10) << current->prev << " "
                 << setw(10) << current->left << " "
                 << setw(10) << current->right << " ";

            if (current->color==BLACK) cout << "BLACK"; else cout << "RED";
            cout << endl;

            if (current->right) debug(current->right);
        }

        void debug2(struct leaf<element> *current, int black)
        {
            if (current == nil) {cout << black << " "; return;}
            if (current->color == BLACK) black++;

            debug2(current->left, black);
            debug2(current->right, black);
        }
        //очистка дерева
        void clear_tree(struct leaf<element> *current)
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

        //переработать!!
        //копирование дерева из аргумента в текущее
        void copy_tree(struct leaf<element> *current, struct leaf<element> *source)
        {
            if (source) //копируемое дерево не пустое
            {
            	if (root == 0)
            	{
                	root = current = create_new_leaf(source->data, source->color);
            	}
            	if (source->left)
            	{
            		current->left = create_new_leaf(source->left->data, source->left->color);
                	copy_tree(current->left, source->left);
            	}
            	if (source->right)
            	{
            		current->right = create_new_leaf(source->right->data, source->left->color);
                	copy_tree(current->right, source->right);
            	}
            }
        }

        //получение адреса элемента
        struct leaf<element>* get_addr_of_leaf(struct leaf<element> *current, element x)
        {
            if (current == nil) return 0;//
            if (current->data == x) return current; //нашли адрес нужного элемента

            if (x < current->data)
                return get_addr_of_leaf(current->left, x);
            else //x > data
                return get_addr_of_leaf(current->right, x);
        };

        //добавление элемента в дерево без учета свойств
        void add_leaf(struct leaf<element> *parent, struct leaf<element> *&current, element x)
        {
            if (current == nil) //осуществляем вставку
            {
                //создали новый красный лист
                current = create_new_leaf(x, RED);
                current->prev = parent; //установили связь с предыдущим уровнем
                //восстановим свойства
                balance_after_add(current);
                return;
            }
            //переходим в соответствующую ветвь
            if (x < current->data)
                add_leaf(current, current->left, x);
            else
                add_leaf(current, current->right, x);
        }

        //удаление листа
        bool del_leaf(struct leaf<element> *&current, element x)
        {
            if (current == nil) return false;
            if (x < current->data) return del_leaf(current->left, x);
            if (x > current->data) return del_leaf(current->right, x);

            // => (x == current->data)
            if (current->left != nil && current->right != nil) //есть оба ребенка
            {   //сведем к случаю, когда детей не больше одного

                if (current->left->right == nil) //у левого сына нет правых сыновей
                {
                    current->data = current->left->data; //переместили данные
                    return del_leaf(current->left, current->left->data);
                }
                else //у левого сына есть правые сыновья
                {
                    struct leaf<element> *i;
                    for (i = current->left; i->right->right != nil; i = i->right);
                    current->data = i->right->data;
                    return del_leaf(i->right, i->right->data);
                }
            }
            //к данному моменту известно, что текущий имеет не более одного сына

            struct leaf<element> *temp = current;
            struct leaf<element> *parent = current->prev;

            if (current->left == nil && current->right == nil) //детей нет
            {
                current = nil;
            }
            else
            if (current->left != nil) //есть только левый
            {
                current = current->left;
                current->prev = temp->prev;
            }
            else //есть только правый
            {
                current = current->right;
                current->prev = temp->prev;
            }

            //debug();
            //если удаляемый красный - свойства не нарушаются, если красный, то св-ва выполнены
            if (temp->color == BLACK)
                balance_after_del(parent, current); //запускаем балансировку
            //debug();
            delete temp;
            return true;

        }

        //восстановление свойств дерева после удаления
        void balance_after_del(struct leaf<element> *parent, struct leaf<element> *current)
        {
            ///Код в данной функции избыточен, это очевидно
            ///Логику можно оптимизировать, избежав ненужных проверок
            if (current == root || current->color == RED)
            {
                current->color = BLACK;
                return;
            }
            struct leaf<element> *brother; //брат текущего листа

            if (current == parent->left)
                brother = parent->right;
            else
                brother = parent->left;
            //брат красный, его дети и корень по св-вам черный)
            if (brother->color == RED) //&& brother->left->color == BLACK && brother->right->color == BLACK
            {
                parent->color = RED; //родитель красный
                brother->color = BLACK; //брат черный
                //брат становится дедом текущего
                if (brother == parent->right)
                    rotate_leaf_left(parent);
                else
                    rotate_leaf_right(parent);

                return balance_after_del(parent, current);
            }

            if (brother->color == BLACK)
            {
                //дети брата черные
                if (brother->left->color == BLACK && brother->right->color == BLACK)
                {
                    brother->color = RED;
                    return balance_after_del(parent->prev, parent); //анализируем уровень выше
                }

                //внутренний сын - красный, внешний - черный
                if (brother == parent->right && brother->left->color == RED && brother->right->color == BLACK)
                {
                    brother->color = RED;
                    brother->left->color = BLACK;
                    rotate_leaf_right(brother);
                    return balance_after_del(parent, current);
                }
                //симметрично
                if (brother == parent->left && brother->left->color == BLACK && brother->right->color == RED)
                {
                    brother->color = RED;
                    brother->right->color = BLACK;
                    rotate_leaf_left(brother);
                    return balance_after_del(parent, current); //свели к следующему случаю
                }

                //внешний - красный, внутренний - любой
                if (brother == parent->right && brother->right->color == RED)
                {
                    brother->color = parent->color;
                    parent->color = BLACK;
                    brother->right->color = BLACK;
                    rotate_leaf_left(parent);
                    return;
                }
                //симметрично предыдушему случаю
                if (brother == parent->left && brother->left->color == RED)
                {
                    brother->color = parent->color;
                    parent->color = BLACK;
                    brother->left->color = BLACK;
                    rotate_leaf_right(parent);
                    return;
                }
            }





        }

        //восстановление свойств дерева после добавления
        void balance_after_add(struct leaf<element> *current)
        {
            struct leaf<element> *parent = current->prev;

            if (current == root) //мы в корне
            {
                current->color = BLACK; //корень всегда черный
                return;
            }
            if (parent->color == BLACK) //родитель черный - свойства выполнены
                return;
            ////если прошли два верхних условия, то дедушка листа существует
            struct leaf<element> *grandparent = parent->prev;

            //дядя и родитель красные
            if (grandparent->left->color == RED && grandparent->right->color == RED)
            {
                //родитель и дядя становятся черными
                grandparent->left->color = BLACK;
                grandparent->right->color = BLACK;
                //дедушка - красным
                grandparent->color = RED;
                balance_after_add(grandparent);
                return;
            }
            ////на данном этапе известно, что родитель красный, а дядя черный

            //текущий не самый левый внук
            if (grandparent->left == parent && parent->right == current)
            {
                rotate_leaf_left(parent);
                balance_after_add(parent);
                return;
            }
            // и не самый правый
            if (grandparent->right == parent && parent->left == current)
            {
                rotate_leaf_right(parent);
                balance_after_add(parent);
                return;
            }

            //текущий либо самый левый внук, либо самый правый
            parent->color = BLACK;
            grandparent->color = RED;

            if (grandparent->left == parent && parent->left == current) //самый левый внук
                rotate_leaf_right(grandparent);
            else //самый правый внук
                rotate_leaf_left(grandparent);

        }

        //левый поворот листа
        bool rotate_leaf_left(struct leaf<element> *current)
        {
            if (current == nil) return false;
            struct leaf<element> *parent = current->prev;
            struct leaf<element> *child = current->right;
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
        }

        //правый поворот листа
        bool rotate_leaf_right(struct leaf<element> *current)
        {
            if (current == nil) return false;
            struct leaf<element> *parent = current->prev;
            struct leaf<element> *child = current->left;
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
        }

        void deep(struct leaf<element> *current, int &max, int &min, int d)
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

    	binary_tree()
        {
            nil = create_new_leaf(0, BLACK);
            root = nil;
        }

        ~binary_tree()
        {
        	clear_tree();
        	delete nil;
        }

        binary_tree (const binary_tree<element> &x)
        {
            root = 0;
        	copy_tree(root, x.root);
        }

        binary_tree operator= (const binary_tree<element> &x)
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

        void add_leaf(element x)
        {
        	add_leaf(0, root, x);
        }

		bool del_leaf(element x)
		{
			return del_leaf(root, x);
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

        void debug2()
        {
            debug2(root,0);
            cout<< endl;
        }

        void print_root()
        {
            cout << "root = " << root << endl;
        }

        //существует ли такой элемент в дереве
        bool is_exist(element x)
        {
            return get_addr_of_leaf(root, x);
        }

        //левый поворот элемента
        bool rotate_leaf_left(element x)
        {
            return rotate_leaf_left(get_addr_of_leaf(root, x));
        }

        //правый поворот элемента
        bool rotate_leaf_right(element x)
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
