#include "Frame.h"



// builder
/*
Frame::Frame(string id_, double freq_, Calc* source_, int nb_target_, int nb_fragment_) :
	id_frame(id_),
	freq_transmission(freq_),
	calc_source(source_), // does it work ?
	nb_target(nb_target_),
	nb_fragment(nb_fragment_),
	calc_target(vector<Calc*>()),
	param_fragment(vector<Parameter*>()),
	size_fragment(vector<int>()),
	target_occ(vector<int>()),
	fragment_occ(vector<int>()),
	freqRequest_fragment(list<double>()),
	load(0.),
	snFilled(vector<SubNet*>()),
	fixed_frame(false),
	fixed_routing(false)
{
}

Frame::Frame(string id_, double freq_, Calc* source_, int nb_target_, int nb_fragment_,
			 bool fixed_frame_) :
	id_frame(id_),
	freq_transmission(freq_),
	calc_source(source_), // does it work ?
	nb_target(nb_target_),
	nb_fragment(nb_fragment_),
	calc_target(vector<Calc*>()),
	param_fragment(vector<Parameter*>()),
	size_fragment(vector<int>()),
	target_occ(vector<int>()),
	fragment_occ(vector<int>()),
	freqRequest_fragment(list<double>()),
	load(0.),
	snFilled(vector<SubNet*>()),
	fixed_frame(fixed_frame_),
	fixed_routing(false)
{
}*/

Frame::Frame(string id_, double freq_, Calc* source_, int nb_target_, int nb_fragment_,
			 bool fixed_content_, bool fixed_routing_) :
	id_frame(id_),
    rest_path(0),
	freq_transmission(freq_),
	calc_source(source_), // does it work ?
	nb_target(nb_target_),
	nb_fragment(nb_fragment_),
	calc_target(vector<Calc*>()),
	param_fragment(vector<Parameter*>()),
	size_fragment(vector<int>()),
	load_CAN(0.),
	load_CANFD(0.),
	latency(0.),
	target_occ(vector<int>()),
	fragment_occ(vector<int>()),
	freqRequest_fragment(list<double>()),
	load(0.),
	snFilled(vector<SubNet*>()),
	fixed_content(fixed_content_),
	fixed_routing(fixed_routing_)
{
}


Frame::~Frame() {
	// TODO
}

// get set
void Frame::setSNFilled(vector<SubNet*> snFilled_, bool force) {
	if ((!fixed_routing) || force) {
		snFilled = snFilled_;
	}
}

// methods
void Frame::updateLoad() {
	load = 0.;
	if (nb_fragment > 0) {
		int size_ = 0;
		for (int i=0;i<nb_fragment;++i) {
			size_ += size_fragment[i];
		}

		// Only complete bytes can be sent
		if (size_%8 != 0) {
			size_ = size_ + (8 - size_%8);
		}

		if ((RESOLUTION_MODE == 2) && no_par_astride_bytes) {
			vector< vector<Parameter*> > parameterArrangement;
			if (this->arrangeParamPlacement(parameterArrangement)) {
				size_ = 8 * parameterArrangement.size();
			} else {
				// the size is computed as before
			}
		}


/* Formule utilisée avant pour calculer la charge
		cout << "size = " << size_ << " + size encapsulation = " << size_encapsulation << endl;



		load = (size_+size_encapsulation) * freq_transmission * 1.1;
*/

        frame_duration =((34+size_)*1.1+13)*0.002;
		frame_durationFD=((16.5*1.1+12.5)*0.002+(10+size_)*1.1*0.0005+22*0.0005);

		load = ((34+size_)*1.1+13)*0.002 *freq_transmission/1000*100;
		load_CAN =((34+size_)*1.1+13)*0.002 *freq_transmission/1000*100;
		load_CANFD=((16.5*1.1+12.5)*0.002+(10+size_)*1.1*0.0005+22*0.0005)*freq_transmission/1000*100;
		// load_CANFD = ((34+size_)*1.1+35) *freq_transmission;

       // cout << "size = " << size_ << " * frequency " << freq_transmission << " load = " << load << endl;
       // getchar();

	}
}


bool Frame::arrangeParamPlacement() const {
	vector< vector<Parameter*> > parameterArrangement;
	return arrangeParamPlacement(parameterArrangement);
}


