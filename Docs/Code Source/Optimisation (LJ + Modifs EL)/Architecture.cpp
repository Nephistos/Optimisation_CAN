#include "Architecture.h"

extern int Nombre_Calc; //HERE

int initFrameFromBFDHeuristic(vector<Parameter>* par, vector<Calc> const * const calcList,
							  vector<Frame>* fr, vector<Frame> const * const frIni			) {

	int nbFrame = 0;
	fr->clear();
	fr->reserve(2*par->size());	// worst case if every parameter fits in a different frame

	vector<Frame*> fixedFrameFromSrc;		// frames such that Frame::isContentFixed == true
	vector<Frame*> semiFixedFrameFromSrc;	// frames that contain at least one parameter such that
											// Parameter::isHostFixed == true

	for (vector<Calc>::const_iterator itsrc=calcList->begin() ; itsrc!=calcList->end() ; ++itsrc) {

		int nbFrame_old = nbFrame;

		// Store fixed frames transmitted by the current source calculator
		fixedFrameFromSrc.clear();
		semiFixedFrameFromSrc.clear();
		for (vector<Frame>::const_iterator itfr = frIni->begin() ; itfr!=frIni->end() ; ++itfr) {
			if (itfr->getCalcSource()==&*itsrc) {
				if (itfr->isContentFixed()) {
					fixedFrameFromSrc.push_back(const_cast<Frame*>(&*itfr));
				} else {
					bool semiFixedFrame = false;
					for (int ipar=0 ; ipar<itfr->getNbFragment() ; ++ipar) {
						semiFixedFrame |= itfr->getParamFragment()[ipar]->isHostFixed();
					}
					if (semiFixedFrame) {
						semiFixedFrameFromSrc.push_back(const_cast<Frame*>(&*itfr));
					}
				}
			}
		}
		// Sort the frame by descending transmission frequency
		std::sort(semiFixedFrameFromSrc.begin(), semiFixedFrameFromSrc.end(), greaterFrameFreq);

		// Find all parameters from the current source and store them into parFromSrc in the decreasing
		// order of their highest frequency request and, at equal frequency, decreasing size
		// Note that parameters that have already been placed inside a fixed frame or that have been fixed
		// into a frame host are not added.
		vector<Parameter*> parFromSrc;
		vector<double> hFreqRequest;
		for (vector<Parameter>::iterator itpar=par->begin() ; itpar!=par->end() ; ++itpar) {
			if (itpar->getCalcSource() == &(*itsrc)) {
				if (!itpar->isHostFixed()) {
					vector<double> parFreqRequest = itpar->getFreqTarget();
					double newHighestFreqRequest = *max_element(parFreqRequest.begin(),
																					parFreqRequest.end());

					// Find the position to insert the parameter
					int iPosToInsert = hFreqRequest.size();
					if (iPosToInsert>0) {
						bool withinRange = true;
						while ( (newHighestFreqRequest>hFreqRequest[iPosToInsert-1]) && withinRange ) {
							iPosToInsert--;
							if (iPosToInsert<=0) withinRange = false;
						}
						while( (newHighestFreqRequest==hFreqRequest[iPosToInsert-1])	&&
							   (itpar->getSize()>parFromSrc[iPosToInsert-1]->getSize())	&&
							   withinRange 												) {
							iPosToInsert--;
							if (iPosToInsert<=0) withinRange = false;
						}
					}

					// Insert the parameter
					hFreqRequest.insert(hFreqRequest.begin()+iPosToInsert, newHighestFreqRequest);
					parFromSrc.insert(parFromSrc.begin()+iPosToInsert, &(*itpar));
				}
			}
		}

		/// Create the set of frames that will be emitted by the current source
		vector<Frame*> frameFromSrc;
		// Add the parameters that are fixed to a host
		for (vector<Frame*>::iterator itsffr = semiFixedFrameFromSrc.begin()	;
			 itsffr != semiFixedFrameFromSrc.end()									;
			 ++itsffr																) {

			// Create the "semi fixed" frame
			Frame newframe((*itsffr)->getIdFrame(), 0, const_cast<Calc*>(&(*itsrc)), 0, 0, false, false);
			fr->push_back(newframe);
			nbFrame++;
			frameFromSrc.push_back(&(fr->back()));

			// Add the parameters fixed to this host
			bool forceAddParameter = false;
			for (vector<Parameter*>::const_iterator itpar=(*itsffr)->getParamFragment().begin()	;
				 itpar!=(*itsffr)->getParamFragment().end() 								;
				 ++itpar																	) {

				if ((*itpar)->isHostFixed()) {
					fr->back().addParameter(const_cast<Parameter*>(*itpar), forceAddParameter);
				}
			}
		}
		// At this point, fr is sorted by decreasing transmission frequency because semiFixedFrameFromSrc
		// was.
		for (int iparFromSrc=0 ; iparFromSrc<(int)parFromSrc.size() ; ++iparFromSrc) {
			Parameter* curPar = parFromSrc[iparFromSrc];
			vector<Frame*> emptyFrameVec;
			curPar->setFrameHost(emptyFrameVec);
			bool param_assigned = false;

			// "Name" of the new frame, if it is to be created
			string baseIdFrame = itsrc->getIdElem() + "_"
						 + static_cast<ostringstream*>( &(ostringstream() << (nbFrame)) )->str();
			Frame newframe(baseIdFrame, 0, const_cast<Calc*>(&(*itsrc)), 0, 0);

			// Creation of a new frame if it is the first parameter or if the source has just changed
			if (frameFromSrc.size()==0) {
				fr->push_back(newframe);
				frameFromSrc.push_back(&(fr->back()));
				nbFrame++;
				if (fr->back().addParameter(curPar)) {
					param_assigned = true;
				}
			}

			/// old comment:
			// Pack the parameter in the frame so that the additional charge is the lowest i.e. :
			// - find the first frame where the parameter fits (frames are necessarily created with
			//   decreasing frequency, one must iterate from the end)
			// 		(- if the frequency is the same, pack it in)
			// 		- (if the frequency is higher [than the parameter's], )compute freqFrame*sizeParam
			//		  (packing into the existing frame) and freqParam*(sizeParam+47) (packing into a
			//		  new frame added AT THE END) and do what is the best
			// - if it does not fit, pack into a new frame added at the end
			//
			/// new comment:
			// Pack the parameter in the frame so that the additional charge is the lowest i.e. :
			// - find the first frame where the parameter fits (frames are necessarily created with
			//   decreasing frequency, one must iterate from the end)
			// 		- add the parameter to this frame and compute the cost of adding it
			//		- compute the cost it would take to create a frame for this parameter alone. If it is
			//		  cheaper, remove the parameter from the frame, create a frame, insert it into the
			//		  frame list at the end and into frameFromSrc so that the order of decreasing freq
			//		  stays true.
			// - if it does not fit, pack into a new frame added at the end, and add the pointer to it
			// 	 into frameFromSrc so that the order of decreasing frequency stays true.
			for (int ifrFromSrc=frameFromSrc.size()-1;(ifrFromSrc>=0) && (!param_assigned);--ifrFromSrc) {
				double load_old = frameFromSrc[ifrFromSrc]->getLoad();

				if ( frameFromSrc[ifrFromSrc]->addParameter(curPar) ) {
					double costAddToCurFrame = frameFromSrc[ifrFromSrc]->getLoad() - load_old;
					double costAddToNewFrame = hFreqRequest[iparFromSrc] *
						((((int) ((curPar->getSize() + 7) / 8) * 8 + 34) *1.1) +13)*0.002/1000*100;

					if (costAddToCurFrame<costAddToNewFrame) {	// < to add a new frame if ==
						param_assigned = true;
					} else {
						frameFromSrc[ifrFromSrc]->remParameter(curPar);

						// Insert a new frame so that frameFromSrc remains ordered by descending frequency
						vector<Frame*>::iterator posToInsert = frameFromSrc.begin();
						for (																	;
							  (posToInsert != frameFromSrc.end()) &&
							  ((*posToInsert)->getFreqTransmission() > hFreqRequest[iparFromSrc]) ;
							  ++posToInsert														) {
						}

						fr->push_back(newframe);
						frameFromSrc.insert(posToInsert, &(fr->back()));
						nbFrame++;
						if (fr->back().addParameter(curPar)) {
							param_assigned = true;
						}
					}
				}
			}

			if (!param_assigned) {	// did not fit into any frame
				// Insert a new frame so that frameFromSrc remains ordered by descending frequency
				vector<Frame*>::iterator posToInsert = frameFromSrc.begin();
				for (	;
					  (posToInsert != frameFromSrc.end()) &&
					  ((*posToInsert)->getFreqTransmission() > hFreqRequest[iparFromSrc])	;
					  ++posToInsert															) {
				}
				fr->push_back(newframe);
				frameFromSrc.insert(posToInsert, &(fr->back()));
				nbFrame++;
				if (fr->back().addParameter(curPar)) {
					param_assigned = true;
				}
			}
		}
		// for (int ifr=0 ; ifr<fr->size(); ++ifr) {
			// if ((*fr)[ifr].getIdFrame().compare("ClimBOX_99")==0) (*fr)[ifr].toString(true);
		// }

		// Add the frames whose content is fixed
		for (vector<Frame*>::iterator itffr = fixedFrameFromSrc.begin()	;
			 itffr != fixedFrameFromSrc.end()							;
			 ++itffr													) {
			Frame newframe((*itffr)->getIdFrame(), 0, const_cast<Calc*>(&(*itsrc)), 0, 0, true, false);
			fr->push_back(newframe);
			nbFrame++;

			for (vector<Parameter*>::const_iterator itpar=(*itffr)->getParamFragment().begin()	;
				 itpar!=(*itffr)->getParamFragment().end() 										;
				 ++itpar																		){

				bool forceAddParameter = true;
				fr->back().addParameter(const_cast<Parameter*>(*itpar), forceAddParameter);
			}
		}

		// Creates a few (50-100% more) empty frames for that source so that optimization goes well.
		int nbSrcFrame = nbFrame - nbFrame_old;
		for (int i=0 ; i<nbSrcFrame+1 ; ++i) {	//////////////// here 100% empty frames more !
			string baseIdFrame = itsrc->getIdElem() + "_"
								 + static_cast<ostringstream*>( &(ostringstream() << (nbFrame)) )->str();
			Frame newframe(baseIdFrame, 0, const_cast<Calc*>(&(*itsrc)), 0, 0);
			fr->push_back(newframe);
			nbFrame++;
		}
	}

	return nbFrame;
}


