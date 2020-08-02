/* 
 * File:   Classifier.cpp
 * Author: Udhav
 * 
 * Created on 4 May, 2020, 7:33 PM
 */

#include "Classifier.h"

Classifier::Classifier() {
    //Setting up SVM's parameters
    params.svm_type = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.gamma = 20;

    params.term_crit.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
    params.term_crit.max_iter = 1000;
    params.term_crit.epsilon = 1e-6;

}

float *Classifier::calculateStdDeviation(float **data_mat, int row, int col, float *mean) {
    float *std_deviation = (float *)calloc(col,sizeof(float));
    
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            std_deviation[j] = std_deviation[j] + pow(data_mat[i][j]-mean[j],2);
        }
    }
    
    for(int j=0; j<col;j++){
        std_deviation[j] = sqrt(std_deviation[j]/row);
    }
    return std_deviation;
}

float *Classifier::calculateMean(float** data_mat, int row, int col) {
    float *mean = (float *)calloc(col, sizeof(float));
    
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            mean[j] = mean[j] + data_mat[i][j];
        }
    }
    for (int j=0; j < col; j++) {
        mean[j] = mean[j] / row;
    }
    return mean;
}

void displayVector(float *v, int n){
    for(int i=0; i<n; i++){
        cout<<v[i]<<" ";
    }
}

void Classifier::normalizeFV(float **data_mat, int row, int col){
    for(int i=0;i<row;i++){
        float mean =0;
        float std_deviation = 0;
        for(int j=0;j<col;j++){
            mean += data_mat[i][j];
        }
        mean /= col;
        for(int j=0;j<col;j++){
            std_deviation = std_deviation + pow(data_mat[i][j]-mean,2);
        }
        std_deviation /= col-1;
        for(int j=0;j<col;j++){
                data_mat[i][j] = (data_mat[i][j]-mean)/std_deviation;
        }
    }
}

void Classifier::Normalize(float** data_mat, int row, int col) {
    float *mean;
    float *std_deviation;
    mean = calculateMean(data_mat, row, col);
    std_deviation = calculateStdDeviation(data_mat, row, col, mean);
    for(int i=0;i<row;i++){
        for(int j=0; j<col;j++){
            data_mat[i][j] = (data_mat[i][j]-mean[j])/std_deviation[j];
        }
    }
}

void Classifier::TrainClassifier(float** training_data, int row, int col){
    ref_mean = calculateMean(training_data, row, col);
    ref_std_deviation = calculateStdDeviation(training_data, row, col, ref_mean);
//    cout<<"==========================="<<endl;
//    cout<<"Mean values"<<endl;
//    cout<<"==========================="<<endl;
//    displayVector(ref_mean,col);
//    cout<<"==========================="<<endl;
//    cout<<"Std. Deviation values"<<endl;
//    cout<<"==========================="<<endl;
//    displayVector(ref_std_deviation,col);
    
}

void Classifier::TrainSVMClassifier(float **training_data, float* labels, int row, int col){
    Mat training_data_mat(row, col, CV_32FC1, training_data);
    Mat labelsMat(row,1,CV_32FC1,labels);
    SVM.train_auto(training_data_mat, labelsMat, Mat(), Mat(), params);
}

void Classifier::TestData(float **testing_data, int row, int col)
{
    cout<<"==========================="<<endl;
    cout<<"Output values"<<endl;
    cout<<"==========================="<<endl;
    
    float output_value;

    for(int i=0;i<row;i++)
    {
        Mat test_data_mat(1, col, CV_32FC1, testing_data[i]);
        Mat result();
    
        output_value = SVM.predict(test_data_mat,result);
        cout<<"["<<i<<"] "<<result<<" , "<< output_value<< endl;
        
//        cout<<"["<<i<<"] "<<output_value<<endl;
    }
}

void Classifier::ClassifyData(float** testing_data, int row, int col){
    int error_threshold =24;
    int genuine = 0;
    int forged = 0;
    for(int i=0;i<row;i++){
        int error_count = 0;
        for(int j=0;j<col;j++){
            if(testing_data[i][j]>ref_mean[j]+2*ref_std_deviation[j] ||
                    testing_data[i][j]<ref_mean[j]-2*ref_std_deviation[j]){
                error_count++;
            }
        }
        if(error_count >= error_threshold){
            cout<<"["<<i<<"] "<<"forged"<<endl;
            forged++;
        }
        else{
            cout<<"["<<i<<"] "<<"genuine"<<endl;
            genuine++;
        }            
    }
    cout<<"genuine : "<<genuine<<endl;
    cout<<"forged : "<<forged<<endl;
    
}

Classifier::~Classifier() {
}
