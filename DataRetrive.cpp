//
//  DataRetrive.cpp
//  Licurl
//
//  Created by Katherine Zeng on 12/2/21.
//
#pragma once

#include "DataRetrive.h"
#include "Vector.h"
#include "Config.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <fstream>
#include "curl/curl.h"
#include <ctime>
#include <sstream>
//#include <Windows.h>



void* myrealloc(void* ptr, size_t size)
{
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}
int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)data;
    if ((mem->size + realsize) >= mem->total_size)
        mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
        if (mem->size > mem->total_size)
            mem->total_size = mem->size;
    }
    return realsize;
}
int write_data(void* ptr, size_t size, size_t nmemb, void* data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)data;
    mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

void DataRetriever::populateSymbolVector() {
    /*
    this is a function to populate symbols from a csv file into a vector
    */

    // must add benchmark first
    symbolList.push_back(config.benchmark);
    nameList.push_back(config.benchmark);

    // add component stocks
    ifstream fin;
    fin.open(config.cIWB1000SymbolFile, ios::in);
    string line, name, symbol;
    while (getline(fin, line))
    {
        //fin >> line;
        stringstream sin(line);
        getline(sin, symbol, ',');
        getline(sin, name);
        symbolList.push_back(symbol);
        nameList.push_back(name);

    }
    //fin.close();
}

string DataRetriever::createUrl(const string& symbol) {
    /*-------------------------------------------------------
    this is a member function to create data crapper url.
    member data config and a parameter symbol are used.
    ---------------------------------------------------*/
    string url_request = config.url_common + symbol + ".US?" + "from=" + config.start_date + "&to=" + config.end_date + "&api_token=" + config.api_token + "&period=d";
    return url_request;
}

int DataRetriever::requestMarketData(const string& symbol, StockStruct& stockdata) {
    /*--------------
    this a member function to get data for a single stock
    the steps are:
    1. get data using curl
    2. parse data and store data in "stockdata"

    return type is int. if the process is successful, 0 is returned.
    otherwise, 1 is returned
    ----------------*/
    struct MemoryStruct data;
    data.memory = NULL;
    data.size = 0;

    string url;
    url = createUrl(symbol);
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

    // perform, then store the expected code in result
    CURLcode result;
    result = curl_easy_perform(handle);

    if (result != CURLE_OK)
    {
        // if errors have occured, tell us what is wrong with result
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
        return 1;
    }
    stringstream sData;
    sData.str(data.memory);
    string sValue, sDate;
    double dValue = 0;
    string line;
    while (getline(sData, line)) {
        size_t found = line.find('-');
        if (found != std::string::npos)
        {
            //cout << line << endl;
            sDate = line.substr(0, line.find_first_of(','));
            line.erase(line.find_last_of(','));
            sValue = line.substr(line.find_last_of(',') + 1);
            dValue = strtod(sValue.c_str(), NULL);
            //cout << sDate << " " << std::fixed << ::setprecision(2) << dValue << endl;
            stockdata.adjusted_close.push_back(dValue);
            stockdata.date.push_back(sDate);
        }
    }

    free(data.memory);
    data.size = 0;

    return 0;
}

int DataRetriever::requestMarketData(const string& symbol, StockStruct& stockdata, MemoryStruct data) {
    /*--------------
    this a member function to get data for a single stock
    the steps are:
    1. get data using curl
    2. parse data and store data in "stockdata"

    return type is int. if the process is successful, 0 is returned.
    otherwise, 1 is returned
    ----------------*/


    string url;
    url = createUrl(symbol);
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

    // perform, then store the expected code in result
    CURLcode result;
    result = curl_easy_perform(handle);

    if (result != CURLE_OK)
    {
        // if errors have occured, tell us what is wrong with result
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
        return 1;
    }
    stringstream sData;
    sData.str(data.memory);
    string sValue, sDate;
    double dValue = 0;
    string line;
    while (getline(sData, line)) {
        size_t found = line.find('-');
        if (found != std::string::npos)
        {
            //cout << line << endl;
            sDate = line.substr(0, line.find_first_of(','));
            line.erase(line.find_last_of(','));
            sValue = line.substr(line.find_last_of(',') + 1);
            dValue = strtod(sValue.c_str(), NULL);
            //cout << sDate << " " << std::fixed << ::setprecision(2) << dValue << endl;
            stockdata.adjusted_close.push_back(dValue);
            stockdata.date.push_back(sDate);
        }
    }


    return 0;
}

