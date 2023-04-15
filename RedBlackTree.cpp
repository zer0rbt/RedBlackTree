#include <iostream>
#include <vector>

using namespace std;

template<class type>
struct Node {
    bool color = false; // 1 is red, 0 is black
    type value;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;
};

template<class type>
struct RBTree {
    /// Неотъемлемая часть кода; Каждая из этих функций обязательно должна быть в дереве.
    Node<type> *nil = new Node<type>();
    Node<type> *root = nil;


    // Вставка
    void insert(type value) {
        // Создаем новый узел и инициализируем его
        auto *pNode = new Node<type>();
        pNode->value = value;
        insert(pNode);
    }
    // Удаление
    void remove(type key) {
        Node<type> *pNode = pFind(root, key);
        remove(pNode);
    }

private:
    void remove(Node<type> *pNode) {
        nil->parent = nullptr; // Это костыль.
        // Если у pNode нет детей
        if (pNode->right == nil && pNode->left == nil) {
            if (pNode == root)
                root = nil;
            else {
                // Изменяем ссылку на pNode у "отца" на nil
                if (pNode->parent->right == pNode)
                    pNode->parent->right = nil;
                else
                    pNode->parent->left = nil;
            }
            // Если удаляемый узел черный, то возможно нарушение баланса
            if (pNode->color == 0) {
                nil->parent = pNode->parent;
                fixDeleting(nil);
            }
            return;
        }
        // Если у "pNode" есть дети
        Node<type> *x = pNode;
        if (pNode->right == nil or pNode->left == nil) {
            // Заменяем узел "pNode" на его ребенка "x" у отца
            pNode->parent = (pNode->right == nil) ? pNode->left : pNode->right;
        } else {
            // Если есть оба ребенка, то находим узел "x" с минимальным значением справа от "pNode"
            x = x->right;
            while (x->left != nil) x = x->left;

            if (x->right != nil) // Если "x" имеет правого ребенка, то меняем ссылку на "x" у отца
                x->right->parent = x->parent;
            if (x == root) {// Если "x" - корень, то обновляем ссылку на корень
                root = x->right;
            } else {// Иначе у родителя ссылку на "x" меняем на ссылку на первого ребенка "x"
                x->parent->left = x->right;
            }
        }

        bool go = (pNode->color == false);
        if (x != pNode) {
            // Если заменили узел, то копируем значение "x" в pNode
            pNode->color = x->color;
            pNode->value = x->value;
        }
        if (go) // Если удаляемый узел был черным, то возможно нарушение баланса
            fixDeleting(pNode);
    }

    Node<type> *brother(Node<type> *pNode) {
        return (pNode->parent->left == pNode) ? (pNode->parent->right) : (pNode->parent->left);
    }

    Node<type> *pFind(Node<type> *pNode, type data) {
        if (pNode == nil) {
            return nil;
        }
        if (data < pNode->value) {
            return pFind(pNode->left, data);
        } else if (data > pNode->value) {
            return pFind(pNode->right, data);
        } else {
            return pNode;
        }
    }

    void insert(Node<type> *pNode) {
        // Инициализируем новый узел
        pNode->left = nil;
        pNode->right = nil;
        pNode->parent = nil;

        // Если дерево пустое, то новый узел становится корнем
        if (root == nil) {
            root = pNode;
        } else {
            // Ищем место для нового узла в дереве
            Node<type> *parent = nil;
            Node<type> *current = root;
            pNode->color = 1; // Новый узел становится красным
            while (current != nil) {
                parent = current;
                if (pNode->value < current->value) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }
            // Устанавливаем связи между новым узлом и его родителем
            pNode->parent = parent;
            if (pNode->value < parent->value)
                parent->left = pNode;
            else
                parent->right = pNode;

            // Проверяем и исправляем возможные нарушения свойств красно-черного дерева
            fixInsert(pNode);
        }
    }

