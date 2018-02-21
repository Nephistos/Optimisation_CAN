#include "IOManagement.h"


/// File scope variables. But they are actually defined in const_tool.h as extern, thus they are used
/// as global variables
int RESOLUTION_MODE = 0;

char DF_PARAM_CALC_IO[CSTRINGSIZE];
char DF_FRAME_CALC_IO[CSTRINGSIZE];
char DF_SUBNET_INI[CSTRINGSIZE];
char DF_USER_TOPO[CSTRINGSIZE];

char DF_NEW_CALC[CSTRINGSIZE];
char DF_NEW_FRAME[CSTRINGSIZE];
char DF_REST_FRAME[CSTRINGSIZE];

int nb_SN_max = 0;					// in case of automatic generation of network (multistart)
int nb_max_calc_per_SN = 0;
int nb_max_SN_per_calc = 0;
int nb_max_SN_FD_per_calc = 0;
vector<double> max_load_supported;		// in bits/s
vector<double> max_load_allowed;
int nb_bridge = 0;
int bridge_nb_max_connector = 0;
int bridge_nb_max_connector_FD =0;

double price_SN = 0.;
double price_connector = 0.;
double price_bridge = 0.;

int size_encapsulation = 0;
int nb_byte_max = 0;
bool no_par_astride_bytes = false;

int nb_it_spreadPath = 0;
int mode_opt_rout=0;
int path_order_max = 0;

int size_tabu = 0;
int nb_it_tabu = 0;
bool allow_stochastic = false;
int nb_choice_rem = 0;
int nb_choice_add = 0;
int nb_it_max_without_change = 0;

double Tmax = 0.;
double Tfactor = 0.;
double param_exp = 0.;
int nb_it_SA = 0;

int nb_it_gen = 0;
int nb_multi_start = 0;
bool do_multistart = false;

int nb_network_out = 0;
bool printToFile = false;
int Nombre_Calc = 0; //HERE
/// End of global variables



bool cfgFromFile() {
	bool valid_cfg = true;
	vector< vector<string> > cfgContent;
	int line = -1;

	cfgContent = readDataFile(string(CFG_FILE), '\t');

	// MODE DE RESOLUTION
	RESOLUTION_MODE = atoi(cfgContent[5][0].c_str());
	if ((RESOLUTION_MODE != 1) && (RESOLUTION_MODE != 2) && (RESOLUTION_MODE != 3) ) valid_cfg = false;

	// FICHIERS DE DONNEES
	line = 8;
	if (cfgContent[line][0].size() < CSTRINGSIZE)
		cfgContent[line][0].copy(DF_PARAM_CALC_IO, cfgContent[line][0].size());
	else
		valid_cfg = false;
	line++;
	if (cfgContent[line][0].size() < CSTRINGSIZE)
		cfgContent[line][0].copy(DF_FRAME_CALC_IO, cfgContent[line][0].size());
	else
		valid_cfg = false;
	line++;
	if (cfgContent[line][0].size() < CSTRINGSIZE)
		cfgContent[line][0].copy(DF_SUBNET_INI, cfgContent[line][0].size());
	else
		valid_cfg = false;
	line++;
	if (cfgContent[line][0].size() < CSTRINGSIZE)
		cfgContent[line][0].copy(DF_USER_TOPO, cfgContent[line][0].size());
	else
		valid_cfg = false;

    line++;
    if (cfgContent[line][0].size() < CSTRINGSIZE)
		cfgContent[line][0].copy(DF_REST_FRAME, cfgContent[line][0].size());
	else
		valid_cfg = false;
   /* line++;
    if (cfgContent[line][0].size() < CSTRINGSIZE)
		cfgContent[line][0].copy(DF_NEW_CALC, cfgContent[line][0].size());
	else
		valid_cfg = false;
    line++;
    if (cfgContent[line][0].size() < CSTRINGSIZE)
		cfgContent[line][0].copy(DF_NEW_FRAME, cfgContent[line][0].size());
	else
		valid_cfg = false;*/

	// PARAMETRES D'ARCHITECTURE
	line+=3;
	nb_SN_max = atoi(cfgContent[line][0].c_str());
	line++;
	//HERE
	Nombre_Calc = atoi(cfgContent[line][0].c_str());
	line++;
	//HERE
	nb_max_calc_per_SN = atoi(cfgContent[line][0].c_str());
	line++;
	nb_max_SN_per_calc = atoi(cfgContent[line][0].c_str());
	line++;
	nb_max_SN_FD_per_calc = atoi(cfgContent[line][0].c_str());
	line++;
	double inutile;
	inutile = atof(cfgContent[line][0].c_str());
	line++;
	inutile = atof(cfgContent[line][0].c_str());
	line++;
	nb_bridge = atoi(cfgContent[line][0].c_str());
	line++;
	bridge_nb_max_connector = atoi(cfgContent[line][0].c_str());
	line++;
	bridge_nb_max_connector_FD = atoi(cfgContent[line][0].c_str());

	// PRIX
	line+=3;
	price_SN = atof(cfgContent[line][0].c_str());
	line++;
	price_connector = atof(cfgContent[line][0].c_str());
	line++;
	price_bridge = atof(cfgContent[line][0].c_str());

	// CONSTITUTION DES TRAMES
	line+=3;
	size_encapsulation = atoi(cfgContent[line][0].c_str());
	line++;
	nb_byte_max = atoi(cfgContent[line][0].c_str());
	line++;
	if (atoi(cfgContent[line][0].c_str()) == 1)
		no_par_astride_bytes = true;
	else
		no_par_astride_bytes = false;

	// OPTIMISATION ROUTAGE
	line+=3;
	nb_it_spreadPath = atoi(cfgContent[line][0].c_str());
	line++;
	mode_opt_rout = atoi(cfgContent[line][0].c_str());
	line++;
	if (atoi(cfgContent[line][0].c_str()) == -1)
		path_order_max = nb_SN_max;
	else if (atoi(cfgContent[line][0].c_str()) > 0)
		path_order_max = atoi(cfgContent[line][0].c_str());
	else
		valid_cfg = false;

	// OPTIMISATION TOPOLOGIE
	line+=3;
	size_tabu = atoi(cfgContent[line][0].c_str());
	line++;
	nb_it_tabu = atoi(cfgContent[line][0].c_str());
	line++;
	if (atoi(cfgContent[line][0].c_str()) == 1)
		allow_stochastic = true;
	else
		allow_stochastic = false;
	line++;
	nb_choice_rem = atoi(cfgContent[line][0].c_str());
	line++;
	nb_choice_add = atoi(cfgContent[line][0].c_str());
	line++;
	nb_it_max_without_change = atoi(cfgContent[line][0].c_str());

	// OPTIMISATION MESSAGERIE
	line+=3;
	Tmax = atof(cfgContent[line][0].c_str());
	line++;
	Tfactor = atof(cfgContent[line][0].c_str());
	line++;
	param_exp = atof(cfgContent[line][0].c_str());
	line++;
	nb_it_SA = atoi(cfgContent[line][0].c_str());

	// TABOU MULTISTART
	line+=3;
	nb_it_gen = atoi(cfgContent[line][0].c_str());
	line++;
	nb_multi_start = atoi(cfgContent[line][0].c_str());
	line++;
	if (atoi(cfgContent[line][0].c_str()) == 1)
		do_multistart = true;
	else
		do_multistart = false;

	// SORTIE
	line+=3;
	nb_network_out = atoi(cfgContent[line][0].c_str());
	line++;
	if (atoi(cfgContent[line][0].c_str()) == 1)
		printToFile = true;
	else
		printToFile = false;



	return valid_cfg;
}


