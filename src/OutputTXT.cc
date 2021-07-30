/**
 * @file OutputTXT.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "OutputTXT.hpp"

/**
 * \ingroup OutputTXT
 * 
 * Function to generate TXT data files with each beam  properties at Start, Foil and Drift.
 * 
 * @param[in] in Parameters read from input.txt file. 
 * @param[in] b Each species particles (= beam) information.
 * @param[in] nT Total number of particles merged accross MPI ranks.
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 * @param[in] label Spatial run identifier - at Start, Foil, Drift.
 */
void save_txt (Query in, map<G4String,PartBeams> * b, G4int * nT, G4int ifdrift, G4String * label){
    G4String fname = "data.txt";
    ofstream myfile;
    myfile.open(fname, ofstream::out | ofstream::app);
    G4int write_pos = myfile.tellp();
    if ( write_pos == 0 ){
        G4cout << "Generated file "<< fname << G4endl;
        myfile << "BEAM PROPERTIES\n";
        myfile << "species\t" << " #parts\t" << "#steps\t" << "<En>\t" << "RMS En\t" << "<x>\t" 
        << "<y>\t" << "<z>\t" << "RMS x\t" << "RMS y\t";
        if (in.profile =="flat-top"){
            myfile << "Delta/RMS z\t";
        }
        else {
            myfile << "RMS z\t";
        }
        myfile << "RMS atan(px/pz)\t" << "RMS atan(py/pz)\t" 
        << "Emittance x\t" << "Emittance y\t" << "<vz>\n";

        myfile << " \t" << " \t" << " \t" << "MeV\t" << "MeV\t" << "mm\t" << "mm\t" << "mm\t"
        << "mm\t" << "mm\t" << "mm\t" << "rad\t" << "rad\t" << "mm\t" << "mm\t" << "[c]\n";
    }
    myfile << *label << "\n";
    
    G4int nspecies = in.s_list.size();
    for (G4int i = 0; i < nspecies ; ++i){
        G4String name = in.s_list[i];
        if ( nT[i] > 0 ){
            myfile << name << "\t" << nT[i] << "\t"
            << (*b)[name].nsteps << "\t"
            << (*b)[name].b_mean_en[ifdrift] << "\t"
            << (*b)[name].b_std_en[ifdrift] << "\t"
            << (*b)[name].b_mean_x[ifdrift] << "\t"
            << (*b)[name].b_mean_y[ifdrift] << "\t"
            << (*b)[name].b_mean_z[ifdrift] << "\t"
            << (*b)[name].b_std_x[ifdrift] << "\t"
            << (*b)[name].b_std_y[ifdrift]<< "\t"
            << (*b)[name].b_std_z[ifdrift] << "\t"
            << (*b)[name].b_std_div_x[ifdrift] << "\t"
            << (*b)[name].b_std_div_y[ifdrift] << "\t"
            << (*b)[name].b_em_x[ifdrift] << "\t"
            << (*b)[name].b_em_y[ifdrift] << "\t"
            << (*b)[name].b_mean_vz[ifdrift]/c_light <<  "\n";
            G4cout << "Added " << name << " to file \n";
        }
    }
}
