#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct matrixObject{
        int* matrix = NULL;
        int columns = NULL;
        int rows = NULL;
} matrixObject;

int main(int argc, char* argv[]){
        int* matrix = readTrainingData("idfk");


}

matrixObject* readTrainingData(char* fileName){
        FILE* fp = fopen(fileName, "r");
        int attributes= 0;
        int sets = 0;

        int matrix[sets][attributes] = NULL;

        int i = 0;
        while(i < sets){
                int j = 0;
                while(j < attributes){
                        fscanf(fp,"%f", &matrix[i][j]);

                        j++;
                }
                i++;

                char bum;
                fscanf(fp, "%c", &bum);
        }

        matrixObject Mat = NULL;
        Mat.matrix = matrix;
        Mat.columns = attributes;
        Mat.rows = sets;
        return Mat;
}

 
 
 void findInverse(matrixObject Mat){

        //gets data of matrix to RREF
        int rows = Mat.rows;
        int columns = Mat.columns;
        int* original =Mat.matrix;


        //creates identity matrix
        int* new[rows][columns];
        int i = 0;
        while(i < rows){
                int j = 0;
                while(j < columns){
                        if(i == j){
                                new[i][j] = 1;
                        }else{
                                new[i][j] = 0;
                        }
                        j++;
                }
                i++;
        }

        //iterate thru each row in each column
        //get the first number in each row
        //divide the entire row by the first number
        //unless that first number is 0
        //subtract all of the elements in other rows by the elements in the first row

        int colIt = 0; //iterates thru columns
        while(colIt < columns){ //while not done reducing all rows
                int rowIt = colIt; //iterates through rows, but note that you only need to iterate thru specific rows tht correspond with unreduced cols
                while(rowIt < rows){ //divides all elements in specific rows by the first element
                        int div = original[rowIt][0]; //gets the first element
                        if(div == 0){ //need to 
                                //go thru each row until you find a nonzero
                                //add the div = 0 row to the div = nonzero
                                //if I put a continue statement w/o incrementing, it might divide the row by the 1st element 

                                int idkIt = colIt;//this probably sets the iterator to iterate btwn all possible pivot cols until it finds the pivot col
                                int pivotRow = -1;
                                while(idkIt < rows){
                                        int nnn = original[idkIt][colIt]; //gets first nonzero element in desired row
                                        if(nnn != 0){
                                                pivotRow = idkIt;
                                                break;
                                        }
                                        idkIt++;
                                }


                                //at this point, you should know which row is nonzero, via the pivotRow variable
                                //now add nonpivot =  pivot + nonpivot 
    
                                //nonpivot is represented by rowIt
                                //pivot by pivotRow, 
    
                                int lmaoIt = colIt; //begins with 1st element
                                while(lmaoIt < columns){
                                        original[rowIt][lmaoIt] = original[rowIt][lmaoIt] + original[pivotRow][lmaoIt];
                                        lmaoIt++;
                                }

                                continue; //if I put a continue statement w/o incrementing, the next loop will go thru the same row probably
                        }

                                //divide everything in row by the div
                                //also perform the same operation on the "new" matrix
                                int colIt2 = 0;
                                while(colIt2 < columns){
                                        int origNum = original[rowIt][colIt2];
                                        original[rowIt][colIt2] = origNum /div;

                                        int newNum = new[rowIt][colIt2];
                                        new[rowIt][colIt2] = newNum / div;
                                        colIt2++;
                                }
    
                        rowIt++;
                }
                //AT THIS POINT, ALL THE ROWS WILL HAVE A 1 IN THE TARGET COLUMN

                //subtract the "pivot row" from the "non-pivot rows"
                //how do we know what's a pivot row?
                //the row that rowIt is currently on is a pivot row. 
                //to clarify: only subtract from the non-pivot rows after the pivot row. 
    
                int roroIt = colIt + 1; //this might cause array out of bounds at some point!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                while(roroIt < rows){//iterates thru rows
                        int colcolIt = colIt;
                        while(colcolIt < columns){//goes thru each element in each row
                                original[roroIt][colcolIt] = original[roroIt][colcolIt] - original[colIt][colcolIt]; //this is shitty...but colIt = rowIt b4 iteration so it should work        
                                colcolIt++;
                        }
                        roroIt++;
                }
    
                colIt++;
        }
        //AT THIS POINT, IT SHOULD BE AN REF MATRIX WITH 
    
    

}