bool Frame::arrangeParamPlacement(vector< vector<Parameter*> >& parameterArrangement) const {

	parameterArrangement.clear();

	if (accumulate(size_fragment.begin(),size_fragment.end(),0) > 8*nb_byte_max) {
		// sum of parameters' lengths is too high
		return false;
	}

	if (no_par_astride_bytes) {
		// copies and sort the parameter sizes by descending order
		vector<Parameter*> parList = param_fragment;

		// Sort the parameters by decreasing size
		vector<int> sizeList(parList.size(), 0);
		sort(parList.begin(), parList.end(), greaterParamSize);
		for (int ip=0 ; ip<(int)sizeList.size() ; ++ip) {
			sizeList[ip] = parList[ip]->getSize();
		}

		// Now use a kind of FFD algorithm to fill the bytes
		if (sizeList.size()>0) {
			if (sizeList.front()>8) {	// No way to avoid placing a parameter astride two bytes; the
										// smart placing within this frame is given up.
				if (accumulate(sizeList.begin(), sizeList.end(), 0) > 8*nb_byte_max) {
					return false;
				} else {
					parameterArrangement.push_back(parList);
				}
			} else {
				vector<int> bytes(1,0);
				parameterArrangement.push_back(vector<Parameter*>(0,nullptr));
				for (vector<int>::iterator itpar = sizeList.begin() ; itpar != sizeList.end() ;
																						++itpar) {
					bool placed = false;

					// Place the param fragment into the fist byte it fits.
					for (int ibyte = 0 ; (ibyte<(int)bytes.size()) && (!placed) ; ++ibyte) {
						if (bytes[ibyte]+*itpar<=8) {
							parameterArrangement[ibyte].push_back(
								parList[distance(sizeList.begin(),itpar)] );
							bytes[ibyte] += *itpar;
							placed = true;
						}
					}
					if ((!placed) && ((int)bytes.size()<nb_byte_max)) {
						parameterArrangement.push_back(
								vector<Parameter*>(1,parList[distance(sizeList.begin(),itpar)]) );
						bytes.push_back(*itpar);
						placed = true;
					} else if (!placed) {
						return false;
					}
				}
			}
		}
	} else {
		parameterArrangement.push_back(param_fragment);
	}

	return true;
}


bool Frame::addParameter(Parameter* newpar, bool force) {

	int newpar_size = newpar->getSize();
	vector<Calc*> calcTarget_newpar = newpar->getCalcTarget();
	// Quick check of the source
	if (newpar->getCalcSource()->getIdElem().compare(calc_source->getIdElem()) != 0) {
		// Not the same source
		cout << "Source" << endl;
		return false;
	}
	if (fixed_content && !force) return false;

	// Does the frame already contains this parameter ?
	bool isFragmentAlreadyIn = false;
	string newpar_id = newpar->getIdParam();
	int iparHere = numObj(param_fragment, newpar_id);
	if (iparHere>=0) {
		isFragmentAlreadyIn = true;
	}
	if (!isFragmentAlreadyIn) {		// If no, is there enough room to add it ?
		int size_ = 0;
		for (int i=0;i<nb_fragment;++i) {
			size_ += size_fragment[i];
		}
		// if (size_+newpar_size>8*nb_byte_max) {		// Would be the new size valid ?
			// The size is too big.
		if (size_+newpar_size>8*nb_byte_max) {					// Is there a place to put the parameter ?
			return false;
		} else {
			param_fragment.push_back(newpar);
			size_fragment.push_back(newpar_size);
			nb_fragment++;
			if (!arrangeParamPlacement()) {
				param_fragment.pop_back();
				size_fragment.pop_back();
				nb_fragment--;
				return false;
			}
			fragment_occ.push_back(1);
			newpar->addFrameHost(this, force);
		}
	} else {						// If yes, add the occurrence
		fragment_occ[iparHere] = fragment_occ[iparHere]+1;
		newpar->addFrameHost(this);		//////////////////////////////// to check
	}

	// Does the frame contain each target of this parameter ?
	int nb_new_tgt = calcTarget_newpar.size();
	bool isCurTargetAlreadyIn = false;
	for (int inewtg=0;inewtg<nb_new_tgt;++inewtg) {
		string newpar_tgtId = calcTarget_newpar[inewtg]->getIdElem();
		int iTgtHere = numObj(calc_target, newpar_tgtId);
		if (iTgtHere>=0) {
			isCurTargetAlreadyIn = true;
		}
		if (!isCurTargetAlreadyIn) {	// If no, add the target
			calc_target.push_back(calcTarget_newpar[inewtg]);
			target_occ.push_back(1);
			nb_target++;
		} else {							// If yes, add an occurrence
			target_occ[iTgtHere] = target_occ[iTgtHere]+1;
		}

		// Anyway, it is a new frequency request
		double newfreqRequest = newpar->getFreqTarget()[inewtg];
		list<double>::iterator adding_position = freqRequest_fragment.begin();
		// Add to the list of requested frequency so that the list remains ordered decreasingly.
		for (																					 ;
			 (adding_position!=freqRequest_fragment.end()) && ((*adding_position)>newfreqRequest);
			 ++adding_position																	 ) {
			// nothing to do but increase the position iterator
		}
		freqRequest_fragment.insert(adding_position, newfreqRequest);

		isCurTargetAlreadyIn = false;
	}

	freq_transmission = freqRequest_fragment.front();	// The list cannot be empty and the first element
														// is its maximum

	this->updateLoad();

	return true;
}


