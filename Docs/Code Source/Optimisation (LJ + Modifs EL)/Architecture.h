#ifndef ARCHITECTURE_H_INCLUDED
#define ARCHITECTURE_H_INCLUDED

#include "includes.h"
#include "const_tool.h"

#include "Calc.h"
#include "Element.h"
#include "Frame.h"
#include "Parameter.h"
#include "PathFrame.h"
#include "SubNet.h"
#include "Criteria.h"

#include "Constraint.h"





/***********
 * initFrameFromBFDHeuristic(vector<Parameter>* par, vector<Calc> const * const calcList,
							 vector<Frame>* fr, vector<Frame> const * const frIni			)
 *
 * Initialize the message set using a Best Fit Decreasing (BFD) heuristic. It consists in taking one
 * parameter after another in the decreasing order of their highest reception frequency request and at
 * equal frequency in their decreasing size, and try:
 * 1)	to fit the parameter into the last frame created (which emission frequency is at least greater
 *		than the parameter's)
 * 2)	to create a new frame with only the parameter into
 * If both option are viable, the one which induce the lowest increase of the total load of the message
 * set is done.
 * At the end of the creation of frames, additionnal frames from each source calculator are created empty
 * (their load is thus 0. bps).
 *
 * INPUT	vector<Calc> const * const calcList
 *				refers to the list of calculators
 *
 * 			vector<Frame> const * const frIni
 *				refers to the list of frames of the initial message set
 *
 * MODIFY	vector<Parameter>* par
 *				refers to the list of parameters. Their Parameter::frame_host is modified through
 *				Frame::addParameter(Parameter*)
 *
 *  		vector<Frame>* fr
 *				refers to the list of frames that is built by this function
 *
 * OUTPUT	int: number of frames of the initial message set
 ***********/
int initFrameFromBFDHeuristic(vector<Parameter>* par, vector<Calc> const * const calcList,
							  vector<Frame>* fr, vector<Frame> const * const frIni			);


/***********
 * initPath(vector<SubNet>* subNetData, vector<Frame>* frameData, vector<PathFrame>* pathFrameData,
			bool verbose=false)
 *
 * Initialize a path for each pair (Frame,Target(Frame)) and return the number of paths NOT found. The
 * call to drawPath(...) will print the number of paths not resolved if verbose is true.
 *
 * INPUT	vector<SubNet>* subNetData
 *				refers to the list of subnetworks that may be filled by the frames.
 *
 *			bool verbose
 *				true to print the number of paths not found
 *
 * MODIFY	vector<Frame>* frameData
 *				refers to the list of frames. Their field Frame::snFilled is modified.
 *
 *  		vector<PathFrame>* pathFrameData
 *				refers to the list of initial frame paths. Modified during the call to drawPath(...)
 *
 * OUTPUT	number (int) of path that could not be resolved (in other words, number of frame requests
 *			(by a target calculator) that could not be satisfied)
 ***********/
int initPath(vector<SubNet>* subNetData, vector<Frame>* frameData, vector<PathFrame>* pathFrameData,
			 bool verbose=false);


/***********
 * drawPath(vector<Frame>* frameData, vector<PathFrame>* pfData, bool verbose=false)
 *
 * Initialize a path for each pair (Frame,Target(Frame)) and return the number of paths NOT found. Print
 * the number of paths not resolved if requested.
 *
 * INPUT	bool verbose
 *				true to print the number of paths not found
 *
 * MODIFY	vector<Frame>* frameData
 *				refers to the list of frames. Their field Frame::snFilled is modified.
 *
 *  		vector<PathFrame>* pathFrameData
 *				refers to the list of initial frame paths.
 *
 * OUTPUT	number (int) of path that could not be resolved (in other words, number of frame requests
 *			(by a target calculator) that could not be satisfied)
 ***********/
int drawPath(vector<Frame>* frameData, vector<PathFrame>* pfData, bool verbose=false);


/***********
 * spreadPathperFrame(vector<Frame>* frameData, vector<SubNet>* subNetData)
 *
 * For each frame (in the order of their decreasing loads), call the routing optimisation. Note that no
 * assertion of the feasibility of routing is done here.
 * The loads on the subnetwork are automatically updated with the routes changes.
 *
 * MODIFY	vector<Frame>* frameData
 *				refers to the list of frames
 *
 *			vector<SubNet>* subNetData
 *				refers to the list of subnetworks
 ***********/
