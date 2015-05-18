
/*
    Hao Zhuang, 
    hao.zhuang@cs.ucsd.edu
    Department of Computer Science and Engineering
    University of California, San Diego, CA
*/


/*
    CSE240A 2013 Fall Branch Predictor Contest

References
 [1] D. A. Jimenez and C. Lin, "Neural Methods for Dynamic Branch Prediction", TOCS 2002.
 [2] D. A. Jimenez and C. Lin, "Dynamic Branch Prediction with Perceptrons", HPCA 2001 
 [3] D. A. Jimenez, "Piecewise Linear Branch Prediction", ISCA 2005.

*/

#include <math.h>
#include "predictor.h"

/* 
 * Table initilization 
 */

#define h 15 
#define n 16  // should be power of 2 
#define m 16   // should be power of 2  

int weight_width = 8;


// the size of weight table: n*m*(h+1) * 8 
int W[n][m][h+1];


// the size of GHR table is equivalent to h bit 
bool GHR[h]  ; 


// the size of GA (address) log2(m)*h bits, here is 4*h 
int GA[h]   ; 


/* 
 To Ashish: This is the place of table size calculation: 
 
 Total size = n*m*(h+1) + h + log2(m)*h = 32843 (bits) 
 < 33024 = 32K + 256 (bits) constraint  
 
*/




/* 
 * Parameters initilization 
 */

// for the weight bound 
int _MAX_VAL = (int) (pow(2,weight_width-1)); // upper bound 
int _MIN_VAL = (-1)*(int) pow(2,weight_width-1); // lower bound 

// int threshold = (int) ( floor(1.93*(h)  + 14)) ; // try the HPCA 01, TOCS 2002 paper
int threshold = (int) ( floor(2.14*(h+1)  + 20.58)) ; // try 

int output =  0; // variable to collect weights and then make prediction correspondingly 
bool predict = false; // current prediction

void init_predictor ()
{

    // init. the weight coeff. 3D array
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < m ; j++)
        {
            for( int k = 0 ; k < h + 1 ; k++)
            {
                W[i][j][k] = 0;
            }
        }
    }

    // init. the GA and GHR
    for( int k = 0 ; k < h ; k++){

        GA[k] = 0 ;
        GHR[k] = false ; 
    
    }
}

bool make_prediction (unsigned int pc)
{
    int address = pc % n;           // hash to the weight array  
    output = W[address][0][0];      // load the bias value  

    for(int i = 0 ; i < h  ; i++){
        // add the training values 
        int ga = GA[i];
        if ( GHR[i] == true ) 
        {
            output += W[address][ga][i+1];
        }
        else
        {
            output -= W[address][ga][i+1];
        }

    }

    // make the prediction 
    if( output >= 0 )  
        predict = true;
    else
        predict = false;

    return predict ;

}

void train_predictor (unsigned int pc, bool outcome)
{
    int address = pc % n;

    // training entrance:  outcome contradict the prediction ||  under threshold 
    if(  ( predict != outcome ) || (  ( output < threshold) && ( output > -threshold))   ) 
    {

        // update the bias value  
        if ( outcome == true){
            if(  W[address][0][0] < _MAX_VAL) // upper bound check to go up 
            {
                W[address][0][0] +=1;
            }
        }
        else
        {
            if (  W[address][0][0] > _MIN_VAL  )// lower bound check to go down  
            {
                W[address][0][0] -=1;
            }
        }

        // update the traning values 
        for(int i = 0 ; i < h ; i++){
            int ga = GA[i] ;        // branch address hash to the weight array
            if (GHR[i] == outcome ){

                if( W[address][ga][i+1] < _MAX_VAL) // upper bound check  to go up :w
                {
                    W[address][ga][i+1] += 1 ; 
                }
            }
            else
            {
                if(( W[address][ga][i+1] > _MIN_VAL) ) // lower bound check to go down 
                {
                    W[address][ga][i+1] -= 1 ; 
                }
            }
        }
    } 
    // end of training 

    // move the current outcome into GHR and current branch address into GA 
    for ( int i = h-1 ; i >= 1 ; i-- ){
        GA[i] = GA[i-1];
        GHR[i] = GHR[i-1];
    }
    GA[0] = pc % m ;
    GHR[0] = outcome ;
    // end of updating GHR and GA

}

// end of code 
// coded by Hao Zhuang @ UCSD CSE
// 2013.11 