void printcfg() {
	cout << endl;
	cout << "-------------------------------------------------------------" << endl;
	cout << "RESOLUTION_MODE: " << RESOLUTION_MODE << endl;
	cout << endl;
	cout << "DF_PARAM_CALC_IO: " << string(DF_PARAM_CALC_IO) << endl;
	cout << "DF_FRAME_CALC_IO: " << string(DF_FRAME_CALC_IO) << endl;
	cout << "DF_SUBNET_INI: " << string(DF_SUBNET_INI) << endl;
	cout << "DF_USER_TOPO: " << string(DF_USER_TOPO) << endl;
	cout << endl;
	cout << "DF_NEW_CALC: " << string(DF_NEW_CALC) << endl;
	cout << "DF_NEW_FRAME: " << string(DF_NEW_FRAME) << endl;
	cout << endl;
	cout << "nb_SN_max: " << nb_SN_max << endl;
	cout << "nb_max_calc_per_SN: " << nb_max_calc_per_SN << endl;
	cout << "nb_max_SN_per_calc: " << nb_max_SN_per_calc << endl;
	cout << "nb_max_SN_FD_per_calc: " << nb_max_SN_FD_per_calc << endl;
	//cout << "max_load_supported: " << max_load_supported << endl;
	//cout << "max_load_allowed: " << max_load_allowed << endl;
	cout << "nb_bridge: " << nb_bridge << endl;
	cout << "bridge_nb_max_connector: " << bridge_nb_max_connector << endl;
	cout << "bridge_nb_max_connector_FD: " << bridge_nb_max_connector_FD << endl;
	cout << endl;
	cout << "price_SN: " << price_SN << endl;
	cout << "price_connector: " << price_connector << endl;
	cout << "price_bridge: " << price_bridge << endl;
	cout << endl;
	cout << "size_encapsulation: " << size_encapsulation << endl;
	cout << "nb_byte_max: " << nb_byte_max << endl;
	cout << "no_par_astride_bytes: " << no_par_astride_bytes << endl;
	cout << endl;
	cout << "nb_it_spreadPath: " << nb_it_spreadPath << endl;
	cout << "path_order_max: " << path_order_max << endl;
	cout << endl;
	cout << "size_tabu: " << size_tabu << endl;
	cout << "nb_it_tabu: " << nb_it_tabu << endl;
	cout << "allow_stochastic: " << allow_stochastic << endl;
	cout << "nb_choice_rem: " << nb_choice_rem << endl;
	cout << "nb_choice_add: " << nb_choice_add << endl;
	cout << "nb_it_max_without_change: " << nb_it_max_without_change << endl;
	cout << endl;
	cout << "Tmax: " << Tmax << endl;
	cout << "Tfactor: " << Tfactor << endl;
	cout << "param_exp: " << param_exp << endl;
	cout << "nb_it_SA: " << nb_it_SA << endl;
	cout << endl;
	cout << "nb_it_gen: " << nb_it_gen << endl;
	cout << "nb_multi_start: " << nb_multi_start << endl;
	cout << "do_multistart: " << do_multistart << endl;
	cout << endl;
	cout << "nb_network_out: " << nb_network_out << endl;
	cout << "printToFile: " << printToFile << endl;
	cout << "-------------------------------------------------------------" << endl;
	cout << endl;
}

