//  https://stackoverflow.com/questions/785097/how-do-i-implement-a-b%C3%A9zier-curve-in-c
#include <opencv2/opencv.hpp>

int interpolate( float from , float to , float percent )
{
    int difference = to - from;
    return from + ( difference * percent );
}    

int main(){
    int rows = 320;
    int cols = rows;
    cv::Mat image(rows, cols, CV_32F);

    float x1 = 10, y1 = 10,
        x2 = 10, y2 = 250,
        x3 = 250, y3 = 250;
    float x = 0, y = 0, 
        xa = 0, ya = 0,
        xb =0, yb = 0;

    for( float i = 0 ; i < 1 ; i += 0.0001 )
    {
        // The Green Line
        xa = interpolate( x1 , x2 , i );
        ya = interpolate( y1 , y2 , i );
        xb = interpolate( x2 , x3 , i );
        yb = interpolate( y2 , y3 , i );

        // The Black Dot
        x = interpolate( xa , xb , i );
        y = interpolate( ya , yb , i );
        
        image.at<float>(x, y) = 255;
    }
    
    int key = 0;
    while(key != 27)
    {
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }

    return 0;
}