/**
 * @file Part.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "Part.hpp"

/**
 * Constructor.
 */
Part::Part ()
{
}

/**
 * Destructor.
 */
Part::~Part ()
{
}

/**
 * Function to set each particle information at start/end of event.
 * 
 * @param[in] p_name Particle species name.
 * @param[in] p_process Particle creation process.
 * @param[in] p_x Particle position in x.
 * @param[in] p_y Particle position in y.
 * @param[in] p_z Particle position in z.
 * @param[in] p_px Particle momentum in x.
 * @param[in] p_py Particle momentum in y.
 * @param[in] p_pz Particle momentum in z.
 * @param[in] p_tEn Particle total Energy.
 * @param[in] p_tg Particle global end time.
 * @param[in] p_step Particle final step registered.
 */
void Part::set_part (G4String p_name, G4String p_process, G4double p_x, G4double p_y, G4double p_z,
    G4double p_px, G4double p_py, G4double p_pz, G4double p_tEn, G4double p_tg, G4int p_step)
{
    name = p_name;
    process = p_process;
    x = p_x;
    y = p_y;
    z = p_z;
    px = p_px;
    py = p_py;
    pz = p_pz;
    tEn = p_tEn;
    tg = p_tg;
    steps = p_step;
}

/**
 * Constructor, where all properties are zeroed.
 */
PartBeams::PartBeams ()
{
    np = 0;
    for (G4int i = 0; i < 2; ++i){
        b_mean_x[i] = 0.0;
        b_mean_y[i] = 0.0;
        b_mean_z[i] = 0.0;
        b_std_x[i] = 0.0;
        b_std_y[i] = 0.0;
        b_std_z[i] = 0.0;
        b_mean_vx[i] = 0.0;
        b_mean_vy[i] = 0.0;
        b_mean_vz[i] = 0.0;
        b_std_vx[i] = 0.0;
        b_std_vy[i] = 0.0;
        b_std_vz[i] = 0.0;
        b_em_x[i] = 0.0;
        b_em_y[i] = 0.0;
        b_div_x[i] = 0.0;
        b_div_y[i] = 0.0;
        b_std_div_x[i] = 0.0;
        b_std_div_y[i] = 0.0;
    }
}

/**
 * Destructor.
 */
PartBeams::~PartBeams ()
{
}

/**
 * Function to set species beam properties.
 * 
 * @param[in] p Vector of particle data.
 * @param[in] n Beam species name.
 * @param[in] m Beam species mass.
 * @param[in] q Beam species charge.
 */
void PartBeams::set_partBeams (vector<Part> * p, G4String n, G4double m, G4double q)
{
    name = n;
    mass = m;
    charge = q;
    np = 0;
    for (Part particle : *p){
        if (particle.name == n) np++;
    }
    x.reserve(np);
    y.reserve(np);
    z.reserve(np);
    px.reserve(np);
    py.reserve(np);
    pz.reserve(np);
    tEn.reserve(np);
    tg.reserve(np);
    gg.reserve(np);
    bb.reserve(np);
    steps.reserve(np);
    for (Part particle : *p){
        if (particle.name == n){
            process.push_back(particle.process);
            x.push_back(particle.x);
            y.push_back(particle.y);
            z.push_back(particle.z);
            px.push_back(particle.px);
            py.push_back(particle.py);
            pz.push_back(particle.pz);
            tEn.push_back(particle.tEn);
            tg.push_back(particle.tg);
            steps.push_back(particle.steps);
        }
    }
    // Momentum in Geant4 is set in electron_mass_c2
    // electron_mass_c2 = 0.510999 MeV
    // c_light = 299.792 mm/ns
    for (G4int i = 0; i < np; ++i){
        gg.push_back(tEn[i]/electron_mass_c2);
        bb.push_back(sqrt(1.0-1.0/pow(gg[i],2)));
        vx.push_back(px[i]/gg[i]/electron_mass_c2*c_light);
        vy.push_back(py[i]/gg[i]/electron_mass_c2*c_light);
        vz.push_back(pz[i]/gg[i]/electron_mass_c2*c_light);
    }
}

/**
 * Function to set primary beam data.
 * 
 * @param[in] b Primary beam information.
 * @param[in] n Beam species name.
 * @param[in] proc Beam creation process label - primary.
 * @param[in] m Beam species mass.
 * @param[in] q Beam species charge.
 */
