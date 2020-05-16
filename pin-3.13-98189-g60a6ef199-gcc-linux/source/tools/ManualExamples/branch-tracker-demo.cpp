/*////////////////
Author:  Parth Saraswat
Program: Branch predictor Pintool for demo purposes
//////////////////*/
#include <stdio.h>
#include <iostream>
#include "pin.H"
//#include "../../../../Pred3.h"
#include "../../../../app/src/Pred3.cc"
#include "../../../../app/src/Pred4.cc"
#include <map>

// USE STATS: 1 for when you want to use stats, 0 for when you want to track and predict all branches
int use_stats = 1;

// Pred objects for checking simulator accuracy
Pred3 p1L( 11 );
// 2-Level predictors for sweeping
Pred4 p2L( 0, 12, 5 );

int g_stats_en      = !( use_stats );
float n_unique      = 0;
float n_branches    = 0;
float n_taken       = 0;
float n_pred_taken  = 0;
float n_pred_ntaken = 0;
float n_correct     = 0;
// Map to keep track of unique branches
std::map<unsigned long, int> unique_branches;

// Variables for accuracy calculation
float n_correct1L = 0;
bool pred1L;
float n_correct2L = 0;
bool pred2L;
float accuracy;
float n_correct_satcounter = 0;
bool pred_satcounter;
// Stats
int nop_tag = 0;

enum STATE { ST,
             T,
             NT,
             SNT };
STATE state = T;

FILE *trace;
FILE *acc;

/* ===================================================================== */
/* Accuracy helper                                                       */
/* ===================================================================== */

VOID AccuracyHelper( bool taken )
{
  if ( g_stats_en ) {
    if ( taken == pred1L ) {
      ( n_correct1L )++;
    }
    if ( taken == pred2L ) {
      ( n_correct2L )++;
    }
    if ( taken == pred_satcounter ) {
      ( n_correct_satcounter )++;
    }
  }
}

// index gen

/* ===================================================================== */
/* Branch predictor models                                               */
/* 1. 2-bit saturating counter: way 1                                    */
/* 2. 2-bit saturating counter: way 2                                    */
/* 3. 1-level BHT wrapper                                                */
/* 4. 2-level BHT wrapper                                                */
/* ===================================================================== */

/*---------------------------------------------------------------------- */
/* 2 bit saturating counter */
/*---------------------------------------------------------------------- */

VOID PredModel1_SatCounter( bool taken )
{
  if ( g_stats_en ) {
    // State outputs:
    if ( state == ST || state == T ) {
      n_pred_taken++;
      pred_satcounter = TRUE;
    }
    else {
      n_pred_ntaken++;
      pred_satcounter = FALSE;
    }
    // State transitions
    switch ( state ) {
      case ST:
        state = ( taken ? ST : T );
        break;
      case T:
        state = ( taken ? ST : NT );
        break;
      case NT:
        state = ( taken ? T : SNT );
        break;
      case SNT:
        state = ( taken ? NT : SNT );
        break;
      default:
        state = ST;
        break;
    }
  }
}

/* ===================================================================== */
/* Enables/disables stats                                                */
/* ===================================================================== */

VOID StatsEnable( VOID *ip )
{
  if ( use_stats ) {
    if ( nop_tag == 1 ) {
      printf("\033[0;32m");
      printf( "stats on\n" );
      g_stats_en = 1;
    }
    else if ( nop_tag == 2 ) {
      printf("\033[0;31m");
      printf( "stats off\n" );
      printf("\033[0m");
      g_stats_en = 0;
    }
  }
}

/* ========================================================================= */
/* Appends to branch trace and counts branches and whether they were taken   */
/* ========================================================================= */

VOID RecordBranch( VOID *ip, BOOL taken )
{
  if ( g_stats_en ) {
    n_branches++;
    unsigned long key;
    key = reinterpret_cast<unsigned long>( ip );
    if ( unique_branches.count( key ) == 0 ) {
      unique_branches[key] = 1;
      n_unique++;
    }
    // push back current branch into vector
  }
}
/* ========================================================================= */
/* Trace generator                                                           */
/* ========================================================================= */

VOID TraceGen( VOID *ip, BOOL taken )
{
  if ( g_stats_en ) {
    fprintf( trace, "IP: %p O: %d\n", ip, taken );
  }
}