/*int importNewCalc(vector<Calc>* calcData, vector<Parameter>* paramData, vector<Frame>* frameData,
					   vector<SubNet>* snData) {

    string dataFileCalc = DF_NEW_CALC;
    string dataFileFrame = DF_NEW_FRAME;
    int nbframe = frameData->size();

     vector< vector<string> > dataTableFrame = readDataFile(dataFileFrame, ';');

     if (dataTableFrame.empty()) {
		cerr << "No data found in [" << dataFileFrame << "]" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	int nbFrame = dataTableFrame.size() - 2;
	int nbCalc;
	if(nbFrame >= 0) {

        bool endCalcList = false;

		vector< vector<int> > calc_rec_param_num;		// for each Calc, indices of the frames to be recieved
		for (int icol = nb_non_calc_column_fcioFile				;
			 icol < (int)dataTableFrame[0].size() && !endCalcList; ++icol) {

			if (dataTableFrame[0][icol].compare("") == 0) {
				endCalcList = true;
			} else {
                string id_calc = dataTableFrame[0][icol];
				int nb_param_trans_ = 0;
				int nb_param_rec_ = 0;
				vector<int> noTarget;
				for (int iFrame = 0; iFrame < nbFrame; ++iFrame) {

					int noLine = iFrame + nb_line_header_fciofile+1;
                    //cout << noLine << " "<< icol << dataTableFrame[noLine][icol] <<  endl;
						//getchar();
					if (( (dataTableFrame[noLine])[icol].compare("T") == 0 ) ||
						( (dataTableFrame[noLine])[icol].compare("T\r") == 0 ) ) {
						nb_param_trans_++;
					}
					if (( (dataTableFrame[noLine])[icol].compare("R") == 0 ) ||
						( (dataTableFrame[noLine])[icol].compare("R\r") == 0 ) ) {
						nb_param_rec_++;
						noTarget.push_back(iFrame);

					}
				}
				int nb_connector = atoi(dataTableFrame[1][icol].c_str());
				int nb_trames = atoi(dataTableFrame[2][icol].c_str());
				cout << nb_connector << endl;
                calc_rec_param_num.push_back(noTarget);
				if (icol - nb_non_calc_column_fcioFile +1 > (int)calcData->size()) {
                    //cout << id_calc << endl;
                    calcData->push_back(Calc(id_calc, false, nb_param_trans_, nb_param_rec_, nb_connector, nb_trames));
                    //cout << "+1" << endl;
				} else {
                    nb_param_rec_ += (*calcData)[icol - nb_non_calc_column_fcioFile].getNbParamTransmitted();
                    nb_param_trans_ += (*calcData)[icol - nb_non_calc_column_fcioFile].getNbParamRecieved();
                    (*calcData)[icol - nb_non_calc_column_fcioFile].setNbParamTransmitted(nb_param_trans_);
                    (*calcData)[icol - nb_non_calc_column_fcioFile].setNbParamRecieved(nb_param_rec_);
				}
			}
		}

		nbCalc = calcData->size();


		// Creation of the Frame objects (and Parameter objects)
		bool endframe = false;
		for (int iline=nb_line_header_fciofile ; (iline<(int)dataTableFrame.size()) && !endframe ; ++iline) {

			string id_frame = dataTableFrame[iline][0];
			if (id_frame.compare("") == 0) {
				endframe = true;
				break;
			}

			Calc* source = nullptr;
			vector<Calc*> calc_target;

			for (int icalc=0 ; icalc<nbCalc ; ++icalc) {

				int icol = nb_non_calc_column_fcioFile + icalc;

				if (( dataTableFrame[iline][icol].compare("T") == 0 ) ||
					( dataTableFrame[iline][icol].compare("T\r") == 0 ) ) {

					if (source!=nullptr) {
						cerr << "Frame \"" << id_frame << "\" has more than one source calculator defined"
							 << " in file \"" << dataFileFrame << "\". Only one source will be "
							 << "considered." << endl;
					}
					source = &(*calcData)[icalc];
				}
				if (( dataTableFrame[iline][icol].compare("R") == 0 ) ||
					( dataTableFrame[iline][icol].compare("R\r") == 0 ) ) {

					calc_target.push_back(&(*calcData)[icalc]);
				}
			}

			double period = atof( (dataTableFrame[iline])[2].c_str() ) / 1000.;	// converted into s
			double freq;
			if (period > 0) freq = 1./period;
			else freq = 0;
			int size_ = atoi( (dataTableFrame[iline])[3].c_str() ) * 8;
			paramData->push_back(Parameter(id_frame, size_, source, 0));
			for (int itgt=0 ; itgt<(int)calc_target.size() ; ++itgt) {
				paramData->back().addTarget(calc_target[itgt], freq);
			}

			frameData->push_back(Frame(id_frame, freq, source, 0, 0, true, false));
			if (!frameData->back().addParameter(&paramData->back(), true)) {
				cerr << "Problem while creating frame " << id_frame << "." << endl;
			}
		}


		for (int icalc=0;icalc<nbCalc;++icalc) {
			vector<Parameter*> param_recieved_ = (*calcData)[icalc].getParamRecieved();
			vector<double> freq_request_ = (*calcData)[icalc].getFreqRequest();
			for (int iTgt=0;iTgt<(int)calc_rec_param_num[icalc].size();++iTgt) {
				param_recieved_.push_back( &(*paramData)[calc_rec_param_num[icalc][iTgt]] );
				freq_request_.push_back( ((*frameData)[calc_rec_param_num[icalc][iTgt]]).getFreqTransmission() );
				// because freq requested of the parameter = freq of the frame here
			}
			(*calcData)[icalc].setParamRecieved(param_recieved_);
			(*calcData)[icalc].setFreqRequest(freq_request_);
		}
	} else {
		cerr << "The specified datafile [" << dataFileFrame << "] could not be properly imported" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}


	/// Fix  the routing of the specified frames
	int noColRouting = nb_non_calc_column_fcioFile + nbCalc + 1;

	// Creation of the SubNet objects
	int nocol;
	int isn = 0;


	for (int iline = nb_line_header_fciofile ; iline < (int)dataTableFrame.size() ; ++iline) {
		int ifr = iline - nb_line_header_fciofile +nbframe;
		// Read the filled SN only if the routing is fixed
		if ((int)dataTableFrame[iline].size() > noColRouting) {

			if ( (dataTableFrame[iline][noColRouting].compare("1") == 0) ||
				 (dataTableFrame[iline][noColRouting].compare("1\r") == 0) ) {

				if (!(*frameData)[ifr].isContentFixed()) {
					cerr << "Frame " << (*frameData)[ifr].getIdFrame() << "'s content is not fixed."
						 << endl;
				} else {
					(*frameData)[ifr].fixRouting();
					isn = 0;
					vector<SubNet*> snfilled;
					for (nocol=noColRouting+1									;
						(nocol<(int)dataTableFrame[1].size()) && (isn<nb_SN_max)	;
						++nocol, ++isn											) {

						double loadOnSN = atof( dataTableFrame[iline][nocol].c_str() );
						if (loadOnSN>0.001) {
							snfilled.push_back(&(*snData)[isn]);
						}
					}
					bool force = true;
					(*frameData)[ifr].setSNFilled(snfilled, force);
				}
			}
		}
	}

    return 0;
}*/

