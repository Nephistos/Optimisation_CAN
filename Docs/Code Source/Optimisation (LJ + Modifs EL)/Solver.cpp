#include "Solver.h"


using namespace std;


void optimTabu(vector<Calc>* calcList, vector<SubNet>* snList, vector<Frame>* frList,
			   vector< vector<int> > const& calcToCalcConnection,
			   vector< vector<int> > const& snToCalcConnection,
			   vector< vector<int> > const& constFrame,
			   queue< vector<Calc> > & calcBest, queue< vector<SubNet> > & subNetBest,
			   queue< vector<Frame> > & frameBest										) {

	cout << "Maximum number of iterations: " << nb_it_tabu << endl;
	cout << "Length of the tabu list: " << size_tabu << endl;

	// variables target of the copies
	vector<Calc> calcCopy;
	vector<SubNet> subNetCopy;
	vector<Frame> frameCopy;

	// The following will contain information about the best solution
	Criteria bestCriteria(-1.,-1.,-1.);

	// Tabu structure
	queue< vector<string> > tabu_list;

	// Evaluate the initial architecture
	bool isArchiValid = false;
	Criteria curValue = evaluateArchi(frList, calcList, snList, calcToCalcConnection, snToCalcConnection, constFrame,
										&isArchiValid);

	// If the initial architecture is valid, try to store it as the best known solution
	if (isArchiValid) {
        //Criteria cmaxload(curValue.getCost(), max_load_allowed);
		Criteria cmaxload(curValue.getCost(), max_load_allowed,100.);
		if ( newcomp_Criteria(curValue, cmaxload) ) {
			updateBestTopology(frameBest, calcBest, subNetBest, frList, calcList, snList);
			bestCriteria = curValue;
		}
	}


	int it = 0;
	int it_without_change = 0;

	while (it < nb_it_tabu) {
		cout << "it " << it << "\t";

		bool isRemoval = true;

		Criteria bestValue_it(1000, max_load_allowed[0]*1000,100.);
		// this initial value is expected to be much larger than what con be found in the iteration
		int ibestChoice = -1;

		/// Begin Define the neighborhood to visit
		vector< vector<int> > choice_rem, choice_add;
		vector<int> nextChoice(2,-1);
		for (int isn=0 ; isn < (int)snList->size() ; ++isn) {
            int nb_voisin=0;
			for (int ical=0 ; ical < (int)calcList->size() ; ++ical) {
                if (snToCalcConnection[isn][ical] == 0) {
                        if (((*snList)[isn].getIsCanFD() && (*calcList)[ical].getNbMaxConnectorFD()>0)
                            || !((*snList)[isn].getIsCanFD()))
                   // cout << isn << "   " << ical << "   "<< snToCalcConnection[isn][ical] << endl;
                    nextChoice[0] = ical;
                    nextChoice[1] = isn;

                    if ( (*snList)[isn].containCalc(&(*calcList)[ical]) ) {
                        // Is the movement in the tabu list ?
                        bool isTabu = false;
                        queue< vector<string> > tabu_copy = tabu_list;
                        while (!tabu_copy.empty()) {
                            if ( (tabu_copy.front()[0].compare(_ADD_) == 0) &&
                                 (tabu_copy.front()[1].compare((*calcList)[nextChoice[0]].getIdElem()) == 0) &&
                                 (tabu_copy.front()[2].compare((*snList)[nextChoice[1]].getIdElem()) == 0) ) {
                                    isTabu = true;
                            }
                            tabu_copy.pop();
                        }

                        // Add to the neighborhood iff the movement is not tabu
                    if (!isTabu) choice_rem.push_back(nextChoice);
                     //   cout << nextChoice[0] << " "<< nextChoice[1] << endl;

                    } else {
                        choice_add.push_back(nextChoice);
                 //       cout << nextChoice[0] << " " << nextChoice[1] << endl;
                    }
                }
			}
		}
		// If the stochastic version is asked, the visited neighborhood is restricted to the number of
		// neighbor defined in the configuration file.
		if (allow_stochastic) {
			int nb_choice_rem_to_delete = choice_rem.size() - nb_choice_rem;

			for (int ichoice=0;ichoice<nb_choice_rem_to_delete;++ichoice) {
				int ichoice_to_delete = rand() % choice_rem.size();
				choice_rem.erase(choice_rem.begin()+ichoice_to_delete);
			}

			int nb_choice_add_to_delete = choice_add.size() - nb_choice_add;

			for (int ichoice=0;ichoice<nb_choice_add_to_delete;++ichoice) {
				int ichoice_to_delete = rand() % choice_add.size();
				choice_add.erase(choice_add.begin()+ichoice_to_delete);
			}
		}
		/// End Define the neighborhood to visit
		int aux = 0;

		if (isRemoval) {	// FOR A REMOVAL //

			for (int ichoice = 0 ; ichoice < (int)choice_rem.size() ; ++ichoice) {

				copyNetwork(calcList, snList, frList, &calcCopy, &subNetCopy, &frameCopy);

				if (subNetCopy[choice_rem[ichoice][1]].remCalc(&calcCopy[choice_rem[ichoice][0]])) {
                    //cout << (calcCopy)[choice_rem[ichoice][0]].getNbSNConnexion() << endl;
                    /** If the calculator was no connection with subnets, add a random connection **/
                    aux = 0;
                    if ((calcCopy)[choice_rem[ichoice][0]].getNbSNConnexion() == 0) {
                        for (int ichoice2 = 0; ichoice2 < (int)choice_add.size() && aux == 0  ; ichoice2++) {
                            if(choice_add[ichoice2][0] == choice_rem[ichoice][0]) {
                                subNetCopy[choice_add[ichoice2][1]].addCalc(&calcCopy[choice_add[ichoice2][0]]);
                                aux = 1;
                            }
                        }
                    }

					bool isCurArchiValid = false;
					Criteria curValue = evaluateArchi(&frameCopy, &calcCopy, &subNetCopy,
														calcToCalcConnection, snToCalcConnection, constFrame,
														&isCurArchiValid);

					if (isCurArchiValid) {
						// Is the current modification the best within the neighborhood
						// (and valid in terms of load) ?
                        Criteria cmaxload(curValue.getCost(), max_load_allowed,1000.);
                        if ( newcomp_Criteria(curValue, cmaxload) ) {
                            //chooseBest_sum(ibestChoice, bestValue_it, ichoice, curValue);   BEFORE
							chooseBest(ibestChoice, bestValue_it, ichoice, curValue);
						}
					}
				}
			}
		}

		if (ibestChoice < 0) {	// FOR A NEW CONNECTION //
			isRemoval = false;

			for (int ichoice = 0; ichoice < (int)choice_add.size() ; ichoice++) {

				copyNetwork(calcList, snList, frList, &calcCopy, &subNetCopy, &frameCopy);

				subNetCopy[choice_add[ichoice][1]].addCalc(&calcCopy[choice_add[ichoice][0]]);

				bool isCurArchiValid = false;
				Criteria curValue = evaluateArchi(&frameCopy, &calcCopy, &subNetCopy,
													calcToCalcConnection, snToCalcConnection, constFrame,
													&isCurArchiValid);

				if (isCurArchiValid) {
					// Is the current modification the best within the neighborhood
					// (and valid in terms of load) ?
					// No test against max_load_allowed so that adding a connection should be done
					// when working with a randomly initialized network that is still overloaded.
					chooseBest(ibestChoice, bestValue_it, ichoice, curValue);
					//chooseBest_sum(ibestChoice, bestValue_it, ichoice, curValue);
				}

			}
		}

		/// Begin actual processing
		if (ibestChoice>=0) {

			if (isRemoval) {	// IN CASE OF REMOVAL
				int icalc = choice_rem[ibestChoice][0];
				int iSNToremove = choice_rem[ibestChoice][1];

				// Remove the connection
				(*snList)[iSNToremove].remCalc(&(*calcList)[icalc]);

			} else {	// IN CASE OF NEW CONNECTION
				int iCalcToConnect = choice_add[ibestChoice][0];
				int iSNToConnect = choice_add[ibestChoice][1];

				/////////////////////////// Do sth linked to Criteria : if (curCriteria < neighborCriteria)
				// There is a new connection: the network is worse than before.
				vector<string> newChange(3);
				newChange[0] = _ADD_;
				newChange[1] = (*calcList)[iCalcToConnect].getIdElem();
				newChange[2] = (*snList)[iSNToConnect].getIdElem();
				tabu_list.push(newChange);
				// time_tabu.push(it);//////////////////////////////////////////////

				if ((int)tabu_list.size() > size_tabu) {	// if true, should be == size_tabu+1
					tabu_list.pop();/////////////////////////////////////////////////////////////////////////////////////
				}

				// Add the connection
				(*snList)[iSNToConnect].addCalc(&(*calcList)[iCalcToConnect]);
			}
		} else {
			it_without_change++;
		}
		/// End actual processing


		/// Begin printing some info about the iteration result
		int iChosenCalc = 0;
		int iChosenSN = 0;
		if ((ibestChoice>=0)&&isRemoval) {
			iChosenCalc = choice_rem[ibestChoice][0];
			iChosenSN = choice_rem[ibestChoice][1];
		}
		if ((ibestChoice>=0)&&(!isRemoval)) {
			iChosenCalc = choice_add[ibestChoice][0];
			iChosenSN = choice_add[ibestChoice][1];
		}

		double nextMaxLoad = bestValue_it.getMaxLoads();
		double nextLatency = bestValue_it.getLatency();
		cout << bestValue_it.getCost() << "EUR\t";
		if (ibestChoice>=0) {
			cout << nextMaxLoad << "%  \t"; //max load supported for CAN
			cout << nextLatency << "ms  \t";
			if(isRemoval) cout << "Remove: "; else cout << "Add: ";
			cout << "c" << iChosenCalc << "/sn" << iChosenSN;
		}
		/// End printing some info about the iteration result


		/// Begin updating the best architecture
		// Store the current network if it is the best AND is valid AND below the limit
		bool isArchiValid = false;
		Criteria valueBestNeighbor = evaluateArchi(frList, calcList, snList,
													calcToCalcConnection, snToCalcConnection, constFrame,
													&isArchiValid);

		// [Note] in theory: valueBestNeighbor == bestValue_it
		if (isArchiValid && (ibestChoice>=0)) {
                cout << "  archi valide";

			double cost = valueBestNeighbor.getCost();

            //Criteria newmax(cost, max_load_allowed);
			Criteria newmax(cost, max_load_allowed, 10000.);
			if ( ((bestCriteria.getCost()<0.) && (newcomp_Criteria(valueBestNeighbor, newmax))) ||
				 ((valueBestNeighbor<bestCriteria) && (newcomp_Criteria(valueBestNeighbor, newmax)))) {

				if (bestCriteria.getCost()<0) {	///////////	// 	re-initialize the tabu iteration when the
															//	first valid solution is obtained.
					it=0;
				}

				updateBestTopology(frameBest, calcBest, subNetBest, frList, calcList, snList);
				bestCriteria = valueBestNeighbor;
				cout << "\t*New best solution*";
			}
		}
		/// End updating the best architecture


		cout << endl;

		if (it_without_change >= nb_it_max_without_change) {
			cout << "Tabu aborted." << endl;
			break;
		}

		it++;
	}

	// Set original objects to the best solution found
	if (!calcBest.empty()) {
		copyNetwork(&calcBest.back(), &subNetBest.back(), &frameBest.back(), calcList, snList,
					frList);
	} else {
		// do nothing. The current calcList, [...], are the one to be returned
		cout << "No valid configuration found so far." << endl << endl;
	}
}


