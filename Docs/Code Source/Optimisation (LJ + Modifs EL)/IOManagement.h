#ifndef IOMANAGEMENT_H_INCLUDED
#define IOMANAGEMENT_H_INCLUDED



#include "includes.h"
#include <fstream>
#include <cstdlib>
// #include <queue>
// #include <cmath>
// #include <thread>

#include "Calc.h"
#include "Element.h"
#include "Frame.h"
#include "Parameter.h"
#include "PathFrame.h"
#include "SubNet.h"
#include "Criteria.h"

// #include "const_tool.h"





/// Other file scope variables. These one do not need to be defined as extern as they are only used here.
// For reading inputs
const int nb_non_transmission_line_pcioFile = 3;	// 2 line header + 1 bottom line
const int nb_non_calc_column_pcioFile = 8;
const int nb_non_frame_line_fcioFile = 4;	// 2 line header + 1 bottom line
const int nb_non_calc_column_fcioFile = 5;	// 5 column header
const int nb_line_header_fciofile = 4;




/***********
 * cfgFromFile()
 ***********/
bool cfgFromFile();


/***********
 * printcfg()
 ***********/
void printcfg();

int importNewCalc(vector<Calc>* calcData, vector<Parameter>* paramData, vector<Frame>* frameData,
					   vector<SubNet>* snData);


/***********
 * importFcioFileData(vector<Calc>* calcData, vector<Parameter>* paramData, vector<Frame>* frameData)
 *
 * Imports data about the defined calculators and frames from the FcioFile datafile. Virtual parameters
 * are created (1 per frame) to simulate the presence of parameters in the frames and thus ensure the
 * delivery of the frames (the constraint is formulated in terms of delivery of parameters).
 *
 * OUTPUT	***TO BE UPDATED***
 * MODIFY	vector<Calc>* calcData: refers to the list of all different calculators
 * 			vector<Parameter>* paramData: refers to the list of all different parameters
 * 			vector<Frame>* frameData: refers to the list of all different frames
 ***********/
int importFcioFileData(vector<Calc>* calcData, vector<Parameter>* paramData, vector<Frame>* frameData,
					   vector<SubNet>* snData);


/***********
 * importPcioFileData(vector<Calc>* calcData, vector<Parameter>* paramData, vector<Frame>* frameIni)
 *
 * Imports data about the defined calculators, parameters and frames from the PcioFile datafile.
 *
 * MODIFY	vector<Calc>* calcData: refers to the list of all different calculators
 * 			vector<Parameter>* paramData: refers to the list of all different parameters
 * 			vector<Frame>* frameIni: refers to the list of all different frames
 * OUTPUT	Number (int) of parameters
 ***********/
int importPcioFileData(vector<Calc>* calcData, vector<Parameter>* paramData, vector<Frame>* frameIni);


/***********
 * initSubNet(vector<SubNet>* sn, vector<Calc>* calcData, string dataSNFile)
 *
 * Initialize the subnetworks with those read in dataSNFile.
 *
 * INPUT	string dataSNFile: data file of the initial calculator-subnetwork connections
 * MODIFY	vector<SubNet>* sn: refers to the list of existing subnetworks
 * 			vector<Calc>* calcData: refers to the list of calculators
 * OUTPUT	Number (int) of initial subnetworks created
 ***********/
// int importInitSubNet(vector<SubNet>* sn, vector<Calc>* calcData, string dataSNFile);
int actualSubNet(vector<SubNet>* sn, vector<Calc>* calcData, vector<Frame>* fr, string dataSNFile, vector< vector<int> > * snToCalcConnection
                 , vector< vector<int> > * calcToCalcConnection);

int importInitSubNet(vector<SubNet>* sn, vector<Calc>* calcData, vector<Frame>* fr, string dataSNFile);


void importTableConstraintsFrame (const string& nameConstraintsFile, vector<Frame>* fr,
                                  vector< vector<int> > * constFrame);
/***********
 * importUserDefinedTopologyConstraints(string const nameUserDefTopoFile,
										  vector<Calc> const * const calcList,
										  vector< vector<int> > * calcToCalcConnection,
										  vector< vector<int> > * snToCalcConnection	)
 *
 * Imports from the the user defined topology constraint file additional constraints on topology (eg.
 * fixed parts) and store them into two matrices:
 * *calcToCalcConnection (symetric) which stores if two given calculators must/must not have any common
 * subnetwork and
 * *snToCalcConnection which stores if a given calculator must/must not be connected to a
 * given subnetwork.
 * As parameter is given the list of already imported calculators. This list and the one defined in the
 * user defined topology constraint file must be the same.
 *
 * INPUT	string const nameUserDefTopoFile
 *				path to the user defined topology constraint file
 *
 *			vector<Calc> const * const calcList
 *				refers to the already imported list of calculators
 *
 * MODIFY	vector< vector<int> > * calcToCalcConnection
 *				matrix that summarizes if calculators of indices i and j must have at least one common
 *				network (value +1) or must not have any	(value -1). All others values are not considered
 *				(ie. not associated with a constraint).
 *
 * 			vector< vector<int> > * snToCalcConnection
 *				matrix (i,j) that summarizes if calculator of index j must be connected to subnetwork of
 *				index i (value +1) or must not (value -1). All others values are not considered (ie. not
 *				associated with a constraint).
 ***********/
void importUserDefinedTopologyConstraints(const string& nameUserDefTopoFile,
										  vector<Calc> const * const calcList,
										  vector< vector<int> > * calcToCalcConnection,
										  vector< vector<int> > * snToCalcConnection	);


