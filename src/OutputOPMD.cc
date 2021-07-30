/**
 * @file OutputOPMD.cc
 * @author Ligia Diana Amorim
 * @date 06/2021
 * @copyright GPos 2021 LBNL
 */

#include "OutputOPMD.hpp"

using CLHEP::e_SI;

/**
 * \ingroup OutputOPMD
 * Functions to generated openPMD files with each beam data at Start, Foil and Drift.
 * To reduce memory load, particle creation processes are mapped to integers.
 * 
 * @param[in] process Label of the process that generated each particle.
 */
G4int process_map (G4String process){
    G4int p_id = 50;
    vector<G4String> p_map{ "conv","Decay","annihil", "eIoni", "phot", "compt", "photonNuclear",
    "electronNuclear", "hBertiniCaptureAtRest","primary", "protonInelastic","neutronInelastic",
    "CoulombScat", "positronNuclear", "hIoni","pi-Inelastic", "tInelastic","hadElastic", "nCapture",
    "dInelastic","pi+Inelastic", "sigma+Inelastic", "eBrem", "prim"
    };

    for (vector<G4int>::size_type i = 0; i < p_map.size(); ++i){
        if (process.compareTo(p_map[i]) == 0)
        {
            p_id = i;
        }
    }
    if (p_id == 50)
    {
        G4ExceptionDescription msg;
        msg << "\nError: process "<< process <<" was not found in list\n";
        G4Exception("process_map()", "GPos error #11.0",JustWarning,msg);
        G4cout << G4endl;
        p_id = 50;
    }
    return p_id;
}


/**
 * \ingroup OutputOPMD
 * Function that stores all relevant particle data in openPMD format accross all MPI ranks.
 * 
 * @param[in] in Parameters read from input.txt file. 
 * @param[in] b Each species particles (= beam) information.
 * @param[in] nT Total number of particles merged accross MPI ranks.
 * @param[in] nS Number of particles added up to each MPI rank.
 * @param[in] label Spatial run identifier - at Start, Foil, Drift.
 */
void save_outOPMD (Query in, map<G4String,PartBeams> * b, G4int * nT, G4int * nS, G4String * label){
    auto scalar = RecordComponent::SCALAR;
    G4int np;
    // Saving each beam into openPMD h5 format
    G4int nspecies = in.s_list.size();
    for (G4int i = 0; i < nspecies; ++i){
        vector<G4int> i_process;
        G4String name = in.s_list[i];
        if (nT[i] > 0){
            if ( b->find(name) != b->end() ){
                np = (*b)[name].np;
                i_process.reserve(np);
                for (G4int j = 0; j < np; j++)
                {
                    i_process.push_back(process_map((*b)[name].process[j]));
                }
            }
            else
            {
                np = 0;
            }
            G4String fname;
            if(!label->empty()) fname = name+*label+"_opmd.h5";
            else fname = name+"_opmd.h5";

            Series series = Series(fname, Access::CREATE, MPI_COMM_WORLD);
            Iteration& iter = series.iterations[1];
            ParticleSpecies& part=iter.particles[name];

            Datatype ddouble = determineDatatype<G4double>();
            Extent g_extent = {(unsigned long long) nT[i]};
            Dataset d = Dataset(ddouble, g_extent);

            /*
            Mass and charge are scalars instead of vectors to relieve memory load
            For that reason openPMD needs makeConstant()
            If particles do not exist in an MPI rank, then this will stall the simulation
            So mass and charge data was merged prior - in Properties.cc
            */
            Record mass = part["mass"];
            mass.setUnitDimension({{UnitDimension::M,1}});
            RecordComponent mass_scalar = mass[scalar];
            mass_scalar.resetDataset( d );
            mass_scalar.setUnitSI(1.7826619216279e-30);
            mass_scalar.makeConstant((*b)[name].mass);
            
            Record charge = part["charge"];
            charge.setUnitDimension({{UnitDimension::I,1},{UnitDimension::T,1}});
            RecordComponent charge_scalar = charge[scalar];
            charge_scalar.resetDataset( d );
            charge_scalar.setUnitSI(1.0);
            charge_scalar.makeConstant((*b)[name].charge*e_SI);
            
            //Datatype datatype = determineDatatype(shareRaw((*b)[name].x));
            part["position"].setUnitDimension({{UnitDimension::L,1}});
            part["position"]["x"].resetDataset( d );
            part["position"]["x"].setUnitSI(1.e-3);
            part["positionOffset"]["x"].resetDataset( d );
            part["positionOffset"]["x"].makeConstant( 0.0 );

            part["position"]["y"].resetDataset( d );
            part["position"]["y"].setUnitSI(1.e-3);
            part["positionOffset"]["y"].resetDataset( d );
            part["positionOffset"]["y"].makeConstant( 0.0 );

            part["position"]["z"].resetDataset( d );
            part["position"]["z"].setUnitSI(1.e-3);
            part["positionOffset"]["z"].resetDataset( d );
            part["positionOffset"]["z"].makeConstant( 0.0 );

            part["momentum"].setUnitDimension({{UnitDimension::M,1}, {UnitDimension::L,1},
            {UnitDimension::T,-1}});
            part["momentum"]["x"].resetDataset( d );
            part["momentum"]["x"].setUnitSI(5.344285992638191e-22);

            part["momentum"]["y"].resetDataset( d );
            part["momentum"]["y"].setUnitSI(5.344285992638191e-22);

            part["momentum"]["z"].resetDataset( d );
            part["momentum"]["z"].setUnitSI(5.344285992638191e-22);

            Record energy = part["kinetic_energy"];
            energy.setUnitDimension({{UnitDimension::M,1},{UnitDimension::L,2},
                {UnitDimension::T,-2}});
            RecordComponent energy_scalar = energy[scalar];
            energy_scalar.resetDataset( d );
            energy_scalar.setUnitSI(1.6021773e-13);

            Record process = part["process_id"];
            RecordComponent process_scalar = process[scalar];
            Datatype datatype_p = determineDatatype(shareRaw(i_process));
            Dataset dset_p = Dataset(datatype_p, g_extent);
            process_scalar.resetDataset(dset_p);

            Record steps = part["steps"];
            RecordComponent steps_scalar = steps[scalar];
            Datatype datatype_s = determineDatatype<G4int>();
            Dataset dset_s = Dataset(datatype_s, g_extent);
            steps_scalar.resetDataset(dset_s);
            
            if (np > 0) {
                Extent c_extent = {(unsigned long long) np};
                Offset c_offset = {(unsigned long long) (nS[i]-np)};
                part["position"]["x"].storeChunk((*b)[name].x, c_offset, c_extent);
                part["position"]["y"].storeChunk((*b)[name].y, c_offset, c_extent);
                part["position"]["z"].storeChunk((*b)[name].z, c_offset, c_extent);
                part["momentum"]["x"].storeChunk((*b)[name].px, c_offset, c_extent);
                part["momentum"]["y"].storeChunk((*b)[name].py, c_offset, c_extent);
                part["momentum"]["z"].storeChunk((*b)[name].pz, c_offset, c_extent);
                energy_scalar.storeChunk((*b)[name].tEn, c_offset, c_extent);
                process_scalar.storeChunk( i_process, c_offset, c_extent);
                steps_scalar.storeChunk( (*b)[name].steps, c_offset, c_extent);
            }
            series.flush();
            G4cout << "Stored data into file " << fname << "\n";
        }
    }
}
