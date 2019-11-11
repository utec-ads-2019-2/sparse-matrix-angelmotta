#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include "node.h"

using namespace std;

template <typename T>
class Matrix {
private:
    Node<T> *root;
    unsigned rows, columns;
    vector<Node<T>*> vec_cols;
    vector<Node<T>*> vec_rows;

    void push_front(Node<T>** head, Node<T>** node_ptr) {
        if(*head == nullptr){
            *head = *node_ptr;
        }
        else{
            (*node_ptr)->next = *head;
            *head = *node_ptr;
        }
    }

public:
    /*
    Matrix(unsigned rows, unsigned columns);

    void set(unsigned, unsigned, T);
    T operator()(unsigned, unsigned) const;
    Matrix<T> operator*(T scalar) const;
    Matrix<T> operator*(Matrix<T> other) const;
    Matrix<T> operator+(Matrix<T> other) const;
    Matrix<T> operator-(Matrix<T> other) const;
    Matrix<T> transpose() const;
    void print() const;

    ~Matrix();
     */
    Matrix(unsigned _rows, unsigned _cols) : rows(_rows), columns(_cols) {
        vec_rows.reserve(rows);
        for(int i=0; i<rows; ++i){
            Node<T>* head = nullptr;
            vec_rows.push_back(head);
        }
        vec_cols.reserve(rows);
        for(int j=0; j<columns; ++j){
            Node<T>* head = nullptr;
            vec_cols.push_back(head);
        }
    }

    void set(unsigned x, unsigned y, T value){
        Node<T>* node_ptr = new Node<T>(x,y,value);
        // Update linked list for row x
        if(vec_rows[x] == nullptr){
            vec_rows[x] = node_ptr;
            //push_front(&vec_rows[x], &node_ptr);
        }
        else{
            auto current = vec_rows[x];
            Node<T>* node_prev = nullptr;
            while(y > current->col){
                if(current->next){
                    node_prev = current;
                    current = current->next;   // if next is different to nullptr go for the next
                }
                else break;
            }
            if(!node_prev){ // if new node should be the new head
                push_front(&vec_rows[x], &node_ptr);
            }
            else{
                node_prev->next = node_ptr;
                node_ptr->next = current;
            }
        }
        if(vec_cols[y] == nullptr){
            vec_cols[y] = node_ptr;
            //push_front(&vec_cols[y], &node_ptr);
        }
    }

    void print(){
        for(int i=0; i<rows; ++i){
            for(int j=0; j<columns; ++j){
                bool found = 0;
                if(vec_cols[j] == nullptr) cout << "0 ";
                else{ // run vertically over the list
                    Node<T>* actual = vec_cols[j];
                    while(actual != nullptr){
                        if(actual->row == i){
                            found = 1;
                            cout << actual->data << " ";
                            break;
                        }
                        if(actual->row > i){    // avoid unuseful iterations
                            break;
                        }
                        actual = actual->down;
                    }
                    if(!found) cout << "0 ";
                }
            }
            cout << '\n';
        }
        cout << '\n';
    }

    ~Matrix(){
        cout << "Destructor Matrix Sparse" << '\n';
        for(int i=0; i<rows; ++i){
            if(vec_rows[i] != nullptr){
                cout << "Kill en fila: " << i << '\n';
                vec_rows[i]->killSelf();
            }
        }
    }
};

#endif //SPARSE_MATRIX_MATRIX_H