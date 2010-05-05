//-*- Mode: C++ -*-
// $Id: AliHLTEMCALConstants.h 35357 2009-10-08 13:24:38Z phille $

/**************************************************************************
 * This file is property of and copyright by the Experimental Nuclear     *
 * Physics Group, Dep. of Physics                                         *
 * University of Oslo, Norway, 2006                                       *
 *                                                                        * 
 * Author: Svein Lindal slindal@fys.uio.no for the ALICE DCS Project.  *
 * Contributors are mentioned in the code where appropriate.              *
 * Please report bugs to slindal@fys.uio.no                                * 
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#ifndef ALIHLTEMCALCONSTANTS_H
#define ALIHLTEMCALCONSTANTS_H

class AliHLTCaloConstants;


class AliHLTEMCALConstants : public AliHLTCaloConstants
{
  
public:
  AliHLTEMCALConstants();
  ~AliHLTEMCALConstants();
  virtual void InitConstants(); 
  ClassDef(AliHLTEMCALConstants, 1)
};

#endif
