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

#include "DataAnalytics.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <cstdlib>
using namespace std;

//default constructor
DataAnalytics::DataAnalytics()
{
    //size is 0
    row = 0;
    col = 0;
    clusters = 0;
    
    //all dynamic arrays point to NULL
    data = NULL;
    membership = NULL;
    centroids = NULL;
}

//constructor with a size and value
DataAnalytics::DataAnalytics(int _row, int _col, int value)
{
    //set the value to 0, and set size
    value = 0;
    row = _row;
    col = _col;
    clusters = 0;
    membership = NULL;
    centroids = NULL;
    
    //allocate a dynamic 2D array for the data and fill it with 0
    data = new double*[row];
    
    for(int i = 0; i < row; i++)
        data[i] = new double[col];
    
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            data[i][j] = value;
}

//constructor that copies a primitive 2D array
DataAnalytics::DataAnalytics(double *anArray[], int _row, int _col)
{
    row = _row;
    col = _col;
    clusters = 0;
    data = new double*[row];
    membership = NULL;
    centroids = NULL;
    
    for(int i = 0; i < row; i++)
        data[i] = new double[col];
    
    //set the contents the new data array to the contents of the primitive array
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            data[i][j] = anArray[i][j];
}

//copy constructor
DataAnalytics::DataAnalytics(const DataAnalytics &rhs)
{
    //size validation of rhs
    if(rhs.row < 0 || rhs.col != 2)
        exit(1);
    
    row = rhs.row;
    col = rhs.col;
    clusters = 0;
    data = new double*[row];
    membership = NULL;
    centroids = NULL;
    
    for(int i = 0; i < row; i++)
        data[i] = new double[col];
    
    //set the contents the new data array to the contents of the right hand side's data array
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            data[i][j] = rhs.data[i][j];
}

//takes in a DataAnalytics, overloads the assignment operator, and returns the new DataAnalytics that is now equal to the lhs
const DataAnalytics & DataAnalytics::operator=(const DataAnalytics &rhs)
{
    //exit if rhs object is a different size
    if(rhs.row != row || rhs.col != col)
        exit(1);
    
    //create dynamic arrays for centroids and memberships
    clusters = rhs.clusters;
    centroids = new double*[clusters];
    
    for(int i = 0; i < clusters; i++)
        centroids[i] = new double[col];
    membership = new int[row];

    //copy the centroids and memberships of the rhs
    for(int i = 0; i < row; i++)
        membership[i] = rhs.membership[i];
    
    for(int i = 0; i < clusters; i++)
    {
        centroids[i][0] = rhs.centroids[i][0];
        centroids[i][1] = rhs.centroids[i][1];
    }
    //return lhs object so that the operator is cascadable
    return *this;
}


//takes in a DataAnalytics, overloads the == operator, and returns a boolean representing whether the lhs equals the rhs
bool DataAnalytics::operator==(const DataAnalytics &rhs) const
{
    if(row != rhs.row || col != rhs.col || clusters != rhs.clusters)
        return false;
    
    for(int i = 0; i < row; i++)
    {
        if(membership[i] != rhs.membership[i])
            return false;
        for(int j = 0; j < col; j++)
            if(data[i][j] != rhs.data[i][j])
                return false;
    }
    
    for(int i = 0; i < clusters; i++)
        for(int j = 0; j < col; j++)
            if(centroids[i][j] != rhs.centroids[i][j])
                return false;

    return true;
}

//takes in a DataAnalytics, overloads the != operator, and returns a boolean based on whether the lhs is not equal to the rhs
bool DataAnalytics::operator!=(const DataAnalytics &rhs) const
{
    if(row != rhs.row || col != rhs.col || clusters != rhs.clusters)
        return true;
    
    for(int i = 0; i < row; i++)
    {
        if(membership[i] != rhs.membership[i])
            return true;
        for(int j = 0; j < col; j++)
            if(data[i][j] != rhs.data[i][j])
                return true;
    }
    
    for(int i = 0; i < clusters; i++)
        for(int j = 0; j < col; j++)
            if(centroids[i][j] != rhs.centroids[i][j])
                return true;

    return false;
}

//no input, overloads the negation operator, and negates each element of the operator if they are numbers
DataAnalytics DataAnalytics::operator-() const
{
    DataAnalytics temp(*this);
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            temp.data[i][j] = -data[i][j];
    return temp;
}

