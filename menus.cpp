//
//  menu.cpp
//  FRE6883_TeamProject
//
//  Created by Yiyu Gan on 12/11/21.
//
#pragma once

#include <iomanip>
#include "menus.h"
#include <iostream>
#include "Calculation.h"
#include "Grouping.h"
#include "Config.h"
#include <string>
#include <fstream> 
using namespace std;

// Gnuplot
void plotResults(vector<vector<vector<double>>> finalres) {
    FILE* gnuplotPipe, * tempDataFile;
    const char* tempDataFileName1 = "Beat";
    const char* tempDataFileName2 = "Meet";
    const char* tempDataFileName3 = "Miss";
    const char* tempDataFileName0 = "Report Date";
    int dataSize = finalres[0][0].size() - 1;
    double x, y, x2, y2, x3, y3, x4, y4;
    int i;
    //gnuplotPipe = _popen("C:\\PROGRA~1\\gnuplot\\bin\\gnuplot.exe", "w");    for PC
    gnuplotPipe = _popen(CONFIG.gnu_location, "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempDataFileName1, tempDataFileName2, tempDataFileName3, tempDataFileName0);
        fflush(gnuplotPipe);

        fopen_s(&tempDataFile, tempDataFileName1, "w");
        int index_move = dataSize / 2;
        for (i = 0; i <= dataSize; i++) {
            x = i;
            y = finalres[0][2][i];
            fprintf(tempDataFile, "%lf %lf\n", x - dataSize / 2, y-finalres[0][2][index_move]);
        }
        fclose(tempDataFile);

        fopen_s(&tempDataFile, tempDataFileName2, "w");
        for (i = 0; i <= dataSize; i++) {
            x2 = i;
            y2 = finalres[1][2][i];
            fprintf(tempDataFile, "%lf %lf\n", x2 - dataSize / 2, y2 - finalres[1][2][index_move]);
        }
        fclose(tempDataFile);

        fopen_s(&tempDataFile, tempDataFileName3, "w");
        for (i = 0; i <= dataSize; i++) {
            x3 = i;
            y3 = finalres[2][2][i];
            fprintf(tempDataFile, "%lf %lf\n", x3 - dataSize / 2, y3 - finalres[2][2][index_move]);
        }
        fclose(tempDataFile);

        fopen_s(&tempDataFile, tempDataFileName0, "w");
        int N = 20;
        double lower_bound1 = *min_element(finalres[0][2].begin(), finalres[0][2].end());
        double upper_bound1 = *max_element(finalres[0][2].begin(), finalres[0][2].end());
        double lower_bound2 = *min_element(finalres[1][2].begin(), finalres[1][2].end());
        double upper_bound2 = *max_element(finalres[1][2].begin(), finalres[1][2].end());
        double lower_bound3 = *min_element(finalres[2][2].begin(), finalres[2][2].end());
        double upper_bound3 = *max_element(finalres[2][2].begin(), finalres[2][2].end());

        vector<double> lower_bound;
        lower_bound.push_back(lower_bound1);
        lower_bound.push_back(lower_bound2);
        lower_bound.push_back(lower_bound3);
        double lower = *min_element(lower_bound.begin(), lower_bound.end());

        vector<double> upper_bound;
        upper_bound.push_back(upper_bound1);
        upper_bound.push_back(upper_bound2);
        upper_bound.push_back(upper_bound3);
        double upper = *max_element(upper_bound.begin(), upper_bound.end());



        for (i = -5; i <= N + 5; i++) {
            x4 = 0;
            y4 = lower + i * (upper - lower) / N;
            fprintf(tempDataFile, "%lf %lf\n", x4, y4);
        }
        fclose(tempDataFile);

        printf("press enter to continue...");
        getchar();

        fprintf(gnuplotPipe, "exit \n");
    }
    else {
        printf("gnuplot not found...");
    }
}



