//
//  Vector.cpp
//  Licurl
//
//  Created by Katherine Zeng on 12/2/21.
//

#include "Vector.h"
#include <stdio.h>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

vector<double> operator-(const vector<double>& X, const vector<double>& Y) {
    int d = (int)X.size();
    vector<double> Z(d);
    for (int i = 0; i < d; i++) {
        Z[i] = X[i] - Y[i];
    }
    return Z;
}


vector<double> operator+(const vector<double>& X, const vector<double>& Y) {
    int d = (int)X.size();
    vector<double> Z(d);
    for (int i = 0; i < d; i++) {
        Z[i] = X[i] + Y[i];
    }
    return Z;
}


void operator+=(vector<double>& X, const vector<double>& Y) {
    int d = (int)X.size();
    for (int i = 0; i < d; i++) {
        X[i] += Y[i];
    }
}

vector<double> operator/(const vector<double>& X, const double& a) {
    int d = (int)X.size();
    vector<double> Z(d);
    for (int i = 0; i < d; i++) {
        Z[i] = X[i] / a;
    }
    return Z;
}

vector<double> operator*(const vector<double>& X, const vector<double>& Y) {
    int d = (int)X.size();
    vector<double> Z(d);
    for (int i = 0; i < d; i++) {
        Z[i] = X[i] * Y[i];
    }
    return Z;
}
ostream& operator<<(ostream& out, vector<double>& x) {
    vector <double>::iterator iter;
    for (iter = x.begin(); iter != x.end(); iter++) {
        out << *iter << ",";
    }
    return out;
}

ostream& operator<<(ostream& out, vector<string>& x) {
    vector<string>::iterator iter;
    for (iter = x.begin(); iter != x.end(); iter++) {
        out << *iter << ",";
    }
    return out;
}

vector<double> sqrt2(const vector<double>& X) {
    int d = (int)X.size();
    vector<double> Z(d);
    for (int i = 0; i < d; i++) {
        Z[i] = sqrt(X[i]);
    }
    return Z;
}