int initPath(vector<SubNet>* subNetData, vector<Frame>* frameData, vector<PathFrame>* pathFrameData,
			 bool verbose) {

	// Authorize the frames to fill all the subnetworks
	vector<SubNet*> allSN;
	for (vector<SubNet>::iterator itSN = subNetData->begin() ; itSN != subNetData->end() ; ++itSN) {
		allSN.push_back(&(*itSN));
	}
	for (vector<Frame>::iterator itFr = frameData->begin() ; itFr != frameData->end() ; ++itFr) {
		if (itFr->isRoutingFixed() == false) {
			itFr->setSNFilled(allSN);
		}	// else we must keep the route initialized during definition

	}

	return drawPath(frameData, pathFrameData, verbose);
}


int drawPath(vector<Frame>* frameData, vector<PathFrame>* pfData, bool verbose) {

	pfData->clear();

	int nbNonExistentPath = 0;

	for (vector<Frame>::iterator itfr = frameData->begin();itfr!=frameData->end();++itfr) {

		for (vector<Calc*>::const_iterator ittgt = itfr->getCalcTarget().begin() ;
			 ittgt != itfr->getCalcTarget().end(); ++ittgt) {

			// Calc* curtgt = *ittgt;
			PathFrame curPF = PathFrame(&(*itfr), itfr->getCalcSource(), *ittgt);
			(*pfData).push_back(curPF);
			if(!(*pfData).back().generatePath()) { //try to create a path
				nbNonExistentPath++;
			}
		}
	}

	if (verbose) {
		cout << "===== Nb of paths not found:\t" << nbNonExistentPath << " =====" << endl << endl;
	}

	return nbNonExistentPath;
}


