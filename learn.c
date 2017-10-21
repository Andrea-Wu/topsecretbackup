#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _matrixObject{
	double** matrix;
	int columns;
	int rows;
} matrixObject;

int main(int argc, char* argv[]);
//matrixObject findInverse(matrixObject Mat);
matrixObject* trainingDataToMatrix(char* fileName);
matrixObject* createIdentity(int size);
void printMatrix(matrixObject* Mat);
int findNonZeroRow(int col, matrixObject* Mat);

int main(int argc, char* argv[]){
	matrixObject* hi = createIdentity(3);
	printMatrix(hi);

	return 0;
		
}

matrixObject* createIdentity(int size){
	matrixObject* newMatrix = (matrixObject*)malloc(sizeof(matrixObject));

	//this should create an empty double matrix
        double** mat = (double**)malloc(sizeof(double*) * size);

	int i;
	for(i= 0; i < size; i++){
		mat[i] = (double*)malloc(sizeof(double) * size);
	} 

	//sets the struct's members
        newMatrix -> matrix = mat;
        newMatrix -> rows = size;
        newMatrix -> columns = size;


	//sets the numbers inside of the matrix
	//identity!!
        i = 0;
        while(i < size){
                int j = 0;
                while(j < size){
                        if(i == j){
                                mat[i][j] = (double)1;
                        }else{
                                mat[i][j] = (double)0;
                        }
                        j++;
                }
                i++;
        }

	return newMatrix;
	
}
void printMatrix(matrixObject* Mat){
	double** matrix = (*Mat).matrix;
	int i = 0;

	while(i < Mat -> rows){
		int j = 0;
		while(j < Mat -> columns){
			printf("%f\t", matrix[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}


matrixObject* trainingDataToMatrix(char* fileName){
	FILE* fp = fopen(fileName, "r");
	const int attributes= 0;
	const int sets = 0;
	
	matrixObject* Mat = (matrixObject*)malloc(sizeof(matrixObject));


	//creates empty matrix
	double** matrix = (double**)malloc(sizeof(double*) * sets);	
	int i;
	for(i = 0; i < sets; i++){
		matrix[i] = (double*)malloc(sizeof(double) * attributes);
	}

	//scans numbers into matrix
	i = 0;
	while(i < sets){
		int j = 0;
		while(j < attributes){
			fscanf(fp,"%lf", &matrix[i][j]); 

			j++;
		}
		i++;

		char bum; //this scans the newline character
		fscanf(fp, "%c", &bum);
	}	
	
	//sets struct members
	Mat -> matrix = matrix;
	Mat -> columns = attributes;
	Mat -> rows = sets;
	return Mat;
}

int findNonZeroRow(int col, matrixObject* Mat){//returns the first row below the current one that doesn't have zero in pivot position
	//check for nonzero in column x, in all rows after x
	
	double** thisMatrix = Mat -> matrix;
	int size = Mat -> rows;
	int i;
	int pivotRow = -1;
	for(i = col+1; i < size; i++){
		if(thisMatrix[i][col] != 0){
			pivotRow = i;
		}
	}
	return pivotRow;
}

void addRows(matrixObject* Mat, int base, int addMe, double multiplier){ //base = base + addMe
	//loop to iterate thru row
	int rowSize = Mat -> columns;
	double** matrix = Mat -> matrix;

	int i;
	for(i = 0; i < rowSize; i++){
		matrix[base][i] = matrix[base][i] + (multiplier * matrix[addMe][i]);
	}

}

void scaleRow(matrixObject* Mat, int row, double div){ //t
	int rowSize = Mat -> columns;
	double** matrix = Mat -> matrix;
	
	int i;
	for(i = 0; i < rowSize; i++){
		matrix[row][i] = matrix[row][i]/ div;
	}
	
}

matrixObject* findInverse(matrixObject* Mat){

	//gets data of matrix to RREF
	int size = Mat -> rows;
	double** original = Mat -> matrix;


	//create identity matrix
	matrixObject* newMat = createIdentity(size);

	//iterate thru each row in each column
	//get the first number in each row
	//divide the entire row by the first number
	//unless that first number is 0
	//subtract all of the elements in other rows by the elements in the first row
	
	int colIt = 0; //iterates thru columns
	while(colIt < size){ //while not done reducing all rows
		int rowIt = colIt; //iterates through rows, but note that you only need to iterate thru specific rows tht correspond with unreduced cols
		while(rowIt < size){ //divides all elements in specific rows by the first element
			double  div = original[rowIt][0]; //gets the first element
			if(div == 0){ //need to 
				//go thru each row until you find a nonzero
				//add the div = 0 row to the div = nonzero
				//if I put a continue statement w/o incrementing, it might divide the row by the 1st element 

				int pivotRow = findNonZeroRow(colIt,Mat); 

				//at this point, you should know which row is nonzero, via the pivotRow variable
				//now add nonpivot =  pivot + nonpivot 
			
				addRows(Mat, rowIt, pivotRow, (double)1); //(MatrixName, baseRow, addendRow, multiplier)
				addRows(newMat, rowIt, pivotRow, (double)1);
				continue; //if I put a continue statement w/o incrementing, the next loop will go thru the same row probably
			}

				//divide everything in row by the div
				//also perform the same operation on the "new" matrix
				scaleRow(Mat, rowIt, div);
				scaleRow(newMat, rowIt, div);
				
			printMatrix(Mat);
			printMatrix(newMat);
			printf("\n");
			rowIt++;
		}
		//AT THIS POINT, original: ALL THE ROWS WILL HAVE A 1 IN THE TARGET COLUMN
		//		new: 

		//subtract the "pivot row" from the "non-pivot rows"
		//how do we know what's a pivot row?
		//the row that rowIt is currently on is a pivot row. 
		//to clarify: only subtract from the non-pivot rows after the pivot row. 		

		//int roroIt = colIt + 1;
		int roroIt;
		for(roroIt = colIt + 1; roroIt < size; roroIt++){	
			addRows(Mat, roroIt, colIt, (double)-1);
			addRows(newMat, roroIt, colIt, (double)-1);
		}
		
		colIt++;
		printMatrix(Mat);
		printMatrix(newMat);
	}
	//AT THIS POINT, IT SHOULD BE AN REF MATRIX WITH 1s IN THE PIVOT POSITIONS 
	
	int rowIt1 = 0;


	while(rowIt1 < size-1){//for each row except for the last
		colIt = rowIt1 + 1;
		while(colIt < size){
			double numInCol = original[rowIt1][colIt]; //gets element 
			 //for each NONZERO column, SUBTRACT a corresponding pivot column until it becomes zero
			 
			addRows(Mat, rowIt1, colIt, (double)(-1 * numInCol));
			addRows(newMat, rowIt1, colIt, (double)(-1 * numInCol));
		}
		rowIt1++;
	}
	
	return 0;

}


