#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <vector>
#include "opencv4/opencv2/opencv.hpp"
#include "opencv4/opencv2/core/core.hpp"
#include "opencv4/opencv2/highgui/highgui.hpp"
#include "opencv4/opencv2/imgcodecs/imgcodecs.hpp"

using namespace std;
using namespace cv;

//图片输出函数
void output_rotate(float angle_max, Mat* dst_nearest, Mat* dst_bilinear, Mat* dst_bicubic);
void output_distort(bool is_pos, float radius, Mat* dst_nearest, Mat* dst_bilinear, Mat* dst_bicubic);
//获得旋转扭曲和畸变下原图的坐标
vector <vector<float>> rotate_coordinates(float angle_max, int radius, int rows, int cols, float center_x, float center_y);
vector <vector<float>> distort_coordinates(float radius, bool is_pos, int rows, int cols, float center_x, float center_y);
//三种插值方法
void interpolations(vector <vector<float>> coordinates, int rows, int cols, Mat* dst_nearest, Mat* dst_bilinear, Mat* dst_bicubic);
uchar nearest_point(int i, int j, float u, float v, Mat* rgbChannel);
uchar bilinear_point(int i, int j, float u, float v, Mat* rgbChannel);
uchar bicubic_point(int i, int j, float u, float v, Mat* rgbChannel);
float S(float x);
//通道分离
vector <Mat> rgbChannels(3);