void updateBestTopology(queue< vector<Frame> >& frBest, queue< vector<Calc> >& calcBest,
						queue< vector<SubNet> >& snBest,
						vector<Frame> const * const frList, vector<Calc> const * const calcList,
						vector<SubNet> const * const snList) {

	vector<Frame> frame_buf;
	vector<Calc> calc_buf;
	vector<SubNet> subNet_buf;

	frBest.push(frame_buf);		//
	calcBest.push(calc_buf);		// just a kind of "resize with one more element" for queue
	snBest.push(subNet_buf);	//

	copyNetwork(calcList, snList, frList, &calcBest.back(), &snBest.back(),	&frBest.back());

	if ((int)calcBest.size()>nb_network_out) {
		calcBest.pop();
		snBest.pop();
		frBest.pop();
	}
}


void updateBestArchi(queue< vector<Frame> >& frBest, queue< vector<Calc> >& calcBest,
					 queue< vector<SubNet> >& snBest, queue< vector<Parameter> >& parBest,
					 vector<Frame> const * const frList, vector<Calc> const * const calcList,
					 vector<SubNet> const * const snList, vector<Parameter> const * const parList) {

	vector<Frame> frame_buf;
	vector<Calc> calc_buf;
	vector<SubNet> subNet_buf;
	vector<Parameter> param_buf;

	frBest.push(frame_buf);		//
	calcBest.push(calc_buf);	// just a kind of "resize with one more element" for queue
	snBest.push(subNet_buf);	//
	parBest.push(param_buf);

	copyNetworkFull(calcList, snList, frList, parList,
					&calcBest.back(), &snBest.back(), &frBest.back(), &parBest.back());

	if ((int)calcBest.size()>nb_network_out) {
		calcBest.pop();
		snBest.pop();
		frBest.pop();
		parBest.pop();
	}
}


