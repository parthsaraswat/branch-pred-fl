#include "Pred3.h"
#include <stdio.h>
#include <iostream>

int main()
{
    // Creating a 1 level BHT with 32 entries
    Pred3 p1(5);
    std::cout << "The mask used in this case will be " << p1.mask_gen(5) << std::endl;

    // Putting a variable on the stack in order to get a pointer
    int a = 4;
    int *pt_a = &a;

    // Performing a prediction
    std::cout << "Performing first prediction. Given that the states are all initialised to taken, we expect the prediction to be = 1 (taken)" << std::endl;
    bool pred = p1.predict(pt_a);
    std::cout << "THE PREDICTION IS " << pred << std::endl;

    // Performing 2 updates in order to see the state transition from T -> NT -> SNT
    std::cout << "PERFORMING FIRST UPDATE: " << std::endl;
    p1.update(pt_a, false);
    std::cout << "PERFORMING SECOND UPDATE:" << std::endl;
    p1.update(pt_a, false);

    // Performing 2nd prediction after the 2 updates
    std::cout << "Performing second prediction. Given that the state has been updated to STRONGLY NOT TAKEN, we expect the prediction to be = 0 (not taken)" << std::endl;
    bool pred2 = p1.predict(pt_a);
    std::cout << "THE PREDICTION IS " << pred2 << std::endl;

    return 0;
}