int DataRetriever::populate_Return_Info() {
    /*-----------
    this is a member function to get data for all the stocks and store in a map
    before executing this function, populateSymbolList must be executed first.
    After successfully executing this function, the member data stockInfo is updated with stock data
    this function may take 1-2 minutes
    use a printf function to show the progress in the consle.
    the reference for the console progress is: https://blog.csdn.net/u014311125/article/details/102798699
    return type is int.
    if the process is successful, 0 is returned. Otherwise,-1 is returned.
    ---------*/

    if (handle)
    {

        //get stocks data
        vector<string>::iterator itr_symbol = symbolList.begin();
        vector<string>::iterator itr_name = nameList.begin();
        int i = 0;
        int all_block_num = symbolList.size();
        struct MemoryStruct data;
        data.memory = NULL;
        data.size = 0;
        data.total_size = 0;
        for (; itr_symbol != symbolList.end(); itr_symbol++)
        {
            // Retrive price data
            //cout << "parsing " << *itr << endl;
            data.size = 0;
            memset(data.memory, '\0', data.total_size);
            StockStruct stockdata;
            stockdata.name = *itr_name;

            itr_name++;
            if (!requestMarketData(*itr_symbol, stockdata, data)) {
                // cout << *itr_symbol << "price_length:"<< stockdata.adjusted_close.size() << endl;
                if (stockdata.date.size() == 0) {
                    //cout << "No price data for " << *itr_symbol << endl;
                    continue;
                }

                // Calculate Daily Return for each stock: first day's return assumed to be 0               
                vector<double>::iterator itr;
                double prev_price = stockdata.adjusted_close[0];
                double cum_return = 0.0;
                for (itr = stockdata.adjusted_close.begin(); itr != stockdata.adjusted_close.end(); itr++)
                {
                    double today_return = (*itr - prev_price) / prev_price;
                    stockdata.Daily_Return.push_back(today_return);
                    cum_return += today_return;
                    stockdata.Cum_Daily_Return.push_back(cum_return);
                    prev_price = *itr;
                }
                // Calcualte AR for each stock: be careful that some stocks' price vectors have different size
                if (*itr_symbol != config.benchmark) {
                    if (stockdata.date.size() == stockInfo[config.benchmark].date.size()) {
                        stockdata.AR = stockdata.Daily_Return - stockInfo[config.benchmark].Daily_Return;
                    }
                    else {
                        vector<string>::iterator itr = std::find(stockInfo[config.benchmark].date.begin(), stockInfo[config.benchmark].date.end(), stockdata.date.front());
                        int start_date_index = std::distance(stockInfo[config.benchmark].date.begin(), itr);
                        itr = std::find(stockInfo[config.benchmark].date.begin(), stockInfo[config.benchmark].date.end(), stockdata.date.back());
                        int end_date_index = std::distance(stockInfo[config.benchmark].date.begin(), itr);
                        vector<double> return_slice(stockInfo[config.benchmark].Daily_Return.begin() + start_date_index, stockInfo[config.benchmark].Daily_Return.begin() + end_date_index + 1);
                        stockdata.AR = stockdata.Daily_Return - return_slice;
                    }
                }
                stockInfo[*itr_symbol] = stockdata;

                //cout << "success" << endl;
            }
            else {
                return -1;
            }

            
            if (i < all_block_num - 1)
            {
                printf("\rReading Data [%.2lf%%]:", i * 100.0 / (all_block_num - 1));
            }
            else
            {
                printf("\rReading Data Completed [%.2lf%%]:", i * 100.0 / (all_block_num - 1));
            }
            int show_num = i * 40 / all_block_num;
            for (int j = 1; j <= show_num; j++)
            {
                std::cout << "█";
            }
            i++;
        }
        free(data.memory);
        data.size = 0;
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return -1;
    }
    return 0;
}

map<string, StockStruct> DataRetriever::getStockInfo() {
    /*------------------
this is a public member function.
if we do not populate the stock data previously, then first we will populate the stock info;
 otherwise, we will directly return the stockInfo.
-----------------*/
    if (stockInfo.empty()) {
        //if not retrieve data, first retrieve data;
        //cout << "start populate symbol" << endl;
        populateSymbolVector();
        //cout << "start populate ReturnInfo" << endl;
        populate_Return_Info();
       // cout << "start populate EarningInfo" << endl;
        populate_Earning_info();
    }
    return stockInfo;

}


void DataRetriever::populate_Earning_info() {
    // Read EarningsAnnouncements.csv
    ifstream fin;
    fin.open(config.cIWB1000EarningFile);
    string line;

    // Skip the first line of cvs file: Ignore the first 500 characters, or until first \n, whichever is met first
    fin.ignore(500, '\n');

    while (!fin.eof())
    {
        fin >> line;
        vector<string> v;
        stringstream ss(line);

        // Split each line into vector by comma
        while (ss.good())
        {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
        }

        /*
         Before populating map, we first need to modify the date formate for Earning_Reporting_Date.
         The date format in EarningAnnoucement.csv is "25-May-21", and we need to convert it to "2021-05-25"
         */
        stringstream date1;
        date1 << v[1];

        struct tm tm;
        date1 >> std::get_time(&tm, "%d-%b-%y");  // here we make some changes so that windows can run without using strptime

        char date2[11];
        strftime(date2, sizeof(date2), "%F", &tm);
        v[1] = date2;

        /*
         Populate map follow the order : v[0] = ticker, v[1] = date, v[2] = period_ending,
         v[3] = estimate, v[4] = reported, v[5] = surprise, v[6] = surprise%
         */
        auto it = stockInfo.find(v[0]);
        if (it != stockInfo.end()) {
            it->second.Earning_Report_Date = v[1];
            it->second.Period_Ending = v[2];
            it->second.Estimated_Earnings = stod(v[3]);
            it->second.Reported_Earnings = stod(v[4]);
            it->second.Surprise = stod(v[5]);
            it->second.Surprise_Percent = stod(v[6]);
            stockSurprise.push_back(stod(v[6]));
            //store the stock surprise in a seperate vector is to calculate the group more conveniently

            vector<string>::iterator itr2 = std::find(it->second.date.begin(), it->second.date.end(), it->second.Earning_Report_Date);
            it->second.Zero_Day = std::distance(it->second.date.begin(), itr2);
            if (it->second.Zero_Day < N || it->second.Zero_Day+N>=it->second.date.size() ){
                Invalid_Symbols.push_back(v[0]);
            }
            else { Valid_Symbols.push_back(v[0]); }
        }
        else {
            cout << "Cannot find in stockinfo: " << v[0] << endl;
            throw "";
        }

    }
}