void metaTabu(vector<Parameter>* vpar, vector<Calc>* calcList, vector<SubNet>* snList,
			  vector<Frame>* frList, vector<PathFrame>* pathFrameData,
			  vector< vector<int> > const& calcToCalcConnection,
			  vector< vector<int> > const& snToCalcConnection,
			  vector< vector<int> > const& constFrame,
			  int nbItMax, int nbItHeating, double temp_max								) {

	/// -----
	cout << "Maximum number of iterations: " << nb_it_tabu << endl;
	cout << "Length of the tabu list: " << size_tabu << endl;

	// variables target of the copies
	vector<Calc> calcCopy;
	vector<SubNet> subNetCopy;
	vector<Frame> frameCopy;
	vector<Parameter> paramCopy;
	vector<PathFrame> pfCopy;

	// The following will contain information about the best solution
	queue< vector<Calc> > calcBest;
	queue< vector<SubNet> > subNetBest;
	queue< vector<Frame> > frameBest;
	queue< vector<Parameter> > paramBest;
	Criteria bestCriteria(-1.,-1.,-1.);

	// Tabu structure
	queue< vector<string> > tabu_list;
//	queue<int> time_tabu;

	// Evaluate the initial architecture
	bool isArchiValid = false;
	Criteria curValue = evaluateArchi(frList, calcList, snList, calcToCalcConnection, snToCalcConnection, constFrame,
										&isArchiValid);

	// If the initial architecture is valid, try to store it as the best known solution
	if (isArchiValid) {
		Criteria cmaxload(curValue.getCost(), max_load_allowed,1000.);
		cout << curValue.getMaxLoads() << "%" << endl;
		if ( curValue < cmaxload ) {
			updateBestArchi(frameBest, calcBest, subNetBest, paramBest, frList, calcList, snList, vpar);
			bestCriteria = curValue;
		}
	}
	int it = 0;
	int it_without_change = 0;
	/// -----


	bool optimMS = false;	// so that as long as the parameters are all delivered with low loads, there
							// is no need to optimise the message set

	while (it<nb_it_tabu) {
		cout << "it " << it << "\t";

		bool isRemoval = true;

		Criteria bestValue_it(1000, max_load_allowed[0]*1000,1000.);
		// this initial value is expected to be much larger than what con be found in the iteration
		int ibestChoice = -1;

		/// Begin Define the neighborhood to visit
		vector< vector<int> > choice_rem, choice_add;
		vector<int> nextChoice(2,-1);
		for (int isn=0 ; isn<(int)snList->size() ; ++isn) {
			for (int ical=0 ; ical<(int)calcList->size() ; ++ical) {

				nextChoice[0] = ical;
				nextChoice[1] = isn;

				if ( (*snList)[isn].containCalc(&(*calcList)[ical]) ) {
					// Is the movement in the tabu list ?
					bool isTabu = false;
					queue< vector<string> > tabu_copy = tabu_list;
					while (!tabu_copy.empty()) {
						if ( (tabu_copy.front()[0].compare(_ADD_) == 0) &&
							 (tabu_copy.front()[1].compare((*calcList)[nextChoice[0]].getIdElem()) == 0) &&
							 (tabu_copy.front()[2].compare((*snList)[nextChoice[1]].getIdElem()) == 0) ) {
								isTabu = true;
						}
						tabu_copy.pop();
					}

					// Add to the neighborhood iif the movement is not tabu
					if (!isTabu) {
						choice_rem.push_back(nextChoice);
					}

				} else {
					choice_add.push_back(nextChoice);
				}
			}
		}

		// If the stochastic version is asked, the visited neighbourhood is restricted to the number of
		// neighbour defined in the configuration file.
		if (allow_stochastic) {
			int nb_choice_rem_to_delete = choice_rem.size() - nb_choice_rem;

			for (int ichoice=0;ichoice<nb_choice_rem_to_delete;++ichoice) {
				int ichoice_to_delete = rand() % choice_rem.size();
				choice_rem.erase(choice_rem.begin()+ichoice_to_delete);
			}

			int nb_choice_add_to_delete = choice_add.size() - nb_choice_add;

			for (int ichoice=0;ichoice<nb_choice_add_to_delete;++ichoice) {
				int ichoice_to_delete = rand() % choice_add.size();
				choice_add.erase(choice_add.begin()+ichoice_to_delete);
			}
		}
		/// End Define the neighborhood to visit


		if (isRemoval) {	// FOR A REMOVAL //

			for (int ichoice=0;ichoice<(int)choice_rem.size();++ichoice) {
				cout << ichoice+1 << "/" << choice_rem.size() << " " << flush;
				copyNetworkFull(calcList, snList, frList, vpar,
								&calcCopy, &subNetCopy, &frameCopy, &paramCopy);



				if (subNetCopy[choice_rem[ichoice][1]].remCalc(&calcCopy[choice_rem[ichoice][0]])) {
					bool isCurArchiValid = false;
					Criteria curValue = evaluateArchi(&frameCopy, &calcCopy, &subNetCopy,
														calcToCalcConnection, snToCalcConnection, constFrame,
														&isCurArchiValid);

					if (isCurArchiValid && optimMS) {	///////////////////////////////////
						optimLoadsSimulatedAnnealing2(&paramCopy, &calcCopy, &subNetCopy, &frameCopy,
													  &pfCopy,
													  calcToCalcConnection, snToCalcConnection, constFrame,
													  nbItMax, temp_max);

					}

					curValue = evaluateArchi(&frameCopy, &calcCopy, &subNetCopy,
												calcToCalcConnection, snToCalcConnection, constFrame,
												&isCurArchiValid);

					if (isCurArchiValid) {
						// Is the current modification the best witin the neighborhood
						// (and valid in terms of load) ?
						Criteria cmaxload(curValue.getCost(), max_load_allowed, 1000.);
						if ( curValue < cmaxload ) {
							chooseBest(ibestChoice, bestValue_it, ichoice, curValue);
						}
					}
				}
			}
			cout << endl;
		}

		if (ibestChoice<0) {	// FOR A NEW CONNECTION //
			isRemoval = false;
			optimMS = true; ////////////////////
			for (int ichoice=0;ichoice<(int)choice_add.size();ichoice++) {
				cout << ichoice+1 << "/" << choice_add.size() << " " << flush;
				copyNetworkFull(calcList, snList, frList, vpar,
								&calcCopy, &subNetCopy, &frameCopy, &paramCopy);

				if (subNetCopy[choice_add[ichoice][1]].addCalc(&calcCopy[choice_add[ichoice][0]])) {

					bool isCurArchiValid = false;
					Criteria curValue = evaluateArchi(&frameCopy, &calcCopy, &subNetCopy,
														calcToCalcConnection, snToCalcConnection, constFrame,
														&isCurArchiValid);

					if (isCurArchiValid) {
						optimLoadsSimulatedAnnealing2(&paramCopy, &calcCopy, &subNetCopy, &frameCopy,
													  &pfCopy,
													  calcToCalcConnection, snToCalcConnection, constFrame,
													  nbItMax, temp_max);
					}

					curValue = evaluateArchi(&frameCopy, &calcCopy, &subNetCopy,
												calcToCalcConnection, snToCalcConnection, constFrame,
												&isCurArchiValid);
					if (isCurArchiValid) {
						// Is the current modification the best witin the neighborhood
						// (and valid in terms of load) ?
						// No test against max_load_allowed so that adding a connection should be done
						// when working with a randomly initialized network that is still overloaded.
						chooseBest(ibestChoice, bestValue_it, ichoice, curValue);
					}
				}
			}
			cout << endl;
		}

		/// Begin actual processing
		if (ibestChoice>=0) {

			if (isRemoval) {	// IN CASE OF REMOVAL
				int icalc = choice_rem[ibestChoice][0];
				int iSNToremove = choice_rem[ibestChoice][1];

				// Remove the connection
				(*snList)[iSNToremove].remCalc(&(*calcList)[icalc]);

			} else {	// IN CASE OF NEW CONNECTION
				int iCalcToConnect = choice_add[ibestChoice][0];
				int iSNToConnect = choice_add[ibestChoice][1];

				/////////////////////////// Do sth linked to Criteria : if (curCriteria < neighborCriteria)
				// There is a new connection: the network is worse than before.
				vector<string> newChange(3);
				newChange[0] = _ADD_;
				newChange[1] = (*calcList)[iCalcToConnect].getIdElem();
				newChange[2] = (*snList)[iSNToConnect].getIdElem();
				tabu_list.push(newChange);
				// time_tabu.push(it);//////////////////////////////////////////////

				if ((int)tabu_list.size() > size_tabu) {	// if true, should be == size_tabu+1
					tabu_list.pop();/////////////////////////////////////////////////////////////////////////////////////
				}

				// Add the connection
				(*snList)[iSNToConnect].addCalc(&(*calcList)[iCalcToConnect]);
			}
		} else {
			it_without_change++;
		}
		/// End actual processing

		// if (tabu_list.size() > 0) {
			// if ( it-time_tabu.front() > size_tabu) {
				// tabu_list.pop();////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// time_tabu.pop();
			// }
		// }

		/// Begin printing some info about the iteration result
		int iChosenCalc = 0;
		int iChosenSN = 0;
		if ((ibestChoice>=0)&&isRemoval) {
			iChosenCalc = choice_rem[ibestChoice][0];
			iChosenSN = choice_rem[ibestChoice][1];
		}
		if ((ibestChoice>=0)&&(!isRemoval)) {
			iChosenCalc = choice_add[ibestChoice][0];
			iChosenSN = choice_add[ibestChoice][1];
		}

		double nextMaxLoad = bestValue_it.getMaxLoads();
		cout << bestValue_it.getCost() << "EUR\t";
		if (ibestChoice>=0) {
			cout << nextMaxLoad/max_load_supported[0]*100 << "%   \t"; //max load supported for CAN
			cout << "c" << iChosenCalc << "/sn" << iChosenSN;
		}
		/// End printing some info about the iteration result


		/// Begin updating the best architecture
		// Store the current network if it is the best AND is valid AND below the limit
		bool isArchiValid = false;
		Criteria valueBestNeighbor = evaluateArchi(frList, calcList, snList,
													calcToCalcConnection, snToCalcConnection, constFrame,
													&isArchiValid);
		if (isArchiValid) {
			optimLoadsSimulatedAnnealing2(vpar, calcList, snList, frList, &pfCopy,
										  calcToCalcConnection, snToCalcConnection, constFrame,
										  nbItMax, temp_max);
			valueBestNeighbor = evaluateArchi(frList, calcList, snList,
												calcToCalcConnection, snToCalcConnection, constFrame,
												&isArchiValid);
		}

		// [Note] in theory: valueBestNeighbor == bestValue_it
		if (isArchiValid && (ibestChoice>=0)) {

			double cost = valueBestNeighbor.getCost();

			if ( ((bestCriteria.getCost()<0.) && (valueBestNeighbor<Criteria(cost,max_load_allowed,1000.))) ||
				 ((valueBestNeighbor<bestCriteria) && (valueBestNeighbor<Criteria(cost,max_load_allowed,1000.)))) {

			/*	if (bestCriteria.getCost()<0) {	///////////	// 	re-initialize the tabu iteration when the
															//	first valid solution is obtained.
					it=0;
				}*/

				updateBestTopology(frameBest, calcBest, subNetBest, frList, calcList, snList);
				bestCriteria = valueBestNeighbor;
				cout << "\t*New best solution*";
			}
		}
		/// End updating the best architecture

		cout << endl;

		if (it_without_change >= nb_it_max_without_change) {
			cout << "Tabu aborted." << endl;
			break;
		}

		it++;
	}

	// Set original objects to the best solution found
	if (!calcBest.empty()) {
		copyNetwork(&calcBest.back(), &subNetBest.back(), &frameBest.back(), calcList, snList,
					frList);
	} else {
		// do nothing. The current calcList, [...], are the one to be returned
		cout << "No valid configuration found so far." << endl << endl;
	}

	// And print the other best ones
	while(!calcBest.empty()) {
		bool printThere = false;

		bool isBestArchiValid;
		Criteria valueBestArchi = evaluateArchi(&frameBest.front(), &calcBest.front(), &subNetBest.front(),
												calcToCalcConnection, snToCalcConnection, constFrame,
												&isBestArchiValid);

		// SubNet print
		for (int ii=0;ii<(int)subNetBest.front().size() && printThere;++ii) {
			subNetBest.front()[ii].toString();
		}
		if (printThere) cout << "*+*+*+*+*+*+*+*+*+*+*+*+*+**+*+*+*+*+*+*+*+*+*+*+*+*+*" << endl;

		calcBest.pop();
		subNetBest.pop();
		frameBest.pop();
	}

	// Beware: pathFrameData has been modified within the while loop and does not correspond to the other
	// ***Data variables.
}


