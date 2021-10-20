/* 
 * File:   Methods.cpp
 * Author: Ujjwal
 * 
 * Created on 3 may, 2021, 11:28 AM
 */


#include "Methods.h"

Methods::Methods() {
}

Methods::~Methods() {
}

/*
 * This function computes the center of gravity of the image.
 * @input: Image structure consisting of orig image and boundaries of sub-image
 *  @output: Center of Gravity Point
 */
void Methods::calculateCOG(Image* input) {
    CvPoint COG = cvPoint(0, 0);
    char *data = input->img->imageData;
    int widthstep = input->img->widthStep;
    int sign_pixel_count = 0;

    for (int i = input->x_upper + 1; i < input->x_lower - 1; i++) {
        for (int j = input->y_left + 1; j < input->y_right - 1; j++) {
            if (data[(i * widthstep) + j] == 0) {
                COG.x += i;
                COG.y += j;
                sign_pixel_count += 1;
            }
        }
    }
    if (sign_pixel_count != 0) {
        COG.x /= sign_pixel_count;
        COG.y /= sign_pixel_count;
    } else {
        COG.x = (input->x_lower + input->x_upper) / 2;
        COG.y = (input->y_left + input->y_right) / 2;
    }
    input->cog = COG;
    input->sign_pixel_count = sign_pixel_count;
}

void Methods::calculateCOG(vector<Image> *result) {
    unsigned long count = 0;
    Image temp_img;

    for (count = 0; count < result->size(); count++) {
        temp_img = result->at(count);
        Methods::calculateCOG(&temp_img);
        result->at(count).cog = temp_img.cog;
        result->at(count).sign_pixel_count = temp_img.sign_pixel_count;
    }
}

void Methods::partitionImage(IplImage* bin_image, int n_partition,
        vector<Image> *result) {
    Image temp_image(bin_image, 0, bin_image->height, 0, bin_image->width);
    vector<Image> sub_images;
    temp_image.img = bin_image;
    temp_image.x_lower = bin_image->height - 1;
    temp_image.x_upper = 0;
    temp_image.y_left = 0;
    temp_image.y_right = bin_image->width - 1;
    sub_images.push_back(temp_image);
    CvPoint temp_COG;
    unsigned long count = 0;
    while (sub_images.size() < n_partition) {
        temp_image = sub_images.front();
        Methods::calculateCOG(&temp_image);
        temp_COG = temp_image.cog;
        sub_images.at(0).cog = temp_COG;

        sub_images.push_back(Image(bin_image, temp_image.x_upper, temp_COG.x,
                temp_image.y_left, temp_COG.y));
        sub_images.push_back(Image(bin_image, temp_image.x_upper, temp_COG.x,
                temp_COG.y, temp_image.y_right));
        sub_images.push_back(Image(bin_image, temp_COG.x, temp_image.x_lower,
                temp_image.y_left, temp_COG.y));
        sub_images.push_back(Image(bin_image, temp_COG.x, temp_image.x_lower,
                temp_COG.y, temp_image.y_right));
        sub_images.erase(sub_images.begin());
        //count++;
    }
    //    cout << "outt" << endl;
    result->swap(sub_images);
    //    cout << "size: " << sub_images.size() << " " << result->size() << endl;
}

void Methods::showSubImages(vector<Image> *result, IplImage *orig_img) {
    Image temp_image;
    //    cout << "show subimages: " << result->size() << endl;
    for (unsigned int i = 0; i < result->size(); i++) {
        temp_image = result->at(i);
        cvLine(orig_img, cvPoint(temp_image.y_left, temp_image.x_lower),
                cvPoint(temp_image.y_right, temp_image.x_lower), cvScalar(0, 0, 0, 0), 1);
        cvLine(orig_img, cvPoint(temp_image.y_left, temp_image.x_upper),
                cvPoint(temp_image.y_right, temp_image.x_upper), cvScalar(0, 0, 0, 0), 1);
        cvLine(orig_img, cvPoint(temp_image.y_left, temp_image.x_lower),
                cvPoint(temp_image.y_left, temp_image.x_upper), cvScalar(0, 0, 0, 0), 1);
        cvLine(orig_img, cvPoint(temp_image.y_right, temp_image.x_lower),
                cvPoint(temp_image.y_right, temp_image.x_upper), cvScalar(0, 0, 0, 0), 1);
    }
}

float Methods::calculateDistance(CvPoint pt) {
    return sqrt(pow(pt.x, 2) + pow(pt.y, 2));
}

void Methods::calculateCOGDistances(vector<Image>* sub_images) {
    for (int i = 0; i < sub_images->size(); i++) {
        sub_images->at(i).cog_distance = Methods::calculateDistance(sub_images->at(i).cog);
    }
}

float Methods::calculateAngle(CvPoint pt) {
    if (pt.x == 0) {
        return 1.57;
    } else {
        return abs(atan(pt.y / pt.x));
    }
}

void Methods::calculateCOGAngles(vector<Image>* sub_images) {
    for (int i = 0; i < sub_images->size(); i++) {
        sub_images->at(i).cog_angle = Methods::calculateAngle(sub_images->at(i).cog);
    }
}

vector<float> *Methods::generateFeatureVector(vector<Image>* sub_images, int choice) {
    Methods::calculateCOGAngles(sub_images);
    Methods::calculateCOGDistances(sub_images);


    vector<float> *pixel_count = new vector<float>();
    vector<float> *cog_distance = new vector<float>();
    vector<float> *cog_angle = new vector<float>();
    vector<float> *fv = new vector<float>();
    int size = sub_images->size();
    Image temp_image;
    for (int i = 0; i < size; i++) {
        temp_image = sub_images->at(i);
        pixel_count->push_back(temp_image.sign_pixel_count * 1.0);
        cog_distance->push_back(temp_image.cog_distance);
        cog_angle->push_back(temp_image.cog_angle);
    }
    if (choice == 0) // only pixel count as FV
    {
        fv->swap(*pixel_count);
    } else if (choice == 1) // only COG distance as FV
    {
        fv->swap(*cog_distance);
    } else if (choice == 2) {
        fv->swap(*cog_angle);
    } else if (choice == 3) {
        int i = 0;
        while (i < pixel_count->size())
            fv->push_back(pixel_count->at(i++));
        i = 0;
        while (i < cog_distance->size())
            fv->push_back(cog_distance->at(i++));
        i = 0;
        while (i < cog_angle->size())
            fv->push_back(cog_distance->at(i++));
    }
    return fv;
}

void Methods::binarizeImage(IplImage* input) {
    uchar *data = (uchar *) input->imageData;
    int widthstep = input->widthStep;
    for (int i = 0; i < input->height; i++) {
        for (int j = 0; j < input->width; j++) {
            if (data[i * widthstep + j] < 225)
                data[i * widthstep + j] = 0;
            else
                data[i * widthstep + j] = 255;
        }
    }
}

void Methods::printMatrix(float **mat, int row, int col) {
    cout << "===========================" << endl;
    cout << "Printing Matrix" << endl;
    cout << "===========================" << endl;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

void Methods::printSubImage(vector<Image> *v) {
    Image temp_img;
    for (int i = 0; i < v->size(); i++) {
        temp_img = v->at(i);
        cout << temp_img.x_upper << " , " << temp_img.x_lower << " , " << temp_img.y_left << " , " << temp_img.y_right << " , " << temp_img.sign_pixel_count << endl;
        //        cout<<temp_img.cog_angle<<" , "<<temp_img.cog_distance<<" , "<<temp_img.sign_pixel_count<<endl;
    }
    cout << endl;
}