    // Важный момент: поворачиваем "тяня" за вершину, т.е. передаем ту вершину, которая уйдет "вверх";
    // Просто в разных источниках поворот реализуется по разному, так что это упоминуть нужно.
    // В остальном это просто повороты, так что здесь без комментариев.
    // <Если вы будете дорабатывать код и у вас возникнут проблемы, смотрите в первую очередь на повороты>
    void right_rotate(Node<type> *pNode) {
        if (pNode->parent != nil) {
            if (pNode->right != nil) {
                pNode->parent->left = pNode->right;
                pNode->right->parent = pNode->parent;
            } else pNode->parent->left = nil;

            pNode->right = pNode->parent;
            pNode->parent = pNode->right->parent;
            if (pNode->parent == nil) {
                root = pNode;
            } else {
                if (pNode->right->parent->right == pNode->right)
                    pNode->right->parent->right = pNode;
                else pNode->right->parent->left = pNode;
            }
            pNode->right->parent = pNode;
        }
    };

    void left_rotate(Node<type> *pNode) {
        if (pNode->parent != nil) {
            if (pNode->left != nil) {
                pNode->parent->right = pNode->left;
                pNode->left->parent = pNode->parent;
            } else pNode->parent->right = nil;

            pNode->left = pNode->parent;
            pNode->parent = pNode->left->parent;
            if (pNode->parent == nil) {
                root = pNode;
            } else {
                if (pNode->left->parent->left == pNode->left)
                    pNode->left->parent->left = pNode;
                else pNode->left->parent->right = pNode;
            }
            pNode->left->parent = pNode;
        }
    };

    void fixDeleting(Node<type> *pNode) {
        // далее родственные связи относительно pNode
        while (pNode->color == false and pNode != root) {//pNode — черный узел и не корень
            if (pNode->parent->left == pNode) { //pNode — левый ребенок

                if (brother(pNode)->color == true) {
                    // случай 1: "брат" красный
                    brother(pNode)->color = 0;
                    pNode->parent->color = 1;
                    this->left_rotate(brother(pNode));
                }
                if (brother(pNode)->left->color == 0 and brother(pNode)->right->color == 0) {
                    // случай 2: у "брата" черные дети
                    brother(pNode)->color = 1;
                } else {
                    if (brother(pNode)->right->color == 0) {
                        // случай 3: "брат" красный с черными правым ребенком
                        brother(pNode)->left->color = 0;
                        brother(pNode)->color = 1;
                        this->right_rotate((brother(pNode)->left));
                    }
                    // случай 4: "брат" черный с любыми детьми
                    brother(pNode)->color = pNode->parent->color;
                    pNode->parent->color = 0;
                    brother(pNode)->right->color = 0;
                    this->left_rotate(brother(pNode));
                    pNode = root;
                }
            } else { // pNode — правый ребенок
                // все случаи аналогичны тому, что рассмотрено выше
                if (brother(pNode)->color == true) {
                    brother(pNode)->right->color = false;
                    pNode->parent->color = true;
                    this->right_rotate(pNode->parent);
                }
                if (brother(pNode)->left->color == 0 and brother(pNode)->left->color == 0) {
                    brother(pNode)->color = true;
                } else {
                    if (brother(pNode)->left->color == 0) {
                        brother(pNode)->left->color = 0;
                        brother(pNode)->color = 1;
                        this->left_rotate((brother(pNode)->right));
                    }
                    brother(pNode)->color = pNode->parent->color;
                    pNode->parent->color = 0;
                    brother(pNode)->left->color = 0;
                    this->right_rotate(brother(pNode));
                    pNode = root;
                }
            }
        }
        pNode->color = 0;
        root->color = 0;
    };