//getters and setters
int DataAnalytics::getrow() const
{
    return row;
}
void DataAnalytics::setrow(int _row)
{
    //input validation
    if(_row < 0)
        exit(1);
    
    row = _row;
    //allocate the correct amount of space for the data array based on the rows
    data = new double*[row];
    for(int i = 0; i < row; i++)
        data[i] = new double[col];
}
int DataAnalytics::getcol() const
{
    return col;
}
void DataAnalytics::setcol(int _col)
{
    //simply sets the cols since program can only handle 2D array data
    //input validation
    if(_col != 2)
        exit(1);
    col = _col;
}
int DataAnalytics::getClusters() const
{
    return clusters;
}
double** DataAnalytics::getData() const
{
    return data;
}
double** DataAnalytics::getCentroids() const
{
    return centroids;
}
int* DataAnalytics::getMembership() const
{
    return membership;
}

//print the data 2D array
void DataAnalytics::printData() const
{
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
            cout << data[i][j] << " ";
        cout << endl;
    }
}

//overloading the outstream << operator
ostream& operator<<(ostream &os, const DataAnalytics &rhs)
{
    //prints the point and membership of each row of the data
    for(int i = 0; i < rhs.row; i++)
    {
        for(int j = 0; j < rhs.col; j++)
            os << rhs.data[i][j] << " ";
        os << " cluster " << rhs.membership[i] << endl;
    }
    return os;
}

//overloading the instream >> operator in order to input text files with a 2D array
istream& operator>>(istream &is, const DataAnalytics &rhs)
{
    for(int i = 0; i < rhs.row; i++)
    {
        for(int j = 0; j < rhs.col; j++)
        {
            //catches any error while overloading ifstream
            try {is >> rhs.data[i][j];} catch (...) {cout << "The data file is smaller than the given rows" << endl;};
        }
    }
    return is;
}

//takes in the column of data and returns its mean as a double
double DataAnalytics::mean(int _col) const
{
    double mean = 0.0;
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            if(j == _col)
                mean += data[i][j];
    mean /= row;
    return mean;
}

//takes in the column of data and returns its standard deviation as a double
double DataAnalytics::stdDev(int _col) const
{
    double origMean = mean(_col);
    double newMean = 0.0;
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
        {
            if(j == _col)
                newMean += ((data[i][j] - origMean)*(data[i][j] - origMean));
        }
    newMean /= row;
    return sqrt(newMean);
}

//takes in the column of data and returns its median as a double
double DataAnalytics::median(int _col) const
{
    double array[row];
    for(int i = 0; i < row; i++)
        array[i] = data[i][_col];
    int n = (int)(sizeof(array) / sizeof(array[0]));
    sort(array, array + n);
    if(n % 2 != 0)
        return array[n / 2];
    return (array[(n - 1) / 2] + array[n / 2]) / 2.0;
}

//zero moment statistical analysis that prints the minimum and maximum X and Y value
void DataAnalytics::zeroMoment() const
{
    double minX = data[0][0];
    double maxX = data[0][0];
    double minY = data[0][1];
    double maxY = data[0][1];
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(j == 0)
            {
                if(data[i][j] < minX)
                    minX = data[i][j];
                if(data[i][j] > maxX)
                    maxX = data[i][j];
            }
            if(j == 1)
            {
                if(data[i][j] < minY)
                    minY = data[i][j];
                if(data[i][j] > maxY)
                    maxY = data[i][j];
            }
        }
    }
    
    cout << "Total data points: " << row << endl;
    cout << "X min value: " << minX << endl;
    cout << "X max value: " << maxX << endl;
    cout << "Y min value: " << minY << endl;
    cout << "Y max value: " << maxY << endl;
}

//first moment statistical analysis that prints the mean of the X and Y values
void DataAnalytics::firstMoment() const
{
    cout << "X average value: " << mean(0) << endl;
    cout << "Y avearge value: " << mean(1) << endl;
}

