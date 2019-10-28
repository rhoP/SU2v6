/*!
 * \file CMLParamReader.cpp
 * \brief Reads a native ASCII file containing the values of machine learning parameters.
 * \author T. Economon
 * \version 6.2.0 "Falcon"
 *
 * The current SU2 release has been coordinated by the
 * SU2 International Developers Society <www.su2devsociety.org>
 * with selected contributions from the open-source community.
 *
 * The main research teams contributing to the current release are:
 *  - Prof. Juan J. Alonso's group at Stanford University.
 *  - Prof. Piero Colonna's group at Delft University of Technology.
 *  - Prof. Nicolas R. Gauger's group at Kaiserslautern University of Technology.
 *  - Prof. Alberto Guardone's group at Polytechnic University of Milan.
 *  - Prof. Rafael Palacios' group at Imperial College London.
 *  - Prof. Vincent Terrapon's group at the University of Liege.
 *  - Prof. Edwin van der Weide's group at the University of Twente.
 *  - Lab. of New Concepts in Aeronautics at Tech. Institute of Aeronautics.
 *
 * Copyright 2012-2019, Francisco D. Palacios, Thomas D. Economon,
 *                      Tim Albring, and the SU2 contributors.
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/toolboxes/CLinearPartitioner.hpp"
#include "../include/CMLParamReader.hpp"

CMLParamReader::CMLParamReader(CConfig        *val_config,
                                               unsigned short val_iZone,
                                               unsigned short val_nZone,
                                               unsigned long global_points)
        : CMeshReaderFVM(val_config, val_iZone, val_nZone) {

    /* Store the current zone to be read and the total number of zones. */
    myZone = val_iZone;
    nZones = val_nZone;

    /* Store the mesh filename since we will open/close multiple times. */
    MLParam_Filename = config->GetMLParam_FileName();

    /* Read the basic metadata and perform some basic error checks. */
    MLParam_file.open(MLParam_Filename.c_str(), ios::in);
    ReadMetadata();
    MatchParamsPoints(global_points);
    /* Read and store the parameter values */
    ReadParameterValues();

    MLParam_file.close();

}

CMLParamReader::~CMLParamReader() { }

void CMLParamReader::ReadMetadata() {

    bool harmonic_balance = config->GetUnsteady_Simulation() == HARMONIC_BALANCE;
    bool multizone_file = config->GetMultizone_Mesh();

    /*--- Check if parameter file is open ---*/
    if (MLParam_file.fail()) {
        SU2_MPI::Error(string("Error opening parameter file.") +
                       string(" \n Check if the file exists."), CURRENT_FUNCTION);
    }

    /*--- If more than one, find the curent zone in the mesh file. ---*/

    string text_line;
    string::size_type position;
    if ((nZones > 1 && multizone_file) || harmonic_balance) {
        if (harmonic_balance) {
            if (rank == MASTER_NODE)
                cout << "Reading time instance " << config->GetiInst()+1 << "." << endl;
        } else {
            bool foundZone = false;
            while (getline (MLParam_file,text_line)) {
                /*--- Search for the current domain ---*/
                position = text_line.find ("IZONE=",0);
                if (position != string::npos) {
                    text_line.erase (0,6);
                    unsigned short jZone = atoi(text_line.c_str());
                    if (jZone == myZone+1) {
                        if (rank == MASTER_NODE)
                            cout << "Reading zone " << myZone << " from SU2 parameter file." << endl;
                        foundZone = true;
                        break;
                    }
                }
            }
            if (!foundZone) {
                SU2_MPI::Error(string("Could not find the IZONE= keyword or the zone contents.") +
                               string(" \n Check the SU2 parameter file format."),
                               CURRENT_FUNCTION);
            }
        }
    }

    /*--- Read the metadata: total number of machine learning parameters. ---*/

    bool foundNPARA = false;

    while (getline (MLParam_file, text_line)) {

        /*--- Read the number of parameters of the problem ---*/

        position = text_line.find ("NPARA=",0);
        if (position != string::npos) {
            text_line.erase (0,6);
            numberOfMLParameters = atoi(text_line.c_str());
            for (unsigned long iPara = 0; iPara < numberOfMLParameters; iPara++)
                getline (MLParam_file, text_line);
            foundNPARA = true;
        }
    }

    /* Throw an error if the parameter keyword was not found. */
    if (!foundNPARA) {
        SU2_MPI::Error(string("Could not find NPARA= keyword.") +
                       string(" \n Check the SU2 parameter file format."),
                       CURRENT_FUNCTION);
    }
}

void CMLParamReader::ReadParameterValues() {

    /*--- Reserve memory for the vector of parameters ---*/

    ML_Parameters.reserve(numberOfMLParameters);

    /*--- Read the parameters into our data structure. ---*/

    string text_line;
    string::size_type position;
    getline (MLParam_file, text_line);
    position = text_line.find("NPARA=",0);
    while (MLParam_file.is_open() && (position != string::npos) ) {
        double par_val{0.0};
        MLParam_file >> par_val;
        ML_Parameters.push_back(par_val);
    }


}


void CMLParamReader::MatchParamsPoints(unsigned long global_points) {
    if(numberOfMLParameters != global_points){
        SU2_MPI::Error(string("Mismatch between the number of parameters and number of points in the problem ") +
                       string(" \n Check the parameter file."),
                       CURRENT_FUNCTION);
    }
}