void PartBeams::set_partBeams (Beam * b, G4String n, G4String proc, G4double m, G4double q)
{
    name = n;
    mass = m;
    charge = q;
    np = b->beam_energy.size();
    x.reserve(np);
    y.reserve(np);
    z.reserve(np);
    px.reserve(np);
    py.reserve(np);
    pz.reserve(np);
    tEn.reserve(np);
    tg.reserve(np);
    gg.reserve(np);
    bb.reserve(np);
    steps.reserve(np);
    tEn = b->beam_energy;

    G4double dir_norm;
    G4double p_norm;
    G4double fac;
    for (G4int i = 0; i < np; ++i){
        process.push_back(proc);
        x.push_back(b->beam_position[i].x());
        y.push_back(b->beam_position[i].y());
        z.push_back(b->beam_position[i].z());

        dir_norm = sqrt(pow(b->beam_direction[i].x(),2) +
        pow(b->beam_direction[i].y(),2) + pow(b->beam_direction[i].z(),2));
        // Momentum in Geant4 is set in electron_mass_c2
        // electron_mass_c2 = 0.510999 MeV
        // c_light = 299.792 mm/ns
        gg.push_back(tEn[i]/electron_mass_c2);
        bb.push_back(sqrt(1.0-1.0/pow(gg[i],2)));
        p_norm = gg[i]*bb[i]*electron_mass_c2;
        fac = p_norm/dir_norm;
        px.push_back(fac*b->beam_direction[i].x());
        py.push_back(fac*b->beam_direction[i].y());
        pz.push_back(fac*b->beam_direction[i].z());
        fac = bb[i]/dir_norm*c_light;
        vx.push_back(fac*b->beam_direction[i].x());
        vy.push_back(fac*b->beam_direction[i].y());
        vz.push_back(fac*b->beam_direction[i].z());
        tg.push_back(0.0);
        steps.push_back(0);
    }
}

/**
 * Function to select only particles within frms distance from spatial and momentum beam centroid - 
 * used to compute properties at end of Foil and Drift positions.
 * 
 * @param[in] in Parameters read from input.txt file. 
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 */
void PartBeams::Select (Query in, G4int ifdrift){
    G4int np_new = 0;
    for (G4int i = 0; i < np; ++i){
        if (abs(x[i]-b_mean_x[ifdrift]) < b_std_x[ifdrift]*in.frms){
            if(abs(y[i]-b_mean_y[ifdrift]) < b_std_y[ifdrift]*in.frms){
                if(abs(z[i]-b_mean_z[ifdrift]) < b_std_z[ifdrift]*in.frms){
                    if (abs(vx[i]-b_mean_vx[ifdrift]) < b_std_vx[ifdrift]*in.frms){
                        if(abs(vy[i]-b_mean_vy[ifdrift]) < b_std_vy[ifdrift]*in.frms){
                            if(abs(vz[i]-b_mean_vz[ifdrift]) < b_std_vz[ifdrift]*in.frms){
                                x[np_new] = x[i];
                                y[np_new] = y[i];
                                z[np_new] = z[i];
                                px[np_new] = px[i];
                                py[np_new] = py[i];
                                pz[np_new] = pz[i];
                                vx[np_new] = vx[i];
                                vy[np_new] = vy[i];
                                vz[np_new] = vz[i];
                                tEn[np_new] = tEn[i];
                                tg[np_new] = tg[i];
                                gg[np_new] = gg[i];
                                bb[np_new] = bb[i];
                                process[np_new] = process[i];
                                np_new++;
                            }
                        }
                    }
                }
            }
        }
    }
    if (np_new < np){
        G4cout <<"\n\nSELECTION: Only "<<np_new<<" "<<name<<" out of "<<np <<" ("
        << G4double(np_new)/G4double(np)*100.0 <<"%) were within " <<in.frms<<" region.\n\n" ;
        np = np_new;
        x.erase(x.begin() + np_new, x.end());
        y.erase(y.begin() + np_new, y.end());
        z.erase(z.begin() + np_new, z.end());
        px.erase(px.begin() + np_new, px.end());
        py.erase(py.begin() + np_new, py.end());
        pz.erase(pz.begin() + np_new, pz.end());
        vx.erase(vx.begin() + np_new, vx.end());
        vy.erase(vy.begin() + np_new, vy.end());
        vz.erase(vz.begin() + np_new, vz.end());
        tEn.erase(tEn.begin() + np_new, tEn.end());
        tg.erase(tg.begin() + np_new, tg.end());
        gg.erase(gg.begin() + np_new, gg.end());
        bb.erase(bb.begin() + np_new, bb.end());
        steps.erase(steps.begin() + np_new, steps.end());
        process.erase(process.begin() + np_new, process.end());
    }
    else{
        G4ExceptionDescription msg;
        msg << "\nAll particles were within the selected RMS region.\n";
        G4Exception("PartBeams::Select()",
                    "GPos warning #7.0",JustWarning,msg);
    } 
}

