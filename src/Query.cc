/**
 * @file Query.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "Query.hpp"
using CLHEP::mm;
using CLHEP::MeV;
using CLHEP::radian;


/**
 * Constructor.
 * 
 * Default values were chosen based on the positron beam generation using a 10 GeV electron beam and
 *  tungsten foil case study explored for the BELLA center @ LBNL (2021).
 */
Query::Query ()
{
    debug = false;
    np = 200;
    totq = 1e-9;
    en = 10.e3*MeV;
    de = 1.0*MeV;
    frms = 4.0;
    z_focal = -4*mm;
    s_bprimary_x = 1.e-2*mm;
    s_bprimary_y = 1.e-2*mm;
    s_bprimary_z = 1.e-2*mm;
    s_angle_x = 0.0*radian;
    s_angle_y = 0.0*radian;
    type = "e-";
    profile = "gaussian";
    symmetrize = false;
    x_beam = G4ThreeVector(0.0*mm,0.0*mm,-4*mm);
    w_world = G4ThreeVector(10.*mm,10.*mm,5.*mm);
    hasfoil = true;
    w_foil = G4ThreeVector(10.*mm,10.*mm,0.5*mm);
    x_foil = G4ThreeVector(0.0*mm,0.0*mm,2.25*mm);
    t_foil="G4_W";
    drift = 0.0*mm;
    iflens = false;
    lens = 0.0*mm;
    lens_pz = 65*MeV;
    ifback = true;
    s_list = {"e-_primary","e-","e+"};
    encutoff = {0.0,0.0,0.0};
    pzcutoff = {0.0,0.0,0.0};
}

/**
 * Destructor.
 */
Query::~Query ()
{}

/**
 * Function that reads the file: input.txt.
 */
void Query::read_input ()
{
    G4String fname = "input.txt";
    ifstream inputFile (fname);
    G4String buff;
    if (inputFile.is_open()){
        G4String line;
        while(getline(inputFile,line)) {
            while (line.length() == 0 ){
                getline(inputFile,line);
            }
            G4String ll(line);
            set_variable(ll);
        }
        inputFile.close();
        if (hasfoil){
            if (x_foil[2]+w_foil[2]/2.0>w_world[2]/2.0){
                G4ExceptionDescription msg;
                msg << "\nThe foil has to fit inside the World space of thickness "
                << w_world[2] <<".\n";
                G4Exception("Query::read_input()",
                            "GPos error #15.0",FatalException,msg);
            }
        }
        if (profile != "gaussian" && profile != "flat-top"){
            G4ExceptionDescription msg;
            msg <<"\nAvailable primary beam profiles are only gaussian "
            <<"and flat-top.\n";
            G4Exception("Query::read_input()",
                         "GPos error #15.1",FatalException,msg);
        }
    }
    else{
        G4ExceptionDescription msg;
        msg <<"\nCouldn't open "<< fname <<".\n";
        G4Exception("Query::read_input()",
                    "GPos error #15.2",FatalException,msg);
    }
    set_cuttoff();
}

/**
 * Function that scans each parameter of the input file.
 */
