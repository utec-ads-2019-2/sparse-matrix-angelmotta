#include <iostream>
#include "tester/tester.h"
#include "matrix.h"

int main() {
    //Tester::execute();
    // Transpose
    Matrix<int> matsparsa(3,3);
    matsparsa.set(0,0,1);
    matsparsa.set(0,1,2);
    matsparsa.set(0,2,3);
    matsparsa.set(1,0,4);
    matsparsa.set(1,1,5);
    matsparsa.set(1,2,6);
    matsparsa.print();
    auto matTranspose = matsparsa.transpose();
    matTranspose.print();
    /*
    Matrix<int> matsparsa(4,4);
    matsparsa.set(0,0,4);
    //matsparsa.print();
    matsparsa.set(0,3,5);
    //matsparsa.print();
    matsparsa.set(0,1,3);
    //matsparsa.print();
    matsparsa.set(0,2,8);
    //matsparsa.print();
    matsparsa.set(1,2,5);
    //matsparsa.print();
    matsparsa.set(2,2,9);
    matsparsa.print();
    cout << " -- MULTI -- \n";
    auto mat_multi = matsparsa * 3;
    mat_multi.print();
     */
    // Suma
    /*cout << " -- SUMA -- \n";
    Matrix<int> mat2sparsa(4,4);
    mat2sparsa.set(0,0,4);
    mat2sparsa.set(1,0,2);
    mat2sparsa.set(2,0,2);
    mat2sparsa.print();
    auto mat_sum = matsparsa + mat2sparsa;
    mat_sum.print();
    cout << "--- SUMA -- \n";*/
    return EXIT_SUCCESS;
}

