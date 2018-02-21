#ifndef FRAME_H_INCLUDED
#define FRAME_H_INCLUDED

#include "includes.h"
#include "const_tool.h"

#include "Calc.h"
#include "Parameter.h"
#include "SubNet.h"

#include <cmath>
#include <list>


class Frame {
private:
	string id_frame;
	int id_num;

	int rest_path;

	double freq_transmission;				// refers to the frequency at which the frame must be emitted
	Calc* calc_source;						// refers to the source calculator
	int nb_target;
	int nb_fragment;
	vector<Calc*> calc_target;				// refers to the targets
	vector<Parameter*> param_fragment;		// refers to the parameters contained
	vector<int> size_fragment;				// sizes of the parameters contained

    double load_CAN;                         // charge induit si réseau CAN
	double load_CANFD;                       // charge induit si réseau CAN FD
	double latency;                         //temps de latence de la trame

	double frame_duration;              // refers to the duration of the frame on a CAN subnet
	double frame_durationFD;            // refers to the duration of the frame on a CAN-FD subnet

	vector<int> target_occ;					// number of occurence of the i-th target (ie. how many
											// parameters must be delivered to this calculator)
	vector<int> fragment_occ;				// number of occurence of the i-th parameter (ie. to how many
											// target it has to be delivered)
	list<double> freqRequest_fragment;		// Decreasingly ordered list of frequency requests
											// 1 element per pair (fragment, target of the fragment)
											// => (sum of target_occ elements) elements
	double load;		// in bps
	vector<SubNet*> snFilled;

	bool fixed_content;
	bool fixed_routing;

public:
	// constructor
	// Frame(string id_, double freq_, Calc* source_, int nb_target_, int nb_fragment_);
	// Frame(string id_, double freq_, Calc* source_, int nb_target_, int nb_fragment_,
		  // bool fixed_frame_);
	Frame(string id_, double freq_, Calc* source_, int nb_target_, int nb_fragment_,
		  bool fixed_content_=false, bool fixed_routing_=false);
	~Frame();

	// get set
	string getIdFrame() const { return id_frame; }
	void setIdFrame(string id_) { id_frame = id_; }

	int getIdNumFrame() const { return id_num; }
	void setIdNumFrame(int id_) { id_num = id_; }
	int getRestPath() const { return rest_path; }
	void setRestPath(int rest_) { rest_path = rest_; }

	double getFreqTransmission() const { return freq_transmission; }
	void setFreqTransmission(double freq_) { freq_transmission = freq_; }
	Calc* getCalcSource() const { return calc_source; }
	void setCalcSource(Calc* calc_) { calc_source = calc_; }
	int getNbTarget() const { return nb_target; }
	void setNbTarget(int nb_target_) { nb_target = nb_target_; }
	int getNbFragment() const { return nb_fragment; }
	void setNbFragment(int nb_fragment_) { nb_fragment = nb_fragment_; }
	vector<Calc*> const& getCalcTarget() const { return calc_target; }
	void setCalcTarget(vector<Calc*> calc_table_) { calc_target = calc_table_; }
	vector<Parameter*> const& getParamFragment() const { return param_fragment; }
	void setParamFragment(vector<Parameter*> paramList) { param_fragment = paramList; }
	vector<int> getSizeFragment() const { return size_fragment; }
	void setSizeFragment(vector<int> sizeList) { size_fragment = sizeList; }
	int getSize() const {
	    int resp = 0;
        for(int i = 0; i < (int)size_fragment.size(); i++) resp += size_fragment[i];
        return(resp);
	}
	double getLoadFD() const {return load_CANFD;}
	double getLoad() const { return load_CAN; }
	double getLatency() const {return latency;}
	double getDurationFD() const {return frame_durationFD;}
	double getDuration() const { return frame_duration; }
	vector<SubNet*> getSNFilled() const { return snFilled; }
	void setSNFilled(vector<SubNet*> snFilled_, bool force=false);
	bool isContentFixed() const { return fixed_content; }
	bool isRoutingFixed() const { return fixed_routing; }


