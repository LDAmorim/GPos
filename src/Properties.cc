/**
 * @file Properties.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "Properties.hpp"

/**
 * Constructor.
 * 
 * @param[in] in Parameters read from input.txt file. 
 */
Properties::Properties (Query in)
{
    input = in;
}

/**
 * Destructor.
 */
Properties::~Properties ()
{
}

/**
 * Main function where all the properties functions of all the beams at end of Foil and Drift are 
 * called and results are merged accross all MPI ranks and stored in TXT and OPMD file formats.
 * 
 * @param[in] tf Pointer to global time at the end of the foil
 * @param[in] p  Pointer to vector with particle data
 * @param[in] mq Pointer to map of all species mass and charge data
 */
void Properties::ComputeBeams (G4double * tf, vector<Part> * p,
map<G4String,map<G4String,G4double>> * mq){
    G4int nspecies = input.s_list.size();
    map<G4String, PartBeams> beams;
    for (G4int i = 0; i < nspecies; ++i ) {
        if ( mq -> find(input.s_list[i]) != mq -> end() ){
            beams[input.s_list[i]].set_partBeams(p, input.s_list[i], (*mq)[input.s_list[i]]["mass"],
            (*mq)[input.s_list[i]]["charge"]);
            if (input.debug) beams[input.s_list[i]].Print("at end of run");
            np0[i] = beams[input.s_list[i]].np;
        }
        else np0[i] = 0;
    }
    for ( auto & beam : beams ) {
        beam.second.Propagate(tf);
        if (input.debug) beam.second.Print("at foil");
    }
    MPI_Allreduce(np0, npT, nspecies, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Scan(np0, npS, nspecies, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if (input.debug) {
        for ( G4int j = 0; j < nspecies; ++j ) {
            G4cout << "Local # " << input.s_list[j] << " : " << np0[j] << "\n";
            G4cout << "MPI merged # " << input.s_list[j] << " : " << npT[j]
            << " and MPI scan # : " << npS[j] << "\n";
        }
    }
    if (input.frms > 0){
        MeanSTD(&beams, np0, npT, 0);
        for (G4int k = 0; k < nspecies; ++k ) {
            if ( np0[k] > 0 ){
                beams[input.s_list[k]].Select(input, 0);
                np0[k] = beams[input.s_list[k]].np;
            }
            else np0[k] = 0;
            npT[k] = 0;
        }
        MPI_Allreduce(np0, npT, nspecies, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        MPI_Scan(np0, npS, nspecies, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    }

    Analysis(&beams, np0, npT, 0);
    G4String * label = new G4String;
    *label = "_foil";
    ShareMQ(&beams, np0, npT);
    save_outOPMD(input, &beams, npT, npS, label);
    *label = "Foil";
    G4int rank = G4MPImanager::GetManager()->GetRank();
    if (rank == 0) save_txt(input, &beams, npT, 0, label);
    if (input.drift > 0.0){
        *td0 = numeric_limits<double>::max();
        for (G4int l = 0; l < nspecies; ++l ) {
            if ( npT[l] > 0 ){
                *td0 = min(*td0,beams[input.s_list[l]].get_tdrift(input));
            }
        }
        MPI_Allreduce(td0, tdT, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
        for (G4int mi = 0; mi < nspecies; ++mi ) {
            if ( npT[mi] > 0 ){ 
                beams[input.s_list[mi]].Drift(tdT, input);
                if (input.debug) beams[input.s_list[mi]].Print("after drift");
            }
        }
        Analysis(&beams, np0, npT, 1);
        *label = "_drift";
        save_outOPMD(input, &beams, npT, npS, label);
        *label = "Drift";
        if (rank == 0) save_txt(input, &beams, npT, 1, label);
    }
}

/**
 * Function to compute beam mean and RMS position and momentum merged accross all MPI ranks.
 * 
 * @param[in] b Pointer to beam data
 * @param[in] n0  Pointer to number of beam particles in each rank
 * @param[in] nT Pointer to total number of particles accross all MPI ranks
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 */
void Properties::MeanSTD (map<G4String,PartBeams> * b, G4int * n0, G4int * nT, G4int ifdrift)
{
    G4int nspecies = input.s_list.size();
    for (G4int i = 0; i < nspecies ; ++i){
        if ( nT[i] > 0 ){
            G4String name = input.s_list[i];
            G4double dnpT = (double)(nT[i]);
            if ( n0[i] > 0 ){
                (*b)[name].Centroids(mpi_in);
                for (G4int j = 0; j < 7; j++) mpi_in[j] = mpi_in[j]/dnpT;
            }
            MPI_Allreduce(mpi_in, mpi_out, 7, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            (*b)[name].set_globalC(ifdrift, mpi_out);
            if ( n0[i] > 0 ){
                for (G4int k=0; k < 7; k++){
                    mpi_in[k] = 0;
                    mpi_out[k] = 0;
                }
                (*b)[name].StandardDeviation(input, ifdrift, mpi_in);
                for (G4int l = 0; l < 7; l++) mpi_in[l] = mpi_in[l]/dnpT;
            }
            MPI_Allreduce(mpi_in, mpi_out, 7, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            (*b)[name].set_globalSTD(ifdrift, mpi_out);
            if ( n0[i] > 0 ){
                for (G4int mi = 0; mi < 7; mi++){
                    mpi_in[mi] = 0;
                    mpi_out[mi] = 0;
                }
            }
        }
    }
}

/**
 * Function to compute all beam properties merged accross all MPI ranks.
 * 
 * @param[in] b Pointer to beam data
 * @param[in] n0  Pointer to number of beam particles in each rank
 * @param[in] nT Pointer to total number of particles accross all MPI ranks
 * @param[in] ifdrift Determines if user requested drift in input.txt file.
 */
void Properties::Analysis (map<G4String,PartBeams> * b, G4int * n0, G4int * nT, G4int ifdrift)
{
    G4int nspecies = input.s_list.size();
    for (G4int i = 0; i < nspecies ; ++i){
        if ( nT[i] > 0 ){
            G4String name = input.s_list[i];
            G4double dnpT = (double)(nT[i]);
            if ( n0[i] > 0 ){
                (*b)[name].Centroids(mpi_in);
                for (G4int j = 0; j < 7; j++) mpi_in[j] = mpi_in[j]/dnpT;
            }
            MPI_Allreduce(mpi_in, mpi_out, 7, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            (*b)[name].set_globalC(ifdrift, mpi_out);
            if ( n0[i] > 0 ){
                for (G4int k = 0; k < 7; k++){
                    mpi_in[k] = 0;
                    mpi_out[k] = 0;
                }
                (*b)[name].StandardDeviation(input, ifdrift, mpi_in);
                for (G4int l = 0; l < 7; l++) mpi_in[l] = mpi_in[l]/dnpT;
            }
            MPI_Allreduce(mpi_in, mpi_out, 7, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            (*b)[name].set_globalSTD(ifdrift, mpi_out);
            if ( n0[i] > 0 ){
                for (G4int mi = 0; mi < 7; mi++){
                    mpi_in[mi] = 0;
                    mpi_out[mi] = 0;
                }
                (*b)[name].Divergence(mpi_in);
                for (G4int n = 0; n < 4; n++) mpi_in[n]=mpi_in[n]/dnpT;
            }
            MPI_Allreduce(mpi_in, mpi_out, 4, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            (*b)[name].set_globalDiv(ifdrift, mpi_out);
            if ( n0[i] > 0 ){
                for (G4int o=0; o < 4; o++){
                    mpi_in[o] = mpi_out[o];
                    mpi_out[o] = 0;
                }
                (*b)[name].Div_Emittance(ifdrift, mpi_in);
                for (G4int p = 0; p < 6; p++) mpi_in[p]=mpi_in[p]/dnpT;
            }
            MPI_Allreduce(mpi_in, mpi_out, 6, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            (*b)[name].set_globalDivEm(ifdrift, mpi_out, &nT[i]);
            if ( n0[i] > 0 ){
                for (G4int q = 0; q < 7; q++){
                    mpi_in[q] = 0;
                    mpi_out[q] = 0;
                }
            }
            G4int nsteps0 = (*b)[name].get_nsteps();
            MPI_Allreduce(&nsteps0, nsT, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            (*b)[name].set_nsteps(nsT);
        }
    }
}

/**
 * Function to store beams mass and charge information merged accross all MPI ranks (for openPMD).
 * 
 * @param[in] b Pointer to beam data
 * @param[in] n0  Pointer to number of beam particles in each rank
 * @param[in] nT Pointer to total number of particles accross all MPI ranks
 */
void Properties::ShareMQ (map<G4String,PartBeams> * b, G4int * n0, G4int * nT)
{
    G4int nspecies = input.s_list.size();
    for (G4int i = 0; i < nspecies ; ++i){
        if ( nT[i] > 0 ){
            G4String name = input.s_list[i];
            if ( n0[i] > 0 ){
                mpi_in[0] = (*b)[name].mass;
                mpi_in[1] = (*b)[name].charge;
                mpi_in[2] = 1.0;
            }
            else {
                mpi_in[0] = 0.0;
                mpi_in[1] = 0.0;
                mpi_in[2] = 0.0;
            }
            MPI_Allreduce(mpi_in, mpi_out, 3, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
            (*b)[name].set_MQ(mpi_out[0]/mpi_out[2], mpi_out[1]/mpi_out[2]);
            if ( n0[i] > 0 ){
                for (G4int j = 0; j < 3; j++){
                    mpi_in[j] = 0;
                    mpi_out[j] = 0;
                }
            }
        }
    }
}