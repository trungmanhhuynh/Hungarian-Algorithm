#include "HungarianAlgorithm.h"

HungarianAlgorithm::HungarianAlgorithm(){
}

HungarianAlgorithm::~HungarianAlgorithm(){
}

void HungarianAlgorithm::solve(Mat CostMatrix_In,Mat& Assignment_Out){
 
   this->CostMatrix = CostMatrix_In.clone()  ; 
    

// The algorithm consists of 8 steps 
   bool done = false ;
   int step = 0;  
   while(!done){ 
    
     switch(step){
        case 0:
 		step_zero(&step); 
		break ;
     	case 1: 
     		step_one(&step); 
		break ; 
    	case 2: 
		step_two(&step); 
		break; 
 	case 3: 
		step_three(&step);  
		break ; 
 	case 4: 
		step_four(&step); 
		break; 
 	case 5: 
		step_five(&step); 
		Assignment_Out = this->AssignmentMatrix ; 
		done = true ; 
		break; 
     }
  
   }
}
/* Step 0: Check square matrix 
 * check if CostMatrix is square, if not, we have to add dummy row/col with their
 * entries are 0
 */
void HungarianAlgorithm::step_zero(int* step){ 
	
//	showCostMatrix(step);
//	adding row
 	if(CostMatrix.cols > CostMatrix.rows){ 
		Mat ZeroRow = Mat(1,CostMatrix.cols,CV_64FC1) ; 
		vconcat(CostMatrix,ZeroRow,CostMatrix); 
	}
	if(CostMatrix.cols < CostMatrix.rows){ 
		Mat ZeroCol = Mat(CostMatrix.rows,1,CV_64FC1); 
		hconcat(CostMatrix,ZeroCol,CostMatrix); 
	}
	showCostMatrix(step);
	*step = 1; 	 

}
/* Step 1: Subtract row minima 
 * For each row, find the lowest element and subtract it from each element in a row
 */ 
void HungarianAlgorithm::step_one(int* step){ 

	double minVal ; 
	for(int i =0 ; i < CostMatrix.rows ; i++) { 
   		minMaxLoc(CostMatrix.row(i),&minVal, NULL, NULL, NULL); 
  	 CostMatrix.row(i) = CostMatrix.row(i) - minVal;  
	} 
	showCostMatrix(step) ;
	*step = 2;  
}
/*Step 1 : Subtrac col minima 
 * For each col, find the lowest element and subtract it from each element in a col
 */ 
void HungarianAlgorithm::step_two(int* step){

	double minVal ; 
        for(int i =0 ; i < CostMatrix.cols ; i++) {
                minMaxLoc(CostMatrix.col(i),&minVal, NULL, NULL, NULL);
         CostMatrix.col(i) = CostMatrix.col(i) - minVal;
        }
        showCostMatrix(step);
	*step = 3; 

}
/*Step 3: Cover all zeros with minimum number of row/col lines, if #lines = rank of matrix (# rows) 
 * then we finished. if not go to step 4 */
void HungarianAlgorithm::step_three(int* step){
	
	getAssignmentMatrix(); 

	cout <<" Step 3:" << endl <<  "Assignment Matrix : " << endl << AssignmentMatrix << endl ;
	
	//Draw lines to cover all zeros 
	//Mark all rows that have no assigned zeros 
	//Mark all unmarked cols that have zeros in newly marked rows. 
	//Mark all unmarked rows that have assigned zeros in newly marked cols. 
        //Repeat until there is not cols/rows to mark. 
        MaskCol = Mat::zeros(1,CostMatrix.rows,CV_64FC1);
	MaskRow = Mat::ones(1,CostMatrix.rows,CV_64FC1);  
	// Mark all rows that have no assigned zeros 
	for(int i = 0 ;i < CostMatrix.rows ; i++ ) {
	   for(int j = 0; j < CostMatrix.cols; j++){
		if(AssignmentMatrix.at<double>(i,j) ==0){ 
			MaskRow.at<double>(i) = 0; 
		 	
		}   	
	   }
	}
	bool change = true ; 
	while(change){ 
	// Mark all cols that have zeros in marked rows.
        change = false ; 
	for(int i = 0 ; i< CostMatrix.rows ; i++ ){
		if(MaskRow.at<double>(i) == 1){ 
		   for(int j = 0 ; j < CostMatrix.cols ; j++){ 
 		        if(CostMatrix.at<double>(i,j) == 0 && MaskCol.at<double>(j) == 0 ){
				MaskCol.at<double>(j) = 1; 
				change = true ; 
			}
		    }
		}
	}
	
	// Mark all rows that have assigned zeros in marked cols 
	for(int j = 0 ; j < CostMatrix.cols ; j++) { 
		if(MaskCol.at<double>(j) == 1){
		   for(int i = 0 ; i < CostMatrix.rows ; i++ ) {
		  	if(AssignmentMatrix.at<double>(i,j) == 0 && MaskRow.at<double>(i) == 0 ) { 
				MaskRow.at<double>(i) = 1 ;
				change = true ; 
			}
		   }
		}	
	} // end for 
	
        }// end while 
	// number of lines = #unmarked rows + # marked cols; 
	int Nlines = 0 ; 
	for(int i = 0 ; i < CostMatrix.rows ; i++) { 
	  if(MaskCol.at<double>(i) == 1 ) Nlines++;  
          if(MaskRow.at<double>(i) == 0 ) Nlines++; 	
	}	
        if(Nlines == CostMatrix.rows) *step = 5 ; 
	else *step = 4;

	cout << "MaskCol :" << endl << MaskCol << endl ; 
        cout << "MaskRow :" << endl << MaskRow << endl ; 
	cout << "Nlines :" << endl << Nlines << endl;  
}

