#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _matrixObject{
	double** matrix;
	int columns;
	int rows;
} matrixObject;

typedef struct iHateTHIS{  //this is so I can return two matrices from one function
	matrixObject* matObj;
	matrixObject* vectorObj;
} shittyReturn;

int main(int argc, char* argv[]);
matrixObject* findInverse(matrixObject* Mat);
shittyReturn* trainingDataToMatrix(char* fileName);
matrixObject* createIdentity(int size);
void printMatrix(matrixObject* Mat);
int findNonZeroRow(int col, matrixObject* Mat);
matrixObject* findTranspose(matrixObject* Mat);
matrixObject* multiplyMatrices(matrixObject* Mat1, matrixObject* Mat2);
matrixObject* findWeights(matrixObject* X, matrixObject* Y); //variable names are from the assignment instructions

int main(int argc, char* argv[]){
	//gets data from training file
	shittyReturn* data = trainingDataToMatrix(argv[1]);
	matrixObject* Mat = data -> matObj;
	matrixObject* Vect = data -> vectorObj;

	matrixObject* Weights = findWeights(Mat, Vect);	
	
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
	double** matrix = Mat -> matrix;
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


shittyReturn* trainingDataToMatrix(char* fileName){
	FILE* fp = fopen(fileName, "r");
	int attributes;
	int sets;

	fscanf(fp, "%d\n", &attributes);
	fscanf(fp, "%d\n", &sets);
	
	//creates empty column vector
	double** prices = (double**)malloc(sizeof(double*) * sets);
	int lol;
	for(lol = 0; lol < 1; lol++){
		prices[lol] = (double*)malloc(sizeof(double) * 1);
	}//this is so extra i'm sorry 

	//creates empty matrix
	double** matrix =(double**)malloc(sizeof(double*) * sets);	
	int i;
	for(i = 0; i < sets; i++){
		matrix[i] = (double*)malloc(sizeof(double) * attributes);
	}

	//scans numbers into matrix
	i = 0;
	while(i < sets){
		int j = 0;
		while(j < attributes){
			if(feof(fp)){
				break;
			}
			fscanf(fp,"%lf,", &matrix[i][j]); 
			j++;
		}
		if(feof(fp)){
			break;
		}
		fscanf(fp, "%lf\n", &prices[i][1]);
		i++;
	}	
	
	// create matObj and set members
	matrixObject* matObj = (matrixObject*)malloc(sizeof(matrixObject));
	matObj -> matrix = matrix;
	matObj -> columns = attributes;
	matObj -> rows = sets;

	//create vectorObj and set members
	matrixObject* vectorObj = (matrixObject*)malloc(sizeof(matrixObject));
	vectorObj -> matrix = prices;
	vectorObj -> rows = sets;
	vectorObj -> columns = 1;

	//puts matObj and vectorObj into shittyReturnImplementation
	shittyReturn* shit = (shittyReturn*)malloc(sizeof(shittyReturn));
	shit -> matObj = matObj;
	shit -> vectorObj = vectorObj;
	
	return shit;
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
	
	 //iterates thru columns
	 int colIt;
	for(colIt = 0; colIt < size; colIt++){ //while not done reducing all rows
		int rowIt = colIt; //iterates through rows, but note that you only need to iterate thru specific rows tht correspond with unreduced cols
		for(rowIt = colIt; rowIt < size; rowIt++){ //divides all elements in specific rows by the first element
			double  div = original[rowIt][colIt]; //gets the first element
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
		
		printMatrix(Mat);
		printMatrix(newMat);
	}
	//AT THIS POINT, IT SHOULD BE AN REF MATRIX WITH 1s IN THE PIVOT POSITIONS 
	
	int rowIt1;
	for(rowIt1 = 0;rowIt1 < size-1; rowIt1++){//for each row except for the last
		for(colIt = rowIt1 + 1;colIt < size; colIt++){
			double numInCol = original[rowIt1][colIt]; //gets element 
			 //for each NONZERO column, SUBTRACT a corresponding pivot column until it becomes zero
			 
			addRows(Mat, rowIt1, colIt, (double)(-1 * numInCol));
			addRows(newMat, rowIt1, colIt, (double)(-1 * numInCol));
		}
	}
	
	return newMat;

}

matrixObject* findTranspose(matrixObject* Mat){
	double** origMatrix = Mat -> matrix;
	int origRows = Mat -> rows;
	int origCols = Mat -> columns;
	

	//creates empty matrix w/ opposite dimensions
	double** newMatrix = (double**)malloc(sizeof(double*) * origCols);
	int i;
	for(i = 0; i < origCols; i++){
		newMatrix[i] = (double*)malloc(sizeof(double) * origRows);
	}

	//fills empty matrix
	int j;
	for(j = 0; j < origRows; j++){
		int k;
		for(k = 0; k < origCols; k++){
			newMatrix[k][j] = origMatrix[j][k];
		}
	}

	//create matrix struct and inititalizes members
	matrixObject* newMat = (matrixObject*)malloc(sizeof(matrixObject));
	newMat -> matrix = newMatrix;
	newMat -> columns = origRows;
	newMat -> rows = origCols;

	return newMat;
	
}

matrixObject* multiplyMatrices(matrixObject* Mat1, matrixObject* Mat2){
	double** matrix1 = Mat1 -> matrix;
	double** matrix2 = Mat2 -> matrix;

	int rows1 = Mat1 -> rows;
	int rows2 = Mat2 -> rows;
	//int cols1 = Mat1 -> columns;  //in a properly formatted input, this should be the same as rows2
	int cols2 = Mat2 -> columns;

	//create the resulting matrix: dimensions are rows1 x cols2
	double** newMatrix = (double**)malloc(sizeof(double*) * rows1);
	int i;
	for(i = 0; i < rows1; i++){
		newMatrix[i] = (double*)malloc(sizeof(double) * cols2);
	}

	//multiplies and sets items in new matrix
	int j;
	for(j = 0; j < rows1; j++){
		int k;
		for(k = 0; k < cols2; k++){
			int sum = 0;
			int iter;
			for(iter = 0; iter < rows2; iter++){
				sum = matrix1[j][iter] + matrix2[iter][k];
			}

			newMatrix[j][k] = sum;
		}
	}

	//creates new matrix struct and initializes members
	matrixObject* newMat = (matrixObject*)malloc(sizeof(matrixObject));
	newMat -> matrix = newMatrix;
	newMat -> rows = rows1;
	newMat -> columns = cols2;

	return newMat;
	
}

matrixObject* findWeights(matrixObject* X, matrixObject* Y){
	matrixObject* transX = findTranspose(X);
	matrixObject* transX_x_X = multiplyMatrices(transX, X);
	matrixObject* inv = findInverse(transX_x_X);
	matrixObject* inv_x_transX = multiplyMatrices(inv,transX);
	matrixObject* inv_x_transX_x_Y = multiplyMatrices(inv_x_transX, Y);
	//sorry about variable names

	return inv_x_transX_x_Y;
}




























