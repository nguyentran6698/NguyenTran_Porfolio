/*
Name: Nguyen Tran
UTD_ID: HPT200001
Date: 03/02/2023
Assignment_3

*/
#include<iostream>
#include<string>
#include<fstream>
#include<cmath>
#include<vector>
#include<typeinfo>
#include<map>
#include<algorithm>
#include<chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;
/*-----------------------------------------------------------------------------------*/
vector<double> probVector(vector<vector<int>> matrixOne, vector<double> matrixTwo){
    vector<double> res;
    double product;
    for(int i = 0 ; i < matrixOne.size() ; i++){
        product = (matrixOne[i].at(0) * matrixTwo.at(0)) + (matrixOne[i].at(1) * matrixTwo.at(1));
        res.push_back(1/(1 + exp(-1 * product)));
    }
    
    return res;
}
vector<double> errorFind(vector<int> matrixOne, vector<double> matrixTwo){
    vector<double> errors;
    for(int i = 0 ; i < matrixOne.size() ; i++){
        errors.push_back(matrixOne[i] - matrixTwo[i]);
    }
    return errors;
}
vector<double> proba_funct(vector<int> sexTestData, vector<double> weights){
    // First we need to generate a odd between the test and weights
    vector<double> logOdds;
    vector<vector<int>> dataMatrix;
    vector<double> probabilities;
    // Initialize matrix for sexTest with col 1 with 1 and col 2 with test value
    for(int i = 0 ; i < sexTestData.size() ; i++){
        dataMatrix.push_back(vector<int>());
        dataMatrix.at(i).push_back(1);
        dataMatrix.at(i).push_back(sexTestData.at(i));
    }
    for(int i = 0 ; i < dataMatrix.size(); i++){
        double product = 0.0;
        for(int j = 0 ; j < 2 ; j++){
            product += dataMatrix[i][j] * weights[j];
        }
        logOdds.push_back(product);
    }
    for(int i = 0 ; i < logOdds.size() ; i++){
        probabilities.push_back(1.0 / (1.0 + exp(-logOdds[i])));
    }
    return probabilities;
}
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
/*----------------------------------------------------------------------------*/
vector<double> gradient_descent(vector<int> sexTrainData, vector<int> labels){
    vector<double> weights{1.0,1.0};
    int duration = 50000;
    vector<vector<int>> dataMatrix;
    vector<double>prob_vector;
    vector<double> errors;
    vector<double>gradients;
    double tempSum = 0.0;
    double learningRate = 0.001;
    for(int i = 0 ; i < sexTrainData.size() ; i++){
        dataMatrix.push_back(vector<int>());
        dataMatrix.at(i).push_back(1);
        dataMatrix.at(i).push_back(sexTrainData.at(i));
    }
    for(int i = 0 ; i < duration ;i++){
        prob_vector = probVector(dataMatrix,weights);
        for(int i = 0 ; i < prob_vector.size() ; i++){
            tempSum += prob_vector[i];
        }
        errors = errorFind(labels,prob_vector);
        // let start with the transpose first
        vector<vector<int>> transpose{vector<int>(),vector<int>()};
        // we transpose first what i
        for (int k = 0; k < dataMatrix.size(); k++) {
            transpose[0].push_back(dataMatrix[k].at(0));
            transpose[1].push_back(dataMatrix[k].at(1));
        }
        // then we dot product with error
        for(int k = 0 ; k < transpose.size() ; k++){
            double product = (transpose[k].at(0) * errors.at(0)) + (transpose[k].at(1) * errors.at(1));
            double productScale = product * learningRate;
            weights[k] += productScale;
        }
        
    }
    return weights;
}

int main(){
    vector<int> survivedTrain(800);
    vector<int> survivedTest(246);
    vector<int> sexTrain(800);
    vector<int> sexTest(246);
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
            survivedTrain.at(currRow) = stoi(tempVec.at(2));
            sexTrain.at(currRow) = stoi(tempVec.at(3));
        }else{
            int row = currRow % 800;
            survivedTest.at(row) = stoi(tempVec.at(2));
            sexTest.at(row) = stoi(tempVec.at(3));
        }
        currRow++;
    }
    auto start = high_resolution_clock::now();
    vector<double> weights = gradient_descent(sexTrain,survivedTrain);
    vector<double> probabilities = proba_funct(sexTest,weights);
    vector<int> predict_values = predict_funct(probabilities);
    auto end = high_resolution_clock::now();
    // calculate accuracy
    double cnt = 0.0;
    for(int i = 0 ; i < survivedTest.size(); i++){
        if(survivedTest[i] == predict_values[i])
            cnt += 1.0;
    }
    double accuracy = cnt / survivedTest.size();
    //calculate sensitivity
    double TP = 0;
    double FN = 0;
    for(int i = 0 ; i < survivedTest.size() ; i++){
        if(survivedTest[i] == 1 && predict_values[i] == 1){
            TP++;
        }else if(survivedTest[i] == 1 && predict_values[i] == 0){
            FN++;
        }
    }
    double sensitivity_score =(TP) / (TP + FN);
    // // calculate specificity
    double TN = 0;
    double FP = 0;
    for(int i = 0 ; i < survivedTest.size() ; i++){
        if(survivedTest[i] == 0 && predict_values[i] == 0){
            TN++;
        }else if(survivedTest[i] == 0 && predict_values[i] == 1){
            FP++;
        }
    }
    double specificity_score =(TN) / (TN + FP);
    // display
    auto duration = duration_cast<seconds>(end - start);
    cout << "accuracy = " << accuracy  << endl;
	cout << "sensitivity = " << sensitivity_score << endl;
	cout << "specificity = " << specificity_score << endl;
    cout << "Time taken is " << duration.count() << " seconds" << endl;
    myFile.close();
    return 0;
}
