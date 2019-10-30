/*!
 * \file CTurb_ML_Structure.cpp
 * \brief Class for enhancing the turbulence model with machine learning.
 * \author Rohit P.
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


#include "CTurb_ML_Structure.hpp"
#include "./mpi_structure.hpp"

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "config_structure.hpp"
#include "../include/toolboxes/CLinearPartitioner.hpp"
/*
     * \brief  Constructor of the class CTurbML
     * \param[in] nParam - Number of Parameters of the Turbulence model.
     * \param[in] config - Definition of the particular problem.
*/
CTurbML::CTurbML(CConfig  *val_config, unsigned long global_points) {
    this->config = val_config;
    /* Store the mesh filename since we will open/close multiple times. */
    MLParam_Filename = config->GetMLParam_FileName();
    /* Read the basic metadata and perform some basic error checks. */
    ReadMetadata();
    MatchParamsPoints(global_points);
    /* Read and store the parameter values */
    ReadParameterValues();
}

CTurbML::~CTurbML() { }

void CTurbML::ReadMetadata() {
    MLParam_file.open(MLParam_Filename.c_str(), ios::in);

    /*--- Check if parameter file is open ---*/
    if (MLParam_file.fail()) {
        SU2_MPI::Error(string("Error opening parameter file.") +
                       string(" \n Check if the file exists."), CURRENT_FUNCTION);
    }
    string text_line;
    string::size_type position;
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
    MLParam_file.close();
}

void CTurbML::ReadParameterValues() {
    MLParam_file.open(MLParam_Filename.c_str(), ios::in);
    /*--- Reserve memory for the vector of parameters ---*/

    ML_Parameters.reserve(numberOfMLParameters);
    cout << "Reading the parameter values. " << endl;
    /*--- Read the parameters into our data structure. ---*/

    string text_line;
    string::size_type position;
    getline (MLParam_file, text_line);
    position = text_line.find("NPARA=",0);
    unsigned long ipar = 0;
    while (MLParam_file.is_open() && (ipar < numberOfMLParameters)) {
        double par_val{0.0};
        MLParam_file >> par_val;
        ML_Parameters.push_back(par_val);
        ipar++;
    }

    MLParam_file.close();
}


void CTurbML::MatchParamsPoints(unsigned long global_points) {
    if(numberOfMLParameters != global_points){
        SU2_MPI::Error(string("Mismatch between the number of parameters and number of points in the problem ") +
                       string(" \n Check the parameter file."),
                       CURRENT_FUNCTION);
    }
}













