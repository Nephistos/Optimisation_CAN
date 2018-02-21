#ifndef CONSTRAINT_H_INCLUDED
#define CONSTRAINT_H_INCLUDED

#include "includes.h"
#include "const_tool.h"

#include "Calc.h"
#include "Element.h"
#include "Frame.h"
#include "Parameter.h"
#include "PathFrame.h"
#include "SubNet.h"
#include "Criteria.h"



/***********
 * checkTopologyValidity(vector<Calc> const * const calcList, vector<SubNet> const * const snList,
						   vector< vector<int> > const& calcToCalcConnection,
						   vector< vector<int> > const& snToCalcConnection,
						   bool verbose=false														)
 *
 * Check the validity of the topology against the CAN bus constraint (eg. max. 16 calculators per bus),
 * the number of connectors available on each calculator or bridge, the use of bridges and call for the
 * check of constraints about fixed parts of the topology.
 * Constraint about routing (eg. delivering all frames/parameters) are NOT evaluated here.
 *
 * INPUT	vector<Calc> const * const calcList
 *  			refers to the list of calculators
 *
 *			vector<SubNet> const * const snList
 *  			refers to the list of subnetworks
 *
 *			vector< vector<int> > const& calcToCalcConnection
 *				input for user defined topology constraints; see checkAdditionnalTopologyConstraint(...)
 *
 *			vector< vector<int> > const& snToCalcConnection
 *				input for user defined topology constraints; see checkAdditionnalTopologyConstraint(...)
 *
 *			bool verbose
 *				true to print the result of the check; false by default
 *
 * OUTPUT	A bool
 *				true if all the constraints evaluated are satisfied, false otherwise.
 ***********/
bool checkTopologyValidity(vector<Calc> const * const calcList, vector<SubNet> const * const snList,
						   vector< vector<int> > const& calcToCalcConnection,
						   vector< vector<int> > const& snToCalcConnection,
						   bool verbose=false														);


/***********
 * checkAdditionnalTopologyConstraint(vector<Calc> const * const calcList,
										vector<SubNet> const * const snList,
										vector< vector<int> > const& calcToCalcConnection,
										vector< vector<int> > const& snToCalcConnection	)
 *
 * Check the validity of the topology against the user defined constrained regarding fixing parts of the
 * topology: pairs of calculators that must/must not have a common network and subnetworks entirely/
 * partly fixed.
 * This function is intended to be called by checkTopologyValidity(...).
 *
 * INPUT	vector<Calc> const * const calcList
 *  			refers to the list of calculators
 *
 *			vector<SubNet> const * const snList
 *  			refers to the list of subnetworks
 *
 *			vector< vector<int> > const& calcToCalcConnection
 *  			matrix that summarizes if calculators of indices i and j must have at least one common
 *				network (value +1) or must not have any	(value -1). All others values are not considered
 *				(ie. not associated with a constraint).
 *
 *			vector< vector<int> > const& snToCalcConnection
 * 				matrix (i,j) that summarizes if calculator of index j must be connected to subnetwork of
 *				index i (value +1) or must not (value -1). All others values are not considered (ie. not
 *				associated with a constraint).
 *
 * OUTPUT	A bool
 *				true if all the constraints evaluated are satisfied, false otherwise.
 ***********/
bool checkAdditionnalTopologyConstraint(vector<Calc> const * const calcList,
										vector<SubNet> const * const snList,
										vector< vector<int> > const& calcToCalcConnection,
										vector< vector<int> > const& snToCalcConnection	);


/***********
 * checkRoutingValidity(vector<Calc> const * const calcList, vector<PathFrame> const * const pfList,
						  bool verbose)
 *
 * Check the validity of the routing against the delivery of all the parameters contained in the field
 * Calc::param_recieved at an adequate frequency (ie. greater than or equal to the value specified in
 * Calc::freq_request).
 * Moreover, the parameters are to be within a frame whose
 *
 *
 * INPUT	vector<Calc> const * const calcList
 *  			refers to the list of calculators
 *
 *			vector<SubNet> const * const snList
 *  			refers to the list of subnetworks
 *
 *			vector< vector<int> > const& calcToCalcConnection
 *  			matrix that summarize if calculators of indices i and j must have at least one common
 *				network (value +1) or must not have any	(value -1). All others values are not considered
 *				(ie. not associated with a constraint).
 *
 *			vector< vector<int> > const& snToCalcConnection
 * 				matrix (i,j) that summarize if calculator of index j must be connected to subnetwork of
 *				index i (value +1) or must not (value -1). All others values are not considered (ie. not
 *				associated with a constraint).
 *
 * OUTPUT	A bool
 *				true if all the constraints evaluated are satisfied, false otherwise.
 ***********/
bool checkRoutingValidity(vector<Calc> const * const calcList, vector<PathFrame> const * const pfList,
						  bool verbose=false															);


#endif // CONSTRAINT_H_INCLUDED
