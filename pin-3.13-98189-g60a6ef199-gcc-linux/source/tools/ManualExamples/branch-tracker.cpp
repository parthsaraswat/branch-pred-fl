/*////////////////
Author:  Parth Saraswat
Program: Prints out the instruction pointer values of any Branch instruction
//////////////////*/
#include <stdio.h>
#include <iostream>
#include "pin.H"
#include <vector>
#include <map>
//#include "../../../../Pred3.h"
#include "../../../../app/src/Pred3.cc"
#include "../../../../app/src/Pred4.cc"

// USE STATS: 1 for when you want to use stats, 0 for when you want to track and predict all branches
int use_stats = 0;
int g_stats_en = !(use_stats);

// Pred objects for checking simulator accuracy
std::vector<Pred3> p1L;
std::vector<Pred4> p2L;
// Map for storing the dimensions of the 2L preds
std::vector<std::map<char, size_t> > dims;
Pred3 p0(0);
Pred3 p1(1);
Pred3 p5(5);
Pred3 p9(9);
Pred3 p11(11);
Pred3 p15(15);
Pred3 p20(20);
// 2-Level predictors for sweeping

Pred4 p2L0(0, 0, 0);
Pred4 p2L1(0, 0, 2);
Pred4 p2L2(0, 0, 5);
Pred4 p2L3(0, 0, 10);

Pred4 p2L4(0, 5, 0);
Pred4 p2L5(0, 5, 2);
Pred4 p2L6(0, 5, 5);
Pred4 p2L7(0, 5, 10);

Pred4 p2L8(0, 10, 0);
Pred4 p2L9(0, 10, 2);
Pred4 p2L10(0, 10, 5);
Pred4 p2L11(0, 10, 10);

Pred4 p2L12(0, 15, 0);
Pred4 p2L13(0, 15, 2);
Pred4 p2L14(0, 15, 5);
Pred4 p2L15(0, 15, 10);

Pred4 p2L16(1, 0, 0);
Pred4 p2L17(1, 0, 2);
Pred4 p2L18(1, 0, 5);
Pred4 p2L19(1, 0, 10);

Pred4 p2L20(1, 5, 0);
Pred4 p2L21(1, 5, 2);
Pred4 p2L22(1, 5, 5);
Pred4 p2L23(1, 5, 10);

Pred4 p2L24(1, 10, 0);
Pred4 p2L25(1, 10, 2);
Pred4 p2L26(1, 10, 5);
Pred4 p2L27(1, 10, 10);

Pred4 p2L28(1, 15, 0);
Pred4 p2L29(1, 15, 2);
Pred4 p2L30(1, 15, 5);
Pred4 p2L31(1, 15, 10);

Pred4 p2L32(5, 0, 0);
Pred4 p2L33(5, 0, 2);
Pred4 p2L34(5, 0, 5);
Pred4 p2L35(5, 0, 10);

Pred4 p2L36(5, 5, 0);
Pred4 p2L37(5, 5, 2);
Pred4 p2L38(5, 5, 5);
Pred4 p2L39(5, 5, 10);

Pred4 p2L40(5, 10, 0);
Pred4 p2L41(5, 10, 2);
Pred4 p2L42(5, 10, 5);
Pred4 p2L43(5, 10, 10);

Pred4 p2L44(5, 15, 0);
Pred4 p2L45(5, 15, 2);
Pred4 p2L46(5, 15, 5);
Pred4 p2L47(5, 15, 10);

// Counters and stats enable flags
float n_branches = 0;
float n_taken = 0;
float n_pred_taken = 0;
float n_pred_ntaken = 0;
float n_correct = 0;

// Variables for sweeping accuracy: n_correct & bools
float n_correct_1L[7];
bool pred1L[7];

float n_correct_2L[48];
bool pred2L[48];

// Sweep testing variables done
float accuracy;
bool prediction = false;

int nop_tag;
enum STATE
{
  ST,
  T,
  NT,
  SNT
};
STATE state = ST;

FILE *trace;
FILE *acc;
FILE *plots;

// Helper index generator for BHT indexing

/* ===================================================================== */
/* Accuracy helper                                                       */
/* ===================================================================== */

VOID AccuracyHelper(bool taken)
{
  if (g_stats_en)
  {
    // 1L check
    for (size_t i = 0; i < 7; i++)
    {
      if (pred1L[i] == taken)
      {
        n_correct_1L[i]++;
      }
    }
    // 2L check
    for (size_t i = 0; i < 48; i++)
    {
      if (pred2L[i] == taken)
      {
        n_correct_2L[i]++;
      }
    }
  }
}

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

