#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *next, *down;
    T data;
    int row, col;

public:
    void killSelf() {
        // TODO
        if(this->next != nullptr){
            this->next->killSelf();
        }
        delete this;
    }

    Node(int x, int y, T value): row{x}, col{y}, data{value}, next{nullptr}, down{nullptr} {}    // Constructor

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