int main( int argc, char** argv) 
{
    Mat src = imread("THU.jpg");   
	split(src, rgbChannels);

    int cols = src.cols;
    int rows = src.rows;
    float center_x = (float)(cols - 1) / (float)2;//图像中心点坐标
    float center_y = (float)(rows - 1) / (float)2;

    float angle_maxs[] = {M_PI_4, M_PI_2, 3*M_PI_4, M_PI};//旋转扭曲最大旋转角度
    int rotate_radius = floor(fmin(cols/2, rows/2))-1;//旋转半径选为图片允许的最大半径
    float distort_radius[] = {(float)0.75 * rotate_radius, (float)rotate_radius};//图像畸变的球面半径

    for (int i = 0; i <= 3; i++)
    {
        vector <vector<float>> rotate_coors = rotate_coordinates(angle_maxs[i], rotate_radius, rows, cols, center_x, center_y);
        Mat dst_nearest(rows, cols, CV_8UC3, Scalar_<uchar>(0,0,0));
        Mat dst_bilinear(rows, cols, CV_8UC3, Scalar_<uchar>(0,0,0));
        Mat dst_bicubic(rows, cols, CV_8UC3, Scalar_<uchar>(0,0,0));
        interpolations(rotate_coors, rows, cols, &dst_nearest, &dst_bilinear, &dst_bicubic);
        output_rotate(angle_maxs[i], &dst_nearest, &dst_bilinear, &dst_bicubic);
    }

    for (int i = 0; i <= 3; i++)
    {
        vector <vector<float>> distort_coors;
        Mat dst_nearest(rows, cols, CV_8UC3, Scalar_<uchar>(0,0,0));
        Mat dst_bilinear(rows, cols, CV_8UC3, Scalar_<uchar>(0,0,0));
        Mat dst_bicubic(rows, cols, CV_8UC3, Scalar_<uchar>(0,0,0));
        bool is_pos;
        if (i % 2 == 0)
            is_pos = true;//正向畸变, 产生凸透镜效果
        else
            is_pos = false;//负向畸变, 产生凹透镜效果
        distort_coors = distort_coordinates(distort_radius[i/2], is_pos, rows, cols, center_x,center_y);
        interpolations(distort_coors, rows, cols, &dst_nearest, &dst_bilinear, &dst_bicubic);
        output_distort(is_pos, distort_radius[i/2], &dst_nearest, &dst_bilinear, &dst_bicubic);
    }
    return 0;
}
void output_rotate(float angle_max, Mat* dst_nearest, Mat* dst_bilinear, Mat* dst_bicubic)
{
    char filename[30];
    sprintf(filename, "rotate_nearest_%.2f.png", angle_max);
    imwrite(filename, *dst_nearest);
    sprintf(filename, "rotate_bilinear_%.2f.png", angle_max);
    imwrite(filename, *dst_bilinear);
    sprintf(filename, "rotate_bicubic_%.2f.png", angle_max);
    imwrite(filename, *dst_bicubic);
}
void output_distort(bool is_pos, float radius, Mat* dst_nearest, Mat* dst_bilinear, Mat* dst_bicubic)
{
    char filename[30];
    if (is_pos == true)
    {
        sprintf(filename, "distort_nearest_pos_%.2f.png", radius);
        imwrite(filename, *dst_nearest);
        sprintf(filename, "distort_bilinear_pos_%.2f.png", radius);
        imwrite(filename, *dst_bilinear);
        sprintf(filename, "distort_bicubic_pos_%.2f.png", radius);
        imwrite(filename, *dst_bicubic);
    }
    else
    {
        sprintf(filename, "distort_nearest_neg_%.2f.png", radius);
        imwrite(filename, *dst_nearest);
        sprintf(filename, "distort_bilinear_neg_%.2f.png", radius);
        imwrite(filename, *dst_bilinear);
        sprintf(filename, "distort_bicubic_neg_%.2f.png", radius);
        imwrite(filename, *dst_bicubic);
    }
}
vector <vector<float>> rotate_coordinates(float angle_max, int radius, int rows, int cols, float center_x, float center_y)
{ 
    vector <vector<float>> rotate_coors;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            vector <float> rotate_coor;
            float angle = 0;
            float x = 0;
            float y = 0;
            float distance = sqrt(pow(i - center_x, 2) +  pow(j - center_y, 2));     
            if (distance < radius)//仅在到中心点的距离小于旋转半径时进行坐标变换
            {
                angle = angle_max * (radius - distance) / radius;
                x = (i - center_x) * cos(angle) - (j - center_y) * sin(angle);
                y = (i - center_x) * sin(angle) + (j - center_y) * cos(angle);
                rotate_coor.push_back(x + center_x);
                rotate_coor.push_back(y + center_y);
            }
            else
            {
                rotate_coor.push_back(i);
                rotate_coor.push_back(j);
            }
            rotate_coors.push_back(rotate_coor);
        }
    }
    return rotate_coors;
}
vector <vector<float>> distort_coordinates(float radius, bool is_pos, int rows, int cols, float center_x, float center_y)
{
    vector <vector<float>> distort_coors;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            vector <float> distort_coor;
            float x = 0;
            float y = 0;
            float distance = sqrt(pow(i - center_x, 2) +  pow(j - center_y, 2));
            if (is_pos == true)
            {
                x = (radius/distance * asin(distance/radius)) * (i - center_x);
                y = (radius/distance * asin(distance/radius)) * (j - center_y);
            }
            else
            {
                x = (radius/distance * sin(distance/radius)) * (i - center_x);
                y = (radius/distance * sin(distance/radius)) * (j - center_y);
            }
            distort_coor.push_back(x + center_x);
            distort_coor.push_back(y + center_y);
            distort_coors.push_back(distort_coor);
        }
    }
    return distort_coors;
}
void interpolations(vector <vector<float>> coordinates, int rows, int cols, Mat* dst_nearest, Mat* dst_bilinear, Mat* dst_bicubic)
{
    vector <vector<float>>::iterator iter = coordinates.begin();
    for (int x = 0; x < rows ; x++ )
    {
        for (int y = 0; y < cols ; y++ )
        {
            int i = floor((*iter)[0]);
            int j = floor((*iter)[1]);
            float u = (*iter)[0] - i;
            float v = (*iter)[1] - j;
            if ( i >= 0 && i + 1 < rows && j >= 0 && j + 1 < cols)
            {
                (*dst_nearest).at<cv::Vec3b>(x,y)[0] = nearest_point(i, j, u, v, &rgbChannels[0]);
                (*dst_nearest).at<cv::Vec3b>(x,y)[1] = nearest_point(i, j, u, v, &rgbChannels[1]);
                (*dst_nearest).at<cv::Vec3b>(x,y)[2] = nearest_point(i, j, u, v, &rgbChannels[2]);
                (*dst_bilinear).at<cv::Vec3b>(x,y)[0] = bilinear_point(i, j, u, v, &rgbChannels[0]);
                (*dst_bilinear).at<cv::Vec3b>(x,y)[1] = bilinear_point(i, j, u, v, &rgbChannels[1]);
                (*dst_bilinear).at<cv::Vec3b>(x,y)[2] = bilinear_point(i, j, u, v, &rgbChannels[2]);
                if ( i - 1 >= 0 && i + 2 < rows && j - 1 >= 0  && j + 2 < cols)
                {
                    (*dst_bicubic).at<cv::Vec3b>(x,y)[0] = bicubic_point(i, j, u, v, &rgbChannels[0]);               
                    (*dst_bicubic).at<cv::Vec3b>(x,y)[1] = bicubic_point(i, j, u, v, &rgbChannels[1]);            
                    (*dst_bicubic).at<cv::Vec3b>(x,y)[2] = bicubic_point(i, j, u, v, &rgbChannels[2]);
                }
            }
            if (iter != coordinates.end())
                iter ++;
        }
    }
}
uchar nearest_point(int i, int j, float u, float v, Mat* rgbChannel)
{
    uchar inter_val = 0;
    if (u <= 0.5)
    {
        if (v <= 0.5)
            inter_val = (*rgbChannel).at<uchar>(i,j);
        else
            inter_val = (*rgbChannel).at<uchar>(i,j+1);
    }
    else
    {
        if (v <= 0.5)
            inter_val = (*rgbChannel).at<uchar>(i+1,j);
        else
            inter_val = (*rgbChannel).at<uchar>(i+1,j+1);
    }
    return inter_val;
}
uchar bilinear_point(int i, int j, float u, float v, Mat* rgbChannel)
{
    Mat A = (Mat_<float>(1,2)<<1-u,u);
    Mat B = (Mat_<float>(2,2)<<(*rgbChannel).at<uchar>(i,j),(*rgbChannel).at<uchar>(i,j+1),(*rgbChannel).at<uchar>(i+1,j),(*rgbChannel).at<uchar>(i+1,j+1));
    Mat C = (Mat_<float>(2,1)<<1-v,v);

    Mat result = (A * B * C);
    int res_val = clamp((int)result.at<float>(0,0), 0, 255);//防止出现overshooting或undershooting现象
    uchar inter_val = (uchar)res_val;

    return inter_val;
}
uchar bicubic_point(int i, int j, float u, float v, Mat* rgbChannel)
{
    Mat A = (Mat_<float>(1,4)<<S(u+1),S(u),S(u-1),S(u-2));
    Mat C = (Mat_<float>(4,1)<<S(v+1),S(v),S(v-1),S(v-2));
    Mat B (4,4,CV_32F, Scalar(0));

    for (int m = i - 1 ; m <= i + 2; m++)
        for (int n = j - 1 ; n <= j + 2; n++)
            B.at<float>(m - (i-1), n - (j-1)) = (*rgbChannel).at<uchar>(m,n);

    Mat result = (A * B * C);
    int res_val = clamp((int)result.at<float>(0,0), 0, 255);//防止出现overshooting或undershooting现象
    uchar inter_val = (uchar)res_val;

    return inter_val;
}
float S(float x)//双三次插值基函数
{
    if (fabsf(x) <= 1)
        return (1 - 2 * pow(fabsf(x),2) + pow(fabsf(x),3));
    else if (fabsf(x) <= 2)
        return (4 - 8 * fabsf(x) + 5 * pow(fabsf(x),2) - pow(fabsf(x),3));
    else
        return (0.0); 
}