//second moment statistical analysis that prints the variance over the mean of the X and Y values
void DataAnalytics::secondMoment() const
{
    double meanX = mean(0);
    double meanY = mean(1);
    double varianceX = 0.0;
    double varianceY = 0.0;
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
        {
            if(j == 0)
                varianceX += ((data[i][j] - meanX)*(data[i][j] - meanX));
            if(j == 1)
                varianceY += ((data[i][j] - meanY)*(data[i][j] - meanY));
        }
    varianceX /= row;
    varianceY /= row;
    cout << "Variance on X: " << varianceX << endl;
    cout << "Variance on Y: " << varianceY << endl;
}

//third moment statistical analysis that prints the skew of the X and Y values
void DataAnalytics::thirdMoment() const
{
    double skewX = ((3 * (mean(0) - median(0))) / stdDev(0));
    double skewY = ((3 * (mean(1) - median(1))) / stdDev(1));
    
    cout << "Skew on X: " << skewX << endl;
    cout << "Skew on Y: " << skewY << endl;
}

//fourth moment statistical analysis that prints the kurtosis of the X and Y values
void DataAnalytics::fourthMoment() const
{
    //initializes variables
    double kurtosisX = 0.0;
    double kurtosisY = 0.0;
    double meanX = mean(0);
    double meanY = mean(1);
    double varianceX = 0.0;
    double varianceY = 0.0;
    
    for (int i = 0; i < row; i++)
    {
        kurtosisX += (data[i][0] - meanX)*(data[i][0] - meanX)*(data[i][0] - meanX)*(data[i][0] - meanX);
        kurtosisY += (data[i][1] - meanY)*(data[i][1] - meanY)*(data[i][1] - meanY)*(data[i][1] - meanY);
    }
    
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
        {
            if(j == 0)
                varianceX += ((data[i][j] - meanX)*(data[i][j] - meanX));
            if(j == 1)
                varianceY += ((data[i][j] - meanY)*(data[i][j] - meanY));
        }
    varianceX /= row;
    varianceY /= row;
    double nX = (double)sqrt(varianceX);
    double nY = (double)sqrt(varianceY);
    kurtosisX /=(row*nX*nX*nX*nX);
    kurtosisX -= 3.0;
    kurtosisY /=(row*nY*nY*nY*nY);
    kurtosisY -= 3.0;
    
    cout << "Kurtosis of X: " << kurtosisX << endl;
    cout << "Kurtosis of Y: " << kurtosisY << endl;
}

//takes in integers x1, y1, x2, and y2 and returns the distance from the first point to the second point
double DataAnalytics::distance(double x, double y, double x2, double y2) const
{
    return sqrt(pow(x - x2, 2) + pow(y - y2, 2));
}