bool spreadPathperFrame(vector<Frame> * const frameData, vector<SubNet> * const subNetData,
                         vector< vector<int> > const& constFrame, vector<Calc> * const calcData);


/***********
 * copyNetwork(vector<Calc> const * const calcData, vector<SubNet> const * const subNetData,
			   vector<Frame> const * const frameData,
			   vector<Calc>* calcCopy, vector<SubNet>* subNetCopy, vector<Frame>* frameCopy)
 *
 * Makes a copy of the network topology and of the frames. All the fields of the copy objects refer to
 * copy objects - except for the type Parameter (for which they refer to the original objects) - preventing
 * any interaction between original and copy objects.
 *
 * INPUT:	vector<Calc> const * const calcData
 *				refers to the list of calculators to be copied
 *
 * 			vector<SubNet> const * const subNetData
 *				refers to the list of subnetworks to be copied
 *
 *			vector<Frame> const * const frameData
 *				refers to the list of frames to be copied
 *
 * MODIFY	vector<Calc>* calcCopy
 *				refers to the copy of the list of calculators
 *
 *			vector<SubNet>* subNetCopy
 *				refers to the copy of the list of subnetworks
 *
 *			vector<Frame>* frameCopy
 *				refers to the copy of the list of frames
 ***********/
void copyNetwork(vector<Calc> const * const calcData, vector<SubNet> const * const subNetData,
				 vector<Frame> const * const frameData,
				 vector<Calc>* calcCopy, vector<SubNet>* subNetCopy, vector<Frame>* frameCopy	);


/***********
 * copyData(vector<Calc> const * const calcData, vector<Frame> const * const frameData,
			vector<Calc>* calcCopy, vector<Frame>* frameCopy							)
 *
 * Makes a copy of the data (calculators and frames). All the fields of the copy objects refer
 * to copy objects - except for the type Parameter (for which they refer to the original objects) -
 * preventing any interaction between original and copy objects.
 *
 * INPUT:	vector<Calc> const * const calcData
 *				refers to the list of calculators to be copied
 *
 *			vector<Frame> const * const frameData
 *				refers to the list of frames to be copied
 *
 * MODIFY	vector<Calc>* calcCopy
 *				refers to the copy of the list of calculators
 *
 *			vector<Frame>* frameCopy
 *				refers to the copy of the list of frames
 ***********/
void copyData(vector<Calc> const * const calcData, vector<Frame> const * const frameData,
			  vector<Calc>* calcCopy, vector<Frame>* frameCopy							);


/***********
 * copyNetworkFull(vector<Calc> const * const calcData, vector<SubNet> const * const subNetData,
				   vector<Frame> const * const frameData,	vector<Parameter> const * const paramData,
				   vector<Calc>* calcCopy, vector<SubNet>* subNetCopy, vector<Frame>* frameCopy,
				   vector<Parameter>* paramCopy													)
 *
 * Makes a copy of the network topology, of the frames and of the parameters. All the fields of the copy
 * objects refer to copy objects, preventing any interaction between original and copy objects.
 *
 * INPUT:	vector<Calc> const * const calcData
 *				refers to the list of calculators to be copied
 *
 * 			vector<SubNet> const * const subNetData
 *				refers to the list of subnetworks to be copied
 *
 *			vector<Frame> const * const frameData
 *				refers to the list of frames to be copied
 *
 *			vector<Parameter> const * const paramData
 *				refers to the list of parameters to be copied
 *
 * MODIFY	vector<Calc>* calcCopy
 *				refers to the copy of the list of calculators
 *
 *			vector<SubNet>* subNetCopy
 *				refers to the copy of the list of subnetworks
 *
 *			vector<Frame>* frameCopy
 *				refers to the copy of the list of frames
 *
 *			vector<Parameter>* paramCopy
 *				refers to the copy of the list of parameters
 ***********/
void copyNetworkFull(vector<Calc> const * const calcData, vector<SubNet> const * const subNetData,
					 vector<Frame> const * const frameData,	vector<Parameter> const * const paramData,
					 vector<Calc>* calcCopy, vector<SubNet>* subNetCopy, vector<Frame>* frameCopy,
					 vector<Parameter>* paramCopy													);


