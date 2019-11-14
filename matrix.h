#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include <stdexcept>
#include <vector>
#include <iomanip>
#include "node.h"

using namespace std;

template <typename T>
class Matrix {
private:
    unsigned rows, columns;
    vector<Node<T>*> vec_cols;
    vector<Node<T>*> vec_rows;

    void initMatrix(){
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

public:

    Matrix(unsigned _rows, unsigned _cols) : rows(_rows), columns(_cols) {
        initMatrix();
    }

    Matrix(const Matrix &other) : rows(other.rows), columns(other.columns) { //Copy constructor
        // Init
        initMatrix();
        for(int i=0 ; i < this->rows ; ++i) {
            for(int j=0 ; j < this->columns ; ++j) {
                set(i, j, other.operator()(i, j) );
            }
        }
    }

    void deleteNode(unsigned x, unsigned y){
        Node<T>* node_bkp_down = nullptr;
        Node<T>* node_bkp_next = nullptr;
        if(vec_cols[y]->row == x){  // si el head apunta al nodo que deseamos eliminar
            // Delete for Y
            node_bkp_down = vec_cols[y]->down;
            delete vec_cols[y];
            vec_cols[y] = node_bkp_down;
            // Delete for X
            return;
        }
        auto current = vec_cols[y];
        while(current->down && current->down->row < x){
            current = current->down;
        }

        if(current->down && current->down->data == x){
            auto node_bkp_down = current->down->down;
            delete current->down;
            current->down = node_bkp_down;
        }
        else return;
        // Horizontalmente
    }

    void set(unsigned x, unsigned y, T value){
        //Node<T>* node_ptr = new Node<T>(x,y,value);
        /*if(value == 0){
            deleteNode(x,y);
            return;
        }*/
        Node<T>* node_ptr = nullptr;
        Node<T>* node_prev = nullptr;
        Node<T>* current = nullptr;
        bool done = 0;
        // Update linked list for row x
        if(vec_rows[x] == nullptr){     // if linked list is empty
            //cout << "Empty horizontal list \n";
            node_ptr = new Node<T>(x,y,value);
            vec_rows[x] = node_ptr;
        }
        else{ // if exist at least one node in the row
            current = vec_rows[x];      // first time current is head
            // new logic
            while(current){
                if(current->col == y){
                    //cout << "xx update existing node xx \n";
                    current->data = value;
                    return;
                }
                else if(y > current->col){
                    node_prev = current;
                    current = current->next;    // if next is nullptr done = 0 and do push_back
                }
                else{  // new node should be a column before the current
                    node_ptr = new Node<T>(x,y,value);
                    if(!node_prev){
                        //cout << "before the first and only filled col\n";
                        node_ptr->next = current;
                        current = node_ptr;     // update head = node_ptr
                        done = 1;
                        break;
                    }
                    else { // so new node should be between 2 other nodes in that row
                        //cout << "node_data: " << node_ptr->data << "between other 2 nodes \n";
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
                        //cout << node_ptr->data << "->" << node_ptr->down->data << "\n";
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

    Matrix <T> transpose() const {
        Matrix<T> result(this->columns, this->rows);    // invierte las dimensiones de la matriz
        for(int i =0; i<this->columns; ++i){
            for(int j=0; j<this->rows; ++j){
                auto value = this->operator()(j,i);
                if(value !=0) result.set(i,j,value);  // swap values
            }
        }
        return result;
    }

    T operator()(unsigned x, unsigned y) const{
        auto current = vec_cols[y];
        while(current){
            if(x == current->row){
                return current->data;
            }
            else if(x > current->row){
                current = current->down;
            }
            else{
                break;
            }
        }
        return 0;
    }

    Matrix<T>& operator=(const Matrix<T> &other){
        for(int i=0; i<other.rows; ++i){
            for(int j=0; j<other.columns; ++j){
                set(i,j,other.operator()(i,j));
            }
        }
        return (*this);
    }

    Matrix<T> operator*(T scalar) const{    // Multiplication by scalar
        Matrix<T> result(this->rows, this->columns);
        //cout << (*this).operator()(0,1) << "\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                auto value = (*this).operator()(i,j);
                if(value != 0) result.set(i,j,value * scalar);
            }
        }
        return result;
    }

    Matrix<T> operator*(Matrix<T> other) const{
        if(!this->columns == other.rows) {
            string msg_error = "Rows and columns between two matrix does not match";
            throw runtime_error(msg_error);
        }
        Matrix <T> result(this->rows, other.columns);
        for (int k = 0; k < other.columns; ++k) {
            for (int i = 0; i<this->rows; ++i) {
                auto suma = 0;
                for (int j = 0; j<this->columns; ++j) {
                    suma += (this->operator()(i, j) * other.operator()(j, k));
                }
                result.set(i, k, suma);
            }
        }
        return result;
    }

    Matrix<T> operator+(Matrix<T> other) const{
        if(this->rows != other.rows || this->columns != other.columns) {
            string msg_error = "Matrix are of different dimensions";
            throw runtime_error(msg_error);
        }
        Matrix<T> resultado(this->rows, this->columns);
        //cout << (*this).operator()(0,1) << "\n";
        //cout << other.operator()(0,2) << "\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                auto value_m1 = (*this).operator()(i,j);
                auto value_m2 = other.operator()(i,j);
                auto res = value_m1 + value_m2;
                //cout << "sum("  << i << "," << j << ") " << value_m1 << "+" << value_m2 << " -> " << res << '\n';
                resultado.set(i,j,res);
                //cout << "stored: " << resultado(i,j) << '\n';
            }
        }
        return resultado;
    }

    Matrix<T> operator-(Matrix<T> other) const{
        if(this->rows != other.rows || this->columns != other.columns) {
            string msg_error = "Matrix are of different dimensions";
            throw runtime_error(msg_error);
        }
        Matrix<T> resultado(this->rows, this->columns);
        //cout << (*this).operator()(0,1) << "\n";
        //cout << other.operator()(0,2) << "\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                auto value_m1 = (*this).operator()(i,j);
                auto value_m2 = other.operator()(i,j);
                auto res = value_m1 - value_m2;
                //cout << "resta("  << i << "," << j << ") " << value_m1 << "+" << value_m2 << " -> " << res << '\n';
                resultado.set(i,j,res);
                //cout << "stored: " << resultado(i,j) << '\n';
            }
        }
        return resultado;
    }

