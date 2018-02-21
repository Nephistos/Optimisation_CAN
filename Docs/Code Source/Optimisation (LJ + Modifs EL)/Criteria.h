#ifndef CRITERIA_H_INCLUDED
#define CRITERIA_H_INCLUDED

#include "includes.h"
#include "const_tool.h"

using namespace std;

class Criteria {
private:
	double cost;
	vector<double> loads;
	double latency;

public:
	// constructor/destructor
	// Criteria(const double cost_, const vector<double>& loads_);
	// Criteria(const double cost_, const double load);
	Criteria(const double cost_, const vector<double>& loads_, const double latency_);
	Criteria(const double cost_, const double load, const double latency_);
	~Criteria();

	// get/set
	double getCost() const { return cost; }
	void setCost(const double newcost) { cost = newcost; }
	vector<double> getLoads() const { return loads; }
	double getMaxLoads() const {
	     double maxi = *max_element(loads.begin(),loads.end());
	     return maxi;
    }
	void setLoads(const vector<double>& newLoads) { loads = newLoads; }
	double getLatency() const{ return latency;}
	void setLatency( double newLatency){latency=newLatency;}

	// methods
	friend void chooseBest(int& idBest, Criteria& cBest, int& id2, Criteria& c2);
	friend void chooseBest_sum(int& idBest, Criteria& cBest, int& id2, Criteria& c2);
};

// operator
// Note: the following allows one to compare two criteria whose "loads" do not have the same length.
inline bool operator<(const Criteria& c1, const Criteria& c2) {
	// First: compare the cost
	if (c1.getCost() < c2.getCost()) {
		// cout << c1.getCost() << " - " << c2.getCost() << "\t";
		return true;
	}

	// Second: at equal cost, compare the max load
	if (c1.getCost() == c2.getCost()) {
        // si on optimise la charge des réseaux
        if (mode_opt_rout==0){
            vector<double> loads1 = c1.getLoads();
            vector<double> loads2 = c2.getLoads();
            double maxLoad1 = *max_element(loads1.begin(), loads1.end());
            double maxLoad2 = *max_element(loads2.begin(), loads2.end());
            //cout << "Maxload = " << maxLoad1 << " - " << maxLoad2 << endl;
            if (maxLoad1 < maxLoad2 ) {
                return true;
            }
		}
        // si on optimise la latence
        if (mode_opt_rout==1) {
            if (c1.getLatency()<c2.getLatency()){
                    return true;
                }
            }

        // si on optimise la charge puis la latence
        if (mode_opt_rout==2){
            vector<double> loads1 = c1.getLoads();
            vector<double> loads2 = c2.getLoads();
            double maxLoad1 = *max_element(loads1.begin(), loads1.end());
            double maxLoad2 = *max_element(loads2.begin(), loads2.end());
            if (maxLoad1 < maxLoad2 ) {
                return true;
            }
            if (maxLoad1 == maxLoad2) {
                if (c1.getLatency()<c2.getLatency()){
                    return true;
                }
            }
        }
	}

	// In the other cases
	return false;
}

bool newcomp_Criteria(const Criteria& c1, const Criteria& c2);

inline bool operator>(const Criteria& c1, const Criteria& c2) { return (c2<c1); }
inline bool operator<=(const Criteria& c1, const Criteria& c2) { return !(c2<c1); }
inline bool operator>=(const Criteria& c1, const Criteria& c2) { return !(c1<c2); }


#endif // CRITERIA_H_INCLUDED
