//
//  main.cpp
//  FinalProject
//  The DataAnalytics object
//      1. Receives a text file of coordinate points and        its dimensions from a user
//      2. Loads the data into a dynamically allocated 2D       array
//      3. Conducts a statistical analysis on the data
//      4. Performs a kMeans Clustering and Classification      on the data points
//
//  Input: text file with line seperated coordinates that each have an x and y value seperated with a space, number of rows as an integer, number of cols (2)
//  ***Object only works on 2-Dimensional data***
//
//  Output:
//      1. Statistical moments of the data
//      2. number of kMeans Clustering iterations
//      3. A centroid value paired with its data points         for each cluster
//
//  Created by Shayon Ghoshroy on 12/9/19 at 12:00 am.
//  Copyright © 2019 Shy Guy. All rights reserved.
//
// IMPORTANT ADDITIONAL NOTES
//  1. the main.cpp file was changed to allow for proper file opening (original lines are commented out)
//  2. an extremely large amount of data points will overflow the stack on your computer due to the statistical analysis section of the program
//  3.  if an object is created with a default constructor, you must use setrow() and setcol() to allocate the correct amount of memory
//  4. When using the overloaded assignment operator, the left hand side must not have been put through kMeans Clustering beforehand
//  5. You MUST use the kMeans clustering method before using the << operator, classify(), or when placing the object on the right hand side of the assignment operator
//  6. main.cpp and DataAnalytics.cpp must both be compiled at the same time --> g++ DataAnalytics.cpp main.cpp

//#include "DataAnalytics.cpp"
#include "DataAnalytics.h"
#include <iostream>
#include <cstdlib>
//#include <fstream>
using namespace std;
int main(int argc, const char * argv[]) {
    int row, col;
    string fname;
    ifstream in_file;
    ofstream out_file;
    DataAnalytics train, test;
    //DataAnalytics train;
    
    cout << "Please enter the name of the train file, rows, and columns: ";
    cin >> fname;
    cin >> row;
    cin >> col;
    train.setrow(row);
    train.setcol(col);
    
    //in_file.open(fname);
    in_file.open(fname.c_str());
    if (in_file.fail())
    {
        cout << "Error openning the file " << argv[1] << " \n";
        exit(1);
    }
    
    in_file >> train;
    in_file.close();
    
    cout << "Zeroeth statistical moment: " << endl;
    train.zeroMoment();
    
    cout << "First statistical moment: " << endl;
    train.firstMoment();
    
    cout << "Second statistical moment: " << endl;
    train.secondMoment();
    
    cout << "Third statistical moment: " << endl;
    train.thirdMoment();
    
    cout << "Forth statistical moment: " << endl;
    train.fourthMoment();
    
    train.kMeansClustering(2);
    
     cout << "Please enter the name of the testing file, rows, and columns: ";
     cin >> fname;
     cin >> row;
     cin >> col;
     test.setrow(row);
     test.setcol(col);
     
     //in_file.open(fname);
    in_file.open(fname.c_str());
     if (in_file.fail())
     {
     cout << "Error openning the file " << argv[1] << " \n";
     exit(1);
     }
     in_file >> test;
     in_file.close();
     
     test = train;
     test.classify();
     
     cout << "Please enter the output file: ";
     cin >> fname;
     
     //out_file.open(fname);
    out_file.open(fname.c_str());
     if (out_file.fail())
     {
     cout << "Error openning the file " << argv[2] << " \n";
     exit(1);
     }
     
     out_file << test << endl;
     out_file.close();
    
    return 0;
}
