#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "includes.h"
#include <fstream>
#include <cstdlib>
#include <queue>
#include <cmath>
// #include <thread>		// for testing purposes, not used

#include "Calc.h"
#include "Element.h"
#include "Frame.h"
#include "Parameter.h"
#include "PathFrame.h"
#include "SubNet.h"
#include "Criteria.h"

#include "IOManagement.h"
#include "Architecture.h"
#include "Constraint.h"
#include "Solver.h"
#include "AddCalc.h"

#include "const_tool.h"

#include <windows.h>	// must be included the last



int main();


void optimTopologyWrapper(char* date);


void optimTopologyLauncher(vector<Frame>* frList, vector<Calc>* calcList, vector<SubNet>* snList,
						   vector< vector<int> > const& calcToCalcConnection,
						   vector< vector<int> > const& snToCalcConnection,
						   vector< vector<int> > const& constFrame,
						   queue< vector<Calc> > & calcBest, queue< vector<SubNet> > & subNetBest,
						   queue< vector<Frame> > & frameBest										);


void optimMessageSetWrapper(char* date);


void optimMessageSetLauncher(vector<Parameter>* parList, vector<Frame>* frList, vector<Calc>* calcList,
							 vector<SubNet>* snList,
							 vector< vector<int> > const& calcToCalcConnection,
							 vector< vector<int> > const& snToCalcConnection						);


#endif // MAIN_H_INCLUDED
