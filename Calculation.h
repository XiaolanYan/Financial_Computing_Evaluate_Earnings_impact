//
//  Calculation.hpp
//  Licurl
//
//  Created by Katherine Zeng on 12/2/21.
//
#pragma once

#ifndef Calculation_hpp
#define Calculation_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "DataRetrive.h"
#include "Vector.h"
using namespace std;


class Calculation
{
private:
    int N;
    const map<string, StockStruct> stockinfo;

public:
    // Constructor with parameter
    Calculation(int N_, const map<string, StockStruct> ptr_to_info) : N(N_), stockinfo(ptr_to_info)
    {
        srand((unsigned)time(NULL));
    }
    vector<double> Calculate_AAR(vector<string> Group_List);
    vector<double> Calculate_CAAR(const vector<double>& AAR);
    vector<vector<vector<double>>> Bootstrap(const vector<string>& Beat, const vector<string>& Meet, const vector<string>& Miss);

    ~Calculation() {}


};

#endif /* Calculation_hpp */
