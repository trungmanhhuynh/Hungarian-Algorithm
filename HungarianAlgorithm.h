#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv ;
using namespace std;

#ifndef HUNGARIAN_ALGORITHM_H
#define HUNGARIAN_ALGORITHM_H 

class HungarianAlgorithm{
private: 
      Mat CostMatrix ; 
      Mat AssignmentMatrix ; 
      Mat MaskCol ; 
      Mat MaskRow ;  
public:   
      HungarianAlgorithm(); 
      ~HungarianAlgorithm();
    
      void solve(Mat CostMatrix_In, Mat& Assignment_Out) ; 
      void step_zero(int *step) ; 
      void step_one(int* step) ; 
      void step_two(int* step); 
      void step_three(int* step); 
      void step_four(int* step); 
      void step_five(int* step); 
      void getAssignmentMatrix(); 
      void showCostMatrix(int* step);  
  };  
 #endif