VOID PredModel1_SatCounter(bool taken)
{
  if (g_stats_en)
  {
    std::cout << "Current state = " << state << std::endl;
    // State outputs:
    if (state == ST || state == T)
    {
      n_pred_taken++;
      prediction = TRUE;
    }
    else
    {
      n_pred_ntaken++;
      prediction = FALSE;
    }
    // State transitions
    switch (state)
    {
    case ST:
      state = (taken ? ST : T);
      break;
    case T:
      state = (taken ? ST : NT);
      break;
    case NT:
      state = (taken ? T : SNT);
      break;
    case SNT:
      state = (taken ? NT : SNT);
      break;
    default:
      state = ST;
      break;
    }
  }
}

/*---------------------------------------------------------------------- */
/* 2 bit saturating counter which jumps to STRONG states directly        */
/*---------------------------------------------------------------------- */

VOID PredModel2_SatCounter(bool taken)
{
  if (g_stats_en)
  {
    std::cout << "Current state = " << state << std::endl;
    // State outputs:
    if (state == ST || state == T)
    {
      n_pred_taken++;
      prediction = TRUE;
    }
    else
    {
      n_pred_ntaken++;
      prediction = FALSE;
    }
    // State transitions
    switch (state)
    {
    case ST:
      state = (taken ? ST : T);
      break;
    case T:
      state = (taken ? ST : SNT);
      break;
    case NT:
      state = (taken ? ST : SNT);
      break;
    case SNT:
      state = (taken ? NT : SNT);
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

VOID StatsEnable(VOID *ip)
{
  if (use_stats)
  {
    if (nop_tag == 1)
    {
      printf("\033[0;32m");
      printf("stats on\n");
      g_stats_en = 1;
    }
    else if (nop_tag == 2)
    {
      printf("\033[0;31m");
      printf("stats off\n");
      printf("\033[0m");
      g_stats_en = 0;
    }
  }
}

/* ========================================================================= */
/* Appends to branch trace and counts branches and whether they were taken   */
/* ========================================================================= */

VOID RecordBranch(VOID *ip, BOOL taken)
{
  if (g_stats_en)
  {
    n_branches++;
    //fprintf(trace,"Branch at %p\n", ip);
    if (taken)
    {
      //fprintf(trace, "was taken\n");
      n_taken++;
    }
    else
    {
      //fprintf(trace, "was not taken\n");
    }
  }
}
/* ========================================================================= */
/* Trace generator                                                           */
/* ========================================================================= */

VOID TraceGen(VOID *ip, BOOL taken)
{
  if (g_stats_en)
  {
    fprintf(trace, "IP: %p O: %d\n", ip, taken);
  }
}

/* ========================================================================= */
/* Wrapper for 1LBHT                                                         */
/* ========================================================================= */
VOID Pred3Wrapper(VOID *ip, BOOL taken)
{
  if (g_stats_en)
  {
    void *arg_ip = ip;
    bool arg_taken = taken;

    for (size_t i = 0; i < 7; i++)
    {
      pred1L[i] = p1L[i].predict(arg_ip);
      p1L[i].update(arg_ip, arg_taken);
    }
  }
}

/* ========================================================================= */
/* Wrapper for 2LBHT                                                         */
/* ========================================================================= */
VOID Pred4Wrapper(VOID *ip, BOOL taken)
{
  if (g_stats_en)
  {
    void *arg_ip = ip;
    bool arg_taken = taken;

    for (size_t i = 0; i < 48; i++)
    {
      pred2L[i] = p2L[i].predict(arg_ip);
      p2L[i].update(arg_ip, arg_taken);
    }
  }
}

// Is called for every instruction and instruments branches
VOID Instruction(INS ins, VOID *v)
{
  // Checking for turning stats on and off
  if ((INS_Opcode(ins) == XED_ICLASS_NOP) && (INS_OperandCount(ins) == 2) && (INS_OperandIsMemory(ins, 0)) && (INS_OperandMemoryBaseReg(ins, 0) == 10) && (INS_OperandIsReg(ins, 1)) && (INS_OperandReg(ins, 1) == 56))
  {
    nop_tag = INS_OperandMemoryDisplacement(ins, 0);
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)StatsEnable, IARG_INST_PTR, IARG_END);
  }
  //

  if (INS_IsBranch(ins))
  {
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RecordBranch,
                   IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END);

    /*   INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)TraceGen, 
                     IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END); */

    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Pred3Wrapper,
                   IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END);

    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Pred4Wrapper,
                   IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END);

    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)AccuracyHelper,
                   IARG_BRANCH_TAKEN, IARG_END);
  }
}

