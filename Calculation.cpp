//
//  Calculation.cpp
//  Licurl
//
//  Created by Katherine Zeng on 12/2/21.
//

#include "Calculation.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <random>


// Group_list contains the stock symbol in this group
vector<double> Calculation::Calculate_AAR(vector<string> Group_List) {
    random_device rng;
    mt19937 urng(rng());
    shuffle(Group_List.begin(), Group_List.end(), urng);
    vector<double> AAR;
    for (int day = 0; day < 2 * N + 1; day++) {
        double res = 0.0;
        vector<string>::iterator itr1;
        for (int i = 0; i < 80; i++) {
            auto it = stockinfo.find(Group_List[i]);
            if (it != stockinfo.end()) {
                res += it->second.AR[it->second.Zero_Day - N + day];
            }
            else { 
                cout << "Cannot find in stockinfo: " << Group_List[i] << endl; 
            }
        }
        res /= 80;
        AAR.push_back(res);
    }
    return AAR;
}


vector<double> Calculation::Calculate_CAAR(const vector<double>& AAR) {
    vector<double> CAAR;
    CAAR.push_back(AAR[0]);
    for (int i = 1; i < 2 * N + 1; i++) {
        CAAR.push_back(CAAR[i - 1] + AAR[i]);
    }
    return CAAR;
}


/*
 Assume Grouping function return three Group Name vector<string>
 Bootstramp return a matrix that each cell contains vector<double>

 final results are something like this
 //        Avg_AAR          AAR_STD         Avg_CAAR        CAAR_STD
 // Beat  vector(2*N+1)  vector(2*N+1)    vector(2*N+1)   vector(2*N+1)
 // Meet  vector(2*N+1)  vector(2*N+1)    vector(2*N+1)   vector(2*N+1)
 // Miss  vector(2*N+1)  vector(2*N+1)    vector(2*N+1)   vector(2*N+1)

*/
vector<vector<vector<double>>> Calculation::Bootstrap(const vector<string>& Beat, const vector<string>& Meet, const vector<string>& Miss) {
    cout << "start bootstrapping" << endl;
    // Initialize final results
    vector<vector<vector<double>>> finalres;
    finalres.resize(3);
    for (int i = 0; i < 3; i++) {
        finalres[i].resize(4);
    }

    vector<double> Beat_Avg_AAR(2 * N + 1, 0);
    vector<double> Meet_Avg_AAR(2 * N + 1, 0);
    vector<double> Miss_Avg_AAR(2 * N + 1, 0);
    vector<double> Beat_AAR_square(2 * N + 1, 0);
    vector<double> Meet_AAR_square(2 * N + 1, 0);
    vector<double> Miss_AAR_square(2 * N + 1, 0);

    vector<double> Beat_Avg_CAAR(2 * N + 1, 0);
    vector<double> Meet_Avg_CAAR(2 * N + 1, 0);
    vector<double> Miss_Avg_CAAR(2 * N + 1, 0);
    vector<double> Beat_CAAR_square(2 * N + 1, 0);
    vector<double> Meet_CAAR_square(2 * N + 1, 0);
    vector<double> Miss_CAAR_square(2 * N + 1, 0);

    // Repeat 40 times
    for (int i = 0; i < 40; i++) {

        // Get three group stock symol vector<string> from Grouping Function
        // vector<string> Beat, Miss, Meet;  Need to modify with Grouping Function

        vector<double> Beat_AAR = Calculate_AAR(Beat);
        vector<double> Meet_AAR = Calculate_AAR(Meet);
        vector<double> Miss_AAR = Calculate_AAR(Miss);
        vector<double> Beat_CAAR = Calculate_CAAR(Beat_AAR);
        vector<double> Meet_CAAR = Calculate_CAAR(Meet_AAR);
        vector<double> Miss_CAAR = Calculate_CAAR(Miss_AAR);

        Beat_Avg_AAR += Beat_AAR;
        Meet_Avg_AAR += Meet_AAR;
        Miss_Avg_AAR += Miss_AAR;

        // Using Variance[x] = E[x^2] - E^2[x] to calculate standard deviation
        Beat_AAR_square += Beat_AAR * Beat_AAR;
        Meet_AAR_square += Meet_AAR * Meet_AAR;
        Miss_AAR_square += Miss_AAR * Miss_AAR;

        Beat_Avg_CAAR += Beat_CAAR;
        Meet_Avg_CAAR += Meet_CAAR;
        Miss_Avg_CAAR += Miss_CAAR;

        // Using Variance[x] = E[x^2] - E^2[x] to calculate standard deviation
        Beat_CAAR_square += Beat_CAAR * Beat_CAAR;
        Meet_CAAR_square += Meet_CAAR * Meet_CAAR;
        Miss_CAAR_square += Miss_CAAR * Miss_CAAR;

    }

    /*
     Follow the order to fill in the matrix
     finalres[0][0] = Beat Avg AAR; finalres[0][1] = Beat AAR STD; finalres[0][2] = Beat Avg CAAR; finalres[0][3] = Beat CAAR STD;
     finalres[1][0] = Meet Avg AAR; finalres[1][1] = Meet AAR STD; finalres[1][2] = Meet Avg CAAR; finalres[1][3] = Meet CAAR STD;
     finalres[2][0] = Miss Avg AAR; finalres[2][1] = Miss AAR STD; finalres[2][2] = Miss Avg CAAR; finalres[1][3] = Miss CAAR STD;
     */
    finalres[0][0] = Beat_Avg_AAR / 40.0;
    finalres[1][0] = Meet_Avg_AAR / 40.0;
    finalres[2][0] = Miss_Avg_AAR / 40.0;

    // Using Variance[x] = E[x^2] - E^2[x] to calculate standard deviation
    finalres[0][1] = sqrt2(Beat_AAR_square / 40.0 - finalres[0][0] * finalres[0][0]);
    finalres[1][1] = sqrt2(Meet_AAR_square / 40.0 - finalres[1][0] * finalres[1][0]);
    finalres[2][1] = sqrt2(Miss_AAR_square / 40.0 - finalres[2][0] * finalres[2][0]);

    finalres[0][2] = Beat_Avg_CAAR / 40.0;
    finalres[1][2] = Meet_Avg_CAAR / 40.0;
    finalres[2][2] = Miss_Avg_CAAR / 40.0;

    finalres[0][3] = sqrt2(Beat_CAAR_square / 40.0 - finalres[0][2] * finalres[0][2]);
    finalres[1][3] = sqrt2(Meet_CAAR_square / 40.0 - finalres[1][2] * finalres[1][2]);
    finalres[2][3] = sqrt2(Miss_CAAR_square / 40.0 - finalres[2][2] * finalres[2][2]);

    return finalres;
}


