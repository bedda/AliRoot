//-*- Mode: C++ -*-
// $Id$
#ifndef ALIHLTTPCHWCFEMULATOR_H
#define ALIHLTTPCHWCFEMULATOR_H

//* This file is property of and copyright by the ALICE HLT Project        * 
//* ALICE Experiment at CERN, All rights reserved.                         *
//* See cxx source for full Copyright notice                               *

#include "AliHLTTPCHWCFExtractorUnit.h"
#include "AliHLTTPCHWCFPeakFinderUnit.h"
#include "AliHLTTPCHWCFProcessorUnit.h"
#include "AliHLTTPCHWCFMergerUnit.h"
#include "AliHLTTPCHWCFDivisionUnit.h"

class AliHLTTPCClusterMCData;
class AliHLTTPCClusterMCLabel;

//  @class   AliHLTTPCHWCFEmulator
//  @author Sergey Gorbunov <sergey.gorbunov@fias.uni-frankfurt.de>
//  @author Torsten Alt <talt@cern.ch> 
//  @brief  FPGA ClusterFinder Emulator for TPC
//  @note
//
class AliHLTTPCHWCFEmulator 
{
 public:  

  /** standard constructor */
   AliHLTTPCHWCFEmulator();
  
  /** destructor */
  virtual ~AliHLTTPCHWCFEmulator();
   
  /** set debug level */
  void SetDebugLevel( int val ){ fDebug = val; }

  /** initialisation 
   */
  void Init( const AliHLTUInt32_t *mapping, AliHLTUInt32_t configWord1, AliHLTUInt32_t configWord2 );
  
  /** Loops over all rows finding the clusters 
   */
  int FindClusters( const AliHLTUInt32_t *rawEvent,
		    AliHLTUInt32_t rawEventSize32,
		    AliHLTUInt32_t *output,
		    AliHLTUInt32_t &outputSize32,
		    const AliHLTTPCClusterMCLabel *mcLabels,
		    AliHLTUInt32_t nMCLabels,
		    AliHLTTPCClusterMCData *outputMC
		    );

  /* useful tools */

  /** read the word written in big endian format (lowest byte first) 
   */
  static AliHLTUInt32_t ReadBigEndian( AliHLTUInt32_t word );

  /** write a word in big endian format (least byte first) 
   */
  static  AliHLTUInt32_t WriteBigEndian( AliHLTUInt32_t word );

  /** create configuration word 
   **/
  static void CreateConfiguration
    ( bool doDeconvTime, bool doDeconvPad, bool doFlowControl,  
      bool doSinglePadSuppression, bool bypassMerger, 
      AliHLTUInt32_t clusterLowerLimit,AliHLTUInt32_t singleSeqLimit, 
      AliHLTUInt32_t mergerDistance, bool useTimeBinWindow, AliHLTUInt32_t chargeFluctuation, bool useTimeFollow,
      AliHLTUInt32_t &configWord1, AliHLTUInt32_t &configWord2  );
 
  /** create default configuration word 
   **/
  static void CreateDefaultConfiguration( AliHLTUInt32_t &configWord1, AliHLTUInt32_t &configWord2 ){
    CreateConfiguration(0,0,0,1,0,0,0, 3, 1, 0, 1, configWord1, configWord2 );
  }

  /** set tagging of deconvoluted clusters
   **/
  void SetTagDeconvolutedClusters( AliHLTUInt32_t option ){ fDivisionUnit.SetTagDeconvolutedClusters( option ); }
  void SetTagEdgeClusters( AliHLTUInt32_t option ){ fDivisionUnit.SetTagEdgeClusters( option ); }

  /** set RCU2 flag (data is not split in two branches)
   **/
  void SetProcessingRCU2Data( bool b ){ fChannelExtractor.SetRCU2Flag( b ); }
  
  void SetNoiseSuppression(int ns) {fPeakFinderUnit.SetNoiseSuppression(ns);}
  void SetNoiseSuppressionMinimum(int ns) {fPeakFinderUnit.SetNoiseSuppressionMinimum(ns);}
  void SetNoiseSuppressionNeighbor(int n) {fPeakFinderUnit.SetNoiseSuppressionNeighbor(n);}
  void SetSmoothing(int s) {fPeakFinderUnit.SetSmoothing(s);}
  void SetSmoothingThreshold(int s) {fPeakFinderUnit.SetSmoothingThreshold(s);}
  void SetClusterQMaxLowerLimit(int l) {fDivisionUnit.SetClusterQMaxLowerLimit(l);}
  void SetImprovedDeconvolution(int d) {fChannelProcessor.SetImprovedDeconvolution(d);}
  void SetCorrectEdgeClusters(int c) {fDivisionUnit.SetCorrectEdgeClusters(c);}

 private: 

  /** copy constructor prohibited */
  AliHLTTPCHWCFEmulator(const AliHLTTPCHWCFEmulator&);
  /** assignment operator prohibited */
  AliHLTTPCHWCFEmulator& operator=(const AliHLTTPCHWCFEmulator&);
 
  int  fDebug; // debug level
  const AliHLTUInt32_t *fkMapping; //! mapping array
  AliHLTTPCHWCFExtractorUnit fChannelExtractor; //! transient
  AliHLTTPCHWCFPeakFinderUnit fPeakFinderUnit; //! transient
  AliHLTTPCHWCFProcessorUnit fChannelProcessor; //! transient
  AliHLTTPCHWCFMergerUnit    fChannelMerger; //! transient
  AliHLTTPCHWCFDivisionUnit  fDivisionUnit;   //! transient
};
#endif