VOID Fini(INT32 code, VOID *v)
{
  fprintf(acc, "Total branches = %f\n", n_branches);
  fprintf(acc, "====================================\n");
  fprintf(acc, "           1-level sweep\n");
  fprintf(acc, "====================================\n");

  for (size_t i = 0; i < 7; i++)
  {
    fprintf(acc, "Accuracy 1L [%zu] = %f percent\n", i, (100 * (n_correct_1L[i]) / n_branches));
  }
  fprintf(acc, "====================================\n");
  fprintf(acc, "           2-level sweep\n");
  fprintf(acc, "====================================\n");
  for (size_t i = 0; i < 48; i++)
  {
    fprintf(acc, "2L(%zu, %zu, %zu): accuracy = %f p\n", (dims[i])['m'], (dims[i])['n'], (dims[i])['k'], (100 * (n_correct_2L[i]) / n_branches));
    fprintf(plots, "%f, %f\n", (100 * (n_correct_2L[i]) / n_branches), p2L[i].get_size());
  }
  fclose(trace);
  fclose(acc);
  fclose(plots);
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
  PIN_ERROR("This Pintool prints the IPs of branch instructions and whether they were taken\n" + KNOB_BASE::StringKnobSummary() + "\n");
  return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char *argv[])
{
  // Initialising accuracy helper values
  for (size_t i = 0; i < 7; i++)
  {
    n_correct_1L[i] = 0;
  }

  for (size_t i = 0; i < 48; i++)
  {
    n_correct_2L[i] = 0;
  }

  // Vector of 1Ls

  p1L.push_back(p0);
  p1L.push_back(p1);
  p1L.push_back(p5);
  p1L.push_back(p9);
  p1L.push_back(p11);
  p1L.push_back(p15);
  p1L.push_back(p20);

  // Vector of 2Ls

  p2L.push_back(p2L0);
  p2L.push_back(p2L1);
  p2L.push_back(p2L2);
  p2L.push_back(p2L3);
  p2L.push_back(p2L4);
  p2L.push_back(p2L5);
  p2L.push_back(p2L6);
  p2L.push_back(p2L7);
  p2L.push_back(p2L8);
  p2L.push_back(p2L9);
  p2L.push_back(p2L10);
  p2L.push_back(p2L11);
  p2L.push_back(p2L12);
  p2L.push_back(p2L13);
  p2L.push_back(p2L14);
  p2L.push_back(p2L15);
  p2L.push_back(p2L16);
  p2L.push_back(p2L17);
  p2L.push_back(p2L18);
  p2L.push_back(p2L19);
  p2L.push_back(p2L20);
  p2L.push_back(p2L21);
  p2L.push_back(p2L22);
  p2L.push_back(p2L23);
  p2L.push_back(p2L24);
  p2L.push_back(p2L25);
  p2L.push_back(p2L26);
  p2L.push_back(p2L27);
  p2L.push_back(p2L28);
  p2L.push_back(p2L29);
  p2L.push_back(p2L30);
  p2L.push_back(p2L31);
  p2L.push_back(p2L32);
  p2L.push_back(p2L33);
  p2L.push_back(p2L34);
  p2L.push_back(p2L35);
  p2L.push_back(p2L36);
  p2L.push_back(p2L37);
  p2L.push_back(p2L38);
  p2L.push_back(p2L39);
  p2L.push_back(p2L40);
  p2L.push_back(p2L41);
  p2L.push_back(p2L42);
  p2L.push_back(p2L43);
  p2L.push_back(p2L44);
  p2L.push_back(p2L45);
  p2L.push_back(p2L46);
  p2L.push_back(p2L47);

  // Gather dimensions of all our 2-level predictors
  for (size_t i = 0; i < 48; i++)
  {
    dims.push_back(p2L[i].get_dims());
  }
  if (PIN_Init(argc, argv))
    return Usage();
  trace = fopen("trace-branches.txt", "w");
  acc = fopen("accuracy-sim.out", "w");
  plots = fopen("1L_acc_v_storage.csv", "w");
  printf("Starting Pintool... \n");
  INS_AddInstrumentFunction(Instruction, 0);

  PIN_AddFiniFunction(Fini, 0);
  // Never returns
  PIN_StartProgram();

  return 0;
}
