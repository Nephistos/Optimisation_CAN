#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED


#include "includes.h"
#include "const_tool.h"

#include "Calc.h"
#include "Element.h"
#include "Frame.h"
#include "Parameter.h"
#include "PathFrame.h"
#include "SubNet.h"
#include "Criteria.h"

#include "Architecture.h"



/**********
 * optimTabuSto(vector<Calc>* calcData, vector<SubNet>* subNetData, vector<Frame>* frameData,
				vector<PathFrame>* pathFrameData												)
 *
 * Optimize a topology using a stochastic tabu search (only part of the current neighbourhood is visited
 * at each iteration; see the const.h file for the parameter defining the size of the neighbourhood).
 *
 * MODIFY	vector<Calc>* calcData
 *				refers to the list of calculators
 *
 * 			vector<SubNet>* subNetData
 *				refers to the list of subnetworks
 *
 *			vector<Frame>* frameData
 *				refers to the list of frames
 *
 *			vector<PathFrame>* pathFrameData
 *				refers to the list of path (routes) of the frames
 **********/
void optimTabuSto(vector<Calc>* calcData, vector<SubNet>* subNetData, vector<Frame>* frameData,
				vector<PathFrame>* pathFrameData												);


/**********
 * optimTabu(vector<Calc>* calcList, vector<SubNet>* snList, vector<Frame>* frList,
			 vector< vector<int> > const& calcToCalcConnection,
			 vector< vector<int> > const& snToCalcConnection							)
 *
 * Optimize a topology using a deterministic tabu search (the current neighbourhood is visited at each
 * iteration).
 *
 * INPUT	vector< vector<int> > const& calcToCalcConnection
 *				matrix for user defined topology constraints
 *
 *			vector< vector<int> > const& snToCalcConnection
 *				matrix for user defined topology constraints
 *
 * MODIFY	vector<Calc>* calcList
 *				refers to the list of calculators
 *
 * 			vector<SubNet>* snList
 *				refers to the list of subnetworks
 *
 *			vector<Frame>* frList
 *				refers to the list of frames
 **********/
void optimTabu(vector<Calc>* calcList, vector<SubNet>* snList, vector<Frame>* frList,
			   vector< vector<int> > const& calcToCalcConnection,
			   vector< vector<int> > const& snToCalcConnection,
			   vector< vector<int> > const& constFrame,
			   queue< vector<Calc> > & calcBest, queue< vector<SubNet> > & subNetBest,
			   queue< vector<Frame> > & frameBest										);


/**********
 * updateBestTopology(queue< vector<Frame> >& frBest, queue< vector<Calc> >& calcBest,
					  queue< vector<SubNet> >& snBest,
					  vector<Frame> const * const frList, vector<Calc> const * const calcList,
					  vector<SubNet> const * const snList										)
 *
 * Store a new best topology into the queue of the best topologies. Then, if the size of the queue exceeds
 * the required number of topologies to store, the worst stored topology is removed.
 * Note that due to the call to this function, not the n best topologies visited are stored but the n last
 * "current best topologies".
 *
 * INPUT	vector<Frame> const * const frList
 *				list of frames of the new best topology. Note that this topology is not evaluated here and
 *				is not asserted to be better that the current topologies stored.
 *
 *			vector<Calc> const * const calcList
 *				list of calculators of the new best topology. Note that this topology is not evaluated
 *				here and is not asserted to be better that the current topologies stored.
 *
 *			vector<SubNet> const * const snList
 *				list of subnetworks of the new best topology. Note that this topology is not evaluated
 *				here and is not asserted to be better that the current topologies stored.
 *
 * MODIFY	queue< vector<Frame> >& frBest
 *				refers to the stored lists of frames of the best topologies
 *
 * 			queue< vector<Calc> >& calcBest
 *				refers to the stored lists of calculators of the best topologies
 *
 * 			queue< vector<SubNet> >& snBest
 *				refers to the stored lists of subnetworks of the best topologies
 **********/
void updateBestTopology(queue< vector<Frame> >& frBest, queue< vector<Calc> >& calcBest,
						queue< vector<SubNet> >& snBest,
						vector<Frame> const * const frList, vector<Calc> const * const calcList,
						vector<SubNet> const * const snList										);