bool Frame::remParameter(Parameter* par) {
	string id_par = par->getIdParam();

	if (fixed_content) return false;

	// Look for the current position of the parameter in the frame
	int iparHere = numObj(param_fragment, id_par);

	if (iparHere < 0) {
		cerr << "Param \"" << id_par << "\" could not be found in frame \"" << id_frame << "\"." << endl;
		return false;
	}
	// Remove the parameter fragment from the frame
	fragment_occ[iparHere] = fragment_occ[iparHere]-1;
	if (!par->remFrameHost(this)) {
		// this->toString(true);
		// cout << "this adress: " << this << endl;
		// par->toString(true);
		// cout << "Containing frame: " << par->getFrameHost()[0]->getIdFrame() << endl;
	}

	// If the parameter is no longer needed in the frame
	if (fragment_occ[iparHere]==0) {
		param_fragment.erase(param_fragment.begin()+iparHere);
		size_fragment.erase(size_fragment.begin()+iparHere);
		fragment_occ.erase(fragment_occ.begin()+iparHere);
		nb_fragment--;
	}

	// Look for the targets of the parameter within the targets of the frame
	vector<Calc*> calcTarget_par = par->getCalcTarget();
	int nb_old_tgt = calcTarget_par.size();
	for (int ioldtg=0;ioldtg<nb_old_tgt;++ioldtg) {
		int iTgtHere = numObj(calc_target, calcTarget_par[ioldtg]->getIdElem());

		if (iTgtHere < 0) {
			cerr << "Target \"" << calcTarget_par[ioldtg]->getIdElem()
				 << "\" of Parameter \"" << id_par
				 << "\" could not be found in frame \"" << id_frame << "\"." << endl;
			return false;
		}
		target_occ[iTgtHere] = target_occ[iTgtHere]-1;

		// If the target is no longer needed
		if (target_occ[iTgtHere] == 0) {
			calc_target.erase(calc_target.begin()+iTgtHere);
			target_occ.erase(target_occ.begin()+iTgtHere);
			nb_target--;
		}

		// Anyway it is a frequency request to remove
		double oldFreqRequest = par->getFreqTarget()[ioldtg];
		list<double>::iterator removing_position =
						find(freqRequest_fragment.begin(), freqRequest_fragment.end(), oldFreqRequest);
		// Remove from the list of requested frequency. The list remains ordered decreasingly.
		if(removing_position!=freqRequest_fragment.end()) {
			freqRequest_fragment.erase(removing_position);
		}
	}

	if (!freqRequest_fragment.empty()) {
		freq_transmission = freqRequest_fragment.front();	// The first element of the list
															// is its maximum
	} else {
		freq_transmission = 0.;
	}

	this->updateLoad();

	return true;
}


bool Frame::isFrameValid() {
	// TODO
	return true;
}

