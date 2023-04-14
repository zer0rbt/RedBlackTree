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
    Node<type> *nil = new Node<type>();
    Node<type> *root = nil;

    // Операция поиска
    bool find(Node<type> *node, type data) {
        if (node == nil) {
            return false;
        }
        if (data < node->value) {
            return find(node->left, data);
        } else if (data > node->value) {
            return find(node->right, data);
        } else {
            return true;
        }
    }

    // Поиск узла с минимальным значением
    Node<type> *minValueNode(Node<type> *node) {
        Node<type> *current = node;
        while (current->left != nil) {
            current = current->left;
        }
        return current;
    }

    // Вставка
    void insert(type value) {
        Node<type> *node = new Node<type>();
        node->left = nil;
        node->right = nil;
        node->parent = nil;
        node->value = value;

        if (root == nil) {
            root = node; // проблема (?)
        } else {
            Node<type> *parent = nil;
            Node<type> *current = root;
            node->color = 1;
            while (current != nil) {
                parent = current;
                if (node->value < current->value) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }
            node->parent = parent;
            if (node->value < parent->value) {
                parent->left = node;
                parent->right = node;
            }
            fixInsert(node);
        }
    }

    void pprint() {
        pprint(root, 0);
    };

    int height() {
        return height(root, 1);
    }

    void remove(type key) {
        nil->parent = nullptr;


        Node<type> *p = root;
        // находим узел с ключом key
        while (p->value != key) {
            if (p->value < key)
                p = p->right;
            else
                p = p->left;

        }
        // у p нет детей
        if (p->right == nil && p->left == nil) {
            if (p == root)
                root = nil;
            else //ссылку на p у "отца" меняем на nil
            { if (p->parent->right == p) p->parent->right = nil; else p->parent->left = nil; }
            if (p->color == 0) {
                nil->parent = p->parent;
                fixDeleting(nil);
            }
            return;
        }

        Node<type> *y = p;
        if (p->right == nil or p->left == nil)
            p->parent = (p->right == nil) ? p->left : p->right; //ссылку на у от "отца" меняем на ребенка y
        else {
            // два ребенка
            while (y->left != nil) y = y->left;
            if (y->right != nil) //y имеет правого ребенка
                y->right->parent = y->parent;
            if (y == root)//y — корень
                root = y->right;
            else y->parent->left = y->right; //у родителя ссылку на y меняем на ссылку на первого ребенка y

        }
        bool go = (p->color == false);
        if (y != p) {
            p->color = y->color;
            p->value = y->value;
        }
        if (go)
            // при удалении черной вершины могла быть нарушена балансировка
            fixDeleting(p);
    }

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


private:
    Node<type> *brother(Node<type> *pNode) {
        return (pNode->parent->left == pNode) ? (pNode->parent->right) : (pNode->parent->left);
    }

    void fixDeleting(Node<type> *p) {
        // далее родственные связи относительно p
        while (p->color == false and p != root) {//p — черный узел и не корень
            if (p->parent->left == p) { //p — левый ребенок

                if (brother(p)->color == true) {//"брат" красный
                    brother(p)->color = 0;
                    p->parent->color = 1;
                    this->left_rotate(brother(p));
                }
                if (brother(p)->left->color == 0 and brother(p)->right->color == 0)//
                    brother(p)->color = 1;
                else {
                    if (brother(p)->right->color == 0) { // случай, рассматриваемый во втором подпункте:
                        brother(p)->left->color = 0;         // "брат" красный с черными правым ребенком
                        brother(p)->color = 1;
                        this->right_rotate((brother(p)->left));

                    }
                    brother(p)->color = p->parent->color;   // случай, рассматриваемый в последнем подпункте
                    p->parent->color = 0;
                    brother(p)->right->color = 0;
                    this->left_rotate(brother(p));
                    p = root;
                }
            } else { // p — правый ребенок
                // все случаи аналогичны тому, что рассмотрено выше
                if (brother(p)->color == true) {
                    brother(p)->right->color = false;
                    p->parent->color = true;
                    this->right_rotate(p->parent);
                }
                if (brother(p)->left->color == 0 and brother(p)->left->color == 0)//у "брата" черные дети
                    brother(p)->color = true;
                else {
                    if (brother(p)->left->color == 0) {//левый ребенок "брата" черный
                        brother(p)->left->color = 0;
                        brother(p)->color = 1;
                        this->left_rotate((brother(p)->right));
                    }
                    brother(p)->color = p->parent->color;
                    p->parent->color = 0;
                    brother(p)->left->color = 0;
                    this->right_rotate(brother(p));
                    p = root;
                }
            }
        }
        p->color = 0;
        root->color = 0;
    };

    void fixInsert(Node<type> *node) {
        while (node != root && node->parent->color == true) {
            if (node->parent == node->parent->parent->left) {
                Node<type> *uncle = node->parent->parent->right;
                if (uncle != nil && uncle->color == true) {
                    node->parent->color = false;
                    uncle->color = false;
                    node->parent->parent->color = true;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        left_rotate(node);
                    }
                    node->parent->color = false;
                    node->parent->parent->color = true;
                    right_rotate(node->parent->parent);
                }
            } else {
                Node<type> *uncle = node->parent->parent->left;
                if (uncle != nil && uncle->color == true) {
                    node->parent->color = false;
                    uncle->color = false;
                    node->parent->parent->color = true;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        right_rotate(node);
                    }
                    node->parent->color = false;
                    node->parent->parent->color = true;
                    left_rotate(node->parent->parent);
                }
            }
        }
        root->color = false;
    }



    int height(Node<type> *pNode, int curr_height) {
        int heightl = curr_height;
        int heightr = curr_height;
        if (pNode->right != nil) heightr = height(pNode->right, curr_height + 1);
        if (pNode->left != nil) heightl = height(pNode->left, curr_height + 1);
        return max(heightl, heightr);

    }

    void pprint(Node<type> *pNode, int bsc) {
        for (int i = 0; i < bsc; i++)
            cout << "--";
        if (pNode != nil)
            cout << pNode->value << " [" << pNode->color << "]" << endl;
        if (pNode->left != nil) pprint(pNode->left, bsc + 1);
        if (pNode->right != nil) pprint(pNode->right, bsc + 1);

    }


};


int main() {
    RBTree<int> intBT;
    vector<int> a = {6, 3, 8, 2, 5, 7, 79, 8};
    for (int i = 0; i < a.size(); i++) {
        intBT.insert(a[i]);
    }
    intBT.pprint();
    cout << "++++++++++++++++++++++\n";
    intBT.remove(3);
    intBT.pprint();
    intBT.print();
    return 0;
}
//
// Created by zer-oRBT on 25.03.2023.
//