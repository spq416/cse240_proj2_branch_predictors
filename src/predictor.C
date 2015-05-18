#include <string.h>
//first I would like to announce the usage of the resources in the predictor
// The whole predictor is based on the neural network predictor
// so there are basically two parts in 	the predictor
// first is the weight matrix, which is a two dimensional matrix, the row indicate the entry number, which is resulted by current PC address
// the column corresponding to history, if history in that position is taken, then plus, otherwise, minus
// so we can start from define the matrix property
#define NumEntry 160
#define history_length 24


//then the second part is history, we can represent history just by using a single integer


//we have two part for the prediction part, but we still need train part.
// in train part, we should define some threshold for weight and output, in case that to improve the accuracy
//first define the max and min value for the weight value to enable fast response to change in trace
//each weight I use 8 bits, so it is easy to derive the max and min value
#define max_weight 127
#define min_weight -128

// then we will have threshold value for the output, this is according to the paper
#define max_threshold (1.93*history_length+14)
#define min_threshold (-1*max_threshold)

int entry;
int output;
bool predictor;
int weight[NumEntry][history_length];
int bias[NumEntry];
int GHR;
void init_predictor ()
{
  memset(weight,0,sizeof(weight));
  memset(bias,0,sizeof(bias));
  GHR = 0;
}

bool make_prediction (unsigned int pc)
{
  int GHR_detector=1;
  entry = pc % NumEntry;
  output=bias[entry];//set the initial value of output as the bias value 
  //according to the history bit to decide whether plus weight or minus weight
  for(int i=0; i < history_length; i++){
         if((GHR&GHR_detector) == 0)
                output -= weight[entry][i];
         else
                output += weight[entry][i];
         GHR_detector = GHR_detector << 1;
        }
  if( output>= 0 )
        predictor = true;
  else
        predictor = false;

 return predictor;
}

void train_predictor (unsigned int pc, bool outcome)
{
 int bias_change;
 int GHR_change;
 int GHR_detector=1;
 int coefficient;	
 if(outcome == true){
        bias_change = 1;
 }
 else{
        bias_change = -1;
 }
 if(outcome!=predictor || (output<max_threshold && output > min_threshold))
 {
              if(bias[entry]>min_weight && bias[entry] < max_weight)
                bias[entry] = bias[entry] + bias_change;
        
        for(int i=0; i < history_length; i++){        
                if( ( ( (GHR&GHR_detector) != 0) && (outcome == true) ) || ( ((GHR&GHR_detector) == 0)&&(outcome == false) ) )
                        coefficient = 1;
                else
                        coefficient  = -1;
                if ( weight[entry][i] > min_weight && weight[entry][i] < max_weight ) {
                        weight[entry][i] = weight[entry][i] + coefficient;
                }
                GHR_detector = GHR_detector << 1;
        }
 }
GHR = ( GHR << 1)  ;
if( outcome==true)
     GHR = GHR+1;
}



