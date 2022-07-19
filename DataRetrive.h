//
//  DataRetrive.hpp
//  Licurl
//
//  Created by Katherine Zeng on 12/2/21.
//
#pragma once
#ifndef DataRetrive_hpp
#define DataRetrive_hpp

#include <stdio.h>

#include "Config.h"
#include <iostream>
#include <vector>
#include <map>
#include <curl/curl.h>
using namespace std;
struct StockStruct {
    vector<double> adjusted_close;
    vector<string> date;
    string name = "";//stock full name
    string Group_Name;//to be filled by grouping function
    string Earning_Report_Date;
    int Zero_Day; // index of Earning Report Date in date(vector<string>)
    string Period_Ending;
    double Estimated_Earnings;
    double Reported_Earnings;
    double Surprise;
    double Surprise_Percent;
    vector<double> Daily_Return;
    vector<double> AR;
    vector<double> Cum_Daily_Return;// it seems this needs to be included in stock information
};

struct MemoryStruct {
    char* memory;
    size_t size;
    size_t total_size;
};

class DataRetriever {
private:
    Config config;
    vector<string> symbolList;  // contains IWB // I wish symbolList could be populated by EarningAnnouncement.csv, which will save a lot of trouble
    vector<string> nameList;  // contains IWB
    map<string, StockStruct> stockInfo;  // contains IWB
    vector<string> Valid_Symbols;  // those stocks in EarningAnnouncement.csv with enough historical price for 2N+1
    vector<string> Invalid_Symbols;  // those stocks in EarningAnnouncement.csv without enough historical price for 2N+1
    vector<double> stockSurprise;
    CURL* handle;
    int N;

    //populate symbollist and namelist into member data symbolList and nameList
    void populateSymbolVector();
    //create url for a given symbol
    string createUrl(const string& symbol);
    //request, clean and store data for a given stock
    int requestMarketData(const string& symbol, StockStruct& stockdata);//single request for a single stock
    int requestMarketData(const string& symbol, StockStruct& stockdata, MemoryStruct data);//single request for a single stock

                                                                        //populate all the stocks and the benchmark index info into stockInfo
    int populate_Return_Info();

    // Read data from EarningAnouncement.csv and store in the map stockInfo
    void populate_Earning_info();


public:
    //constructor
    DataRetriever(int N_) :N(N_) {
        // set up the program environment that libcurl needs
        curl_global_init(CURL_GLOBAL_ALL);
        // curl_easy_init() returns a CURL easy handle
        handle = curl_easy_init();
    }
    
    DataRetriever() :N(60){
        // set up the program environment that libcurl needs
        curl_global_init(CURL_GLOBAL_ALL);
        // curl_easy_init() returns a CURL easy handle
        handle = curl_easy_init();
    }
    ~DataRetriever() {
        // cleanup since you've used curl_easy_init
        curl_easy_cleanup(handle);
        // release resources acquired by curl_global_init()
        curl_global_cleanup();
    }
    // get stock adjust close and date data from curl
    map<string, StockStruct> getStockInfo();
    vector<string> getsymbolList() const { return symbolList; }
    vector<string> getValidSymbols() const { return Valid_Symbols; }
    vector<string> getInvalidSymbols() const { return Invalid_Symbols; }
    vector<double> getSurprice() const { return stockSurprise; }
    void setN(int N_) { N = N_; }
};
#endif /* DataRetrive_hpp */