/**
 * Function to propagate beam particles ballistically from Foil to Drift positions.
 * 
 * @param[in] tf Final global time merged accross all threads and MPI ranks.
 */
void PartBeams::Propagate (G4double * tf){
    for (G4int i = 0; i < np; ++i){
        // Valid for both input.back and !input.back
        x[i] = x[i] + vx[i]*(*tf-tg[i]);
        y[i] = y[i] + vy[i]*(*tf-tg[i]);
        z[i] = z[i] + vz[i]*(*tf-tg[i]);
        tg[i] = *tf;
    }
}

/**
 * Function to compute beam mean energy, position and velocity properties at each MPI rank.
 * 
 * @param[in] arr Array address to store all beam properties.
 */
void PartBeams::Centroids (G4double arr[])
{
    // No division is done before MPI merge
    G4double av_x = 0.0;
    G4double av_y = 0.0;
    G4double av_z = 0.0;
    G4double av_vx = 0.0;
    G4double av_vy = 0.0;
    G4double av_vz = 0.0;
    G4double av_en = 0.0;
    for (G4int pit = 0; pit<np; ++pit){
        av_en += tEn[pit];
        av_x += x[pit];
        av_y += y[pit];
        av_z += z[pit];
        av_vx += vx[pit];
        av_vy += vy[pit];
        av_vz += vz[pit];
    };
    arr[0] = av_en;
    arr[1] = av_x;
    arr[2] = av_y;
    arr[3] = av_z;
    arr[4] = av_vx;
    arr[5] = av_vy;
    arr[6] = av_vz;
}

/**
 * Function to store mean properties merged accross all MPI ranks into beam class.
 * 
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 * @param[in] arr Array address where beam properties were stored.
 */
void PartBeams::set_globalC (G4int ifdrift, G4double arr[])
{
    b_mean_en[ifdrift] = arr[0];
    b_mean_x[ifdrift] = arr[1];
    b_mean_y[ifdrift] = arr[2];
    b_mean_z[ifdrift] = arr[3];
    b_mean_vx[ifdrift] = arr[4];
    b_mean_vy[ifdrift] = arr[5];
    b_mean_vz[ifdrift] = arr[6];
}

/**
 * Function to compute RMS properties at each MPI rank.
 * 
 * @param[in] in Parameters read from input.txt file. 
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 * @param[in] arr Array address to store all beam properties.
 */
void PartBeams::StandardDeviation (Query in, G4int ifdrift, G4double arr[])
{
    G4double std_en = 0.0;
    G4double std_x = 0.0;
    G4double std_y = 0.0;
    G4double std_z = 0.0;
    G4double std_vx = 0.0;
    G4double std_vy = 0.0;
    G4double std_vz = 0.0;
    for (G4int pit = 0; pit < np; ++pit){
        std_en += pow(tEn[pit]-b_mean_en[ifdrift],2);
        std_x += pow(x[pit]-b_mean_x[ifdrift],2);
        std_y += pow(y[pit]-b_mean_y[ifdrift],2);
        if (in.profile != "flat-top") std_z += pow(z[pit]-b_mean_z[ifdrift],2);
        std_vx += pow(vx[pit]-b_mean_vx[ifdrift],2);
        std_vy += pow(vy[pit]-b_mean_vy[ifdrift],2);
        std_vz += pow(vz[pit]-b_mean_vz[ifdrift],2);
    };
    arr[0] = std_en;
    arr[1] = std_x;
    arr[2] = std_y;
    if (in.profile == "flat-top"){
        G4double max = *max_element(z.begin(), z.end());
        G4double min = *min_element(z.begin(), z.end());
        arr[3] = max-min;
    }
    else {
	    arr[3] = std_z;
    }
    arr[4] = std_vx;
    arr[5] = std_vy;
    arr[6] = std_vz;
}

/**
 * Function to store RMS properties merged accross all MPI ranks into beam class.
 * 
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 * @param[in] arr Array address where beam properties were stored.
 */
