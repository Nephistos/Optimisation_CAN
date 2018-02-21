#include <string>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}



#include "main.h"


using namespace std;



int main() {

	srand(seed_random_gen);

	tm * timeinfo;
	time_t  timein;
	time_t  timeout;
	char timestr[100];
	char namefile[100];
	time(&timein);
	timeinfo = localtime(&timein);
	strftime(timestr, 100, "%Y-%m-%d_%H:%M:%S", timeinfo);

	strftime(namefile, 100, "%Y_%m_%d_%H_%M_%S", timeinfo);
	ofstream fo;								// target file to print
	streambuf* buffer_fo = fo.rdbuf();			// buffer to output file
	streambuf* buffer_cout = cout.rdbuf();		// buffer to standard output

	cfgFromFile();

	if (printToFile) {
		// Redirecting the standard output
		string out_dir = "out_" + string(namefile);
		if (CreateDirectory(const_cast<char*>(out_dir.c_str()), NULL)
														|| ERROR_ALREADY_EXISTS == GetLastError()) {
			string nameFileOut = "./" + out_dir + "/" + namefile;
			fo.open(nameFileOut, ofstream::out);
			cout.rdbuf(buffer_fo);
		} else {
			cerr << "Could not create or write in directory \"" << out_dir << "\". Output files are "
				 << "written in the same directory as the program." << endl;
			fo.open(namefile, ofstream::out);
			cout.rdbuf(buffer_fo);
		}
	}

	cout << string(timestr) << endl << endl;

	printcfg();

	if (RESOLUTION_MODE == 1) {
		optimTopologyWrapper(namefile);
	}

	if (RESOLUTION_MODE == 2) {
		optimMessageSetWrapper(namefile);
	}

	if( RESOLUTION_MODE == 3) {
        optimNewCalc(namefile);
	}


	time(&timeout);
	timeinfo = localtime(&timeout);
	strftime(timestr, 100, "%Y-%m-%d_%H:%M:%S", timeinfo);

	cout << endl << string(timestr) << endl;

	if (printToFile) {
		// Resetting the standard output
		fo.close();
		cout.rdbuf (buffer_cout);
	}

    system("PAUSE"); //areter sans fermer la fenetre
	return EXIT_SUCCESS;
}