/*
bool Frame::spreadFrame(vector<SubNet> const* snList, vector< vector<bool> > const& curNetworkMatrix) {
	 vector< vector<int> > curNetworkMatrix_int;

	for (int i=0; i<(int)curNetworkMatrix.size(); ++i) {
		vector<int> newline(curNetworkMatrix[i].size(), 0);

		for (int j=0; j<(int)curNetworkMatrix[i].size(); ++j) {
			if (curNetworkMatrix[i][j]) { newline[j] = 1; }
		}

		curNetworkMatrix_int.push_back(newline);
	}

	return this->spreadFrame(snList, curNetworkMatrix_int);	// call for the int version
}*/


bool Frame::spreadFrame(vector<SubNet> const* snList, vector<Frame> const* frameData, vector< vector<int> > const& curNetworkMatrix,
                        vector<vector<int> > const& constFrame) {

	int nbSN = snList->size();

	if ( ((int)curNetworkMatrix.size()!=nbSN) || ((int)curNetworkMatrix[0].size()!=nbSN) ) {
		cerr << "Inconsistent parameters for function spreadFrame." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	if (fixed_routing) return true;

	// Remove the load of this frame from the subnetwork it currently uses:
	for (vector<SubNet*>::iterator snf = snFilled.begin(); snf != snFilled.end(); ++snf) {
		//(*snf)->addLoad(-LOADMULTFACTOR * load, getSize(), -getFreqTransmission());
		(*snf)->addLoad(getSize(), -getFreqTransmission());
	}

    vector<int> SNtoavoid(nbSN, 0);
	// Sets the vector of increasing-priority-to-use (=the first has the highest priority NOT to be used)
	// SubNet indices (ie. increasing current load)
	vector<SubNet> snListCopy = *snList;
	vector<int> idSNtoUse(nbSN,-1);			// vector of indices in snList of SN ordered by priority

    if (rest_path == 1) {
        for(int i = 0; i < id_num; i++) {
            if (constFrame[id_num][i] == 1) {
                vector<SubNet*> toavoid = (*frameData)[i].getSNFilled();
                for(int is = 0; is < (int)toavoid.size(); is++) {
                    SNtoavoid[toavoid[is]->getIdNUMSubNet()] = 1;
                }
            }
        }
    }

    int reg = nbSN;
    int max_sn;
	for (int isn=0;isn<nbSN && reg >= 0;++isn) {
		max_sn = max_element(snListCopy.begin(), snListCopy.end(), compareSNLoads) - snListCopy.begin();
		if(SNtoavoid[max_sn] == 1) {
            idSNtoUse[reg] = max_sn;
            snListCopy[idSNtoUse[reg]].setLoad(-1.);
            reg--;
            isn--;
        } else {
            idSNtoUse[isn] = max_sn;
            snListCopy[idSNtoUse[isn]].setLoad(-1.);
        }
	}


	// Check if some SubNet are required because they are the only connection of the source or a target
	//vector<int> idRequiredSN;				// vector of indices in snList of required subnetworks
	// TODO

	// Set the initial subnets (SN connected to calc_source)
	vector<int> snSource(nbSN, 0);
	for (int iSNsource=0;iSNsource<calc_source->getNbSNConnexion();++iSNsource) {
		snSource[ numObj(snList, calc_source->getConnectedSN()[iSNsource]->getIdElem()) ] = 1;
	}

	// Compute on which SN the frame should be spread
	vector< vector<int> > curSpread = curNetworkMatrix;
	vector<int> isSNDelivered(nbSN,0);
	bool validNetwork = false;

	for (int isn=-1;isn<nbSN;++isn) {
		vector< vector<int> > oldInitialSpread = curSpread;
		vector<int> oldsnSource = snSource;
		vector<int> oldSNDelivered = isSNDelivered;

		// Forbid connections from and to SN to delete
		if (isn>=0) {	// first try (isn == -1) without forbiding any SN
			for (int isn2=0;isn2<nbSN;++isn2) {
				curSpread[isn2][idSNtoUse[isn]] = 0;
				curSpread[idSNtoUse[isn]][isn2] = 0;
			}
			snSource[idSNtoUse[isn]] = 0;
		}
		vector< vector<int> > initialSpread = curSpread;

		// Compute the transfer matrix
		curSpread = matrix_power(curSpread, path_order_max);

		// Compute the allowed SN that are reached from the source
		// This is given by (using row vector-matrix product) : isSNDelivered[j] = snSource[k] * curSpread[k][j]
		for (int j=0;j<nbSN;++j) {
			isSNDelivered[j] = 0;
			for (int k=0;(k<nbSN) && (isSNDelivered[j]==0);++k) {
				isSNDelivered[j] = (snSource[k] * curSpread[k][j]);
			}
		}

		// Check if all the targets could be reached
		bool targetNonDelivered = false;
		for (int itg=0 ; (itg<(int)calc_target.size())&&(!targetNonDelivered) ; ++itg) {
			bool isTgDelivered = false;
			vector<SubNet*> snTarget = calc_target[itg]->getConnectedSN();
			// For each SN connected to the current target, check if the frame has reached it.
			for (int iconSN = 0;(iconSN < calc_target[itg]->getNbSNConnexion()) && (!isTgDelivered);++iconSN) {
				// Is that SN delivered ?
				if ( isSNDelivered[ numObj(snList,snTarget[iconSN]->getIdElem()) ] != 0 ) {
					isTgDelivered = true;
				}
			}
			targetNonDelivered = (!isTgDelivered);
		}

		if (targetNonDelivered) {
			curSpread = oldInitialSpread;
			snSource = oldsnSource;
			isSNDelivered = oldSNDelivered;
		}
		else {
			curSpread = initialSpread;
		}

		// validNetwork === true means at least one of the initialSpread configuration was valid
		validNetwork = (validNetwork || (!targetNonDelivered));

		if ((isn<0) && targetNonDelivered) break;	// there is no solution
	}

	// Actually spread the subnets
	vector<SubNet*> newSNFilled;
	if (validNetwork) {
		for (int isn = 0; isn < nbSN; ++isn) {
			if (isSNDelivered[isn]!=0) {
				newSNFilled.push_back(const_cast<SubNet*>(&(*snList)[isn]));
			}
		}
	}
	snFilled.clear();
	snFilled = newSNFilled;

	// Add the load of this frame from the subnetwork it now uses
	// and update the latency of the frame (it depends on the subnet it uses)
	latency=-0.075;
	for (vector<SubNet*>::iterator snf=snFilled.begin();snf<snFilled.end();++snf) {
		//(*snf)->addLoad(LOADMULTFACTOR * load, getSize(), getFreqTransmission());
		(*snf)->addLoad(getSize(),getFreqTransmission());
		if ((*snf)->getIsCanFD()){
            latency+=frame_durationFD;
		}
		else{
            latency+=frame_duration;
		}
		latency+=0.075;

	}

	return validNetwork;
}


string Frame::toString(bool toPrint) const {
	ostringstream str;
	str << "++++++++++ Frame " << id_frame << " ++++++++++" << endl;
	str << "+ Freq: " << freq_transmission << " Hz / Period: " << 1/freq_transmission*1000 << "ms" << endl;
	str << "+ Source: " << calc_source->getIdElem() << " (" << calc_source << ")" << endl;
	str << "+ Nb calc target: " << nb_target << " (" << calc_target.size() << " stored)" << endl;
	str << "+ ";
	for (int jj=0;jj<(int)calc_target.size();++jj) {
		str << calc_target[jj]->getIdElem() << "\t";
	}
	str << endl;
	str << "+ Nb fragments : " << nb_fragment << " (" << param_fragment.size() << " stored)" << endl;
	str << "+ ";
	for (int jj=0;jj<(int)param_fragment.size();++jj) {
		str << param_fragment[jj]->getIdParam() << "(" << size_fragment[jj] << " bits)" << "\t";
	}
	str << endl;
	str << "+ Current load: " << load << "bps" << endl;
	str << "++++++++++++++++++++++++++++++" << endl;

	if (toPrint){
		cout << str.str();
	}


	// ostringstream str;
	// cout << "++++++++++ Frame " << id_frame << " ++++++++++" << endl;
	// cout << "+ Freq: " << freq_transmission << " Hz / Period: " << 1/freq_transmission*1000 << "ms" << endl;
	// cout << "+ Source: " << calc_source->getIdElem() << endl;
	// cout << "+ Nb calc target: " << nb_target << " (" << calc_target.size() << " stored)" << endl;
	// cout << "+ ";
	// for (int jj=0;jj<calc_target.size();++jj) {
		// cout << calc_target[jj]->getIdElem() << "\t";
	// }
	// cout << endl;
	// cout << "+ Nb fragments : " << nb_fragment << " (" << param_fragment.size() << " stored)" << endl;
	// cout << "+ ";
	// for (int jj=0;jj<param_fragment.size();++jj) {
		// cout << param_fragment[jj]->getIdParam() << "(" << size_fragment[jj] << " bits)" << "\t";
	// }
	// cout << endl;
	// cout << "+ Current load: " << load << "bps" << endl;
	// cout << "++++++++++++++++++++++++++++++" << endl;

	return str.str();
}


bool compareFrameLoads(Frame const* f1, Frame const* f2) {
	return (f1->getLoad() < f2->getLoad());
}


bool smallerFrameFreq(Frame const* f1, Frame const* f2) {
	return (f1->getFreqTransmission() < f2->getFreqTransmission());
}


bool greaterFrameFreq(Frame const* f1, Frame const* f2) {
	return smallerFrameFreq(f2, f1);
}


int numObj(vector<Frame> const * const vecObj, const string& id) {
	int num = -1;
	for (int ii=0;ii<(int)(*vecObj).size();++ii) {
		if ((*vecObj)[ii].getIdFrame().compare(id) == 0) num = ii;
	}
	// if (num < 0) cerr << "Object " << id << " was not found in the stored objects." << endl;
	return num;
}


/*
vector< vector<bool> > matrix_power(vector< vector<bool> > mat, int pow) {
	if (mat.size()==0) cerr << "Cannot play with empty matrices." << endl;
	else if (mat.size() != mat[0].size()) cerr << "Cannot play with non square matrices." << endl;

	int size_mat = mat.size();
	vector< vector<bool> > res = mat;
	for (int order=1;order<pow;++order) {
		// do res <- res*mat
		vector< vector<bool> > buf = res;
		for (int i=0;i<size_mat;++i) {
			for (int j=0;j<size_mat;++j) {
				for (int k=0;k<size_mat;++k) {
					res[i][j] = res[i][j] + buf[i][k]*mat[k][j];
				}
			}
		}
	}

	return res;
}*/


vector< vector<bool> > matrix_power(vector< vector<bool> > mat, int pow) {
	if (mat.size()==0) cerr << "Cannot play with empty matrices." << endl;
	else if (mat.size() != mat[0].size()) cerr << "Cannot play with non square matrices." << endl;

	int size_mat = mat.size();
	vector< vector<bool> > res = mat;
	for (int order=1;order<pow;++order) {
		// do res <- res*mat
		vector< vector<bool> > buf = res;
		for (int i=0;i<size_mat;++i) {
			for (int j=i;j<size_mat;++j) {
				for (int k=0;(k<size_mat)&&(!res[i][j]);++k) {
					res[i][j] = (buf[i][k]&&mat[k][j]);
				}
				res[j][i] = res[i][j];
			}
		}
	}

	return res;
}


vector< vector<int> > matrix_power(vector< vector<int> > mat, int pow) {
	if (mat.size()==0) cerr << "Cannot play with empty matrices." << endl;
	else if (mat.size() != mat[0].size()) cerr << "Cannot play with non square matrices." << endl;

	int size_mat = mat.size();
	//int qtd1 = 0;
	vector< vector<int> > res = mat;
	for (int order = 1; order < pow; ++order) {
		// do res <- res*mat
		vector< vector<int> > buf = res;	// implicit bool to int
		for (int i = 0; i < size_mat; ++i) {
			for (int j = i; j < size_mat; ++j) {
				for (int k = 0; (k < size_mat) && (res[i][j] == 0); ++k) {
					res[i][j] = (buf[i][k]*mat[k][j]);
				}
				res[j][i] = res[i][j];
                //if(res[i][j] == 1) {
                  //  if(i == j) qtd1++;
                    //else qtd1 += 2;
                //}
			}
		}
		//if(qtd1 == size_mat*size_mat) return res;
        //qtd1 = 0;
	}
	return res;
}

