#ifndef PREDICTOR_H
#define PREDICTOR_H

/*
    Hao Zhuang, 
    hazhuang@eng.ucsd.edu
    A53029734 
    Department of Computer Science and Engineering
    University of California, San Diego, CA

    CSE240A 2013 Fall Branch Predictor Contest

References
 [1] D. A. Jimenez and C. Lin, "Neural Methods for Dynamic Branch Prediction", TOCS 2002.
 [2] D. A. Jimenez and C. Lin, "Dynamic Branch Prediction with Perceptrons", HPCA 2001 
 [3] D. A. Jimenez, "Piecewise Linear Branch Prediction", ISCA 2005.

*/


// *--------!!!!  All the parameters are defined in predictor.C !!!!! 
// Following is untouched 



/*
  Define all your tables and their sizes here.
  All tables must be statically-sized.
  Please don't call malloc () or any of its relatives within your
  program.  The predictor will have a budget, namely (32K + 256) bits
  (not bytes).  That encompasses all storage (all tables, plus GHR, if
  necessary -- but not PC) used for the predictor.  That may not be
  the amount of storage your predictor uses, however -- for example,
  you may implement 2-bit predictors with a table of ints, in which
  case the simulator will use more memory -- that's okay, we're only
  concerned about the memory used by the simulated branch predictor.
*/





/*
  Initialize the predictor.
*/
void init_predictor ();

/*
  Make a prediction for conditional branch instruction at PC 'pc'.
  Returning true indicates a prediction of taken; returning false
  indicates a prediction of not taken.
*/
bool make_prediction (unsigned int pc);

/*
  Train the predictor the last executed branch at PC 'pc' and with
  outcome 'outcome' (true indicates that the branch was taken, false
  indicates that the branch was not taken).
*/
void train_predictor (unsigned int pc, bool outcome);

#endif