void optimTopologyWrapper(char* date) {

	vector<Calc> calcData;
	vector<Parameter> paramData;
	vector<Frame> frameData;
	vector<SubNet> subNetData;
	vector< vector<int> > calcToCalcConnection;
	vector< vector<int> > snToCalcConnection;

	cout << "[RESOLUTION_MODE " << RESOLUTION_MODE << "]" << endl << endl;

	cout << "Number of iteration for pathframe optimization: " << nb_it_spreadPath << endl << endl;

	/// --- Begin input interface --- ///
	// Import data: Calc, Parameter and Frame
	importFcioFileData(&calcData, &paramData, &frameData, &subNetData);

	// Add bridges at the end of the list of Calc if needed
//	int nbCalc = calcData.size();
	for (int ibdg=0 ; ibdg<nb_bridge ; ++ibdg) {
		string id_bridge = "BDG" +
							static_cast<ostringstream*>( &(ostringstream() << ibdg) )->str();
		Calc bridge(id_bridge, true, 0, 0, bridge_nb_max_connector, bridge_nb_max_connector_FD, 1000);
		calcData.push_back(bridge);
	}

	// Import data: initial subnetworks
	importInitSubNet(&subNetData, &calcData, &frameData, DF_SUBNET_INI);

	// Import data: user defined topology constraints
	calcToCalcConnection.reserve(2*calcData.size());
	snToCalcConnection.reserve(2*nb_SN_max);
	importUserDefinedTopologyConstraints(DF_USER_TOPO, &calcData,
											&calcToCalcConnection, &snToCalcConnection);

	vector< vector<int> > constFrame;
	constFrame.reserve(2*frameData.size());
    importTableConstraintsFrame(DF_REST_FRAME, &frameData, &constFrame);
	/// --- End input interface --- ///


	// Launch the optimization
	queue< vector<Calc> > calcBest;
	queue< vector<SubNet> > subNetBest;
	queue< vector<Frame> > frameBest;
	optimTopologyLauncher(&frameData, &calcData, &subNetData, calcToCalcConnection, snToCalcConnection, constFrame,
						  calcBest, subNetBest, frameBest);


	/// --- Begin output interface --- ///
	// Print out the optimized network
	for (int ii=0;ii<(int)subNetData.size();++ii) {
		subNetData[ii].toString();
	}
	cout << endl;
	cout << "======================================================================" << endl;
	cout << "**********************************************************************" << endl;
	cout << "======================================================================" << endl;
	cout << endl << endl;

	int nb_out = calcBest.size();
	for (int i_out=0 ; !calcBest.empty() ; ++i_out) {
		// Try to output file in the specific run directory
		string out_dir = "out_" + string(date);
		if (CreateDirectory(const_cast<char*>(out_dir.c_str()), NULL)
														|| ERROR_ALREADY_EXISTS == GetLastError()) {
			string nameFileOut = "./" + out_dir + "/frame_calc_io_out_" + string(date)
								 + "_" + patch::to_string(nb_out-i_out) + ".csv";
			// The (nb_out-i_out)-th best topology is accessed using .front(). The best one will be the
			// last printed out.

			outputFrameSynthesisFile(nameFileOut, &calcBest.front(), &frameBest.front(),
									 &subNetBest.front());
			string nameTopoFileOut = "./" + out_dir + "/topology_" + string(date) +
									 "_" + patch::to_string(nb_out-i_out) + ".csv";
			outputTopologyFile(nameTopoFileOut, &calcBest.front(), &subNetBest.front());
		} else {
			cerr << "Could not create or write in directory \"" << out_dir << "\". Output files are "
				 << "written in the same directory as the program." << endl;
			string nameFileOut = "/frame_calc_io_out_" + string(date) + "_" + patch::to_string(nb_out-i_out) + ".csv";
			outputFrameSynthesisFile(nameFileOut, &calcBest.front(), &frameBest.front(),
									 &subNetBest.front());
			string nameTopoFileOut = "/topology_" + string(date) + "_" + patch::to_string(nb_out-i_out) + ".csv";
			outputTopologyFile(nameTopoFileOut, &calcBest.front(), &subNetBest.front());
		}

		double cost = evaluateCost(&calcBest.front(), &subNetBest.front());
		vector<double> loadsArchi = evaluateLoadPerSN(&subNetBest.front());
		double maxload = *max_element(loadsArchi.begin(), loadsArchi.end());

		cout << "Subscript \"_" + patch::to_string(nb_out-i_out) + ".csv\"'s criteria value: "
			 << cost << "E / " << maxload << "bits/s ("
			 << maxload << "%)." << endl; // max supported for CAN

		calcBest.pop();
		subNetBest.pop();
		frameBest.pop();
	}
	/// --- End output interface --- ///


	/// ** BEGIN MULTISTART ** ///
	if (do_multistart) {
		cout << endl << endl;
		cout << "+------------+" << endl;
		cout << "| MULTISTART |"<< endl;
		cout << "+------------+" << endl << endl << endl;

		vector< queue< vector<Calc> > > calcBest_MS;
		vector< queue< vector<SubNet> > > subNetBest_MS;
		vector< queue< vector<Frame> > > frameBest_MS;


		for (int imultistart=0;(imultistart<nb_multi_start) && do_multistart;++imultistart) {

			cout << endl << endl;
			cout << "  Multistart " << imultistart+1 << "/" << nb_multi_start << endl;
			cout << "+------------------+" << endl;

			queue< vector<Calc> > calcBest_MSit;
			queue< vector<SubNet> > subNetBest_MSit;
			queue< vector<Frame> > frameBest_MSit;

			calcBest_MS.push_back(calcBest_MSit);
			subNetBest_MS.push_back(subNetBest_MSit);
			frameBest_MS.push_back(frameBest_MSit);


			// Building a new network from scratch
			vector<Calc> calcCopy;
			vector<Frame> frameCopy;
			vector<SubNet> subNetCopy;
			vector<PathFrame> pathFrameCopy;
			bool newNetworkValid = constructValidNetwork(&calcData, /*&subNetData, */&frameData,
												   &calcCopy, &subNetCopy, &frameCopy, &pathFrameCopy,
												   calcToCalcConnection, snToCalcConnection);

			if (newNetworkValid) {
				optimTopologyLauncher(&frameCopy, &calcCopy, &subNetCopy,
									  calcToCalcConnection, snToCalcConnection, constFrame,
									  calcBest_MS.back(), subNetBest_MS.back(), frameBest_MS.back());
			}

			// Print out the optimized network
			for (int ii=0;ii<(int)subNetCopy.size();++ii) {
				subNetCopy[ii].toString();
			}
		}

		// Among the best topologies visited, stored in calcBest_MS, subNetBest_MS and frameBest_MS, one
		// would like to find the "nb_network_out" best topologies. To get really different topologies,
		// we consider only the set of topologies that consist of the one best topology obtained at each
		// multistart (MS) iteration. First the criteria are compared to find these best topologies, then
		// they are printed out in filesthat end with "_MS_#_.csv" where the character # is i for the i-th
		// best topology until i=nb_network_out.

		vector<Criteria> bestCrit_MS;		// will contain the Criteria-s of the (only one) best topology
											// found from each start of the multistart, ordered by
											// increasing value (ie. from the best to the worst).
		vector<int> i_bestCrit_MS;			// the i-th element will be the multistart iteration number of
											// i-th best criteria

		for (int itMS = 0 ; itMS < nb_multi_start ; ++itMS) {
//			bool isArchiValid;
			// the very best topology is at the end of the queues of frameBest_MS, thus on access to it
			// using .back()

			double costArchi = evaluateCost(&calcBest_MS[itMS].back(), &subNetBest_MS[itMS].back());
			vector<double> loadsArchi = evaluateLoadPerSN(&subNetBest_MS[itMS].back());
			double latencyArchi = evaluateLatency(&frameBest_MS[itMS].back());
			Criteria curCrit(costArchi, loadsArchi, latencyArchi);

			vector<Criteria>::iterator itrank;
			vector<int>::iterator it_i_rank;
			bool rankFound = false;
			for (itrank = bestCrit_MS.begin(), it_i_rank = i_bestCrit_MS.begin()	;
				(itrank != bestCrit_MS.end()) && !rankFound							;
				++itrank, ++it_i_rank												) {
				if (curCrit < *itrank) {
					rankFound = true;
				}
			}

			if (itrank!=bestCrit_MS.begin()) {
				--itrank;		// when the position in found within the for loop, the iteration ends with
								// abn increment, which must be cancelled.
				--it_i_rank;
			}
			bestCrit_MS.insert(itrank, curCrit);
			i_bestCrit_MS.insert(it_i_rank, itMS);
		}

		for (int ii=0 ; ii<(int)bestCrit_MS.size() ; ++ii) {
			cout << "ii: " << bestCrit_MS[ii].getMaxLoads() << " / i_bestCrit_MS: " << i_bestCrit_MS[ii] << endl;
		}

		for (int i_out=0 ; i_out<nb_network_out ; i_out++) {

			// Try to output file in the specific run directory
			string out_dir = "out_" + string(date);
			if (CreateDirectory(const_cast<char*>(out_dir.c_str()), NULL)
															|| ERROR_ALREADY_EXISTS == GetLastError()) {
				string nameFileOut = "./" + out_dir + "/frame_calc_io_out_" + string(date)
									 + "_MS_" + patch::to_string(i_out+1) + ".csv";

                cout << i_out << " " << i_bestCrit_MS[i_out] << " "<< subNetBest_MS.size() << endl;
				outputFrameSynthesisFile(nameFileOut,
										 &calcBest_MS[i_bestCrit_MS[i_out]].back(),
										 &frameBest_MS[i_bestCrit_MS[i_out]].back(),
										 &subNetBest_MS[i_bestCrit_MS[i_out]].back());

				string nameTopoFileOut = "./" + out_dir + "/topology_" + string(date) +
										 "_MS_" + patch::to_string(i_out+1) + ".csv";
				outputTopologyFile(nameTopoFileOut,
								   &calcBest_MS[i_bestCrit_MS[i_out]].back(),
								   &subNetBest_MS[i_bestCrit_MS[i_out]].back());
			} else {
				cerr << "Could not create or write in directory \"" << out_dir << "\". Output files are "
					 << "written in the same directory as the program." << endl;
				string nameFileOut = "/frame_calc_io_out_" + string(date) + "_MS_" + patch::to_string(i_out+1)
									 + ".csv";
				outputFrameSynthesisFile(nameFileOut,
										 &calcBest_MS[i_bestCrit_MS[i_out]].back()	,
										 &frameBest_MS[i_bestCrit_MS[i_out]].back()	,
										 &subNetBest_MS[i_bestCrit_MS[i_out]].back());

				string nameTopoFileOut = "/topology_" + string(date) + "_MS_" + patch::to_string(i_out+1) + ".csv";
				outputTopologyFile(nameTopoFileOut,
								   &calcBest_MS[i_bestCrit_MS[i_out]].back(),
								   &subNetBest_MS[i_bestCrit_MS[i_out]].back());
			}

			double cost = bestCrit_MS[i_out].getCost();
			double maxload = bestCrit_MS[i_out].getMaxLoads();
			cout << "Subscript \"_MS_" + patch::to_string(i_out+1) + ".csv\"'s criteria value: "
				 << cost << "E / " << maxload << "bits/s ("
				 << maxload/500000*100 << "%)." << endl; //max supported for CAN
            cout << nb_network_out << endl;
		}
	}
	/// ** END MULTISTART ** ///
}


