//
//  Grouping.h
//  HuProject
//
//  Created by Sail on 2021/12/6.
//
#pragma once

#ifndef Grouping_h
#define Grouping_h
#include <stdio.h>
#include "Config.h"
#include "DataRetrive.h"
#include <iostream>
#include <vector>
#include <map>
#include <curl/curl.h>
#include <algorithm>
using namespace std;
void get_treshold(vector<double>& StockSurprise, double& lo, double& hi) {
    sort(StockSurprise.begin(), StockSurprise.end());
    int size = StockSurprise.size();
    int lo_index = size / 3;
    int hi_index = 2 * lo_index;

    lo = StockSurprise[lo_index];
    hi = StockSurprise[hi_index];
}

vector<string> validate_symbollist(vector<string> group, vector<string> Valid_symbols)
{
    sort(group.begin(), group.end());
    sort(Valid_symbols.begin(), Valid_symbols.end());
    vector<string> result(group);
    auto itr = set_intersection(group.begin(), group.end(), Valid_symbols.begin(), Valid_symbols.end(), result.begin());
    result.resize(itr - result.begin());
    return result;
}

void grouping(map<string, StockStruct>& data, vector<double>& StockSurprise, vector<string>& Beat, vector<string>& Meet, vector<string>& Miss, vector<string> Valid_symbols) {
    double lo, hi;
    get_treshold(StockSurprise, lo, hi);

    map<string, StockStruct>::iterator itr;
    for (itr = data.begin(); itr != data.end(); itr++) {
        if (itr->second.Surprise_Percent >= hi) {
            Beat.push_back(itr->first);
            itr->second.Group_Name = "Beat";
        }
        else if (itr->second.Surprise_Percent >= lo) {
            Meet.push_back(itr->first);
            itr->second.Group_Name = "Meet";
        }
        else
            Miss.push_back(itr->first);
        itr->second.Group_Name = "Miss";
    }
    Beat = validate_symbollist(Beat, Valid_symbols);
    Meet = validate_symbollist(Meet, Valid_symbols);
    Miss = validate_symbollist(Miss, Valid_symbols);
}


#endif /* Grouping_h */
