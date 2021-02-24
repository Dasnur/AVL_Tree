#include <iostream>

using namespace std;

class Node {
private:
public:
    Node *left;
    Node *right;
    int64_t data;
    int height;

    Node(int64_t node_data) {
        left = nullptr;
        right = nullptr;
        data = node_data;
        height = 1;
    }

};


class AVLTree {
private:
    Node *root_node;

    void destroyNode(Node *start_node) {
        if (start_node == nullptr)
            return;
        if (start_node->left != nullptr) {
            destroyNode(start_node->left);
        }
        if (start_node->right != nullptr) {
            destroyNode(start_node->right);
        }
        delete start_node;
    }

    Node *fixHeight(Node *cur_node) {
        int h1, h2;

        if (cur_node->left == nullptr)
            h1 = 0;
        else
            h1 = cur_node->left->height;
        if (cur_node->right == nullptr)
            h2 = 0;
        else
            h2 = cur_node->right->height;
        cur_node->height = max(h1, h2) + 1;
        return cur_node;
    }

    int getBalance(Node *cur_node) {
        int h2, h1;
        if (cur_node->left == nullptr)
            h1 = 0;
        else
            h1 = cur_node->left->height;
        if (cur_node->right == nullptr)
            h2 = 0;
        else
            h2 = cur_node->right->height;
        return (h2 - h1);
    }

    Node *rotateRight(Node *cur_node) {
        Node *left_node = cur_node->left;
        cur_node->left = left_node->right;
        left_node->right = cur_node;
        fixHeight(cur_node);
        fixHeight(left_node);
        return left_node;
    }

    Node *rotateLeft(Node *cur_node) {
        Node *right_node = cur_node->right;
        cur_node->right = right_node->left;
        right_node->left = cur_node;
        fixHeight(cur_node);
        fixHeight(right_node);
        return right_node;
    }

    Node *balance(Node *cur_node) {
        fixHeight(cur_node);
        int b = getBalance(cur_node);
        if (b == 2) {
            if (getBalance(cur_node->right) < 0)
                cur_node->right = rotateRight(cur_node->right);
            return rotateLeft(cur_node);
        }
        if (b == -2) {
            if (getBalance(cur_node->left) > 0)
                cur_node->left = rotateLeft(cur_node->left);
            return rotateRight(cur_node);
        }
        return cur_node;
    }

    Node *insertHelper(Node *cur_node, int64_t insert_data) {
        if (cur_node == nullptr)
            return new Node(insert_data);
        if (insert_data < cur_node->data) {
            cur_node->left = insertHelper(cur_node->left, insert_data);
        }
        if (insert_data > cur_node->data) {
            cur_node->right = insertHelper(cur_node->right, insert_data);
        }
        return balance(cur_node);
    }

    Node *searchHelper(Node *cur_node, int64_t search_data) {
        if (cur_node == nullptr || cur_node->data == search_data)
            return cur_node;
        if (search_data > cur_node->data)
            return searchHelper(cur_node->right, search_data);
        if (search_data < cur_node->data)
            return searchHelper(cur_node->left, search_data);
    }

    Node *find_min(Node *cur_node) {
        if (cur_node->left != nullptr)
            find_min(cur_node->left);
        else
            return cur_node;
    }

    Node *deleteHelper(Node *cur_node, int64_t delete_data) {
        if (cur_node == nullptr)
            return nullptr;
        if (delete_data < cur_node->data) {
            cur_node->left = deleteHelper(cur_node->left, delete_data);
            return balance(cur_node);
        }
        if (delete_data > cur_node->data) {
            cur_node->right = deleteHelper(cur_node->right, delete_data);
            return balance(cur_node);
        }
        if (cur_node->left == nullptr)
            return cur_node->right;
        else if (cur_node->right == nullptr)
            return cur_node->left;
        else {
            int64_t min_data = find_min(cur_node->right)->data;
            cur_node->data = min_data;
            cur_node->right = deleteHelper(cur_node->right, min_data);
            return balance(cur_node);
        }
    }

public:
    AVLTree() {
        root_node = nullptr;
    }

    ~AVLTree() {
        destroyNode(root_node);
    };

    void insert(int64_t insert_data) {
        root_node = insertHelper(root_node, insert_data);
    }

    bool exists(int64_t search_data) {
        return (searchHelper(root_node, search_data) != nullptr);
    }

    void delete_key(int64_t delete_data) {
        root_node = deleteHelper(root_node, delete_data);
    }

    pair<int64_t, bool> searchNext(int64_t input_data) {
        Node *cur_node = root_node;
        Node *res = nullptr;
        while (cur_node != nullptr) {
            if (cur_node->data > input_data) {
                res = cur_node;
                cur_node = cur_node->left;
            } else
                cur_node = cur_node->right;
        }
        if (res == nullptr) {
            return make_pair(0, false);
        } else
            return make_pair(res->data, true);
    }

    pair<int64_t, bool> searchPrev(int64_t input_data) {
        Node *cur_node = root_node;
        Node *res = nullptr;
        while (cur_node != nullptr) {
            if (cur_node->data < input_data) {
                res = cur_node;
                cur_node = cur_node->right;
            } else
                cur_node = cur_node->left;
        }
        if (res == nullptr) {
            return make_pair(0, false);
        } else
            return make_pair(res->data, true);
    }

};

int main() {
    AVLTree bst;
    string command_type;
    int64_t command_data;
    pair<int64_t, bool> search_data;
    while (cin >> command_type) {
        cin >> command_data;
        if (command_type == "insert")
            bst.insert(command_data);
        if (command_type == "delete")
            bst.delete_key(command_data);
        if (command_type == "exists")
            cout << (bst.exists(command_data) ? "true" : "false") << endl;
        if (command_type == "next") {
            search_data = bst.searchNext(command_data);
            if (search_data.second)
                cout << search_data.first << endl;
            else
                cout << "none" << endl;
        }
        if (command_type == "prev") {
            search_data = bst.searchPrev(command_data);
            if (search_data.second)
                cout << search_data.first << endl;
            else
                cout << "none" << endl;
        }
    }
    return 0;
}