bool spreadPathperFrame(vector<Frame> * const frameData, vector<SubNet> * const subNetData,
                         vector< vector<int> > const& constFrame,
                        vector<Calc> * const calcData) {

	// Sort along the load of the frames
	vector<Frame*> frameToTreat;
	for (int ifr=0;ifr<(int)frameData->size();++ifr) {
		frameToTreat.push_back(&(*frameData)[ifr]);
	}
	stable_sort(frameToTreat.begin(), frameToTreat.end(), compareFrameLoads);

	// Computes the connection matrix of the network
	vector< vector<int> > curNetworkMatrix;		// 2 times faster than using vector< vector<bool> >
	vector< vector<int> > SNCalcConnection;
	computeNetworkConMatrix(subNetData, &curNetworkMatrix, &SNCalcConnection, calcData);


	for (int i = 0 ; i < nb_it_spreadPath ; ++i) {
		// Treat the frames
		for (int ifr = frameToTreat.size()-1 ; ifr >= 0 ; --ifr) {
			frameToTreat[ifr]->spreadFrame(subNetData, frameData, curNetworkMatrix, constFrame);		// updates the loads
		}

		if (i==0) {
			// at the first iteration, all SN are marked as "used" but their load is 0, thus the load
			// upload is biased and must be re-done correctly.
			updateLoadsFromFrame(subNetData, frameData);
		}
	}



    return true;
}




