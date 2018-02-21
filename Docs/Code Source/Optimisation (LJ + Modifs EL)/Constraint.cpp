#include "Constraint.h"



bool checkTopologyValidity(vector<Calc> const * const calcList, vector<SubNet> const * const snList,
						   vector< vector<int> > const& calcToCalcConnection,
						   vector< vector<int> > const& snToCalcConnection,
						   bool verbose																) {

	bool valid = true;

	// Constraint on the number of calculators per subnetwork
	for (int isn=0 ;(isn < (int)snList->size()) && valid ; ++isn) {
		if ( (*snList)[isn].getNbCalc() > nb_max_calc_per_SN ) valid = false;
		if (verbose && !valid) cout << (*snList)[isn].getIdElem() << " has too many ("
								  << (*snList)[isn].getNbCalc() << ") connections." << endl;
	}



	// Constraint on the number of subnetwork a calculator can be connected to
	// Use of bridges
	for (int ical = 0 ;(ical < (int)calcList->size()) && valid ; ++ical) {
		// Case is not a bridge
		if ( !(*calcList)[ical].getIsBridge() ) {
            if ((*calcList)[ical].getNbSNConnexion() > (*calcList)[ical].getNbMaxConnector())  {
                valid = false;
                if (verbose&&!valid) cout << (*calcList)[ical].getIdElem() << " has too many connections."
									  << endl;
            }
            if ((*calcList)[ical].getNbSNConnexionFD() > (*calcList)[ical].getNbMaxConnectorFD())  {
                valid = false;
                if (verbose&&!valid) cout << (*calcList)[ical].getIdElem() << " has too many FD connections."
									  << endl;
            }
		}


		// Case is a bridge : must use between 1 and bridge_nb_max_connector connectors
		if ( valid && (*calcList)[ical].getIsBridge() &&
			 ((*calcList)[ical].getNbSNConnexion() > bridge_nb_max_connector) ) {
			valid = false;
			if (verbose&&!valid) cout << (*calcList)[ical].getIdElem() << " has too many connections."
									  << endl;
		}

		if ( valid && (*calcList)[ical].getIsBridge() && ((*calcList)[ical].getNbSNConnexion() == 0) ) {
			// valid = (isInit || false);
			valid = false;
			if (verbose&&!valid) cout << (*calcList)[ical].getIdElem() << " has not been used."
									  << endl;
		}

	}

	if (valid) {
		valid = checkAdditionnalTopologyConstraint(calcList, snList,
													calcToCalcConnection, snToCalcConnection);
	}

	//	cout << "Topology valid: " << valid << endl;
	//getchar();

	if (verbose) {
		cout << "===== Validity of the current topology:\t" << valid << " =====" << endl << endl;
	}

	return valid;
}