void Menu()
{
    //static map<string, StockStruct> stockInfo;
    static map<string, StockStruct> Data;// = DR.getStockInfo();
    static int if_case1 = 0;
    static vector<double> surprise;
    static vector<string> ValidSymbols;
    static vector<string> Beat, Meet, Miss;
    static int N;
    static string ticker;
    static vector<vector<vector<double>>> finalres;
    static int if_case3 = 0;
    
    DataRetriever stock;
    int option;

    while (true) {
        cout << "--------Main Menu----------" << endl;
        cout << "Please choose from the following five options: " << endl;
        cout << "1: Enter N to retrieve historical price data for all stocks " << endl;
        cout << "2: View one stock infomations " << endl;
        cout << "3: Bootstrap and show AAR, AAR-STD, CAAR, CAAR-STD for one group " << endl;
        cout << "4: Show the gnuplot graph with CAAR for all 3 group " << endl;
        cout << "5: Exit the program " << endl;
        cout << "---------Please select your option:--------" << endl;
        cin >> option;
        switch (option)
        {
            // retrieve historical price data for all stocks
        case 1:
        {
            cout << "--------Please enter N: ---------" << endl;

            cin >> N;
            if(N < 60)
            {
                cout << "Sorry, N is smaller than 60. It has to be greater than or equal to 60. Please reenter: " << endl;
                cin >> N;
            }
            else if(N > 120) {
                cout << "Warning: N is greater than 120 (two quaters). We will searh the maximum(120) for you. " << endl;
                N = 120;
            }
            clock_t t1, t2;
            t1 = clock();
            //code goes here
            
            stock.setN(N);
            Data = stock.getStockInfo();
            surprise = stock.getSurprice();
            ValidSymbols = stock.getValidSymbols();
            grouping(Data, surprise, Beat, Meet, Miss, ValidSymbols);
            if_case1 = 1;
            if_case3 = 0;
            t2 = clock();
            float diff((float)t2 - (float)t1);
            diff = diff / 1000;
            cout << "Data Retriever Done!! " <<"Time taken: "<<diff<<" seconds"<< endl;
            break;
        }
        // View one stock infomations, including daily prices, cumulative daily returns, the group the stock belongs to, earning announcement date, period ending, estimated, reported earnings, surprise and surprise%
        case 2: //bug
        {
            if (if_case1 == 0) {
                cout << "Please first retrieve historical data" << endl;
                cout << "...Going back to menu" << endl;
                Sleep(10);
                continue;
            }
            //StockStruct stockdata;
            cout << "---------Please enter the stock ticker that you would like to view: ---------" << endl;
            cin >> ticker;
            //getline(cin, ticker);
            cout << "=============Below is information for "<<ticker<<"============"<<endl;

            auto it = Data.find(ticker);
            if (it != Data.end())
            {
                cout << "Date: " << it->second.date << endl << endl;
                cout << "Daily Price: " << it->second.adjusted_close << endl<<endl;
                cout << "Cumulative daily return: " << it->second.Cum_Daily_Return << endl << endl;
                cout << "The group the stock belongs to: " << it->second.Group_Name << endl << endl;
                cout << "Earning announcement date: " << it->second.Earning_Report_Date << endl << endl;
                cout << "Period ending: " << it->second.Period_Ending << endl << endl;
                cout << "Estimated: " << it->second.Estimated_Earnings << endl << endl;
                cout << "Reported earnings: " << it->second.Reported_Earnings << endl << endl;
                cout << "Surprise: " << it->second.Surprise << endl << endl;
                cout << "Surprise%: " << it->second.Surprise_Percent << endl << endl;
            }
            else
                cout << endl<< "Sorry, we do not have infomation for this stock currently. " << endl << endl;

            break;
        }

        // Show AAR, AAR-STD, CAAR, and CAAR-STD for one group
        case 3:
        {
            if (if_case1 == 0) {
                cout << "Please first retrieve historical data" << endl;
                continue;
            }
            clock_t t1, t2;
            t1 = clock();
            Calculation cal(N, Data);
            finalres = cal.Bootstrap(Beat, Meet, Miss);
            if_case3 = 1;//switch the boostrap status to finished;
            t2 = clock();
            cout << "Boostrap Finished! Time taken: " << ((float)t2 - float(t1)) / 1000 << " seconds" << endl;
            cout << "Please choose a group you would like to view from the following: " << endl;
            cout << "1. Beat Group" << endl;
            cout << "2. Meet Group" << endl;
            cout << "3. Miss Group" << endl;
            cout << "4. Omit" << endl;

            int choice;
            cin >> choice;
            if (choice == 1)
            {
                cout << "Beat Group statistics" << endl;
            }
            else if (choice == 2)
            {
                cout << "Meat Group statistics" << endl;
            }
            else if (choice == 3)
            {
                cout << "Miss Group statistics" << endl;
            }
            else if (choice == 4) {
                break;
            }
            else { cout << "Please enter a valid option!" << endl; }
            
//            cout << setw(5) << "Avg_AAR" << setw(10) << "AAR_STD" << setw(10) << "Avg_CAAR" << setw(10) << "CAAR_STD" << endl;
            int i = choice - 1;
            vector<string> v = { "Beat","Meet","Miss" };
            cout << "--------------result for group " << v[i] << "--------------" << endl;
            cout << setw(5) << "Avg_AAR" << endl << setw(10) << finalres[i][0] << endl << endl << setw(10) << "AAR_STD" << endl << setw(10) << finalres[i][1] << endl << endl << "Avg_CAAR" << endl << setw(10) << finalres[i][2] << endl << endl << setw(10) << "CAAR_STD" << endl << setw(10) << finalres[i][3] << endl;
            //fout << setw(5) << "Avg_AAR" << setw(10) << "AAR_STD" << setw(10) << "Avg_CAAR" << setw(10) << "CAAR_STD" << endl;
            //fout << setw(5) << finalres[i][0] << setw(10) << finalres[i][1] << setw(10) << finalres[i][2] << setw(10) << finalres[i][3] << endl;
            ofstream fout;
            fout.open("BoostrapResult.txt");
            for (int j = 0; j < 3; j++) {
                fout << "--------------result for group " << v[j] << "--------------" << endl;
                fout << setw(5) << "Avg_AAR" <<endl<< setw(10) << finalres[j][0] << endl << endl << setw(10) << "AAR_STD" <<endl<< setw(10) << finalres[j][1] << endl << endl << "Avg_CAAR" << endl << setw(10) << finalres[j][2] << endl << endl << setw(10) << "CAAR_STD" << endl << setw(10) << finalres[j][3] << endl;
            }
            break;
            break;
        }


        // Gnuplot
        case 4:
        {
            if (if_case1 == 0) {
                cout << "Please first retrieve historical data!" << endl << endl;
                continue;
            }
            if (if_case3 == 0)
            {
                cout << "Please first bootstrap" << endl << endl;
                continue;
            }
            //cout << finalres << endl;
            plotResults(finalres);
            break;
        }


        // Exit the program
        case 5:
            cout << "Exit the program..." << endl;
            Sleep(10);
            exit(0);

        default:
            cout << "Please enter a valid option." << endl;
        }
    }

}