int importFcioFileData(vector<Calc>* calcData, vector<Parameter>* paramData, vector<Frame>* frameData,
					   vector<SubNet>* snData														) {

	// Process DF_FRAME_CALC_IO
	calcData->reserve(200);
	paramData->reserve(5000);
	frameData->reserve(500);

	//int nbTransmission = 0;
	int nbCalc;
	int nbFrame;

	string dataFilePath = DF_FRAME_CALC_IO;

	vector< vector<string> > dataTableFcioFile = readDataFile(dataFilePath, ';');
	// 1 line = 1 frame

	if (dataTableFcioFile.empty()) {
		cerr << "No data found in [" << dataFilePath << "]" << endl; getchar();
		system("PAUSE");
		exit(EXIT_FAILURE);

	}


	nbFrame = dataTableFcioFile.size() - nb_non_frame_line_fcioFile;

	if (nbFrame >= 0) {

		// Creation of the Calc objects
		bool endCalcList = false;
		vector< vector<int> > calc_rec_param_num;		// for each Calc, indices of the frames to be recieved
		for (int icol = nb_non_calc_column_fcioFile				;
			 icol<(int)dataTableFcioFile[0].size() && !endCalcList	;
			 ++icol												) {

			if (dataTableFcioFile[0][icol].compare("") == 0) {
				endCalcList = true;
			} else {
				string id_calc = dataTableFcioFile[0][icol];
				int nb_param_trans_ = 0;
				int nb_param_rec_ = 0;
				vector<int> noTarget;
				for (int iFrame=0;iFrame<nbFrame;++iFrame) {
					int noLine = iFrame + nb_line_header_fciofile;
					if (( (dataTableFcioFile[noLine])[icol].compare("T") == 0 ) ||
						( (dataTableFcioFile[noLine])[icol].compare("T\r") == 0 ) ) {
						nb_param_trans_++;
					}
					if (( (dataTableFcioFile[noLine])[icol].compare("R") == 0 ) ||
						( (dataTableFcioFile[noLine])[icol].compare("R\r") == 0 ) ) {
						nb_param_rec_++;
						noTarget.push_back(iFrame);
					}
				}
				int nb_connector = atoi(dataTableFcioFile[1][icol].c_str());
				int nb_connectorFD = atoi(dataTableFcioFile[2][icol].c_str());
				int nb_frame_bridge = atoi(dataTableFcioFile[3][icol].c_str());
				calcData->push_back(Calc(id_calc, false, nb_param_trans_, nb_param_rec_, nb_connector, nb_connectorFD, nb_frame_bridge));

				calc_rec_param_num.push_back(noTarget);
			}
		}
		nbCalc = calcData->size();

		// Creation of the Frame objects (and Parameter objects)
		bool endframe = false;
		for (int iline=nb_line_header_fciofile ; (iline<(int)dataTableFcioFile.size()) && !endframe ; ++iline) {

			string id_frame = dataTableFcioFile[iline][0];
			if (id_frame.compare("") == 0) {
				endframe = true;
				break;
			}

			Calc* source = nullptr;
			vector<Calc*> calc_target;

			for (int icalc = 0; icalc < nbCalc; ++icalc) {

				int icol = nb_non_calc_column_fcioFile + icalc;

				if (( dataTableFcioFile[iline][icol].compare("T") == 0 ) ||
					( dataTableFcioFile[iline][icol].compare("T\r") == 0 ) ) {

					if (source!=nullptr) {
						cerr << "Frame \"" << id_frame << "\" has more than one source calculator defined"
							 << " in file \"" << dataFilePath << "\". Only one source will be "
							 << "considered." << endl;
					}
					source = &(*calcData)[icalc];
				}
				if (( dataTableFcioFile[iline][icol].compare("R") == 0 ) ||
					( dataTableFcioFile[iline][icol].compare("R\r") == 0 ) ) {

					calc_target.push_back(&(*calcData)[icalc]);
				}
			}

			double period = atof( (dataTableFcioFile[iline])[2].c_str() ) / 1000.;	// converted into s
			double freq;
			if (period > 0) freq = 1./period;
			else freq = 0;
			int size_ = atoi( (dataTableFcioFile[iline])[3].c_str() ) * 8;
			paramData->push_back(Parameter(id_frame, size_, source, 0));
			for (int itgt=0 ; itgt<(int)calc_target.size() ; ++itgt) {
				paramData->back().addTarget(calc_target[itgt], freq);
			}

			frameData->push_back(Frame(id_frame, freq, source, 0, 0, true, false));
			if (!frameData->back().addParameter(&paramData->back(), true)) {
				cerr << "Problem while creating frame " << id_frame << "." << endl;
			}
		}

		for (int icalc=0;icalc<nbCalc;++icalc) {
			vector<Parameter*> param_recieved_;
			vector<double> freq_request_;

			for (int iTgt=0;iTgt<(int)calc_rec_param_num[icalc].size();++iTgt) {
				param_recieved_.push_back( &(*paramData)[calc_rec_param_num[icalc][iTgt]] );
				freq_request_.push_back( ((*frameData)[calc_rec_param_num[icalc][iTgt]]).getFreqTransmission() );
				// because freq requested of the parameter = freq of the frame here
			}
			(*calcData)[icalc].setParamRecieved(param_recieved_);
			(*calcData)[icalc].setFreqRequest(freq_request_);
		}

	} else {
		cerr << "The specified datafile [" << dataFilePath << "] could not be properly imported" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);

	}


	/// Fix  the routing of the specified frames
	int noColRouting = nb_non_calc_column_fcioFile + nbCalc + 1;

	// Creation of the SubNet objects
	int nocol;
	int isn = 0;
	for (nocol=noColRouting+1 ; nocol<(int)dataTableFcioFile[1].size() ; ++nocol, ++isn) {
		string id_SN;
		if (dataTableFcioFile[0][nocol].compare("") != 0) {
			id_SN = dataTableFcioFile[0][nocol];
		} else {
			id_SN = "SN" + static_cast<ostringstream*>( &(ostringstream() << isn) )->str();
		}
        if(atof(dataTableFcioFile[2][nocol].c_str()) == 0.0) cout << "Attention: the charge of the subnets its read only with a point '.'" << endl;
		max_load_supported.push_back(atof(dataTableFcioFile[3][nocol].c_str()));
		max_load_allowed.push_back(atof(dataTableFcioFile[2][nocol].c_str()));
        bool isFD;
        if (atoi(dataTableFcioFile[1][nocol].c_str())==1){isFD=true;}
        else {isFD=false;}
		snData->push_back(SubNet(isn,id_SN, isFD));
		max_load_allowed[isn]=100;
	}
	if (isn > nb_SN_max) {
		cerr << "There are more initial subnetworks than the max number allowed." << endl;
		system("PAUSE");
		exit(EXIT_SUCCESS);
	}
	for ( ; isn<nb_SN_max ; ++isn) {
		string id_SN = "SN" + static_cast<ostringstream*>( &(ostringstream() << isn) )->str();
		snData->push_back(SubNet(isn,id_SN));
	}

	for (int iline=nb_line_header_fciofile ; iline<(int)dataTableFcioFile.size() ; ++iline) {
		int ifr = iline - nb_line_header_fciofile;

		// Read the filled SN only if the routing is fixed
		if ((int)dataTableFcioFile[iline].size()>noColRouting) {

			if ( (dataTableFcioFile[iline][noColRouting].compare("1") == 0) ||
				 (dataTableFcioFile[iline][noColRouting].compare("1\r") == 0) ) {

				if (!(*frameData)[ifr].isContentFixed()) {
					cerr << "Frame " << (*frameData)[ifr].getIdFrame() << "'s content is not fixed."
						 << endl;
				} else {
					(*frameData)[ifr].fixRouting();
					isn = 0;
					vector<SubNet*> snfilled;
					for (nocol=noColRouting+1									;
						(nocol<(int)dataTableFcioFile[1].size()) && (isn<nb_SN_max)	;
						++nocol, ++isn											) {

						double loadOnSN = atof( dataTableFcioFile[iline][nocol].c_str() );
						if (loadOnSN>0.001) {
							snfilled.push_back(&(*snData)[isn]);
						}
					}
					bool force = true;
					(*frameData)[ifr].setSNFilled(snfilled, force);
				}
			}
		}
	}

	return 0;
}