//naive k-means clustering of data
void DataAnalytics::kMeansClustering(int _clusters)
{
    cout << "Running kMeans Clustering..." << endl;
    clusters = _clusters;
    //allocating space for centroid and membership arrays
    centroids = new double*[clusters];
    for(int i = 0; i < clusters; i++)
        centroids[i] = new double[col];
    membership = new int[row];
    
    //generate non-repeating random indexes
    int* usedX = new int[row];
    int* usedY = new int[row];
    for(int i = 0; i < row; i++)
    {
        usedX[i] = 0;
        usedY[i] = 0;
    }
    int xIndex;
    int yIndex;
    //randomly choose data points to equal the centroid of each cluster
    for(int i = 0; i < clusters; i++)
    {
        xIndex = rand() % row;
        yIndex = rand() % row;
        while(usedX[xIndex] == 1)
            xIndex = rand() % row;
        while(usedY[yIndex] == 1)
            yIndex = rand() % row;
        usedX[xIndex] = 1;
        usedY[yIndex] = 1;
        centroids[i][0] = data[xIndex][0];
        centroids[i][1] = data[yIndex][1];
    }
    delete[] usedX;
    delete[] usedY;
    
    //create a copy array of centroids to store original
    double** origCentroids = new double*[clusters];
    for(int i = 0; i < clusters; i++)
        origCentroids[i] = new double[col];
    
    //iterate over every centroid until they stop moving
    int iteration = 0;
    bool centroidChange = true;
    while(centroidChange)
    {
        iteration++;
        //calculate membership of each training data point
        double minDist;
        double newDist;
        int centroidRow;
        for(int i = 0; i < row; i++)
        {
            //initialize min distance to distance to first centroid
            minDist = distance(data[i][0], data[i][1], centroids[0][0], centroids[0][1]);
            centroidRow = 0;
            for(int j = 1; j < clusters; j++)
            {
                newDist = distance(data[i][0], data[i][1], centroids[j][0], centroids[j][1]);
                if(newDist < minDist)
                {
                    minDist = newDist;
                    centroidRow = j;
                }
            }
            membership[i] = centroidRow;
        }
        
        //set the original array equal to the current centroids
        for(int i = 0; i < clusters; i++)
        {
            cout << "Originial Centroid: ";
            for(int j = 0; j < col; j++)
            {
                origCentroids[i][j] = centroids[i][j];
                cout << origCentroids[i][j] << " ";
            }
            cout << endl;
        }
        
        //iterate over every cluster
        int clusterSize;
        double meanX;
        double meanY;
        for(int currCluster = 0; currCluster < clusters; currCluster++)
        {
            //reset the means and number of points in cluster
            meanX = 0.0;
            meanY = 0.0;
            clusterSize = 0;
            //iterate through every row of the data
            for(int i = 0; i < row; i++)
            {
                //if the current row is in the current cluster
                if(membership[i] == currCluster)
                {
                    //add to the means and the size of the cluster
                    clusterSize++;
                    meanX += data[i][0];
                    meanY += data[i][1];
                }
            }
            //calculate the means
            meanX /= clusterSize;
            meanY /= clusterSize;
            //assign them as the new centroid values
            cout << "New Centroid: " << meanX << " " << meanY << endl;
            //set centroids equal to the means
            centroids[currCluster][0] = meanX;
            centroids[currCluster][1] = meanY;
        }
        
        //check to see if new centroids are different than the originals
        bool hasNotChanged = true;
        for(int i = 0; i < clusters; i++)
            if(centroids[i][0] != origCentroids[i][0] || centroids[i][1] != origCentroids[i][1])
                hasNotChanged = false;
        
        //print fitness
        cout << "Iteration # " << iteration << endl;
        
        //if the centroid did not change
        if(hasNotChanged)
        {
            //print info about each cluster
            cout << "kMeans Clustering finished at " << iteration << " iterations" << endl;
            for(int i = 0; i < clusters; i++)
            {
                cout << "\nCluster " << i+1 << "\n---------" << endl;;
                cout << "Centroid: " << centroids[i][0] << " " <<  centroids[i][1] << endl;;
                cout << "Points:";
                for(int j = 0; j < row; j++)
                    if(membership[j] == i)
                        cout << " (" << data[j][0] << "," << data[j][1] << ")";
            }
            cout << endl;
            //end loop
            centroidChange = false;
        }
    }
    
    //delete space allocated for original centroid array
    for(int i = 0; i < clusters; i++)
        delete[] origCentroids[i];
    delete[] origCentroids;
    
}

//takes in nothing, returns nothing
//calculates the membership of its data based on the preset number of clusters and centroids
void DataAnalytics::classify()
{
    double minDist;
    double newDist;
    int centroidRow;
    for(int i = 0; i < row; i++)
    {
        //initialize min distance to distance to first centroid
        minDist = distance(data[i][0], data[i][1], centroids[0][0], centroids[0][1]);
        centroidRow = 0;
        //find the minimum distances
        for(int j = 1; j < clusters; j++)
        {
            newDist = distance(data[i][0], data[i][1], centroids[j][0], centroids[j][1]);
            if(newDist < minDist)
            {
                minDist = newDist;
                centroidRow = j;
            }
        }
        membership[i] = centroidRow;
    }
    //print info
    cout << "\nClassification\n--------------" << endl;
    for(int i = 0; i < clusters; i++)
    {
        cout << "\nCluster " << i+1 << "\n---------" << endl;;
        cout << "Centroid: " << centroids[i][0] << " " <<  centroids[i][1] << endl;;
        cout << "Points:";
        for(int j = 0; j < row; j++)
            if(membership[j] == i)
                cout << " (" << data[j][0] << "," << data[j][1] << ")";
    }
    cout << endl;
}

//destructor
DataAnalytics::~DataAnalytics()
{
    for(int i = 0; i < row; i++)
        delete[] data[i];
    delete membership;
    for(int i = 0; i < clusters; i++)
        delete[] centroids[i];
    delete[] data;
    delete[] centroids;
}
