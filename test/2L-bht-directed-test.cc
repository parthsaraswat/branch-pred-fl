//========================================================================
// 2L-bht-directed-tests.cc
//========================================================================
// This file contains the tests for the 1L bht predictor

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "utst.h"
#include "../src/Pred4.h"

//------------------------------------------------------------------------
// test_case_1_def_constructor
//------------------------------------------------------------------------
// A simple test case that tests the default constructor
void test_case_1_def_constructor()
{
  printf( "\n%s\n", __func__ );
  Pred4 p;
  // Extracting the dimensions using the design for test function
  std::map<char, size_t> dims;
  dims = p.get_dims();

  // Checks whether the BHT was initialised to the right size or not
  UTST_ASSERT_INT_EQ( dims['m'], 0 );
  UTST_ASSERT_INT_EQ( dims['n'], 4 );
  UTST_ASSERT_INT_EQ( dims['k'], 2 );
  // Checks whether all elements in the BHT were initialised to the right state
  for ( size_t i = 0; i < dims['n']; i++ ) {
    for ( size_t j = 0; j < dims['k']; j++ ) {
      UTST_ASSERT_INT_EQ( p.get_state( 1, 2 ), 1 );
    }
  }
}

//------------------------------------------------------------------------
// test_case_2_non_def_constructor
//------------------------------------------------------------------------
// A simple test case that tests the non default constructor

void test_case_2_nondef_constructor()
{
  printf( "\n%s\n", __func__ );
  // Checks whether the BHT was initialised to the right size or not
  for ( size_t i = 0; i < 3; i++ ) {
    Pred4 p( i, i + 1, i + 2 );

    // Extracting the dimensions using the design for test function
    std::map<char, size_t> dims;
    dims = p.get_dims();

    UTST_ASSERT_INT_EQ( dims['m'], i );
    UTST_ASSERT_INT_EQ( dims['n'], i + 1 );
    UTST_ASSERT_INT_EQ( dims['k'], i + 2 );

    // Checks whether all elements in the PHTs were initialised to the right state
    for ( size_t i = 0; i < dims['n']; i++ ) {
      for ( size_t j = 0; j < dims['k']; j++ ) {
        UTST_ASSERT_INT_EQ( p.get_state( 1, 2 ), 1 );
      }
    }
  }
}

//------------------------------------------------------------------------
// test_case_3_predict
//------------------------------------------------------------------------
// tests the predict function

void test_case_3_predict()
{
  printf( "\n%s\n", __func__ );
  Pred4 p;
  for ( size_t i = 0; i < 20; i++ ) {
    void *ip  = reinterpret_cast<void *>( 0xdead * i );
    bool pred = p.predict( ip );
    UTST_ASSERT_TRUE( pred );
  }
}

//------------------------------------------------------------------------
// test_case_4_update
//------------------------------------------------------------------------
// tests the update function

void test_case_4_update()
{
  printf( "\n%s\n", __func__ );
  Pred4 p( 0, 10, 5 );

  // Extracting the dimensions using the design for test function
  std::map<char, size_t> dims;
  dims = p.get_dims();

  // Checking for state update to WNT
  void *ip = reinterpret_cast<void *>( 12 );
  p.update( ip, 0 );
  UTST_ASSERT_INT_EQ( p.get_state( 12, 0 ), 2 );
}

//------------------------------------------------------------------------
// main
//------------------------------------------------------------------------
int main( int argc, char **argv )
{
  __n = ( argc == 1 ) ? 0 : atoi( argv[1] );

  if ( ( __n == 0 ) || ( __n == 1 ) )
    test_case_1_def_constructor();
  if ( ( __n == 0 ) || ( __n == 2 ) )
    test_case_2_nondef_constructor();
  if ( ( __n == 0 ) || ( __n == 3 ) )
    test_case_3_predict();
  if ( ( __n == 0 ) || ( __n == 4 ) )
    test_case_4_update();
}