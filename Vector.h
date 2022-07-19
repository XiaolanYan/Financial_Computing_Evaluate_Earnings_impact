//
//  Vector.hpp
//  Licurl
//
//  Created by Katherine Zeng on 12/2/21.
//
#pragma once

#ifndef Vector_hpp
#define Vector_hpp

#include <stdio.h>
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

vector<double> operator-(const vector<double>& X, const vector<double>& Y);
vector<double> operator+(const vector<double>& X, const vector<double>& Y);
void operator+=(vector<double>& X, const vector<double>& Y);
vector<double> operator/(const vector<double>& X, const double& a);
vector<double> operator*(const vector<double>& X, const vector<double>& Y);
vector<double> sqrt2(const vector<double>& X);
ostream& operator<<(ostream& out, vector<string>& x);
ostream& operator<<(ostream& out, vector<double>& x);


#endif /* Vector_hpp */