	// methods
	void fixRouting() { fixed_routing = true; }


	/***********
	 * updateLoad()
	 *
	 * Update Frame::load and FRAM::duration_time_CAN -using the frequency and the size of the frame. The size depends if parameters
	 * are allowed to be placed astride two bytes; in any case it includes the bits of encapsulation.
	 * The load of the frame does NOT include the multiplicator factor that models bit-stuffing.
	 *
	 * MODIFY	Frame::load
	 ***********/
	void updateLoad();




	/***********
	 * arrangeParamPlacement(vector< vector<Parameter*> >& parameterArrangement)
	 *
	 * If placing parameters astride two bytes is forbidden, use a First Fit Decreasing (FFD) heuristics
	 * to arrange the parameters into bytes. The content of the frame is represented with a vector of
	 * bytes (parameterArrangement), from which the size of the datafield of the frame can be infered.
	 * The function returns if a valid arrangement could be found with this non-exact heuristic.
	 * Note that if any parameter is strictly larger than 8 bits, the function does not search any smart
	 * placement and returns TRUE.
	 * The present object is not modified by this method.
	 *
	 * MODIFY	vector< vector<Parameter*> >& parameterArrangement
	 *				represents the content of the frame, separated in bytes
	 *
	 * OUTPUT	bool that reports if a valid arrangement could be found using the FFD heuristics.
	 ***********/
	bool arrangeParamPlacement() const;
	bool arrangeParamPlacement(vector< vector<Parameter*> >& parameterArrangement) const;


	/***********
	 * addParameter(Parameter* newpar)
	 *
	 * Tries to fit the parameter passed into the frame. If it is possible, the characteristics of the
	 * parameter (reception frequency requests, target calculators) are added "smartly" to the frame. The
	 * load of the frame is then updated.
	 * Note that the parameter and the frame must share the same source calculator.
	 *
	 * MODIFY	Parameter* newpar
	 *				refers to the parameter to add to the frame. Its field Parameter::frame_host could be
	 *				updated.
	 *
	 * OUTPUT	bool that reports if the parameter could be added
	 ***********/
	bool addParameter(Parameter* newpar, bool force=false);


	/***********
	 * remParameter(Parameter* par)
	 *
	 * Tries to remove the parameter passed from the content of the frame. The load of the frame is then
	 * updated.
	 * Note that the parameter must be contained by the present frame at the beginning.
	 *
	 * MODIFY	Parameter* newpar
	 *				refers to the parameter to add to the frame. Its field Parameter::frame_host should be
	 *				updated.
	 *
	 * OUTPUT	bool that reports if the parameter and its characteristics could be removed
	 ***********/
	bool remParameter(Parameter* par);


	bool isFrameEmpty() { return (nb_fragment==0); }

	bool isFrameValid();	// TODO

	/***********
	 * spreadFrame(vector<SubNet>* snList, vector< vector<bool> > const& curNetworkMatrix)
	 * spreadFrame(vector<SubNet>* snList, vector< vector<int> > const& curNetworkMatrix)
	 *
	 * Given the curent network topology (represented as the bus to bus connection square matrix
	 * curNetworkMatrix) and the available subnetworks, route the frame so that it avoids in priority the
	 * most loaded subnetworks. If the routing is possible, the output routing (field Frame::snFilled)
	 * is then guaranteed to deliver the frame to all targets.
	 *
	 * Note:
	 * the int version was twice as faster as the bool version, due to the specific implementation of
	 * vector<bool>; the bool version has then been changed to a wrapper that calls tne int version
	 *
	 * INPUT	vector<SubNet> const* snList
	 *				refers to the list of available subnetworks
	 *
	 *			vector< vector<bool> > const& curNetworkMatrix
	 *			or
	 *			vector< vector<int> > const& curNetworkMatrix
	 *				subnetwork to subnetwork connection matrix: the (i,j)th element equals true (or 1) if
	 *				and only if the i-th  and the j-th subnetworks have a common calculator. The routing
	 *				can then pass the frame from one to the another subnetwork.
	 *
	 * MODIFY	the present object (eg. Frame::snFilled)
	 *
	 * OUTPUT	a bool qualifying if the routing of the frame is possible (if true, the frame is routed)
	 ***********/
	//bool spreadFrame(vector<SubNet> const* snList, vector< vector<bool> > const& curNetworkMatrix);
    bool spreadFrame(vector<SubNet> const* snList, vector<Frame> const* frameData, vector< vector<int> > const& curNetworkMatrix,
                        vector<vector<int> > const& constFrame);

