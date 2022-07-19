//
//  Config.hpp
//  Licurl
//
//  Created by Katherine Zeng on 12/2/21.
//

#ifndef Config_hpp
#define Config_hpp

#include <stdio.h>
#pragma once
#include <iostream>
using namespace std;
struct Config {

    const string start_date = "2020-11-01";
    const string end_date = "2021-11-01";
    const char* cIWB1000SymbolFile = "Russell_1000_component_stocks.csv";
    const char* cIWB1000EarningFile = "EarningsAnnouncements.csv";
    const string api_token = "61a4f06fbdca60.59813312";
    const string url_common = "https://eodhistoricaldata.com/api/eod/";
    const string benchmark = "IWB";
    const char* gnu_location = "D:/gnuplot/bin/gnuplot.exe";
};

static Config CONFIG;
#endif /* Config_hpp */