/* Step 4: Create additional zeros by: 
 *  1. Find smallest entries that is NOT COVERED by a line 
 *  2. Subtract each UNCOVERED rows by this smallest. 
 *  3. Add this smallest to COVERED cols .
 * */
void HungarianAlgorithm::step_four(int* step){
 
     //1. Find smalest entries that is not covered by a lines. 
      double minVal = 9999999999 ;   // this is INF
      for(int i = 0 ; i < CostMatrix.rows ; i++){ 
         for(int j = 0 ; j < CostMatrix.cols ; j++){ 
	   if(MaskCol.at<double>(j) == 0 && MaskRow.at<double>(i) == 1){
	       if(CostMatrix.at<double>(i,j) < minVal) minVal = CostMatrix.at<double>(i,j); 
	   }
	} 
      }
      cout << "MinVal : " << minVal << endl ; 
      // 2. Subtract each UNCOVERED ROW by this smallest value
      for(int i = 0 ; i < CostMatrix.rows ; i++){ 
        if(MaskRow.at<double>(i) == 1 ){ 
		CostMatrix.row(i) = CostMatrix.row(i) - minVal ; 
	}
      }    
      // 3. Add each COVERED COLs by this smallest value ; 
      for(int j = 0 ; j < CostMatrix.cols ; j++) { 
        if(MaskCol.at<double>(j) == 1) { 
            	CostMatrix.col(j) = CostMatrix.col(j) + minVal;    
	}
      }
      showCostMatrix(step); 
      // Back to step 3  
     *step = 3; 

}
/* Step 5: Find assignment matrix.  
 * 1. Find and rows that have single zero, and assign it,
 * 2. Cross all other 
 * */ 

void HungarianAlgorithm::step_five(int* step){


}
void HungarianAlgorithm::getAssignmentMatrix(){
	
	AssignmentMatrix = CostMatrix.clone(); 
	// For every row, looking for single zero, assign it, and cross out other zeros in the same rows
	// or cols. 
	int zeroCount ; 
        bool hasSingleZero = false; 
        bool done = false ; 
        Mat CompleteMatrix = Mat::zeros(1,CostMatrix.rows,CV_64FC1); 
        int thisRow, thisCol; 
        bool tempflag = true;
 
        while(!done){ 
   	  //Find row that has single zero
   	  hasSingleZero = false ; 
	   for(int i = 0 ; i < AssignmentMatrix.rows ; i++ ) {
	   	zeroCount = 0 ;  
	   	for(int j=0; j < AssignmentMatrix.cols ; j++) { 
		    if(AssignmentMatrix.at<double>(i,j) == 0 && CompleteMatrix.at<double>(i)==0){
		    	++zeroCount ; 
		    	thisCol = j ; 
	   	    }
	   	}
	        if(zeroCount == 1){
	   		hasSingleZero = true;  	
	  		thisRow = i ;
			break ; 
 		} 
	   } //end for 
	  if(hasSingleZero){ 
	    if(CompleteMatrix.at<double>(thisRow) == 1) done = true; 
	    else{ 
	       CompleteMatrix.at<double>(thisRow) = 1 ; 
               for(int j = 0 ; j  < AssignmentMatrix.cols ; j++){ 
			if(AssignmentMatrix.at<double>(thisRow,j) ==0 ){
				AssignmentMatrix.col(j) = - 1; 
			}
    	 	 }
		AssignmentMatrix.row(thisRow)  = -1 ; 
		AssignmentMatrix.at<double>(thisRow,thisCol) = 0 ; 
	    }	 
	  }else{ // not having single zero rows, find the fist zero, do the assignment task and back  
	       tempflag = false ; 
		for(int i =0 ; i < AssignmentMatrix.rows ; i++) { 
		  for(int j = 0 ; j < AssignmentMatrix.cols ; j++) {
			if(AssignmentMatrix.at<double>(i,j) == 0) { 
			   if(CompleteMatrix.at<double>(i) == 1) {
				 done = true ;
				 tempflag = true ;  
			   	 break ; 
			    }else{
				AssignmentMatrix.row(i) = -1 ; 
				AssignmentMatrix.col(j) = -1; 	
				AssignmentMatrix.at<double>(i,j) = 0 ; 		
			   	CompleteMatrix.at<double>(i) = 1 ; 
				tempflag = true ; 
				break ; 
			     } //end else 

			} //end if  
		  }//end for 
		 if(tempflag) break ;
		}// end for 

	}//end else 
      
      }// end while 
}

void HungarianAlgorithm::showCostMatrix(int* step){ 
  cout << "Step :" << *step  << endl << "Cost Matrix : " << endl << CostMatrix << endl; 
}