/***********
 * readDataFile(string dataFilePath, char delim)
 *
 * Read a datafile and returns its content.
 *
 * INPUT	string dataFilePath: the data file path
 * 			char delim: the splitting character used
 * OUTPUT	A vector< vector<string> > (vector of the file rows)
 ***********/
vector< vector<string> > readDataFile(string dataFilePath, char delim);


/***********
 * void outputMessagingFile(string name_file_out, vector<Calc>* calcOut, vector<Frame>* frameOut)
 *
 * Write the output message set in a file whose format is close to the input message set files (.csv file
 * with ";" as separator). The list of column are namely:
 * - Source: the source calculator of the parameter (1 line = 1 parameter)
 * - Frame: the name of the frame in which the parameter is placed
 * - Frame Period (ms): the period of emission the frame
 * - Parameter: the name of the parameter of the line
 * - Size (bits): the size (in bits) of the parameter
 * - Frame Charge (bps): the busload induced by the frame computed as:
 *	 (length of the frame) / (period (in s))
 * 	 Note that the calculus does NOT include the multiplicator factor.
 * - Then one column per ECU: here are printed the period (in ms) at which the parameter must be recieved
 * 	 by the ECU (0 for on event) or nothing if the ECU does not need the parameter.
 *
 * INPUT	string name_file_out
 *				string that represent the path where the output file must be written.
 *				If a previous file exists and is accessible, it will be deleted. If the file exists and is
 *				not accessible (eg. because it is currently opened), nothing will happen and a warning
 *				message is printed.
 *
 *			vector<Calc>* calcOut
 *				refers to the list of calculators
 *
 *			vector<Frame>* frameOut
 *				refers to the list of frames that defines the message set
 ***********/
void outputMessagingFile(string name_file_out, vector<Calc>* calcOut, vector<Frame>* frameOut);


/***********
 * outputFrameSynthesisFile(string name_file_out, vector<Calc>* calcOut, vector<Frame>* frameOut,
							  vector<SubNet>* snOut													)
 *
 * Write the output message set in a file (.csv file with ";" as separator).
 * The list of column are namely:
 * - Frame name: the name of the frame (1 line = 1 frame)
 * - Identifier (hex): unknown (a "?" is written)
 * - Frame Period (ms): the period of emission the frame
 * - Size (Bytes): the size (in bytes) of the frame.
 * - Frame Load (bps): the busload induced by the frame computed as:
 *	 (length of the frame) / (period (in s))
 *	 This does NOT include the factor LOADMULTFACTOR that simulates bit-stuffing.
 * - Then one column per ECU: here are printed "T" if the ECU emits the frame, "R" if it recieves it and
 *	 otherwise nothing.
 * Then a second table at the right of the previous one indicates the routing of the frame.
 *
 * INPUT	string name_file_out
 *				string that represent the path where the output file must be written.
 *				If a previous file exists and is accessible, it will be deleted. If the file exists and is
 *				not accessible (eg. because it is currently opened), nothing will happen and a warning
 *				message is printed.
 *
 *			vector<Calc>* calcOut
 *				refers to the list of calculators
 *
 *			vector<Frame>* frameOut
 *				refers to the list of frames that defines the message set
 *			vector<SubNet>* snOut
 *				refers to the list of subnetworks that defines the topology
 ***********/
 void outputFrameSynthesisFile(string name_file_out, vector<Calc>* calcOut, vector<Frame>* frameOut,
							  vector<SubNet>* snOut													);


/***********
 * outputTopologyFile(string name_file_out, vector<Calc>* calcOut, vector<SubNet>* snOut)
 *
 * Write the topology in a file.
 *
 * INPUT	string name_file_out
 *				string that represent the path where the output file must be written.
 *				If a previous file exists and is accessible, it will be deleted. If the file exists and is
 *				not accessible (eg. because it is currently opened), nothing will happen and a warning
 *				message is printed.
 *
 *			vector<Calc>* calcOut
 *				refers to the list of calculators
 *
 *			vector<SubNet>* snOut
 *				refers to the list of subnetworks that defines the topology
 ***********/
void outputTopologyFile(string name_file_out, vector<Calc>* calcOut, vector<SubNet>* snOut);


/***********
 * vector<string> &split(const string &s, char delim, vector<string> &elems)
 *
 * Cut a string in smaller strings (elems) at positions where the character delim is found (the delimiting
 * character is removed). The resulting smaller strings are placed into a vector (elems) and returned.
 *
 * INPUT	const string &s
 *				input string to be splitted
 *
 *			char delim
 *				delimitting character
 *
 * MODIFY	vector<string> &elems
 *				vector of strings resulting from the splitting process are added (pushed) in there.
 *
 * OUTPUT	vector<string>&
 *				vector of strings resulting from the splitting process (same as what is pushed into elems)
 ***********/
vector<string> &split(const string &s, char delim, vector<string> &elems);


/***********
 * vector<string> split(const string &s, char delim)
 *
 * Cut a string in smaller strings at positions where the character delim is found (the delimiting
 * character is removed). The resulting smaller strings are returned in a vector of string.
 *
 * INPUT	const string &s
 *				input string to be splitted
 *
 *			char delim
 *				delimitting character
 *
 * OUTPUT	vector<string>&
 *				vector of strings resulting from the splitting process.
 ***********/
vector<string> split(const string &s, char delim);


#endif // IOMANAGEMENT_H_INCLUDED