/***********
 * adaptParamToCopiedNetwork(vector<Parameter> * const par, vector<Calc> const * const calcCopy,
							 vector<Frame> const * const frameCopy								)
 *
 * Links a list of parameters with the list of calculators and the list of frames that come from a copy.
 * Only the Parameters objects are modified. Changes in the calculators and frames objects must be done
 * separately.
 *
 * INPUT:	vector<Calc> const * const calcCopy
 *				refers to the list of calculators to be pointed to
 *
 *			vector<Frame> const * const frameCopy
 *				refers to the list of frames to be pointed to
 *
 * MODIFY	vector<Parameter> * const par
 *				refers to the list of parameters to be modified
 ***********/
void adaptParamToCopiedNetwork(vector<Parameter> * const par, vector<Calc> const * const calcCopy,
							   vector<Frame> const * const frameCopy								);


/***********
 * constructValidNetwork(vector<Calc>* calcData, vector<Frame>* frameData,
						 vector<Calc>* calcGen, vector<SubNet>* subNetGen, vector<Frame>* frameGen,
						 vector<PathFrame>* pathFrameGen,
						 vector< vector<int> > const& calcToCalcConnection,
						 vector< vector<int> > const& snToCalcConnection							)
 *
 * Try to build a topology that satisfies all the constraints of the topology (and NOT necessarily
 * routing constraints).
 * The data (calcData and frameData) are copied and a set of calculator-to-subnetwork connections are
 * chosen randomly, using the maximum number of connectors available on each calculator.
 *
 *
 ***********/
bool constructValidNetwork(vector<Calc>* calcData, vector<Frame>* frameData,
						   vector<Calc>* calcGen, vector<SubNet>* subNetGen, vector<Frame>* frameGen,
						   vector<PathFrame>* pathFrameGen,
						   vector< vector<int> > const& calcToCalcConnection,
						   vector< vector<int> > const& snToCalcConnection							);


/***********
 * computeNetworkConMatrix(vector<SubNet>* subNetData, vector< vector<int> >* snGraph)
 *
 * Compute the matrix of common calculators between buses and store it into a vector of vector of int
 * (and not into a vector< vector<bool> > for computation time reasons when using that object).
 *
 * INPUT	vector<SubNet> * const subNetData
 *				refers to the list of subnetwork (and thus to the topology)
 *
 * MODIFY	vector< vector<int> >* snGraph
 *				matrix of size (number of subnetworks) x (number of subnetworks) whose components equal 1
 *				if the i-th nd the j-th subnetworks have a common calculator and 0 otherwise
 ***********/
void computeNetworkConMatrix(vector<SubNet> * const subNetData, vector< vector<int> >* snGraph,
                              vector< vector<int> >* SNCalcConnection, vector<Calc> * const calcData);


/**********
 * clearNetwork(vector<SubNet>* snList)
 *
 * Delete the subnetworks containing 0 or 1 calculator AND are at the end of the subnetwork list.
 *
 * MODIFY	vector<SubNet>* snList: refers to the list of subnetworks. Note that the deconnection of a
 *			calculator from a subnetwork implies the modification of this Calc's attributes.
 **********/
void clearNetwork(vector<SubNet>* snList);


/***********
 * updateLoads(vector<SubNet>* subNetData, vector<Frame>* frameData, vector<PathFrame>* pathFrameData)
 *
 * Compute how many times a given frame (ie. how many paths from the same frame) takes a subnetwork,
 * update which subnetworks are taken by each frame and update the loads of all the subnetwork.
 *
 * INPUT	vector<PathFrame>* pathFrameData
 *				refers to the list of frame paths
 *
 * MODIFY	vector<SubNet>* subNetData
 *				refers to the list of subnetworks ; their load is updated
 *
 * 			vector<Frame>* frameData
 *				refers to the list of frame ; their snFilled is updated
 *
 * OUTPUT	A vector< vector<int> > synthetizing how many times a given frametakes a subnetwork.
 ***********/
vector< vector<int> > updateLoads(vector<SubNet>* subNetData, vector<Frame>* frameData,
								  vector<PathFrame>* pathFrameData						);