void PartBeams::set_globalSTD (G4int ifdrift, G4double arr[])
{
    b_std_en[ifdrift] = sqrt(arr[0]);
    b_std_x[ifdrift] = sqrt(arr[1]);
    b_std_y[ifdrift] = sqrt(arr[2]);
	b_std_z[ifdrift] = sqrt(arr[3]);
    b_std_vx[ifdrift] = sqrt(arr[4]);
    b_std_vy[ifdrift] = sqrt(arr[5]);
    b_std_vz[ifdrift] = sqrt(arr[6]);
}

/**
 * Function to compute beam divergence information at each MPI rank.
 * 
 * @param[in] arr Array address to store beam divergence auxiliary values.
 */
void PartBeams::Divergence (G4double arr[])
{
    G4double div_x = 0.0;
    G4double div_y = 0.0;
    G4double theta_x = 0.0;
    G4double theta_y = 0.0;
    for (G4int pit = 0; pit < np; ++pit){
        if (vz[pit] != 0){
            G4double tx = px[pit]/pz[pit];
            G4double ty = py[pit]/pz[pit];
            div_x += atan(tx);
            div_y += atan(ty);
            tx = tx*bb[pit]*gg[pit];
            ty = ty*bb[pit]*gg[pit];
            theta_x += tx;
            theta_y += ty;
        }
        else{
            G4ExceptionDescription msg;
            msg << "\nParticle with vz = 0 included\n";
            G4Exception("PartBeams::Emittance()",
            "GPos error #13.0",FatalException,msg);
        }
    }
    arr[0] = div_x;
    arr[1] = div_y;
    arr[2] = theta_x;
    arr[3] = theta_y;
}

/**
 * Function to store divergence data merged accross all MPI ranks into beam class.
 * 
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 * @param[in] arr Array address where beam divergence values were stored.
 */
void PartBeams::set_globalDiv (G4int ifdrift, G4double arr[])
{
    b_div_x[ifdrift] = arr[0];
    b_div_y[ifdrift] = arr[1];
}

/**
 * Function to compute beam emittance information at each MPI rank.
 * 
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 * @param[in] arr Array address to store beam divergence auxiliary values.
 */
void PartBeams::Div_Emittance (G4int ifdrift, G4double arr[])
{
    G4double std_divx = 0.0;
    G4double std_divy = 0.0;
    G4double std_tx = 0.0;
    G4double std_ty = 0.0;
    G4double x_tx = 0.0;
    G4double y_ty = 0.0;
    for (G4int pit = 0; pit < np; ++pit){
        if (vz[pit] != 0){
            G4double tx = px[pit]/pz[pit];
            G4double ty = py[pit]/pz[pit];
            std_divx += pow(atan(tx)-b_div_x[ifdrift],2);
            std_divy += pow(atan(ty)-b_div_y[ifdrift],2);
            tx = tx*bb[pit]*gg[pit];
            ty = ty*bb[pit]*gg[pit];
            std_tx += pow(tx-arr[2],2);
            std_ty += pow(ty-arr[3],2);
            x_tx += (x[pit]-b_mean_x[ifdrift])*(tx-arr[2]);
            y_ty += (y[pit]-b_mean_y[ifdrift])*(ty-arr[3]);
        }
    }
    arr[0] = std_divx;
    arr[1] = std_divy;
    arr[2] = std_tx;
	arr[3] = std_ty;
    arr[4] = x_tx;
    arr[5] = y_ty;
}

/**
 * Function to store emittance data merged accross all MPI ranks into beam class.
 * 
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 * @param[in] arr Array address where beam divergence values were stored.
 * @param[in] nT Pointer to total number of particles accross all MPI ranks
 */
void PartBeams::set_globalDivEm (G4int ifdrift, G4double arr[], G4int * nT)
{
    b_std_div_x[ifdrift] = sqrt(arr[0]);
    b_std_div_y[ifdrift] = sqrt(arr[1]);
    if (*nT>2){
        b_em_x[ifdrift] = sqrt(pow(b_std_x[ifdrift],2)*arr[2]-pow(arr[4],2));
        b_em_y[ifdrift] = sqrt(pow(b_std_y[ifdrift],2)*arr[3]-pow(arr[5],2));
    }
}

/**
 * Function to compute minimum time for beam particles to travel drift distance.
 * 
 * @param[in] in Parameters read from input.txt file. 
 */
G4double PartBeams::get_tdrift (Query in){
    G4double tdrift = numeric_limits<double>::max();
    G4double zdrift = in.w_world.z()/2.0+in.drift;
    if (np > 0){
        for (G4int pit = 0; pit<np; ++pit){
            tdrift = min((zdrift-z[pit])/vz[pit], tdrift);
        }
    }
    return tdrift;
}