void copyNetwork(vector<Calc> const * const calcData, vector<SubNet> const * const subNetData,
				 vector<Frame> const * const frameData,
				 vector<Calc>* calcCopy, vector<SubNet>* subNetCopy, vector<Frame>* frameCopy	) {

	calcCopy->clear();
	subNetCopy->clear();
	frameCopy->clear();
	// TODO: reserve()

	for (int ical=0;ical<(int)calcData->size();++ical) {
		Calc newCopy = (*calcData)[ical];	// default copy contructor should be enough
		calcCopy->push_back(newCopy);
	}

	for (int isn=0;isn<(int)subNetData->size();++isn) {
		SubNet newCopy = (*subNetData)[isn];

		// use the copy objects to fill the field SubNet::calcList
		vector<Calc*> calcListCopy;
		for (int iconCal=0;iconCal<newCopy.getNbCalc();++iconCal) {
			int ical = numObj(calcCopy, newCopy.getCalcList()[iconCal]->getIdElem());
			calcListCopy.push_back(&(*calcCopy)[ical]);
		}
		newCopy.setCalcList(calcListCopy);

		subNetCopy->push_back(newCopy);
	}

	// use the copy objects to fill the field Calc::connected_SN
	for (int ical=0;ical<(int)calcData->size();++ical) {
		vector<SubNet*> connected_SNCopy;
		for (int iconSN=0;iconSN<(*calcData)[ical].getNbSNConnexion();++iconSN) {
			int isn = numObj(subNetCopy, (*calcData)[ical].getConnectedSN()[iconSN]->getIdElem());
			connected_SNCopy.push_back(&(*subNetCopy)[isn]);
		}
		(*calcCopy)[ical].setConnectedSN(connected_SNCopy);
	}

	for (int ifr=0;ifr<(int)frameData->size();++ifr) {
		Frame newCopy = (*frameData)[ifr];

		// use the copy objects to fill the fields Frame::calc_source and calc_target
		int isource = numObj(calcCopy, newCopy.getCalcSource()->getIdElem());
		newCopy.setCalcSource(&(*calcCopy)[isource]);

		vector<Calc*> calc_targetCopy;
		for (int itg=0;itg<newCopy.getNbTarget();++itg) {
			int icaltg = numObj(calcCopy, newCopy.getCalcTarget()[itg]->getIdElem());
			calc_targetCopy.push_back(&(*calcCopy)[icaltg]);
		}
		newCopy.setCalcTarget(calc_targetCopy);

		vector<SubNet*> snfilled;
		for (vector<SubNet>::iterator itsn=subNetCopy->begin()	;
			 itsn!=subNetCopy->end()							;
			 ++itsn												) {
			int isn=numObj(newCopy.getSNFilled(), itsn->getIdElem());
			if (isn>=0) {
				snfilled.push_back(&(*itsn));
			}
		}
		bool force = true;
		newCopy.setSNFilled(snfilled, force);

		frameCopy->push_back(newCopy);
	}

	// the fields Calc::param_recieved and Frame::param_fragment still refers to the original objects !

}


void copyData(vector<Calc> const * const calcData, vector<Frame> const * const frameData,
			  vector<Calc>* calcCopy, vector<Frame>* frameCopy							) {

	calcCopy->clear();
	frameCopy->clear();
	calcCopy->reserve(2*calcData->size());
	frameCopy->reserve(2*frameData->size());

	for (int ical=0;ical<(int)calcData->size();++ical) {
		Calc newCopy = (*calcData)[ical];
		newCopy.setConnectedSN(vector<SubNet*>());
		newCopy.setNbSNConnexion(0);
		calcCopy->push_back(newCopy);
	}

	for (int ifr=0;ifr<(int)frameData->size();++ifr) {
		Frame newCopy = (*frameData)[ifr];

		// use the copy objects to fill the fields Frame::calc_source and calc_target
		int isource = numObj(calcCopy, newCopy.getCalcSource()->getIdElem());
		newCopy.setCalcSource(&(*calcCopy)[isource]);

		vector<Calc*> calc_targetCopy;
		for (int itg=0;itg<newCopy.getNbTarget();++itg) {
			int icaltg = numObj(calcCopy, newCopy.getCalcTarget()[itg]->getIdElem());
			calc_targetCopy.push_back(&(*calcCopy)[icaltg]);
		}
		newCopy.setCalcTarget(calc_targetCopy);

		frameCopy->push_back(newCopy);
	}

	// the fields Calc::param_recieved and Frame::param_fragment still refers to the original objects !
}


void copyNetworkFull(vector<Calc> const * const calcData, vector<SubNet> const * const subNetData,
					 vector<Frame> const * const frameData,	vector<Parameter> const * const paramData,
					 vector<Calc>* calcCopy, vector<SubNet>* subNetCopy, vector<Frame>* frameCopy,
					 vector<Parameter>* paramCopy													) {

	paramCopy->clear();
	paramCopy->reserve(2 * paramData->size());

	copyNetwork(calcData, subNetData, frameData, calcCopy, subNetCopy, frameCopy);
	// the fields Calc::param_recieved and Frame::param_fragment still refers to the original objects !
	// Same goes for Parameter::calc_source, Parameter::calc_target and Parameter::frame_host

	// Copy the parameters
	for (int ipar = 0 ; ipar < (int)paramData->size() ; ++ipar) {
		Parameter newCopy = (*paramData)[ipar];
		paramCopy->push_back(newCopy);
	}

	// Update Calc::param_recieved
	for (vector<Calc>::iterator itCalc = calcCopy->begin()	;
			itCalc != calcCopy->end()						;
			++itCalc										) {

		int ipar;
		vector<Parameter*> param_rec;

		for (vector<Parameter*>::const_iterator itparrec = itCalc->getParamRecieved().begin()	;
				itparrec != itCalc->getParamRecieved().end()							;
				++itparrec																) {

			ipar = numObj(paramCopy, (*itparrec)->getIdParam());
			param_rec.push_back(&(*paramCopy)[ipar]);
		}
		itCalc->setParamRecieved(param_rec);
	}

	// Update Frame::param_fragment
	for (vector<Frame>::iterator itfr = frameCopy->begin()	;
			itfr != frameCopy->end()							;
			++itfr												) {

		int ipar;
		vector<Parameter*> param_fragment;
		for (vector<Parameter*>::const_iterator itparrec = itfr->getParamFragment().begin()	;
				itparrec != itfr->getParamFragment().end()								;
				++itparrec																) {

			ipar = numObj(paramCopy, (*itparrec)->getIdParam());
			param_fragment.push_back(&(*paramCopy)[ipar]);
		}
		itfr->setParamFragment(param_fragment);
	}

	// Update Parameter::calc_source, Parameter::calc_target and Parameter::frame_host
	adaptParamToCopiedNetwork(paramCopy, calcCopy, frameCopy);

}


