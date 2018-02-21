#include "AddCalc.h"

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

//using namespace std;

void optimNewCalc (char* date) {

    vector<Calc> calcData;
	vector<Parameter> paramData;
	vector<Frame> frameData;
	vector<SubNet> subNetData;
    vector< vector<int> > calcToCalcConnection;
	vector< vector<int> > snToCalcConnection;

    cout << "[RESOLUTION_MODE " << RESOLUTION_MODE << "]" << endl << endl;

    /// --- Begin input interface --- ///
	// Import data: Calc, Parameter and Frame
	importFcioFileData(&calcData, &paramData, &frameData, &subNetData); //paramData = frames

    // Import data: user defined topology constraints
	calcToCalcConnection.reserve(2*calcData.size());
	snToCalcConnection.reserve(2*nb_SN_max);
	importUserDefinedTopologyConstraints(DF_USER_TOPO, &calcData,
											&calcToCalcConnection, &snToCalcConnection);

   // importNewCalc(&calcData, &paramData, &frameData, &subNetData);

	// Add bridges at the end of the list of Calc if needed
	for (int ibdg=0 ; ibdg<nb_bridge ; ++ibdg) {
		string id_bridge = "BDG" +
							static_cast<ostringstream*>( &(ostringstream() << ibdg) )->str();
		Calc bridge(id_bridge, true, 0, 0, bridge_nb_max_connector, bridge_nb_max_connector_FD, 1000);
		calcData.push_back(bridge);
	}

    actualSubNet(&subNetData, &calcData, &frameData, DF_SUBNET_INI, &snToCalcConnection, &calcToCalcConnection);

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
			 << maxload/500000*100 << "%)." << endl; //max load supported for CAN

		calcBest.pop();
		subNetBest.pop();
		frameBest.pop();
	}
	/// --- End output interface --- ///



}

void addCalcWithMessage(char* date) {

	vector<Calc> calcData;
	vector<Parameter> paramData;
	vector<Frame> frameData;
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
		Calc bridge(id_bridge, true, 0, 0, bridge_nb_max_connector,bridge_nb_max_connector_FD,1000);
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

	/// --- Begin finding an initial network --- ///
	bool isArchiIniValid = false;
	ifstream f(DF_SUBNET_INI);
    if (f.good()) {
		f.close();
		vector<SubNet> subNetData;
		//importInitSubNet(&subNetData, &calcData, &frameHeuristic, DF_SUBNET_INI);
        actualSubNet(&subNetData, &calcData, &frameHeuristic, DF_SUBNET_INI, &snToCalcConnection, &calcToCalcConnection);

        vector< vector<int> > constFrame;
        for(int i = 0; i < (int)frameHeuristic.size(); i++){
                for(int j = 0; j < (int)frameHeuristic.size(); j++)
                    constFrame[i][j] = 0;
        }

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
		cerr << "No initial network found." << endl;
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
