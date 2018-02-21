#ifndef CONST_TOOL_H_INCLUDED
#define CONST_TOOL_H_INCLUDED

// // #define DF_PARAM_CALC_IO "..\\..\\Data\\param_calc_io.csv"
// // #define DF_PARAM_CALC_IO "../../Data/param_calc_io_mod.csv"	// instance 2
// #define DF_PARAM_CALC_IO "../../Data/param_calc_io_mod_2.csv"	// instance 1
// // #define DF_FRAME_CALC_IO "..\\..\\Data\\cas1_frame_calc_io.csv"
// // #define DF_SUBNET_INI "../../Data/cas1_subnet_ini.csv"
// #define DF_FRAME_CALC_IO "..\\..\\Data\\frame_calc_io.csv"
// #define DF_SUBNET_INI "../../Data/subnet_ini.csv"
// #define DF_USER_TOPO "../../Data/user_defined_topology.csv"

#define _REM_ "Removing"
#define _ADD_ "Adding"
#define CSTRINGSIZE 300

#include<vector>


// #define RESOLUTION_MODE 1	//	1: fixed frames (thus not using parameters): optimize network
							//	2: free frames from scratch (using only parameters)
							// -- below : to update --


// For random generation
const int seed_random_gen = 12345;

// Resolution mode
extern int RESOLUTION_MODE;

// Data files
const char CFG_FILE[] = "cfg.txt";
extern char DF_PARAM_CALC_IO[];
extern char DF_FRAME_CALC_IO[];
extern char DF_SUBNET_INI[];
extern char DF_USER_TOPO[];

//Data files for add new calculators
extern char DF_NEW_CALC[];
extern char DF_NEW_FRAME[];
extern char DF_REST_FRAME[]; //file with restricted frames

// Architecture parameters
extern int nb_SN_max;
extern int nb_max_calc_per_SN;
extern int nb_max_SN_per_calc;
extern int nb_max_SN_FD_per_calc;
extern std::vector<double> max_load_supported;
extern std::vector<double> max_load_allowed;
extern int nb_bridge;
extern int bridge_nb_max_connector;
extern int bridge_nb_max_connector_FD;

extern double price_SN;
extern double price_connector;
extern double price_bridge;

// Frame creation parameter
extern int size_encapsulation;
extern int nb_byte_max;
extern bool no_par_astride_bytes;

#define LOADMULTFACTOR 1.0

// For pathframe optimization
extern int nb_it_spreadPath;
extern int mode_opt_rout;
extern int path_order_max;

// For tabu optimization
extern int size_tabu;
extern int nb_it_tabu;
extern bool allow_stochastic;
extern int nb_choice_rem;		// not used if the deterministic tabu is used
extern int nb_choice_add;		// not used if the deterministic tabu is used
extern int nb_it_max_without_change;

// For simulated annealing
extern double Tmax;			// Tmax at the first iteration
extern double Tfactor;
extern double param_exp;
const int heating_first = 0;	////// not heating !! "greedy" should be more appropriate
const int nb_it_SA_first = 2000;	// minimum 100 !
const int nb_it_heating = 0;
extern int nb_it_SA;
const int nb_picked_src = 200;

// For combined optimization
extern int size_loop_SA;

// For multistart
extern int nb_it_gen;					// max number of iteration to generate a network automatically
extern int nb_multi_start;
extern bool do_multistart;

// For the output
extern int nb_network_out;
extern bool printToFile;

// bool testprint = false;
// extern bool testprint;

#endif // CONST_TOOL_H_INCLUDED
