#include<iostream>
#include<string>
#include<fstream>
#include<cmath>
#include<vector>
#include<typeinfo>
#include<map>
#include<iomanip>
#include<algorithm>
#include<chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;
/*---------------------------------------------------------------------------*/
vector<int> predict_funct(vector<double> probabilities){
    vector<int> res;
    for(int i = 0 ; i < probabilities.size() ; i++){
        if(probabilities[i] > 0.5){
            res.push_back(1);
        }else{
            res.push_back(0);
        }
    }
    return res;
}
/*---------------------------------------------------------------------------*/
template <typename T>
vector<double> prio_cond_prob(vector<T> survive){
    double counts[2] = {0.0};
    // total_counts[0] is total count of did not survive
    // total_counts[1] is total count of did survive
    vector<double> res;
    for(int i = 0 ; i < survive.size() ; i++){
        counts[survive[i]]++;
    }
    res.push_back(counts[0] / survive.size());
    res.push_back(counts[1] / survive.size());
    return res;
    
}
template <typename T>
vector<vector<double>> sex_cond_prob(vector<T> survive, vector<T> sexTest){
    vector<vector<double>> counts(2, vector<double>(2, 0));
    vector<double> res;
    // counts[0][0] is count of females who didn't survive
    // counts[1][0] is count of female who didn't survive
    // counts[0][1] is count of males who  survived
    // counts[1][1] is count of males who survived
    double total_counts[2] = {0,0}; 
    // total_counts[0] is total count of did not survive
    // total_counts[1] is total count of did survive
    for (int i = 0; i < survive.size(); i++) {
        counts[survive[i]][sexTest[i]]++;
        total_counts[survive[i]]++;
    }




    vector<vector<double>> sex(2, vector<double>(3, 0));
    for (int i = 0; i < 2; i++) {
        sex[0][i] = counts[0][i] / total_counts[0];
        sex[1][i] = counts[1][i] / total_counts[1];
    }
    return sex;
}
template<typename T>
vector<vector<double>> class_cond_prob(vector<T> survive, vector<T> pClass) {
    vector<vector<double>> counts(2, vector<double>(3, 0));
    vector<double> total_counts(3, 0);
    // counts[0][0] is count of group 1 who didn't survive
    // counts[0][1] is count of group 2 who didn't survive
    // counts[0][2] is count of group 3 who didn't survive
    // counts[1][0] is count of group 1 who  survived
    // counts[1][1] is count of group 2 who survived
    // counts[1][2] is count of group 3 who survived
    // Count survival and non-survival for each class
    for (int i = 0; i < survive.size(); i++) {
        counts[survive[i]][pClass[i] - 1]++;
        total_counts[pClass[i] - 1]++;
    }
    // Calculate class survival rates
    vector<vector<double>> class_survival(2, vector<double>(3, 0));
    for (int i = 0; i < 3; i++) {
        class_survival[0][i] = counts[0][i] / total_counts[0];
        class_survival[1][i] = counts[1][i] / total_counts[1];
    }
    return class_survival;
}
double age_likelihood(int age, double mean, double variance){
    double ageVal = age * 1.0;
    double pi = 3.14159265358979323846;
    return 1 / sqrt(2 * pi * variance) * exp(-1 * (pow((ageVal - mean),2))/(2 * variance));
}
template<typename T>
vector<vector<double>> age_cond_prob(vector<T> survive, vector<T> ageTest){
    vector<vector<double>> counts(2, vector<double>(3, 0));
    vector<double> res;
    // counts[0][0] is count the sum of people did not survived
    // counts[1][0] is count the sum of people did survived
    // counts[0][1] is count the mean of people did not survived
    // counts[1][1] is count the mean of people did  survived
    // counts[0][2] is count the var of people did  survived
    // counts[1][2] is count the var of people did  survived

    double total_counts[2] = {0,0};
    for(int i = 0 ; i < survive.size() ; i++){
        counts[survive[i]][0] += ageTest[i];
        total_counts[survive[i]]++;
    }
    // calculate the mean
    counts[0][1] = counts[0][0] / total_counts[0];
    counts[1][1] = counts[1][0] / total_counts[1];
    // now we calculate numerator std for survived and not survived
    for(int i = 0 ; i < survive.size() ; i++){
        double meanVal = counts[survive[i]][1];
        counts[survive[i]][2] += pow((ageTest[i] - meanVal),2);
    }
    counts[0][2] = counts[0][2] / (total_counts[0] - 1);
    counts[1][2] = counts[1][2] / (total_counts[1] - 1);
    vector<vector<double>> ages(2, vector<double>(2, 0));
    for (int i = 1; i < 3; i++) {
        ages[0][i-1] = counts[0][i];     // death of the age
        ages[1][i-1] = counts[1][i];     // sur of the age
    }
    return ages;

}
int main(){
	vector<int> survivedTrain(800);
	vector<int> survivedTest(246);
	vector<int> sexTrain(800);
	vector<int> sexTest(246);
	vector<int> ageTrain(800);
	vector<int> ageTest(246);
	vector<int> classTrain(800);
	vector<int> classTest(246);
    string fileName = "titanic_project.csv";
    string line;
    fstream myFile;
    vector<string> temp(5);
    // Open File
    myFile.open(fileName, ios::in);
    // Read the first line 
    char delim = ',';
    int currRow = 0;
    getline(myFile,line);
    while(myFile.good() && currRow < 1046){
		getline(myFile,line);
        stringstream s(line);
        vector<string> tempVec(5);
        int i = 0;
        while(!s.eof()){
            getline(s,tempVec.at(i),delim);
            i++;
        }
        // now seperate between train and tes
        if (currRow < 800){
			classTrain.at(currRow) = stoi(tempVec.at(1));
			survivedTrain.at(currRow) = stoi(tempVec.at(2));
			sexTrain.at(currRow) = stoi(tempVec.at(3));
			ageTrain.at(currRow) = stoi(tempVec.at(4));
        }else{
			classTest.at(currRow % 800) = stoi(tempVec.at(1));
			survivedTest.at(currRow % 800) = stoi(tempVec.at(2));
			sexTest.at(currRow % 800) = stoi(tempVec.at(3));
			ageTest.at(currRow % 800) = stoi(tempVec.at(4));
        }
        currRow++;
    }
    auto start = high_resolution_clock::now();
    // Conditional Probability for Test
    vector<double> prioProbTest = prio_cond_prob(survivedTrain);
    vector<vector<double>> sexProbTest = sex_cond_prob(survivedTrain,sexTrain);
    vector<vector<double>> classProbTest = class_cond_prob(survivedTrain,classTrain);
    vector<vector<double>> ageProbTest = age_cond_prob(survivedTrain,ageTrain);
    auto end = high_resolution_clock::now();
    cout << setw(25) << "Survival " << setw(15) << "Not Survival" << endl;
    for(int i = 0 ; i < 5; i++){
        double sumS = sexProbTest[1][sexTest[i]] * classProbTest[1][classTest[i]] * prioProbTest[1] * age_likelihood(ageTest[i] , ageProbTest[1][0] , ageProbTest[1][1]);
        double sumNS = sexProbTest[0][sexTest[i]] * classProbTest[0][classTest[i]] * prioProbTest[0] * age_likelihood(ageTest[i] , ageProbTest[0][0] , ageProbTest[0][1]);
        double resSurvival = sumS / (sumS + sumNS);
        double resNSurvival = sumNS / (sumS + sumNS);
        cout << "Passenger " << i <<  ": " << setw(10) << resSurvival << setw(15) << resNSurvival << endl;
    }
    auto duration = duration_cast<seconds>(end - start);
    cout << "Time taken is " << duration.count() << " seconds" << endl;
    cout <<"------------------------------------------------------" << endl;
}