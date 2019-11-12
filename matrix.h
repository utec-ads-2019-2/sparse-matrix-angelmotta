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
        Node<T>* node_ptr = nullptr;
        Node<T>* node_prev = nullptr;
        Node<T>* current = nullptr;
        bool done = 0;
        //bool update_operation = 0;
        // Update linked list for row x
        if(vec_rows[x] == nullptr){     // if linked list is empty
            node_ptr = new Node<T>(x,y,value);
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
                    //update_operation = 1;
                    //break;
                    return;
                }
                else if(y > current->col){
                    node_prev = current;
                    current = current->next;    // if next is nullptr insert node after while
                }
                else{  // new node should be a column before the current
                    node_ptr = new Node<T>(x,y,value);
                    if(!node_prev){
                        cout << "before the first and only filled col\n";
                        node_ptr->next = current;
                        current = node_ptr;     // update head = node_ptr
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
                //cout << "go to the last position..push_back \n";
                node_ptr = new Node<T>(x,y,value);
                if(node_prev && y > node_prev->col) node_prev->next = node_ptr;
            }
        }
        // Update Pointers for vertical linked list - column y
        //if(update_operation) return;    // It's Not needed update something
        done = 0;
        node_prev = nullptr;
        current = nullptr;
        if(vec_cols[y] == nullptr){  // if head is nullptr
            //cout << "Empty verical list \n";
            vec_cols[y] = node_ptr;
        }
        else{  // update existing linked list for column y
            current = vec_cols[y];
            // Start New Logic
            while(current){
                if(x > current->row){
                    //cout << "Go down for the next \n";
                    node_prev = current;
                    current = current->down;
                }
                else{
                    if(!node_prev){
                        //cout << "before the first and only in that column...node_ptr->data:" << node_ptr->data << " current->data: " << current->data << "\n";
                        node_ptr->down = current;
                        cout << node_ptr->data << "->" << node_ptr->down->data << "\n";
                        vec_cols[y] = node_ptr;     // update head = node_ptr
                        //cout << "head: " << vec_cols[y]->data << "\n";
                        done = 1;
                        break;
                    }
                    else { // so new node should be between 2 other nodes in that col
                        //cout << "between other 2 nodes in that col \n";
                        auto bkp_node_prev_down = node_prev->down;
                        node_prev->down = node_ptr;
                        node_ptr->down = bkp_node_prev_down;
                        done = 1;
                        break;
                    }
                }
            }
            if(!done){
                //cout << "go to the last position..push_back_down \n";
                if(node_prev && x > node_prev->row) node_prev->down = node_ptr;
            }
        }
    }

    T operator()(unsigned x, unsigned y) const{
        auto current = vec_rows[x];
        while(current){
            if(y == current->col){
                return current->data;
            }
            else if(y > current->col){
                current = current->next;
            }
            else{
                //cout << "Value: No hay valor en esa columna \n";
                return 0;
            }
        }
        //cout << "Val: No hay mas despues de la ultima columna filled \n";
        return 0;
    }

    Matrix<T> operator*(T scalar) const{
        Matrix<T> result(rows, columns);
        cout << "Multiplication by scalar \n";
        //cout << (*this).operator()(0,1) << "\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                auto value = (*this).operator()(i,j);
                if(value != 0) result.set(i,j,value * scalar);
            }
        }
        return result;
    }

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
                        actual = actual->down;      // go down over the list
                    }
                    if(!found) cout << "0 ";
                }
            }
            cout << '\n';
        }
        cout << '\n';
        /*
        cout << "-- Start Test --\n";
        auto current = vec_rows[0];
        while(current){
            cout << current->data << " ";
            current = current->next;
        }
        cout << '\n';
        cout << "-- End Test --\n";
         */
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