bool checkAdditionnalTopologyConstraint(vector<Calc> const * const calcList,
										vector<SubNet> const * const snList,
										vector< vector<int> > const& calcToCalcConnection,
										vector< vector<int> > const& snToCalcConnection	) {

	int ncalc = calcList->size() - nb_bridge;
	int nSN = snList->size();

	if (((int)calcToCalcConnection.size() != ncalc	) ||
		((int)snToCalcConnection.size() != nSN		) ||
		((int)calcToCalcConnection.size() == 0		) ||
		((int)snToCalcConnection.size() == 0			) )	{

		cerr << "Data inconsistency between calculators or subnetworks list and additional topology"
			 << "constraints: number of line." << endl;
		cout << "calcToCalcConnection.size(): " << calcToCalcConnection.size()
			 << "\tncalc: " << ncalc << endl;
		cout << "snToCalcConnection.size(): " << snToCalcConnection.size()
			 << "\tnSN: " << nSN << endl;
        system("PAUSE");
		exit(EXIT_SUCCESS);

	} else if ( ((int)calcToCalcConnection[0].size() != ncalc) ||
				((int)snToCalcConnection[0].size() != ncalc) 	) {

		cerr << "Data inconsistency between calculators or subnetworks list and additional topology"
			 << "constraints: number of columns." << endl;
		cout << "calcToCalcConnection[0].size(): " << calcToCalcConnection[0].size()
			 << "\tncalc: " << ncalc << endl;
		cout << "snToCalcConnection[0].size(): " << snToCalcConnection[0].size()
			 << "\tncalc: " << ncalc << endl;
        system("PAUSE");
		exit(EXIT_SUCCESS);

	}
	int valid = true;

	for (int ical=0 ; (ical<ncalc) && valid ; ++ical) {
		for (int ical2=0 ; (ical2<ncalc) && valid ; ++ical2) {

			if ( (calcToCalcConnection[ical][ical2]==-1) || (calcToCalcConnection[ical][ical2]==1) ) {

				// Look for a common network
				bool isACommonSN = false;
				for (int iconsn=0 ; (iconsn<(*calcList)[ical].getNbSNConnexion())&&(!isACommonSN) ;
																							++iconsn) {

					if (numObj((*calcList)[ical2].getConnectedSN(),
					    (*calcList)[ical].getConnectedSN()[iconsn]->getIdElem()) >= 0) {

						isACommonSN = true;
					}
				}
				if ((calcToCalcConnection[ical][ical2] == -1) && isACommonSN) {
																		// there should not be any common
																		// SN but there is one
					valid = false;
				}
				if ((calcToCalcConnection[ical][ical2] == 1) && !isACommonSN) {
																		// there should be a common SN and
																		// there is no
					valid = false;
				}
			}
		}

		for (int isn=0 ; (isn<nSN) && valid ; ++isn) {
			if (snToCalcConnection[isn][ical] == -1) {
				if ((*snList)[isn].containCalc(&(*calcList)[ical])) {	// the SN and the calc should not
																		// be connected but they are
					valid = false;
				}
			}
			else if (snToCalcConnection[isn][ical] == 1) {
				if (!(*snList)[isn].containCalc(&(*calcList)[ical])) {	// the SN and the calc should be
																		// connected but they are not
					valid = false;
				}
				if ((*snList)[isn].getIsCanFD() && (*calcList)[ical].getNbMaxConnectorFD()==0){  // the SN and the calc should not be connected
				    valid = false;                                                              // since the SN is FD and the calc has no FD connectors
				}
			}
		}
	}
	return valid;
}


bool checkRoutingValidity(vector<Calc> const * const calcList, vector<PathFrame> const * const pfList,
						  bool verbose																	){

	bool valid = true;
	int nbPath = pfList->size();

	// Check constraints on the paths
	for (int itg=0;(itg<(int)calcList->size()) && valid;++itg) {
		for (int ipar=0;(ipar<(*calcList)[itg].getNbParamRecieved()) && valid;++ipar) {
			string id_param = (*calcList)[itg].getParamRecieved()[ipar]->getIdParam();


			// Constraints
			double freq_request = (*calcList)[itg].getFreqRequest()[ipar];
			string id_source = (*calcList)[itg].getParamRecieved()[ipar]->getCalcSource()->getIdElem();
			string id_target = (*calcList)[itg].getIdElem();
			bool delivered = false;

			// Are they fulfilled ?
			for (int ipath=0;(ipath<nbPath) && (!delivered);++ipath) {
				if ( ( (*pfList)[ipath].getSource()->getIdElem().compare(id_source) == 0 ) &&
					 ( (*pfList)[ipath].getTarget()->getIdElem().compare(id_target) == 0 ) &&
					 ( (*pfList)[ipath].getFrame()->getFreqTransmission() >= freq_request ) &&
					 ( (*pfList)[ipath].isPathValid() ) ) {
						// Is the current parameter in that frame ?
						Frame* curFrame = (*pfList)[ipath].getFrame();
						for (	int iparFrame=0											;
								(iparFrame<curFrame->getNbFragment()) && (!delivered)	;
								++iparFrame												) {

							if (curFrame->getParamFragment()[iparFrame]->getIdParam().compare(id_param)
									== 0															  ) {
								delivered = true;
							}

						}
				}
			}

			if (!delivered) {
				valid = false;
			}
		}
	}

	// TODO: Check constraint on the load here ?

	if (verbose) {
		cout << "===== Validity of the current configuration:\t" << valid << " =====" << endl << endl;
	}

	return valid;
}

