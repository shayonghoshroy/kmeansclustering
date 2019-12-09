//
//  DataAnalytics.cpp
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

#ifndef DataAnalytics_h
#define DataAnalytics_h
#include <fstream>
#include <stdio.h>
using namespace std;

class DataAnalytics {
public:
    //constructors
    DataAnalytics();
    DataAnalytics(int row, int col, int value);
    DataAnalytics(double *anArray[], int _row, int _col);
    DataAnalytics(const DataAnalytics &);
    //destructor
    ~DataAnalytics();
    
    //operator overloading
    const DataAnalytics & operator=(const DataAnalytics &rhs);
    bool operator==(const DataAnalytics &rhs) const;
    bool operator!=(const DataAnalytics &rhs) const;
    DataAnalytics operator-() const;
    
    //getters and setters
    void printData() const;
    int getrow() const;
    void setrow(int);
    int getcol() const;
    void setcol(int);
    int getClusters() const;
    double** getData() const;
    double** getCentroids() const;
    int* getMembership() const;
    
    //statistical analysis functions
    double mean(int _col) const;
    double stdDev(int _col) const;
    double median(int _col) const;
    double distance(double x, double y, double x2, double y2) const;
    void zeroMoment() const;
    void firstMoment() const;
    void secondMoment() const;
    void thirdMoment() const;
    void fourthMoment() const;
    void kMeansClustering(int _clusters);
    void classify();
    
    //ifstream and ofstream overloading
    friend std::ostream& operator<<(std::ostream &os, const DataAnalytics& rhs);
    friend std::istream& operator>>(std::istream &is, const DataAnalytics& rhs);

private:
    int row;
    int col;
    int clusters;
    double** data;
    double** centroids;
    int* membership;
    
};

#endif /* DataAnalytics_h */