void optimTopologyLauncher(vector<Frame>* frList, vector<Calc>* calcList, vector<SubNet>* snList,
						   vector< vector<int> > const& calcToCalcConnection,
						   vector< vector<int> > const& snToCalcConnection,
						   vector< vector<int> > const& constFrame,
						   queue< vector<Calc> > & calcBest, queue< vector<SubNet> > & subNetBest,
						   queue< vector<Frame> > & frameBest										) {

	// Evaluate the initial architecture
	bool isArchiValid;
	if ((int)snList->size() != nb_SN_max) {
		snList->resize(nb_SN_max, SubNet(-1,"NULL"));
		for (int isn=0;isn<nb_SN_max;++isn) {
			string id_newsn = static_cast<ostringstream*>( &(ostringstream() << isn) )->str();
			(*snList)[isn].setIdElem(id_newsn);
			(*snList)[isn].setIdNUMSubNet(isn);
		}
		cout << "The initial subnetwork list has been resized." << endl;
	}

	Criteria valueArchi = evaluateArchi(frList, calcList, snList,
							calcToCalcConnection, snToCalcConnection, constFrame, &isArchiValid);


	if (isArchiValid) {
		cout << "Provided topology is valid." << endl;
	} else {
		cout << "Provided topology is not valid." << endl;
	}

	optimTabu(calcList, snList, frList, calcToCalcConnection, snToCalcConnection, constFrame,
			  calcBest, subNetBest, frameBest									);
}


