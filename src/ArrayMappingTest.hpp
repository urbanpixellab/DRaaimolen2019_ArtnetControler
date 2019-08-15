//
//  ArrayMappingTest.hpp
//  Sequencer
//
//  Created by Enrico Becker on 31/07/2019.
//

#ifndef ArrayMappingTest_hpp
#define ArrayMappingTest_hpp

#include <stdio.h>
#include "ofMain.h"

#define LENGTH 5

class ArrayMappingTest
{
public:
    ArrayMappingTest();
    ~ArrayMappingTest();
    
    void fillArray(int &value);
    void mapReverseArray();
    void test();
    
    int *getOutArray(){return out;};
    int *getReverseArray(){return *reverse;};
    
private:
    int out[LENGTH];
    
    int *reverse[LENGTH];
};

#endif /* ArrayMappingTest_hpp */
