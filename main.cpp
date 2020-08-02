/* 
 * File:   main.cpp
 * Author: Udhav
 *
 * Created on 15 April, 2020, 3:18 AM
 */

#include <cstdlib>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include "Methods.h"
#include "Classifier.h"

#define ROW 200
#define COL 200
#define CHOICE 3
#define REF_PATH "../data/Reference/"
#define TEST_PATH "../data/Simulated/"
#define fvLength() if(CHOICE==3) ? 192 : 64
#define _DEBUG 0

using namespace std;
using namespace cv;

/*
 * 
 */
int main() {
    IplImage *orig_img; // original signature image
    Methods m; //creating an object of class containing general methods.
    vector<float> *fv = new vector<float>(); //vector contains fv of sign images
    vector<Methods::Image> *v = new vector<Methods::Image > (); //vector contains subimages of a sign image
    float **training_data; // 2D array containing feature vector for training
    training_data = (float**) malloc(ROW * sizeof (float*));
    for (int i = 0; i < ROW; i++) {
        training_data[i] = (float*) malloc(COL * sizeof (float));
    }
    float *labels = (float *)calloc(ROW , sizeof(float));
    int i = 0, n_train_images = 0; //temp loop variables

    while (1) {
        std::stringstream sstm;
        sstm << REF_PATH << (i + 1) << ".png";
        orig_img = cvLoadImage(sstm.str().c_str(), 0); //loading reference images
        if (orig_img == NULL) //if no more image get out of the loop
        {
            cout << "total no of images read: " << i << endl;
            break;
        }

        m.binarizeImage(orig_img); //converting image into binary image and other preprocessing
        m.partitionImage(orig_img, 192, v); //partition the image into sub images
        m.calculateCOG(v); // calculates the center of gravity of subimages

        if (_DEBUG) {
            cvNamedWindow("output", 0);
            m.showSubImages(v, orig_img);
            cvShowImage("output", orig_img);
            m.printSubImage(v);
            cvWaitKey(0);
        }

        //generates the feature vector of the image. Calculates angle,distance within.
        fv = m.generateFeatureVector(v, CHOICE);
        //Copying the training feature vectors to the 2D-array
        for (int j = 0; j < fv->size(); j++) {
            training_data[i][j] = fv->at(j);
        }
        //cleaning the feature vector and subimage vector before next iteration
        fv->clear();
        v->clear();
        i++;
    }
    
    n_train_images = i;
    for(i=0;i<n_train_images;i++){
        labels[i]=1.0;
    }
    
    Classifier sign_classifier;
    sign_classifier.TrainClassifier(training_data,n_train_images,192);
    
    if(_DEBUG)
        m.printMatrix(training_data, n_train_images, 192);
    sign_classifier.ClassifyData(training_data,n_train_images,192);
    
    i=0;
    int n_test_image = 0;
    while (1) {
        std::stringstream sstm;
        sstm << TEST_PATH << (i + 1) << ".png";
        orig_img = cvLoadImage(sstm.str().c_str(), 0); //loading reference images
        if (orig_img == NULL) //if no more image get out of the loop
        {
            cout << "total no of images read: " << i << endl;
            break;
        }

        m.binarizeImage(orig_img); //converting image into binary image
        m.partitionImage(orig_img, 192, v); //partition the image into sub images
        m.calculateCOG(v); // calculates the center of gravity of subimages

        //generates the feature vector of the image. Calculates angle,distance within.
        fv = m.generateFeatureVector(v, CHOICE);
        //Copying the training feature vectors to the 2D-array
        for (int j = 0; j < fv->size(); j++) {
            training_data[i][j] = fv->at(j);
        }
        //cleaning the feature vector and subimage vector before next iteration
        fv->clear();
        v->clear();
        i++;
    }
    n_test_image = i;
    sign_classifier.ClassifyData(training_data,n_test_image,192);
    return 0;
}