    void print2(){
        for(int i=0;i<rows;++i){
            auto node_ptr = vec_rows[i];
            while(node_ptr){
                cout << node_ptr->data << " ";
                node_ptr = node_ptr->next;
            }
            cout << '\n';
        }
        cout << '\n';
    }

    void print(){
        for(int i=0; i<rows; ++i){
            for(int j=0; j<columns; ++j){
                bool found = 0;
                if(vec_cols[j] == nullptr) cout << setw(4) << "0";
                else{ // run vertically over the list
                    Node<T>* actual = vec_cols[j];
                    while(actual){
                        if(actual->row == i){
                            found = 1;
                            cout << setw(4) << actual->data;
                            break;
                        }
                        if(actual->row > i){    // avoid unuseful iterations
                            break;
                        }
                        actual = actual->down;      // go down over the list
                    }
                    if(!found) cout << setw(4) << "0";
                }
            }
            cout << '\n';
        }
        cout << '\n';
    }

    ~Matrix(){
        //cout << "Destructor Matrix Sparse" << '\n';
        for(int i=0; i<rows; ++i){
            if(vec_rows[i] != nullptr){
                //cout << "Kill en fila: " << i << " " << vec_rows[i]->data << " " << '\n';
                vec_rows[i]->killSelf();
            }
        }
    }
};

#endif //SPARSE_MATRIX_MATRIX_H