/**********
 * updateBestArchi(queue< vector<Frame> >& frBest, queue< vector<Calc> >& calcBest,
				   queue< vector<SubNet> >& snBest, queue< vector<Parameter> >& parBest,
				   vector<Frame> const * const frList, vector<Calc> const * const calcList,
				   vector<SubNet> const * const snList, vector<Parameter> const * const parList)
 *
 * Store a new best topology into the queue of the best architectures. Then, if the size of the queue
 * exceeds the required number of architectures to store, the worst stored topology is removed.
 * Note that due to the call to this function, not the n best architectures visited are stored but the n
 * last "current best architectures".
 *
 * INPUT	vector<Frame> const * const frList
 *				list of frames of the new best architecture. Note that this architecture is not evaluated
 *				here and is not asserted to be better that the current architectures stored.
 *
 *			vector<Calc> const * const calcList
 *				list of calculators of the new best architecture. Note that this architecture is not
 *				evaluated here and is not asserted to be better that the current architectures stored.
 *
 *			vector<SubNet> const * const snList
 *				list of subnetworks of the new best architecture. Note that this architecture is not
 *				evaluated here and is not asserted to be better that the current architectures stored.
 *
 *			vector<Parameter> const * const parList
 *				list of parameters of the new best architecture. Note that this architecture is not
 *				evaluated here and is not asserted to be better that the current architectures stored.
 *
 * MODIFY	queue< vector<Frame> >& frBest
 *				refers to the stored lists of frames of the best architectures
 *
 * 			queue< vector<Calc> >& calcBest
 *				refers to the stored lists of calculators of the best architectures
 *
 * 			queue< vector<SubNet> >& snBest
 *				refers to the stored lists of subnetworks of the best architectures
 *
 * 			queue< vector<Parameter> >& parBest
 *				refers to the stored lists of parameters of the best architectures
 **********/
void updateBestArchi(queue< vector<Frame> >& frBest, queue< vector<Calc> >& calcBest,
					 queue< vector<SubNet> >& snBest, queue< vector<Parameter> >& parBest,
					 vector<Frame> const * const frList, vector<Calc> const * const calcList,
					 vector<SubNet> const * const snList, vector<Parameter> const * const parList);


/**********
 * metaTabu(vector<Parameter>* vpar, vector<Calc>* calcList, vector<SubNet>* snList,
			vector<Frame>* frList, vector<PathFrame>* pathFrameData,
			vector< vector<int> > const& calcToCalcConnection,
			vector< vector<int> > const& snToCalcConnection,
			int nbItMax, int nbItHeating, double temp_max								)
 *
 * Optimize an architecture using a deterministic tabu search (the current neighbourhood is visited at
 * each iteration) as a metaheuristic. Each visited architecture consists of a fixed topology; the message
 * set is optimised for this topology using the optimLoadsSimulatedAnnealing(...) function.
 *
 * INPUT	vector< vector<int> > const& calcToCalcConnection
 *				matrix for user defined topology constraints
 *
 *			vector< vector<int> > const& snToCalcConnection
 *				matrix for user defined topology constraints
 *
 *			int nbItMax
 *				number of iterations of the message set optimisation
 *
 *			int nbItHeating
 *				*TO BE REMOVED*
 *
 *			double temp_max
 *				initial temperature of the message set optimisation
 *
 * MODIFY	vector<Parameter>* vpar
 *				refers to the list of parameters
 *
 *			vector<Calc>* calcList
 *				refers to the list of calculators
 *
 * 			vector<SubNet>* snList
 *				refers to the list of subnetworks
 *
 *			vector<Frame>* frList
 *				refers to the list of frames
 *
 *			vector<PathFrame>* pathFrameData
 *				refers to the list of path (routes) of the frames
 **********/
void metaTabu(vector<Parameter>* vpar, vector<Calc>* calcList, vector<SubNet>* snList,
			  vector<Frame>* frList, vector<PathFrame>* pathFrameData,
			  vector< vector<int> > const& calcToCalcConnection,
			  vector< vector<int> > const& snToCalcConnection,
			  vector< vector<int> > const& constFrame,
			  int nbItMax, int nbItHeating, double temp_max								);


/**********
 *	optimLoadsSimulatedAnnealing2(vector<Parameter>* vpar, vector<Calc>* calcList,
								  vector<SubNet>* subNetCur, vector<Frame>* frameCur,
								  vector<PathFrame>* pathFrameCur,
								  vector< vector<int> > const& calcToCalcConnection,
								  vector< vector<int> > const& snToCalcConnection,
								  int nbItMax, double Tmax								)
 *
 * Optimise the message set from an initial solution using a simulated annealing algorithm. Parameters of
 * the simulated annealing are given in parameters.
 *
 * INPUT	vector< vector<int> > const& calcToCalcConnection
 *				matrix for user defined topology constraints
 *
 *			vector< vector<int> > const& snToCalcConnection
 *				matrix for user defined topology constraints
 *
 *			int nbItMax
 *				number of iterations of the message set optimisation
 *
 *			double Tmax
 *				initial temperature of the message set optimisation
 *
 * MODIFY	vector<Parameter>* vpar
 *				refers to the list of parameters
 *
 *			vector<Calc>* calcList
 *				refers to the list of calculators
 *
 * 			vector<SubNet>* subNetCur
 *				refers to the list of subnetworks
 *
 *			vector<Frame>* frameCur
 *				refers to the list of frames
 *
 *			vector<PathFrame>* pathFrameCur
 *				refers to the list of path (routes) of the frames
 **********/
void optimLoadsSimulatedAnnealing2(vector<Parameter>* vpar, vector<Calc>* calcList,
								  vector<SubNet>* subNetCur, vector<Frame>* frameCur,
								  vector<PathFrame>* pathFrameCur,
								  vector< vector<int> > const& calcToCalcConnection,
								  vector< vector<int> > const& snToCalcConnection,
								  vector< vector<int> > const& constFrame,
								  int nbItMax, double Tmax);


#endif // SOLVER_H_INCLUDED