void optimLoadsSimulatedAnnealing2(vector<Parameter>* vpar, vector<Calc>* calcList,
								  vector<SubNet>* subNetCur, vector<Frame>* frameCur,
								  vector<PathFrame>* pathFrameCur,
								  vector< vector<int> > const& calcToCalcConnection,
								  vector< vector<int> > const& snToCalcConnection,
								  vector< vector<int> > const& constFrame,
								  int nbItMax, double Tmax) {

	int nbVParam = vpar->size();
	int nbNonEmptyFrame = 0;
	for (vector<Frame>::iterator itfr=frameCur->begin();itfr!=frameCur->end();++itfr) {
		if (!itfr->isFrameEmpty()) nbNonEmptyFrame++;
	}

	double Tini = Tmax;			// 1.
	int nbTryMaxPerIt = 100;
	double min_diff_load = 0.01;	// bits/s
	int nb_accepted_change = 0;

	vector< vector<int> > curNetworkMatrix;
	vector< vector<int> > SNCalcConnection;
	computeNetworkConMatrix(subNetCur, &curNetworkMatrix, &SNCalcConnection, calcList);

	bool isCurArchiValid;
	Criteria valueIni = evaluateArchi(frameCur, calcList, subNetCur,
										calcToCalcConnection, snToCalcConnection, constFrame, &isCurArchiValid);

	vector<Calc*> srcList;

	vector< vector<Frame*> > hostFrames;

	for (vector<Calc>::iterator itsrc=calcList->begin() ; itsrc!=calcList->end() ; ++itsrc) {

		vector<Frame*> srcFrame;
		for (vector<Frame>::iterator itfr=frameCur->begin();itfr!=frameCur->end();++itfr) {
			if ((itfr->getCalcSource() == &(*itsrc)) && (!itfr->isContentFixed())) {
				srcFrame.push_back(&(*itfr));
			}
		}

		hostFrames.push_back(srcFrame);

		srcList.push_back(&(*itsrc));
	}

	double T = Tini;
	for (int it=0 ; (it<nbItMax) && (nbVParam!=0) ; ++it) {

		if (it%(nbItMax/1000)==0) T*=Tfactor;

		bool itsuccess = false;
		vector<double> oldloads_unsorted = evaluateLoadPerSN(subNetCur);
		vector<double> oldloads = oldloads_unsorted;
		stable_sort(oldloads.begin(), oldloads.end());
		reverse(oldloads.begin(), oldloads.end());

		for (int nbTry=0;(!itsuccess) && (nbTry<nbTryMaxPerIt); ) {	// nbTry is incremented inside the loop

			Parameter* par;
			Frame* oldframe;
			int iChosenParam;

			/// Find a Parameter not fixed whose host is not fixed as well
			do {
				iChosenParam = rand() % nbVParam;
				par = &(*vpar)[iChosenParam];
				oldframe = par->getFrameHost()[0];	////////////////	ok if each parameter has only 1 frame host
				++nbTry;
			} while ( ((!(*vpar)[iChosenParam].isHostFixed()) || (!oldframe->isContentFixed())) &&
					  (nbTry<nbTryMaxPerIt)															);

			if ((!(*vpar)[iChosenParam].isHostFixed()) || (!oldframe->isContentFixed())) {
				// means the previous loop has ended but did not find a satisfying parameter to pick
				break;
			}

			vector<Calc*>::iterator itsrc = find(srcList.begin(), srcList.end(), par->getCalcSource());

			int ical = distance( srcList.begin(), itsrc );
			int iChosenFrame;
			Frame* newframe;

			/// Find a target host not fixed
			do {
				iChosenFrame = rand() % hostFrames[ical].size();
				newframe = hostFrames[ical][iChosenFrame];
				++nbTry;
			} while (((oldframe == newframe) || (newframe->isContentFixed())) && (nbTry<nbTryMaxPerIt));

			if ((oldframe == newframe) || (newframe->isContentFixed())) {
				// means the previous loop has ended but did not find a satisfying frame to pick
				break;
			}

			int size_oldfr_old = oldframe->getSize();
			int size_newfr_old = newframe->getSize();
			double freq_oldfr_old = oldframe->getFreqTransmission();
			double freq_newfr_old = newframe->getFreqTransmission();
			vector<SubNet*> snfilled_oldfr_old = oldframe->getSNFilled();
			vector<SubNet*> snfilled_newfr_old = newframe->getSNFilled();
			bool newFrameWasEmpty = newframe->isFrameEmpty();

			itsuccess = newframe->addParameter(par);	// update the newframe load

			if (itsuccess) {

				itsuccess = oldframe->remParameter(par);	// update the oldframe load

				// The loads are updated inside spreadFrame BUT faultly because it uses only the new
				// load. That is why snFilled are reset before calling this function.
				vector<SubNet*> voidSNVector;
				voidSNVector.clear();
				newframe->setSNFilled(voidSNVector);
				oldframe->setSNFilled(voidSNVector);

				// Update the loads partially manually
				for (int isnf=0;isnf<(int)snfilled_oldfr_old.size();++isnf) {
					//snfilled_oldfr_old[isnf]->addLoad(-LOADMULTFACTOR*load_oldfr_old, oldframe->getSize(), -(oldframe->getFreqTransmission()));
					snfilled_oldfr_old[isnf]->addLoad(size_oldfr_old,freq_oldfr_old);
				}
				for (int isnf=0;isnf<(int)snfilled_newfr_old.size();++isnf) {
					//snfilled_newfr_old[isnf]->addLoad(-LOADMULTFACTOR*load_newfr_old, newframe->getSize(), -(newframe->getFreqTransmission()));
					snfilled_newfr_old[isnf]->addLoad(size_newfr_old, freq_newfr_old);
				}

				// And then spread the modified frames and update the load automatically.
				newframe->spreadFrame(subNetCur, frameCur, curNetworkMatrix, constFrame);			// incomplete spread frame
				oldframe->spreadFrame(subNetCur, frameCur, curNetworkMatrix, constFrame);			// > loads could be better

				vector<double> newloads = evaluateLoadPerSN(subNetCur);
				stable_sort(newloads.begin(), newloads.end());
				reverse(newloads.begin(), newloads.end());

				double deltaLoads = 0.;
				for (int isn=0;(isn<(int)newloads.size()) && (deltaLoads<min_diff_load);++isn) {
					deltaLoads = newloads[isn]-oldloads[isn];
				}

				// Acceptance probability
				double proba_test;
				// double proba_test = 1 / (1 + exp(param_sig*deltaLoads/T));
				proba_test = exp(-param_exp*deltaLoads/T);
				if (deltaLoads<=0) proba_test = 1;
				// if (ith<nbItHeating) proba_test = (deltaLoads<=0);

				double rand_number = (double) rand() / (RAND_MAX);
				if (rand_number < proba_test) {	// if true, the change is accepted
					if (oldframe->isFrameEmpty()) {
						nbNonEmptyFrame--;
					}

					if (newFrameWasEmpty) {
						nbNonEmptyFrame++;
					}
					nb_accepted_change++;
				} else {						// the change is not accepted
					oldframe->setSNFilled(voidSNVector);
					newframe->setSNFilled(voidSNVector);
					oldframe->addParameter(par);
					newframe->remParameter(par);
					oldframe->setSNFilled(snfilled_oldfr_old);
					newframe->setSNFilled(snfilled_newfr_old);

					for (int isn=0;isn<(int)subNetCur->size();++isn) {
						(*subNetCur)[isn].setLoad(oldloads_unsorted[isn]);
					}

				}

				// Every x%, launch the complete path optimization algorithm
				// if ((it%(nbItMax/100))==0) {
					// spreadPathperFrame(frameCur, subNetCur);
					// updateLoadsFromFrame(subNetCur, frameCur);
				// }
			}
		}

		// if (!itsuccess) cout << "No success" << endl;

		// if ((it%(nbItMax/10))==0) {
			// vector<double> curloads = evaluateLoadPerSN(subNetCur);

			// cout << (double) it/(double) nbItMax *100 << "% complete\t"
				 // << nbNonEmptyFrame << " non-empty frames\t"
				 // << "Maxload: " << *max_element(curloads.begin(), curloads.end()) << "bits/s\t"
				 // << "Acceptance rate: " << (double) nb_accepted_change/(double) (it+1) * 100 << "%"
				 // << endl;
		// }
	}

	spreadPathperFrame(frameCur, subNetCur, constFrame, calcList);

}



