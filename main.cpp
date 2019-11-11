#include <iostream>
#include "tester/tester.h"
#include "matrix.h"

int main() {
    Tester::execute();
    Matrix<int> matsparsa(6,6);
    matsparsa.print();
    matsparsa.set(1,3,8);
    matsparsa.print();
    matsparsa.set(0,1,5);
    matsparsa.print();
    matsparsa.set(1,2,4);
    matsparsa.set(1,4,4);
    matsparsa.print();
    return EXIT_SUCCESS;
}