/* ========================================================================= */
/* Wrapper for 1LBHT                                                         */
/* ========================================================================= */
VOID Pred3Wrapper( VOID *ip, BOOL taken )
{
  if ( g_stats_en ) {
    void *arg_ip   = ip;
    bool arg_taken = taken;
    pred1L         = p1L.predict( arg_ip );

    p1L.update( arg_ip, arg_taken );
  }
}

/* ========================================================================= */
/* Wrapper for 2LBHT                                                         */
/* ========================================================================= */
VOID Pred4Wrapper( VOID *ip, BOOL taken )
{
  if ( g_stats_en ) {
    // p3.print_contents();
    void *arg_ip   = ip;
    bool arg_taken = taken;
    pred2L         = p2L.predict( arg_ip );

    p2L.update( arg_ip, arg_taken );
  }
}

// Is called for every instruction and instruments branches
VOID Instruction( INS ins, VOID *v )
{
  // Checking for turning stats on and off
  if ( ( INS_Opcode( ins ) == XED_ICLASS_NOP ) && ( INS_OperandCount( ins ) == 2 ) && ( INS_OperandIsMemory( ins, 0 ) ) && ( INS_OperandMemoryBaseReg( ins, 0 ) == 10 ) && ( INS_OperandIsReg( ins, 1 ) ) && ( INS_OperandReg( ins, 1 ) == 56 ) ) {
    nop_tag = INS_OperandMemoryDisplacement( ins, 0 );
    INS_InsertCall( ins, IPOINT_BEFORE, (AFUNPTR)StatsEnable, IARG_INST_PTR, IARG_END );
  }
  //

  if ( INS_IsBranch( ins ) ) {
    // keep a track of number and IP of branches
    INS_InsertCall( ins, IPOINT_BEFORE, (AFUNPTR)RecordBranch,
                    IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END );

    // if you want to enable traces
    /*   INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)TraceGen, 
                     IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END); */

    // Saturating counter
    INS_InsertCall( ins, IPOINT_BEFORE, (AFUNPTR)PredModel1_SatCounter,
                    IARG_BRANCH_TAKEN, IARG_END );

    // Wrapper for 1-level predictor
    INS_InsertCall( ins, IPOINT_BEFORE, (AFUNPTR)Pred3Wrapper,
                    IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END );

    // Wrapper for 2-level predictor
    INS_InsertCall( ins, IPOINT_BEFORE, (AFUNPTR)Pred4Wrapper,
                    IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END );

    // helps calculate accuracy of both predictors
    INS_InsertCall( ins, IPOINT_BEFORE, (AFUNPTR)AccuracyHelper,
                    IARG_BRANCH_TAKEN, IARG_END );
  }
}

VOID Fini( INT32 code, VOID *v )
{
  fprintf( acc, "Total branches  = %f\n", n_branches );
  fprintf( acc, "Unique branches = %f\n", n_unique );
  fprintf( acc, "ACCURACY_SC     = %f percent\n", ( 100 * ( n_correct_satcounter / n_branches ) ) );
  fprintf( acc, "ACCURACY_1L     = %f percent\n", ( 100 * ( n_correct1L / n_branches ) ) );
  fprintf( acc, "ACCURACY_2L     = %f percent\n", ( 100 * ( n_correct2L / n_branches ) ) );

  printf( "Total branches  = %f\n", n_branches );
  printf( "Unique branches = %f\n\n", n_unique );
  printf( "ACCURACY_SC     = %f percent\n", ( 100 * ( n_correct_satcounter / n_branches ) ) );
  printf( "ACCURACY_1L     = %f percent\n", ( 100 * ( n_correct1L / n_branches ) ) );
  printf( "ACCURACY_2L     = %f percent\n", ( 100 * ( n_correct2L / n_branches ) ) );

  //fprintf(trace, "#eof\n");
  fclose( trace );
  fclose( acc );
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
  PIN_ERROR( "This Pintool prints the IPs of branch instructions and whether they were taken\n" + KNOB_BASE::StringKnobSummary() + "\n" );
  return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main( int argc, char *argv[] )
{
  if ( PIN_Init( argc, argv ) )
    return Usage();
  trace = fopen( "trace-demo.txt", "w" );
  acc   = fopen( "accuracy-demo.out", "w" );
  printf( "Starting Pintool... \n" );
  INS_AddInstrumentFunction( Instruction, 0 );

  PIN_AddFiniFunction( Fini, 0 );
  // Never returns
  PIN_StartProgram();
  return 0;
}
