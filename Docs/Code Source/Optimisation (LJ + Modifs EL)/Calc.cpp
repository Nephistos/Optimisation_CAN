#include "Calc.h"


using namespace std;

int Calc::counter = 0;

// constructors

Calc::Calc(const string& id_, const bool isBridge_, const int nb_max_con_, const int nb_max_con_FD_):
	Element(counter, id_, false),
	isBridge(isBridge_),
	nb_param_transmitted(0),
	nb_param_recieved(0),
	nb_max_connector(nb_max_con_),
	nb_max_connector_FD(nb_max_con_FD_),
	nb_SN_connection(0),
	nb_SN_connection_FD(0)
{
	param_recieved = vector<Parameter*>();
	freq_request = vector<double>();
	connected_SN = vector<SubNet*>();
	++counter;
	//cout << endl << counter << endl;
}

Calc::Calc(const string& id_, const bool isBridge_, const int nb_param_trans_, const int nb_param_rec_,
		   const int nb_max_con_,int nb_max_con_FD, const int nb_max_trames_):
	Element(counter, id_, false),
	isBridge(isBridge_),
	nb_param_transmitted(nb_param_trans_),
	nb_param_recieved(nb_param_rec_),
	nb_max_connector(nb_max_con_),
	nb_max_connector_FD(nb_max_con_FD),
	nb_max_trames_bridge(nb_max_trames_),
	nb_SN_connection(0),
	nb_SN_connection_FD(0)
{
	param_recieved = vector<Parameter*>();
	freq_request = vector<double>();
	connected_SN = vector<SubNet*>();
	nb_trames_bridge =0;
	++counter;
	//cout << endl << counter << endl;
}

Calc::~Calc() {
    //--counter;
	// TODO
}

// get / set

// methods
bool Calc::addConnection(SubNet* sn, bool& isSNFD) {
	// TODO : check if the connection already exists
	// do NOT check if there is an available connector on the calculator (LJ)
	connected_SN.push_back(sn);
	nb_SN_connection++;
	if (isSNFD){
        nb_SN_connection_FD++;
	}
	return true;
}


bool Calc::remConnection(Element* sn, bool& isSNFD) {
	bool success = false;
	if (sn->getIsSubNet()) {
		string id_ = sn->getIdElem();

		for (int isn=0;isn<nb_SN_connection;++isn) {
			if (((Element*) connected_SN[isn])->getIdElem().compare(id_) == 0) {
				connected_SN.erase(connected_SN.begin()+isn);
				nb_SN_connection--;
				if (isSNFD){
                    nb_SN_connection_FD--;
				}
				return success = true;	// must stop the loop here because the number of SN has decreased
			}
		}
	} else {
		cerr << "Wrong type: " << sn->getIdElem() << " is not a SubNet object." << endl;
	}

	return success;
}


string Calc::toString(bool toPrint) const {
	ostringstream str;
	str << "********** Calc " << id_element << " (" << this << ")" << " **********" << endl;
	str << "Nb of available connectors: " << nb_max_connector << endl;
	str << "Nb of available FD connectors: " << nb_max_connector_FD << endl;
	str << "Nb param trans : " << nb_param_transmitted << endl;
	str << "Nb param recieved : " << nb_param_recieved << endl;
	str << "Nb param recieved stored : " << param_recieved.size() << endl;
	str << "*                            *" << endl;
	str << "Nb connected SN : " << nb_SN_connection << endl;
	str << "Nb connected FD SN : " << nb_SN_connection_FD << endl;
	str << "Nb connected SN stored: " << connected_SN.size() << endl;
	for (int jj=0; jj<(int)connected_SN.size(); ++jj) {
		str << ((Element*) connected_SN[jj])->getIdElem() << "\t";
	}
	str << endl;
	str << "******************************" << endl;

	if (toPrint){
		cout << str.str();
	}

	return str.str();
}


int numObj(vector<Calc> const * const vecObj, const string& id) {
	int num = -1;
	for (int ii=0; ii<(int)(*vecObj).size(); ++ii) {
		if ((*vecObj)[ii].getIdElem().compare(id) == 0) num = ii;
	}
	if (num < 0) cerr << "Object " << id << " was not found in the stored objects." << endl;
	return num;
}


int numObj(const vector<Calc*>& vecObj, const string& id) {
	int num = -1;
	for (int ii=0; ii<(int)vecObj.size(); ++ii) {

		if (vecObj[ii]->getIdElem().compare(id) == 0) num = ii;
	}
	// if (num < 0) cerr << "Object " << id << " was not found in the stored objects." << endl;
	return num;
}

