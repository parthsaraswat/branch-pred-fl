//========================================================================
// 1L-bht-directed-tests.cc
//========================================================================
// This file contains the tests for the 1L bht predictor

#include <stdio.h>
#include <stdlib.h>
#include "utst.h"
#include "../src/Pred3.h"

//------------------------------------------------------------------------
// test_case_1_def_constructor
//------------------------------------------------------------------------
// A simple test case that tests the default constructor
void test_case_1_def_constructor()
{
  printf( "\n%s\n", __func__ );
  Pred3 p;
  size_t size = p.get_size();

  // Checks whether the BHT was initialised to the right size or not
  UTST_ASSERT_INT_EQ( size, 16 );

  // Checks whether all elements in the BHT were initialised to the right state
  for ( size_t i = 0; i < size; i++ ) {
    UTST_ASSERT_INT_EQ( p.get_state( i ), 1 );
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
  for ( size_t i = 0; i < 20; i++ ) {
    Pred3 p( i );
    UTST_ASSERT_INT_EQ( p.get_size(), pow( 2, i ) );
  }
}

//------------------------------------------------------------------------
// test_case_3_predict
//------------------------------------------------------------------------
// tests the predict function

void test_case_3_predict()
{
  printf( "\n%s\n", __func__ );
  Pred3 p;
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
  Pred3 p( 5 );

  //Pushing all elements to WNT and checking

  for ( size_t i = 0; i < p.get_size(); i++ ) {
    void *ip = reinterpret_cast<void *>( i );
    p.update( ip, 0 );
    UTST_ASSERT_INT_EQ( p.get_state( i ), 2 );
  }
  void *ip = reinterpret_cast<void *>( 12 );

  // Pushing element 12 to SNT and checking

  p.update( ip, 0 );
  UTST_ASSERT_INT_EQ( p.get_state( 12 ), 3 );

  // Pushing elements into SNT, to make sure they stay in SNT
  for ( size_t i = 0; i < p.get_size(); i++ ) {
    void *ip = reinterpret_cast<void *>( i );
    for ( size_t j = 0; j < 5; j++ ) {
      p.update( ip, 0 );
    }
    UTST_ASSERT_INT_EQ( p.get_state( i ), 3 );
  }
}

//------------------------------------------------------------------------
// test_case_5_integration
//------------------------------------------------------------------------
// integrates everything and tests

void test_case_5_integration()
{
  printf( "\n%s\n", __func__ );
  Pred3 p( 12 );
  void *ip  = reinterpret_cast<void *>( 0xdead );
  void *ip1 = reinterpret_cast<void *>( 35 );
  void *ip2 = reinterpret_cast<void *>( 153 );
  void *ip3 = reinterpret_cast<void *>( 3500 );

  // Initial check
  bool pred = p.predict( ip );
  UTST_ASSERT_TRUE( pred );

  // performing selective updates
  p.update( ip1, 1 );
  p.update( ip2, 0 );
  p.update( ip3, 1 );

  // Checking if correct states have been set
  UTST_ASSERT_INT_EQ( p.get_state( 35 ), 0 );
  UTST_ASSERT_INT_EQ( p.get_state( 153 ), 2 );
  UTST_ASSERT_INT_EQ( p.get_state( 3500 ), 0 );

  // Checking if the predictions are correct
  UTST_ASSERT_TRUE( p.predict( ip1 ) );
  UTST_ASSERT_FALSE( p.predict( ip2 ) );
  UTST_ASSERT_TRUE( p.predict( ip3 ) );
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
  if ( ( __n == 0 ) || ( __n == 5 ) )
    test_case_5_integration();
}