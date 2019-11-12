#include <iostream>
#include "tester/tester.h"
#include "matrix.h"

int main() {
    Tester::execute();
    Matrix<int> matsparsa(4,4);
    matsparsa.set(0,0,4);
    matsparsa.print();
    matsparsa.set(0,3,5);
    matsparsa.print();
    matsparsa.set(0,1,3);
    matsparsa.print();
    matsparsa.set(0,2,8);
    matsparsa.print();
    matsparsa.set(1,2,5);
    matsparsa.print();
    matsparsa.set(2,2,9);
    matsparsa.print();
    /*
    matsparsa.set(0,0,4);
    matsparsa.print();
    matsparsa.set(0,3,5);
    matsparsa.print();
    matsparsa.set(0,2,3);
    matsparsa.print();
    */
    return EXIT_SUCCESS;
}

