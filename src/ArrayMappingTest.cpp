//
//  ArrayMappingTest.cpp
//  Sequencer
//
//  Created by Enrico Becker on 31/07/2019.
//

#include "ArrayMappingTest.hpp"

ArrayMappingTest::ArrayMappingTest()
{
    float t1 = ofGetElapsedTimef();
    int v = 0;
    fillArray(v);
    float t2 = ofGetElapsedTimef();
    cout << "used time " << t2-t1 << endl;
    mapReverseArray();
    test();
}

ArrayMappingTest::~ArrayMappingTest(){}

void ArrayMappingTest::fillArray(int &value)
{
    for(int i = 0;i < LENGTH;i++)
    {
        out[i] = value;
    }
}

void ArrayMappingTest::mapReverseArray()
{
    for (int i = 0; i < LENGTH;i++)
    {
        reverse[i] = &out[LENGTH-1-i];
    }
}

void ArrayMappingTest::test()
{
    cout << "out0 " << out[0] << endl;
    *reverse[4] = 12;
    cout << "out0 nach mapping " << out[0] << endl;
    out[0] = 11;
    cout << "out0 neu beschrieben " << out[0] << endl;
    *reverse[4] = 10;
    cout << "out0 nach mapping " << out[0] << endl;

}