int importPcioFileData(vector<Calc>* calcData, vector<Parameter>* paramData, vector<Frame>* frameIni) {

	calcData->reserve(200);
	paramData->reserve(5000);
	frameIni->reserve(500);

	int nbTransmission = 0;
	int nbCalc = 0;
	int nbParam;
	int nbFrame;

	// Process DF_PARAM_CALC_IO
	string dataFilePath = DF_PARAM_CALC_IO;
	// Read the data
	vector< vector<string> > dataTablePcioFile = readDataFile(dataFilePath, ';');
	// 1 line = 1 transmission

	if (dataTablePcioFile.empty()) {
		cerr << "No data found in [" << dataFilePath << "]" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	// Initial number of parameters (includes "NULL" parameters that will be deleted)
	nbTransmission = dataTablePcioFile.size() - nb_non_transmission_line_pcioFile;
	// Number of calculators
	if (dataTablePcioFile.size()>0) {
		nbCalc = dataTablePcioFile[0].size() - nb_non_calc_column_pcioFile;
	}

	cout << "Initial number of fragments transmission : " << nbTransmission << endl;
	cout << "Initial number of calculators: " << nbCalc << endl;


	if (nbTransmission > 0) {

		// Create the Calculators (the ones in the datafile plus the bridges if any)
		// Bridges are created as normal calculators here. They will be configured later on.
		int line = 0;
		string id_provi = "Calc_";
		calcData->resize(nbCalc+nb_bridge,Calc(id_provi, false, nb_max_SN_per_calc, nb_max_SN_FD_per_calc));
		for (int ical=0;ical<nbCalc;++ical) {
			(*calcData)[ical].setIdCalc(dataTablePcioFile[line][ical+nb_non_calc_column_pcioFile]);
			(*calcData)[ical].setNbMaxConnector(atoi(dataTablePcioFile[line+1][ical+nb_non_calc_column_pcioFile].c_str()) );
			(*calcData)[ical].setNbMaxConnectorFD(atoi(dataTablePcioFile[line+2][ical+nb_non_calc_column_pcioFile].c_str()) ); //HERE
		}

		// Create the Parameters
		paramData->resize(nbTransmission,Parameter("NULL", 0, nullptr, 0));
		nbParam = 0;
		frameIni->resize(nbTransmission, Frame("NULL", 0., nullptr, 0, 0));
		nbFrame = 0;

		for (int iTrans=2;iTrans<nbTransmission+1;++iTrans) {	// skip the 1 line header
			// The name of the candidate Parameter
			string id_param = dataTablePcioFile[iTrans][4];

			// Process only if the name is not "NULL" or "byte_filling"
			if ((id_param.compare("NULL") != 0) && (id_param.compare("byte_filling") != 0)) {

				// Get the source Calc
				vector<Calc>::iterator sce = calcData->begin();
				while (sce->getIdElem().compare(dataTablePcioFile[iTrans][0]) != 0) { ++sce; }
				Calc* source = &(*sce);

				// Create the Param using its name, its size and its source
				bool fixed_frame_host = ( atoi((dataTablePcioFile[iTrans])[7].c_str()) == 1 );
				(*paramData)[nbParam] = Parameter(id_param, atoi( (dataTablePcioFile[iTrans])[5].c_str() ),
												 source, 0, fixed_frame_host);

				// Get the targets name and frequency request
				vector<Calc*> calc_target;
				vector<double> freq_request;
				for (int ical=0;ical<nbCalc;++ical) {
					int col = ical+nb_non_calc_column_pcioFile;
					if ( (dataTablePcioFile[iTrans][col].compare("") != 0)
						 && (dataTablePcioFile[iTrans][col].compare("_") != 0)) {	// it is a target
						// For the reason why "_": see the split functions
						calc_target.push_back(&(*calcData)[ical]);

						double period_request = atof(dataTablePcioFile[iTrans][col].c_str()) / 1000.;
						if (period_request == 0) {
							freq_request.push_back(0);
						} else {
							freq_request.push_back(1./period_request);
						}

						// Update the target Calc
						vector<Parameter*> listParamRecieved = (*calcData)[ical].getParamRecieved();
						vector<double> freq_request_target = (*calcData)[ical].getFreqRequest();
						listParamRecieved.push_back(&(*paramData)[nbParam]);
						freq_request_target.push_back(freq_request.back());
						(*calcData)[ical].setParamRecieved(listParamRecieved);
						(*calcData)[ical].setFreqRequest(freq_request_target);
						(*calcData)[ical].setNbParamRecieved(listParamRecieved.size());

						(*paramData)[nbParam].addTarget(&(*calcData)[ical], freq_request.back());
					}
				}
				// Update the current Parameter
				if (calc_target.size()>0) {
					int ifr = numObj(frameIni, dataTablePcioFile[iTrans][1]);

					if (ifr<0) {		// it is a new frame
						// if (nbFrame>0) (*frameIni)[nbFrame].toString(true);
						nbFrame++;
						ifr = nbFrame-1;
						bool fixed_content = ( atoi((dataTablePcioFile[iTrans])[2].c_str()) == 1 );
						(*frameIni)[ifr] = Frame(dataTablePcioFile[iTrans][1], 0., source, 0, 0,
												 fixed_content);
					}

					// Add the parameter to the frame with the "force=true" call so that Frame::addParam()
					// works even if Frame::fixed_content=true.
					bool addingsuccess = (*frameIni)[ifr].addParameter(&(*paramData)[nbParam], true);

					if (!addingsuccess) {
						cout << "Problem while importing frame \"" << (*frameIni)[ifr].getIdFrame()
							 << "\" (parameter \"" << (*paramData)[nbParam].getIdParam() << "\")." << endl;
					} else {
						if ((*frameIni)[ifr].isContentFixed()) {
							(*paramData)[nbParam].fixHost();
						}
					}
				} else {	// no target: the parameter is removed and a message is printed out
					cerr << "Parameter " << id_param << " has been removed because it had no target."
						 << endl;
					nbParam--;
				}

				nbParam++;
			}

		}

		paramData->resize(nbParam, Parameter("Void", 0, nullptr, 0));
		frameIni->resize(nbFrame, Frame("Void", 0., nullptr, 0, 0));

		cout << "Actual number of parameters to transmit: " << nbParam << endl;

	} else {
		cerr << "The specified datafile [" << dataFilePath << "] could not be properly imported" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	return nbParam;
}


int actualSubNet(vector<SubNet>* sn, vector<Calc>* calcData, vector<Frame>* fr, string dataSNFile, vector< vector<int> > * snToCalcConnection
                 , vector< vector<int> > * calcToCalcConnection) {

	int nbCalc = calcData->size() - nb_bridge;

	vector< vector<string> > snTable = readDataFile(dataSNFile, ';'); //read the archive with the name of ECUS and X

	if (snTable.size()<0) {
		cerr << "No data to initialize the topology/the routing." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}


	// Prepare the list of SN
	int oldnbSN = sn->size();
	sn->resize(nb_SN_max, SubNet(-1,"NULL")); //5 5

	for (int isn=oldnbSN;isn<nb_SN_max;++isn) {
		string id_ = "SN" + static_cast<ostringstream*>( &(ostringstream() << isn) )->str();
		(*sn)[isn] = SubNet(isn, id_);
	}

	// Add the connections between calculators and subnetworks
	for (int ical = 0 ; ical < nbCalc ; ++ical) {

        if(ical < (int)snTable.size()) {
            int icalhere = numObj(calcData, snTable[ical][0]);
            if (icalhere<0) {
                cerr << "Datafile inconsistency: the lists of calculators are different." << endl;
                system("PAUSE");
                exit(EXIT_FAILURE);
            }

            for (int isn=0 ; (isn<(int)snTable[0].size()-1) && (isn<nb_SN_max) ; ++isn) {
                int icol = isn+1;
                if (snTable[ical][icol].compare("X") == 0 ||
                    snTable[ical][icol].compare("B") == 0 ||
                    snTable[ical][icol].compare("X\r") == 0 ||
                    snTable[ical][icol].compare("B\r") == 0	) (*sn)[isn].addCalc(&(*calcData)[icalhere]);
            }
        }
	}

    // Add the connections between calculators and subnetworks
	for (int ical = 0 ; ical < nbCalc ; ++ical) {
        if(ical < (int)snTable.size()) {
            int icalhere = numObj(calcData, snTable[ical][0]);
            if (icalhere<0) {
                cerr << "Datafile inconsistency: the lists of calculators are different." << endl;
                system("PAUSE");
                exit(EXIT_FAILURE);
            }

            int aux = 0;
            for (int isn=0 ; (isn<(int)snTable[0].size()-1) && (isn<nb_SN_max) ; ++isn) {
                int icol = isn+1;
                if (snTable[ical][icol].compare("X") != 0 &&
                    snTable[ical][icol].compare("B") != 0 &&
                    snTable[ical][icol].compare("X\r") != 0 &&
                    snTable[ical][icol].compare("B\r") != 0	) aux++;

                if ((aux == (int)snTable[0].size()-1) || (aux == nb_SN_max) ){
                    int cont = 0;
                    for(int isn = 0; (isn<nb_SN_max); ++isn) {
                        if (cont < (*calcData)[ical].getNbMaxConnector() && (*sn)[nb_SN_max-1-isn].getNbCalc() < nb_max_calc_per_SN) {
                            (*sn)[nb_SN_max-1-isn].addCalc(&(*calcData)[ical]);
                            cont++;
                        }
                    }
                }
            }
        }
	}

  return 0;
}

int importInitSubNet(vector<SubNet>* sn, vector<Calc>* calcData, vector<Frame>* fr, string dataSNFile) {

	int nbCalc = calcData->size() - nb_bridge;

	vector< vector<string> > snTable = readDataFile(dataSNFile, ';'); //read the archive with the name of ECUS and X

	if (snTable.size()<0) {
		cerr << "No data to initialize the topology/the routing." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}


	// Prepare the list of SN
	int oldnbSN = sn->size();
	sn->resize(nb_SN_max, SubNet(-1,"NULL")); //5 5

	for (int isn=oldnbSN;isn<nb_SN_max;++isn) {
		string id_ = "SN" + static_cast<ostringstream*>( &(ostringstream() << isn) )->str();
		(*sn)[isn] = SubNet(isn,id_);
	}

	// Add the connections between calculators and subnetworks
	for (int ical = 0 ; ical < nbCalc ; ++ical) {

		int icalhere = numObj(calcData, snTable[ical][0]);
		if (icalhere<0) {
			cerr << "Datafile inconsistency: the lists of calculators are different." << endl;
			system("PAUSE");
			exit(EXIT_FAILURE);
		}

		for (int isn=0 ; (isn<(int)snTable[0].size()-1) && (isn<nb_SN_max) ; ++isn) {
			int icol = isn+1;
			if (snTable[ical][icol].compare("X") == 0 ||
				snTable[ical][icol].compare("B") == 0 ||
				snTable[ical][icol].compare("X\r") == 0 ||
				snTable[ical][icol].compare("B\r") == 0	) {

				(*sn)[isn].addCalc(&(*calcData)[icalhere]);
                //cout<<"ical : "<<ical<<endl<<"icol : "<<icol<<endl<<"isn : "<<isn<<endl<<endl;
			}
		}
	}

	// Fix  the routing of the specified frames (iff the frames itselves have a fixed content
	/*int noLine = nbCalc+2;
	while (noLine<snTable.size() && snTable[noLine].size()>1) {
		vector<SubNet*> snfilled;
		int ifr = numObj(fr, snTable[noLine][0]);

		for (int isn=0 ; (isn<snTable[0].size()) && (isn<nb_SN_max) && (isn<snTable[noLine].size()-1) ;
																								++isn) {
			int nocol = isn+1;
			if (snTable[noLine][nocol].compare("1") == 0 ||
				snTable[noLine][nocol].compare("1\r") == 0) {

				if (ifr<0) {
					cerr << "Frame " << snTable[noLine][0] << " was not found in the frame data." << endl;
				} else if (!(*fr)[ifr].isContentFixed()) {
					cerr << "Frame " << snTable[noLine][0] << "'s content is not fixed." << endl;
				} else {
					snfilled.push_back(&(*sn)[isn]);
					(*fr)[ifr].fixRouting();
				}
			}
		}
		bool force = true;
		(*fr)[ifr].setSNFilled(snfilled, force);
		noLine++;
	}*/
	return 0;
}


void importTableConstraintsFrame (const string& nameConstraintsFile, vector<Frame>* fr,
                                  vector< vector<int> > * constFrame) {
    int nbframe = fr->size();

    vector< vector<string> > dataTable = readDataFile(nameConstraintsFile, ';');
    if (dataTable.size() == 0) {
		cerr << "Problem while importing frame constraints file." << endl
			 << "dataTable.size()==0" << endl;
        system("PAUSE");
		exit(EXIT_FAILURE);
	} else if ((int)dataTable[0].size() != nbframe +1) {
		cerr << "Problem while importing user frame constraints file." << endl
			 << "dataTable[0].size() (" << dataTable[0].size() << ") != number of frames (" << nbframe << ") + 1" << endl;
        system("PAUSE");
		exit(EXIT_FAILURE);
	}

	int iline = 1;
	int icol;
	constFrame->resize(nbframe, vector<int>(nbframe, 0));
    while (iline < (int)dataTable.size()) {
        for (int iframe=0 ; iframe<nbframe ; ++iframe) {
			icol = iframe+1;
			if (dataTable[iline][icol].compare("X") == 0 || dataTable[iline][icol].compare("X\n") == 0
                || dataTable[iline][icol].compare("X\r\n") == 0) {
				(*constFrame)[iline-1][iframe] = 1;
				(*fr)[iframe].setRestPath(1);
				(*fr)[iframe].setIdNumFrame(iframe);
			} else {
				(*constFrame)[iline-1][iframe] = 0;
				(*fr)[iframe].setIdNumFrame(iframe);
			}
        }
        iline++;
    }
}


void importUserDefinedTopologyConstraints(const string& nameUserDefTopoFile,
										  vector<Calc> const * const calcList,
										  vector< vector<int> > * calcToCalcConnection,
										  vector< vector<int> > * snToCalcConnection	) {

	int ncalc = calcList->size() - nb_bridge;

	vector< vector<string> > dataTable = readDataFile(nameUserDefTopoFile, ';');

	if (dataTable.size()==0) {
		cerr << "Problem while importing user data topology constraints file." << endl
			 << "dataTable.size()==0" << endl;
        system("PAUSE");
		exit(EXIT_FAILURE);
	} else if ((int)dataTable[0].size() != ncalc+1) {
		cerr << "Problem while importing user data topology constraints file." << endl
			 << "dataTable[0].size() != ncalc+1" << endl;
        system("PAUSE");
		exit(EXIT_FAILURE);
	}

	int iline = 0;
	int icol = -1;

	vector<int> calcIndices(ncalc,-1);		// store the index in *calcList of the ical-th calculator
											// of the datafile.
	for (int ical=0 ; ical<ncalc ; ++ical){
		icol = ical+1;
		calcIndices[ical] = numObj(calcList, dataTable[iline][icol]);
		if (calcIndices[ical]<0) {
			cerr << "Problem while importing user data topology constraints file." << endl
				 << "calcIndices[ical]<0" << endl;
            system("PAUSE");
			exit(EXIT_FAILURE);
		}
	}
	++iline;

	/// First part of the data: calc to calc connection constraints
	calcToCalcConnection->resize(ncalc, vector<int>(ncalc, 0));
	int icalc = 0;
	while (dataTable[iline][0].compare("") != 0) {
		icalc = iline-1;
		if ( ((int)dataTable[iline].size() < iline) || (icalc>=ncalc) ) {
			cerr << "Problem while importing user data topology constraints file." << endl
				 << "(dataTable[iline].size() < iline) || (icalc>=ncalc)" << endl;
            system("PAUSE");
			exit(EXIT_FAILURE);
		}

		for (int ical2=0 ; ical2<iline ; ++ical2) {	// only the lower triangle of the datafile matrix
													// is used
			icol = ical2+1;
			if ( (dataTable[iline][icol].compare("1") == 0) ||
				 (dataTable[iline][icol].compare("1\n") == 0) ||
				 (dataTable[iline][icol].compare("1\r\n") == 0)	) {
				(*calcToCalcConnection)[calcIndices[icalc]][calcIndices[ical2]] = 1;
				(*calcToCalcConnection)[calcIndices[ical2]][calcIndices[icalc]] = 1;
			} else if ( (dataTable[iline][icol].compare("-1") == 0) ||
						(dataTable[iline][icol].compare("-1\n") == 0) ||
						(dataTable[iline][icol].compare("-1\r\n") == 0)	) {
				(*calcToCalcConnection)[calcIndices[icalc]][calcIndices[ical2]] = -1;
				(*calcToCalcConnection)[calcIndices[ical2]][calcIndices[icalc]] = -1;
			}
		}

		++iline;
	}

	icalc = iline-1;
	if (icalc<ncalc) {	// checking of the squareness of the matrix
		cerr << "Problem while importing user data topology constraints file." << endl
			 << "icalc<ncalc" << endl;
        system("PAUSE");
		exit(EXIT_FAILURE);
	}

	++iline;	// separation line

	/// Second part of the data: calc to subnetwork connection constraints
	int isn = 0;
	while (iline < (int)dataTable.size()) {
		//icalc = iline-1;
		if ((int)dataTable[iline].size() < ncalc) {
			cerr << "Problem while importing user data topology constraints file." << endl
				 << "dataTable[iline].size() < ncalc" << endl;
            system("PAUSE");
			exit(EXIT_FAILURE);
		}

		if (isn > nb_SN_max) {
			cerr << "Problem while importing user data topology constraints file: too many buses defined"
				 << endl;
            system("PAUSE");
			exit(EXIT_FAILURE);
		}
		vector<int> curSNConnection(ncalc, 0);

		for (int ical2=0 ; ical2<ncalc ; ++ical2) {	// only the lower triangle of the datafile matrix
													// is used
			icol = ical2+1;
			if (dataTable[iline][icol].compare("1") == 0) {
				curSNConnection[calcIndices[ical2]] = 1;
			} else if (dataTable[iline][icol].compare("-1") == 0) {
				curSNConnection[calcIndices[ical2]] = -1;
			}
		}

		snToCalcConnection->push_back(curSNConnection);

		++iline;
		++isn;
	}
}


vector< vector<string> > readDataFile(string dataFilePath, char delim) {

	vector< vector<string> > dataTable;

	ifstream ifs (dataFilePath.c_str(), ifstream::in);

	if (ifs.is_open()) {
		string line;
		while(!ifs.eof()) {
			getline(ifs,line);
			vector<string> tokens;
			split(line, delim, tokens);
			dataTable.push_back(tokens);
		}
		ifs.close();
	} else {
		cerr << "Datafile [" << dataFilePath << "] could not be opened" << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);

	}

	// To delete the last line of the file if empty
	if ( (dataTable.back().size() == 0) ||
		 (dataTable.back()[0].compare ("") == 0) ||
		 (dataTable.back()[0].compare ("\r") == 0) ) {
		dataTable.pop_back();
	}

	return dataTable;

}

//MODE2 ==> Mess
void outputMessagingFile(string name_file_out, vector<Calc>* calcOut, vector<Frame>* frameOut) {
	// string name_file_out = "param_calc_io_out.csv";

	ofstream ofs (name_file_out.c_str(), ofstream::out);

	if (ofs.is_open()) {

		// Write the header
		ofs << "Source;Frame;Fix frame;Frame Period (ms);Parameter;Size (bits);Frame Charge (bps);"
			<< "Fix param in frame;";
		for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {
			ofs << itcal->getIdElem() << ";";
		}
		ofs << endl;

		ofs << "Connector number;";
		for (int i=1 ; i<nb_non_calc_column_pcioFile ; ++i) {
			ofs << ";";
		}
		for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {
			ofs << itcal->getNbMaxConnector() << ";";
		}
		ofs << endl;

//		int nbpar = 0;

		// Write the content
		for (vector<Frame>::iterator itfr=frameOut->begin() ; itfr!=frameOut->end() ; ++itfr) {

			double period = 0.;
			if (itfr->getFreqTransmission() != 0) period = (1./itfr->getFreqTransmission())*1000.;

			stringstream colfr;
			colfr	<< itfr->getCalcSource()->getIdElem() << ";"
					<< itfr->getIdFrame() << ";"
					<< itfr->isContentFixed() << ";"
					<< period << ";";

			stringstream linecontent;
			vector< vector<Parameter*> > frameContent;
			itfr->arrangeParamPlacement(frameContent);

			for (vector< vector<Parameter*> >::iterator itbyte=frameContent.begin() ;
				 itbyte!=frameContent.end()											;
				 ++itbyte															) {

				int size_byte = 0;
				for (vector<Parameter*>::iterator itpar=itbyte->begin()	;
					 itpar!=itbyte->end()								;
					 ++itpar											) {

					linecontent << colfr.str() << (*itpar)->getIdParam() << ";"
								<< (*itpar)->getSize() << ";"
								<< itfr->getLoad() << ";"
								<< (*itpar)->isHostFixed() << ";";

					for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {
						int itgthere = numObj((*itpar)->getCalcTarget(), itcal->getIdElem());
						if (itgthere>=0) {	// the current calc is one of the targets
							double freqtgt = (*itpar)->getFreqTarget()[itgthere];
							if (freqtgt == 0) {
								linecontent << 0;
							} else {
								linecontent << 1/freqtgt*1000;
							}
						}
						linecontent << ";";
					}

					linecontent << endl;

					ofs << linecontent.rdbuf() << flush;

					size_byte += (*itpar)->getSize();
				}

				// Add bits to fill bytes
				int size_byte_filling = 8 - (size_byte%8);
				if (size_byte_filling == 8) size_byte_filling = 0;

				if (size_byte_filling>0) {
					linecontent << colfr.str() << "byte_filling" << ";"
								<< size_byte_filling << ";"
								<< itfr->getLoad() << ";"
								<< 0 << ";";

					for (int ical=0 ; ical<(int)calcOut->size() ; ++ical) {
						linecontent << ";";
					}
					linecontent << endl;

					ofs << linecontent.rdbuf() << flush;
				}
			}
		}

		ofs.close();
	} else {
		cerr << "Cannot access to file " << name_file_out << ". Possible cause: the file is opened in "
			 << "another software." << endl;
	}

}

//MODE2 ==> Synthesis
void outputFrameSynthesisFile(string name_file_out, vector<Calc>* calcOut, vector<Frame>* frameOut,
							  vector<SubNet>* snOut													) {

	ofstream ofs (name_file_out.c_str(), ofstream::out);

	if (ofs.is_open()) {

		// Write the header
		ofs << "Frame Name;Identifier (hex);Period (ms);Size (Bytes);Frame Load (%); Frame Load FD(%); Latency (ms)";
		for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {
			ofs << ";" << itcal->getIdElem();
		}
		ofs << ";;fixed routing";
		for (vector<SubNet>::iterator itsn=snOut->begin() ; itsn!=snOut->end() ; ++itsn)  {
			ofs << ";" << itsn->getIdElem();
		}

		ofs << endl;


		ofs << "Connector number";
        //for (int i=1 ; i<nb_non_calc_column_fcioFile-1 ; ++i) { (LJ)
        for (int i=1 ; i<nb_non_calc_column_fcioFile+2 ; ++i) {
			ofs << ";";
		}
		for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {
			ofs << ";" << itcal->getNbMaxConnector();
		}
		ofs << ";;FD?";
		for (vector<SubNet>::iterator itsn=snOut->begin() ; itsn!=snOut->end() ; ++itsn)  {

			ofs << ";" << itsn->getIsCanFD();
		}

		ofs << endl;

		ofs << "Connector FD number";
		for (int i=1 ; i<nb_non_calc_column_fcioFile+2 ; ++i) {
			ofs << ";";
		}
		for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {
			ofs << ";" << ""; //itcal->getNbMaxConnectorFD();
		}

        ofs << ";;"; //HERE
		for (vector<SubNet>::iterator itsn=snOut->begin() ; itsn!=snOut->end() ; ++itsn)  {
			ofs << ";" << itsn->getLoad();
		}
		ofs << endl;

		ofs << "Max Frames Bridge"; //HERE
		ofs << ";;;;;;";
		for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {
			ofs << ";" << "1000";
		}
		ofs << endl; //HERE

		for (vector<Frame>::iterator itfr=frameOut->begin() ; itfr!=frameOut->end() ; ++itfr) {
			if (!itfr->isFrameEmpty()) {
				double period = 0.;
				if (itfr->getFreqTransmission() != 0) period = (1./itfr->getFreqTransmission())*1000.;

				int size_ = 0;
				vector< vector<Parameter*> > parameterArrangement;
				if (itfr->arrangeParamPlacement(parameterArrangement)) {
					if (parameterArrangement.size()>1) {
						size_ = 8 * parameterArrangement.size();
					} else {
						for (vector< vector<Parameter*> >::iterator itvec=parameterArrangement.begin()	;
							 itvec!=parameterArrangement.end()											;
							 ++itvec																	) {

							for (vector<Parameter*>::iterator itpar=itvec->begin()	;
								 itpar!=itvec->end()								;
								 ++itpar											) {

								size_ += (*itpar)->getSize();
							}
						}
						if (size_%8!=0)
							size_ = size_ + (8 - size_%8);
					}
				}

				stringstream colfr;
				colfr	<< itfr->getIdFrame() << ";"
						<< "?;"
						<< period << ";"
						<< size_/8 << ";"
						<< itfr->getLoad()<<";"
						<< itfr->getLoadFD()<<";"
						<< itfr->getLatency();

				for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {
					colfr << ";";


					if (numObj(itfr->getCalcTarget(), itcal->getIdElem())>=0) {
															// the current calc is one of the targets
						colfr << "R";
					}
					if (itfr->getCalcSource()->getIdElem().compare(itcal->getIdElem()) == 0) {
															// the current calc is the source
						colfr << "T";
					}
				}
				colfr << ";" ;

				// Print the routing
				colfr << ";" << itfr->isRoutingFixed();
				int i = 0;
                double newload;
				for (vector<SubNet>::iterator itsn=snOut->begin() ; itsn!=snOut->end() ; ++itsn)  {
					if (numObj(itfr->getSNFilled(), itsn->getIdElem()) >= 0) {	// the current SN is used
																				// for routing
						/*if(max_load_supported[i] == 500000)
                            colfr << ";" << itfr->getLoad()/max_load_supported[i]*100;
                        else {
                            newload = (16.5*1.1 + 12.5)*0.002 + (10 + itfr->getSize()*8)*1.1*(1/max_load_supported[i]);
                            if(itfr->getSize() < 17) newload += 22/max_load_supported[i];
                            else newload += 27/max_load_supported[i];
                            colfr << ";" << newload*itfr->getFreqTransmission()/1000*100;
                        }*/

                        if(itsn->getIsCanFD()){
                            colfr << ";" << itfr->getLoadFD();
                        }
                        else{
                            colfr << ";" << itfr->getLoad();
                        }
					} else {
						colfr << ";0";
					}
					i++;
				}
				ofs << colfr.rdbuf() << endl;
			}
		}

		ofs.close();
	} else {
		cerr << "Cannot access to file " << name_file_out << ". Possible cause: the file is opened in "
			 << "another software." << endl;
	}

int __first;
}


void outputTopologyFile(string name_file_out, vector<Calc>* calcOut, vector<SubNet>* snOut) {

	ofstream ofs (name_file_out.c_str(), ofstream::out);

	if (ofs.is_open()) {

		//for (vector<SubNet>::iterator itsn=snOut->begin() ; itsn!=snOut->end() ; ++itsn) {
			//ofs << ";" << itsn->getIdElem();
		//}
		//ofs << endl;

		for (vector<Calc>::iterator itcal=calcOut->begin() ; itcal!=calcOut->end() ; ++itcal) {

			ofs << itcal->getIdElem();

			for (vector<SubNet>::iterator itsn=snOut->begin() ; itsn!=snOut->end() ; ++itsn) {

				if ( itsn->containCalc(&(*itcal)) ) {
					ofs << ";X";
				} else {
					ofs << ";";
				}

			}
			ofs << endl;
		}
		ofs.close();
	} else {
		cerr << "Cannot access to file " << name_file_out << ". Possible cause: the file is opened in "
			 << "another software." << endl;
	}
}


vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);

	// On Windows, if the line ends with ';' no empty token is created after. Thus I add a character at
	// the end of the line.
	//cout << ss.str()[ss.str().size()-1] << "\t";
	if (ss.str()[ss.str().size()-1] == ';') {
		string newss = ss.str();
		newss.push_back('_');
		ss.str(newss);
	}

    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

