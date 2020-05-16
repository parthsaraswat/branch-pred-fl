//========================================================================
// UTST
//========================================================================
// A very simple set of macros for unit testing.
//
// Author: Yanghui Ou
// Date  : September 13, 2019
//

#ifndef UTST_H
#define UTST_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

// __n > 0 means zoom-in mode, __n == 0 means zoon-out mode
static int __n = 1;

//------------------------------------------------------------------------
// UTST_ASSERT_TRUE( expr_ )
//------------------------------------------------------------------------
// Checks to see if the expression is true.

#define UTST_ASSERT_TRUE( expr_ )                                    \
  if ( !( expr_ ) ) {                                                \
    if ( __n == 0 )                                                  \
      printf( "\n" );                                                \
    printf( " - [ " RED "FAILED" RESET " ] Line %d\n", __LINE__ );   \
    exit( 1 );                                                       \
  }                                                                  \
  else if ( __n > 0 ) {                                              \
    printf( " - [ " GREEN "passed" RESET " ] Line %d\n", __LINE__ ); \
  }                                                                  \
  else {                                                             \
    printf( GREEN "." RESET );                                       \
  }

//------------------------------------------------------------------------
// UTST_ASSERT_FALSE( expr_ )
//------------------------------------------------------------------------
// Checks to see if the expression is false.

#define UTST_ASSERT_FALSE( expr_ )                                   \
  if ( ( expr_ ) ) {                                                 \
    if ( __n == 0 )                                                  \
      printf( "\n" );                                                \
    printf( " - [ " RED "FAILED" RESET " ] Line %d\n", __LINE__ );   \
    exit( 1 );                                                       \
  }                                                                  \
  else if ( __n > 0 ) {                                              \
    printf( " - [ " GREEN "passed" RESET " ] Line %d\n", __LINE__ ); \
  }                                                                  \
  else {                                                             \
    printf( GREEN "." RESET );                                       \
  }

//------------------------------------------------------------------------
// UTST_ASSERT_INT_EQ( expr0_, expr1_ )
//------------------------------------------------------------------------
// Checks to see if the two expressions are equal using the != operator.

#define UTST_ASSERT_INT_EQ( expr0_, expr1_ )                                                                     \
  if ( (int)( expr0_ ) != (int)( expr1_ ) ) {                                                                    \
    if ( __n == 0 )                                                                                              \
      printf( "\n" );                                                                                            \
    printf( " - [ " RED "FAILED" RESET " ] Line %d: %d != %d\n", __LINE__, (int)( expr0_ ), (int)( expr1_ ) );   \
    exit( 1 );                                                                                                   \
  }                                                                                                              \
  else if ( __n > 0 ) {                                                                                          \
    printf( " - [ " GREEN "passed" RESET " ] Line %d: %d == %d\n", __LINE__, (int)( expr0_ ), (int)( expr1_ ) ); \
  }                                                                                                              \
  else {                                                                                                         \
    printf( GREEN "." RESET );                                                                                   \
  }

//------------------------------------------------------------------------
// UTST_ASSERT_FLOAT_EQ( expr0_, expr1_ )
//------------------------------------------------------------------------
// Checks to see if the two expressions are within eps_ of each other.

#define UTST_ASSERT_FLOAT_EQ( expr0_, expr1_, eps_ )                                                                    \
  if ( fabs( ( expr0_ ) - ( expr1_ ) ) > (double)( eps_ ) ) {                                                           \
    if ( __n == 0 )                                                                                                     \
      printf( "\n" );                                                                                                   \
    printf( " - [ " RED "FAILED" RESET " ] Line %d:  %f != %f\n", __LINE__, (double)( expr0_ ), (double)( expr1_ ) );   \
    exit( 1 );                                                                                                          \
  }                                                                                                                     \
  else if ( __n > 0 ) {                                                                                                 \
    printf( " - [ " GREEN "passed" RESET " ] Line %d:  %f == %f\n", __LINE__, (double)( expr0_ ), (double)( expr1_ ) ); \
  }                                                                                                                     \
  else {                                                                                                                \
    printf( GREEN "." RESET );                                                                                          \
  }

#endif