/**
 * Function to compute beam properties after the drift with(out) thin lens.
 * 
 * @param[in] g_td Drift time merged accross all MPI ranks.
 * @param[in] in Parameters read from input.txt file. 
 */
void PartBeams::Drift (G4double * g_td, Query in){
    if (np > 0){
        if (in.iflens){
	    G4double gref = sqrt(1.0+pow((in.lens_pz/electron_mass_c2),2));
            for (G4int i = 0; i < np; ++i){
                // Momentum in Geant4 is set in electron_mass_c2
                // electron_mass_c2 = 0.510999 MeV
                // c_light = 299.792 mm/ns
                // Kinetic Energy = (gamma -1) m c^2
                G4double d0 = in.lens+in.w_world.z()/2.0-z[i];
                G4double tlens = d0/vz[i];
                G4double k = -2.0*charge*(gref-1.0/gref)*electron_mass_c2/d0;
                // Properties near the foil
                G4double x0 = x[i];
                G4double y0 = y[i];
                G4double px0 = px[i];
                G4double py0 = py[i];
                // Properties right after the thin lens
                z[i] = in.lens+in.w_world.z()/2.0;
                G4double vzinv = c_light/vz[i];
                G4double pzinv = 1.0/pz[i];
                G4double M11 = 1.0;
                G4double M12 = d0*pzinv;
                G4double M21 = k*vzinv;
                G4double M22 = d0*k*vzinv*pzinv+1.0;
                x[i] = M11*x0 + M12*px0;
                y[i] = M11*y0 + M12*py0;
                px[i] = M21*x0 + M22*px0;
                py[i] = M21*y0 + M22*py0;
                // pz is a constant of motion
                gg[i] = sqrt(1.0+(pow(px[i],2)+pow(py[i],2)+pow(pz[i],2))/pow(electron_mass_c2,2));
                bb[i] = sqrt(1.0-1.0/pow(gg[i],2));
                tEn[i] = gg[i]*electron_mass_c2;
                vx[i] = px[i]/electron_mass_c2/gg[i]*c_light;
                vy[i] = py[i]/electron_mass_c2/gg[i]*c_light;
                vz[i] = pz[i]/electron_mass_c2/gg[i]*c_light;
                // Properties after the drift
                G4double td = *g_td-tlens;
                x[i] += td*vx[i];
                y[i] += td*vy[i];
                z[i] += td*vz[i];
                tg[i] += *g_td;
            } 
        }
        else{
            for (G4int k = 0; k < np; ++k){
                x[k] += *g_td*vx[k];
                y[k] += *g_td*vy[k];
                z[k] += *g_td*vz[k];
                tg[k] = *g_td;
            }
        }
    }
}

/**
 * Function to print beam properties if running in debug mode.
 * 
 * @param[in] l Beam species name. 
 */
void PartBeams::Print (G4String l){
    G4int rank = G4MPImanager::GetManager()->GetRank();
    G4String fname ="mpi_"+to_string(rank)+".txt";
    ofstream f;
    f.open(fname, ofstream::out | ofstream::app);
    
    f << "\tData from beam " << l << "\nName\tProcess\tStep\t";
    f << "x\ty\tz\tpx\tpy\tpz\ttotE\tGlobalt\tmass\tcharge\n";
    for (G4int i = 0; i < np; ++i){
        f << name << "\t" << process[i] << "\t" << steps[i] << "\t" << x[i] << "\t" << y[i] 
        << "\t" << z[i] << "\t" << px[i] << "\t" << py[i] << "\t" << pz[i] << "\t" << tEn[i] 
        << "\t" << tg[i] << "\t" << mass << "\t" << charge << "\n";
    }
    f.close();
    G4cout << "Beam of " << name << " stored to " << fname  << " : " << l << "\n";
}

/**
 * Function to compute number of steps at each MPI rank.
 * 
 * @return Total number of steps at each MPI rank
 */
G4int PartBeams::get_nsteps (){
    G4int ns = 0;
    for (G4int pit = 0; pit < np; ++pit) ns += steps[pit];
    return ns;
}

/**
 * Function to store total number of steps merged accross all MPI ranks.
 */
void PartBeams::set_nsteps (G4int * ns){
    nsteps = *ns;
}

/**
 * Function to store charge and mass information merged accross all MPI ranks (needed for openPMD).
 */
void PartBeams::set_MQ(G4double m, G4double q){
    mass = m;
    charge = q;
}