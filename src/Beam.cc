/**
 * @file Beam.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "Beam.hpp"

using CLHEP::mm;
using CLHEP::MeV;
using CLHEP::radian;

/**
 * Constructor.
 */
Beam::Beam ()
{
}

/**
 * Destructor.
 */
Beam::~Beam ()
{
}

/**
 * Function to set primary particles (= events) properties according to values in input.txt.
 * 
 * @param[in] in Parameters read from input.txt file. 
 */
void Beam::set_beam (Query in){
    G4int nranks = G4MPImanager::GetManager()->GetTotalSize();
    beam_np = in.np/nranks;
    beam_type = in.type;

    G4double symmetry[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
    if (in.symmetrize){
	beam_np = beam_np*4;
    }

    beam_energy.reserve(beam_np);
    beam_position.reserve(beam_np);
    beam_direction.reserve(beam_np);

    if(beam_np >= 1){
        G4double mean_arr[6] = {in.en,in.x_beam[0],in.x_beam[1],in.x_beam[2],0.0,0.0};
        G4double sigma_arr[6] = {in.de*MeV,in.s_bprimary_x,in.s_bprimary_y,
            in.s_bprimary_z,in.s_angle_x*radian,in.s_angle_y*radian};
        G4double rand_n[6];
        G4double fac[3] = {0.0,0.0,1.0};
        for (G4int l = 0; l < beam_np; ++l){
            if (in.debug == true){
                G4cout << "Randomly generated numbers:\n";
            }
            for (G4int m = 0; m < 6;++m){
                rand_n[m] = G4RandGauss::shoot(mean_arr[m],sigma_arr[m]);
                if (in.debug == true){
                    G4cout << rand_n[m] << "\n";
                }
            }
            beam_energy.push_back(rand_n[0]);
            if (in.profile == "flat-top") {
                G4double beam_cz = mean_arr[3]-sigma_arr[3]/2.0;
                rand_n[3] = beam_cz+(G4double)l*sigma_arr[3]/(G4double)beam_np;
            }
            if (in.s_angle_x != 0.0 || in.s_angle_y != 0.0) {
                G4double angle_x = rand_n[4];
                G4double angle_y = rand_n[5];
                fac[2] = sqrt(1.0/(1.0+pow(tan(angle_x),2)+pow(tan(angle_y),2)));
                fac[0] = fac[2]*tan(angle_x);
                fac[1] = fac[2]*tan(angle_y);
                if (in.z_focal != 0.0) {
                    G4double dz = in.z_focal - rand_n[3];
                    rand_n[1] = rand_n[1] - dz*fac[0]/fac[2];
                    rand_n[2] = rand_n[2] - dz*fac[1]/fac[2];
                }
            }
            if (in.symmetrize){
                for (G4int n = 0; n < 4;++n){
                    beam_position.push_back(G4ThreeVector(rand_n[1]*symmetry[n][0],
                                                     rand_n[2]*symmetry[n][1],
                                                     rand_n[3]));
                    if (in.s_angle_x != 0.0 || in.s_angle_y != 0.0) {
                        beam_direction.push_back(G4ThreeVector(fac[0]*symmetry[n][0],
                                                fac[1]*symmetry[n][1],fac[2]));
                    }
                    else {
                        beam_direction.push_back(G4ThreeVector(fac[0],fac[1],fac[2]));
                    }
                }
            }
            else {
                beam_position.push_back(G4ThreeVector(rand_n[1],rand_n[2],rand_n[3]));
                beam_direction.push_back(G4ThreeVector(fac[0],fac[1],fac[2]));
            }
        }
    }
    else {
        G4ExceptionDescription msg;
        msg << "\n#beam particles (np) must be at least 1.\n";
        G4Exception("Beam::set_beam()",
                    "GPos error #1.0",FatalException,msg);
    }
}

/**
 * Function to retrieve primary particle centroid.
 * 
 * @param[in] eventID Particle (= event) identifier number.
 */
G4ThreeVector Beam::get_position (G4int eventID){
    return beam_position[eventID];
}

/**
 * Function to retrieve primary particle kinetic energy.
 * 
 * @param[in] eventID Particle (= event) identifier number.
 */
G4double Beam::get_energy (G4int eventID){
    return beam_energy[eventID];
}
/**
 * Function to retrieve primary particle normalized momentum.
 * 
 * @param[in] eventID Particle (= event) identifier number.
 */
G4ThreeVector Beam::get_direction (G4int eventID){
    return beam_direction[eventID];
}