void Query::set_variable (G4String line)
{
    if (line.find("=")){
        replace(line.begin(), line.end(), '=', ' ');
        stringstream iss(line);
        G4String word;
        vector<G4String> val;
        while (iss >> word){
            val.push_back(word);
        }
        if (val[0] == "debug") {
            if (strcasecmp("true",val[1]) == 0){
                debug = true;
            }
        }
        if (val[0] == "np") np = stoi(val[1]);
        if (val[0] == "totq") totq=stod(val[1]);
        if (val[0] == "en") en = stod(val[1]);
        if (val[0] == "de") de = stod(val[1]);
        if (val[0] == "frms") frms = stod(val[1]);
        if (val[0] == "z_focal") z_focal = stod(val[1]);
        if (val[0] == "s_bprimary_x") s_bprimary_x=stod(val[1]);
        if (val[0] == "s_bprimary_y") s_bprimary_y=stod(val[1]);
        if (val[0] =="s_bprimary_z") s_bprimary_z = stod(val[1]);
        if (val[0] == "x_beam") x_beam = G4ThreeVector(stod(val[1]),stod(val[2]),stod(val[3]));
        if (val[0] == "w_world") w_world = G4ThreeVector(stod(val[1]),stod(val[2]),stod(val[3]));
        if (val[0] == "symmetrize"){
            if (strcasecmp("true",val[1]) == 0){
                symmetrize = true;
            }
        }
        if (val[0] == "s_angle_x") s_angle_x = stod(val[1]);
        if (val[0] == "s_angle_y") s_angle_y = stod(val[1]);
        if (val[0] == "type") type = val[1];
        if (val[0] == "profile") profile = val[1];
        if (val[0] == "drift") drift = stod(val[1]);
        if (val[0] == "iflens"){
            if (strcasecmp("true",val[1]) == 0){
                iflens = true;
                if (drift != 0.0){
                    lens = drift/2.0;
                }
                else{
                    G4ExceptionDescription msg;
                    msg << "\nLens shouldn't be used without drift space.\n";
                    G4Exception("Query::set_variable()",
                    "GPos error #15.3",FatalException,msg);
                }
            }
        }
        if (val[0] == "lens"){
            if (iflens){
                lens = stod(val[1]);
                if ( lens>drift ){
                    G4ExceptionDescription msg;
                    msg << "\nLens hass to fal within drift distance.\n";
                    G4Exception("Query::set_variable()",
                    "GPos error #15.4",FatalException,msg);
                }
            }
            else{
                G4ExceptionDescription msg;
                msg << "\nLens was not added (iflens = false).\n";
                G4Exception("Query::set_variable()",
                "GPos error #15.5",FatalException,msg);
            }
        }
        if (val[0] == "lens_pz"){
            if (iflens) lens_pz=stod(val[1]);
            else{
                G4ExceptionDescription msg;
                msg << "\nLens was not added (iflens = false).\n";
                G4Exception("Query::set_variable()",
                "GPos error #15.6",FatalException,msg);
            }
        }
        if (val[0] == "ifback"){
            if (strcasecmp("true",val[1]) == 0){
                ifback=true;
            }
        }
        if (val[0] == "hasfoil"){
            if (strcasecmp("false",val[1]) == 0){
                hasfoil=false;
            }
        }
        if (val[0] == "w_foil"){
            if (hasfoil) w_foil=G4ThreeVector(stod(val[1]),stod(val[2]),stod(val[3]));
            else{
                G4ExceptionDescription msg;
                msg << "\nNo foil added, so w_foil will be neglected.\n";
                G4Exception("Query::set_variable()",
                "GPos error #15.7",JustWarning,msg);
            }
        }
        if (val[0] == "x_foil"){
            if (hasfoil) x_foil=G4ThreeVector(stod(val[1]),stod(val[2]),stod(val[3]));
            else{
                G4ExceptionDescription msg;
                msg << "\nNo foil added, so x_foil will be neglected.\n";
                G4Exception("Query::set_variable()",
                "GPos error #15.8",JustWarning,msg);
            }
        }
        if (val[0] == "t_foil"){
            if (hasfoil) t_foil=val[1];
            else{
                G4ExceptionDescription msg;
                msg << "\nNo foil added, so t_foil will be neglected.\n";
                G4Exception("Query::set_variable()",
                "GPos error #15.9",JustWarning,msg);
            }
        }
        if (val[0] == "s_list"){
            s_list.clear();
            for (size_t i = 1; i < val.size(); ++i) {
                s_list.push_back(val[i]);
            }
        }
        if (val[0] == "encutoff"){
            encutoff.clear();
            for (size_t i = 1; i < val.size(); ++i) {
                encutoff.push_back(stod(val[i]));
            }
        }
        if (val[0] == "pzcutoff"){
            pzcutoff.clear();
            for (size_t i = 1; i < val.size(); ++i) {
                pzcutoff.push_back(stod(val[i]));
            }
        }
    }
}

/**
 * Cuttoff sanity-check function.
 **/
