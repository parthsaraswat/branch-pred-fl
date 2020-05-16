//========================================================================
// Pred3.cc
//========================================================================
// Implementations for 1L BHT
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


#include "Pred3.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>

// Getter functions
//------------------------------------------------------------------------
// get_size()
//------------------------------------------------------------------------
// gets the size of the BHT

size_t Pred3::get_size()
{
  return m_length;
}

//------------------------------------------------------------------------
// set_size()
//------------------------------------------------------------------------

void Pred3::set_size( size_t n )
{
  m_idx_bits = n;
  m_length   = static_cast<size_t>(pow(2.0, static_cast<double>(m_idx_bits)));
}
//------------------------------------------------------------------------
// get_state(idx)
//------------------------------------------------------------------------
// gets the contents of the BHT at index 'idx'

int Pred3::get_state( size_t idx )
{
  return m_bht[idx];
}
// Helper functions
//------------------------------------------------------------------------
// print_bht()
//------------------------------------------------------------------------
// Prints the contents of the BHt


void Pred3::print_bht()
{
  std::cout <<"Current contents of BHT are:" << std::endl;
  for (size_t i = 0; i < m_length; i++)
    std::cout <<"BHT["<<i<<"]: "<< m_bht[i] << std::endl;
}

// CONSTRUCTORS:
//------------------------------------------------------------------------
// Pred3()
//------------------------------------------------------------------------
// Default constructor for 1L BHT
Pred3::Pred3()
{
  m_idx_bits = 4;
  m_mask     = 15;
  m_length   = 16;
  for (int i = 0; i < 16; i++){
      m_bht.push_back(T);
  }
}

//------------------------------------------------------------------------
// Pred3( int n_idx_bits )
//------------------------------------------------------------------------
//Non default constructor for 1L BHT

Pred3::Pred3(int n_idx_bits)
{
  m_idx_bits      = n_idx_bits;
  m_length        = static_cast<size_t>(pow(2.0, static_cast<double>(m_idx_bits)));
  m_mask          = mask_gen(m_idx_bits);
  for (size_t i = 0; i < m_length; i++){
      m_bht.push_back(T);
  }
}

// MEMBER FUNCTIONS:
//------------------------------------------------------------------------
// mask_gen(int)
//------------------------------------------------------------------------
// Generates the mask for index generation

unsigned long Pred3::mask_gen(int m_idx_bits)
{  
  unsigned long mask = (static_cast<unsigned long>(pow(2.0, static_cast<double>(m_idx_bits))) - 1);
  return mask;
}

//------------------------------------------------------------------------
// index_gen(void* ip)
//------------------------------------------------------------------------
// Generates the index by reinterpret casting the IP


size_t Pred3::index_gen(void *ip)
{
  unsigned long inst_ptr = reinterpret_cast <unsigned long> (ip);
  size_t idx = (inst_ptr & m_mask);
  //std::cout << "The index for " << ip << " is " << idx << std::endl;
  return idx;
}

//------------------------------------------------------------------------
// predict(void* ip)
//------------------------------------------------------------------------
// Returns a bool prediction: 0 -> not taken, 1 -> taken


bool Pred3::predict(void* ip)
{
  size_t idx        = index_gen(ip);
  int curr_state  = m_bht[idx];
    // Predicting logic
    if (curr_state == ST || curr_state == T )
      m_prediction = true;
    else 
      m_prediction = false;
 // print_bht();
 // std::cout << "The prediction is " << m_prediction << std::endl;
  return m_prediction;
}

//------------------------------------------------------------------------
// update(void* ip, bool taken)
//------------------------------------------------------------------------
// Updates one BHT element's state


void Pred3::update( void* ip, bool taken )
{
  size_t idx = index_gen(ip);
  int curr_state = m_bht[idx];
  switch ( curr_state ){
    case ST:
      //std::cout << "The state ST is " << curr_state << std::endl;
      m_bht[idx] = ( taken ? ST : T   );
      break;
    case T:
      //std::cout << "The state T is " << curr_state << std::endl;
      m_bht[idx] = ( taken ? ST : NT  );
      break;
    case NT:
      //std::cout << "The state NT is " << curr_state << std::endl;
      m_bht[idx] = ( taken ? T : SNT  );
      break;
    case SNT:
      //std::cout << "The state SNT is " << curr_state << std::endl;
      m_bht[idx] = ( taken ? NT : SNT );
      break;
    default:
      std::cout << "Defaulting\n";
      std::cout << "Default value is " << curr_state << std::endl;
      m_bht[idx] = ST;
      break;      
    }
    //std::cout << "Updated BHT[" <<idx<<"] to "<< m_bht[idx] <<std::endl;
}

