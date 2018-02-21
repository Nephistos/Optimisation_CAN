#include "SubNet.h"


int SubNet::counter = 0;

// constructor
SubNet::SubNet(int i, string id_, bool is_FD_):
	Element(i, id_, true, is_FD_),
	nb_calc(0),
	calcList(vector<Calc*>()),
	load(0.0),
	is_can_FD(is_FD_)
{
    max_load_supp = max_load_supported[i];
    max_load_allow = max_load_allowed[i];
    ++counter;
    //cout << "Counter : " <<  counter << endl;
}


/*SubNet::SubNet(const SubNet& sn_):
	Element(sn_.id_element, true),
	nb_calc(sn_.nb_calc),
	calcList(sn_.calcList),
	charge(0.)
{
}*/

SubNet::~SubNet() {
	// TODO
	--counter;
	//cout << counter << endl;
	//getchar();
}

// get set

// methods
bool SubNet::addCalc(Calc* calc_) {

	if (!this->containCalc(calc_)) {  // do NOT check if there is an available connector on the calculator (LJ)
		calc_->addConnection(this, is_can_FD);
		calcList.push_back(calc_);
		nb_calc++;
		return true;
	} else {
		return false;
	}

	return false;
}


bool SubNet::remCalc(Calc* calc_) {
	bool success = false;
	//string id_ = calc_->getIdCalc();
    int id_ = calc_->getIdNUMCalc();

	//cout << endl << "Je cherche: " << id_ << " " << calc_->getIdNUMCalc() << endl;
	//cout << "Num calcs: " << getNbCalc() << endl;


    for (int ical=0;ical<nb_calc;++ical) {
        //if (calcList[ical]->getIdElem().compare(id_) == 0) {
        if (calcList[ical]->getIdNUMCalc() == id_) {
            //cout << "J'ai trouv'e: " <<  calcList[ical]->getIdCalc() << " " << calcList[ical]->getIdNUMCalc() << endl;
            //getchar();
            calcList[ical]->remConnection((Element*) this, is_can_FD);
            calcList.erase(calcList.begin()+ical);
            nb_calc--;
            return success = true;	// must stop the loop here because the number of SN has decreased
        }
    }


	return success;
}


/*double SubNet::addLoad(double v, int size_bytes, double frequence) {
    double newload = 0.0;
    // TODO : changer le calcul de charge selon que le sous réseau est CAN-FD ou non
     if (is_can_FD) {}
    if (!is_can_FD) {}
    load+=v*100;
    if(max_load_supp == 500000) {       // équivalent de if is_can_FD
        load += (v*100)/max_load_supp;
    } else {
        newload = (16.5*1.1 + 12.5)*0.002 + (10 + size_bytes*8)*1.1*(1/max_load_supp);
        if(size_bytes < 17) newload += 22/max_load_supp;
        else newload += 27/max_load_supp;
        //cout << newload << " "<< frequence << "  " << 1/frequence << endl; getchar();
        load+= newload*frequence/1000*100;
       // cout << newload*frequence/1000*100 << " "<< load << getchar();
    }
	return load;
}*/

double SubNet::addLoad(int size_, double frequence_){
    if (is_can_FD){
        load+=((16.5*1.1 + 12.5)*0.002 + (10 + size_)*1.1*0.0005+22*0.0005)*frequence_/1000/max_load_allow*100;
    }
    else {
        load+=((34+size_)*1.1+13) *0.002*frequence_/1000/max_load_allow*100;
    }
    return load;
}


bool SubNet::containCalc(const Calc* calc_) const {
	bool test = false;
	//cout<<"Objet : "<<(*calc_)<<" - Adresse  : "<<endl;
	int curCalc = (*calc_).getIdNUMElem();

	for (int ical=0;ical<nb_calc;++ical) {
		if (calcList[ical]->getIdNUMElem() == curCalc) test = true;
	}
	return test;
}


string SubNet::toString(bool toPrint) const {
	ostringstream str;
	str << endl << "---------- SubNet " << id_element << " ----------" << endl;
	str << "Nb connected calc : " << nb_calc << endl;
	str << "Nb connected calc stored: " << calcList.size() << endl;
	for (int jj=0; jj<(int)calcList.size(); ++jj) {
		str << calcList[jj]->getIdElem() << "\t";
	}
	str << endl;
	str << "Current load : " << load << endl;
	str << "------------------------------" << endl;

	if (toPrint){
		cout << str.str();
	}

	return str.str();
}


bool compareSNLoads(SubNet sn1, SubNet sn2) {
	return (sn1.getLoad() < sn2.getLoad());
}


bool compareSNLoads(SubNet* sn1, SubNet* sn2) {
	return (sn1->getLoad() < sn2->getLoad());
}


int numObj(vector<SubNet> const * const vecObj, const string& id) {
	int num = -1;
	for (int ii=0; ii<(int)(*vecObj).size(); ++ii) {
		if ((*vecObj)[ii].getIdElem().compare(id) == 0) num = ii;
	}
	if (num < 0) cerr << "Object \"" << id << "\" was not found in the stored objects." << endl;
	return num;
}


int numObj(const vector<SubNet*>& vecObj, const string& id) {
	int num = -1;
	for (int ii=0; ii<(int)vecObj.size(); ++ii) {
		if (vecObj[ii]->getIdElem().compare(id) == 0) num = ii;
	}
	// if (num < 0) cerr << "Object " << id << " was not found in the stored objects." << endl;
	return num;
}