void adaptParamToCopiedNetwork(vector<Parameter> * const par, vector<Calc> const * const calcCopy,
							   vector<Frame> const * const frameCopy								) {

	// CAUTION: calcCopy and frameCopy are not modified; consequently their references must already point
	// to the *par objects.
	for (int ipar=0;ipar<(int)par->size();++ipar) {

		Parameter* curPar = &(*par)[ipar];

		// Change the source address
		int iSourceCopy = numObj(calcCopy, curPar->getCalcSource()->getIdElem());
		curPar->setCalcSource(&(*calcCopy)[iSourceCopy]);

		// Change the targets addresses
		vector<Calc*> newTargets;
		for (int itg=0;itg<curPar->getNbTarget();++itg) {
			int iCurCalcTarget = numObj(calcCopy, curPar->getCalcTarget()[itg]->getIdElem());
			newTargets.push_back(const_cast<Calc*> (&(*calcCopy)[iCurCalcTarget]));
		}
		curPar->setCalcTarget(newTargets);

		// Change the host frames adresses
		int iCurFrameHostCopy;
		for (int ihost=0;ihost<curPar->getNbFramesHost();++ihost) {
			iCurFrameHostCopy = numObj(frameCopy, curPar->getFrameHost()[ihost]->getIdFrame());
			if (iCurFrameHostCopy<0) cout << "curPar->getNbFramesHost(): " << curPar->getNbFramesHost() << endl;
			curPar->swapHost(curPar->getFrameHost()[ihost], &(*frameCopy)[iCurFrameHostCopy], true);
		}
	}
}


