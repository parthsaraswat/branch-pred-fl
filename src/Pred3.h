//========================================================================
// Pred3.h
//========================================================================
// Declarations for the 1LBHT class.
/*---------------------------------------------------------------------------+
|1 level BHT predictor                                                       |
+----------------------------------------------------------------------------+
|     +---------+     +-----+                                                |
|     |   PC    |     |  T  |                                                |
|     +---------+     +-----+                                                |
|          +-+        |  T  |                                                |
|           |         +-----+                                                |
|           |         | NT  |                                                |
|           |         +-----+                                                |
|           |         | SNT |                                                |
|           |         +-----+                                                |
|           +-------->+ WNT |                                                |
|                     +-----+                                                |
|                     | ST  |                                                |
|                     +--+--+                                                |
|                        |                                                   |
|                        |                                                   |
|                        |    +------------------+                           |
|                        +--->+ FSM output logic +                           |
|                             +--------+---------+                           |
|                                      |                                     |
|                                      |                                     |
|                                      |                                     |
|                                      v                                     |
|                                  Prediction!                               |
|                                                                            |
+----------------------------------------------------------------------------+
----------------------------------------------------------------------------- */


#include <stdio.h>
#include <math.h>
#include <vector>
//#include "pin.H"

class Pred3 {
  public:
  //constructor
    Pred3();
    Pred3(int n_idx_bits);
  //methods
    size_t  index_gen( void* ip );
    unsigned long mask_gen( int n_bits );
    void update( void* ip, bool taken );
    bool predict( void* ip );
    void print_bht();
    size_t get_size();
    void set_size( size_t );
    int get_state( size_t );
  //member vars
  private:
    enum                STATE {ST, T, NT, SNT};
    unsigned long       m_mask;
    size_t              m_idx_bits;
    size_t              m_length;
    std::vector<STATE>  m_bht;
    bool                m_prediction;
    float               m_correct;
};

