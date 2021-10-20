/* 
 * File:   Classifier.h
 * Author: ujjwal
 *
 * Created on 4 May, 2021, 7:33 PM
 */

#ifndef CLASSIFIER_H
#define	CLASSIFIER_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <math.h>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

class Classifier {
public:
    Classifier();
    void Normalize(float **data_mat, int row, int col);
    void TrainSVMClassifier(float **training_data, float* labels, int row, int col);
    void TrainClassifier(float **training_data, int row, int col);
    void TestData(float **testing_data, int row, int col);
    void ClassifyData(float** testing_data, int row, int col);
    void normalizeFV(float **data_mat, int row, int col);
    virtual ~Classifier();
private:
    CvSVMParams params;
    CvSVM SVM;
    float *ref_mean;
    float *ref_std_deviation;
    float *calculateMean(float** data_mat, int row, int col);
    float *calculateStdDeviation(float **data_mat, int row, int col, float *mean);
};

#endif	/* CLASSIFIER_H */

