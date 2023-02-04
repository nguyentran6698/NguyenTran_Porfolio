#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

double sumOfVector(vector<double>);
double findMean(vector<double>);
double findMedian(vector<double>);
double findRange(vector<double>);
double compCovariance(vector<double> , vector<double>);
double compCorrelation(vector<double> , vector<double>);
/*Utils Function*/
double getMin(vector<double>);
double getMax(vector<double>);

int main(int argc, char** argv)
{
    ifstream inFS;
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);
    cout << "Opening file Boston.csv" << endl;
    inFS.open("Boston.csv");
    if(!inFS.is_open()){
        cout << "Could not open file Boston.csv" << endl;
        return 1;
    }

    cout << "Reading line 1" << endl;
    getline(inFS, line);
    cout << "Heading: " << line << endl;
    int numObservations = 0;
    while(inFS.good()){
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in , '\n');
        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);
        numObservations++;

    }
    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "new length " << rm.size() << endl;
    cout << "Closing file Boston.csv" << endl;
    inFS.close();
    // Assignment begin from here 
    cout << "Number of records " << numObservations << endl;
    cout <<"---------------------------------------------------" << endl;
    // Sum of Records 
    cout << "Sum of records rm: " << sumOfVector(rm) << endl;
    cout << "Sum of records medv: " << sumOfVector(medv) << endl;
    cout <<"---------------------------------------------------" << endl;
    // Mean of the records
    cout << "Mean of records rm: " << findMean(rm) << endl;
    cout << "Mean of records medv: " << findMean(medv) << endl;
    cout <<"---------------------------------------------------" << endl;
    // Median of the records 
    cout << "Median of records rm: " << findMedian(rm) << endl;
    cout << "Median of records medv: " << findMedian(medv) << endl;
    cout <<"---------------------------------------------------" << endl;
    // Range of the records 
    cout << "Range of records rm: " << findRange(rm) << endl;
    cout << "Range of records medv: " << findRange(medv) << endl;
    cout <<"---------------------------------------------------" << endl;
    // Covariance of the records 
    cout << "Covariance of records: " << compCovariance(rm,medv) << endl;
    cout <<"---------------------------------------------------" << endl;
    // Correlation of the records
    cout << "Correlation of records: " << compCorrelation(rm,medv) << endl;
}

/*Utils function*/
double getMin(vector<double> n)
{
    double res = n.at(0);
    for (auto x: n)
        res = min(res, x);
    return res;
}
 
double getMax(vector<double> n)
{
    double res = n.at(0);
    for (auto x: n)
        res = max(res, x);
    return res;
}
double getVariance(vector<double> n){
    double mean = findMean(n);
    double sqDiff = 0;
    for (auto x : n)
        sqDiff += (x - mean) * (x - mean);
    return sqDiff / n.size();
}
double compCorrelation(vector<double> x, vector<double> y){
    double covariance = compCovariance(x,y);
    double varianceX = getVariance(x);
    double varianceY = getVariance(y);
    return covariance / ( sqrt(varianceX) * sqrt(varianceY)) ;  

}


double compCovariance(vector<double> x, vector<double> y){
    double meanX = findMean(x);
    double meanY = findMean(y);
    double sumMean = 0.0;
    int n = x.size() + y.size();
    for(int i = 0 ; i < x.size() ; i++){
        sumMean += (x.at(i) - meanX) * (y.at(i) - meanY); 
    }
    return  sumMean / (n - 1);  
}

double sumOfVector(vector<double> values){
    double result = 0.0;
    for(double x: values){
        result += x;
    }
    return result;
}
double findMean(vector<double> values){
    double res = 0.0;
    int sum = sumOfVector(values);
    return (double)sum / (double)values.size();
}
double findMedian(vector<double> values){
    sort(values.begin() , values.end());
    int size = values.size();
    int middle = int(size / 2);
    if(size % 2 == 0){
        return (values.at(middle - 1) + values.at(middle)) / 2.0;
    }else{
        return values.at(middle);
    }
}
double findRange(vector<double> values){
    return getMax(values) - getMin(values);
}