    void fixInsert(Node<type> *pNode) {
        // Пока родитель узла красный, есть нарушение свойств красно-черного дерева
        while (pNode != root && pNode->parent->color == true) {
            if (pNode->parent ==
                pNode->parent->parent->left) { // Если родитель узла - левый ребенок его деда (случаи симметричны)
                Node<type> *uncle = pNode->parent->parent->right;
                if (uncle != nil && uncle->color == true) { // Случай 1: Если дядя узла красный
                    // Перекрасить родителя, дядю и дедушку узла
                    pNode->parent->color = false;
                    uncle->color = false;
                    pNode->parent->parent->color = true;
                    // Переместить указатель на узел на его дедушку
                    pNode = pNode->parent->parent;
                } else { // Случай 2: Если дядя узла черный
                    if (pNode == pNode->parent->right) { // Если узел является правым ребенком своего родителя
                        // Поднять узел на уровень выше к его родителю и выполнить левый поворот
                        pNode = pNode->parent;
                        left_rotate(pNode->right);
                    }
                    // Перекрасить родителя и дедушку узла, а затем выполнить правый поворот на дедушке
                    pNode->parent->color = false;
                    pNode->parent->parent->color = true;
                    right_rotate(pNode->parent);
                }
            } else { // Если родитель узла - правый ребенок его деда (случаи симметричны)
                Node<type> *uncle = pNode->parent->parent->left;
                if (uncle != nil && uncle->color == true) {
                    pNode->parent->color = false;
                    uncle->color = false;
                    pNode->parent->parent->color = true;
                    pNode = pNode->parent->parent;
                } else {
                    if (pNode == pNode->parent->left) {
                        pNode = pNode->parent;
                        right_rotate(pNode->left);
                    }
                    pNode->parent->color = false;
                    pNode->parent->parent->color = true;
                    left_rotate(pNode->parent);
                }
            }
        }
        // Убедиться, что корень дерева черный
        root->color = false;
    }


/////////////////// Тут начинается раздел вспомогательных функций////////////////////////
public:
    // The function elemInTree() checks if the <data> element of type <type> exists in the red-black tree
    bool elemInTree(type data) {
        return (pFind(root, data) != nil);
    }

    // The function min() returns minimum value from the red-black tree
    type min() {
        return pMin(root);
    }

    /* Returns red-black tree in easy-to-check format; useful for tests.
     * OutputFormat: {root_node}, parent->{pNode}, parent->{pNode}, ...
     */
    string vectorize() {
        return vectorize(root);
    };

    /*    The pprint function (short for "pretty print") is used to output a red-black tree as a formatted list of nodes.
      Each pNode is displayed with an indent determined by the current level of nesting (the number of "--" characters before the pNode value).
      The output for each pNode is on a separate line and contains the pNode value and its color in square brackets.
    */
    void pprint() {
        pprint(root, 0);
    }

    // The function height() returns minimum value from the red-black tree
    int height() {
        return height(root, 1);
    }

private:
    Node<type> *pMin(Node<type> *pNode) {
        Node<type> *current = pNode;
        while (current->left != nil) {
            current = current->left;
        }
        return current;
    }

    int height(Node<type> *pNode, int curr_height) {
        int heightl = curr_height;
        int heightr = curr_height;
        if (pNode->right != nil) heightr = height(pNode->right, curr_height + 1);
        if (pNode->left != nil) heightl = height(pNode->left, curr_height + 1);
        return max(heightl, heightr);

    }

    /*    The pprint function (short for "pretty print") is used to output a red-black tree as a formatted list of nodes.
      Function arguments:
        pNode: a pointer to the current node, starting from the root of the tree.
        bsc: an integer value that specifies the current level of nesting in the tree.
      Each node is displayed with an indent determined by the current level of nesting (the number of "--" characters before the node value).
      The output for each node is on a separate line and contains the pNode value and its color in square brackets.
    */
    void pprint(Node<type> *pNode, int bsc) {
        for (int i = 0; i < bsc; i++)
            cout << "--";
        if (pNode != nil)
            cout << pNode->value << " [" << pNode->color << "]" << endl;
        if (pNode->left != nil) pprint(pNode->left, bsc + 1);
        if (pNode->right != nil) pprint(pNode->right, bsc + 1);
    }

    string vectorize(Node<type> *pNode) {
        string subStr;
        if (pNode != nil)
            subStr += "{value:" + to_string(pNode->value) + " , color=" + to_string(pNode->color) + "}, ";
        if (pNode->left != nil) subStr += "|" + to_string(pNode->value) + "|->" + vectorize(pNode->left);
        if (pNode->right != nil) subStr += "|" + to_string(pNode->value) + "|->" + vectorize(pNode->right);
        return subStr;
    };

};


int main() {
    RBTree<int> intBT;
    vector<int> a = {6, 3, 8, 2, 5, 7, 79, 8};
    for (int i = 0; i < a.size(); i++) {
        intBT.insert(a[i]);
    }
    intBT.pprint();
    cout << "++++++++++++++++++++++\n";
    intBT.remove(7);
    intBT.pprint();
    cout << intBT.vectorize();
    return 0;
}
//
// Created by zer-oRBT on 25.03.2023.
//