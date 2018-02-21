#ifndef SUBNET_H_INCLUDED
#define SUBNET_H_INCLUDED

#include "includes.h"

#include "const_tool.h"

#ifndef ELEMENT_H_INCLUDED
	#include "Element.h"
	#define ELEMENT_H_INCLUDED
#endif
#ifndef CALC_H_INCLUDED
	#include "Calc.h"
	#define CALC_H_INCLUDED
#endif



class SubNet : public Element {
private:
	int nb_calc;										// number of calculators connected to this
	vector<Calc*> calcList;								// pointers to the calculators connected to this
	double load;										// load on this

	static int counter;

	double max_load_supp;
	double max_load_allow;
	bool is_can_FD;

public:
	// constructor
	SubNet(int i, string id_, bool is_FD_=false);

	//constructor fd
	// SubNet(int i, string id_, bool is_FD_);
	//SubNet(const SubNet& sn_);
	~SubNet();

	// get set
	int getIdNUMSubNet() const { return getIdNUMElem(); }
	void setIdNUMSubNet(const int i_) { setIdNum(i_); }
	string getIdSubNet() const { return getIdElem(); }
	int getNbCalc() const { return nb_calc; }
	void setNbCalc(const int nb_) { nb_calc = nb_; }
	vector<Calc*> getCalcList() const {return calcList; }
	void setCalcList(const vector<Calc*>& calcList_) { calcList = calcList_; }
	double getLoad() const { return load; }
	void setLoad(const double load_) { load = load_; }
	bool getIsCanFD () const { return is_can_FD;}
	void setIsCanFD(const bool is_FD_) {is_can_FD=is_FD_;}

	// methods
	/***********
	 * addCalc(Calc* calc_)
	 *
	 * Add the calc passed in parameter to the present subnetwork and return if it succeeded.
	 *
	 * MODIFY	Calc* calc_
	 *				the calculator to add; its fields Calc::connected_SN (and thus Calc::nb_SN_connexion)
	 *				are modified
	 *
	 *			SubNet::calcList and SubNet::nb_calc of the present object are modified
	 *
	 * OUTPUT	a bool that qualifies if the operation went well
	 ***********/
	bool addCalc(Calc* calc_);


	/***********
	 * remCalc(Calc* calc_)
	 *
	 * Remove the calc passed in parameter from the present subnetwork (if it was within) and return if it
	 * succeeded.
	 *
	 * MODIFY	Calc* calc_
	 *				the calculator to remove; its fields Calc::connected_SN (and thus
	 *				Calc::nb_SN_connexion) are modified
	 *
	 *			SubNet::calcList and SubNet::nb_calc of the present object are modified
	 *
	 * OUTPUT	a bool that qualifies if the operation went well
	 ***********/
	bool remCalc(Calc* calc_);


	/***********
	 * addLoad(const double v)
	 *
	 * Add v to the current load, update the load and return the new load.
	 *
	 * INPUT	double v
	 *				load to add (in bits/s)
	 *
	 * MODIFY	SubNet::load
	 *
	 * OUTPUT	the new load
	 ***********/
	double addLoad(int size_,double frequence_);


	/***********
	 * containCalc(const Calc* const calc_)
	 *
	 * Return if the present subnetwork contains the calculator passed in parameter.
	 *
	 * INPUT	const Calc* const calc_
	 *				the calculator that is tested to belong to the present subnetwork
	 *
	 * OUTPUT	a bool that qualifies if the calculator belongs to the present subnetwork
	 ***********/
	bool containCalc(const Calc* const calc_) const;



	string toString(bool toPrint=true) const;
};


/***********
 * compareSNLoads(SubNet sn1, SubNet sn2)
 * comparepSNLoads(SubNet* sn1, SubNet* sn2)
 *
 * Return if subnetwork sn1 is strictly less loaded than subnetwork sn2.
 * Equivalent to sn1.load < sn2.load
 *
 * INPUT	SubNet sn1 or SubNet* sn1
 *				first subnetwork
 *
 *			SubNet sn2 or SubNet* sn2
 *				second subnetwork
 *
 * OUTPUT	bool that qualifies if the first subnetwork (sn1) is strictly less loaded than the second one
 *			(sn2)
 ***********/
bool compareSNLoads(SubNet sn1, SubNet sn2);
bool comparepSNLoads(SubNet* sn1, SubNet* sn2);


/***********
 * numObj(vector<Template>* vecObj, string id)
 *
 * Return the position in vecObj of the object whose id is id. vecObj should contain unique id objects.
 *
 * INPUT	vector<Template>* vecObj: the vector of object of class Template in which looking for
 * 			string id: the id to be found
 * OUTPUT	A position (int)
 ***********/
int numObj(vector<SubNet> const * const vecObj, const string& id);

int numObj(const vector<SubNet*>& vecObj, const string& id);



#endif // SUBNET_H_INCLUDED
