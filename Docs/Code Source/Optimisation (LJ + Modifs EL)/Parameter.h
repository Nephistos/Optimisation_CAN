#ifndef PARAMETER_H_INCLUDED
#define PARAMETER_H_INCLUDED

#include "includes.h"

#include "Calc.h"
// #include "Frame.h"


// Forward declaration
class Frame;


class Parameter {
private:
	string id_param;							// id (unique) of the parameter
	int size_;									// size in bits
	int nb_target;								// number of calculators the parameter must be delivered to
	int nb_frames_host;							// number of different frames the parameter is put into
	Calc* calc_source;							// pointer to the calculator that emits the parameter
	vector<Calc*> calc_target;					// pointers to the calculators (targets) that must recieve
												// the parameter
	vector<double> freq_request_target;			// frequency at which the targets must recieve the
												// parameter
	vector<Frame*> frame_host;					// pointers to the frames (host) the parameter is put into
	vector<int> size_request;					// size in bits of the fragments of this parameter in the
												// different frames (useful if the parameter cal be cut
												// into fragments)
	bool fixed_frame_host;						// is (are) the host frame(s) fixed

public:
	// constructor
	Parameter(string id_, int size2_, Calc* calc_source_, int nb_target_, bool fixed_=false);
	~Parameter();

	// get / set
	string getIdParam() const { return id_param; }
	void setIdParam(const string& id_) { id_param = id_; }
	int getSize() const {	return size_; }
	void setSize(const int size2_) { size_ = size2_; }
	int getNbTarget() const { return nb_target; }
	void setNbTarget(const int nb_target_) { nb_target = nb_target_; }
	int getNbFramesHost() const { return nb_frames_host; }
	void setNbFramesHost(const int nb_frames_host_) { nb_frames_host = nb_frames_host_; }
	Calc* getCalcSource() const { return calc_source; }
	void setCalcSource(Calc const * const calc_) { calc_source = const_cast<Calc*>(calc_); }
	vector<Calc*> getCalcTarget() const { return calc_target; }
	void setCalcTarget(const vector<Calc*>& calc_table_) { calc_target = calc_table_; }
	vector<double> getFreqTarget() const { return freq_request_target; }
	void setFreqTarget(const vector<double>& freq_request_) { freq_request_target = freq_request_; }
	vector<Frame*> getFrameHost() const { return frame_host; }
	// void setFrameHost(vector<Frame*> frame_host_) { frame_host = frame_host_; }
	void setFrameHost(const vector<Frame*>& frame_host_);
	vector<int> getSizeRequest() const { return size_request; }
	void setSizeRequest(const vector<int>& size_request_) { size_request = size_request_; }
	bool isHostFixed() const { return fixed_frame_host; }

	// methods
	void fixHost() { fixed_frame_host = true; }
	void addTarget(Calc* new_target, double new_freq_request);
	bool addFrameHost(Frame* new_host, bool force=false);
	bool remFrameHost(Frame* host);
	bool swapUniqueHost(Frame* newhost);
	bool swapHost(Frame const * const oldhost, Frame const * const newhost, bool force=false);
	string toString(bool toPrint=true) const;

};


/***********
 * smallerParamSize(Parameter const* p1, Parameter const* p2);
 *
 * Return if parameter p1 has a strictly smaller size than parameterp2. Equivalent to:
 * (p1.size < p2.size).
 *
 * INPUT	Parameter const* p1
 *				refers to the fisrt parameter to compare
 *
 *			Parameter const* p2
 *				refers to the second parameter to compare
 *
 * OUTPUT	boolean true if the first parameter has a strictly smaller size than the second one, false
 *			otherwise
 ***********/
bool smallerParamSize(Parameter const* p1, Parameter const* p2);


/***********
 * greaterParamSize(Parameter const* p1, Parameter const* p2);
 *
 * Return if parameter p1 has a strictly greater size than parameterp2. Equivalent to:
 * (p1.size > p2.size).
 *
 * INPUT	Parameter const* p1
 *				refers to the fisrt parameter to compare
 *
 *			Parameter const* p2
 *				refers to the second parameter to compare
 *
 * OUTPUT	boolean true if the first parameter has a strictly greater size than the second one, false
 *			otherwise
 ***********/
bool greaterParamSize(Parameter const* p1, Parameter const* p2);


/***********
 * numObj(vector<Template>* vecObj, string id)
 *
 * Return the position in vecObj of the object whose id is id. vecObj should contain unique id objects.
 *
 * INPUT	vector<Template>* vecObj: the vector of object of class Template in which looking for
 * 			string id: the id to be found
 * OUTPUT	A position (int)
 ***********/
int numObj(vector<Parameter> const * const vecObj, const string& id);
int numObj(const vector<Parameter*>& vecObj, const string& id);



#endif // PARAMETER_H_INCLUDED