/***********
 * updateLoadsFromFrame(vector<SubNet>* subNetData, vector<Frame> const * const frameData)
 *
 * First set the loads on the subnetworks to 0. Then update the loads using the field Frame::snFilled and
 * the load of each frame multiplied by the adequate factor.
 *
 * INPUT	vector<Frame> const * const frameData
 *				refers to the list of frame
 *
 * MODIFY	vector<SubNet>* subNetData
 *				refers to the list of subnetworks ; their load is updated
 ***********/
void updateLoadsFromFrame(vector<SubNet>* subNetData, vector<Frame> const * const frameData);


/***********
 * evaluateCost(vector<Calc>* calcData, vector<SubNet>* subNetData)
 *
 * Given the unit cost of elements, evaluate the cost of the topology provided in argument.
 *
 * INPUT:	vector<Calc>* calcData
 *				refers to the list of calculators to be copied
 *
 * 			vector<SubNet>* subNetData
 *				refers to the list of subnetworks to be copied
 *
 * OUTPUT	double: the cost of the topology
 ***********/
double evaluateCost(vector<Calc>* calcData, vector<SubNet>* subNetData);

double evaluateCost(int nbSubNet, int nbConnection);


/**********
 * evaluateNbSN(vector<SubNet>* subNetData)
 *
 * Computes the number of subnetworks.
 *
 * INPUT	vector<SubNet>* subNetData: refers to the list of subnetworks
 * OUTPUT	The number(int) of subnetworks.
 **********/
int evaluateNbSN(vector<SubNet>* subNetData);


/**********
 * evaluateNbUsedConnector(vector<Calc>* calcData)
 *
 * Computes the number of connectors used by each calc (ie how many subnetworks it is connected to).
 *
 * INPUT	vector<Calc>* calcData: refers to the list of calculators
 * OUTPUT	A vector<int>: the list of number of ports of each calculator (in the same order as the input).
 **********/
vector<int> evaluateNbUsedConnector(vector<Calc>* calcData);


/**********
 * evaluateLoadPerSN(vector<SubNet>* subNetData)
 *
 * Returns the list of loads per subnetwork.
 *
 * INPUT	vector<SubNet>* subNetData: refers to the list of subnetworks
 * OUTPUT	A vector<double>: the list of loads per subnetwork.
 **********/
vector<double> evaluateLoadPerSN(vector<SubNet>* subNetData);

/**********
 * evaluateLatency(vector<Frame>* frameData)
 *
 * Returns the latency of the network
 *
 * INPUT	vector<Frame>* frameData: refers to the list of frame
 * OUTPUT	A double: the sum of the frame latency
 **********/
double evaluateLatency(vector<Frame>* frameData);


/**********
 * evaluateArchi(vector<Frame> * const frList, vector<Calc> * const calcList,
					vector<SubNet> * const snList,
					vector< vector<int> > const& calcToCalcConnection,
					vector< vector<int> > const& snToCalcConnection,
					bool* isArchiValid											)
 *
 * Gives a complete evaluation of a topology (validity and criteria) and route it.
 *
 * INPUT	vector<Frame> * const frList
 *				refers to the list of frames
 *
 *			vector<Calc> * const calcList
 *				refers to the list of calculators
 *
 *			vector<SubNet> * const snList
 *				refers to the list of subnetworks
 *
 *			vector< vector<int> > const& calcToCalcConnection
 *				matrix for user defined topology constraints
 *
 *			vector< vector<int> > const& snToCalcConnection
 *				matrix for user defined topology constraints
 *
 *			bool* isArchiValid
 *				qualify if the topology + routing is valid
 *
 * OUTPUT	Criteria that evaluates the topology + routing. If at least one constraint has been violated,
 *			the evaluation returned will be much higher than any valid architecture could be.
 **********/
Criteria evaluateArchi(vector<Frame> * const frList, vector<Calc> * const calcList,
					   vector<SubNet> * const snList,
					   vector< vector<int> > const& calcToCalcConnection,
					   vector< vector<int> > const& snToCalcConnection,
					   vector< vector<int> > const& constFrame,
					   bool* isArchiValid											);

bool checkRoutingConstraint(vector<Frame> * const frameData, vector<SubNet> * const subNetData,
                            vector< vector<int> > const& constFrame, vector<Calc> * const calcData, bool verbose);

#endif // ARCHITECTURE_H_INCLUDED
