#include <iostream>
#include <vector>

using namespace std;

template<class type>
struct Node {
    bool color = false; // 1 is red, 0 is false
    type value;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;
};

template<class type>
struct RBTree {
    Node<type> *root = nullptr;

    void push(type data) {
        Node<type> *pNode = new Node<type>();
        pNode->value = data;
        if (root != nullptr)
            push(root, pNode);
        else root = pNode;
    };

    void print() {
        print(root);
    };

    void pprint() {
        pprint(root, 0);
    };

    int height() {
        return height(root, 1);
    }

    void remove(type key) {
        Node<type> *p = root;
        // находим узел с ключом key
        while (p->value != key) {
            if (p->value < key)
                p = p->right;
            else
                p = p->left;
        }
        // у p нет детей
        if (p->right == nullptr && p->left == nullptr) {
            if (p == root)
                root = nullptr;
            else //ссылку на p у "отца" меняем на nil
            { if (p->parent->right == p) p->parent->right; else p->parent->left; }
            return;
        }

        Node<type> y = nullptr;
        Node<type> q = nullptr;
        if (p->right == nullptr or p->left == nullptr)
            p->parent == (p->right == nullptr)?p->left:p->right; //ссылку на у от "отца" меняем на ребенка y
        else{
        // два ребенка
        y = вершина, со следующим значением ключа // у нее нет левого ребенка
        if y имеет правого ребенка
        y.right.parent = y.parent
        if y — корень
                root = y.right
        else
        у родителя ссылку на y меняем на ссылку на первого ребенка y}
        if y != p
        p.colour = y.colour
        p.key = y.key
        if y.colour == black
        // при удалении черной вершины могла быть нарушена балансировка
        fixDeleting(q)
    }

    void right_rotate(Node<type> *pNode) {

        if (pNode->parent != nullptr) {
            if (pNode->left != nullptr) {
                pNode->parent->right = pNode->left;
                pNode->left->parent = pNode->parent;
            } else pNode->parent->right = nullptr;

            pNode->left = pNode->parent;
            pNode->parent = pNode->left->parent;
            if (pNode->parent == nullptr) {
                root = pNode;
            } else {
                if (pNode->left->parent->right == pNode->left)
                    pNode->left->parent->right = pNode;
                else pNode->left->parent->left = pNode;
            }
            pNode->left->parent = pNode;
        }
    };

    void left_rotate(Node<type> *pNode) {

        if (pNode->parent != nullptr) {
            if (pNode->right != nullptr) {
                pNode->parent->left = pNode->right;
                pNode->right->parent = pNode->parent;
            } else pNode->parent->left = nullptr;

            pNode->right = pNode->parent;
            pNode->parent = pNode->right->parent;
            if (pNode->parent == nullptr) {
                root = pNode;
            } else {
                if (pNode->right->parent->left == pNode->right)
                    pNode->right->parent->left = pNode;
                else pNode->right->parent->right = pNode;
            }
            pNode->right->parent = pNode;
        }
    };


private:
    void push(Node<type> *pNode, Node<type> *pNewNode) {
        if (pNode->value <= pNewNode->value) {
            if (pNode->right == nullptr) {
                pNode->right = pNewNode;
                pNewNode->parent = pNode;
            } else push(pNode->right, pNewNode);
        } else if (pNode->left == nullptr) {
            pNode->left = pNewNode;
            pNewNode->parent = pNode;
        } else push(pNode->left, pNewNode);
    };


    int height(Node<type> *pNode, int curr_height) {
        int heightl = curr_height;
        int heightr = curr_height;
        if (pNode->right) heightr = height(pNode->right, curr_height + 1);
        if (pNode->left) heightl = height(pNode->left, curr_height + 1);
        return max(heightl, heightr);

    }

    void print(Node<type> *pNode) {
        if (pNode->right) subprint(pNode->right);
        cout << pNode->value << " ";
        if (pNode->left) subprint(pNode->left);
    };

    void pprint(Node<type> *pNode, int bsc) {
        for (int i = 0; i < bsc; i++)
            cout << " ";
        cout << pNode->value << endl;
        if (pNode->left) pprint(pNode->left, bsc + 1);
        if (pNode->right) pprint(pNode->right, bsc + 1);

    }


};


int main() {
    RBTree<int> intBT;
    vector<int> a = {6, 4, 2, 6, 8, 1, 9, 2, 10, 0};

    for (int i = 0; i < a.size(); i++)
        intBT.push(a[i]);

    intBT.pprint();
    intBT.right_rotate(intBT.root->right);
    intBT.left_rotate(intBT.root->left);
    intBT.pprint();
    return 0;
}
//
// Created by zer-oRBT on 25.03.2023.
//