void optimMessageSetWrapper(char* date) {

	vector<Calc> calcData;
	vector<Parameter> paramData;
	vector<Frame> frameData;
	// vector<SubNet> subNetList;
//	vector<PathFrame> pathFrameData;
	vector< vector<int> > calcToCalcConnection;
	vector< vector<int> > snToCalcConnection;

	cout << "[RESOLUTION_MODE " << RESOLUTION_MODE << "]" << endl << endl;

	cout << "Number of iteration for pathframe optimization: " << nb_it_spreadPath << endl << endl;


	/// --- Begin input interface --- ///
	importPcioFileData(&calcData, &paramData, &frameData);
	// Note that the bridges have been created but with provisional information, they must be configured.

	// Import data: user defined topology constraints
	importUserDefinedTopologyConstraints(DF_USER_TOPO, &calcData,
										 &calcToCalcConnection, &snToCalcConnection);

	// Configure bridges at the end of the list of Calc if needed
	int nbCalc = calcData.size() - nb_bridge;
	for (int ibdg=nbCalc;ibdg<nbCalc+nb_bridge;++ibdg) {
		string id_bridge = "BDG" +
							static_cast<ostringstream*>( &(ostringstream() << (ibdg-nbCalc)) )->str();
		Calc bridge(id_bridge, true, 0, 0, bridge_nb_max_connector,bridge_nb_max_connector_FD, 1000);
		calcData[ibdg] = bridge;
	}
	/// --- End input interface --- ///

	/// --- Begin obtaining an initial message set --- ///
	vector<Frame> frameHeuristic;
	vector<SubNet> subNetIni;
	vector<Calc> calcIni;
	vector<Frame> frameIni;
	vector<PathFrame> pathFrameIni;
	int nbFrame = initFrameFromBFDHeuristic(&paramData, &calcData, &frameHeuristic, &frameData);
	outputMessagingFile("mess.csv", &calcData, &frameHeuristic);
	/// --- End obtaining an initial message set --- ///

	outputFrameSynthesisFile("synthesis.csv", &calcData, &frameHeuristic, &subNetIni);
	cout << "Initial number of frames: " << nbFrame << endl;

    vector< vector<int> > constFrame;

    //HERE
    for(int i = 0; i < (int)frameHeuristic.size(); i++){
        for(int j = 0; j < (int)frameHeuristic.size(); j++)
            cout<< (constFrame)[i][j] << " ";
        cout<<endl;
    }

    for(int i = 0; i < (int)frameHeuristic.size(); i++){
        for(int j = 0; j < (int)frameHeuristic.size(); j++)
            constFrame[i][j] = 0;
    }


	/// --- Begin finding an initial network --- ///
	bool isArchiIniValid = false;
	ifstream f(DF_SUBNET_INI);
    if (f.good()) {
		f.close();
		vector<SubNet> subNetData;
		importInitSubNet(&subNetData, &calcData, &frameHeuristic, DF_SUBNET_INI);
		evaluateArchi(&frameHeuristic, &calcData, &subNetData,
					  calcToCalcConnection, snToCalcConnection, constFrame, &isArchiIniValid);
		if (isArchiIniValid) {
			copyNetwork(&calcData, &subNetData, &frameHeuristic, &calcIni, &subNetIni, &frameIni);
		}
	} else {
		f.close();
		isArchiIniValid = constructValidNetwork(&calcData, &frameHeuristic,
												&calcIni, &subNetIni, &frameIni, &pathFrameIni,
												calcToCalcConnection, snToCalcConnection);
	}
	if (!isArchiIniValid) {
		// TODO: try other initializations
		cout << "No initial network found." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	// Make the parameters match the new objects
	adaptParamToCopiedNetwork(&paramData, &calcIni, &frameIni);
	/// --- End finding an initial network --- ///


	/// --- Launching the optimization --- ///
	cout << "Now optimizing the message set..." << endl;
	optimMessageSetLauncher(&paramData, &frameIni, &calcIni, &subNetIni,
							calcToCalcConnection, snToCalcConnection);


	/// --- Begin output interface --- ///
	string out_dir = "out_" + string(date);
	if (CreateDirectory(const_cast<char*>(out_dir.c_str()), NULL)
													|| ERROR_ALREADY_EXISTS == GetLastError()) {
		string nameFileOut = "./" + out_dir + "/param_calc_io_out_" + string(date) + ".csv";
		outputMessagingFile(nameFileOut, &calcIni, &frameIni);

		nameFileOut = "./" + out_dir + "/frame_calc_io_out_" + string(date) + ".csv";
		outputFrameSynthesisFile(nameFileOut, &calcIni, &frameIni, &subNetIni);

		string nameTopoFileOut = "./" + out_dir + "/topology_" + string(date) + ".csv";
		outputTopologyFile(nameTopoFileOut, &calcIni, &subNetIni);
	} else {
		cerr << "Could not create or write in directory \"" << out_dir << "\". Output files are "
			 << "written in the same directory as the program." << endl;
		string nameFileOut = "param_calc_io_out_" + string(date) + ".csv";
		outputMessagingFile(nameFileOut, &calcIni, &frameIni);

		nameFileOut = "frame_calc_io_out_" + string(date) + ".csv";
		outputFrameSynthesisFile(nameFileOut, &calcIni, &frameIni, &subNetIni);

		string nameTopoFileOut = "/topology_" + string(date) + ".csv";
		outputTopologyFile(nameTopoFileOut, &calcIni, &subNetIni);
	}
	/// --- End output interface --- ///
}


void optimMessageSetLauncher(vector<Parameter>* parList, vector<Frame>* frList, vector<Calc>* calcList,
							 vector<SubNet>* snList,
							 vector< vector<int> > const& calcToCalcConnection,
							 vector< vector<int> > const& snToCalcConnection						) {
        vector< vector<int> > constFrame;
        for(int i = 0; i < (int)frList->size(); i++){
                for(int j = 0; j < (int)frList->size(); j++)
                    constFrame[i][j] = 0;
        }

	// Evaluate the initial architecture
	bool isArchiValid;
	Criteria valueArchi = evaluateArchi(frList, calcList, snList,
										calcToCalcConnection, snToCalcConnection, constFrame, &isArchiValid);

	if (isArchiValid) {
		cout << "Provided topology is valid." << endl;
	}

	vector<PathFrame> pfList;
	// optimLoadsSimulatedAnnealing2(parList, calcList, snList, frList, &pfList, nb_it_SA, 0, Tmax);

	metaTabu(parList, calcList, snList, frList, &pfList, calcToCalcConnection, snToCalcConnection, constFrame,
				nb_it_SA, 0, Tmax);
}

