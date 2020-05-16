//========================================================================
// Pred4.cc
//========================================================================
// Implementations for the 2LBHT class.
/* +------------------------------------------------------------------------------------------------+
|                                                                                                |
|  +------------------------+                                                                    |
|  |         PC             |                                                                    |
|  +------------------------+                                                                    |
|                 |-----|                                                                        |
|             +---------+                                                                        |
|                |   |     +-------+    +-------+               +-------+                        |
|                |   |     |       |    |       |               |       |                        |
|                |   |     |       |    |       |               |       |                        |
|                |   /m    |       |    |       |               |       |                        |
|                |   |     |       |    |       |               |       |                        |
|                |   |     |       |    |       |               |       |                        |
|                |   +---->+ BHSRT +-+  |  PHT  |   2^k PHTs    |  PHT  |                        |
|                |         |       | |  |       |   <------->   |       |                        |
|                /k        |       | |  |       |               |       |                        |
|                |         |       | |  |       |               |       |                        |
|                |         |       | +-----------------/n-------------->+                        |
|                |         |       |    |       |               |       |                        |
|                |         |       |    |       |               |       |                        |
|                |         +-------+    +-------+  +  +  +  +   +-------+                        |
|                |                          v      v  v  v  v       v                            |
|                |                        XXXXXXXXXXXXXXXXXXXXXXXXXXXX                           |
|                +------------------------->XXXXXXXXXXXXXXXXXXXXXXXX                             |
|                                             XXXXXXXXXXXXXXXXXXXX                               |
|                                                      +                                         |
|                                                      |                                         |
|                                                      |                                         |
|                                                      v                                         |
|                                                 +---------+                                    |
|                                                 |   FSM   |                                    |
|                                                 | Output  |                                    |
|                                                 |  logic  |                                    |
|                                                 +---------+                                    |
|                                                      |                                         |
|                                                      |                                         |
|                                                      v                                         |
|                                                  Prediction!                                   |
|                                                                                                |
+------------------------------------------------------------------------------------------------+ */

#include "Pred4.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

//========================================================================
// HELPER FUNCTIONS
//========================================================================
//------------------------------------------------------------------------
// mask_gen(int)
//------------------------------------------------------------------------
// Generates the mask for index generation

unsigned long mask_gen( int idx_bits )
{
  if ( idx_bits == 0 )
    return 0;
  else {
    unsigned long mask = ( static_cast<unsigned long>( pow( 2.0, static_cast<double>( idx_bits ) ) ) - 1 );
    return mask;
  }
}

//------------------------------------------------------------------------
// index_gen(void* ip)
//------------------------------------------------------------------------
// Generates the index by reinterpret casting the IP

size_t index_gen( void *ip, unsigned long mask )
{
  unsigned long inst_ptr = reinterpret_cast<unsigned long>( ip );
  //std::cout << "The ip is " << inst_ptr << " and the mask is " << mask << std::endl;
  size_t idx = ( inst_ptr & mask );
  //std::cout << "The index for " << ip << " is " << idx << std::endl;
  return idx;
}

//========================================================================
// MEMBER FUNCTIONS
//========================================================================
// Constructors
//------------------------------------------------------------------------
// Pred4()
//------------------------------------------------------------------------
// Default constructor for 2L BHT
Pred4::Pred4()
{
  m_bhsrt_idx    = 0;
  m_pht_idx      = 4;
  m_pht_select   = 2;
  m_bhsrt_length = static_cast<size_t>( pow( 2.0, static_cast<double>( m_bhsrt_idx ) ) );
  m_pht_length   = static_cast<size_t>( pow( 2.0, static_cast<double>( m_pht_idx ) ) );
  m_no_pht       = static_cast<size_t>( pow( 2.0, static_cast<double>( m_pht_select ) ) );
  // Populating the BHSRT
  for ( size_t i = 0; i < m_bhsrt_length; i++ ) {
    bhsrt.push_back( 0 );
  }
  // Populating the entire PHT collection with state 'T'
  for ( size_t i = 0; i < m_no_pht; i++ ) {
    std::vector<STATE> pht;
    for ( size_t j = 0; j < m_pht_length; j++ ) {
      pht.push_back( T );
    }
    pht_collection.push_back( pht );
  }
}
//------------------------------------------------------------------------
// Pred4()
//------------------------------------------------------------------------
// Non default constructor for 2L BHT

Pred4::Pred4( size_t m, size_t n, size_t k )
{
  m_bhsrt_idx    = m;
  m_pht_idx      = n;
  m_pht_select   = k;
  m_bhsrt_length = static_cast<size_t>( pow( 2.0, static_cast<double>( m_bhsrt_idx ) ) );
  m_pht_length   = static_cast<size_t>( pow( 2.0, static_cast<double>( m_pht_idx ) ) );
  m_no_pht       = static_cast<size_t>( pow( 2.0, static_cast<double>( m_pht_select ) ) );
  // Populating the BHSRT
  for ( size_t i = 0; i < m_bhsrt_length; i++ ) {
    bhsrt.push_back( 0 );
  }
  // Populating the entire PHT collection with state 'T'
  for ( size_t i = 0; i < m_no_pht; i++ ) {
    std::vector<STATE> pht;
    for ( size_t j = 0; j < m_pht_length; j++ ) {
      pht.push_back( T );
    }
    pht_collection.push_back( pht );
  }
}