	/***********
	 * toString(bool toPrint=true)
	 *
	 * Put in a string information about the present object and print it on the standard output if the
	 * argument toPrint is true.
	 *
	 * INPUT	bool toPrint
	 *				true to print the information on the standard output
	 *
	 * OUTPUT	a string that sums up information on the field of the present object
	 ***********/
	string toString(bool toPrint=true) const;
};

/***********
 * compareFrameLoads(Frame const* f1, Frame const* f2)
 *
 * Return if the frame f1 has a strictly lower load than frame f2. Equivalent to (f1.load < f2.load).
 *
 * INPUT	Frame const* f1
 *				refers to the fisrt frame to compare
 *
 *			Frame const* f2
 *				refers to the second frame to compare
 *
 * OUTPUT	boolean true if the first frame has a strictly lower load than the second one, false otherwise
 ***********/
bool compareFrameLoads(Frame const* f1, Frame const* f2);


/***********
 * smallerFrameFreq(Frame const* f1, Frame const* f2)
 *
 * Return if the frame f1 has a strictly lower frequency than frame f2. Equivalent to:
 * (f1.freq_transmission < f2.freq_transmission).
 *
 * INPUT	Frame const* f1
 *				refers to the fisrt frame to compare
 *
 *			Frame const* f2
 *				refers to the second frame to compare
 *
 * OUTPUT	boolean true if the first frame has a strictly lower frequency than the second one, false
 *			otherwise
 ***********/
bool smallerFrameFreq(Frame const* f1, Frame const* f2);


/***********
 * greaterFrameFreq(Frame const* f1, Frame const* f2)
 *
 * Return if the frame f1 has a strictly lower frequency than frame f2. Equivalent to:
 * (f1.freq_transmission > f2.freq_transmission).
 *
 * INPUT	Frame const* f1
 *				refers to the fisrt frame to compare
 *
 *			Frame const* f2
 *				refers to the second frame to compare
 *
 * OUTPUT	boolean true if the first frame has a strictly lower frequency than the second one, false
 *			otherwise
 ***********/
bool greaterFrameFreq(Frame const* f1, Frame const* f2);


/***********
 * numObj(vector<Template>* vecObj, string id)
 *
 * Return the position in vecObj of the object whose id is id. vecObj should contain unique id objects.
 *
 * INPUT	vector<Template>* vecObj: the vector of object of class Template in which looking for
 * 			string id: the id to be found
 * OUTPUT	A position (int)
 ***********/
int numObj(vector<Frame> const * const vecObj, const string& id);


/***********
 * matrix_power(vector< vector<int> > mat, int pow)
 *
 * Computes the pow-th power of boolean-like square matrix mat (that means mat should only contain 0 or 1).
 * The result is then returned.
 * Note that working with boolean-like matrices allows some optimization within the loops.
 * Worst case: still in O(n^3) with n the size of the square matrix mat.
 *
 * The bool version is much slower and should not be used.
 *
 * INPUT	vector< vector<int> > mat
 *				square boolean-like matrix
 *
 *			int pow
 *				power
 *
 * OUTPUT	an int (boolean-like) square matrix whose size is the same as mat's
 ***********/
 // vector< vector<bool> > matrix_power(vector< vector<bool> > mat, int pow);
vector< vector<int> > matrix_power(vector< vector<int> > mat, int pow);

#endif // FRAME_H_INCLUDED