bool constructValidNetwork(vector<Calc>* calcData, vector<Frame>* frameData,
						   vector<Calc>* calcGen, vector<SubNet>* subNetGen, vector<Frame>* frameGen,
						   vector<PathFrame>* pathFrameGen,
						   vector< vector<int> > const& calcToCalcConnection,
						   vector< vector<int> > const& snToCalcConnection							) {

	cout << endl;
	cout << "-                 -" << endl;
	cout << "--               --" << endl;
	cout << "--- New Network ---" << endl;
	cout << "--               --" << endl;
	cout << "-                 -" << endl;
	cout << endl;


	subNetGen->clear();

	copyData(calcData, frameData, calcGen, frameGen);

	// Resize the SN list and give name
	subNetGen->reserve(nb_SN_max);
	subNetGen->resize(nb_SN_max, SubNet(-1,"NULL"));
	for (int isn=0;isn<nb_SN_max;++isn) {
		string id_newsn = static_cast<ostringstream*>( &(ostringstream() << isn) )->str();
		(*subNetGen)[isn].setIdElem(id_newsn);
	}

	// Consctruct a topology-valid network where all calculators are connected to all the other within the
	// length

	// Introducing a bit of randomization
	vector<int> randid(calcGen->size(),-1);
	for (int i=0;i<(int)calcGen->size();++i) {
		randid[i] = i;
	}
	random_shuffle(randid.begin(), randid.end());

	int nbConAvailable = 0;
	for (vector<Calc>::iterator itcal=calcGen->begin() ; itcal!=calcGen->end() ; ++itcal) {
		nbConAvailable += itcal->getNbMaxConnector();
	}
	if (nbConAvailable > nb_SN_max * nb_max_calc_per_SN) {
		cerr << "Too few subnetworks vs total number of available connectors." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	for (vector<int>::iterator ical=randid.begin() ; ical!=randid.end() ; ++ical) {

		Calc* itcal = &(*calcGen)[*ical];

		for (int icon=0 ; icon<itcal->getNbMaxConnector() ; ++icon) {

			if (itcal->getNbMaxConnector() <= nb_SN_max) {
				int nb_try = 0;

				bool validCon = false;
				int isn;
				while(!validCon) {	// WARNING: no guarantee that it won't loop forever !!! //
					// Try a connector that does not already exist and that does not make the target
					// subnetwork exceeds its maximum capacity.
					isn = rand() % nb_SN_max;
					if (!(*subNetGen)[isn].containCalc(itcal)
						&&
						((*subNetGen)[isn].getNbCalc() < nb_max_calc_per_SN)) {
                        if ((*subNetGen)[isn].getIsCanFD() && itcal->getNbSNConnexionFD()<itcal->getNbMaxConnectorFD() ) {
                            validCon = true;
                        }
                        if (!((*subNetGen)[isn].getIsCanFD())){
                            validCon =true;
                        }
					}

					nb_try++;
					if (nb_try>10*nb_SN_max) {
						cerr << "Not possible to use all connectors of calculator " << itcal->getIdElem()
							 << "." << endl;
						break;
					}
				}
				(*subNetGen)[isn].addCalc(&*itcal);

			} else {
				cerr << "Too few subnetworks vs number of available connectors of calculator "
					 << itcal->getIdElem() << "." << endl;
                system("PAUSE");
				exit(EXIT_FAILURE);
			}
		}

	}

	// We must check that the network is connected. *** TODO ***
	// vector< vector<int> > snGraph;
	// computeNetworkConMatrix(&subNetGen, &snGraph);

	//bool isArchiValid = false;
	// Check the topology validity (should be valid in all cases)
	bool isArchiValid = checkTopologyValidity(calcGen, subNetGen, calcToCalcConnection, snToCalcConnection,
																								true);

	// Compute an initial routing
	if (isArchiValid) {
		isArchiValid = !initPath(subNetGen, frameGen, pathFrameGen, true);
	}

	return isArchiValid;
}


void computeNetworkConMatrix(vector<SubNet> * const subNetData, vector< vector<int> >* snGraph,
                              vector< vector<int> >* SNCalcConnection, vector<Calc> * const calcData) {

	int nbSN = subNetData->size();
	vector<int> vecfalse(nbSN, 0);
	//cout<<nbSN<<endl;
	snGraph->clear();
	*snGraph = vector< vector<int> >(nbSN, vecfalse);

    vector< vector<int> > SNCalcAux;
	vector<int> vecfalse2(Nombre_Calc, 0); // 30 = nbCalc HERE
	SNCalcAux.clear();
	SNCalcAux = vector< vector<int> >(nbSN, vecfalse2);

	SNCalcConnection->clear();
	*SNCalcConnection = vector< vector<int> >(nbSN, vecfalse);

	for (int isn=0;isn<nbSN;++isn) {
		SubNet cursn = (*subNetData)[isn];
		vector<Calc*> calcList = cursn.getCalcList();

		for (vector<Calc*>::iterator itcalc=calcList.begin() ; itcalc!=calcList.end() ; ++itcalc) {
			vector<SubNet*> connectedSN = (*itcalc)->getConnectedSN();
			for (vector<SubNet*>::iterator itSN = connectedSN.begin() ; itSN != connectedSN.end() ; ++itSN) {
				int numConSN = numObj(subNetData, (*itSN)->getIdElem());
				(*snGraph)[isn][numConSN] = 1;
				(*snGraph)[numConSN][isn] = 1;
				(SNCalcAux)[isn][(*itcalc)->getIdNUMCalc()] = 1;
			}
		}
	}
	/*for(int i = 0 ; i < nbSN; i++) {
        for(int j = 0; j < Nombre_Calc; j++) // HERE
            cout<< (SNCalcAux)[i][j] << " ";
        cout << endl;
	}*/

	//cout<<"SNCALCConnection : "<<(*SNCalcConnection).size()<<endl;
	for(int isn = 0; isn < nbSN; isn++) {
        (*SNCalcConnection)[isn][isn] = 10000;
        for (int jsn = isn+1; jsn < nbSN; jsn++) {
            if((*snGraph)[isn][jsn] == 1) {
                for(int icalc = 0; icalc < Nombre_Calc; icalc++) { //HERE
                    if(SNCalcAux[isn][icalc] == 1 && SNCalcAux[jsn][icalc] == 1)
                        (*SNCalcConnection)[isn][jsn] += (*calcData)[icalc].getNbBridgeable(); //add the number of frames max bridgeables
                        //cout<<"j : "<<jsn<<"    -    "<<"i : "<<isn<<" : "<<(*SNCalcConnection)[isn][jsn]<<endl;
                        (*SNCalcConnection)[jsn][isn] = (*SNCalcConnection)[isn][jsn];
                }
            }
        }
	}
// Now, we will compute also the matrix of SN-calc, 1 if there are a connection 0 if is not
}


void clearNetwork(vector<SubNet>* snList) {

	bool emptysn = true;
	for (vector<SubNet>::iterator itsn=snList->end()-1;(itsn>=snList->begin()) && emptysn;--itsn) {
		emptysn = false;
		if (itsn->getNbCalc() == 0) {
			// delete the SN
			snList->erase(itsn);
			emptysn = true;
		}
	}
}


vector< vector<int> > updateLoads(vector<SubNet>* subNetData, vector<Frame>* frameData,
								  vector<PathFrame>* pathFrameData						) {

	// Zeros the loads
	for (int isn=0;isn<(int)subNetData->size();++isn) {
		(*subNetData)[isn].setLoad(0.);
	}

	// Table synthetizing how many times a given frame takes a subnetwork
	vector<int> initFalse(frameData->size(), 0);
	vector< vector<int> > tableSNFrame(subNetData->size(), initFalse);

	for (int ipath = 0 ; ipath < (int)pathFrameData->size() ; ++ipath) {
		int iframe = numObj( frameData, (*pathFrameData)[ipath].getFrame()->getIdFrame() ) ;
		for (int ielt = 1 ; ielt < (*pathFrameData)[ipath].getLength() ; ielt+=2) {
        //why +=2??? because it shows in order: subnetwork calculateur subnetwork calculateur ...
			int isn = numObj( subNetData, (*pathFrameData)[ipath].getElementList()[ielt]->getIdElem() );
			tableSNFrame[isn][iframe]++;
		}
	}

	// Update the loads using that just-created table.
	for (int isn=0;isn<(int)subNetData->size();++isn){
		for (int ifr=0;ifr<(int)frameData->size();++ifr){
          //      cout << tableSNFrame[isn][ifr] << " ";
			if (tableSNFrame[isn][ifr] > 0) {
				//(*subNetData)[isn].addLoad( (*frameData)[ifr].getLoad(), (*frameData)[ifr].getSize(), (*frameData)[ifr].getFreqTransmission());
				(*subNetData)[isn].addLoad( (*frameData)[ifr].getSize(),(*frameData)[ifr].getFreqTransmission());
			}
		}
		//cout << endl;
       // getchar();
	}

	// Update the attribute snFilled of the frames object
	for (int ifr=0;ifr<(int)frameData->size();++ifr) {
		vector<SubNet*> curSNFilled;
		for (int isn=0 ; (isn<(int)subNetData->size()) ; ++isn) {
			if (tableSNFrame[isn][ifr] > 0) {
				curSNFilled.push_back(&(*subNetData)[isn]);
			}
		}
		(*frameData)[ifr].setSNFilled(curSNFilled);
	}

	return tableSNFrame;
}


void updateLoadsFromFrame(vector<SubNet>* subNetData, vector<Frame> const * const frameData) {

	vector<SubNet*> snFilled;

	for (int isn=0;isn<(int)subNetData->size();++isn) {
		(*subNetData)[isn].setLoad(0.);
	}

	for (int ifr=0;ifr<(int)frameData->size();++ifr){
		snFilled = (*frameData)[ifr].getSNFilled();
		for (int isn2=0;isn2<(int)snFilled.size();++isn2) {
			//snFilled[isn2]->addLoad( (*frameData)[ifr].getLoad()*LOADMULTFACTOR, (*frameData)[ifr].getSize(), (*frameData)[ifr].getFreqTransmission() );
			snFilled[isn2]->addLoad( (*frameData)[ifr].getSize(),(*frameData)[ifr].getFreqTransmission() );
		}
	}
}


double evaluateCost(vector<Calc>* calcData, vector<SubNet>* subNetData) {
	double cost = 0.;

	// Price of the subnetworks
	cost += price_SN * evaluateNbSN(subNetData);

	// Price of the connectors
	vector<int> nbPortPerCalc = evaluateNbUsedConnector(calcData);
	cost = price_connector * accumulate(nbPortPerCalc.begin(), nbPortPerCalc.end(), cost);

	// Price of the bridges
	cost += price_bridge * nb_bridge;

	return cost;
}


double evaluateCost(int nbSubNet, int nbConnection) {
	double cost = 0.;

	// Price of the subnetworks
	cost += price_SN * nbSubNet;

	// Price of the connectors
	cost = price_connector * nbConnection;

	// Price of the bridges
	cost += price_bridge * nb_bridge;

	return cost;
}


int evaluateNbSN(vector<SubNet>* subNetData) {
	int nb = 0;

	for (int isn=0;isn<(int)subNetData->size();++isn) {
		if ((*subNetData)[isn].getNbCalc() >= 2) nb++;
	}

	return nb;
}


vector<int> evaluateNbUsedConnector(vector<Calc>* calcData) {
	vector<int> nbPort(calcData->size(),-1);

	for (int ical=0;ical<(int)calcData->size();++ical) {
		nbPort[ical] = (*calcData)[ical].getNbSNConnexion();
	}

	return nbPort;
}


vector<double> evaluateLoadPerSN(vector<SubNet>* subNetData) {
	vector<double> loads(subNetData->size(),-1.);

	for (int isn=0;isn<(int)subNetData->size();++isn) {
		loads[isn] = (*subNetData)[isn].getLoad();
	}

	return loads;
}

double evaluateLatency(vector<Frame>* frameData) {
    double latency_sum=0. ;
	for (int ifr=0;ifr<(int)frameData->size();++ifr) {
		latency_sum += (*frameData)[ifr].getLatency();
	}
	return latency_sum;
}



Criteria evaluateArchi(vector<Frame> * const frList, vector<Calc> * const calcList,
					   vector<SubNet> * const snList,
					   vector< vector<int> > const& calcToCalcConnection,
					   vector< vector<int> > const& snToCalcConnection,
					   vector< vector<int> > const& constFrame,
					   bool* isArchiValid											) {

	*isArchiValid = true;

	// Evaluate topology constraints
	*isArchiValid = checkTopologyValidity(calcList, snList, calcToCalcConnection, snToCalcConnection,
											false);

	// Compute an initial routing
	vector<PathFrame> pfList;
	if (*isArchiValid) {
		*isArchiValid = !initPath(snList, frList, &pfList, false);
	}

	// Update the loads using the initial routing
	if (*isArchiValid) {
		updateLoads(snList, frList, &pfList);
	}

	// Evaluate routing constraints
	if (*isArchiValid) {
		*isArchiValid = checkRoutingValidity(calcList, &pfList, false);
	}

	// Optimize the routing
	if (*isArchiValid) {
		*isArchiValid = spreadPathperFrame(frList, snList, constFrame, calcList);
	}

    if (*isArchiValid) {
		*isArchiValid = checkRoutingConstraint(frList, snList, constFrame, calcList, false);
	}

	// Evaluate the criteria
	double costArchi = evaluateCost(calcList, snList);
	if (!*isArchiValid) {	// set a prohibitive cost
		costArchi = (costArchi+10) * 100;
	}
	vector<double> loadsArchi = evaluateLoadPerSN(snList);
	double latencyArchi = evaluateLatency(frList);
	Criteria valueArchi(costArchi, loadsArchi, latencyArchi);

	return valueArchi;
}

bool checkRoutingConstraint(vector<Frame> * const frameData, vector<SubNet> * const subNetData,
                            vector< vector<int> > const& constFrame, vector<Calc> * const calcData, bool verbose) {

    int id_num;
    int nbSN = subNetData->size();
    for (vector<Frame>::iterator it = frameData->begin(); it != frameData->end(); it++) {
        if((*it).getRestPath() == 1) {
            id_num = (*it).getIdNumFrame();
            for(int i = 0; i < id_num; i++) {
                if (constFrame[id_num][i] == 1) {
                    vector<int> SNtoavoid(nbSN, 0);
                    vector<SubNet*> toavoid = (*frameData)[i].getSNFilled();
                    vector<SubNet*> filled = (*frameData)[id_num].getSNFilled();
                    for(int is = 0; is < (int)toavoid.size(); is++) {
                        SNtoavoid[toavoid[is]->getIdNUMSubNet()] = 1;
                    }
                    for(int is = 0; is < (int)filled.size(); is++) {
                        if(SNtoavoid[filled[is]->getIdNUMSubNet()] >= 1 ) {
                                if(verbose) cout << endl << "The frame " << (*it).getIdFrame()
                                << " cannot be disjoint with the frame " << (*frameData)[i].getIdFrame() << endl;
                                return false;
                        }
                    }
                }
            }
        }

    }

    /**
    BRIDGE TEST
    **/
/*
    // Computes the connection matrix of the network
	vector< vector<int> > curNetworkMatrix;		// 2 times faster than using vector< vector<bool> >
	vector< vector<int> > SNCalcConnection;
	computeNetworkConMatrix(subNetData, &curNetworkMatrix, &SNCalcConnection, calcData);

	// Now we can see if there are more frames then is possible to bridge
	 for (vector<Frame>::iterator it = frameData->begin(); it < frameData->end(); it++) {

        Calc* calc_source = (*it).getCalcSource();
        // Set the initial subnets (SN connected to calc_source)
        vector<int> snSource(nbSN, 0);
        vector<int> idSource;
        for (int iSNsource=0; iSNsource < calc_source->getNbSNConnexion(); ++iSNsource) {
            snSource[ numObj(subNetData, calc_source->getConnectedSN()[iSNsource]->getIdElem()) ] = 1;
            idSource.push_back(numObj(subNetData, calc_source->getConnectedSN()[iSNsource]->getIdElem()));
        }
        vector<SubNet*> filled = (*it).getSNFilled();
        vector<int> out_list;
        int is = 0;
        while(!filled.empty()) {

            int id = filled[is]->getIdNUMSubNet(); //choose the next subnet
            if(snSource[id] != 1) {
                cout << "sn = " << id << endl;
                cout << "size = " << idSource.size() << endl;
                int entry = 0;
                for(int i = 0; i < (int)idSource.size() && entry == 0; i++) {
                    int calc_s = idSource[i];
                    if(curNetworkMatrix[calc_s][id] > 0 && id != calc_s) {
                        SNCalcConnection[calc_s][id]--;
                        SNCalcConnection[id][calc_s]--;
                        out_list.push_back(id);
                       // cout << "Entrei id = " << id << endl;
                        filled.erase(filled.begin()+is);
                        entry = 1;
                    }
                   // cout << "sai" << endl;
                }
                if(entry != 1){
                        int aux = 0;
                        cout << out_list.size() << endl;
                        for(int ig = 0; ig < (int)out_list.size(); ig++) {
                            if(curNetworkMatrix[out_list[ig]][id] > 0 && id != out_list[ig]) {
                                SNCalcConnection[out_list[ig]][id]--;
                                SNCalcConnection[id][out_list[ig]]--;
                                out_list.push_back(id);
                                filled.erase(filled.begin()+is);
                                ig = 22222;
                            } else aux++;
                        }
                        if(aux == (int)out_list.size()) is++;
                 }

            } else {
                out_list.push_back(id);
                filled.erase(filled.begin()+is);
            }
            if(!filled.empty() && is == (int)filled.size()) is--;
        }
	 } */

    return true;
}

