#include "Parameter.h"



// constructor
Parameter::Parameter(string id_, int size2_, Calc* calc_source_, int nb_target_, bool fixed_):
	id_param(id_),
	size_(size2_),
	nb_target(nb_target_),
	nb_frames_host(0)
{
	calc_source = calc_source_;	// does it work ?
	calc_target = vector<Calc*>();
	freq_request_target = vector<double>();
	frame_host = vector<Frame*>();
	size_request = vector<int>();
	fixed_frame_host = fixed_;
}


Parameter::~Parameter() {
	// TODO : erase calc_target, trame_host, size_request
}


void Parameter::setFrameHost(const vector<Frame*>& frame_host_) {
	frame_host = frame_host_;
	nb_frames_host = frame_host.size();
}


void Parameter::addTarget(Calc* new_target, double new_freq_request) {
	if (numObj(calc_target, new_target->getIdElem()) < 0) {
		calc_target.push_back(new_target);
		freq_request_target.push_back(new_freq_request);
		nb_target++;
	}
}


bool Parameter::addFrameHost(Frame* new_host, bool force) {
	if (fixed_frame_host && !force) return false;
	else {
		frame_host.push_back(new_host);
		nb_frames_host++;
	}
	return true;
}


bool Parameter::remFrameHost(Frame* host) {
	if (fixed_frame_host) return false;
	vector<Frame*>::iterator ithost=find(frame_host.begin(), frame_host.end(), host);
	if (ithost!=frame_host.end()) {
		frame_host.erase(ithost);
		nb_frames_host--;
	} else {
		cerr << "Inconsistent host of Parameter " << id_param << "." << endl;
		return false;
	}
	return true;
}


bool Parameter::swapUniqueHost(Frame* newhost) {	// TO DELETE or use Frame::addParam and Frame::remParam
	if (fixed_frame_host) return false;

	if (nb_frames_host==1) {
		frame_host[0] = newhost;
	} else {
		cerr << "Warning: swapHost should be used instead of swapUniqueHost for Parameter "
			 << id_param << ". No change of frame host has been done yet." << endl;
		return false;
	}
	return true;
}


bool Parameter::swapHost(Frame const * const oldhost, Frame const * const newhost, bool force) { // TO DELETE or use Frame::addParam and Frame::remParam
	if (fixed_frame_host && !force) return false;

	vector<Frame*>::iterator itoldhost=find(frame_host.begin(), frame_host.end(), oldhost);
	if (itoldhost!=frame_host.end()) {
		*itoldhost = const_cast<Frame*>(newhost);
	} else {
		cerr << "Inconsistent host of Parameter " << id_param << "." << endl;
		return false;
	}
	return true;
}


string Parameter::toString(bool toPrint) const {
ostringstream str;
	str << "~~~~~~~~~~ Parameter " << id_param << " ~~~~~~~~~~" << endl;
	str << "Size (bits) : " << size_ << endl;
	str << "Source : " << calc_source->getIdElem() << endl;
	str << "Nb calc target : " << nb_target << " (" << calc_target.size() << " stored)" << endl;
	for (int jj=0;jj<(int)calc_target.size();++jj) {
		str << calc_target[jj]->getIdElem() << "(" << freq_request_target[jj] << "Hz)" << "\t";
	}
	str << endl;
	str << "Nb frame hosts : " << nb_frames_host << " (" << frame_host.size() << " stored)" << endl;
	str << "Adresses:\t";
	for (int jj=0;jj<(int)frame_host.size();++jj) {
		// str << frame_host[jj]->getIdFrame() << " (" << size_request[jj] << " ms)" << "\t";
		str << frame_host[jj] << "\t";
	}
	str << endl;
	str << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

	if (toPrint){
		cout << str.str();
	}

	return str.str();
}


bool smallerParamSize(Parameter const* p1, Parameter const* p2) {
	return (p1->getSize() < p2->getSize());
}


bool greaterParamSize(Parameter const* p1, Parameter const* p2) {
	return smallerParamSize(p2, p1);
}


int numObj(vector<Parameter> const * const vecObj, const string& id) {
	int num = -1;
	for (int ii=0;ii<(int)(*vecObj).size();++ii) {
		if ((*vecObj)[ii].getIdParam().compare(id) == 0) num = ii;
	}
	if (num < 0) cerr << "Object " << id << " was not found in the stored objects." << endl;
	return num;
}


int numObj(const vector<Parameter*>& vecObj, const string& id) {
	int num = -1;
	for (int ii=0;ii<(int)vecObj.size();++ii) {
		if (vecObj[ii]->getIdParam().compare(id) == 0) num = ii;
	}
	// if (num < 0) cerr << "Object " << id << " was not found in the stored objects." << endl;
	return num;
}

