/* 
 * File:   Methods.h
 * Author: ayush
 *
 * Created on 12 October, 2012, 11:28 AM
 */

#ifndef METHODS_H
#define	METHODS_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

class Methods {
public:
    Methods();
    virtual ~Methods();

    struct Image {

        Image() {
        };

        Image(IplImage *image, int xu, int xl, int yl, int yr) {
            img = image;
            x_upper = xu;
            x_lower = xl;
            y_right = yr;
            y_left = yl;
        };
        IplImage *img;
        CvPoint cog; //cog of image
        int x_upper;
        int x_lower;
        int y_right;
        int y_left;
        float cog_distance; //distance of COG of image
        float cog_angle; //angle of COG of image
        int sign_pixel_count; // signature pixel count in image
    };
    void binarizeImage(IplImage *input);
    void calculateCOG(Image *img);
    void partitionImage(IplImage *bin_image, int n_partition, vector<Image> *result);
    void calculateCOG(vector<Image> *result);
    void printMatrix(float **mat, int row, int col);
    void printSubImage(vector<Image> *v);
    void showSubImages(vector<Image> *result, IplImage *orig_img);
    vector<float> *generateFeatureVector(vector<Image> *sub_images, int choice);
private:
    float calculateDistance(CvPoint pt);
    void calculateCOGDistances(vector<Image> *sub_images);
    float calculateAngle(CvPoint pt);
    void calculateCOGAngles(vector<Image> *sub_images);
};

#endif	/* METHODS_H */

