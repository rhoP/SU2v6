/*!
 * \file CTurb_ML_Structure.hpp
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

#pragma once
#include "./mpi_structure.hpp"

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <vector>

#include "config_structure.hpp"
#include "CMLParamReader.hpp"


class CMLParamReader;

/*!
 * \class CTurbML
 * \brief Main class for learning the turbulence model.
 * \ingroup Turbulence_Model
 * \author Rohit P.
 */

class CTurbML {

private:
    CConfig *config; /*!< \brief Local pointer to the config parameter object. */
    unsigned long numberOfMLParameters; /*!< \brief Number of parameter values in the parameter file. */
    string MLParam_Filename; /*!< \brief Name of the SU2 Parameter file being read. */
    ifstream MLParam_file;  /*!< \brief File object for the SU2 ASCII mesh file. */
    std::vector<su2double> ML_Parameters; /*!< \brief Vector containing the parameter values. */
    /*!
     * \brief Reads all SU2 ASCII mesh metadata and checks for errors.
     */
    void ReadMetadata();
    /*!
     * \brief Reads the grid points from an SU2 zone into linear partitions across all ranks.
     */
    void ReadParameterValues();
public:
    /*!
     * \brief Constructor of the CMLParamReader class.
     */
    CTurbML(CConfig *val_config, unsigned long global_points);
    /*!
     * \brief Destructor of the CMLParamReader class.
     */
    ~CTurbML();
    /*!
     * \brief Get the machine learning parameter.
     * \param[in] par_index - Index of point.
     * \return Value of the machine learning parameter.
     */
    su2double Get_iParamML(unsigned long par_index) {return ML_Parameters[par_index]; }
    /*!
     * \brief Set the machine learning parameter.
     * \param[in] par_index - Index of point.
     * \param[in] val_mlparam - New value of the machine learning parameter.
     */
    void Set_iParamML(su2double val_mlparam, unsigned long par_index) {
        ML_Parameters[par_index] = val_mlparam;
    }
    /*!
     * \brief Get the number of machine learning parameters.
     * \return Value of the machine learning parameter.
     */
    unsigned long Get_nParamML() {return numberOfMLParameters; }

    /*!
     * \brief Set the number of machine learning parameters.
     * \return Value of the machine learning parameter.
     */
    void Set_nParamsML(unsigned long val_nParams) {numberOfMLParameters = val_nParams; }

    /*!
     * \brief Match the number of parameters with the global number of points.
     * \return global number of points.
     */
    void MatchParamsPoints(unsigned long global_points);


};