void Query::set_cuttoff ()
{
    if (encutoff.size() != s_list.size()){
        G4ExceptionDescription msg1;
        msg1 << "\nCutoff energy should be set for each species in:\n";
        for (size_t i = 0; i < s_list.size(); i++){
             msg1<<"  "<<s_list[i]<<"\n";
        }
        G4Exception("Query::set_cuttoff()",
                    "GPos error #15.10",FatalException,msg1);
    }
    if (pzcutoff.size() != s_list.size()){
        G4ExceptionDescription msg2;
        msg2 << "\nCutoff pz should be set for each species in:\n";
        for (size_t j = 0; j < s_list.size(); j++){
             msg2 <<"  "<<s_list[j]<<"\n";
        }
        G4Exception("Query::set_cuttoff()",
                    "GPos error #15.11",FatalException,msg2);
    }
    for (size_t k = 0; k < s_list.size(); k++){
        if (pzcutoff[k] > encutoff[k]){
            G4ExceptionDescription msg3;
            msg3 << "\nEnergy cuttoffs needs to be higher than pz cuttoffs\n";
            G4Exception("Query::set_cuttoff()",
                    "GPos error #15.12",FatalException,msg3);
        }
    }
}

/**
 * Function that prints each parameter read from the input file or taken as its default value in the
 *  standard output
 */
void Query::print_input ()
{
    G4cout<<"***************** INPUT PARAMETERS **************\n";
    G4cout<<"Vacuum drift length [mm] = " << drift << "\n";
    if (iflens){
        G4cout<<"A linear lens will be placed at " << lens << " mm\n"
        << " tuned to pz = " << lens_pz << " MeV\n";
    }
    G4cout<<"Total beam charge ="<<totq<<"\n";
    G4cout<<"Beam RMS x [mm] = " << s_bprimary_x << "\n";
    G4cout<<"Beam RMS y [mm] = " << s_bprimary_y << "\n";
    G4cout<<"Beam RMS z [mm] = " << s_bprimary_z << "\n";
    G4cout<<"Beam En [MeV] = "<< en <<"\n";
    G4cout<<"Beam \u03B4 En [MeV] = "<< de <<"\n";
    G4cout<<"Beam RMS selection multiplying factor = "<< frms <<"\n";
    G4cout<<"Beam focal plane position [mm] = "<< z_focal <<"\n";
    G4cout<<"Beam RMS (atan(px/pz)) [rad] = " << s_angle_x << "\n";
    G4cout<<"Beam RMS (atan(py/pz)) [rad] = " << s_angle_y << "\n";
    if (symmetrize){
        G4cout<<"Primary beam is symmetric\n";
        G4cout<<"#"<<type<<"s = "<<np<<" (x4 requested)";
    }
    else {
        G4cout<<"Primary beam is not symmetric\n";
        G4int nranks = G4MPImanager::GetManager()-> GetTotalSize();
        if (nranks  > 1) G4cout<<"#"<<type<<"s = "<< np
        <<" split into each MPI rank\n";
        else G4cout<<"#"<<type<<"s = "<<np<<"\n";
    }
    G4cout<<"Primary beam longitudinal distribution is "<<profile<<"\n";
    if (ifback){
        G4cout<<"At end of run, particles propagate backwards\n";
    }
    G4cout<<"Beam centroid position [mm] = " << x_beam[0] << " " << x_beam[1]
    << " " << x_beam[2]
    <<"\nWorld dimensions [mm] = " << w_world[0] << " " << w_world[1]
    << " " << w_world[2] << "\n";
    if (hasfoil){
        G4cout << "Foil type: " << t_foil 
        << "\nFoil dimensions [mm] = " << w_foil[0] << " " << w_foil[1]
        << " " << w_foil[2]
        <<"\nFoil centroid position [mm] = " << x_foil[0] << " " << x_foil[1]
        << " " << x_foil[2] << "\n";
    }
    G4cout<<"List of species analyzed and corresponding pz, and energy cutoffs:\n";
    for (size_t i = 0; i < s_list.size(); i++){
         G4cout<<"  "<<s_list[i]<<", ";
         G4cout<<"  "<<pzcutoff[i]<<", ";
         G4cout<<"  "<<encutoff[i]<<" [MeV]\n";
    }
    G4cout<<"*************************************************\n";
}
