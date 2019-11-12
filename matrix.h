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
        //Node<T>* node_ptr = new Node<T>(x,y,value);
        Node<T>* node_prev = nullptr;
        Node<T>* current = nullptr;
        bool done = 0;
        // Update linked list for row x
        if(vec_rows[x] == nullptr){     // if linked list is empty
            Node<T>* node_ptr = new Node<T>(x,y,value);
            vec_rows[x] = node_ptr;
        }
        else{ // if exist at least one node in the row
            current = vec_rows[x];      // first time current is head
            // new logic
            while(current){
                if(current->col == y){
                    cout << "update existing node \n";
                    current->data = value;
                    done = 1;
                    break;
                }
                else if(y > current->col){
                    node_prev = current;
                    current = current->next;
                }
                else{  // new node should be a column before the current
                    Node<T>* node_ptr = new Node<T>(x,y,value);
                    if(!node_prev){
                        cout << "before the first filled col\n";
                        node_ptr->next = current;
                        current = node_ptr;
                        done = 1;
                        break;
                    }
                    else { // so new node should be between 2 other nodes in that row
                        cout << "between other 2 nodes \n";
                        auto bkp_node_prev_next = node_prev->next;
                        node_prev->next = node_ptr;
                        node_ptr->next = bkp_node_prev_next;
                        done = 1;
                        break;
                    }
                }
            }
            if(!done){
                cout << "go to the last position..push_back \n";
                Node<T>* node_ptr = new Node<T>(x,y,value);
                if(node_prev && y > node_prev->col) node_prev->next = node_ptr;
            }
        }
        // Update linked list for column y
        done = 0;
        Node<T>* node_ptr = new Node<T>(x,y,value);
        node_prev = nullptr;
        current = nullptr;
        if(vec_cols[y] == nullptr){  // if head is nullptr
            vec_cols[y] = node_ptr;
        }
        else{  // update existing linked list for column y
            current = vec_cols[y];
            while(x > current->row){
                if(current->down){  // if exist the next node so go down for the next
                    node_prev = current;
                    current = current->down;
                }
                else break;
            }
            if(!node_prev){  // if only exist one node in the linked list
                if(x > current->row){
                    current->down = node_ptr;  // push_back_down
                }
                else{  // push_front_down
                    node_ptr->down = vec_cols[y];   // node_ptr->next = head
                    vec_cols[y] = node_ptr;
                }
            }
            else{   // if exist at least two nodes in the linked list
                if(x > current->row){
                    current->down = node_ptr;   // push_back_down
                }
                else{ // new node is between other 2 nodes
                    node_prev->down = node_ptr;
                    node_ptr->down = current;
                }
            }
        }
    }

    /*void print(){
        for(int i=0; i<rows; ++i){
            if(vec_rows[i] == nullptr){
                for(int j=0; j<columns; ++j) cout << "0 ";
            }
            else{
                auto current = vec_rows[i];
                int j = 0;
                while(current){
                    cout << current->data << " ";
                    current = current->next;
                    j++;
                }
                for(;j<columns;++j){
                    cout << "0 ";
                }
            }
            cout << '\n';
        }
    }*/

    void print(){
        for(int i=0; i<rows; ++i){
            for(int j=0; j<columns; ++j){
                bool found = 0;
                if(vec_cols[j] == nullptr) cout << "0 ";
                else{ // run vertically over the list
                    Node<T>* actual = vec_cols[j];
                    while(actual){
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