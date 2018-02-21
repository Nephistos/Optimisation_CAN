#include "Criteria.h"


Criteria::Criteria(const double cost_, const vector<double>& loads_, const double totalLatency_):
		 cost(cost_),
         latency(totalLatency_)
         {
	if (loads_.size()>0) {
		loads = loads_;
	} else {
		cerr << "Not a valid criteria." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
}

Criteria::Criteria(const double cost_, const double load, const double totalLatency_):
		 cost(cost_),
         latency(totalLatency_)
         {
	loads = vector<double>(1,load);
}


Criteria::~Criteria() {
}

void chooseBest_sum(int& idBest, Criteria& cBest, int& id2, Criteria& c2) {

	// First: minimize the cost
	// Second: at equal cost, minimize the sum of loads
	vector<double> load_c2 = c2.getLoads();
	vector<double> load_cBest = cBest.getLoads();

	double sum_c2 = 0.0;
	double sum_cBest = 0.0;
	for(int i = 0; i < (int) load_c2.size(); i++) sum_c2 += load_c2[i];
	for(int i = 0; i < (int) load_cBest.size(); i++) sum_cBest += load_cBest[i];

	if (sum_c2 < sum_cBest) {
		idBest = id2;
		cBest = c2;
	}
}

//chooseBest_sum : minimize the sum of loads
//chooseBest_highcharged : minimize the highest load
void chooseBest(int& idBest, Criteria& cBest, int& id2, Criteria& c2) {

	// First: minimize the cost
	// Second: at equal cost, minimize the highest load
	if (c2<cBest) {
		idBest = id2;
		cBest = c2;
	}
}

bool newcomp_Criteria(const Criteria& c1, const Criteria& c2) {
    // First: compare the cost
	if (c1.getCost() < c2.getCost()) {
		// cout << c1.getCost() << " - " << c2.getCost() << "\t";
		return true;
	}

	// Second: at equal cost, compare the max load
	if (c1.getCost() == c2.getCost()) {
		vector<double> loads1 = c1.getLoads();
		vector<double> loads2 = c2.getLoads();
		if (loads1.size() == loads2.size() ){
            for(int i = 0; i < (int)loads1.size(); i++) {
                if (loads1[i] > loads2[i]) return false;
            }
            return true;
		}
	}

	// In the other cases
	return false;

}

// inline bool operator<(const Criteria& c1, const Criteria& c2) {
	// // First: compare the cost
	// if (c1.getCost() < c2.getCost()) {
		// return true;
	// }

	// // Second: at equal cost, compare the max load
	// if (c1.getCost() == c2.getCost()) {
		// double maxLoad1 = *max_element(c1.getLoads().begin(), c1.getLoads().end());
		// double maxLoad2 = *max_element(c2.getLoads().begin(), c2.getLoads().end());
		// if (maxLoad1 < maxLoad2) {
			// return true;
		// }
	// }

	// // In the other cases
	// return false;
// }
