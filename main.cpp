/* Author: Manh Huynh 
 *
 * Assignment Matching Problem solved by using Hungarian Algorithm
 * Algorithm implemented follow this book: https://books.google.kz/books?id=yXYzLm1e_IUC&pg=PA94&lpg=PA94&dq=job+assignment+problem+matrix+is+non+square&source=bl&ots=1LU3_P-Ss5&sig=k4hlPV0GLjUDT9PxoLEsomt0K6Q&hl=en&sa=X&ei=lFODUcW_FcKQOJfUgPgJ#v=onepage&q=job%20assignment%20problem%20matrix%20is%20non%20square&f=false
 * page 90. 
 *
 *
 * */



//opencv 
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/videoio.hpp> 
//C++
#include <iostream> 
#include <math.h> 
//local headers 
#include "HungarianAlgorithm.h" 


using namespace cv; 
using namespace std; 


int main(int argc, char** argv){

HungarianAlgorithm HungarianAlg ; 

Mat CostMatrix = (Mat_<double>(6,6) << 97,99,40,48,4,84,
				       29,65,13,73,63,94,
				       19,99,86,80,10,30,
				       55,78,32,15,42,13,
				       67,46,21,90,73,98,
				       21,94,11,88,40,37 ); 
Mat AssignmentMatrix ; 



HungarianAlg.solve(CostMatrix,AssignmentMatrix)  ; 

double optimalVal= 0;  ; 
for(int i = 0 ; i < CostMatrix.rows ; i++) {
  for(int j =0 ; j < CostMatrix.cols ; j++){
	if(AssignmentMatrix.at<double>(i,j) == 0) { 
		optimalVal = optimalVal +  CostMatrix.at<double>(i,j);
	}
  }
}
cout << "CostMatrix " << endl << CostMatrix << endl ;   
cout<< "Final Assignment"<< endl  << AssignmentMatrix << endl; 
cout <<"Total Optimal Value =" << optimalVal << endl ;

 
   waitKey(0); 
   return 0 ;  
}
