#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

#include "includes.h"

#include "Element.h"


// forward declaration because of recursive includes
class Parameter;
class SubNet;


class Calc : public Element {
private:
	bool isBridge;							// is the calculator a bridge
	int nb_param_transmitted;				// number of parameters the calculator emits
	int nb_param_recieved;					// number of parameter the calculator recieves
	int nb_max_connector;                   // number of available connectors
	int nb_max_connector_FD;                // number of available FD connectors
	int nb_max_trames_bridge;
	int nb_trames_bridge;
	int nb_SN_connection;					// actual number of connectors used
	int nb_SN_connection_FD;                // actual number of FD connectors used
	vector<Parameter*> param_recieved;		// pointers to the parameters received
	vector<double> freq_request;			// frequency at which the parameters must be received: the
											// i-th element of the vector matches the i-th parameter of
											// vector param_received
	vector<SubNet*> connected_SN;			// pointers to the subnetworks the calculator is connected to

    static int counter;

public:
	// constructors
	Calc();
	Calc(const string& id_, const bool isBridge_, const int nb_max_con_, const int nb_max_con_FD_);
    Calc(const string& id_, const bool isBridge_, const int nb_param_trans_, const int nb_param_rec_,
		 const int nb_max_con_, int nb_max_con_FD, const int nb_max_trames_);
	~Calc();

	// get / set
	int getIdNUMCalc() const { /*cout<<"ID Calculateur : "<<getIdNUMElem()<<endl;*/ return getIdNUMElem(); }
	string getIdCalc() const { return getIdElem(); }
	void setIdCalc(const string& id) {
	     setIdElem(id);
        setIdNum(counter);
        counter++;
    }

    int getNbBridgeable() const {return nb_max_trames_bridge;}

	bool getIsBridge() const { return isBridge; }
	void setIsBridge(const bool isBdg) { isBridge = isBdg; }
	int getNbParamTransmitted() const { return nb_param_transmitted; }
	void setNbParamTransmitted(const int nb_) { nb_param_transmitted = nb_; }
	int getNbParamRecieved() const { return nb_param_recieved; }
	void setNbParamRecieved(const int nb_) { nb_param_recieved = nb_; }
	int getNbMaxConnector() const { return nb_max_connector; }
	void setNbMaxConnector(const int nb_max) { nb_max_connector = nb_max; }
	int getNbMaxConnectorFD() const {return nb_max_connector_FD;}
	void setNbMaxConnectorFD(const int nb_max) { nb_max_connector_FD=nb_max;}
	int getNbSNConnexion() const { return nb_SN_connection; }
	void setNbSNConnexion(const int nb_) { nb_SN_connection = nb_; }
	int getNbSNConnexionFD() const {return nb_SN_connection_FD;}
	void setNbSNConnexionFD(const int nb_){nb_SN_connection_FD = nb_;}
	vector<Parameter*> const & getParamRecieved() const { return param_recieved; }
	void setParamRecieved(const vector<Parameter*>& param_recieved_) { param_recieved = param_recieved_; }
	vector<double> getFreqRequest() const { return freq_request; }
	void setFreqRequest(const vector<double>& freq_request_) { freq_request = freq_request_; }
	vector<SubNet*> const & getConnectedSN() const { return connected_SN; }
	void setConnectedSN(const vector<SubNet*>& subNetList) { connected_SN = subNetList; }

	// methods
	/***********
	 * addConnection(SubNet* sn)
	 *
	 * Add sn to the connected subnetworks and update nb_SN_connection.
	 *
	 * MODIFY	connected_SN
	 *			nb_SN_connection
	 *
	 * DO NOT CALL. Call SubNet::addCalc(Calc*) instead.
	 ***********/
	bool addConnection(SubNet* sn, bool& isSNFD);

	/***********
	 * remConnection(Element* sn)
	 *
	 * Try to remove sn from the connected subnetworks and update nb_SN_connection.
	 *
	 * MODIFY	connected_SN
	 *			nb_SN_connection
	 *
	 * DO NOT CALL. Call SubNet::remCalc(Calc*) instead.
	 ***********/
	bool remConnection(Element* sn, bool& isSNFD);

	string toString(bool toPrint=true) const;
};


/***********
 * numObj(vector<Template>* vecObj, string id)
 *
 * Return the position in vecObj of the object whose id is id. vecObj should contain unique id objects.
 *
 * INPUT	vector<Template>* vecObj: the vector of object of class Template in which looking for
 * 			string id: the id to be found
 * OUTPUT	A position (int)
 ***********/
int numObj(vector<Calc> const * const vecObj, const string& id);
int numObj(const vector<Calc*>& vecObj, const string& id);


#endif // CALC_H_INCLUDED
