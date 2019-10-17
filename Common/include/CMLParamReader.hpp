/*!
 * \file CMLParamReader.hpp
 * \brief Header file for the class CMLParamReader.
 *        The implementations are in the <i>CMLParamReader.cpp</i> file.
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

#include "CMeshReaderFVM.hpp"

/*!
 * \class CMLParamReader
 * \brief Reads a native SU2 file containing values of machine learning parameter at each grid point.
 * \author: Rohit P.
 */
class CMLParamReader: public CMeshReaderFVM {

private:

    unsigned short myZone; /*!< \brief Current SU2 zone index. */
    unsigned short nZones; /*!< \brief Total number of zones in the SU2 file. */

    unsigned long numberOfMLParameters; /*!< \brief Number of parameter values in the parameter file. */

    string MLParam_Filename; /*!< \brief Name of the SU2 Parameter file being read. */
    ifstream MLParam_file;  /*!< \brief File object for the SU2 ASCII mesh file. */

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
    CMLParamReader(CConfig        *val_config,
                           unsigned short val_iZone,
                           unsigned short val_nZone);

    /*!
     * \brief Destructor of the CMLParamReader class.
     */
    ~CMLParamReader(void);

};
