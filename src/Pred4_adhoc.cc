// adhoc Pred4

#include "Pred4.h"
#include <stdio.h>
#include <iostream>

int main()
{
    Pred4 predictor;
    predictor.print_contents();
    Pred4 predictor2(4, 4, 4);
    //predictor2.print_contents();
    //Predicting
    int *pt_a = reinterpret_cast<int*>(0x0000000000400506);
    bool pred = predictor.predict(pt_a);
    std::cout << "Prediction was " << pred << std::endl;
    /* std::cout << "Performing 1st update" << std::endl;
    predictor2.update(pt_a, true);
    std::cout << "Performning 2nd update" << std::endl;
    predictor2.update(pt_a, true);
    std::cout << "Final print" << std::endl;
    predictor2.print_contents();
    bool pred2 = predictor2.predict(pt_a);
    std::cout << "Prediction was " << pred2 << std::endl;
} */
}