#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) 
{
	//OpenCV video capture object
    cv::VideoCapture camera;
	
	//OpenCV image object
    cv::Mat image;
	
	//camera id . Associated to device number in /dev/videoX
	int cam_id; 
	
	//check user args
	switch(argc)
	{
		case 1: //no argument provided, so try /dev/video0
			cam_id = 0;  
			break; 
		case 2: //an argument is provided. Get it and set cam_id
			cam_id = atoi(argv[1]);
			break; 
		default: 
			std::cout << "Invalid number of arguments. Call program as: webcam_capture [video_device_id]. " << std::endl; 
			std::cout << "EXIT program." << std::endl; 
			break; 
	}
	
	//advertising to the user 
	std::cout << "Opening video device " << cam_id << std::endl;

    //open the video stream and make sure it's opened
    if( !camera.open(cam_id) ) 
	{
        std::cout << "Error opening the camera. May be invalid device id. EXIT program." << std::endl;
        return -1;
    }

    //capture loop. Out of user press a key
    while(1)
	{
		//Read image and check it
        if(!camera.read(image)) 
		{
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }
        
	
	//print image dimensions
	//std::cout << "image size is: " << image.rows << "x" << image.cols << std::endl; 
	
	//get central pixel
	int row_central_px = image.rows * 0.5;
	int col_central_px = image.cols * 0.5;

	//get intensity for each chanel BGR 
	int px_value_blue = image.at<cv::Vec3b>(row_central_px,col_central_px)[0] ;
	int px_value_green = image.at<cv::Vec3b>(row_central_px,col_central_px)[1] ;
	int px_value_red = image.at<cv::Vec3b>(row_central_px,col_central_px)[2] ;
	
	//print previous value for each channel BGR 
	//std::cout << "px_value_blue is: " << px_value_blue << "px_value_green is: " << px_value_green << "px_value_red is: " << px_value_red <<std::endl; 
	
	//reset pixels values	
	px_value_blue = 0;
	px_value_green = 0;
	px_value_red = 0;

	//Set number of levels
	int levels = 1;//Two levels of neighbours (8 pixels + current) **** You could change this variable to set diferent level of neighbours
	
	//number of pixels in this matrix
	int number_pixels = pow ((1 + (2 * levels)), 2.0) ;

	//find meddiums value for each channel BGR 
	for(int i = (row_central_px -levels ); i <= (row_central_px +levels); i++){
		for(int j = (col_central_px - levels); j <= (col_central_px + levels); j++)
		{
		    	px_value_blue += image.at<cv::Vec3b>(i, j)[0] ;
			px_value_green += image.at<cv::Vec3b>(i, j)[1] ;
			px_value_red += image.at<cv::Vec3b>(i, j)[2] ;
		}
	}
	
	//print previous value for each channel BGR 
	std::cout << "px_value_blue is: " << px_value_blue << "px_value_green is: " << px_value_green << "px_value_red is: " << px_value_red <<std::endl;
	
	//print previous value for each channel BGR 
	std::cout << "number_pixels is: " << number_pixels<<std::endl;  

	//get middle value for each channel
	int px_value_blue_new = px_value_blue / number_pixels;
	int px_value_green_new = px_value_green / number_pixels;
	int px_value_red_new = px_value_red / number_pixels;

	//print previous value for each channel BGR 
	std::cout << "px_value_blue_new is: " << px_value_blue_new << "px_value_green_new is: " << px_value_green_new << "px_value_red_new is: " << px_value_red_new <<std::endl; 
	

	//set mid intensity for each chanel BGR  
	for(int i = (row_central_px -levels ); i <= (row_central_px +levels); i++){
		for(int j = (col_central_px - levels); j <= (col_central_px + levels); j++)
		{ 				
			image.at<cv::Vec3b>(i, j)[0] = px_value_blue_new ;
			image.at<cv::Vec3b>(i, j)[1] = px_value_green_new;
			image.at<cv::Vec3b>(i, j)[2] = px_value_red_new;
		}
	}

	//show image in a window
        cv::imshow("Output Window", image);

	//Waits 1 millisecond to check if a key has been pressed. If so, breaks the loop. Otherwise continues.
        if(cv::waitKey(1) >= 0) break;
    }   
}