//------------------------------------------------------------------------
// print_contents()
//------------------------------------------------------------------------
// Prints out the following values:
// 0. Details of current object
// 1. Contents of BHSRT
// 2. Contents of PHT collection
//
void Pred4::print_contents()
{
  // 0.
  std::cout << "The member variables are as follows:" << std::endl;
  std::cout << "m, n, k = " << m_bhsrt_idx << ", " << m_pht_idx << ", " << m_pht_select << std::endl;
  std::cout << "BHSRT length   = " << m_bhsrt_length << std::endl;
  std::cout << "PHT length     = " << m_pht_length << std::endl;
  std::cout << "Number of PHTs = " << m_no_pht << std::endl;
  // 1. BHSRT contents
  std::cout << "BHSRT contents:" << std::endl;
  for ( size_t i = 0; i < m_bhsrt_length; i++ )
    std::cout << "BHSRT[" << i << "] = " << bhsrt[i] << std::endl;
  // 2. PHT collection contents
  std::cout << "PHT contents:" << std::endl;
  for ( size_t i = 0; i < m_no_pht; i++ ) {
    for ( size_t j = 0; j < m_pht_length; j++ ) {
      std::cout << "PHT[" << i << "][" << j << "] = " << pht_collection[i][j] << std::endl;
    }
  }
}

//------------------------------------------------------------------------
// predict(void* ip)
//------------------------------------------------------------------------
// Returns a bool prediction: 0 -> not taken, 1 -> taken
bool Pred4::predict( void *ip )
{
  bool pred;
  // Generating the index for the BHSRT
  unsigned long bhsrt_mask = mask_gen( m_bhsrt_idx );
  //std::cout << "BHSRT Mask = " << bhsrt_mask << std::endl;
  size_t bhsrt_idx = index_gen( ip, bhsrt_mask );
  //std::cout << "BHSRT idx  = " << bhsrt_idx << std::endl;

  // Generating the select for the PHT
  unsigned long pht_sel_mask = mask_gen( m_pht_select );

  size_t pht_sel = index_gen( ip, pht_sel_mask );
  /*   std::cout << "PHT Sel ([i]) = " << pht_sel << std::endl;
  std::cout << "BHRST val([j])  = " << bhsrt[bhsrt_idx] << std::endl; */

  STATE curr_state = pht_collection[pht_sel][bhsrt[bhsrt_idx]];
  // Predicting logic
  if ( curr_state == ST || curr_state == T )
    pred = true;
  else
    pred = false;

  return pred;
}

//------------------------------------------------------------------------
// update(void* ip, bool taken)
//------------------------------------------------------------------------
// Updates one BHT element's state

void Pred4::update( void *ip, bool taken )
{
  // Generating the index for the BHSRT
  unsigned long bhsrt_mask = mask_gen( m_bhsrt_idx );
  size_t bhsrt_idx         = index_gen( ip, bhsrt_mask );
  // Generating the select for the PHT
  unsigned long pht_sel_mask = mask_gen( m_pht_select );
  size_t pht_sel             = index_gen( ip, pht_sel_mask );
  // Updating the value of the PHT entry
  STATE curr_state = pht_collection[pht_sel][bhsrt[bhsrt_idx]];
  switch ( curr_state ) {
    case ST:
      //std::cout << "The state ST is " << curr_state << std::endl;
      pht_collection[pht_sel][bhsrt[bhsrt_idx]] = ( taken ? ST : T );
      break;
    case T:
      //std::cout << "The state T is " << curr_state << std::endl;
      pht_collection[pht_sel][bhsrt[bhsrt_idx]] = ( taken ? ST : NT );
      break;
    case NT:
      //std::cout << "The state NT is " << curr_state << std::endl;
      pht_collection[pht_sel][bhsrt[bhsrt_idx]] = ( taken ? T : SNT );
      break;
    case SNT:
      //std::cout << "The state SNT is " << curr_state << std::endl;
      pht_collection[pht_sel][bhsrt[bhsrt_idx]] = ( taken ? NT : SNT );
      break;
    default:
      std::cout << "Defaulting\n";
      std::cout << "Default value is " << curr_state << std::endl;
      pht_collection[pht_sel][bhsrt[bhsrt_idx]] = ST;
      break;
  }
  // Updating the BHSRT entry to reflect new history
  if ( !taken )
    bhsrt[bhsrt_idx] = ( ( bhsrt[bhsrt_idx] << 1 ) & mask_gen( m_pht_idx ) );
  else
    bhsrt[bhsrt_idx] = ( ( ( bhsrt[bhsrt_idx] << 1 ) + 1 ) & mask_gen( m_pht_idx ) );
  //std::cout << "Branch was " << taken << " therefore BHSRT is " << bhsrt[0] << std::endl;
  return;
}

//========================================================================
// DFT FUNCTIONS: getters and setters
//========================================================================
std::map<char, size_t> Pred4::get_dims()
{
  std::map<char, size_t> dims;
  dims['m'] = m_bhsrt_idx;
  dims['n'] = m_pht_idx;
  dims['k'] = m_pht_select;
  return dims;
}

int Pred4::get_state( size_t n, size_t k )
{
  return pht_collection[n][k];
}

double Pred4::get_size()
{
  double size = static_cast<double>((2*m_no_pht*m_pht_length)+(m_pht_idx*m_bhsrt_length));
  return size;
}
