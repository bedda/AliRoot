/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//=============================================================================================
//
//      Class describing geometry of one MFT half-disk support + PCBs
//
//      Contact author: raphael.tieulent@cern.ch
//
//=============================================================================================

#include "TGeoManager.h"
#include "TGeoTube.h"
#include "TGeoCone.h"
#include "AliMFTSupport.h"

ClassImp(AliMFTSupport)

//=============================================================================================

AliMFTSupport::AliMFTSupport():
TNamed(),
fSupportVolume(NULL),
fSupportThickness(0.8), // cm
fPCBThickness(0.1) // cm
{

  // default constructor

}


//=============================================================================================

AliMFTSupport::~AliMFTSupport() {

}


//=============================================================================================
TGeoVolumeAssembly* AliMFTSupport::CreateVolume(Int_t half, Int_t disk){

  AliInfo(Form("Creating Support and PCB for half %d and disk %d ",half, disk));



  fSupportVolume = new TGeoVolumeAssembly(Form("SupportPCB_%d_%d", half, disk));

  TGeoVolume * supportVolume =  CreateSupport(half, disk);
  TGeoVolumeAssembly * pcbVolume =  CreatePCBs(half, disk);


  // Place the core of the support
  fSupportVolume->AddNode(supportVolume, 1);


  // Place the front PCB
  fSupportVolume->AddNode(pcbVolume, 1,new TGeoTranslation(0.,0.,(fSupportThickness+ fPCBThickness)/2.));
  // Place the back PCB (supposing both fron and back are the same shapes)
  fSupportVolume->AddNode(pcbVolume, 2,new TGeoCombiTrans (0.,0.,-(fSupportThickness+ fPCBThickness)/2., new TGeoRotation("rot",0.,180.,0.)));



  return fSupportVolume;
}

//=============================================================================================
TGeoVolumeAssembly* AliMFTSupport::CreatePCBs(Int_t half, Int_t disk){

  AliInfo(Form("Creating PCB for half %d and disk %d ",half, disk));

  TGeoVolumeAssembly * pcbVolume;// = new TGeoVolumeAssembly(Form("PCB_%d_%d", half, disk));

  // Create PCBs
    switch (disk) {
      case 0: pcbVolume =  PCB_00_01(half, disk);
        break;
      case 1: pcbVolume =  PCB_00_01(half, disk);
        break;
      case 2: pcbVolume =  PCB_02(half, disk);
        break;
      case 3: pcbVolume =  PCB_03(half, disk);
        break;
      case 4: pcbVolume =  PCB_04(half, disk);
        break;
    }

  return pcbVolume;
}


//=============================================================================================
TGeoVolumeAssembly* AliMFTSupport::PCB_00_01(Int_t half, Int_t disk){

  TGeoVolumeAssembly * pcbVolume = new TGeoVolumeAssembly(Form("PCB_%d_%d", half, disk));

  // Create Shapes
  Double_t phiMin =0., phiMax=180.;
  Double_t rMin =0., rMax=17.5; // units are cm
  Double_t copperThickness = 0.05; //units are cm
  Double_t fr4Thickness = fPCBThickness - copperThickness;
  Double_t t_delta = 0.01;
  Double_t z_excess=2.0*fPCBThickness;

  TGeoTubeSeg *varnishShape = new TGeoTubeSeg("tube_varnish_00",rMin, rMax, fr4Thickness/2., phiMin+180., phiMax+180.);
  TGeoTubeSeg *copperShape = new TGeoTubeSeg("tube_copper_00",rMin, rMax, copperThickness/2., phiMin+180., phiMax+180.);


// ========================== inner box out PCB_00 ======================================================

// ---------------------- Volume data -------------------------------------
Double_t Box00_dy=2.5;                                                              // Box00
Double_t Box01_dX=12.7, Box01_dY = 7.6;                                              // Box01
Double_t Box02_dX=3.25/2, Box02_dY = 1.9/2; Double_t Box02_X=11.075, Box02_Y = -7.96;  // Box02
Double_t Box03_dX=15.0/2, Box03_dY = 11.9; Double_t Box03_X=0.5/2, Box03_Y = 0.0;    // Box03
Double_t Box04_dX=4.8/2, Box04_dY = 14.2; Double_t Box04_X=0.5/2, Box04_Y = 0.0;      // Box04
Double_t Box05_dX=.251/2, Box05_dY = .491/2; Double_t Box05_X=-14.75/2, Box05_Y = -9.155;      // Box05
Double_t Box06_dX=10.65, Box06_dY = 8.91;       // Box06

// ------------------------ Translations ---------------------------------
TGeoTranslation *tr_Box02= new TGeoTranslation ("PCB0tr_Box02",Box02_X,Box02_Y,0.0); tr_Box02-> RegisterYourself();
TGeoTranslation *tr_Box03= new TGeoTranslation ("PCB0tr_Box03",Box03_X,Box03_Y,0.0); tr_Box03-> RegisterYourself();
TGeoTranslation *tr_Box04= new TGeoTranslation ("PCB0tr_Box04",Box04_X,Box04_Y,0.0); tr_Box04-> RegisterYourself();
TGeoTranslation *tr_Box05= new TGeoTranslation ("PCB0tr_Box05",Box05_X,Box05_Y,0.0); tr_Box05-> RegisterYourself();

// -------------------------- Volume Box ---------------------------------

  TGeoBBox   *Box00 = new TGeoBBox("Box00_PCB_00",rMax+t_delta, Box00_dy, z_excess/2);
  TGeoBBox   *Box01 = new TGeoBBox("Box01_PCB_00",Box01_dX, Box01_dY, z_excess/2);
  TGeoBBox   *Box02v = new TGeoBBox("Box02_PCB_00v",Box02_dX, Box02_dY, fr4Thickness/2); // + box vanrnish
  TGeoBBox   *Box02c = new TGeoBBox("Box02_PCB_00c",Box02_dX, Box02_dY, copperThickness/2); // + box copper
  TGeoBBox   *Box03 = new TGeoBBox("Box03_PCB_00",Box03_dX, Box03_dY, z_excess/2);
  TGeoBBox   *Box04 = new TGeoBBox("Box04_PCB_00",Box04_dX, Box04_dY, z_excess/2);
  TGeoBBox   *Box05 = new TGeoBBox("Box05_PCB_00",Box05_dX, Box05_dY, 2.0*z_excess/2);
  TGeoBBox   *Box06 = new TGeoBBox("Box06_PCB_00",Box06_dX, Box06_dY, z_excess/2);


  TGeoCompositeShape *box_outc = new TGeoCompositeShape
  ("box_out", "Box00_PCB_00+Box01_PCB_00+Box03_PCB_00:PCB0tr_Box03+Box04_PCB_00+Box06_PCB_00");


// ========================== botton+lateral box out PCB_00 ======================================================

// ---------------------- Volume data -------------------------------------

Double_t Box07_dX=8.1,    Box07_dY = 1.0;  Double_t Box07_X=0.0,      Box07_Y = -16.5;  // Box07
Double_t Box08_dX=4.2225, Box08_dY = .60;   Double_t Box08_X=0.,       Box08_Y = -15.5;  // Box08.
Double_t Box09_dX=1.35,  Box09_dY = 2.7;  Double_t Box09_X=0.0,      Box09_Y = 0.0;    // Box09
Double_t Box10_dX=1.35/2,   Box10_dY = 1.61/2;   Double_t Box10_X=1.35,     Box10_Y = 3.5;    // Box10
Double_t lat01_X=16.1,  lat01_Y = -6.8;   // lateral01
Double_t lat02_X=-16.1,  lat02_Y = -6.8;   // lateral02

Double_t Arc_01_dR0=.5, Arc_01_dR=.8,  Arc_01_phy0 = 90.0, Arc_01_phy = 180.0, Arc_01_X=-.85,Arc_01_Y = 2.2;  // Arc01


// ------------------------ Translations ---------------------------------

TGeoTranslation *tr_Box07= new TGeoTranslation ("PCB0tr_Box07",Box07_X,Box07_Y,0.0); tr_Box07-> RegisterYourself();
TGeoTranslation *tr_Box08= new TGeoTranslation ("PCB0tr_Box08",Box08_X,Box08_Y,0.0); tr_Box08-> RegisterYourself();
TGeoTranslation *tr_Box09= new TGeoTranslation ("PCB0tr_Box09",Box09_X,Box09_Y,0.0); tr_Box09-> RegisterYourself();
TGeoTranslation *tr_Box10= new TGeoTranslation ("PCB0tr_Box10",Box10_X,Box10_Y,0.0); tr_Box10-> RegisterYourself();
TGeoTranslation *tr_Arc_01= new TGeoTranslation ("PCB0tr_Arc_01",Arc_01_X,Arc_01_Y,0.0); tr_Arc_01-> RegisterYourself();
TGeoTranslation *tr_lateral_01= new TGeoTranslation ("PCB0tr_lateral_01",lat01_X,lat01_Y,0.0); tr_lateral_01-> RegisterYourself();

TGeoRotation   *rot1 = new TGeoRotation("rot1", 180., 180., 0.);
TGeoCombiTrans *tr_lateral_02= new TGeoCombiTrans ("PCB0tr_lateral_02",lat02_X,lat02_Y,0.0, rot1); tr_lateral_02-> RegisterYourself();

// -------------------------- Volume Box ---------------------------------

  TGeoBBox   *Box07 = new TGeoBBox("Box07_PCB_00",Box07_dX, Box07_dY, z_excess/2);
  TGeoBBox   *Box08 = new TGeoBBox("Box08_PCB_00",Box08_dX, Box08_dY, z_excess/2);
  TGeoBBox   *Box09 = new TGeoBBox("Box09_PCB_00",Box09_dX, Box09_dY, z_excess/2);
  TGeoBBox   *Box10 = new TGeoBBox("Box10_PCB_00",Box10_dX, Box10_dY, 1.1*z_excess/2);
  TGeoTubeSeg *Arc_01 = new TGeoTubeSeg("Arc01_PCB_00",Arc_01_dR0,Arc_01_dR, z_excess/2,Arc_01_phy0,Arc_01_phy);

// -------------- composite botton ----------------------------

   TGeoCompositeShape *box_outb = new TGeoCompositeShape  ("box_outb", "Box07_PCB_00:PCB0tr_Box07-Box08_PCB_00:PCB0tr_Box08");
   TGeoCompositeShape *box_outl = new TGeoCompositeShape  ("box_outl", "Box10_PCB_00:PCB0tr_Box10+(Box09_PCB_00-Arc01_PCB_00:PCB0tr_Arc_01)");

// ======================  holes =========================================

// ---------------------- Volume data ------------------------------------
Double_t Box0a_dX=0.3/2,    Box0a_dY = .3/2;   Double_t Box0a_X=0.0,      Box0a_Y = 0.0; // Box0a
Double_t Tub01_R0=0.0,      Tub01_R = .3/2;   Double_t Tub01_X=14.0,     Tub01_Y=-9.5;  // hole01.
Double_t Tub02_R0=0.0,      Tub02_R = .35/2;  Double_t Tub02_X=11.2,     Tub02_Y=-9.5;  // hole02.
Double_t Tub0dh_X=.3/2,     Tub0dh_Y=0.0;  						 // double hole.
Double_t Tub0dhp_X=-14.0,    Tub0dhp_Y=-9.5;  						 // double hole position.
//--------------------- translations ------------------------------
TGeoTranslation *tr_Tub01= new TGeoTranslation ("PCB0tr_Tub01",Tub01_X,Tub01_Y,0.0);    tr_Tub01-> RegisterYourself();
TGeoTranslation *tr_Tub02l= new TGeoTranslation ("PCB0tr_Tub02l",-Tub02_X,Tub02_Y,0.0); tr_Tub02l-> RegisterYourself();
TGeoTranslation *tr_Tub02r= new TGeoTranslation ("PCB0tr_Tub02r",Tub02_X,Tub02_Y,0.0);  tr_Tub02r-> RegisterYourself();
// ----------------- double hole --------------------------
TGeoTranslation *tr_Tub0dhl= new TGeoTranslation ("PCB0tr_Tub0dhl",-Tub0dh_X,Tub0dh_Y,0.0); tr_Tub0dhl-> RegisterYourself();
TGeoTranslation *tr_Tub0dhr= new TGeoTranslation ("PCB0tr_Tub0dhr",Tub0dh_X,Tub0dh_Y,0.0); tr_Tub0dhr-> RegisterYourself();
TGeoTranslation *tr_Tub0dhp= new TGeoTranslation ("PCB0tr_Tub0dhp",Tub0dhp_X,Tub0dhp_Y,0.0); tr_Tub0dhp-> RegisterYourself();
// -------------------------- Volume holes ---------------------------------
TGeoBBox   *Box0a = new TGeoBBox("Box0a_PCB_00",Box0a_dX, Box0a_dY, z_excess/2);
TGeoTube   *Tube01 = new TGeoTube("Tube01_PCB_00",Tub01_R0, Tub01_R,z_excess/2);
TGeoTube   *Tube02 = new TGeoTube("Tube02_PCB_00",Tub02_R0, Tub02_R,z_excess/2);
TGeoCompositeShape *Doub_hole = new TGeoCompositeShape  ("Doub_hole", "Box0a_PCB_00+Tube01_PCB_00:PCB0tr_Tub0dhl +Tube01_PCB_00:PCB0tr_Tub0dhr");
// -------------- composite holes ----------------------------
TGeoCompositeShape *holes = new TGeoCompositeShape  ("holes", "Tube01_PCB_00:PCB0tr_Tub01+Doub_hole:PCB0tr_Tub0dhp+Tube02_PCB_00:PCB0tr_Tub02l+Tube02_PCB_00:PCB0tr_Tub02r ");


// ======================== final composition ============================


  TGeoCompositeShape *comp_varnish = new TGeoCompositeShape ("varnish_00", "tube_varnish_00-box_out+Box02_PCB_00v:PCB0tr_Box02-(box_outb +box_outl:PCB0tr_lateral_01+box_outl:PCB0tr_lateral_02+Box05_PCB_00:PCB0tr_Box05+holes)");
  TGeoCompositeShape *comp_copper = new TGeoCompositeShape ("copper_00",    "tube_copper_00-box_out+Box02_PCB_00c:PCB0tr_Box02-(box_outb +box_outl:PCB0tr_lateral_01+box_outl:PCB0tr_lateral_02+Box05_PCB_00:PCB0tr_Box05+holes)");



  // Get Mediums
  TGeoMedium *medFR4  = gGeoManager->GetMedium("MFT_FR4$");
  TGeoMedium *medCu  = gGeoManager->GetMedium("MFT_Cu$");

  // Create Volumes
  TGeoVolume *varnishVol = new TGeoVolume(Form("Varnish_%d_%d", half, disk), comp_varnish, medFR4);
  varnishVol->SetVisibility(kTRUE);
  varnishVol->SetLineColor(kGreen);
  varnishVol->SetLineWidth(1);
  varnishVol->SetFillColor(varnishVol->GetLineColor());
  varnishVol->SetFillStyle(4000); // 0% transparent

  TGeoVolume *copperVol = new TGeoVolume(Form("Copper_%d_%d", half, disk), comp_copper, medCu);
  copperVol->SetVisibility(kTRUE);
  copperVol->SetLineColor(kOrange);
  copperVol->SetLineWidth(1);
  copperVol->SetFillColor(copperVol->GetLineColor());
  copperVol->SetFillStyle(4000); // 0% transparent

  // Position Volumes in the mother PCB Volume
  pcbVolume->AddNode(varnishVol, 1,new TGeoTranslation(0.,0.,fr4Thickness/2.));
  pcbVolume->AddNode(copperVol, 1,new TGeoTranslation(0.,0.,-copperThickness/2.));
  return pcbVolume;
}

//******************************* PCB_00 end *****************************************************


TGeoVolumeAssembly* AliMFTSupport::PCB_02(Int_t half, Int_t disk){

  TGeoVolumeAssembly * pcbVolume = new TGeoVolumeAssembly(Form("PCB_%d_%d", half, disk));

  // Create Shapes
  Double_t phiMin =180., phiMax=360.;
  Double_t rMin =0., rMax=17.5; // units are cm
  Double_t copperThickness = 0.05; //units are cm
  Double_t fr4Thickness = fPCBThickness - copperThickness;
  Double_t t_delta = 0.01;
  Double_t z_excess=2.0*fPCBThickness;

// ========================== inner box out PCB_01 ===========================================

// ---------------------- Volume data -------------------------------------
Double_t Box1a_dY=2.5;                                                              // Box10
Double_t Box11_dX=13.1, Box11_dY = 7.6;                                              // Box11
Double_t Box12_dX=.975, Box12_dY = .295; Double_t Box12_X=-12.125, Box12_Y = -7.305;  // +Box12
Double_t Box13_dX=10.9, Box13_dY = 8.91; Double_t Box13_X=-0.5/2, Box13_Y = 0.0;    // Box13
Double_t Box14_dX=9.2, Box14_dY = 11.92; Double_t Box14_X=-0.5/2, Box14_Y = 0.0;      // Box14
Double_t Box15_dX=5.8, Box15_dY = 14.93; Double_t Box15_X=-0.5/2, Box15_Y = 0.0;      // Box15
Double_t Box16_dX=2.7, Box16_dY = .91;  Double_t Box16_X=-0.5/2, Box16_Y = -14.93;      // +Box16

// ------------------------ Translations ---------------------------------
TGeoTranslation *tr_Box12= new TGeoTranslation ("PCB0tr_Box12",Box12_X,Box12_Y,0.0); tr_Box12-> RegisterYourself();
TGeoTranslation *tr_Box13= new TGeoTranslation ("PCB0tr_Box13",Box13_X,Box13_Y,0.0); tr_Box13-> RegisterYourself();
TGeoTranslation *tr_Box14= new TGeoTranslation ("PCB0tr_Box14",Box14_X,Box14_Y,0.0); tr_Box14-> RegisterYourself();
TGeoTranslation *tr_Box15= new TGeoTranslation ("PCB0tr_Box15",Box15_X,Box15_Y,0.0); tr_Box15-> RegisterYourself();
TGeoTranslation *tr_Box16= new TGeoTranslation ("PCB0tr_Box16",Box16_X,Box16_Y,0.0); tr_Box16-> RegisterYourself();

// -------------------------- Volume Box ---------------------------------

  TGeoBBox   *Box1a = new TGeoBBox("Box1a_PCB_01",rMax+t_delta, Box1a_dY, z_excess/2);
  TGeoBBox   *Box11 = new TGeoBBox("Box11_PCB_01",Box11_dX, Box11_dY, z_excess/2);
  TGeoBBox   *Box12v = new TGeoBBox("Box12_PCB_01v",Box12_dX, Box12_dY, fr4Thickness/2); // + box vanrnish
  TGeoBBox   *Box12c = new TGeoBBox("Box12_PCB_01c",Box12_dX, Box12_dY, copperThickness/2); // + box copper
  TGeoBBox   *Box13 = new TGeoBBox("Box13_PCB_01",Box13_dX, Box13_dY, z_excess/2);
  TGeoBBox   *Box14 = new TGeoBBox("Box14_PCB_01",Box14_dX, Box14_dY, z_excess/2);
  TGeoBBox   *Box15 = new TGeoBBox("Box15_PCB_01",Box15_dX, Box15_dY, z_excess/2);
  TGeoBBox   *Box16v = new TGeoBBox("Box16_PCB_01v",Box16_dX, Box16_dY, fr4Thickness/2); // + box vanrnish
  TGeoBBox   *Box16c = new TGeoBBox("Box16_PCB_01c",Box16_dX, Box16_dY, copperThickness/2); // + box copper

  TGeoCompositeShape *box_outc= new TGeoCompositeShape
  ("box_outc", "Box1a_PCB_01+Box11_PCB_01+Box13_PCB_01:PCB0tr_Box13+Box14_PCB_01:PCB0tr_Box14+Box15_PCB_01:PCB0tr_Box15");


// ======================== lateral box out PCB_01 ============================================

// ---------------------- Volume data -------------------------------------

Double_t Box17_dX=.61,    Box17_dY = .81;  Double_t Box17_X=15.4,      Box17_Y = -3.3;  // Box17
Double_t Box18_dX=1.31, Box18_dY = 5.0;   Double_t Box18_X=16.15,       Box18_Y = -7.5;  // Box18.


Double_t Arc01_01_dR0=.5, Arc01_01_dR=.85,  Arc01_01_phy0 = 90.0, Arc01_01_phy = 180.0, Arc01_01_X=-15.3, Arc01_01_Y = -4.6;  // Arc01

// ------------------------ Translations ---------------------------------

TGeoRotation   *rot2 = new TGeoRotation("rot2", 180., 180., 0.);
TGeoCombiTrans *tr_lateral_03= new TGeoCombiTrans ("PCB0tr_lateral_03",0,0,0, rot2); tr_lateral_03-> RegisterYourself();
TGeoTranslation *tr_Box17= new TGeoTranslation ("PCB0tr_Box17",Box17_X,Box17_Y,0.0); tr_Box17-> RegisterYourself();
TGeoTranslation *tr_Box18= new TGeoTranslation ("PCB0tr_Box18",Box18_X,Box18_Y,0.0); tr_Box18-> RegisterYourself();
TGeoTranslation *tr_Arc01= new TGeoTranslation ("PCB0tr_Arc01",15.3,-4.6,0.0); tr_Arc01-> RegisterYourself();

// -------------------------- Volume lateral ---------------------------------

  TGeoBBox   *Box17 = new TGeoBBox("Box17_PCB_01",Box17_dX, Box17_dY, z_excess/2);
  TGeoBBox   *Box18 = new TGeoBBox("Box18_PCB_01",Box18_dX, Box18_dY, z_excess/2);

  TGeoTubeSeg *Arc01_01 = new TGeoTubeSeg("Arc01_PCB_01",Arc01_01_dR0,Arc01_01_dR, z_excess/2,Arc01_01_phy0,Arc01_01_phy);

   TGeoCompositeShape *box_outl_01a = new TGeoCompositeShape  ("box_outl_01a", "Box18_PCB_01:PCB0tr_Box18-(Box17_PCB_01:PCB0tr_Box17+Arc01_PCB_01:PCB0tr_Arc01)");
   TGeoCompositeShape *box_outl_01 = new TGeoCompositeShape  ("box_outl_01", "box_outl_01a+box_outl_01a:PCB0tr_lateral_03");

// ======================== botton box out PCB_01 ============================================

// ---------------------- Volume data -------------------------------------

Double_t Box19_dX=4.2225,    Box19_dY = .3;  Double_t Box19_X=0,      Box19_Y = -17.2;  // Box19
Double_t Box1b0_dX=13./2, Box1b_dY = .327;   Double_t Box1b_X=0,       Box1b_Y = -16.573;  // Box1b.

Double_t Arc02_01_dR0=.5, Arc02_01_dR=1.2,  Arc02_01_phy0 = 90.0, Arc02_01_phy = 180.0, Arc02_01_X=7., Arc02_01_Y = -16.5;  // Arc02
Double_t Arc02_02_dR0=.5, Arc02_02_dR=1.2,  Arc02_02_phy0 = 0.0, Arc02_02_phy = 90.0, Arc02_02_X=-7., Arc02_02_Y = -16.5;  // Arc02

// ------------------------ Translations ---------------------------------

TGeoRotation   *rot3 = new TGeoRotation("rot2", 180., 180., 0.);
TGeoCombiTrans *tr_lateral_04= new TGeoCombiTrans ("PCB0tr_lateral_04",0,0,0, rot2); tr_lateral_04-> RegisterYourself();
TGeoTranslation *tr_Box19= new TGeoTranslation ("PCB0tr_Box19",Box19_X,Box19_Y,0.0); tr_Box19-> RegisterYourself();
TGeoTranslation *tr_Box1b= new TGeoTranslation ("PCB0tr_Box1b",Box1b_X,Box1b_Y,0.0); tr_Box1b-> RegisterYourself();
TGeoTranslation *tr_Arc02= new TGeoTranslation ("PCB0tr_Arc02",Arc02_01_X,Arc02_01_Y ,0.0); tr_Arc02-> RegisterYourself();
TGeoTranslation *tr_Arc02b= new TGeoTranslation ("PCB0tr_Arc02b",Arc02_02_X,Arc02_02_Y ,0.0); tr_Arc02b-> RegisterYourself();

// -------------------------- Volume botton ---------------------------------

  TGeoBBox   *Box19v = new TGeoBBox("Box19_PCB_01v",Box19_dX, Box19_dY, fr4Thickness/2+0.001);
  TGeoBBox   *Box1bv = new TGeoBBox("Box1b_PCB_01v",Box1b0_dX, Box1b_dY, fr4Thickness/2+0.001);

  TGeoTubeSeg *Arc02_01v = new TGeoTubeSeg("Arc02_PCB_01v",Arc02_01_dR0,Arc02_01_dR, fr4Thickness/2+0.001,Arc02_01_phy0,Arc02_01_phy);
  TGeoTubeSeg *Arc02_02v = new TGeoTubeSeg("Arc02_PCB_02v",Arc02_02_dR0,Arc02_02_dR, fr4Thickness/2+0.001,Arc02_02_phy0,Arc02_02_phy);

   TGeoCompositeShape *box_outb_01av = new TGeoCompositeShape  ("box_outb_01av", "Box19_PCB_01v:PCB0tr_Box19+(Box1b_PCB_01v:PCB0tr_Box1b+Arc02_PCB_01v:PCB0tr_Arc02+Arc02_PCB_02v:PCB0tr_Arc02b)");

// -------------------------- Volume botton ---------------------------------

  TGeoBBox   *Box19c = new TGeoBBox("Box19_PCB_01c",Box19_dX, Box19_dY, copperThickness/2);
  TGeoBBox   *Box1bc = new TGeoBBox("Box1b_PCB_01c",Box1b0_dX, Box1b_dY, copperThickness/2);

  TGeoTubeSeg *Arc02_01c = new TGeoTubeSeg("Arc02_PCB_01c",Arc02_01_dR0,Arc02_01_dR, copperThickness/2,Arc02_01_phy0,Arc02_01_phy);
  TGeoTubeSeg *Arc02_02c = new TGeoTubeSeg("Arc02_PCB_02c",Arc02_02_dR0,Arc02_02_dR, copperThickness/2,Arc02_02_phy0,Arc02_02_phy);

   TGeoCompositeShape *box_outb_01ac = new TGeoCompositeShape  ("box_outb_01ac", "Box19_PCB_01c:PCB0tr_Box19+(Box1b_PCB_01c:PCB0tr_Box1b+Arc02_PCB_01c:PCB0tr_Arc02+Arc02_PCB_02c:PCB0tr_Arc02b)");

// ======================  holes =========================================

// ---------------------- Volume data ------------------------------------
Double_t  Boxh11_dX=0.3/2,     Boxh11_dY = 0.3/2;   Double_t  Boxh11_X=0.0,       Boxh11_Y = 0.0; //  Boxh11
Double_t Tubh01_R0=0.0,      Tubh01_R = .3/2;   Double_t Tubh01_X=14.0,     Tubh01_Y=-9.5;  // holeh01.
Double_t Tubh02_R0=0.0,      Tubh02_R = .35/2;  Double_t Tubh02_X=11.2,     Tubh02_Y=-9.5;  // holeh02.
Double_t Tub0dh_X=.3/2,     Tub0dh_Y=0.0;  						 // double holeh.
Double_t Tub0dhp_X=-14.0,    Tub0dhp_Y=-9.5;  						 // double holeh position.
//--------------------- translations ------------------------------
TGeoTranslation *tr_Tubh01= new TGeoTranslation ("PCB0tr_Tubh01",Tubh01_X,Tubh01_Y,0.0);    tr_Tubh01-> RegisterYourself();
TGeoTranslation *tr_Tubh02l= new TGeoTranslation ("PCB0tr_Tubh02l",-Tubh02_X,Tubh02_Y,0.0); tr_Tubh02l-> RegisterYourself();
TGeoTranslation *tr_Tubh02r= new TGeoTranslation ("PCB0tr_Tubh02r",Tubh02_X,Tubh02_Y,0.0);  tr_Tubh02r-> RegisterYourself();
// ----------------- double holeh --------------------------
TGeoTranslation *tr_Tub0dhl= new TGeoTranslation ("PCB0tr_Tub0dhl",-Tub0dh_X,Tub0dh_Y,0.0); tr_Tub0dhl-> RegisterYourself();
TGeoTranslation *tr_Tub0dhr= new TGeoTranslation ("PCB0tr_Tub0dhr",Tub0dh_X,Tub0dh_Y,0.0); tr_Tub0dhr-> RegisterYourself();
TGeoTranslation *tr_Tub0dhp= new TGeoTranslation ("PCB0tr_Tub0dhp",Tub0dhp_X,Tub0dhp_Y,0.0); tr_Tub0dhp-> RegisterYourself();
// -------------------------- Volume holehs ---------------------------------
TGeoBBox   * Boxh11 = new TGeoBBox("Boxh11_PCB_01", Boxh11_dX,  Boxh11_dY, z_excess/2);
TGeoTube   *Tubeh01 = new TGeoTube("Tubeh01_PCB_01",Tubh01_R0, Tubh01_R,z_excess/2);
TGeoTube   *Tubeh02 = new TGeoTube("Tube02_PCB_01",Tubh02_R0, Tubh02_R,z_excess/2);
TGeoCompositeShape *Doub_holeh = new TGeoCompositeShape  ("Doub_holeh", "Boxh11_PCB_01+Tubeh01_PCB_01:PCB0tr_Tub0dhl +Tubeh01_PCB_01:PCB0tr_Tub0dhr");
// -------------- composite holehs ----------------------------
TGeoCompositeShape *holehs = new TGeoCompositeShape  ("holehs", "Tubeh01_PCB_01:PCB0tr_Tubh01+Doub_holeh:PCB0tr_Tub0dhp+Tube02_PCB_01:PCB0tr_Tubh02l+Tube02_PCB_01:PCB0tr_Tubh02r ");





  TGeoTubeSeg *varnishShape = new TGeoTubeSeg("tube_varnish_01",rMin, rMax, fr4Thickness/2., phiMin, phiMax);
  TGeoTubeSeg *copperShape = new TGeoTubeSeg("tube_copper_01",rMin, rMax, copperThickness/2., phiMin, phiMax);


// ======================== final composition ============================


  TGeoCompositeShape *comp_varnish = new TGeoCompositeShape ("varnish_01", "tube_varnish_01-box_outc-box_outl_01-holehs+box_outb_01av+Box12_PCB_01v:PCB0tr_Box12+Box16_PCB_01v:PCB0tr_Box16");
  TGeoCompositeShape *comp_copper = new TGeoCompositeShape ("copper_01",    "tube_copper_01-box_outc-box_outl_01-holehs+box_outb_01ac+Box12_PCB_01c:PCB0tr_Box12+Box16_PCB_01c:PCB0tr_Box16");


// Get Mediums
  TGeoMedium *medFR4  = gGeoManager->GetMedium("MFT_FR4$");
  TGeoMedium *medCu  = gGeoManager->GetMedium("MFT_Cu$");

  // Create Volumes
  TGeoVolume *varnishVol = new TGeoVolume(Form("Varnish_%d_%d", half, disk), comp_varnish, medFR4);
  varnishVol->SetVisibility(kTRUE);
  varnishVol->SetLineColor(kGreen);
  varnishVol->SetLineWidth(1);

  varnishVol->SetFillColor(varnishVol->GetLineColor());
  varnishVol->SetFillStyle(4000); // 0% transparent

  TGeoVolume *copperVol = new TGeoVolume(Form("Copper_%d_%d", half, disk), comp_copper, medCu);
  copperVol->SetVisibility(kTRUE);
  copperVol->SetLineColor(kOrange);
  copperVol->SetLineWidth(1);
  copperVol->SetFillColor(copperVol->GetLineColor());
  copperVol->SetFillStyle(4000); // 0% transparent

  // Position Volumes in the mother PCB Volume
  pcbVolume->AddNode(varnishVol, 1,new TGeoTranslation(0.,0.,fr4Thickness/2.));
  pcbVolume->AddNode(copperVol, 1,new TGeoTranslation(0.,0.,-copperThickness/2.));
  return pcbVolume;

}

//******************************* PCB_01 end *****************************************************
//=============================================================================================
TGeoVolumeAssembly* AliMFTSupport::PCB_03(Int_t half, Int_t disk){

  TGeoVolumeAssembly * pcbVolume = new TGeoVolumeAssembly(Form("PCB_%d_%d", half, disk));

  // Create Shapes
  Double_t phiMin =180., phiMax=360.;
  Double_t rMin =0., rMax=25.5; // units are cm
  Double_t copperThickness = 0.05; //units are cm
  Double_t fr4Thickness = fPCBThickness - copperThickness;
  Double_t t_delta = 0.01;
  Double_t z_excess=2.0*fPCBThickness;

// ========================== inner box out PCB_02 ===========================================

// ---------------------- Volume data -------------------------------------

Double_t Box21_dY=5.+ t_delta;                                              // Box21
Double_t Box22_dX=16.+ t_delta, Box22_dY = 7.0+ t_delta;                              // +Box22
Double_t Box23_dX=.975+ t_delta, Box23_dY = .3+ t_delta; Double_t Box23_X=15.025, Box23_Y =-7.3;    // Box23
Double_t Box24_dX=13.45+ t_delta, Box24_dY = .955+ t_delta; Double_t Box24_X=0.6, Box24_Y =-7.955;      // Box24
Double_t Box25_dX=.2+ t_delta, Box25_dY = .245+ t_delta; Double_t Box25_X=-11.35, Box25_Y = -9.155;      // Box25
Double_t Box26_dX=.85+ t_delta, Box26_dY = .195+ t_delta;  Double_t Box26_X=13.2, Box26_Y = -9.105;      // +Box26
Double_t Box27_dX=11.75+ t_delta, Box27_dY = 1.5+ t_delta;  Double_t Box27_X=.6, Box27_Y = -10.42;      // +Box27
Double_t Box28_dX=.2+ t_delta, Box28_dY = 0.24+ t_delta;  Double_t Box28_X=9.15, Box28_Y = -12.16;      // +Box28
Double_t Box29_dX=9.2+ t_delta, Box29_dY = 1.505+ t_delta;  Double_t Box29_X=-0.25, Box29_Y = -13.425;      // +Box29
Double_t Box2a_dX=4.1+ t_delta, Box2a_dY = .9+ t_delta;  Double_t Box2a_X=-0.25, Box2a_Y = -15.83;      // +Box2a
Double_t Box2b_dX=.85+ t_delta, Box2b_dY = .23+ t_delta;  Double_t Box2b_X=3., Box2b_Y = -16.96;      // +Box2b
Double_t Box2c_dX=2.4+ t_delta, Box2c_dY = .7+ t_delta;  Double_t Box2c_X=-0.25, Box2c_Y = -17.43;      // +Box2c

// ------------------------ Translations ---------------------------------

TGeoTranslation *tr_Box23= new TGeoTranslation ("PCB0tr_Box23",Box23_X,Box23_Y,0.0); tr_Box23-> RegisterYourself();
TGeoTranslation *tr_Box24= new TGeoTranslation ("PCB0tr_Box24",Box24_X,Box24_Y,0.0); tr_Box24-> RegisterYourself();
TGeoTranslation *tr_Box25= new TGeoTranslation ("PCB0tr_Box25",Box25_X,Box25_Y,0.0); tr_Box25-> RegisterYourself();
TGeoTranslation *tr_Box26= new TGeoTranslation ("PCB0tr_Box26",Box26_X,Box26_Y,0.0); tr_Box26-> RegisterYourself();
TGeoTranslation *tr_Box27= new TGeoTranslation ("PCB0tr_Box27",Box27_X,Box27_Y,0.0); tr_Box27-> RegisterYourself();
TGeoTranslation *tr_Box28= new TGeoTranslation ("PCB0tr_Box28",Box28_X,Box28_Y,0.0); tr_Box28-> RegisterYourself();
TGeoTranslation *tr_Box29= new TGeoTranslation ("PCB0tr_Box29",Box29_X,Box29_Y,0.0); tr_Box29-> RegisterYourself();
TGeoTranslation *tr_Box2a= new TGeoTranslation ("PCB0tr_Box2a",Box2a_X,Box2a_Y,0.0); tr_Box2a-> RegisterYourself();
TGeoTranslation *tr_Box2b= new TGeoTranslation ("PCB0tr_Box2b",Box2b_X,Box2b_Y,0.0); tr_Box2b-> RegisterYourself();
TGeoTranslation *tr_Box2c= new TGeoTranslation ("PCB0tr_Box2c",Box2c_X,Box2c_Y,0.0); tr_Box2c-> RegisterYourself();



// -------------------------- Volume Box ---------------------------------

  TGeoBBox   *Box21 = new TGeoBBox("Box21_PCB_02",rMax+t_delta, Box21_dY, z_excess/2);
  TGeoBBox   *Box22 = new TGeoBBox("Box22_PCB_02",Box22_dX, Box22_dY, z_excess/2);
  TGeoBBox   *Box23 = new TGeoBBox("Box23_PCB_02",Box23_dX, Box23_dY, z_excess/2); //
  TGeoBBox   *Box24 = new TGeoBBox("Box24_PCB_02",Box24_dX, Box24_dY, z_excess/2); //
  TGeoBBox   *Box25 = new TGeoBBox("Box25_PCB_02",Box25_dX, Box25_dY, z_excess/2);
  TGeoBBox   *Box26 = new TGeoBBox("Box26_PCB_02",Box26_dX, Box26_dY, z_excess/2);
  TGeoBBox   *Box27 = new TGeoBBox("Box27_PCB_02",Box27_dX, Box27_dY, z_excess/2);
  TGeoBBox   *Box28 = new TGeoBBox("Box28_PCB_02",Box28_dX, Box28_dY, z_excess/2); //
  TGeoBBox   *Box29 = new TGeoBBox("Box29_PCB_02",Box29_dX, Box29_dY, z_excess/2); //
  TGeoBBox   *Box2a = new TGeoBBox("Box2a_PCB_02",Box2a_dX, Box2a_dY, z_excess/2);
  TGeoBBox   *Box2b = new TGeoBBox("Box2b_PCB_02",Box2b_dX, Box2b_dY, z_excess/2); //
  TGeoBBox   *Box2c = new TGeoBBox("Box2c_PCB_02",Box2c_dX, Box2c_dY, z_excess/2); //


  TGeoCompositeShape *box_out02= new TGeoCompositeShape
  ("box_out02", "Box21_PCB_02+Box22_PCB_02+Box23_PCB_02:PCB0tr_Box23+Box24_PCB_02:PCB0tr_Box24+Box25_PCB_02:PCB0tr_Box25+Box26_PCB_02:PCB0tr_Box26+Box27_PCB_02:PCB0tr_Box27+Box28_PCB_02:PCB0tr_Box28+Box29_PCB_02:PCB0tr_Box29+Box2a_PCB_02:PCB0tr_Box2a+Box2b_PCB_02:PCB0tr_Box2b+Box2c_PCB_02:PCB0tr_Box2c");

// --------------------------  botton box ----------------------
Double_t Box2d_dX=2.0+ t_delta, Box2d_dY = 1.0+ t_delta;  Double_t Box2d_X=8., Box2d_Y = -24.5;      // +Box2d
Double_t Box2e_dX=7.+ t_delta, Box2e_dY = .7+ t_delta;  Double_t Box2e_X=0, Box2e_Y = -24.8;      // +Box2e
TGeoTranslation *tr_Box2d= new TGeoTranslation ("PCB0tr_Box2d",Box2d_X,Box2d_Y,0.0); tr_Box2d-> RegisterYourself();
TGeoTranslation *tr_Box2e= new TGeoTranslation ("PCB0tr_Box2e",Box2e_X,Box2e_Y,0.0); tr_Box2e-> RegisterYourself();
TGeoTranslation *tr_Box2f= new TGeoTranslation ("PCB0tr_Box2f",-Box2d_X,Box2d_Y,0.0); tr_Box2f-> RegisterYourself();

  TGeoBBox   *Box2d = new TGeoBBox("Box2d_PCB_02",Box2d_dX, Box2d_dY, z_excess/2);
  TGeoBBox   *Box2e = new TGeoBBox("Box2e_PCB_02",Box2e_dX, Box2e_dY, z_excess/2); //

  TGeoCompositeShape *box_bott02= new TGeoCompositeShape
  ("box_bott02", "Box2d_PCB_02:PCB0tr_Box2d+Box2e_PCB_02:PCB0tr_Box2e+Box2d_PCB_02:PCB0tr_Box2f");
// -----------------------------------------------------------

Double_t Box2g_dX=1.75+ t_delta, Box2g_dY = 6.5+ t_delta;  Double_t Box2g_X=23.75, Box2g_Y = -6.5;      // +Box2g
TGeoTranslation *tr_Box2gl= new TGeoTranslation ("PCB0tr_Box2gl",Box2g_X,Box2g_Y,0.0); tr_Box2gl-> RegisterYourself();
TGeoTranslation *tr_Box2gr= new TGeoTranslation ("PCB0tr_Box2gr",-Box2g_X,Box2g_Y,0.0); tr_Box2gr-> RegisterYourself();
TGeoBBox   *Box2g = new TGeoBBox("Box2g_PCB_02",Box2g_dX, Box2g_dY, z_excess/2);
TGeoCompositeShape *box_lat02= new TGeoCompositeShape ("box_lat02", "Box2g_PCB_02:PCB0tr_Box2gl+Box2g_PCB_02:PCB0tr_Box2gr");
// ----------------------------------------------

Double_t Box2h_dX=3.55, Box2h_dY = .55;  Double_t Box2h_X=-16.474, Box2h_Y = -16.686;      // +Box2h
Double_t Box2i_dX=4.45, Box2i_dY = .55;  Double_t Box2i_X=15.84, Box2i_Y = -17.32;      // +Box2i

TGeoRotation   *rotpcb2h = new TGeoRotation("rotpcb2h", -45., 0., 0.);
TGeoCombiTrans *tr_box2h= new TGeoCombiTrans ("PCB0tr_box2h",Box2h_X,Box2h_Y,0, rotpcb2h); tr_box2h-> RegisterYourself();
TGeoRotation   *rotpcb2i = new TGeoRotation("rotpcb2i", 45., 0., 0.);
TGeoCombiTrans *tr_box2i= new TGeoCombiTrans ("PCB0tr_box2i",Box2i_X,Box2i_Y,0, rotpcb2i); tr_box2i-> RegisterYourself();


TGeoBBox   *Box2h = new TGeoBBox("Box2h_PCB_02",Box2h_dX, Box2h_dY, z_excess/2);
TGeoBBox   *Box2i = new TGeoBBox("Box2i_PCB_02",Box2i_dX, Box2i_dY, z_excess/2);

TGeoCompositeShape *box_hi02= new TGeoCompositeShape ("box_hi02", "Box2h_PCB_02:PCB0tr_box2h+Box2i_PCB_02:PCB0tr_box2i");


// ---------------------------------------------- holes ------------------------------


Double_t Tubh02_XA=-19.5,	Tubh02_YA=-10.0	;		//=.3 A
Double_t Tubh02_XB=-16.,	Tubh02_YB=-10.	;			//=.35 B
Double_t Tubh02_XC=-19.391,	Tubh02_YC=-15.114;	//=.25
Double_t Tubh02_XD=-18.726, 	Tubh02_YD=-15.779;	//=.25
Double_t Tubh02_XE=-16.387,	Tubh02_YE=-12.775;	//=.25
Double_t Tubh02_XF=-18.12,	Tubh02_YF=-16.385;	//=.25
Double_t Tubh02_XG=-17.455,	Tubh02_YG=-17.049;	//=.25
Double_t Tubh02_XH=-15.116,	Tubh02_YH=-14.045;	//=.25
Double_t Tubh02_XI=-16.849,	Tubh02_YI=-17.655;	//=.25
Double_t Tubh02_XJ=-16.185,	Tubh02_YJ=-18.32;	//=.25
Double_t Tubh02_XK=-13.846,	Tubh02_YK=-15.316;	//=.25
Double_t Tubh02_XL=-15.579,	Tubh02_YL=-18.926;	//=.25
Double_t Tubh02_XM=-14.914,	Tubh02_YM=-19.591;	//=.25
Double_t Tubh02_XN=-12.575,	Tubh02_YN=-16.587;	//=.25
Double_t Tubh02_XO=-11.,	Tubh02_YO=-22.	;			//=.35 O
Double_t Tubh02_XP=11.,		Tubh02_YP=-22.	;			//=.35 P
Double_t Tubh02_XQ=11.304,	Tubh02_YQ=-17.857;	//=.25
Double_t Tubh02_XR=13.643,	Tubh02_YR=-20.861;	//=.25
Double_t Tubh02_XS=14.308,	Tubh02_YS=-20.197;	//=.25
Double_t Tubh02_XT=12.575,	Tubh02_YT=-16.587;	//=.25
Double_t Tubh02_XU=14.914,	Tubh02_YU=-19.591;	//=.25
Double_t Tubh02_XV=15.579,	Tubh02_YV=-18.926;	//=.25
Double_t Tubh02_XW=13.846,	Tubh02_YW=-15.316;	//=.25
Double_t Tubh02_XX=16.185,	Tubh02_YX=-18.32;	//=.25
Double_t Tubh02_XY=16.849,	Tubh02_YY=-17.655;	//=.25
Double_t Tubh02_XZ=15.116,	Tubh02_YZ=-14.045;	//=.25
Double_t Tubh02_XAA=17.455,	Tubh02_YAA=-17.049;	//=.25
Double_t Tubh02_XBB=18.12,	Tubh02_YBB=-16.385;	//=.25
Double_t Tubh02_XCC=16.387,	Tubh02_YCC=-12.775;	//=.25
Double_t Tubh02_XDD=18.726,	Tubh02_YDD=-15.779;	//=.25
Double_t Tubh02_XEE=19.391,	Tubh02_YEE=-15.114;	//=.25
Double_t Tubh02_XFF=16.,	Tubh02_YFF=-10.	;			//=.35 FF

// ------------------------------------
Double_t hole102_R=.3/2.; Double_t hole202_R=.35/2.; Double_t hole302_R=.25/2.;

// -----------------  volume -------------
TGeoTube   *Tubeh02A = new TGeoTube("Tubeh0A_PCB_02",0, hole102_R,z_excess/2);
TGeoTube   *Tubeh02B = new TGeoTube("Tubeh0B_PCB_02",0, hole202_R,z_excess/2);
TGeoTube   *Tubeh02C = new TGeoTube("TubeH0C_PCB_02",0, hole302_R,z_excess/2);


// --------------------------------------

TGeoTranslation *tr_Tubh02A= new TGeoTranslation ("PCB0tr_Tubh02A",Tubh02_XA,Tubh02_YA,0.0);    tr_Tubh02A-> RegisterYourself();
TGeoTranslation *tr_Tubh02B= new TGeoTranslation ("PCB0tr_Tubh02B",Tubh02_XB,Tubh02_YB,0.0);    tr_Tubh02B-> RegisterYourself();
TGeoTranslation *tr_Tubh02C= new TGeoTranslation ("PCB0tr_Tubh02C",Tubh02_XC,Tubh02_YC,0.0);    tr_Tubh02C-> RegisterYourself();
TGeoTranslation *tr_Tubh02D= new TGeoTranslation ("PCB0tr_Tubh02D",Tubh02_XD,Tubh02_YD,0.0);    tr_Tubh02D-> RegisterYourself();
TGeoTranslation *tr_Tubh02E= new TGeoTranslation ("PCB0tr_Tubh02E",Tubh02_XE,Tubh02_YE,0.0);    tr_Tubh02E-> RegisterYourself();
TGeoTranslation *tr_Tubh02F= new TGeoTranslation ("PCB0tr_Tubh02F",Tubh02_XF,Tubh02_YF,0.0);    tr_Tubh02F-> RegisterYourself();
TGeoTranslation *tr_Tubh02G= new TGeoTranslation ("PCB0tr_Tubh02G",Tubh02_XG,Tubh02_YG,0.0);    tr_Tubh02G-> RegisterYourself();
TGeoTranslation *tr_Tubh02H= new TGeoTranslation ("PCB0tr_Tubh02H",Tubh02_XH,Tubh02_YH,0.0);    tr_Tubh02H-> RegisterYourself();
TGeoTranslation *tr_Tubh02I= new TGeoTranslation ("PCB0tr_Tubh02I",Tubh02_XI,Tubh02_YI,0.0);    tr_Tubh02I-> RegisterYourself();
TGeoTranslation *tr_Tubh02J= new TGeoTranslation ("PCB0tr_Tubh02J",Tubh02_XJ,Tubh02_YJ,0.0);    tr_Tubh02J-> RegisterYourself();
TGeoTranslation *tr_Tubh02K= new TGeoTranslation ("PCB0tr_Tubh02K",Tubh02_XK,Tubh02_YK,0.0);    tr_Tubh02K-> RegisterYourself();
TGeoTranslation *tr_Tubh02L= new TGeoTranslation ("PCB0tr_Tubh02L",Tubh02_XL,Tubh02_YL,0.0);    tr_Tubh02L-> RegisterYourself();
TGeoTranslation *tr_Tubh02M= new TGeoTranslation ("PCB0tr_Tubh02M",Tubh02_XM,Tubh02_YM,0.0);    tr_Tubh02M-> RegisterYourself();
TGeoTranslation *tr_Tubh02N= new TGeoTranslation ("PCB0tr_Tubh02N",Tubh02_XN,Tubh02_YN,0.0);    tr_Tubh02N-> RegisterYourself();
TGeoTranslation *tr_Tubh02O= new TGeoTranslation ("PCB0tr_Tubh02O",Tubh02_XO,Tubh02_YO,0.0);    tr_Tubh02O-> RegisterYourself();
TGeoTranslation *tr_Tubh02P= new TGeoTranslation ("PCB0tr_Tubh02P",Tubh02_XP,Tubh02_YP,0.0);    tr_Tubh02P-> RegisterYourself();
TGeoTranslation *tr_Tubh02Q= new TGeoTranslation ("PCB0tr_Tubh02Q",Tubh02_XQ,Tubh02_YQ,0.0);    tr_Tubh02Q-> RegisterYourself();
TGeoTranslation *tr_Tubh02R= new TGeoTranslation ("PCB0tr_Tubh02R",Tubh02_XR,Tubh02_YR,0.0);    tr_Tubh02R-> RegisterYourself();
TGeoTranslation *tr_Tubh02S= new TGeoTranslation ("PCB0tr_Tubh02S",Tubh02_XS,Tubh02_YS,0.0);    tr_Tubh02S-> RegisterYourself();
TGeoTranslation *tr_Tubh02T= new TGeoTranslation ("PCB0tr_Tubh02T",Tubh02_XT,Tubh02_YT,0.0);    tr_Tubh02T-> RegisterYourself();
TGeoTranslation *tr_Tubh02U= new TGeoTranslation ("PCB0tr_Tubh02U",Tubh02_XU,Tubh02_YU,0.0);    tr_Tubh02U-> RegisterYourself();
TGeoTranslation *tr_Tubh02V= new TGeoTranslation ("PCB0tr_Tubh02V",Tubh02_XV,Tubh02_YV,0.0);    tr_Tubh02V-> RegisterYourself();
TGeoTranslation *tr_Tubh02W= new TGeoTranslation ("PCB0tr_Tubh02W",Tubh02_XW,Tubh02_YW,0.0);    tr_Tubh02W-> RegisterYourself();
TGeoTranslation *tr_Tubh02X= new TGeoTranslation ("PCB0tr_Tubh02X",Tubh02_XX,Tubh02_YX,0.0);    tr_Tubh02X-> RegisterYourself();
TGeoTranslation *tr_Tubh02Y= new TGeoTranslation ("PCB0tr_Tubh02Y",Tubh02_XY,Tubh02_YY,0.0);    tr_Tubh02Y-> RegisterYourself();
TGeoTranslation *tr_Tubh02Z= new TGeoTranslation ("PCB0tr_Tubh02Z",Tubh02_XZ,Tubh02_YZ,0.0);    tr_Tubh02Z-> RegisterYourself();
TGeoTranslation *tr_Tubh2AA= new TGeoTranslation ("PCB0tr_Tubh2AA",Tubh02_XAA,Tubh02_YAA,0.0);    tr_Tubh2AA-> RegisterYourself();
TGeoTranslation *tr_Tubh2BB= new TGeoTranslation ("PCB0tr_Tubh2BB",Tubh02_XBB,Tubh02_YBB,0.0);    tr_Tubh2BB-> RegisterYourself();
TGeoTranslation *tr_Tubh2CC= new TGeoTranslation ("PCB0tr_Tubh2CC",Tubh02_XCC,Tubh02_YCC,0.0);    tr_Tubh2CC-> RegisterYourself();
TGeoTranslation *tr_Tubh2DD= new TGeoTranslation ("PCB0tr_Tubh2DD",Tubh02_XDD,Tubh02_YDD,0.0);    tr_Tubh2DD-> RegisterYourself();
TGeoTranslation *tr_Tubh2EE= new TGeoTranslation ("PCB0tr_Tubh2EE",Tubh02_XEE,Tubh02_YEE,0.0);    tr_Tubh2EE-> RegisterYourself();
TGeoTranslation *tr_Tubh2FF= new TGeoTranslation ("PCB0tr_Tubh2FF",Tubh02_XFF,Tubh02_YFF,0.0);    tr_Tubh2FF-> RegisterYourself();

// --------------------- double holes -------------------------

TGeoTranslation *tr_Tub0dhl02= new TGeoTranslation ("PCB0tr_Tub0dhl02",-.2,0.0,0.0); tr_Tub0dhl02-> RegisterYourself();
TGeoTranslation *tr_Tub0dhr02= new TGeoTranslation ("PCB0tr_Tub0dhr02",.2,0.0,0.0);  tr_Tub0dhr02-> RegisterYourself();
TGeoTranslation *tr_Tub0dhp02= new TGeoTranslation ("PCB0tr_Tub0dhp02",19.5,-10.0,0.0); tr_Tub0dhp02-> RegisterYourself();

TGeoBBox   * Boxdh02 = new TGeoBBox("Boxdh_PCB_02", .2,  .15, z_excess/2);

TGeoCompositeShape *Doub_holeh02 = new TGeoCompositeShape  ("Doub_holeh02", "Boxdh_PCB_02+Tubeh0A_PCB_02:PCB0tr_Tub0dhl02 +Tubeh0A_PCB_02:PCB0tr_Tub0dhr02");
// --------------------------------------------------------------------------------------------------

TGeoCompositeShape *holes_02 = new TGeoCompositeShape  ("holes_02", "Tubeh0A_PCB_02:PCB0tr_Tubh02A + Tubeh0B_PCB_02:PCB0tr_Tubh02B +Tubeh0B_PCB_02:PCB0tr_Tubh02O+Tubeh0B_PCB_02:PCB0tr_Tubh02P+Tubeh0B_PCB_02:PCB0tr_Tubh2FF+TubeH0C_PCB_02:PCB0tr_Tubh02C+TubeH0C_PCB_02:PCB0tr_Tubh02D+TubeH0C_PCB_02:PCB0tr_Tubh02E+TubeH0C_PCB_02:PCB0tr_Tubh02F+TubeH0C_PCB_02:PCB0tr_Tubh02G+TubeH0C_PCB_02:PCB0tr_Tubh02H+TubeH0C_PCB_02:PCB0tr_Tubh02I+TubeH0C_PCB_02:PCB0tr_Tubh02J+TubeH0C_PCB_02:PCB0tr_Tubh02K+TubeH0C_PCB_02:PCB0tr_Tubh02L+TubeH0C_PCB_02:PCB0tr_Tubh02M+TubeH0C_PCB_02:PCB0tr_Tubh02N+TubeH0C_PCB_02:PCB0tr_Tubh02Q+TubeH0C_PCB_02:PCB0tr_Tubh02R+TubeH0C_PCB_02:PCB0tr_Tubh02S+TubeH0C_PCB_02:PCB0tr_Tubh02T+TubeH0C_PCB_02:PCB0tr_Tubh02U+TubeH0C_PCB_02:PCB0tr_Tubh02V+TubeH0C_PCB_02:PCB0tr_Tubh02W+TubeH0C_PCB_02:PCB0tr_Tubh02X+TubeH0C_PCB_02:PCB0tr_Tubh02Y+TubeH0C_PCB_02:PCB0tr_Tubh02Z+TubeH0C_PCB_02:PCB0tr_Tubh2AA+TubeH0C_PCB_02:PCB0tr_Tubh2BB+TubeH0C_PCB_02:PCB0tr_Tubh2CC+TubeH0C_PCB_02:PCB0tr_Tubh2DD+TubeH0C_PCB_02:PCB0tr_Tubh2EE+TubeH0C_PCB_02:PCB0tr_Tubh2FF+Doub_holeh02:PCB0tr_Tub0dhp02");


// ------------------------------------------------
  TGeoTubeSeg *varnishShape = new TGeoTubeSeg("tube_varnish_02",rMin, rMax, fr4Thickness/2., phiMin, phiMax);
  TGeoTubeSeg *copperShape = new TGeoTubeSeg("tube_copper_02",rMin, rMax, copperThickness/2., phiMin, phiMax);



  TGeoCompositeShape *comp_varnish = new TGeoCompositeShape ("varnish_02", "tube_varnish_02-box_out02-box_bott02-box_lat02-box_hi02-holes_02");
  TGeoCompositeShape *comp_copper = new TGeoCompositeShape ("copper_02", "tube_copper_02-box_out02-box_bott02-box_lat02-box_hi02-holes_02");


  // Get Mediums
  TGeoMedium *medFR4  = gGeoManager->GetMedium("MFT_FR4$");
  TGeoMedium *medCu  = gGeoManager->GetMedium("MFT_Cu$");

  // Create Volumes
  TGeoVolume *varnishVol = new TGeoVolume(Form("Varnish_%d_%d", half, disk), comp_varnish, medFR4);
  varnishVol->SetVisibility(kTRUE);
  varnishVol->SetLineColor(kGreen);
  varnishVol->SetLineWidth(1);
  varnishVol->SetFillColor(varnishVol->GetLineColor());
  varnishVol->SetFillStyle(4000); // 0% transparent

  TGeoVolume *copperVol = new TGeoVolume(Form("Copper_%d_%d", half, disk), comp_copper, medCu);
  copperVol->SetVisibility(kTRUE);
  copperVol->SetLineColor(kOrange);
  copperVol->SetLineWidth(1);
  copperVol->SetFillColor(copperVol->GetLineColor());
  copperVol->SetFillStyle(4000); // 0% transparent

  // Position Volumes in the mother PCB Volume
  pcbVolume->AddNode(varnishVol, 1,new TGeoTranslation(0.,0.,fr4Thickness/2.));
  pcbVolume->AddNode(copperVol, 1,new TGeoTranslation(0.,0.,-copperThickness/2.));
  return pcbVolume;

}

//************************************* final PCB_02 ******************

//=============================================================================================

TGeoVolumeAssembly* AliMFTSupport::PCB_04(Int_t half, Int_t disk){

  TGeoVolumeAssembly * pcbVolume = new TGeoVolumeAssembly(Form("PCB_%d_%d", half, disk));


 // Create Shapes
  Double_t phiMin =180., phiMax=360.;
  Double_t rMin =0., rMax=25.5; // units are cm
  Double_t copperThickness = 0.05; //units are cm
  Double_t fr4Thickness = fPCBThickness - copperThickness;
  Double_t t_delta = 0.01;
  Double_t z_excess=2.0*fPCBThickness;

// ========================== inner box out PCB_03 ===========================================

// ---------------------- Volume data -------------------------------------

Double_t Box31_dY=6.1;                                              // Box31
Double_t Box32_dX=16.5, Box32_dY = 7.0;                              // +Box32
Double_t Box33_dX=1.225, Box33_dY = .3; Double_t Box33_X=15.275, Box33_Y =-7.3;    // Box33
Double_t Box34_dX=14.325, Box34_dY = 8.91; Double_t Box34_X=-0.5/2, Box34_Y = 0.0;      // Box34
Double_t Box35_dX=.65, Box35_dY = .02; Double_t Box35_X=13.3, Box35_Y = -8.93;      // Box35
Double_t Box36_dX=12.95, Box36_dY = 9.4;  Double_t Box36_X=-0.175, Box36_Y = 0;      // +Box36
Double_t Box37_dX=12.6, Box37_dY = 11.92;  Double_t Box37_X=-0.25, Box37_Y = 0;      // +Box37
Double_t Box38_dX=11.225, Box38_dY = 12.4;  Double_t Box38_X=-.175, Box38_Y = 0;      // +Box38
Double_t Box39_dX=10.9, Box39_dY = 14.93;  Double_t Box39_X=-0.25, Box39_Y = 0;      // +Box39
Double_t Box3a_dX=7.825, Box3a_dY = 15.35;  Double_t Box3a_X=-0.175, Box3a_Y = 0;      // +Box3a
Double_t Box3b_dX=7.5, Box3b_dY = 17.94;  Double_t Box3b_X=-0.25, Box3b_Y = 0;      // +Box3b
Double_t Box3c_dX=2.4, Box3c_dY = 18.53;  Double_t Box3c_X=-0.175, Box3c_Y = 0;      // +Box3c

// ------------------------ Translations ---------------------------------

TGeoTranslation *tr_Box33= new TGeoTranslation ("PCB0tr_Box33",Box33_X,Box33_Y,0.0); tr_Box33-> RegisterYourself();
TGeoTranslation *tr_Box34= new TGeoTranslation ("PCB0tr_Box34",Box34_X,Box34_Y,0.0); tr_Box34-> RegisterYourself();
TGeoTranslation *tr_Box35= new TGeoTranslation ("PCB0tr_Box35",Box35_X,Box35_Y,0.0); tr_Box35-> RegisterYourself();
TGeoTranslation *tr_Box36= new TGeoTranslation ("PCB0tr_Box36",Box36_X,Box36_Y,0.0); tr_Box36-> RegisterYourself();
TGeoTranslation *tr_Box37= new TGeoTranslation ("PCB0tr_Box37",Box37_X,Box37_Y,0.0); tr_Box37-> RegisterYourself();
TGeoTranslation *tr_Box38= new TGeoTranslation ("PCB0tr_Box38",Box38_X,Box38_Y,0.0); tr_Box38-> RegisterYourself();
TGeoTranslation *tr_Box39= new TGeoTranslation ("PCB0tr_Box39",Box39_X,Box39_Y,0.0); tr_Box39-> RegisterYourself();
TGeoTranslation *tr_Box3a= new TGeoTranslation ("PCB0tr_Box3a",Box3a_X,Box3a_Y,0.0); tr_Box3a-> RegisterYourself();
TGeoTranslation *tr_Box3b= new TGeoTranslation ("PCB0tr_Box3b",Box3b_X,Box3b_Y,0.0); tr_Box3b-> RegisterYourself();
TGeoTranslation *tr_Box3c= new TGeoTranslation ("PCB0tr_Box3c",Box3c_X,Box3c_Y,0.0); tr_Box3c-> RegisterYourself();



// -------------------------- Volume Box ---------------------------------

  TGeoBBox   *Box31 = new TGeoBBox("Box31_PCB_03",rMax+t_delta, Box31_dY, z_excess/2);
  TGeoBBox   *Box32 = new TGeoBBox("Box32_PCB_03",Box32_dX, Box32_dY, z_excess/2);
  TGeoBBox   *Box33 = new TGeoBBox("Box33_PCB_03",Box33_dX, Box33_dY, z_excess/2); //
  TGeoBBox   *Box34 = new TGeoBBox("Box34_PCB_03",Box34_dX, Box34_dY, z_excess/2); //
  TGeoBBox   *Box35 = new TGeoBBox("Box35_PCB_03",Box35_dX, Box35_dY, z_excess/2);
  TGeoBBox   *Box36 = new TGeoBBox("Box36_PCB_03",Box36_dX, Box36_dY, z_excess/2);
  TGeoBBox   *Box37 = new TGeoBBox("Box37_PCB_03",Box37_dX, Box37_dY, z_excess/2);
  TGeoBBox   *Box38 = new TGeoBBox("Box38_PCB_03",Box38_dX, Box38_dY, z_excess/2); //
  TGeoBBox   *Box39 = new TGeoBBox("Box39_PCB_03",Box39_dX, Box39_dY, z_excess/2); //
  TGeoBBox   *Box3a = new TGeoBBox("Box3a_PCB_03",Box3a_dX, Box3a_dY, z_excess/2);
  TGeoBBox   *Box3b = new TGeoBBox("Box3b_PCB_03",Box3b_dX, Box3b_dY, z_excess/2); //
  TGeoBBox   *Box3c = new TGeoBBox("Box3c_PCB_03",Box3c_dX, Box3c_dY, z_excess/2); //


  TGeoCompositeShape *box_out03= new TGeoCompositeShape
  ("box_out03", "Box31_PCB_03+Box32_PCB_03+Box33_PCB_03:PCB0tr_Box33+Box34_PCB_03:PCB0tr_Box34+Box35_PCB_03:PCB0tr_Box35+Box36_PCB_03:PCB0tr_Box36+Box37_PCB_03:PCB0tr_Box37+Box38_PCB_03:PCB0tr_Box38+Box39_PCB_03:PCB0tr_Box39+Box3a_PCB_03:PCB0tr_Box3a+Box3b_PCB_03:PCB0tr_Box3b+Box3c_PCB_03:PCB0tr_Box3c");


// ======================== lateral box out PCB_03 ============================================

// ---------------------- Volume data -------------------------------------

Double_t Box3d_dX=3.5,    Box3d_dY = 13.5;  Double_t Box3d_X=25.5, Box3d1_X = -25.5,      Box3d_Y = -19.0/2;  // Box3e

TGeoTranslation *tr_Box3d= new TGeoTranslation ("PCB0tr_Box3d",Box3d_X,Box3d_Y,0.0); tr_Box3d-> RegisterYourself();
TGeoTranslation *tr_Box3d1= new TGeoTranslation ("PCB0tr_Box3d1",Box3d1_X,Box3d_Y,0.0); tr_Box3d1-> RegisterYourself();

TGeoBBox   *Box3d = new TGeoBBox("Box3d_PCB_03",Box3d_dX, Box3d_dX, z_excess/2);

TGeoCompositeShape *box_outl_03 = new TGeoCompositeShape  ("box_outl_03", "Box3d_PCB_03:PCB0tr_Box3d+Box3d_PCB_03:PCB0tr_Box3d1");


// ======================== botton box out PCB_03 ============================================

// ---------------------- Volume data -------------------------------------

Double_t Box3e_dX=7.0,    Box3e_dY = .7;  Double_t Box3e_X=0.0,      Box3e_Y = -24.8;  // Box3e
Double_t Box3f_dX=3.0, Box3f_dY = .7;   Double_t Box3f_X=10.0,    Box3f_X1=-10.0,   Box3f_Y = -24.;  // Box3f.


// ------------------------ Translations ---------------------------------

TGeoTranslation *tr_Box3e = new TGeoTranslation ("PCB0tr_Box3e", Box3e_X,Box3e_Y,0.0);   tr_Box3e-> RegisterYourself();
TGeoTranslation *tr_Box3f = new TGeoTranslation ("PCB0tr_Box3f", Box3f_X,Box3f_Y,0.0);   tr_Box3f-> RegisterYourself();
TGeoTranslation *tr_Box3f1= new TGeoTranslation ("PCB0tr_Box3f1",Box3f_X1,Box3f_Y,0.0); tr_Box3f1-> RegisterYourself();

// -------------------------- Volume botton ---------------------------------

  TGeoBBox   *Box3e = new TGeoBBox("Box3e_PCB_03e",Box3e_dX, Box3e_dY, z_excess/2);
  TGeoBBox   *Box3f = new TGeoBBox("Box3f_PCB_03f",Box3f_dX, Box3f_dY, z_excess/2);

   TGeoCompositeShape *box_outb_03 = new TGeoCompositeShape  ("box_outb_03", "Box3e_PCB_03e:PCB0tr_Box3e+Box3f_PCB_03f:PCB0tr_Box3f+Box3f_PCB_03f:PCB0tr_Box3f1");

// --------------------------------------------------------

// ================= holes ===============

Double_t Tubh0A_R0=0.0,      Tubh0A_R = .3/2;    Double_t Tubh0A_X=-19.5,     Tubh0A_Y=-10.0;
Double_t Tubh0B_R0=0.0,      Tubh0B_R = .35/2;   Double_t Tubh0B_X=-16.0,     Tubh0B_Y=-10.0;
Double_t Tubh0O_X=-11.0,     Tubh0O_Y=-22.0;
Double_t Tubh0P_X= 11.0,     Tubh0P_Y=-22.0;
Double_t TubhFF_X= 16.0,     TubhFF_Y=-10.0;
Double_t Tubh0C_R0=0.0,      Tubh0C_R = .25/2;    Double_t Tubh0C_X=-16.5,     Tubh0C_Y=-12.5;
Double_t Tubh0D_X=-19.504,     Tubh0D_Y=-14.839;
Double_t Tubh0E_X=-18.839,     Tubh0E_Y=-15.504;
Double_t Tubh0F_X=-15.229,     Tubh0F_Y=-13.771;
Double_t Tubh0G_X=-18.233,     Tubh0G_Y=-16.11;
Double_t Tubh0H_X=-17.568,     Tubh0H_Y=-16.774;
Double_t Tubh0I_X=-13.959,     Tubh0I_Y=-15.041;
Double_t Tubh0J_X=-16.962,     Tubh0J_Y=-17.38;
Double_t Tubh0K_X=-16.298,     Tubh0K_Y=-18.045;
Double_t Tubh0L_X=-12.688,     Tubh0L_Y=-16.312;
Double_t Tubh0M_X=-15.692,     Tubh0M_Y=-18.651;
Double_t Tubh0N_X=-15.027,     Tubh0N_Y=-19.316;
Double_t Tubh0Q_X= 11.417,     Tubh0Q_Y=-17.583;
Double_t Tubh0R_X= 13.756,     Tubh0R_Y=-20.586;
Double_t Tubh0S_X= 14.421,     Tubh0S_Y=-19.922;
Double_t Tubh0T_X= 12.688,     Tubh0T_Y=-16.312;
Double_t Tubh0U_X= 15.027,     Tubh0U_Y=-19.316;
Double_t Tubh0V_X= 15.692,     Tubh0V_Y=-18.651;
Double_t Tubh0W_X= 13.959,     Tubh0W_Y=-15.041;
Double_t Tubh0X_X= 16.298,     Tubh0X_Y=-18.045;
Double_t Tubh0Y_X= 16.962,     Tubh0Y_Y=-17.38;
Double_t Tubh0Z_X= 15.229,     Tubh0Z_Y=-13.771;
Double_t TubhAA_X= 17.568,     TubhAA_Y=-16.774;
Double_t TubhBB_X= 18.233,     TubhBB_Y=-16.11;
Double_t TubhCC_X= 16.5,       TubhCC_Y=-12.5;
Double_t TubhDD_X= 18.839,     TubhDD_Y=-15.504;
Double_t TubhEE_X= 19.504,     TubhEE_Y=-14.839;

// ------------------ translations ---------------

TGeoTranslation *tr_Tubh0A= new TGeoTranslation ("PCB0tr_Tubh0A",Tubh0A_X,Tubh0A_Y,0.0);    tr_Tubh0A-> RegisterYourself();
TGeoTranslation *tr_Tubh0B= new TGeoTranslation ("PCB0tr_Tubh0B",Tubh0B_X,Tubh0B_Y,0.0);    tr_Tubh0B-> RegisterYourself();
TGeoTranslation *tr_Tubh0C= new TGeoTranslation ("PCB0tr_Tubh0C",Tubh0C_X,Tubh0C_Y,0.0);    tr_Tubh0C-> RegisterYourself();
TGeoTranslation *tr_Tubh0D= new TGeoTranslation ("PCB0tr_Tubh0D",Tubh0D_X,Tubh0D_Y,0.0);    tr_Tubh0D-> RegisterYourself();
TGeoTranslation *tr_Tubh0E= new TGeoTranslation ("PCB0tr_Tubh0E",Tubh0E_X,Tubh0E_Y,0.0);    tr_Tubh0E-> RegisterYourself();
TGeoTranslation *tr_Tubh0F= new TGeoTranslation ("PCB0tr_Tubh0F",Tubh0F_X,Tubh0F_Y,0.0);    tr_Tubh0F-> RegisterYourself();
TGeoTranslation *tr_Tubh0G= new TGeoTranslation ("PCB0tr_Tubh0G",Tubh0G_X,Tubh0G_Y,0.0);    tr_Tubh0G-> RegisterYourself();
TGeoTranslation *tr_Tubh0H= new TGeoTranslation ("PCB0tr_Tubh0H",Tubh0H_X,Tubh0H_Y,0.0);    tr_Tubh0H-> RegisterYourself();
TGeoTranslation *tr_Tubh0I= new TGeoTranslation ("PCB0tr_Tubh0I",Tubh0I_X,Tubh0I_Y,0.0);    tr_Tubh0I-> RegisterYourself();
TGeoTranslation *tr_Tubh0J= new TGeoTranslation ("PCB0tr_Tubh0J",Tubh0J_X,Tubh0J_Y,0.0);    tr_Tubh0J-> RegisterYourself();
TGeoTranslation *tr_Tubh0K= new TGeoTranslation ("PCB0tr_Tubh0K",Tubh0K_X,Tubh0K_Y,0.0);    tr_Tubh0K-> RegisterYourself();
TGeoTranslation *tr_Tubh0L= new TGeoTranslation ("PCB0tr_Tubh0L",Tubh0L_X,Tubh0L_Y,0.0);    tr_Tubh0L-> RegisterYourself();
TGeoTranslation *tr_Tubh0M= new TGeoTranslation ("PCB0tr_Tubh0M",Tubh0M_X,Tubh0M_Y,0.0);    tr_Tubh0M-> RegisterYourself();
TGeoTranslation *tr_Tubh0N= new TGeoTranslation ("PCB0tr_Tubh0N",Tubh0N_X,Tubh0N_Y,0.0);    tr_Tubh0N-> RegisterYourself();
TGeoTranslation *tr_Tubh0O= new TGeoTranslation ("PCB0tr_Tubh0O",Tubh0O_X,Tubh0O_Y,0.0);    tr_Tubh0O-> RegisterYourself();
TGeoTranslation *tr_Tubh0P= new TGeoTranslation ("PCB0tr_Tubh0P",Tubh0P_X,Tubh0P_Y,0.0);    tr_Tubh0P-> RegisterYourself();
TGeoTranslation *tr_Tubh0Q= new TGeoTranslation ("PCB0tr_Tubh0Q",Tubh0Q_X,Tubh0Q_Y,0.0);    tr_Tubh0Q-> RegisterYourself();
TGeoTranslation *tr_Tubh0R= new TGeoTranslation ("PCB0tr_Tubh0R",Tubh0R_X,Tubh0R_Y,0.0);    tr_Tubh0R-> RegisterYourself();
TGeoTranslation *tr_Tubh0S= new TGeoTranslation ("PCB0tr_Tubh0S",Tubh0S_X,Tubh0S_Y,0.0);    tr_Tubh0S-> RegisterYourself();
TGeoTranslation *tr_Tubh0T= new TGeoTranslation ("PCB0tr_Tubh0T",Tubh0T_X,Tubh0T_Y,0.0);    tr_Tubh0T-> RegisterYourself();
TGeoTranslation *tr_Tubh0U= new TGeoTranslation ("PCB0tr_Tubh0U",Tubh0U_X,Tubh0U_Y,0.0);    tr_Tubh0U-> RegisterYourself();
TGeoTranslation *tr_Tubh0V= new TGeoTranslation ("PCB0tr_Tubh0V",Tubh0V_X,Tubh0V_Y,0.0);    tr_Tubh0V-> RegisterYourself();
TGeoTranslation *tr_Tubh0W= new TGeoTranslation ("PCB0tr_Tubh0W",Tubh0W_X,Tubh0W_Y,0.0);    tr_Tubh0W-> RegisterYourself();
TGeoTranslation *tr_Tubh0X= new TGeoTranslation ("PCB0tr_Tubh0X",Tubh0X_X,Tubh0X_Y,0.0);    tr_Tubh0X-> RegisterYourself();
TGeoTranslation *tr_Tubh0Y= new TGeoTranslation ("PCB0tr_Tubh0Y",Tubh0Y_X,Tubh0Y_Y,0.0);    tr_Tubh0Y-> RegisterYourself();
TGeoTranslation *tr_Tubh0Z= new TGeoTranslation ("PCB0tr_Tubh0Z",Tubh0Z_X,Tubh0Z_Y,0.0);    tr_Tubh0Z-> RegisterYourself();
TGeoTranslation *tr_TubhAA= new TGeoTranslation ("PCB0tr_TubhAA",TubhAA_X,TubhAA_Y,0.0);    tr_TubhAA-> RegisterYourself();
TGeoTranslation *tr_TubhBB= new TGeoTranslation ("PCB0tr_TubhBB",TubhBB_X,TubhBB_Y,0.0);    tr_TubhBB-> RegisterYourself();
TGeoTranslation *tr_TubhCC= new TGeoTranslation ("PCB0tr_TubhCC",TubhCC_X,TubhCC_Y,0.0);    tr_TubhCC-> RegisterYourself();
TGeoTranslation *tr_TubhDD= new TGeoTranslation ("PCB0tr_TubhDD",TubhDD_X,TubhDD_Y,0.0);    tr_TubhDD-> RegisterYourself();
TGeoTranslation *tr_TubhEE= new TGeoTranslation ("PCB0tr_TubhEE",TubhEE_X,TubhEE_Y,0.0);    tr_TubhEE-> RegisterYourself();
TGeoTranslation *tr_TubhFF= new TGeoTranslation ("PCB0tr_TubhFF",TubhFF_X,TubhFF_Y,0.0);    tr_TubhFF-> RegisterYourself();


// -----------------  volume -------------
TGeoTube   *Tubeh0A = new TGeoTube("Tubeh0A_PCB_03",Tubh0A_R0, Tubh0A_R,z_excess/2);
TGeoTube   *Tubeh0B = new TGeoTube("Tubeh0B_PCB_03",Tubh0B_R0, Tubh0B_R,z_excess/2);
TGeoTube   *Tubeh0C = new TGeoTube("TubeH0C_PCB_03",Tubh0C_R0, Tubh0C_R,z_excess/2);

// --------------------- double holes -------------------------

TGeoTranslation *tr_Tub0dhl03= new TGeoTranslation ("PCB0tr_Tub0dhl03",-.2,0.0,0.0); tr_Tub0dhl03-> RegisterYourself();
TGeoTranslation *tr_Tub0dhr03= new TGeoTranslation ("PCB0tr_Tub0dhr03",.2,0.0,0.0); tr_Tub0dhr03-> RegisterYourself();
TGeoTranslation *tr_Tub0dhp03= new TGeoTranslation ("PCB0tr_Tub0dhp03",19.3,-10.0,0.0); tr_Tub0dhp03-> RegisterYourself();
// -------------------------- Volume holehs ---------------------------------
TGeoBBox   * Boxdh03 = new TGeoBBox("Boxdh_PCB_03", .2,  .15, z_excess/2);

TGeoCompositeShape *Doub_holeh03 = new TGeoCompositeShape  ("Doub_holeh03", "Boxdh_PCB_03+Tubeh0A_PCB_03:PCB0tr_Tub0dhl03 +Tubeh0A_PCB_03:PCB0tr_Tub0dhr03");



TGeoCompositeShape *holes_03 = new TGeoCompositeShape  ("holes_03", "Tubeh0A_PCB_03:PCB0tr_Tubh0A + Tubeh0B_PCB_03:PCB0tr_Tubh0B +Tubeh0B_PCB_03:PCB0tr_Tubh0O+Tubeh0B_PCB_03:PCB0tr_Tubh0P+Tubeh0B_PCB_03:PCB0tr_TubhFF+TubeH0C_PCB_03:PCB0tr_Tubh0C+TubeH0C_PCB_03:PCB0tr_Tubh0D+TubeH0C_PCB_03:PCB0tr_Tubh0E+TubeH0C_PCB_03:PCB0tr_Tubh0F+TubeH0C_PCB_03:PCB0tr_Tubh0G+TubeH0C_PCB_03:PCB0tr_Tubh0H+TubeH0C_PCB_03:PCB0tr_Tubh0I+TubeH0C_PCB_03:PCB0tr_Tubh0J+TubeH0C_PCB_03:PCB0tr_Tubh0K+TubeH0C_PCB_03:PCB0tr_Tubh0L+TubeH0C_PCB_03:PCB0tr_Tubh0M+TubeH0C_PCB_03:PCB0tr_Tubh0N+TubeH0C_PCB_03:PCB0tr_Tubh0Q+TubeH0C_PCB_03:PCB0tr_Tubh0R+TubeH0C_PCB_03:PCB0tr_Tubh0S+TubeH0C_PCB_03:PCB0tr_Tubh0T+TubeH0C_PCB_03:PCB0tr_Tubh0U+TubeH0C_PCB_03:PCB0tr_Tubh0V+TubeH0C_PCB_03:PCB0tr_Tubh0W+TubeH0C_PCB_03:PCB0tr_Tubh0X+TubeH0C_PCB_03:PCB0tr_Tubh0Y+TubeH0C_PCB_03:PCB0tr_Tubh0Z+TubeH0C_PCB_03:PCB0tr_TubhAA+TubeH0C_PCB_03:PCB0tr_TubhBB+TubeH0C_PCB_03:PCB0tr_TubhCC+TubeH0C_PCB_03:PCB0tr_TubhDD+TubeH0C_PCB_03:PCB0tr_TubhEE+TubeH0C_PCB_03:PCB0tr_TubhFF+Doub_holeh03:PCB0tr_Tub0dhp03");

// ----------------- boxes inner ------------------------------+****************

TGeoRotation   *rotpcb3 = new TGeoRotation("rotpcb3", 45., 0., 0.);
TGeoCombiTrans *tr_boxkk= new TGeoCombiTrans ("PCB0tr_boxkk",15.988,-17.02,0, rotpcb3); tr_boxkk-> RegisterYourself();
  TGeoBBox   *Box3kk = new TGeoBBox("Box3_PCB_03kk",4.45, .55, z_excess/2);
TGeoRotation   *rot5 = new TGeoRotation("rot5", -45., 0., 0.);
TGeoCombiTrans *tr_boxky= new TGeoCombiTrans ("PCB0tr_boxky",-16.596,-16.412,0, rot5); tr_boxky-> RegisterYourself();
  TGeoBBox   *Box3ky = new TGeoBBox("Box3_PCB_03ky",3.55, .55, z_excess/2);

TGeoCompositeShape *boxesi03 = new TGeoCompositeShape  ("boxesi03", "Box3_PCB_03kk:PCB0tr_boxkk+Box3_PCB_03ky:PCB0tr_boxky");

// --------------------------

  TGeoTubeSeg *varnishShape = new TGeoTubeSeg("tube_varnish_03",rMin, rMax, fr4Thickness/2., phiMin, phiMax);
  TGeoTubeSeg *copperShape = new TGeoTubeSeg("tube_copper_03",rMin, rMax, copperThickness/2., phiMin, phiMax);



  TGeoCompositeShape *comp_varnish = new TGeoCompositeShape ("varnish_03", "tube_varnish_03-box_out03-box_outl_03-box_outb_03-holes_03 -boxesi03");
  TGeoCompositeShape *comp_copper = new TGeoCompositeShape  ("copper_03",  "tube_copper_03 -box_out03-box_outl_03-box_outb_03-holes_03 -boxesi03 ");


  // Get Mediums
  TGeoMedium *medFR4  = gGeoManager->GetMedium("MFT_FR4$");
  TGeoMedium *medCu  = gGeoManager->GetMedium("MFT_Cu$");

  // Create Volumes
  TGeoVolume *varnishVol = new TGeoVolume(Form("Varnish_%d_%d", half, disk), comp_varnish, medFR4);
  varnishVol->SetVisibility(kTRUE);
  varnishVol->SetLineColor(kGreen);
  varnishVol->SetLineWidth(1);
  varnishVol->SetFillColor(varnishVol->GetLineColor());
  varnishVol->SetFillStyle(4000); // 0% transparent

  TGeoVolume *copperVol = new TGeoVolume(Form("Copper_%d_%d", half, disk), comp_copper, medCu);
  copperVol->SetVisibility(kTRUE);
  copperVol->SetLineColor(kOrange);
  copperVol->SetLineWidth(1);
  copperVol->SetFillColor(copperVol->GetLineColor());
  copperVol->SetFillStyle(4000); // 0% transparent

  // Position Volumes in the mother PCB Volume
  pcbVolume->AddNode(varnishVol, 1,new TGeoTranslation(0.,0.,fr4Thickness/2.));
  pcbVolume->AddNode(copperVol, 1,new TGeoTranslation(0.,0.,-copperThickness/2.));
  return pcbVolume;

}

//************************************* final PCB_03 ********************************

// ========================== PCB_Psu ===========================================

TGeoVolumeAssembly* AliMFTSupport::PCB_PSU(Int_t half, Int_t disk){

  TGeoVolumeAssembly * pcbVolume = new TGeoVolumeAssembly(Form("PCB_%d_%d", half, disk));
  // Create Shapes
  Double_t phiMin =180., phiMax=360.;
  Double_t rMin =0., rMax=25.5; // units are cm
  Double_t copperThickness = 0.05; //units are cm
  Double_t fr4Thickness = fPCBThickness - copperThickness;
  Double_t t_delta = 0.01;
  Double_t z_excess=2.0*fPCBThickness;

// ========================== inner box out PCB_0 4 ===========================================

// ---------------------- Volume data -------------------------------------

Double_t Box41_dY=5.0;                                              // Box41
Double_t Box42_dX=14.2, Box42_dY = 9.35;                              // +Box42
Double_t Box43_dX=2.5, Box43_dY = 6.0; Double_t Box43_X=25.5, Box43_Y =-5.0;    // Box43
Double_t Box44_dX=14.2, Box44_dY = 2.2; Double_t Box44_X=0., Box44_Y = -25.5;      // Box44
Double_t Box45_dX=15.6/2, Box45_dY = .5; Double_t Box45_X=0., Box45_Y = -20.917;      // Box45
Double_t TSeg41_phi0=203, TSeg41_phi=242, TSeg41_R0 = 22.3, TSeg41_R = 23.3;
Double_t TSeg42_phi0=298, TSeg42_phi=337;
// ------------------------ Translations ---------------------------------

TGeoTranslation *tr_Box43r= new TGeoTranslation ("PCB0tr_Box43r",Box43_X,Box43_Y,0.0); tr_Box43r-> RegisterYourself();
TGeoTranslation *tr_Box43l= new TGeoTranslation ("PCB0tr_Box43l",-Box43_X,Box43_Y,0.0); tr_Box43l-> RegisterYourself();
TGeoTranslation *tr_Box44= new TGeoTranslation ("PCB0tr_Box44",Box44_X,Box44_Y,0.0); tr_Box44-> RegisterYourself();
TGeoTranslation *tr_Box45= new TGeoTranslation ("PCB0tr_Box45",Box45_X,Box45_Y,0.0); tr_Box45-> RegisterYourself();


// -------------------------- Volume Box ---------------------------------

  TGeoBBox   *Box41 = new TGeoBBox("Box41_PCB_04",rMax+t_delta, Box41_dY, z_excess/2);
  TGeoBBox   *Box42 = new TGeoBBox("Box42_PCB_04",Box42_dX, Box42_dY, z_excess/2);
  TGeoBBox   *Box43 = new TGeoBBox("Box43_PCB_04",Box43_dX, Box43_dY, z_excess/2); //
  TGeoBBox   *Box44 = new TGeoBBox("Box44_PCB_04",Box44_dX, Box44_dY, z_excess/2); //
  TGeoBBox   *Box45 = new TGeoBBox("Box45_PCB_04",Box45_dX, Box45_dY, z_excess/2); //
  TGeoTubeSeg *tubes04A = new TGeoTubeSeg("tubes04A",0, 17,  z_excess/2., 213.35, 326.65);
  TGeoTubeSeg *tubes04B = new TGeoTubeSeg("tubes04B",22.3, 23.3,  z_excess/2, 203., 242.);
  TGeoTubeSeg *tubes04C = new TGeoTubeSeg("tubes04C",22.3, 23.3,  z_excess/2, 298., 337.);

  TGeoCompositeShape *box_out04= new TGeoCompositeShape
  ("box_out04", "Box41_PCB_04+Box42_PCB_04+Box43_PCB_04:PCB0tr_Box43l+Box43_PCB_04:PCB0tr_Box43r+Box44_PCB_04:PCB0tr_Box44+Box45_PCB_04:PCB0tr_Box45+tubes04A+tubes04B+tubes04C");


// --------------------------------------------------------

// ================= holes ===============

Double_t Tubh04A_X=-21.749,	Tubh04A_Y=-9.623;
Double_t Tubh04B_X=-21.352,	Tubh04B_Y=-10.475;
Double_t Tubh04C_X=-18.126,	Tubh04C_Y=-8.452;
Double_t Tubh04D_X=-20.827,	Tubh04D_Y=-11.482;
Double_t Tubh04E_X=-20.357,	Tubh04E_Y=-12.296;
Double_t Tubh04F_X=-17.321,	Tubh04F_Y=-10.0;
Double_t Tubh04G_X=-19.747,	Tubh04G_Y=-13.253;
Double_t Tubh04H_X=-19.208,	Tubh04H_Y=-14.024;
Double_t Tubh04I_X=-16.383,	Tubh04I_Y=-11.472;
Double_t Tubh04J_X=-18.517,	Tubh04J_Y=-14.924;
Double_t Tubh04K_X=-17.913,	Tubh04K_Y=-15.644;
Double_t Tubh04L_X=-15.321,	Tubh04L_Y=-12.856;
Double_t Tubh04M_X=-17.146,	Tubh04M_Y=-16.481;
Double_t Tubh04N_X=-16.481,	Tubh04N_Y=-17.146;
Double_t Tubh04O_X=-14.142,	Tubh04O_Y=-14.142;
Double_t Tubh04P_X=-15.644,	Tubh04P_Y=-17.913;
Double_t Tubh04Q_X=-14.924,	Tubh04Q_Y=-18.517;
Double_t Tubh04R_X=-12.856,	Tubh04R_Y=-15.321;
Double_t Tubh04S_X=-14.024,	Tubh04S_Y=-19.208;
Double_t Tubh04T_X=-13.253,	Tubh04T_Y=-19.747;
Double_t Tubh04U_X=-11.472,	Tubh04U_Y=-16.383;
Double_t Tubh04V_X=-12.296,	Tubh04V_Y=-20.357;
Double_t Tubh04W_X=-11.482,	Tubh04W_Y=-20.827;
Double_t Tubh04X_X=-10.0,	Tubh04X_Y=-17.321;
Double_t Tubh04Y_X=-7.646,	Tubh04Y_Y=-21.878;
Double_t Tubh04Z_X=-6.706,	Tubh04Z_Y=-21.878;
Double_t Tubh04AA_X=-7.176,	Tubh04AA_Y=-1.81;
Double_t Tubh04BB_X=-5.852,	Tubh04BB_Y=-21.878;
Double_t Tubh04CC_X=-4.912,	Tubh04CC_Y=-21.878;
Double_t Tubh04DD_X=-5.382,	Tubh04DD_Y=-18.1;
Double_t Tubh04EE_X=-4.058,	Tubh04EE_Y=-21.878;
Double_t Tubh04FF_X=-3.118,	Tubh04FF_Y=-21.878;
Double_t Tubh04GG_X=-3.588,	Tubh04GG_Y=-18.1;
Double_t Tubh04HH_X=-2.264,	Tubh04HH_Y=-21.878;
Double_t Tubh04II_X=-1.324,	Tubh04II_Y=-21.878;
Double_t Tubh04JJ_X=-1.794,	Tubh04JJ_Y=-18.1;
Double_t Tubh04KK_X=-.47,	Tubh04KK_Y=-21.878;
Double_t Tubh04LL_X=.47	,	Tubh04LL_Y=-21.878;
Double_t Tubh04MM_X=0.	,	Tubh04MM_Y=-18.1;
Double_t Tubh04NN_X=1.324,	Tubh04NN_Y=-21.878;
Double_t Tubh04OO_X=2.264,	Tubh04OO_Y=-21.878;
Double_t Tubh04PP_X=1.794,	Tubh04PP_Y=-18.1;
Double_t Tubh04QQ_X=3.118,	Tubh04QQ_Y=-21.878;
Double_t Tubh04RR_X=4.058,	Tubh04RR_Y=-21.878;
Double_t Tubh04SS_X=3.588,	Tubh04SS_Y=-18.1;
Double_t Tubh04TT_X=4.912,	Tubh04TT_Y=-21.878;
Double_t Tubh04UU_X=5.852,	Tubh04UU_Y=-21.878;
Double_t Tubh04VV_X=5.382,	Tubh04VV_Y=-18.1;
Double_t Tubh04WW_X=6.706,	Tubh04WW_Y=-21.878;
Double_t Tubh04XX_X=7.646,	Tubh04XX_Y=-21.878;
Double_t Tubh04YY_X=7.176,	Tubh04YY_Y=-18.1;
Double_t Tubh04ZZ_X=11.482,	Tubh04ZZ_Y=-20.827;
Double_t Tubh04AAA_X=12.296,	Tubh04AAA_Y=-20.357;
Double_t Tubh04BBB_X=10.0,	Tubh04BBB_Y=-17.321;
Double_t Tubh04CCC_X=13.253,	Tubh04CCC_Y=-19.747;
Double_t Tubh04DDD_X=14.024,	Tubh04DDD_Y=-19.208;
Double_t Tubh04EEE_X=11.472,	Tubh04EEE_Y=-16.383;
Double_t Tubh04FFF_X=14.924,	Tubh04FFF_Y=-18.517;
Double_t Tubh04GGG_X=15.644,	Tubh04GGG_Y=-17.913;
Double_t Tubh04HHH_X=12.856,	Tubh04HHH_Y=-15.321;
Double_t Tubh04III_X=16.481,	Tubh04III_Y=-17.146;
Double_t Tubh04JJJ_X=17.146,	Tubh04JJJ_Y=-16.481;
Double_t Tubh04KKK_X=14.142,	Tubh04KKK_Y=-14.142;
Double_t Tubh04LLL_X=17.913,	Tubh04LLL_Y=-15.644;
Double_t Tubh04MMM_X=18.517,	Tubh04MMM_Y=-14.924;
Double_t Tubh04NNN_X=15.321,	Tubh04NNN_Y=-12.856;
Double_t Tubh04OOO_X=19.208,	Tubh04OOO_Y=-14.024;
Double_t Tubh04PPP_X=19.747,	Tubh04PPP_Y=-13.253;
Double_t Tubh04QQQ_X=16.383,	Tubh04QQQ_Y=-11.472;
Double_t Tubh04RRR_X=20.357,	Tubh04RRR_Y=-12.296;
Double_t Tubh04SSS_X=20.827,	Tubh04SSS_Y=-11.482;
Double_t Tubh04TTT_X=17.321,	Tubh04TTT_Y=-10.0;
Double_t Tubh04UUU_X=21.352,	Tubh04UUU_Y=-10.475;
Double_t Tubh04VVV_X=21.749,	Tubh04VVV_Y=-9.623;
Double_t Tubh04WWW_X=18.126,	Tubh04WWW_Y=-8.452;


// ------------------ translations ---------------

TGeoTranslation *tr_Tubh04A  = new TGeoTranslation ("PCB0tr_Tubh04A",Tubh04A_X,Tubh04A_Y,0.0);      tr_Tubh04A-> RegisterYourself();
TGeoTranslation *tr_Tubh04B  = new TGeoTranslation ("PCB0tr_Tubh04B",Tubh04B_X,Tubh04B_Y,0.0);      tr_Tubh04B-> RegisterYourself();
TGeoTranslation *tr_Tubh04C  = new TGeoTranslation ("PCB0tr_Tubh04C",Tubh04C_X,Tubh04C_Y,0.0);      tr_Tubh04C-> RegisterYourself();
TGeoTranslation *tr_Tubh04D  = new TGeoTranslation ("PCB0tr_Tubh04D",Tubh04D_X,Tubh04D_Y,0.0);      tr_Tubh04D-> RegisterYourself();
TGeoTranslation *tr_Tubh04E  = new TGeoTranslation ("PCB0tr_Tubh04E",Tubh04E_X,Tubh04E_Y,0.0);      tr_Tubh04E-> RegisterYourself();
TGeoTranslation *tr_Tubh04F  = new TGeoTranslation ("PCB0tr_Tubh04F",Tubh04F_X,Tubh04F_Y,0.0);      tr_Tubh04F-> RegisterYourself();
TGeoTranslation *tr_Tubh04G  = new TGeoTranslation ("PCB0tr_Tubh04G",Tubh04G_X,Tubh04G_Y,0.0);      tr_Tubh04G-> RegisterYourself();
TGeoTranslation *tr_Tubh04H  = new TGeoTranslation ("PCB0tr_Tubh04H",Tubh04H_X,Tubh04H_Y,0.0);      tr_Tubh04H-> RegisterYourself();
TGeoTranslation *tr_Tubh04I  = new TGeoTranslation ("PCB0tr_Tubh04I",Tubh04I_X,Tubh04I_Y,0.0);      tr_Tubh04I-> RegisterYourself();
TGeoTranslation *tr_Tubh04J  = new TGeoTranslation ("PCB0tr_Tubh04J",Tubh04J_X,Tubh04J_Y,0.0);      tr_Tubh04J-> RegisterYourself();
TGeoTranslation *tr_Tubh04K  = new TGeoTranslation ("PCB0tr_Tubh04K",Tubh04K_X,Tubh04K_Y,0.0);      tr_Tubh04K-> RegisterYourself();
TGeoTranslation *tr_Tubh04L  = new TGeoTranslation ("PCB0tr_Tubh04L",Tubh04L_X,Tubh04L_Y,0.0);      tr_Tubh04L-> RegisterYourself();
TGeoTranslation *tr_Tubh04M  = new TGeoTranslation ("PCB0tr_Tubh04M",Tubh04M_X,Tubh04M_Y,0.0);      tr_Tubh04M-> RegisterYourself();
TGeoTranslation *tr_Tubh04N  = new TGeoTranslation ("PCB0tr_Tubh04N",Tubh04N_X,Tubh04N_Y,0.0);      tr_Tubh04N-> RegisterYourself();
TGeoTranslation *tr_Tubh04O  = new TGeoTranslation ("PCB0tr_Tubh04O",Tubh04O_X,Tubh04O_Y,0.0);      tr_Tubh04O-> RegisterYourself();
TGeoTranslation *tr_Tubh04P  = new TGeoTranslation ("PCB0tr_Tubh04P",Tubh04P_X,Tubh04P_Y,0.0);      tr_Tubh04P-> RegisterYourself();
TGeoTranslation *tr_Tubh04Q  = new TGeoTranslation ("PCB0tr_Tubh04Q",Tubh04Q_X,Tubh04Q_Y,0.0);      tr_Tubh04Q-> RegisterYourself();
TGeoTranslation *tr_Tubh04R  = new TGeoTranslation ("PCB0tr_Tubh04R",Tubh04R_X,Tubh04R_Y,0.0);      tr_Tubh04R-> RegisterYourself();
TGeoTranslation *tr_Tubh04S  = new TGeoTranslation ("PCB0tr_Tubh04S",Tubh04S_X,Tubh04S_Y,0.0);      tr_Tubh04S-> RegisterYourself();
TGeoTranslation *tr_Tubh04T  = new TGeoTranslation ("PCB0tr_Tubh04T",Tubh04T_X,Tubh04T_Y,0.0);      tr_Tubh04T-> RegisterYourself();
TGeoTranslation *tr_Tubh04U  = new TGeoTranslation ("PCB0tr_Tubh04U",Tubh04U_X,Tubh04U_Y,0.0);      tr_Tubh04U-> RegisterYourself();
TGeoTranslation *tr_Tubh04V  = new TGeoTranslation ("PCB0tr_Tubh04V",Tubh04V_X,Tubh04V_Y,0.0);      tr_Tubh04V-> RegisterYourself();
TGeoTranslation *tr_Tubh04W  = new TGeoTranslation ("PCB0tr_Tubh04W",Tubh04W_X,Tubh04W_Y,0.0);      tr_Tubh04W-> RegisterYourself();
TGeoTranslation *tr_Tubh04X  = new TGeoTranslation ("PCB0tr_Tubh04X",Tubh04X_X,Tubh04X_Y,0.0);      tr_Tubh04X-> RegisterYourself();
TGeoTranslation *tr_Tubh04Y  = new TGeoTranslation ("PCB0tr_Tubh04Y",Tubh04Y_X,Tubh04Y_Y,0.0);      tr_Tubh04Y-> RegisterYourself();
TGeoTranslation *tr_Tubh04Z  = new TGeoTranslation ("PCB0tr_Tubh04Z",Tubh04Z_X,Tubh04Z_Y,0.0);      tr_Tubh04Z-> RegisterYourself();
TGeoTranslation *tr_Tubh04AA = new TGeoTranslation ("PCB0tr_Tubh04AA",Tubh04AA_X,Tubh04AA_Y,0.0);     tr_Tubh04AA-> RegisterYourself();
TGeoTranslation *tr_Tubh04BB = new TGeoTranslation ("PCB0tr_Tubh04BB",Tubh04BB_X,Tubh04BB_Y,0.0);     tr_Tubh04BB-> RegisterYourself();
TGeoTranslation *tr_Tubh04CC = new TGeoTranslation ("PCB0tr_Tubh04CC",Tubh04CC_X,Tubh04CC_Y,0.0);     tr_Tubh04CC-> RegisterYourself();
TGeoTranslation *tr_Tubh04DD = new TGeoTranslation ("PCB0tr_Tubh04DD",Tubh04DD_X,Tubh04DD_Y,0.0);     tr_Tubh04DD-> RegisterYourself();
TGeoTranslation *tr_Tubh04EE = new TGeoTranslation ("PCB0tr_Tubh04EE",Tubh04EE_X,Tubh04EE_Y,0.0);     tr_Tubh04EE-> RegisterYourself();
TGeoTranslation *tr_Tubh04FF = new TGeoTranslation ("PCB0tr_Tubh04FF",Tubh04FF_X,Tubh04FF_Y,0.0);     tr_Tubh04FF-> RegisterYourself();
TGeoTranslation *tr_Tubh04GG = new TGeoTranslation ("PCB0tr_Tubh04GG",Tubh04GG_X,Tubh04GG_Y,0.0);     tr_Tubh04GG-> RegisterYourself();
TGeoTranslation *tr_Tubh04HH = new TGeoTranslation ("PCB0tr_Tubh04HH",Tubh04HH_X,Tubh04HH_Y,0.0);     tr_Tubh04HH-> RegisterYourself();
TGeoTranslation *tr_Tubh04II = new TGeoTranslation ("PCB0tr_Tubh04II",Tubh04II_X,Tubh04II_Y,0.0);     tr_Tubh04II-> RegisterYourself();
TGeoTranslation *tr_Tubh04JJ = new TGeoTranslation ("PCB0tr_Tubh04JJ",Tubh04JJ_X,Tubh04JJ_Y,0.0);     tr_Tubh04JJ-> RegisterYourself();
TGeoTranslation *tr_Tubh04KK = new TGeoTranslation ("PCB0tr_Tubh04KK",Tubh04KK_X,Tubh04KK_Y,0.0);     tr_Tubh04KK-> RegisterYourself();
TGeoTranslation *tr_Tubh04LL = new TGeoTranslation ("PCB0tr_Tubh04LL",Tubh04LL_X,Tubh04LL_Y,0.0);     tr_Tubh04LL-> RegisterYourself();
TGeoTranslation *tr_Tubh04MM = new TGeoTranslation ("PCB0tr_Tubh04MM",Tubh04MM_X,Tubh04MM_Y,0.0);     tr_Tubh04MM-> RegisterYourself();
TGeoTranslation *tr_Tubh04NN = new TGeoTranslation ("PCB0tr_Tubh04NN",Tubh04NN_X,Tubh04NN_Y,0.0);     tr_Tubh04NN-> RegisterYourself();
TGeoTranslation *tr_Tubh04OO = new TGeoTranslation ("PCB0tr_Tubh04OO",Tubh04OO_X,Tubh04OO_Y,0.0);     tr_Tubh04OO-> RegisterYourself();
TGeoTranslation *tr_Tubh04PP = new TGeoTranslation ("PCB0tr_Tubh04PP",Tubh04PP_X,Tubh04PP_Y,0.0);     tr_Tubh04PP-> RegisterYourself();
TGeoTranslation *tr_Tubh04QQ = new TGeoTranslation ("PCB0tr_Tubh04QQ",Tubh04QQ_X,Tubh04QQ_Y,0.0);     tr_Tubh04QQ-> RegisterYourself();
TGeoTranslation *tr_Tubh04RR = new TGeoTranslation ("PCB0tr_Tubh04RR",Tubh04RR_X,Tubh04RR_Y,0.0);     tr_Tubh04RR-> RegisterYourself();
TGeoTranslation *tr_Tubh04SS = new TGeoTranslation ("PCB0tr_Tubh04SS",Tubh04SS_X,Tubh04SS_Y,0.0);     tr_Tubh04SS-> RegisterYourself();
TGeoTranslation *tr_Tubh04TT = new TGeoTranslation ("PCB0tr_Tubh04TT",Tubh04TT_X,Tubh04TT_Y,0.0);     tr_Tubh04TT-> RegisterYourself();
TGeoTranslation *tr_Tubh04UU = new TGeoTranslation ("PCB0tr_Tubh04UU",Tubh04UU_X,Tubh04UU_Y,0.0);     tr_Tubh04UU-> RegisterYourself();
TGeoTranslation *tr_Tubh04VV = new TGeoTranslation ("PCB0tr_Tubh04VV",Tubh04VV_X,Tubh04VV_Y,0.0);     tr_Tubh04VV-> RegisterYourself();
TGeoTranslation *tr_Tubh04WW = new TGeoTranslation ("PCB0tr_Tubh04WW",Tubh04WW_X,Tubh04WW_Y,0.0);     tr_Tubh04WW-> RegisterYourself();
TGeoTranslation *tr_Tubh04XX = new TGeoTranslation ("PCB0tr_Tubh04XX",Tubh04XX_X,Tubh04XX_Y,0.0);     tr_Tubh04XX-> RegisterYourself();
TGeoTranslation *tr_Tubh04YY = new TGeoTranslation ("PCB0tr_Tubh04YY",Tubh04YY_X,Tubh04YY_Y,0.0);     tr_Tubh04YY-> RegisterYourself();
TGeoTranslation *tr_Tubh04ZZ = new TGeoTranslation ("PCB0tr_Tubh04ZZ",Tubh04ZZ_X,Tubh04ZZ_Y,0.0);     tr_Tubh04ZZ-> RegisterYourself();
TGeoTranslation *tr_Tubh04AAA= new TGeoTranslation ("PCB0tr_Tubh04AAA",Tubh04AAA_X,Tubh04AAA_Y,0.0);    tr_Tubh04AAA-> RegisterYourself();
TGeoTranslation *tr_Tubh04BBB= new TGeoTranslation ("PCB0tr_Tubh04BBB",Tubh04BBB_X,Tubh04BBB_Y,0.0);    tr_Tubh04BBB-> RegisterYourself();
TGeoTranslation *tr_Tubh04CCC= new TGeoTranslation ("PCB0tr_Tubh04CCC",Tubh04CCC_X,Tubh04CCC_Y,0.0);    tr_Tubh04CCC-> RegisterYourself();
TGeoTranslation *tr_Tubh04DDD= new TGeoTranslation ("PCB0tr_Tubh04DDD",Tubh04DDD_X,Tubh04DDD_Y,0.0);    tr_Tubh04DDD-> RegisterYourself();
TGeoTranslation *tr_Tubh04EEE= new TGeoTranslation ("PCB0tr_Tubh04EEE",Tubh04EEE_X,Tubh04EEE_Y,0.0);    tr_Tubh04EEE-> RegisterYourself();
TGeoTranslation *tr_Tubh04FFF= new TGeoTranslation ("PCB0tr_Tubh04FFF",Tubh04FFF_X,Tubh04FFF_Y,0.0);    tr_Tubh04FFF-> RegisterYourself();
TGeoTranslation *tr_Tubh04GGG= new TGeoTranslation ("PCB0tr_Tubh04GGG",Tubh04GGG_X,Tubh04GGG_Y,0.0);    tr_Tubh04GGG-> RegisterYourself();
TGeoTranslation *tr_Tubh04HHH= new TGeoTranslation ("PCB0tr_Tubh04HHH",Tubh04HHH_X,Tubh04HHH_Y,0.0);    tr_Tubh04HHH-> RegisterYourself();
TGeoTranslation *tr_Tubh04III= new TGeoTranslation ("PCB0tr_Tubh04III",Tubh04III_X,Tubh04III_Y,0.0);    tr_Tubh04III-> RegisterYourself();
TGeoTranslation *tr_Tubh04JJJ= new TGeoTranslation ("PCB0tr_Tubh04JJJ",Tubh04JJJ_X,Tubh04JJJ_Y,0.0);    tr_Tubh04JJJ-> RegisterYourself();
TGeoTranslation *tr_Tubh04KKK= new TGeoTranslation ("PCB0tr_Tubh04KKK",Tubh04KKK_X,Tubh04KKK_Y,0.0);    tr_Tubh04KKK-> RegisterYourself();
TGeoTranslation *tr_Tubh04LLL= new TGeoTranslation ("PCB0tr_Tubh04LLL",Tubh04LLL_X,Tubh04LLL_Y,0.0);    tr_Tubh04LLL-> RegisterYourself();
TGeoTranslation *tr_Tubh04MMM= new TGeoTranslation ("PCB0tr_Tubh04MMM",Tubh04MMM_X,Tubh04MMM_Y,0.0);    tr_Tubh04MMM-> RegisterYourself();
TGeoTranslation *tr_Tubh04NNN= new TGeoTranslation ("PCB0tr_Tubh04NNN",Tubh04NNN_X,Tubh04NNN_Y,0.0);    tr_Tubh04NNN-> RegisterYourself();
TGeoTranslation *tr_Tubh04OOO= new TGeoTranslation ("PCB0tr_Tubh04OOO",Tubh04OOO_X,Tubh04OOO_Y,0.0);    tr_Tubh04OOO-> RegisterYourself();
TGeoTranslation *tr_Tubh04PPP= new TGeoTranslation ("PCB0tr_Tubh04PPP",Tubh04PPP_X,Tubh04PPP_Y,0.0);    tr_Tubh04PPP-> RegisterYourself();
TGeoTranslation *tr_Tubh04QQQ= new TGeoTranslation ("PCB0tr_Tubh04QQQ",Tubh04QQQ_X,Tubh04QQQ_Y,0.0);    tr_Tubh04QQQ-> RegisterYourself();
TGeoTranslation *tr_Tubh04RRR= new TGeoTranslation ("PCB0tr_Tubh04RRR",Tubh04RRR_X,Tubh04RRR_Y,0.0);    tr_Tubh04RRR-> RegisterYourself();
TGeoTranslation *tr_Tubh04SSS= new TGeoTranslation ("PCB0tr_Tubh04SSS",Tubh04SSS_X,Tubh04SSS_Y,0.0);    tr_Tubh04SSS-> RegisterYourself();
TGeoTranslation *tr_Tubh04TTT= new TGeoTranslation ("PCB0tr_Tubh04TTT",Tubh04TTT_X,Tubh04TTT_Y,0.0);    tr_Tubh04TTT-> RegisterYourself();
TGeoTranslation *tr_Tubh04UUU= new TGeoTranslation ("PCB0tr_Tubh04UUU",Tubh04UUU_X,Tubh04UUU_Y,0.0);    tr_Tubh04UUU-> RegisterYourself();
TGeoTranslation *tr_Tubh04VVV= new TGeoTranslation ("PCB0tr_Tubh04VVV",Tubh04VVV_X,Tubh04VVV_Y,0.0);    tr_Tubh04VVV-> RegisterYourself();
TGeoTranslation *tr_Tubh04WWW= new TGeoTranslation ("PCB0tr_Tubh04WWW",Tubh04WWW_X,Tubh04WWW_Y,0.0);    tr_Tubh04WWW-> RegisterYourself();


// -----------------  volume -------------
TGeoTube   *TPCB_04= new TGeoTube("TPCB_04",0., .22/2.0,z_excess/2);

TGeoCompositeShape *holes_04 = new TGeoCompositeShape  ("holes_04", "TPCB_04:PCB0tr_Tubh04A  + TPCB_04:PCB0tr_Tubh04B  + TPCB_04:PCB0tr_Tubh04C+TPCB_04:PCB0tr_Tubh04D  + TPCB_04:PCB0tr_Tubh04E  + TPCB_04:PCB0tr_Tubh04F+TPCB_04:PCB0tr_Tubh04G  + TPCB_04:PCB0tr_Tubh04H  +TPCB_04:PCB0tr_Tubh04I+TPCB_04:PCB0tr_Tubh04J  + TPCB_04:PCB0tr_Tubh04K  + TPCB_04:PCB0tr_Tubh04L+TPCB_04:PCB0tr_Tubh04M  + TPCB_04:PCB0tr_Tubh04N  +TPCB_04:PCB0tr_Tubh04O+TPCB_04:PCB0tr_Tubh04P  + TPCB_04:PCB0tr_Tubh04Q  + TPCB_04:PCB0tr_Tubh04R+TPCB_04:PCB0tr_Tubh04S  + TPCB_04:PCB0tr_Tubh04T  +TPCB_04:PCB0tr_Tubh04U+TPCB_04:PCB0tr_Tubh04V  + TPCB_04:PCB0tr_Tubh04W  + TPCB_04:PCB0tr_Tubh04X+TPCB_04:PCB0tr_Tubh04Y  + TPCB_04:PCB0tr_Tubh04Z  +TPCB_04:PCB0tr_Tubh04AA + TPCB_04:PCB0tr_Tubh04BB + TPCB_04:PCB0tr_Tubh04CC+TPCB_04:PCB0tr_Tubh04DD + TPCB_04:PCB0tr_Tubh04EE +TPCB_04:PCB0tr_Tubh04FF+TPCB_04:PCB0tr_Tubh04GG + TPCB_04:PCB0tr_Tubh04HH + TPCB_04:PCB0tr_Tubh04II+TPCB_04:PCB0tr_Tubh04JJ +TPCB_04:PCB0tr_Tubh04KK + TPCB_04:PCB0tr_Tubh04LL+TPCB_04:PCB0tr_Tubh04MM + TPCB_04:PCB0tr_Tubh04NN + TPCB_04:PCB0tr_Tubh04OO+TPCB_04:PCB0tr_Tubh04PP + TPCB_04:PCB0tr_Tubh04QQ + TPCB_04:PCB0tr_Tubh04RR+TPCB_04:PCB0tr_Tubh04SS + TPCB_04:PCB0tr_Tubh04TT +TPCB_04:PCB0tr_Tubh04UU+TPCB_04:PCB0tr_Tubh04VV + TPCB_04:PCB0tr_Tubh04WW + TPCB_04:PCB0tr_Tubh04XX+TPCB_04:PCB0tr_Tubh04YY +TPCB_04:PCB0tr_Tubh04ZZ +TPCB_04:PCB0tr_Tubh04AAA+ TPCB_04:PCB0tr_Tubh04BBB+ TPCB_04:PCB0tr_Tubh04CCC+TPCB_04:PCB0tr_Tubh04DDD+TPCB_04:PCB0tr_Tubh04EEE+ TPCB_04:PCB0tr_Tubh04FFF+TPCB_04:PCB0tr_Tubh04GGG+ TPCB_04:PCB0tr_Tubh04HHH+ TPCB_04:PCB0tr_Tubh04III+TPCB_04:PCB0tr_Tubh04JJJ+ TPCB_04:PCB0tr_Tubh04KKK+ TPCB_04:PCB0tr_Tubh04LLL+ TPCB_04:PCB0tr_Tubh04MMM+ TPCB_04:PCB0tr_Tubh04NNN +TPCB_04:PCB0tr_Tubh04OOO+TPCB_04:PCB0tr_Tubh04PPP+ TPCB_04:PCB0tr_Tubh04QQQ+ TPCB_04:PCB0tr_Tubh04RRR+TPCB_04:PCB0tr_Tubh04SSS +TPCB_04:PCB0tr_Tubh04TTT+ TPCB_04:PCB0tr_Tubh04UUU+TPCB_04:PCB0tr_Tubh04VVV+ TPCB_04:PCB0tr_Tubh04WWW");


// --------------------------

  TGeoTubeSeg *varnishShape = new TGeoTubeSeg("tube_varnish_04",rMin, rMax, fr4Thickness/2., phiMin, phiMax);
  TGeoTubeSeg *copperShape = new TGeoTubeSeg("tube_copper_04",rMin, rMax, copperThickness/2., phiMin, phiMax);



  TGeoCompositeShape *comp_varnish = new TGeoCompositeShape ("varnish_04", "tube_varnish_04-box_out04-holes_04");
  TGeoCompositeShape *comp_copper = new TGeoCompositeShape  ("copper_04",  "tube_copper_04 -box_out04-holes_04");


  // Get Mediums    -box_out04-box_outl_04-box_outb_04-holes_04 -boxesi04
  TGeoMedium *medFR4  = gGeoManager->GetMedium("MFT_FR4$");
  TGeoMedium *medCu  = gGeoManager->GetMedium("MFT_Cu$");

  // Create Volumes
  TGeoVolume *varnishVol = new TGeoVolume(Form("Varnish_%d_%d", half, disk), comp_varnish, medFR4);
  varnishVol->SetVisibility(kTRUE);
  varnishVol->SetLineColor(kGreen);
  varnishVol->SetLineWidth(1);
  varnishVol->SetFillColor(varnishVol->GetLineColor());
  varnishVol->SetFillStyle(4000); // 0% transparent

  TGeoVolume *copperVol = new TGeoVolume(Form("Copper_%d_%d", half, disk), comp_copper, medCu);
  copperVol->SetVisibility(kTRUE);
  copperVol->SetLineColor(kOrange);
  copperVol->SetLineWidth(1);
  copperVol->SetFillColor(copperVol->GetLineColor());
  copperVol->SetFillStyle(4000); // 0% transparent

  // Position Volumes in the mother PCB Volume
  pcbVolume->AddNode(varnishVol, 1,new TGeoTranslation(0.,0.,fr4Thickness/2.));
  pcbVolume->AddNode(copperVol, 1,new TGeoTranslation(0.,0.,-copperThickness/2.));
  return pcbVolume;

}

//****************************** 04 - end ************************************

//=============================================================================================
TGeoVolume* AliMFTSupport::CreateSupport(Int_t half, Int_t disk){

  AliInfo(Form("Creating PCB for half %d and disk %d ",half, disk));

  // Create Shapes
//  Double_t phiMin =0., phiMax=180.;
//  Double_t rMin =20., rMax=40.; // units are cm

//  TGeoTubeSeg *supportShape = new TGeoTubeSeg(rMin, rMax, fSupportThickness/2., phiMin, phiMax);

  // Get Mediums
  TGeoMedium *medPeek  = gGeoManager->GetMedium("MFT_PEEK$");


// Create Volumes
  TGeoVolume *supportVol = new TGeoVolume();
  switch (disk) {
    case 0: supportVol =  Disc_Support_00();
      break;
    case 1: supportVol =  Disc_Support_01();
      break;
    case 2: supportVol =  Disc_Support_02();
      break;
    case 3: supportVol =  Disc_Support_03();
      break;
    case 4: supportVol =  Disc_Support_04();
      break;
  }

  supportVol->SetMedium(medPeek);
  supportVol->SetName(Form("Support_%d_%d", half, disk));

  //TGeoVolume *supportVol = new TGeoVolume(Form("Support_%d_%d", half, disk), supportShape, medPeek);
  supportVol->SetVisibility(kTRUE);
  supportVol->SetLineColor(kYellow-6);
  supportVol->SetLineWidth(1);
  supportVol->SetFillColor(supportVol->GetLineColor());
  supportVol->SetFillStyle(4000); // 0% transparent

  return supportVol;
}


//=============================================================================================
TGeoVolume* AliMFTSupport::Disc_Support_00 (){


    double rMin = 0,
            rMax = 17.5,
            thickness = .8/2,
            phi0 = 0.,
            phi1 = 180.,
            t_delta = 0.1,
            sup_box_dZ = 0.305/2;



     // ================= Disk_Support_00 - base tube =============


     TGeoTubeSeg *base = new TGeoTubeSeg("D0base",rMin,rMax,thickness,phi0,phi1);
     //TGeoTranslation *tr  = new TGeoTranslation(0., 0., 0.);
     //tr->SetName("D0tr"); tr->RegisterYourself();

     // ======= Disk_Support_00 - Inner cuts =========

      TGeoBBox   *IntCutBox1 = new TGeoBBox("D0IntCutBox1",rMax+t_delta, 2.8/2, thickness+t_delta);
  //    TGeoBBox   *IntCutBox2 = new TGeoBBox("D0IntCutBox2",25/2, 3.4/2,  thickness+t_delta);
      TGeoBBox   *IntCutBox3 = new TGeoBBox("D0IntCutBox3",24.8/2, 13.8/2,  thickness+t_delta);
      TGeoBBox   *IntCutBox4 = new TGeoBBox("D0IntCutBox4",15/2, 19.84/2,  thickness+t_delta);
      TGeoBBox   *IntCutBox5 = new TGeoBBox("D0IntCutBox5",5.1/2, 23.6/2,  thickness+t_delta);


      TGeoBBox   *ExtCutBox1 = new TGeoBBox("D0ExtCutBox1",8.2, 1.15,  thickness+t_delta);
      TGeoTranslation *tr7 = new TGeoTranslation(0., 16.65, 0.);
      tr7->SetName("D0tr7"); tr7->RegisterYourself();

      TGeoBBox   *ExtCutBox2 = new TGeoBBox("D0ExtCutBox2",1.3875, 1.45,  thickness+t_delta);
        TGeoTranslation *tr8 = new TGeoTranslation(-16.1875, 7.9, 0.);
      tr8->SetName("D0tr8"); tr8->RegisterYourself();

            TGeoTranslation *tr6 = new TGeoTranslation(16.1875, 7.9, 0.);
      tr6->SetName("D0tr6"); tr6->RegisterYourself();


      TGeoCompositeShape *IntCutBox = new TGeoCompositeShape
      ("D0IntCutBox", "D0base-(D0IntCutBox1 + D0IntCutBox3+ D0IntCutBox4 + D0IntCutBox5+D0ExtCutBox2:D0tr6  + D0ExtCutBox1:D0tr7+ D0ExtCutBox2:D0tr8 )");

      // ================= Screw holes reliefs  ====================


    //Screw support box 1
    Double_t ssbox1_dX=1.2/2, ssbox1_dY = 1.9/2;
    Double_t ssbox1_X=8.75, ssbox1_Y = 7.86, ssbox1_Z= thickness+sup_box_dZ;
    TGeoBBox   *ssbox1 = new TGeoBBox("D0ssbox1",ssbox1_dX,ssbox1_dY,sup_box_dZ);
    TGeoTranslation *tr_ssbox1= new TGeoTranslation ("D0tr_ssbox1",ssbox1_X,ssbox1_Y,ssbox1_Z);
    tr_ssbox1-> RegisterYourself();

    //Screw support box 2
    Double_t ssbox2_dX=3.05/2, ssbox2_dY = 1.9/2;
    Double_t ssbox2_X=-9.025, ssbox2_Y = 7.86, ssbox2_Z= thickness+sup_box_dZ;
    TGeoBBox   *ssbox2 = new TGeoBBox("D0ssbox2",ssbox2_dX,ssbox2_dY,sup_box_dZ);
    TGeoTranslation *tr_ssbox2= new TGeoTranslation ("D0tr_ssbox2",ssbox2_X,ssbox2_Y,ssbox2_Z);
    tr_ssbox2-> RegisterYourself();

    //Screw support box 3
    Double_t ssbox3_dX=4.6/2, ssbox3_dY = 1.9/2;
    Double_t ssbox3_X=5.35, ssbox3_Y = 10.87, ssbox3_Z= thickness+sup_box_dZ;
    TGeoBBox   *ssbox3 = new TGeoBBox("D0ssbox3",ssbox3_dX,ssbox3_dY,sup_box_dZ);
    TGeoTranslation *tr_ssbox3= new TGeoTranslation ("D0tr_ssbox3",ssbox3_X,ssbox3_Y,ssbox3_Z);
    tr_ssbox3-> RegisterYourself();

    //Screw support box 4
    Double_t ssbox4_dX=4.6/2, ssbox4_dY = 1.88/2;
    Double_t ssbox4_X=-4.85, ssbox4_Y = 10.86, ssbox4_Z= thickness+sup_box_dZ;
    TGeoBBox   *ssbox4 = new TGeoBBox("D0ssbox4",ssbox4_dX,ssbox4_dY,sup_box_dZ);
    TGeoTranslation *tr_ssbox4= new TGeoTranslation ("D0tr_ssbox4",ssbox4_X,ssbox4_Y,ssbox4_Z);
    tr_ssbox4-> RegisterYourself();

    //Screw support box 5
    Double_t ssbox5_dX=4.6/2, ssbox5_dY = 2.12/2;
    Double_t ssbox5_X=.25, ssbox5_Y = 12.86, ssbox5_Z= thickness+sup_box_dZ;
    TGeoBBox   *ssbox5 = new TGeoBBox("D0ssbox5",ssbox5_dX,ssbox5_dY,sup_box_dZ);
    TGeoTranslation *tr_ssbox5= new TGeoTranslation ("D0tr_ssbox5",ssbox5_X,ssbox5_Y,ssbox5_Z);
    tr_ssbox5-> RegisterYourself();

    TGeoCompositeShape *ssboxes = new TGeoCompositeShape ("D0ssboxes",
     "D0ssbox1:D0tr_ssbox1+D0ssbox2:D0tr_ssbox2+D0ssbox3:D0tr_ssbox3+D0ssbox4:D0tr_ssbox4+D0ssbox5:D0tr_ssbox5");

    //Screwholes Area #1
    Double_t AX=8.75, AY=7.41;
    Int_t N_holes = 1;
    TGeoTranslation *tr_holes1= new TGeoTranslation ("D0tr_holes1",AX,AY, 2*sup_box_dZ+thickness);
    tr_holes1-> RegisterYourself();
    TGeoCompositeShape * holes1 = new TGeoCompositeShape();
    holes1 = screw_array(N_holes); holes1->SetName("D0holes1");

    //Screwholes Area #2
    AX=-8.25; AY=7.41; N_holes = 2;
    TGeoTranslation *tr_holes2= new TGeoTranslation ("D0tr_holes2",AX,AY, 2*sup_box_dZ+thickness);
    tr_holes2-> RegisterYourself();
    TGeoCompositeShape * holes2 = new TGeoCompositeShape();
    holes2 = screw_array(N_holes,-1.7); holes2->SetName("D0holes2");

    //Screwholes Area #3
    AX=3.65; AY=10.42; N_holes = 3;
    TGeoTranslation *tr_holes3= new TGeoTranslation ("D0tr_holes3",AX,AY, 2*sup_box_dZ+thickness);
    tr_holes3-> RegisterYourself();
    TGeoCompositeShape * holes3 = new TGeoCompositeShape();
    holes3 = screw_array(N_holes,1.7); holes3->SetName("D0holes3");

    //Screwholes Area #4
    AX=-3.15; AY=10.42; N_holes = 3;
    TGeoTranslation *tr_holes4= new TGeoTranslation ("D0tr_holes4",AX,AY, 2*sup_box_dZ+thickness);
    tr_holes4-> RegisterYourself();
    TGeoCompositeShape * holes4 = new TGeoCompositeShape();
    holes4 = screw_array(N_holes,-1.7); holes4->SetName("D0holes4");

    //Screwholes Area #5 (not aligned!!!)
    //A
    AX=1.95; AY=12.27;
    TGeoTranslation *tr_holes5a= new TGeoTranslation ("D0tr_holes5a",AX,AY, 2*sup_box_dZ+thickness);
    tr_holes5a-> RegisterYourself();
    TGeoCompositeShape * holes5a = new TGeoCompositeShape();
    holes5a = screw_array(1); holes5a->SetName("D0holes5a");
    //B
    AX=0.25; AY=12.52;
    TGeoTranslation *tr_holes5b= new TGeoTranslation ("D0tr_holes5b",AX,AY, 2*sup_box_dZ+thickness);
    tr_holes5b-> RegisterYourself();
    TGeoCompositeShape * holes5b = new TGeoCompositeShape();
    holes5b = screw_array(1); holes5b->SetName("D0holes5b");
    //C
    AX=-1.45; AY=12.43;
    TGeoTranslation *tr_holes5c= new TGeoTranslation ("D0tr_holes5c",AX,AY, 2*sup_box_dZ+thickness);
    tr_holes5c-> RegisterYourself();
    TGeoCompositeShape * holes5c = new TGeoCompositeShape();
    holes5c = screw_array(1); holes5c->SetName("D0holes5c");
    TGeoCompositeShape *holes5 = new TGeoCompositeShape ("D0holes5", "(D0holes5a:D0tr_holes5a+D0holes5b:D0tr_holes5b+D0holes5c:D0tr_holes5c)");

    //ScrewHoles C, D; and E

    TGeoRotation *rotscrewC = new TGeoRotation("D0rotscrewC",0.,-90.,0.);
    rotscrewC->RegisterYourself();

    TGeoRotation *rotscrewDE = new TGeoRotation("D0rotscrewDE",0.,90.,0.);
    rotscrewDE->RegisterYourself();

    //Screwholes C
    AX=15.5, AY=6.5;
    TGeoCompositeShape * holesC = new TGeoCompositeShape();
    holesC = screw_C(); holesC->SetName("D0holesC");
    TGeoCombiTrans * rot_tr_screwC = new TGeoCombiTrans("D0rot_tr_screwC",AX,AY, 0.,rotscrewC);
    rot_tr_screwC->RegisterYourself();


    //Screwholes D
    AX=11.7, AY=1.7;
    TGeoCompositeShape * holesD = new TGeoCompositeShape();
    holesD = screw_D(); holesD->SetName("D0holesD");
    TGeoCombiTrans * rot_tr_screwD = new TGeoCombiTrans("D0rot_tr_screwD",AX,AY, 0.,rotscrewDE);
    rot_tr_screwD->RegisterYourself();

    //Screwholes E
    AX=12.1, AY=1.7;
    TGeoCompositeShape * holesE = new TGeoCompositeShape();
    holesE = screw_E(); holesE->SetName("D0holesE");
    TGeoCombiTrans * rot_tr_screwE = new TGeoCombiTrans("D0rot_tr_screwE",AX,AY, 0.,rotscrewDE);
    rot_tr_screwE->RegisterYourself();

    //Through Hole A
    AX=16.6, AY=2;
    TGeoCompositeShape * ThRA = new TGeoCompositeShape();
    ThRA = through_hole_a(); ThRA->SetName("D0ThRA");
    TGeoTranslation * tr_ThRA = new TGeoTranslation("D0tr_ThRA",AX,AY, 0.);
    tr_ThRA->RegisterYourself();
    //through_hole_a

    //Through Hole B
    AX=16.6, AY=3;
    TGeoCompositeShape * ThRB = new TGeoCompositeShape();
    ThRB = through_hole_b(); ThRB->SetName("D0ThRB");
    TGeoTranslation * tr_ThRB = new TGeoTranslation("D0tr_ThRB",AX,AY, 0.);
    tr_ThRB->RegisterYourself();

    //Through Hole C
    AX=15.5, AY=4.7;
    TGeoCompositeShape * ThRC = new TGeoCompositeShape();
    ThRC = through_hole_c(); ThRC->SetName("D0ThRC");
    TGeoTranslation * tr_ThRC = new TGeoTranslation("D0tr_ThRC",AX,AY, 0.);
    tr_ThRC->RegisterYourself();


    //Through Hole D
    AX=14., AY=9.5;
    TGeoCompositeShape * ThRD = new TGeoCompositeShape();
    ThRD = through_hole_d(); ThRD->SetName("D0ThRD");
    TGeoTranslation * tr_ThRD = new TGeoTranslation("D0tr_ThRD",AX,AY, 0.);
    tr_ThRD->RegisterYourself();

    //Through Hole E
    AX=11.2, AY=9.5;
    TGeoCompositeShape * ThRE = new TGeoCompositeShape();
    ThRE = through_hole_e(); ThRE->SetName("D0ThRE");
    TGeoTranslation * tr_ThRE = new TGeoTranslation("D0tr_ThRE",AX,AY, 0.);
    tr_ThRE->RegisterYourself();

    //Combining all relief holes of one side
    TGeoCompositeShape *holes = new TGeoCompositeShape ("D0holes", "(D0holes1:D0tr_holes1+D0holes2:D0tr_holes2+D0holes3:D0tr_holes3+D0holes4:D0tr_holes4+D0holesC:D0rot_tr_screwC+D0holesD:D0rot_tr_screwD+D0holesE:D0rot_tr_screwE+D0holes5+D0ThRA:D0tr_ThRA+D0ThRB:D0tr_ThRB+D0ThRC:D0tr_ThRC+D0ThRD:D0tr_ThRD+D0ThRE:D0tr_ThRE)");

    //Create screw areas and holes in the backside by rotation
    TGeoRotation *rotback = new TGeoRotation("D0rotback",0.,180.,180.);
    rotback->RegisterYourself();

   //Removing though holes and large voids


    // Void01

    Double_t voidbox_dZ= thickness+t_delta; //This is common to all though holes and voids

    // Void01 box1
    Double_t voidbox_dX=.9/2, voidbox_dY = 3.7/2;
    Double_t voidbox_X=13.35, voidbox_Y = 6.85;
    TGeoBBox   *voidbox1 = new TGeoBBox("D0voidbox1",voidbox_dX,voidbox_dY,voidbox_dZ);
    TGeoTranslation *tr_voidbox1= new TGeoTranslation ("D0tr_voidbox1",voidbox_X,voidbox_Y,0);
    tr_voidbox1-> RegisterYourself();

    // Void01 box2
    voidbox_dX=1.9/2, voidbox_dY = 3.2/2;
    voidbox_X=13.85, voidbox_Y = 3.4;
    TGeoBBox   *voidbox2 = new TGeoBBox("D0voidbox2",voidbox_dX,voidbox_dY,voidbox_dZ);
    TGeoTranslation *tr_voidbox2= new TGeoTranslation ("D0tr_voidbox2",voidbox_X,voidbox_Y,0);
    tr_voidbox2-> RegisterYourself();

    // Void01 box3
    voidbox_dX=3.25/2, voidbox_dY = 1.29/2;
    voidbox_X=12.175, voidbox_Y = 8.055;
    TGeoBBox   *voidbox3 = new TGeoBBox("D0voidbox3",voidbox_dX,voidbox_dY,voidbox_dZ);
    TGeoTranslation *tr_voidbox3= new TGeoTranslation ("D0tr_voidbox3",voidbox_X,voidbox_Y,0);
    tr_voidbox3-> RegisterYourself();

    // Voids arcs

    TGeoTubeSeg *voidsarcA = new TGeoTubeSeg("D0arcA",.5,.75, thickness+t_delta,180.,270.);
    TGeoTubeSeg *voidsarcB = new TGeoTubeSeg("D0arcB",.5,.75, thickness+t_delta,270.,360.);
    TGeoTubeSeg *voidsarcC = new TGeoTubeSeg("D0arcC",.5,.75, thickness+t_delta,0.,90.);
    TGeoTubeSeg *voidsarcD = new TGeoTubeSeg("D0arcD",.5,.75, thickness+t_delta,90.,180.);
    TGeoTubeSeg *voidsarcE = new TGeoTubeSeg("D0arcE",0,.5, thickness+t_delta,0,180);
    TGeoTubeSeg *voidsarcF = new TGeoTubeSeg("D0arcF",15.01,16.5,thickness+t_delta,180-124.446,180-117.048);
    TGeoTubeSeg *voidsarcG = new TGeoTubeSeg("D0arcG",0.,.5,thickness+t_delta,180-117.048,270.-180);

    TGeoTranslation *trv01arc01  = new TGeoTranslation(14.3, 2.3, 0.);//B- -> arcB:trv01arc01
    trv01arc01->SetName("D0trv01arc01"); trv01arc01->RegisterYourself();

    TGeoTranslation *trv01arc01b  = new TGeoTranslation(13.4, 2.3, 0.);//A- -> arcA:trv01arc01b
    trv01arc01b->SetName("D0trv01arc01b"); trv01arc01b->RegisterYourself();

    TGeoTranslation *trv01arc02  = new TGeoTranslation(14.3, 4.5, 0.);//C- -> arcC:trv01arc02
    trv01arc02->SetName("D0trv01arc02"); trv01arc02->RegisterYourself();

    TGeoTranslation *trv01arc02b  = new TGeoTranslation(14.3, 5.5, 0.);//A+ -> arcA:trv01arc02b
    trv01arc02b->SetName("D0trv01arc02b"); trv01arc02b->RegisterYourself();

    TGeoTranslation *trv01arc03  = new TGeoTranslation(13.3, 8.2, 0.);//C- -> arcC:trv01arc03
    trv01arc03->SetName("D0trv01arc03"); trv01arc03->RegisterYourself();

    TGeoTranslation *trv01arc03b  = new TGeoTranslation(12.4, 6.91, 0.);//C+ -> arcC:trv01arc03b
    trv01arc03b->SetName("D0trv01arc03b"); trv01arc03b->RegisterYourself();

    TGeoTranslation *trv01arc04  = new TGeoTranslation(11.05, 7.91, 0.);//A- -> arcA:trv01arc04
    trv01arc04->SetName("D0trv01arc04"); trv01arc04->RegisterYourself();

    TGeoTranslation *trv01arc04b  = new TGeoTranslation(11.05, 8.2, 0.);//D- -> arcD:trv01arc04b
    trv01arc04b->SetName("D0trv01arc04b"); trv01arc04b->RegisterYourself();


    //Smoothing corners
    TGeoCompositeShape *void1 = new TGeoCompositeShape ("D0void1", "D0voidbox1:D0tr_voidbox1+D0voidbox2:D0tr_voidbox2+D0voidbox3:D0tr_voidbox3 +   (D0arcA:D0trv01arc02b+D0arcC:D0trv01arc03b) -(D0arcB:D0trv01arc01+D0arcA:D0trv01arc01b+D0arcC:D0trv01arc02+D0arcC:D0trv01arc03+D0arcA:D0trv01arc04+D0arcD:D0trv01arc04b)");

    //void1->Draw();

    // Void02  *******************************


   // Box  Void02

     voidbox_dX=1.35/2, voidbox_dY = 3.4/2; voidbox_X=8.875, voidbox_Y = 11.5;
     TGeoBBox   *void02_Box001 = new TGeoBBox("D0void02_Box001",voidbox_dX,voidbox_dY,thickness+t_delta);
     TGeoTranslation *tr_v02box001= new TGeoTranslation("D0tr_v02box001",voidbox_X,voidbox_Y,0);
     tr_v02box001-> RegisterYourself();

     voidbox_dX=4.2759/2, voidbox_dY = 1.95/2; voidbox_X=5.13795, voidbox_Y = 13.775;
     TGeoBBox   *void02_Box002 = new TGeoBBox("D0void02_Box002",voidbox_dX,voidbox_dY,thickness+t_delta);
     TGeoTranslation *tr_v02box002= new TGeoTranslation("D0tr_v02box002",voidbox_X,voidbox_Y,0);
     tr_v02box002-> RegisterYourself();

     voidbox_dX=1.1/2, voidbox_dY = .6/2; voidbox_X=7.31392, voidbox_Y = 13.1;
     TGeoBBox   *void02_Box003 = new TGeoBBox("D0void02_Box003",voidbox_dX,voidbox_dY,thickness+t_delta);
     TGeoTranslation *tr_v02box003= new TGeoTranslation("D0tr_v02box003",voidbox_X,voidbox_Y,0);
     tr_v02box003-> RegisterYourself();

   // =============  arcs Void02 ===============

      TGeoTranslation *tr02arcs001  = new TGeoTranslation(8.7, 10.3, 0.);
      tr02arcs001->SetName("D0tr02arcs001"); tr02arcs001->RegisterYourself(); //A- -> arcA:tr02arcs001

      TGeoTranslation *tr02arcs001b  = new TGeoTranslation(9.05, 10.3, 0.);
      tr02arcs001b->SetName("D0tr02arcs001b"); tr02arcs001b->RegisterYourself(); //B- -> arcB:tr02arcs001b

      TGeoTranslation *tr02arcs002  = new TGeoTranslation(7.7, 12.3, 0.);
      tr02arcs002->SetName("D0tr02arcs002"); tr02arcs002->RegisterYourself(); //C+ -> arcC:tr02arcs002

      TGeoTranslation *tr02arcs002b  = new TGeoTranslation(9.05, 13.2, 0.);
      tr02arcs002b->SetName("D0tr02arcs002b"); tr02arcs002b->RegisterYourself(); //C+ -> arcC:tr02arcs002b

      TGeoTranslation *tr02arcs003  = new TGeoTranslation(3.5, 13.3, 0.);
      tr02arcs003->SetName("D0tr02arcs003"); tr02arcs003->RegisterYourself(); //A- -> arcA:tr02arcs003

      TGeoTranslation *tr02arcs003b  = new TGeoTranslation(3.5, 14.25, 0.);
      tr02arcs003b->SetName("D0tr02arcs003b"); tr02arcs003b->RegisterYourself(); //D- -> arcD:tr02arcs003b

      TGeoTranslation *tr02arcs004  = new TGeoTranslation(7.27582, 14.25, 0.);
      tr02arcs004->SetName("D0tr02arcs004"); tr02arcs004->RegisterYourself(); //G- -> arcG:tr02arcs004

      TGeoCompositeShape *void02 = new TGeoCompositeShape ("D0void02", "(D0void02_Box001:D0tr_v02box001+D0void02_Box002:D0tr_v02box002+D0void02_Box003:D0tr_v02box003+D0arcC:D0tr02arcs002+D0arcE:D0tr02arcs002b+D0arcF+D0arcG:D0tr02arcs004)-(D0arcA:D0tr02arcs001+D0arcB:D0tr02arcs001b+D0arcA:D0tr02arcs003+D0arcD:D0tr02arcs003b)");
      //void02->Draw();

      //========================

      //Composing basedisk
      TGeoCompositeShape *basedisk = new TGeoCompositeShape ("D0basedisk", "(D0IntCutBox+D0ssboxes+D0ssboxes:D0rotback-D0void1-D0void02)-D0holes-D0holes:D0rotback-D0void1:D0rotback-D0void02:D0rotback");

      TGeoVolume *vol = new TGeoVolume("Disc_Support_00", basedisk);
      //vol->Raytrace();
      //vol->Draw(); //   TView *view = gPad->GetView(); view->ShowAxis();

      return vol;


}



//=============================================================================================
TGeoVolume* AliMFTSupport::Disc_Support_01 (){

  double rMin = 0,
          rMax = 17.5,
          thickness = .8/2,
          phi0 = 0.,
          phi1 = 180.,
          t_delta = 0.1,
          sup_box_dZ = 0.305/2;



   // ================= Disk_Support_01 - base tube =============


   TGeoTubeSeg *base = new TGeoTubeSeg("base",rMin,rMax,thickness,phi0,phi1);
   TGeoTranslation *tr  = new TGeoTranslation(0., 0., 0.);
   tr->SetName("tr"); tr->RegisterYourself();

   // ======= Disk_Support_01 - Inner cuts =========

    TGeoBBox   *IntCutBox1 = new TGeoBBox("IntCutBox1",rMax+t_delta, 2.4/2, thickness+t_delta);
    TGeoBBox   *IntCutBox2 = new TGeoBBox("IntCutBox2",25/2, 3.4/2,  thickness+t_delta);
    TGeoBBox   *IntCutBox3 = new TGeoBBox("IntCutBox3",23/2, 13.8/2,  thickness+t_delta);
    TGeoBBox   *IntCutBox4 = new TGeoBBox("IntCutBox4",15/2, 19.84/2,  thickness+t_delta);
    TGeoBBox   *IntCutBox5 = new TGeoBBox("IntCutBox5",5.1/2, 23.6/2,  thickness+t_delta);


    TGeoBBox   *ExtCutBox1 = new TGeoBBox("ExtCutBox1",8.2, 1.15,  thickness+t_delta);
    TGeoTranslation *tr7 = new TGeoTranslation(0., 16.65, 0.);
    tr7->SetName("tr7"); tr7->RegisterYourself();

    TGeoBBox   *ExtCutBox2 = new TGeoBBox("ExtCutBox2",1.3875, 1.45,  thickness+t_delta);
      TGeoTranslation *tr8 = new TGeoTranslation(-16.1875, 7.9, 0.);
    tr8->SetName("tr8"); tr8->RegisterYourself();

          TGeoTranslation *tr6 = new TGeoTranslation(16.1875, 7.9, 0.);
    tr6->SetName("tr6"); tr6->RegisterYourself();


    TGeoCompositeShape *IntCutBox = new TGeoCompositeShape
    ("IntCutBox", "base-(IntCutBox1 + IntCutBox2 + IntCutBox3+ IntCutBox4 + IntCutBox5+ExtCutBox2:tr6  + ExtCutBox1:tr7+ ExtCutBox2:tr8 )");

    // ================= Screw holes reliefs  ====================


  //Screw support box 1
  Double_t ssbox1_dX=1.2/2, ssbox1_dY = 1.9/2;
  Double_t ssbox1_X=8.75, ssbox1_Y = 7.86, ssbox1_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox1 = new TGeoBBox("ssbox1",ssbox1_dX,ssbox1_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox1= new TGeoTranslation ("tr_ssbox1",ssbox1_X,ssbox1_Y,ssbox1_Z);
  tr_ssbox1-> RegisterYourself();

  //Screw support box 2
  Double_t ssbox2_dX=3.05/2, ssbox2_dY = 1.9/2;
  Double_t ssbox2_X=-9.025, ssbox2_Y = 7.86, ssbox2_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox2 = new TGeoBBox("ssbox2",ssbox2_dX,ssbox2_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox2= new TGeoTranslation ("tr_ssbox2",ssbox2_X,ssbox2_Y,ssbox2_Z);
  tr_ssbox2-> RegisterYourself();

  //Screw support box 3
  Double_t ssbox3_dX=4.6/2, ssbox3_dY = 1.9/2;
  Double_t ssbox3_X=5.35, ssbox3_Y = 10.87, ssbox3_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox3 = new TGeoBBox("ssbox3",ssbox3_dX,ssbox3_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox3= new TGeoTranslation ("tr_ssbox3",ssbox3_X,ssbox3_Y,ssbox3_Z);
  tr_ssbox3-> RegisterYourself();

  //Screw support box 4
  Double_t ssbox4_dX=4.6/2, ssbox4_dY = 1.88/2;
  Double_t ssbox4_X=-4.85, ssbox4_Y = 10.86, ssbox4_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox4 = new TGeoBBox("ssbox4",ssbox4_dX,ssbox4_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox4= new TGeoTranslation ("tr_ssbox4",ssbox4_X,ssbox4_Y,ssbox4_Z);
  tr_ssbox4-> RegisterYourself();

  //Screw support box 5
  Double_t ssbox5_dX=4.6/2, ssbox5_dY = 2.12/2;
  Double_t ssbox5_X=.25, ssbox5_Y = 12.86, ssbox5_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox5 = new TGeoBBox("ssbox5",ssbox5_dX,ssbox5_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox5= new TGeoTranslation ("tr_ssbox5",ssbox5_X,ssbox5_Y,ssbox5_Z);
  tr_ssbox5-> RegisterYourself();

  TGeoCompositeShape *ssboxes = new TGeoCompositeShape ("ssboxes",
   "ssbox1:tr_ssbox1+ssbox2:tr_ssbox2+ssbox3:tr_ssbox3+ssbox4:tr_ssbox4+ssbox5:tr_ssbox5");

  //Screwholes Area #1
  Double_t AX=8.75, AY=7.41;
  Int_t N_holes = 1;
  TGeoTranslation *tr_holes1= new TGeoTranslation ("tr_holes1",AX,AY, 2*sup_box_dZ+thickness);
  tr_holes1-> RegisterYourself();
  TGeoCompositeShape * holes1 = new TGeoCompositeShape();
  holes1 = screw_array(N_holes); holes1->SetName("holes1");
//  TGeoCompositeShape *cs_holes1 = new TGeoCompositeShape ("cs_holes1", "(basedisk-holes1:tr_holes1)");

  //Screwholes Area #2
  AX=-8.25; AY=7.41; N_holes = 2;
  TGeoTranslation *tr_holes2= new TGeoTranslation ("tr_holes2",AX,AY, 2*sup_box_dZ+thickness);
  tr_holes2-> RegisterYourself();
  TGeoCompositeShape * holes2 = new TGeoCompositeShape();
  holes2 = screw_array(N_holes,-1.7); holes2->SetName("holes2");
//  TGeoCompositeShape *cs_holes2 = new TGeoCompositeShape ("cs_holes2", "(cs_holes1-holes2:tr_holes2)");

  //Screwholes Area #3
  AX=3.65; AY=10.42; N_holes = 3;
  TGeoTranslation *tr_holes3= new TGeoTranslation ("tr_holes3",AX,AY, 2*sup_box_dZ+thickness);
  tr_holes3-> RegisterYourself();
  TGeoCompositeShape * holes3 = new TGeoCompositeShape();
  holes3 = screw_array(N_holes,1.7); holes3->SetName("holes3");
//  TGeoCompositeShape *cs_holes3 = new TGeoCompositeShape ("cs_holes3", "(cs_holes2-holes3:tr_holes3)");

  //Screwholes Area #4
  AX=-3.15; AY=10.42; N_holes = 3;
  TGeoTranslation *tr_holes4= new TGeoTranslation ("tr_holes4",AX,AY, 2*sup_box_dZ+thickness);
  tr_holes4-> RegisterYourself();
  TGeoCompositeShape * holes4 = new TGeoCompositeShape();
  holes4 = screw_array(N_holes,-1.7); holes4->SetName("holes4");
//  TGeoCompositeShape *cs_holes4 = new TGeoCompositeShape ("cs_holes4", "(cs_holes3-holes4:tr_holes4)");

  //Screwholes Area #5 (not aligned!!!)
  //A
  AX=1.95; AY=12.27;
  TGeoTranslation *tr_holes5a= new TGeoTranslation ("tr_holes5a",AX,AY, 2*sup_box_dZ+thickness);
  tr_holes5a-> RegisterYourself();
  TGeoCompositeShape * holes5a = new TGeoCompositeShape();
  holes5a = screw_array(1); holes5a->SetName("holes5a");
  //B
  AX=0.25; AY=12.52;
  TGeoTranslation *tr_holes5b= new TGeoTranslation ("tr_holes5b",AX,AY, 2*sup_box_dZ+thickness);
  tr_holes5b-> RegisterYourself();
  TGeoCompositeShape * holes5b = new TGeoCompositeShape();
  holes5b = screw_array(1); holes5b->SetName("holes5b");
  //C
  AX=-1.45; AY=12.43;
  TGeoTranslation *tr_holes5c= new TGeoTranslation ("tr_holes5c",AX,AY, 2*sup_box_dZ+thickness);
  tr_holes5c-> RegisterYourself();
  TGeoCompositeShape * holes5c = new TGeoCompositeShape();
  holes5c = screw_array(1); holes5c->SetName("holes5c");
  TGeoCompositeShape *holes5 = new TGeoCompositeShape ("holes5", "(holes5a:tr_holes5a+holes5b:tr_holes5b+holes5c:tr_holes5c)");

  //ScrewHoles C, D; and E

  TGeoRotation *rotscrewC = new TGeoRotation("rotscrewC",0.,-90.,0.);
  rotscrewC->RegisterYourself();

  TGeoRotation *rotscrewDE = new TGeoRotation("rotscrewDE",0.,90.,0.);
  rotscrewDE->RegisterYourself();

  //Screwholes C
  AX=15.5, AY=6.5;
  TGeoCompositeShape * holesC = new TGeoCompositeShape();
  holesC = screw_C(); holesC->SetName("holesC");
  TGeoCombiTrans * rot_tr_screwC = new TGeoCombiTrans("rot_tr_screwC",AX,AY, 0.,rotscrewC);
  rot_tr_screwC->RegisterYourself();


  //Screwholes D
  AX=11.7, AY=1.7;
  TGeoCompositeShape * holesD = new TGeoCompositeShape();
  holesD = screw_D(); holesD->SetName("holesD");
  TGeoCombiTrans * rot_tr_screwD = new TGeoCombiTrans("rot_tr_screwD",AX,AY, 0.,rotscrewDE);
  rot_tr_screwD->RegisterYourself();

  //Screwholes E
  AX=12.1, AY=1.7;
  TGeoCompositeShape * holesE = new TGeoCompositeShape();
  holesE = screw_E(); holesE->SetName("holesE");
  TGeoCombiTrans * rot_tr_screwE = new TGeoCombiTrans("rot_tr_screwE",AX,AY, 0.,rotscrewDE);
  rot_tr_screwE->RegisterYourself();

  //Through Hole A
  AX=16.6, AY=2;
  TGeoCompositeShape * ThRA = new TGeoCompositeShape();
  ThRA = through_hole_a(); ThRA->SetName("ThRA");
  TGeoTranslation * tr_ThRA = new TGeoTranslation("tr_ThRA",AX,AY, 0.);
  tr_ThRA->RegisterYourself();
  //through_hole_a

  //Through Hole B
  AX=16.6, AY=3;
  TGeoCompositeShape * ThRB = new TGeoCompositeShape();
  ThRB = through_hole_b(); ThRB->SetName("ThRB");
  TGeoTranslation * tr_ThRB = new TGeoTranslation("tr_ThRB",AX,AY, 0.);
  tr_ThRB->RegisterYourself();

  //Through Hole C
  AX=15.5, AY=4.7;
  TGeoCompositeShape * ThRC = new TGeoCompositeShape();
  ThRC = through_hole_c(); ThRC->SetName("ThRC");
  TGeoTranslation * tr_ThRC = new TGeoTranslation("tr_ThRC",AX,AY, 0.);
  tr_ThRC->RegisterYourself();


  //Through Hole D
  AX=14., AY=9.5;
  TGeoCompositeShape * ThRD = new TGeoCompositeShape();
  ThRD = through_hole_d(); ThRD->SetName("ThRD");
  TGeoTranslation * tr_ThRD = new TGeoTranslation("tr_ThRD",AX,AY, 0.);
  tr_ThRD->RegisterYourself();

  //Through Hole E
  AX=11.2, AY=9.5;
  TGeoCompositeShape * ThRE = new TGeoCompositeShape();
  ThRE = through_hole_e(); ThRE->SetName("ThRE");
  TGeoTranslation * tr_ThRE = new TGeoTranslation("tr_ThRE",AX,AY, 0.);
  tr_ThRE->RegisterYourself();

  //Combining all relief holes of one side
  TGeoCompositeShape *holes = new TGeoCompositeShape ("d1holes", "(holes1:tr_holes1+holes2:tr_holes2+holes3:tr_holes3+holes4:tr_holes4+holesC:rot_tr_screwC+holesD:rot_tr_screwD+holesE:rot_tr_screwE+holes5+ThRA:tr_ThRA+ThRB:tr_ThRB+ThRC:tr_ThRC+ThRD:tr_ThRD+ThRE:tr_ThRE)");

  //Create screw areas and holes in the backside by rotation
  TGeoRotation *rotback = new TGeoRotation("rotback",0.,180.,180.);
  rotback->RegisterYourself();

 //Removing though holes and large voids


  // Void01

  Double_t voidbox_dZ= thickness+t_delta; //This is common to all though holes and voids

  // Void01 box1
  Double_t voidbox_dX=1.8/2, voidbox_dY = 5.5/2;
  Double_t voidbox_X=12.9, voidbox_Y = 5.95;
  TGeoBBox   *voidbox1 = new TGeoBBox("voidbox1",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_voidbox1= new TGeoTranslation ("tr_voidbox1",voidbox_X,voidbox_Y,0);
  tr_voidbox1-> RegisterYourself();

  // Void01 box2
  voidbox_dX=1.8/2, voidbox_dY = 3.3/2;
  voidbox_X=13.9, voidbox_Y = 3.35;
  TGeoBBox   *voidbox2 = new TGeoBBox("voidbox2",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_voidbox2= new TGeoTranslation ("tr_voidbox2",voidbox_X,voidbox_Y,0);
  tr_voidbox2-> RegisterYourself();

  // Void01 box3
  voidbox_dX=3.25/2, voidbox_dY = 1.29/2;
  voidbox_X=12.175, voidbox_Y = 8.055;
  TGeoBBox   *voidbox3 = new TGeoBBox("voidbox3",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_voidbox3= new TGeoTranslation ("tr_voidbox3",voidbox_X,voidbox_Y,0);
  tr_voidbox3-> RegisterYourself();

  // Voids arcs

  TGeoTubeSeg *voidsarcA = new TGeoTubeSeg("arcA",.5,.75, thickness+t_delta,180.,270.);
  TGeoTubeSeg *voidsarcB = new TGeoTubeSeg("arcB",.5,.75, thickness+t_delta,270.,360.);
  TGeoTubeSeg *voidsarcC = new TGeoTubeSeg("arcC",.5,.75, thickness+t_delta,0.,90.);
  TGeoTubeSeg *voidsarcD = new TGeoTubeSeg("arcD",.5,.75, thickness+t_delta,90.,180.);
  TGeoTubeSeg *voidsarcE = new TGeoTubeSeg("arcE",0,.5, thickness+t_delta,0,180);
  TGeoTubeSeg *voidsarcF = new TGeoTubeSeg("arcF",15.01,16.5,thickness+t_delta,180-124.446,180-115.008);
  TGeoTubeSeg *voidsarcG = new TGeoTubeSeg("arcG",0.,.5,thickness+t_delta,245.-180,270.-180);

  TGeoTranslation *trv01arc01  = new TGeoTranslation(14.3, 2.2, 0.);//B- -> arcB:trv01arc01
  trv01arc01->SetName("trv01arc01"); trv01arc01->RegisterYourself();

  TGeoTranslation *trv01arc01b  = new TGeoTranslation(13.5, 2.2, 0.);//A- -> arcA:trv01arc01b
  trv01arc01b->SetName("trv01arc01b"); trv01arc01b->RegisterYourself();

  TGeoTranslation *trv01arc02  = new TGeoTranslation(14.3, 4.5, 0.);//C- -> arcC:trv01arc02
  trv01arc02->SetName("trv01arc02"); trv01arc02->RegisterYourself();

  TGeoTranslation *trv01arc02b  = new TGeoTranslation(14.3, 5.5, 0.);//A+ -> arcA:trv01arc02b
  trv01arc02b->SetName("trv01arc02b"); trv01arc02b->RegisterYourself();

  TGeoTranslation *trv01arc03  = new TGeoTranslation(13.3, 8.2, 0.);//C- -> arcC:trv01arc03
  trv01arc03->SetName("trv01arc03"); trv01arc03->RegisterYourself();

  TGeoTranslation *trv01arc03b  = new TGeoTranslation(11.5, 6.91, 0.);//C+ -> arcC:trv01arc03b
  trv01arc03b->SetName("trv01arc03b"); trv01arc03b->RegisterYourself();

  TGeoTranslation *trv01arc04  = new TGeoTranslation(11.05, 7.91, 0.);//A- -> arcA:trv01arc04
  trv01arc04->SetName("trv01arc04"); trv01arc04->RegisterYourself();

  TGeoTranslation *trv01arc04b  = new TGeoTranslation(11.05, 8.2, 0.);//D- -> arcD:trv01arc04b
  trv01arc04b->SetName("trv01arc04b"); trv01arc04b->RegisterYourself();

  TGeoTranslation *trv01arc05  = new TGeoTranslation(12.5, 2.7, 0.);//C+ -> arcC:trv01arc05
  trv01arc05->SetName("trv01arc05"); trv01arc05->RegisterYourself();

  TGeoTranslation *trv01arc05b  = new TGeoTranslation(12.5, 3.7, 0.);//A- -> arcA:trv01arc05b
  trv01arc05b->SetName("trv01arc05b"); trv01arc05b->RegisterYourself();

  //Smoothing corners
  TGeoCompositeShape *void1 = new TGeoCompositeShape ("void1", "voidbox1:tr_voidbox1+voidbox2:tr_voidbox2+voidbox3:tr_voidbox3 +   (arcA:trv01arc02b+arcC:trv01arc03b+arcC:trv01arc05) -(arcB:trv01arc01+arcA:trv01arc01b+arcC:trv01arc02+arcC:trv01arc03+arcA:trv01arc04+arcD:trv01arc04b+arcA:trv01arc05b)");

  //void1->Draw();

  // Void02  *******************************


 // Box  Void02

   voidbox_dX=1.35/2, voidbox_dY = 3.4/2; voidbox_X=8.875, voidbox_Y = 11.5;
   TGeoBBox   *void02_Box001 = new TGeoBBox("void02_Box001",voidbox_dX,voidbox_dY,thickness+t_delta);
   TGeoTranslation *tr_v02box001= new TGeoTranslation("tr_v02box001",voidbox_X,voidbox_Y,0);
   tr_v02box001-> RegisterYourself();

   voidbox_dX=3.76392/2, voidbox_dY = 2.2/2; voidbox_X=4.88196, voidbox_Y = 13.9;
   TGeoBBox   *void02_Box002 = new TGeoBBox("void02_Box002",voidbox_dX,voidbox_dY,thickness+t_delta);
   TGeoTranslation *tr_v02box002= new TGeoTranslation("tr_v02box002",voidbox_X,voidbox_Y,0);
   tr_v02box002-> RegisterYourself();

   voidbox_dX=1.1/2, voidbox_dY = .6/2; voidbox_X=7.31392, voidbox_Y = 13.1;
   TGeoBBox   *void02_Box003 = new TGeoBBox("void02_Box003",voidbox_dX,voidbox_dY,thickness+t_delta);
   TGeoTranslation *tr_v02box003= new TGeoTranslation("tr_v02box003",voidbox_X,voidbox_Y,0);
   tr_v02box003-> RegisterYourself();

 // =============  arcs Void02 ===============

    TGeoTranslation *tr02arcs001  = new TGeoTranslation(8.7, 10.3, 0.);
    tr02arcs001->SetName("tr02arcs001"); tr02arcs001->RegisterYourself(); //A- -> arcA:tr02arcs001

    TGeoTranslation *tr02arcs001b  = new TGeoTranslation(9.05, 10.3, 0.);
    tr02arcs001b->SetName("tr02arcs001b"); tr02arcs001b->RegisterYourself(); //B- -> arcB:tr02arcs001b

    TGeoTranslation *tr02arcs002  = new TGeoTranslation(7.7, 12.3, 0.);
    tr02arcs002->SetName("tr02arcs002"); tr02arcs002->RegisterYourself(); //C+ -> arcC:tr02arcs002

    TGeoTranslation *tr02arcs002b  = new TGeoTranslation(9.05, 13.2, 0.);
    tr02arcs002b->SetName("tr02arcs002b"); tr02arcs002b->RegisterYourself(); //C+ -> arcC:tr02arcs002b

    TGeoTranslation *tr02arcs003  = new TGeoTranslation(3.5, 13.3, 0.);
    tr02arcs003->SetName("tr02arcs003"); tr02arcs003->RegisterYourself(); //A- -> arcA:tr02arcs003

    TGeoTranslation *tr02arcs003b  = new TGeoTranslation(3.5, 14.5, 0.);
    tr02arcs003b->SetName("tr02arcs003b"); tr02arcs003b->RegisterYourself(); //D- -> arcD:tr02arcs003b

    TGeoTranslation *tr02arcs004  = new TGeoTranslation(6.76387, 14.5, 0.);
    tr02arcs004->SetName("tr02arcs004"); tr02arcs004->RegisterYourself(); //G- -> arcG:tr02arcs004

    TGeoCompositeShape *void02 = new TGeoCompositeShape ("void02", "(void02_Box001:tr_v02box001+void02_Box002:tr_v02box002+void02_Box003:tr_v02box003+arcC:tr02arcs002+arcE:tr02arcs002b+arcF+arcG:tr02arcs004)-(arcA:tr02arcs001+arcB:tr02arcs001b+arcA:tr02arcs003+arcD:tr02arcs003b)");
    //void02->Draw();

    //========================

    //Composing basedisk
    TGeoCompositeShape *basedisk = new TGeoCompositeShape ("basedisk", "(IntCutBox+ssboxes+ssboxes:rotback-void1-void02)-d1holes-d1holes:rotback-void1:rotback-void02:rotback");

    TGeoVolume *vol = new TGeoVolume("Disc_Support_01", basedisk);
    //vol->Raytrace();
    //vol->Draw(); //   TView *view = gPad->GetView(); view->ShowAxis();

    return vol;


}

//=============================================================================================
TGeoVolume* AliMFTSupport::Disc_Support_02 (){
  // define shape components with names + positions

  // ================= constants ===========================

  double rMin = 0,
         rMax = 17.5,
         thickness = .8/2,
         phi0=0.,
         phi1=180. ,
         t_delta=0.1,
         sup_box_dZ = 0.305/2;

  //Rotation for adding and removing symmetric shapes
  TGeoRotation *rotback = new TGeoRotation("rotback",0.,180.,180.);
  rotback->RegisterYourself();

  // =================  basic struture ===================


  // ================= Disk_Support_02 - base =============


  TGeoTubeSeg *base2 = new TGeoTubeSeg("d2base",rMin,rMax,thickness,phi0,phi1);


  // ================= Screw holes reliefs and edge ====================


  //Screw support box 1
  Double_t ssbox1_dX=1.2/2, ssbox1_dY = 1.9/2;
  Double_t ssbox1_X=10.45, ssbox1_Y = 7.86, ssbox1_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox1 = new TGeoBBox("d2ssbox1",ssbox1_dX,ssbox1_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox1= new TGeoTranslation ("d2tr_ssbox1",ssbox1_X,ssbox1_Y,ssbox1_Z);
  tr_ssbox1-> RegisterYourself();

  //Screw support box 2
  Double_t ssbox2_dX=2.9/2, ssbox2_dY = 1.9/2;
  Double_t ssbox2_X=7.9, ssbox2_Y = 10.87, ssbox2_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox2 = new TGeoBBox("d2ssbox2",ssbox2_dX,ssbox2_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox2= new TGeoTranslation ("d2tr_ssbox2",ssbox2_X,ssbox2_Y,ssbox2_Z);
  tr_ssbox2-> RegisterYourself();

  //Screw support box 3
  Double_t ssbox3_dX=2.9/2, ssbox3_dY = 2.53/2;
  Double_t ssbox3_X=4.5, ssbox3_Y = 13.565, ssbox3_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox3 = new TGeoBBox("d2ssbox3",ssbox3_dX,ssbox3_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox3= new TGeoTranslation ("d2tr_ssbox3",ssbox3_X,ssbox3_Y,ssbox3_Z);
  tr_ssbox3-> RegisterYourself();

  //Screw support box 4
  Double_t ssbox4_dX=4.6/2, ssbox4_dY = 2.17/2;
  Double_t ssbox4_X=0.25, ssbox4_Y = 12.835, ssbox4_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox4 = new TGeoBBox("d2ssbox4",ssbox4_dX,ssbox4_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox4= new TGeoTranslation ("d2tr_ssbox4",ssbox4_X,ssbox4_Y,ssbox4_Z);
  tr_ssbox4-> RegisterYourself();

  //Screw support box 5
  Double_t ssbox5_dX=2.9/2, ssbox5_dY = 2.53/2;
  Double_t ssbox5_X=-4.0, ssbox5_Y = 13.565, ssbox5_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox5 = new TGeoBBox("d2ssbox5",ssbox5_dX,ssbox5_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox5= new TGeoTranslation ("d2tr_ssbox5",ssbox5_X,ssbox5_Y,ssbox5_Z);
  tr_ssbox5-> RegisterYourself();

  //Screw support box6
  Double_t ssbox6_dX=2.9/2, ssbox6_dY = 1.9/2;
  Double_t ssbox6_X=-7.4, ssbox6_Y = 10.87, ssbox6_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox6 = new TGeoBBox("d2ssbox6",ssbox6_dX,ssbox6_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox6= new TGeoTranslation ("d2tr_ssbox6",ssbox6_X,ssbox6_Y,ssbox6_Z);
  tr_ssbox6-> RegisterYourself();

  //Screw support box7
  Double_t ssbox7_dX=1.2/2, ssbox7_dY = 1.9/2;
  Double_t ssbox7_X=-9.95, ssbox7_Y = 7.86, ssbox7_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox7 = new TGeoBBox("d2ssbox7",ssbox7_dX,ssbox7_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox7= new TGeoTranslation ("d2tr_ssbox7",ssbox7_X,ssbox7_Y,ssbox7_Z);
  tr_ssbox7-> RegisterYourself();

  //EdgeBox1
  Double_t edgebox1_dX=6.3/2, edgebox1_dY = .37/2;
  Double_t edgebox1_X=3.15, edgebox1_Y = 16.715, edgebox1_dZ= thickness ;
  TGeoBBox   *edgebox1 = new TGeoBBox("d2edgebox1",edgebox1_dX,edgebox1_dY,edgebox1_dZ);
  TGeoTranslation *tr_edgebox1 = new TGeoTranslation ("trd2edgebox1",edgebox1_X,edgebox1_Y,0);
  tr_edgebox1-> RegisterYourself();

  //EdgeBox1
  Double_t edgebox2_dX=0.04/2, edgebox2_dY = .12/2;
  Double_t edgebox2_X=6.48, edgebox2_Y = 16.64, edgebox2_dZ= thickness ;
  TGeoBBox   *edgebox2 = new TGeoBBox("d2edgebox2",edgebox2_dX,edgebox2_dY,edgebox2_dZ);
  TGeoTranslation *tr_edgebox2 = new TGeoTranslation ("trd2edgebox2",edgebox2_X,edgebox2_Y,0);
  tr_edgebox2-> RegisterYourself();

  //EdgeTube1
  TGeoTube *EdgeTube1 = new TGeoTube("d2edgetube1",0,.2,thickness);
  TGeoTranslation *tr_edgetube1 = new TGeoTranslation ("trd2edgetube1",6.3,16.7,0);
  tr_edgetube1-> RegisterYourself();

  //EdgeTubeSeg1
  TGeoTubeSeg *edgetubeseg1 = new TGeoTubeSeg("d2edgetubeseg1",.5,1.1,thickness,180.0,180+67.07);
  TGeoTranslation *tr_edgetubeseg1 = new TGeoTranslation ("trd2edgetubeseg1",7,16.5831,0);
  tr_edgetubeseg1-> RegisterYourself();

  //EdgeTubeSeg2
  TGeoTubeSeg *edgetubeseg2 = new TGeoTubeSeg("d2edgetubeseg2",.5,.7,thickness,35.215,90);
  TGeoTranslation *tr_edgetubeseg2 = new TGeoTranslation ("trd2edgetubeseg2",5.5,16.03,0);
  tr_edgetubeseg2-> RegisterYourself();

  TGeoCompositeShape *ssboxes = new TGeoCompositeShape ("d2ssboxes",
  "d2ssbox1:d2tr_ssbox1 + d2ssbox2:d2tr_ssbox2 + d2ssbox3:d2tr_ssbox3 + d2ssbox4:d2tr_ssbox4 + d2ssbox5:d2tr_ssbox5 + d2ssbox6:d2tr_ssbox6 + d2ssbox7:d2tr_ssbox7 + d2edgebox1:trd2edgebox1 + d2edgebox2:trd2edgebox2 + d2edgetube1:trd2edgetube1 + d2edgetubeseg1:trd2edgetubeseg1 + d2edgetubeseg2:trd2edgetubeseg2");

  TGeoCompositeShape *d2base2 = new TGeoCompositeShape ("d2base2",
  "d2base+d2ssboxes+d2ssboxes:rotback");

  // Cutting disk

  TGeoBBox   *d2_cut0 = new TGeoBBox("d2_cut0",rMax+ t_delta, 2.4/2, thickness+t_delta);
  TGeoBBox   *d2_cut1 = new TGeoBBox("d2_cut1",26./2, 3.4/2,thickness+t_delta);
  TGeoBBox   *d2_cut2 = new TGeoBBox("d2_cut2",24./2, 13.82/2,thickness+t_delta);
  TGeoBBox   *d2_cut3 = new TGeoBBox("d2_cut3",18.7/2, 19.84/2,thickness+t_delta);
  TGeoBBox   *d2_cut4 = new TGeoBBox("d2_cut4",11.9/2, 23.5/2,  thickness+t_delta);
  TGeoBBox   *d2_cut5a = new TGeoBBox("d2_cut5a",3.4/2, 24.6/2,  thickness+t_delta);

  TGeoBBox   *d2_cut5b = new TGeoBBox("d2_cut5b",3.4/2, 24.6/2,  thickness+t_delta); //This dupplication does not remove problems with visualization/rendering
  TGeoTranslation *d2_trcut5a  = new TGeoTranslation(4.25, 0., 0.);
  d2_trcut5a->SetName("d2_trcut5a"); d2_trcut5a->RegisterYourself();
  TGeoTranslation *d2_trcut5b  = new TGeoTranslation(-4.25, 0., 0.);
  d2_trcut5b->SetName("d2_trcut5b"); d2_trcut5b->RegisterYourself();

  TGeoBBox   *d2_cut6 = new TGeoBBox("d2_cut6",1.6/2, 3.0/2,  thickness+t_delta); //This dupplication does not remove problems with visualization/rendering
  TGeoTranslation *d2_trcut6a  = new TGeoTranslation(15.6, 8.0, 0.);
  d2_trcut6a->SetName("d2_trcut6a"); d2_trcut6a->RegisterYourself();
  TGeoTranslation *d2_trcut6b  = new TGeoTranslation(-15.6, 8.0, 0.);
  d2_trcut6b->SetName("d2_trcut6b"); d2_trcut6b->RegisterYourself();

  TGeoBBox   *d2_cut7 = new TGeoBBox("d2_cut7",10./2, .7/2,  thickness+t_delta);
  TGeoTranslation *d2_trcut7  = new TGeoTranslation(0., 17.25, 0.);
  d2_trcut7->SetName("d2_trcut7"); d2_trcut7->RegisterYourself();


  // Void01

  Double_t voidbox_dZ= thickness+t_delta; //This is common to all though holes and voids

  // Void01 box1
  Double_t voidbox_dX=1.3/2, voidbox_dY = 5.5/2;
  Double_t voidbox_X=13.15, voidbox_Y = 5.95;
  TGeoBBox   *voidbox1 = new TGeoBBox("d2voidbox1",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_voidbox1= new TGeoTranslation ("tr_d2voidbox1",voidbox_X,voidbox_Y,0);
  tr_voidbox1-> RegisterYourself();

  // Void01 box2
  voidbox_dX=1.8/2, voidbox_dY = 2.5/2;
  voidbox_X=14.4, voidbox_Y = 2.95;
  TGeoBBox   *voidbox2 = new TGeoBBox("d2voidbox2",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_voidbox2= new TGeoTranslation ("tr_d2voidbox2",voidbox_X,voidbox_Y,0);
  tr_voidbox2-> RegisterYourself();

  // Void01 box3
  voidbox_dX=2.75/2, voidbox_dY = 1.29/2;
  voidbox_X=12.425, voidbox_Y = 8.055;
  TGeoBBox   *voidbox3 = new TGeoBBox("d2voidbox3",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_voidbox3= new TGeoTranslation ("tr_d2voidbox3",voidbox_X,voidbox_Y,0);
  tr_voidbox3-> RegisterYourself();

  // Voids arcs

  TGeoTubeSeg *voidsarcA = new TGeoTubeSeg("d2arcA",.5,.75, thickness+t_delta,180.,270.);
  TGeoTubeSeg *voidsarcB = new TGeoTubeSeg("d2arcB",.5,.75, thickness+t_delta,270.,360.);
  TGeoTubeSeg *voidsarcC = new TGeoTubeSeg("d2arcC",.5,.75, thickness+t_delta,0.,90.);
  TGeoTubeSeg *voidsarcD = new TGeoTubeSeg("d2arcD",.5,.75, thickness+t_delta,90.,180.);
  TGeoTubeSeg *voidsarcE = new TGeoTubeSeg("d2arcE",0,.5, thickness+t_delta,0,180);
  TGeoTubeSeg *voidsarcF = new TGeoTubeSeg("d2arcF",15.01,16.5,thickness+t_delta,180-124.446,180-115.008);
  TGeoTubeSeg *voidsarcG = new TGeoTubeSeg("d2arcG",0.,.5,thickness+t_delta,245.-180,270.-180);

  TGeoTranslation *trv01arc01  = new TGeoTranslation(14.8, 2.2, 0.);//B- -> arcB:trv01arc01
  trv01arc01->SetName("trv01d2arc01"); trv01arc01->RegisterYourself();

  TGeoTranslation *trv01arc01b  = new TGeoTranslation(14.0, 2.2, 0.);//A- -> arcA:trv01arc01b
  trv01arc01b->SetName("trv01d2arc01b"); trv01arc01b->RegisterYourself();

  TGeoTranslation *trv01arc02  = new TGeoTranslation(14.8, 3.7, 0.);//C- -> arcC:trv01arc02
  trv01arc02->SetName("trv01d2arc02"); trv01arc02->RegisterYourself();

  TGeoTranslation *trv01arc02b  = new TGeoTranslation(14.3, 4.7, 0.);//A+ -> arcA:trv01arc02b
  trv01arc02b->SetName("trv01d2arc02b"); trv01arc02b->RegisterYourself();

  TGeoTranslation *trv01arc03  = new TGeoTranslation(13.3, 8.2, 0.);//C- -> arcC:trv01arc03
  trv01arc03->SetName("trv01d2arc03"); trv01arc03->RegisterYourself();

  TGeoTranslation *trv01arc03b  = new TGeoTranslation(12.0, 6.91, 0.);//C+ -> arcC:trv01arc03b
  trv01arc03b->SetName("trv01d2arc03b"); trv01arc03b->RegisterYourself();

  TGeoTranslation *trv01arc04  = new TGeoTranslation(11.55, 7.91, 0.);//A- -> arcA:trv01arc04
  trv01arc04->SetName("trv01d2arc04"); trv01arc04->RegisterYourself();

  TGeoTranslation *trv01arc04b  = new TGeoTranslation(11.55, 8.2, 0.);//D- -> arcD:trv01arc04b
  trv01arc04b->SetName("trv01d2arc04b"); trv01arc04b->RegisterYourself();

  TGeoTranslation *trv01arc05  = new TGeoTranslation(13.0, 2.7, 0.);//C+ -> arcC:trv01arc05
  trv01arc05->SetName("trv01d2arc05"); trv01arc05->RegisterYourself();

  TGeoTranslation *trv01arc05b  = new TGeoTranslation(13.0, 3.7, 0.);//A- -> arcA:trv01arc05b
  trv01arc05b->SetName("trv01d2arc05b"); trv01arc05b->RegisterYourself();

  //Smoothing corners
  TGeoCompositeShape *void1 = new TGeoCompositeShape ("d2void1", "d2voidbox1:tr_d2voidbox1+d2voidbox2:tr_d2voidbox2+d2voidbox3:tr_d2voidbox3 +   (d2arcA:trv01d2arc02b+d2arcC:trv01d2arc03b+d2arcC:trv01d2arc05) -(d2arcB:trv01d2arc01+d2arcA:trv01d2arc01b+d2arcC:trv01d2arc02+d2arcC:trv01d2arc03+d2arcA:trv01d2arc04+d2arcD:trv01d2arc04b+d2arcA:trv01d2arc05b)");

  //void1->Draw();


  // Void02

  voidbox_dZ= thickness+t_delta; //This is common to all though holes and voids

  // Void01 box1
  voidbox_dX=2.88738/2, voidbox_dY = 1./2;
  voidbox_X=8.94369, voidbox_Y = 12.82;
  TGeoBBox   *void2box1 = new TGeoBBox("d2void2box1",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_void2box1= new TGeoTranslation ("tr_d2void2box1",voidbox_X,voidbox_Y,0);
  tr_void2box1-> RegisterYourself();

  // Void01 box2
  voidbox_dX=1.4/2, voidbox_dY = 1.87694/2;
  voidbox_X=7.7, voidbox_Y = 13.7585;
  TGeoBBox   *void2box2 = new TGeoBBox("d2void2box2",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_void2box2= new TGeoTranslation ("tr_d2void2box2",voidbox_X,voidbox_Y,0);
  tr_void2box2-> RegisterYourself();

  // Void01 box3
  voidbox_dX=.5/2, voidbox_dY = .5/2;
  voidbox_X=8.45, voidbox_Y = 13.35;
  TGeoBBox   *void2box3 = new TGeoBBox("d2void2box3",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_void2box3= new TGeoTranslation ("tr_d2void2box3",voidbox_X,voidbox_Y,0);
  tr_void2box3-> RegisterYourself();

  // Voids arcs and circles

  TGeoTubeSeg *void2sarc = new TGeoTubeSeg("d2void2arc",15.9,17.0,thickness+t_delta,180-129.016,180-117.036);

  TGeoTube *void2tube001 = new TGeoTube("d2voidtube001",0,0.5,thickness+t_delta);

  double voidtube_X=10.3874, voidtube_Y = 12.82;
  TGeoTranslation *tr_void2tub001= new TGeoTranslation ("tr_d2void2tube001",voidtube_X,voidtube_Y,0);
  tr_void2tub001-> RegisterYourself();

  voidtube_X=7.5, voidtube_Y = 12.82;
  TGeoTranslation *tr_void2tub002= new TGeoTranslation ("tr_d2void2tube002",voidtube_X,voidtube_Y,0);
  tr_void2tub002-> RegisterYourself();

  voidtube_X=7.5, voidtube_Y = 14.6969;
  TGeoTranslation *tr_void2tub003= new TGeoTranslation ("tr_d2void2tube003",voidtube_X,voidtube_Y,0);
  tr_void2tub003-> RegisterYourself();

  TGeoCompositeShape *void2 = new TGeoCompositeShape ("d2void2", "d2void2arc+d2void2box1:tr_d2void2box1+d2void2box2:tr_d2void2box2+d2void2box3:tr_d2void2box3+d2voidtube001:tr_d2void2tube001+d2voidtube001:tr_d2void2tube002+d2voidtube001:tr_d2void2tube003");

  // Void03

  // Void03 box1
  voidbox_dX=5.5/2+2.*t_delta, voidbox_dY = 1.0/2;
  voidbox_X=2.75, voidbox_Y = 16.03;
  TGeoBBox   *void3box1 = new TGeoBBox("d2void3box1",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_void3box1= new TGeoTranslation ("tr_d2void3box1",voidbox_X,voidbox_Y,0);
  tr_void3box1-> RegisterYourself();

  // Void03 box2
  voidbox_dX=1.55/2+2.*t_delta, voidbox_dY = 1.0/2;
  voidbox_X=.775, voidbox_Y = 14.92;
  TGeoBBox   *void3box2 = new TGeoBBox("d2void3box2",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_void3box2= new TGeoTranslation ("tr_d2void3box2",voidbox_X,voidbox_Y,0);
  tr_void3box2-> RegisterYourself();

  // Void03 box3
  voidbox_dX=2.55/2+2.*t_delta, voidbox_dY = .61/2;
  voidbox_X=1.275, voidbox_Y = 15.255;
  TGeoBBox   *void3box3 = new TGeoBBox("d2void3box3",voidbox_dX,voidbox_dY,voidbox_dZ);
  TGeoTranslation *tr_void3box3= new TGeoTranslation ("tr_d2void3box3",voidbox_X,voidbox_Y,0);
  tr_void3box3-> RegisterYourself();

  // Voids arcs and circles

  TGeoTube *void3tube001 = new TGeoTube("d2void3tube001",0,0.5,thickness+2*t_delta);

  voidtube_X=5.5, voidtube_Y = 16.03;
  TGeoTranslation *tr_void3tub001= new TGeoTranslation ("tr_d2void3tube001",voidtube_X,voidtube_Y,0);
  tr_void3tub001-> RegisterYourself();

  voidtube_X=1.55, voidtube_Y = 14.92;
  TGeoTranslation *tr_void3tub002= new TGeoTranslation ("tr_d2void3tube002",voidtube_X,voidtube_Y,0);
  tr_void3tub002-> RegisterYourself();

  TGeoTube *void3tube003 = new TGeoTube("d2void3tube003",0,0.5,thickness);
  voidtube_X=2.55, voidtube_Y = 15.03;
  TGeoTranslation *tr_void3tub003= new TGeoTranslation ("tr_d2void3tube003",voidtube_X,voidtube_Y,0);
  tr_void3tub003-> RegisterYourself();

  TGeoCompositeShape *void3 = new TGeoCompositeShape ("d2void3", "d2void3box1:tr_d2void3box1+d2void3box2:tr_d2void3box2+d2void3box3:tr_d2void3box3+d2void3tube001:tr_d2void3tube001+d2void3tube001:tr_d2void3tube002-d2void3tube003:tr_d2void3tube003");


    //Screwholes Area #1
    Double_t AX=10.45, AY=7.41; Int_t N_holes = 1;
    TGeoTranslation *tr_d2holes1= new TGeoTranslation ("d2tr_holes1",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes1-> RegisterYourself();
    TGeoCompositeShape * d2holes1 = new TGeoCompositeShape();
    d2holes1 = screw_array(N_holes); d2holes1->SetName("d2holes1");


    //Screwholes Area #2
    AX=8.75; AY=10.42; N_holes = 2;
    TGeoTranslation *tr_d2holes2= new TGeoTranslation ("d2tr_holes2",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes2-> RegisterYourself();
    TGeoCompositeShape * d2holes2 = new TGeoCompositeShape();
    d2holes2 = screw_array(N_holes,-1.7); d2holes2->SetName("d2holes2");


    //Screwholes Area #3
    AX=5.35; AY=13.43; N_holes = 2;
    TGeoTranslation *tr_d2holes3= new TGeoTranslation ("d2tr_holes3",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes3-> RegisterYourself();
    TGeoCompositeShape * d2holes3 = new TGeoCompositeShape();
    d2holes3 = screw_array(N_holes,-1.7); d2holes3->SetName("d2holes3");


    //Screwholes Area #4 (3 pairs out of alligment)
    AX=1.95; AY=12.27; N_holes = 1;
    TGeoTranslation *tr_d2holes4a = new TGeoTranslation ("d2tr_holes4a",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes4a-> RegisterYourself();
    TGeoCompositeShape * d2holes4a = new TGeoCompositeShape();
    d2holes4a = screw_array(N_holes,-1.7); d2holes4a->SetName("d2holes4a");

    AX=0.25; AY=12.52; N_holes = 1;
    TGeoTranslation *tr_d2holes4b = new TGeoTranslation ("d2tr_holes4b",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes4b-> RegisterYourself();
    TGeoCompositeShape * d2holes4b = new TGeoCompositeShape();
    d2holes4b = screw_array(N_holes,-1.7); d2holes4b->SetName("d2holes4b");

    AX=-1.45; AY=12.43; N_holes = 1;
    TGeoTranslation *tr_d2holes4c = new TGeoTranslation ("d2tr_holes4c",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes4c-> RegisterYourself();
    TGeoCompositeShape * d2holes4c = new TGeoCompositeShape();
    d2holes4c = screw_array(N_holes,-1.7); d2holes4c->SetName("d2holes4c");


    //Screwholes Area #5
    AX=-3.15; AY=13.43; N_holes = 2;
    TGeoTranslation *tr_d2holes5= new TGeoTranslation ("d2tr_holes5",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes5-> RegisterYourself();
    TGeoCompositeShape * d2holes5 = new TGeoCompositeShape();
    d2holes5 = screw_array(N_holes,-1.7); d2holes5->SetName("d2holes5");

    //Screwholes Area #6
    AX=-6.55; AY=10.42; N_holes = 2;
    TGeoTranslation *tr_d2holes6= new TGeoTranslation ("d2tr_holes6",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes6-> RegisterYourself();
    TGeoCompositeShape * d2holes6 = new TGeoCompositeShape();
    d2holes6 = screw_array(N_holes,-1.7); d2holes6->SetName("d2holes6");

    //Screwholes Area #7
    AX=-9.95, AY=7.41; N_holes = 1;
    TGeoTranslation *tr_d2holes7= new TGeoTranslation ("d2tr_holes7",AX,AY, 2*sup_box_dZ+thickness);
    tr_d2holes7-> RegisterYourself();
    TGeoCompositeShape * d2holes7 = new TGeoCompositeShape();
    d2holes7 = screw_array(N_holes); d2holes7->SetName("d2holes7");


    //ScrewHoles C, D; and E

    TGeoRotation *rotscrewC = new TGeoRotation("d2rotscrewC",0.,-90.,0.);
    rotscrewC->RegisterYourself();

    TGeoRotation *rotscrewDE = new TGeoRotation("d2rotscrewDE",0.,90.,0.);
    rotscrewDE->RegisterYourself();

    //Screwholes C
    AX=15.5, AY=6.5;
    TGeoCompositeShape * holesC = new TGeoCompositeShape();
    holesC = screw_C(); holesC->SetName("d2holesC");
    TGeoCombiTrans * rot_tr_screwC = new TGeoCombiTrans("d2rot_tr_screwC",AX,AY, 0.,rotscrewC);
    rot_tr_screwC->RegisterYourself();


    //Screwholes D
    AX=12.2, AY=1.7;
    TGeoCompositeShape * holesD = new TGeoCompositeShape();
    holesD = screw_D(); holesD->SetName("d2holesD");
    TGeoCombiTrans * rot_tr_screwD = new TGeoCombiTrans("d2rot_tr_screwD",AX,AY, 0.,rotscrewDE);
    rot_tr_screwD->RegisterYourself();

    //Screwholes E
    AX=12.6, AY=1.7;
    TGeoCompositeShape * holesE = new TGeoCompositeShape();
    holesE = screw_E(); holesE->SetName("d2holesE");
    TGeoCombiTrans * rot_tr_screwE = new TGeoCombiTrans("d2rot_tr_screwE",AX,AY, 0.,rotscrewDE);
    rot_tr_screwE->RegisterYourself();

    //Through Hole A  disk2
    AX=16.6, AY=2;
    TGeoCompositeShape * ThRA = new TGeoCompositeShape();
    ThRA = through_hole_a(); ThRA->SetName("d2ThRA");
    TGeoTranslation * tr_ThRA = new TGeoTranslation("d2tr_ThRA",AX,AY, 0.);
    tr_ThRA->RegisterYourself();
    //through_hole_a

    //Through Hole B  disk2
    AX=16.6, AY=3;
    TGeoCompositeShape * ThRB = new TGeoCompositeShape();
    ThRB = through_hole_b(); ThRB->SetName("d2ThRB");
    TGeoTranslation * tr_ThRB = new TGeoTranslation("d2tr_ThRB",AX,AY, 0.);
    tr_ThRB->RegisterYourself();

    //Through Hole C disk2 - Radius 2.9585 mm
    AX=15.5, AY=4.7;
    //TGeoCompositeShape * ThRC = new TGeoCompositeShape();
    //ThRC = through_hole_c(); ThRC->SetName("d2ThRC");
    TGeoTube *ThRC = new TGeoTube("d2ThRC",0, 0.29585, thickness+t_delta);
    TGeoTranslation * tr_ThRC = new TGeoTranslation("d2tr_ThRC",AX,AY, 0.);
    tr_ThRC->RegisterYourself();


    //Through Hole D disk2 - Radius 1.5 mm
    AX=14., AY=9.5;
    //TGeoCompositeShape * ThRD = new TGeoCompositeShape();
    TGeoTube *ThRD = new TGeoTube("d2ThRD",0, 0.15, thickness+t_delta);
    //ThRD = through_hole_d(); ThRD->SetName("d2ThRD");
    TGeoTranslation * tr_ThRD = new TGeoTranslation("d2tr_ThRD",AX,AY, 0.);
    tr_ThRD->RegisterYourself();

    //Through Hole E
    AX=12.0, AY=9.5;
    //TGeoCompositeShape * ThRE = new TGeoCompositeShape();
    //ThRE = through_hole_e(); ThRE->SetName("d2ThRE");
    TGeoTube *ThRE = new TGeoTube("d2ThRE",0, 0.12295, thickness+t_delta);
    TGeoTranslation * tr_ThRE = new TGeoTranslation("d2tr_ThRE",AX,AY, 0.);
    tr_ThRE->RegisterYourself();



  // compose base
  TGeoCompositeShape *supportcut_comp = new TGeoCompositeShape
  ("Disc_Support_02_shape", "(d2base2-d2_cut0-d2_cut1-d2_cut2-d2_cut3-d2_cut4-d2_cut5a:d2_trcut5a-d2_cut5b:d2_trcut5b-d2_cut6:d2_trcut6a-d2_cut6:d2_trcut6b-d2_cut7:d2_trcut7 )");

  //Combining all relief holes of one side
  TGeoCompositeShape *d2holes = new TGeoCompositeShape ("d2holes", "d2void1 + d2void2 + d2void3 + d2holes1:d2tr_holes1 + d2holes2:d2tr_holes2 + d2holes3:d2tr_holes3 + d2holes4a:d2tr_holes4a+ d2holes4b:d2tr_holes4b+ d2holes4c:d2tr_holes4c + d2holes5:d2tr_holes5 + d2holes6:d2tr_holes6 + d2holes7:d2tr_holes7 + d2holesC:d2rot_tr_screwC + d2holesD:d2rot_tr_screwD + d2holesE:d2rot_tr_screwE + d2ThRA:d2tr_ThRA + d2ThRB:d2tr_ThRB+d2ThRC:d2tr_ThRC + d2ThRD:d2tr_ThRD + d2ThRE:d2tr_ThRE");


  TGeoCompositeShape *basedisk2 = new TGeoCompositeShape ("basesupport2", "Disc_Support_02_shape-d2holes-d2holes:rotback");

  TGeoVolume *vol = new TGeoVolume("Disc_Support_02", basedisk2);

  //vol->Draw();
  return vol  ;
}


//=============================================================================================
TGeoVolume* AliMFTSupport::Disc_Support_03 (){
  // define shape components with names + positions

  // ================= constants ===========================

  double rMin = 0,
         rMax = 25.5,
         thickness = .8/2,
         phi0=0.,
         phi1=180. ,
         t_delta=0.1,
         sup_box_dZ = 0.305/2;


  // =================  basic struture ===================


  // ================= Disk_Support_03 - base =============


  TGeoTubeSeg *base3 = new TGeoTubeSeg("d3base",rMin,rMax,thickness,phi0,phi1);


  // ================= Screw holes reliefs ====================


  //Screw support box 1
  Double_t ssbox1_dX=1.2/2, ssbox1_dY = 1.9/2;
  Double_t ssbox1_X=12.15, ssbox1_Y = 7.86, ssbox1_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox1 = new TGeoBBox("d3ssbox1",ssbox1_dX,ssbox1_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox1= new TGeoTranslation ("d3tr_ssbox1",ssbox1_X,ssbox1_Y,ssbox1_Z);
  tr_ssbox1-> RegisterYourself();

  //Screw support box 2
  Double_t ssbox2_dX=1.2/2, ssbox2_dY = 1.9/2;
  Double_t ssbox2_X=10.45, ssbox2_Y = 10.87, ssbox2_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox2 = new TGeoBBox("d3ssbox2",ssbox2_dX,ssbox2_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox2= new TGeoTranslation ("d3tr_ssbox2",ssbox2_X,ssbox2_Y,ssbox2_Z);
  tr_ssbox2-> RegisterYourself();

  //Screw support box 3
  Double_t ssbox3_dX=4.6/2, ssbox3_dY = 1.9/2;
  Double_t ssbox3_X=7.05, ssbox3_Y = 13.88, ssbox3_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox3 = new TGeoBBox("d3ssbox3",ssbox3_dX,ssbox3_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox3= new TGeoTranslation ("d3tr_ssbox3",ssbox3_X,ssbox3_Y,ssbox3_Z);
  tr_ssbox3-> RegisterYourself();

  //Screw support box 4
  Double_t ssbox4_dX=4.6/2, ssbox4_dY = 2.07/2;
  Double_t ssbox4_X=0.25, ssbox4_Y = 16.995, ssbox4_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox4 = new TGeoBBox("d3ssbox4",ssbox4_dX,ssbox4_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox4= new TGeoTranslation ("d3tr_ssbox4",ssbox4_X,ssbox4_Y,ssbox4_Z);
  tr_ssbox4-> RegisterYourself();

  //Screw support box 5
  Double_t ssbox5_dX=1.2/2, ssbox5_dY = 1.9/2;
  Double_t ssbox5_X=3.65, ssbox5_Y = 15.68, ssbox5_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox5 = new TGeoBBox("d3ssbox5",ssbox5_dX,ssbox5_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox5= new TGeoTranslation ("d3tr_ssbox5",ssbox5_X,ssbox5_Y,ssbox5_Z);
  tr_ssbox5-> RegisterYourself();

  //Screw support box6
  Double_t ssbox6_dX=1.2/2, ssbox6_dY = 1.9/2;
  Double_t ssbox6_X=-3.15, ssbox6_Y = 16.1371, ssbox6_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox6 = new TGeoBBox("d3ssbox6",ssbox6_dX,ssbox6_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox6= new TGeoTranslation ("d3tr_ssbox6",ssbox6_X,ssbox6_Y,ssbox6_Z);
  tr_ssbox6-> RegisterYourself();

  //Screw support box7
  Double_t ssbox7_dX=4.6/2, ssbox7_dY = 1.9/2;
  Double_t ssbox7_X=-6.55, ssbox7_Y = 13.88, ssbox7_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox7 = new TGeoBBox("d3ssbox7",ssbox7_dX,ssbox7_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox7= new TGeoTranslation ("d3tr_ssbox7",ssbox7_X,ssbox7_Y,ssbox7_Z);
  tr_ssbox7-> RegisterYourself();

  //Screw support box8
  Double_t ssbox8_dX=2.9/2, ssbox8_dY = 1.9/2;
  Double_t ssbox8_X=-10.8, ssbox8_Y = 10.87, ssbox8_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox8 = new TGeoBBox("d3ssbox8",ssbox8_dX,ssbox8_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox8= new TGeoTranslation ("d3tr_ssbox8",ssbox8_X,ssbox8_Y,ssbox8_Z);
  tr_ssbox8-> RegisterYourself();

  //Screw support box9
  Double_t ssbox9_dX=1.2/2, ssbox9_dY = 1.9/2;
  Double_t ssbox9_X=-13.35, ssbox9_Y = 7.86, ssbox9_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox9 = new TGeoBBox("d3ssbox9",ssbox9_dX,ssbox9_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox9= new TGeoTranslation ("d3tr_ssbox9",ssbox9_X,ssbox9_Y,ssbox9_Z);
  tr_ssbox9-> RegisterYourself();

  //Detail box 1
  Double_t detailbox1_dX=0.2/2, detailbox1_dY = 5.21/2, detail_box1_dZ = 0.6/2 ;
  Double_t detailbox1_X=15.8, detailbox1_Y = 4.305, detailbox1_Z= 0 ;
  TGeoBBox   *detailbox1 = new TGeoBBox("d3detailbox1",detailbox1_dX,detailbox1_dY,detail_box1_dZ);
  TGeoTranslation *tr_detailbox1= new TGeoTranslation ("d3tr_detailbox1",detailbox1_X,detailbox1_Y,detailbox1_Z);
  tr_detailbox1-> RegisterYourself();

  //Detail box 1b
  Double_t detailbox1b_dX=0.2/2, detailbox1b_dY = 5.21/2, detail_box1b_dZ = 0.6/2 ;
  Double_t detailbox1b_X=-15.8, detailbox1b_Y = 4.305, detailbox1b_Z= 0 ;
  TGeoBBox   *detailbox1b = new TGeoBBox("d3detailbox1b",detailbox1b_dX,detailbox1b_dY,detail_box1b_dZ);
  TGeoTranslation *tr_detailbox1b= new TGeoTranslation ("d3tr_detailbox1b",detailbox1b_X,detailbox1b_Y,detailbox1b_Z);
  tr_detailbox1b-> RegisterYourself();

  //Detail tube001
  TGeoTube *d3det001tube001 = new TGeoTube("d3det001tube001",0,0.3,0.2/2);
  TGeoRotation *rotd3det001tube001 = new TGeoRotation("d3rotdet001tube001",90.,90,0.);
  rotd3det001tube001->RegisterYourself();
 	TGeoCombiTrans *ctd3det001tube001 = new TGeoCombiTrans("ctd3det001tube001", detailbox1_X,1.7,0., rotd3det001tube001);
  ctd3det001tube001->RegisterYourself();

  TGeoCombiTrans *ctd3det001tube001b = new TGeoCombiTrans("ctd3det001tube001b", -detailbox1_X,1.7,0., rotd3det001tube001);
  ctd3det001tube001b->RegisterYourself();

  TGeoCompositeShape *ssboxes = new TGeoCompositeShape ("d3ssboxes",
  "d3ssbox1:d3tr_ssbox1 + d3ssbox2:d3tr_ssbox2 + d3ssbox3:d3tr_ssbox3 + d3ssbox4:d3tr_ssbox4 + d3ssbox5:d3tr_ssbox5 + d3ssbox6:d3tr_ssbox6 + d3ssbox7:d3tr_ssbox7 + d3ssbox8:d3tr_ssbox8 + d3ssbox9:d3tr_ssbox9 + d3detailbox1:d3tr_detailbox1");


  // Disk cuts

  TGeoBBox   *d3_cut1 = new TGeoBBox("d3_cut1",rMax+ t_delta, 1.4, thickness+ t_delta);
  TGeoBBox   *d3_cut2 = new TGeoBBox("d3_cut2",31.8/2, 13.82/2,thickness+ t_delta);
  TGeoBBox   *d3_cut3 = new TGeoBBox("d3_cut3",8.0/2, 10.0/2,thickness+ t_delta);

  TGeoTranslation *trd3_cut3  = new TGeoTranslation(21.5, 0., 0.);
  trd3_cut3->SetName("trd3_cut3"); trd3_cut3->RegisterYourself();
  TGeoTranslation *trd3_cut3b  = new TGeoTranslation(-21.5, 0., 0.);
  trd3_cut3b->SetName("trd3_cut3b"); trd3_cut3b->RegisterYourself();

  TGeoBBox   *d3_cut4 = new TGeoBBox("d3_cut4",23.1/2, 19.84/2,thickness+ t_delta);
  TGeoBBox   *d3_cut5 = new TGeoBBox("d3_cut5",18.7/2, 25.86/2,thickness+ t_delta);
  TGeoBBox   *d3_cut6 = new TGeoBBox("d3_cut6",8.5/2, 29.46/2,thickness+ t_delta);
  TGeoBBox   *d3_cut7 = new TGeoBBox("d3_cut7",5.1/2, 31.92/2,thickness+ t_delta);
  TGeoBBox   *d3_cut8 = new TGeoBBox("d3_cut8",19.8/2, 2.0/2,thickness+ t_delta);

  TGeoTranslation *trd3_cut8  = new TGeoTranslation(0, -24.5, 0.);
  trd3_cut8->SetName("trd3_cut8"); trd3_cut8->RegisterYourself();

  TGeoBBox   *d3_cut9 = new TGeoBBox("d3_cut9",3.0/2, 6.4/2,thickness+ t_delta);
  TGeoTranslation *trd3_cut9  = new TGeoTranslation(23.5, -9.70, 0.);
  trd3_cut9->SetName("trd3_cut9"); trd3_cut9->RegisterYourself();
  TGeoTranslation *trd3_cut9b  = new TGeoTranslation(-23.5, -9.70, 0.);
  trd3_cut9b->SetName("trd3_cut9b"); trd3_cut9b->RegisterYourself();


  //Reliefs
  //Screwholes Area #1
  Double_t AX=12.15, AY=7.41; Int_t N_holes = 1;
  TGeoTranslation *tr_d3holes1= new TGeoTranslation ("d3tr_holes1",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes1-> RegisterYourself();
  TGeoCompositeShape * d3holes1 = new TGeoCompositeShape();
  d3holes1 = screw_array(N_holes); d3holes1->SetName("d3holes1");

  //Screwholes Area #2
  AX=10.45; AY=10.42; N_holes = 1;
  TGeoTranslation *tr_d3holes2= new TGeoTranslation ("d3tr_holes2",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes2-> RegisterYourself();
  TGeoCompositeShape * d3holes2 = new TGeoCompositeShape();
  d3holes2 = screw_array(N_holes,-1.7); d3holes2->SetName("d3holes2");

  //Screwholes Area #3
  AX=8.75; AY=13.43; N_holes = 3;
  TGeoTranslation *tr_d3holes3= new TGeoTranslation ("d3tr_holes3",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes3-> RegisterYourself();
  TGeoCompositeShape * d3holes3 = new TGeoCompositeShape();
  d3holes3 = screw_array(N_holes,-1.7); d3holes3->SetName("d3holes3");

  //Screwholes Area #4 (3 pairs out of alligment)
  AX=1.95; AY=16.46; N_holes = 1;
  TGeoTranslation *tr_d3holes4a = new TGeoTranslation ("d3tr_holes4a",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes4a-> RegisterYourself();
  TGeoCompositeShape * d3holes4a = new TGeoCompositeShape();
  d3holes4a = screw_array(N_holes,-1.7); d3holes4a->SetName("d3holes4a");

  AX=0.25; AY=16.63; N_holes = 1;
  TGeoTranslation *tr_d3holes4b = new TGeoTranslation ("d3tr_holes4b",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes4b-> RegisterYourself();
  TGeoCompositeShape * d3holes4b = new TGeoCompositeShape();
  d3holes4b = screw_array(N_holes,-1.7); d3holes4b->SetName("d3holes4b");

  AX=-1.45; AY=16.57; N_holes = 1;
  TGeoTranslation *tr_d3holes4c = new TGeoTranslation ("d3tr_holes4c",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes4c-> RegisterYourself();
  TGeoCompositeShape * d3holes4c = new TGeoCompositeShape();
  d3holes4c = screw_array(N_holes,-1.7); d3holes4c->SetName("d3holes4c");

  //Screwholes Area #5
  AX=3.65; AY=15.23; N_holes = 1;
  TGeoTranslation *tr_d3holes5= new TGeoTranslation ("d3tr_holes5",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes5-> RegisterYourself();
  TGeoCompositeShape * d3holes5 = new TGeoCompositeShape();
  d3holes5 = screw_array(N_holes,-1.7); d3holes5->SetName("d3holes5");

  //Screwholes Area #6
  AX=-3.15; AY=15.69; N_holes = 1;
  TGeoTranslation *tr_d3holes6= new TGeoTranslation ("d3tr_holes6",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes6-> RegisterYourself();
  TGeoCompositeShape * d3holes6 = new TGeoCompositeShape();
  d3holes6 = screw_array(N_holes,-1.7); d3holes6->SetName("d3holes6");

  //Screwholes Area #7
  AX=-4.85, AY=13.43; N_holes = 3;
  TGeoTranslation *tr_d3holes7= new TGeoTranslation ("d3tr_holes7",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes7-> RegisterYourself();
  TGeoCompositeShape * d3holes7 = new TGeoCompositeShape();
  d3holes7 = screw_array(N_holes,-1.7); d3holes7->SetName("d3holes7");

  //Screwholes Area #8
  AX=-9.95, AY=10.42; N_holes = 2;
  TGeoTranslation *tr_d3holes8= new TGeoTranslation ("d3tr_holes8",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes8-> RegisterYourself();
  TGeoCompositeShape * d3holes8 = new TGeoCompositeShape();
  d3holes8 = screw_array(N_holes,-1.7); d3holes8->SetName("d3holes8");

  //Screwholes Area #9
  AX=-13.35, AY=7.41; N_holes = 1;
  TGeoTranslation *tr_d3holes9= new TGeoTranslation ("d3tr_holes9",AX,AY, 2*sup_box_dZ+thickness);
  tr_d3holes9-> RegisterYourself();
  TGeoCompositeShape * d3holes9 = new TGeoCompositeShape();
  d3holes9 = screw_array(N_holes); d3holes9->SetName("d3holes9");

  //Through Hole001 disk3 - Radius 3.25 mm
  AX=16.6, AY=2.0;
  TGeoTube *Th001 = new TGeoTube("d3Th001",0, 0.325, thickness+t_delta);
  TGeoTranslation * tr_Th001 = new TGeoTranslation("d3tr_Th001",AX,AY, 0.);
  tr_Th001->RegisterYourself();

  //Through Hole002 disk3 - Radius 3.0 mm
  AX=16.6, AY=3.0;
  TGeoTube *Th002 = new TGeoTube("d3Th002",0, 0.3, thickness+t_delta);
  TGeoTranslation * tr_Th002 = new TGeoTranslation("d3tr_Th002",AX,AY, 0.);
  tr_Th002->RegisterYourself();

  //Through Hole003 disk3 - Radius 1.2295 mm
  AX=16.0, AY=10.0;
  TGeoTube *Th003 = new TGeoTube("d3Th003",0, 0.12295, thickness+t_delta);
  TGeoTranslation * tr_Th003 = new TGeoTranslation("d3tr_Th003",AX,AY, 0.);
  tr_Th003->RegisterYourself();

  //Through Hole004 disk3 - Radius 1.5 mm
  AX=19.5, AY=10;
  TGeoTube *Th004 = new TGeoTube("d3Th004",0, 0.15, thickness+t_delta);
  TGeoTranslation * tr_Th004 = new TGeoTranslation("d3tr_Th004",AX,AY, 0.);
  tr_Th004->RegisterYourself();

  //Through Hole005 disk3 - Radius 1.2295 mm
  AX=11.0, AY=22.0;
  TGeoTube *Th005 = new TGeoTube("d3Th005",0, 0.12295, thickness+t_delta);
  TGeoTranslation * tr_Th005 = new TGeoTranslation("d3tr_Th005",AX,AY, 0.);
  tr_Th005->RegisterYourself();

  //Boolean operations

  //Combining all relief holes of one side
  TGeoCompositeShape *d3holes = new TGeoCompositeShape ("d3holes", "d3holes1:d3tr_holes1 + d3holes2:d3tr_holes2 + d3holes3:d3tr_holes3 + d3holes4a:d3tr_holes4a+ d3holes4b:d3tr_holes4b+ d3holes4c:d3tr_holes4c + d3holes5:d3tr_holes5 + d3holes6:d3tr_holes6 + d3holes7:d3tr_holes7 + d3holes8:d3tr_holes8 + d3holes9:d3tr_holes9 + d3Th001:d3tr_Th001 + d3Th002:d3tr_Th002 + d3Th003:d3tr_Th003 + d3Th004:d3tr_Th004 + d3Th005:d3tr_Th005");

  //Adding reliefs
  TGeoCompositeShape *d3base2 = new TGeoCompositeShape ("d3base2", "d3base+d3ssboxes+d3ssboxes:rotback");

  //Removing inner cuts and holes
  TGeoCompositeShape *d3base3 = new TGeoCompositeShape
  ("d3base3", "(d3base2-d3_cut1 - d3_cut2 - d3_cut3:trd3_cut3 - d3_cut3:trd3_cut3b - d3_cut4-d3_cut5 - d3_cut6 - d3_cut7-d3_cut8:trd3_cut8 - d3_cut9:trd3_cut9 - d3_cut9:trd3_cut9b - d3holes -d3holes:rotback)");

  //Adding inner detail
  // As this detail is thinner than the disk, it must be added after removing inner cuts
  TGeoCompositeShape *d3base4 = new TGeoCompositeShape ("d3base4", "d3base3 + d3detailbox1:d3tr_detailbox1 + d3detailbox1b:d3tr_detailbox1b + d3det001tube001:ctd3det001tube001 + d3det001tube001:ctd3det001tube001b");

  TGeoVolume *vol = new TGeoVolume("Support03", d3base4);

  return vol  ;
}

//=============================================================================================
TGeoVolume* AliMFTSupport::Disc_Support_04 (){
  // define shape components with names + positions

  // ================= constants ===========================

  double rMin = 0,
         rMax = 25.5,
         thickness = .8/2,
         phi0=0.,
         phi1=180. ,
         t_delta=0.1,
         sup_box_dZ = 0.305/2;


  // =================  basic struture ===================


  // ================= Disk_Support_04 - base =============


  TGeoTubeSeg *base4 = new TGeoTubeSeg("d4base",rMin,rMax,thickness,phi0,phi1);


  // ================= Screw holes reliefs ====================


  //Screw support box 1
  Double_t ssbox1_dX=1.2/2, ssbox1_dY = 1.9/2;
  Double_t ssbox1_X=12.15, ssbox1_Y = 7.86, ssbox1_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox1 = new TGeoBBox("d4ssbox1",ssbox1_dX,ssbox1_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox1= new TGeoTranslation ("d4tr_ssbox1",ssbox1_X,ssbox1_Y,ssbox1_Z);
  tr_ssbox1-> RegisterYourself();

  //Screw support box 2
  Double_t ssbox2_dX=1.2/2, ssbox2_dY = 1.9/2;
  Double_t ssbox2_X=10.45, ssbox2_Y = 10.87, ssbox2_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox2 = new TGeoBBox("d4ssbox2",ssbox2_dX,ssbox2_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox2= new TGeoTranslation ("d4tr_ssbox2",ssbox2_X,ssbox2_Y,ssbox2_Z);
  tr_ssbox2-> RegisterYourself();

  //Screw support box 3
  Double_t ssbox3_dX=4.6/2, ssbox3_dY = 1.9/2;
  Double_t ssbox3_X=7.05, ssbox3_Y = 13.88, ssbox3_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox3 = new TGeoBBox("d4ssbox3",ssbox3_dX,ssbox3_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox3= new TGeoTranslation ("d4tr_ssbox3",ssbox3_X,ssbox3_Y,ssbox3_Z);
  tr_ssbox3-> RegisterYourself();

  //Screw support box 4
  Double_t ssbox4_dX=4.6/2, ssbox4_dY = 2.07/2;
  Double_t ssbox4_X=0.25, ssbox4_Y = 16.995, ssbox4_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox4 = new TGeoBBox("d4ssbox4",ssbox4_dX,ssbox4_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox4= new TGeoTranslation ("d4tr_ssbox4",ssbox4_X,ssbox4_Y,ssbox4_Z);
  tr_ssbox4-> RegisterYourself();

  //Screw support box 5
  Double_t ssbox5_dX=1.2/2, ssbox5_dY = 1.9/2;
  Double_t ssbox5_X=3.65, ssbox5_Y = 15.68, ssbox5_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox5 = new TGeoBBox("d4ssbox5",ssbox5_dX,ssbox5_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox5= new TGeoTranslation ("d4tr_ssbox5",ssbox5_X,ssbox5_Y,ssbox5_Z);
  tr_ssbox5-> RegisterYourself();

  //Screw support box6
  Double_t ssbox6_dX=1.2/2, ssbox6_dY = 1.9/2;
  Double_t ssbox6_X=-3.15, ssbox6_Y = 16.1371, ssbox6_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox6 = new TGeoBBox("d4ssbox6",ssbox6_dX,ssbox6_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox6= new TGeoTranslation ("d4tr_ssbox6",ssbox6_X,ssbox6_Y,ssbox6_Z);
  tr_ssbox6-> RegisterYourself();

  //Screw support box7
  Double_t ssbox7_dX=4.6/2, ssbox7_dY = 1.9/2;
  Double_t ssbox7_X=-6.55, ssbox7_Y = 13.88, ssbox7_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox7 = new TGeoBBox("d4ssbox7",ssbox7_dX,ssbox7_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox7= new TGeoTranslation ("d4tr_ssbox7",ssbox7_X,ssbox7_Y,ssbox7_Z);
  tr_ssbox7-> RegisterYourself();

  //Screw support box8
  Double_t ssbox8_dX=2.9/2, ssbox8_dY = 1.9/2;
  Double_t ssbox8_X=-10.8, ssbox8_Y = 10.87, ssbox8_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox8 = new TGeoBBox("d4ssbox8",ssbox8_dX,ssbox8_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox8= new TGeoTranslation ("d4tr_ssbox8",ssbox8_X,ssbox8_Y,ssbox8_Z);
  tr_ssbox8-> RegisterYourself();

  //Screw support box9
  Double_t ssbox9_dX=1.2/2, ssbox9_dY = 1.9/2;
  Double_t ssbox9_X=-13.35, ssbox9_Y = 7.86, ssbox9_Z= thickness+sup_box_dZ;
  TGeoBBox   *ssbox9 = new TGeoBBox("d4ssbox9",ssbox9_dX,ssbox9_dY,sup_box_dZ);
  TGeoTranslation *tr_ssbox9= new TGeoTranslation ("d4tr_ssbox9",ssbox9_X,ssbox9_Y,ssbox9_Z);
  tr_ssbox9-> RegisterYourself();

  //Detail box 1
  Double_t detailbox1_dX=0.2/2, detailbox1_dY = 5.21/2, detail_box1_dZ = 0.6/2 ;
  Double_t detailbox1_X=15.8, detailbox1_Y = 4.305, detailbox1_Z= 0 ;
  TGeoBBox   *detailbox1 = new TGeoBBox("d4detailbox1",detailbox1_dX,detailbox1_dY,detail_box1_dZ);
  TGeoTranslation *tr_detailbox1= new TGeoTranslation ("d4tr_detailbox1",detailbox1_X,detailbox1_Y,detailbox1_Z);
  tr_detailbox1-> RegisterYourself();

  //Detail box 1b
  Double_t detailbox1b_dX=0.2/2, detailbox1b_dY = 5.21/2, detail_box1b_dZ = 0.6/2 ;
  Double_t detailbox1b_X=-15.8, detailbox1b_Y = 4.305, detailbox1b_Z= 0 ;
  TGeoBBox   *detailbox1b = new TGeoBBox("d4detailbox1b",detailbox1b_dX,detailbox1b_dY,detail_box1b_dZ);
  TGeoTranslation *tr_detailbox1b= new TGeoTranslation ("d4tr_detailbox1b",detailbox1b_X,detailbox1b_Y,detailbox1b_Z);
  tr_detailbox1b-> RegisterYourself();

  //Detail tube001
  TGeoTube *d4det001tube001 = new TGeoTube("d4det001tube001",0,0.3,0.2/2);
  TGeoRotation *rotd4det001tube001 = new TGeoRotation("d4rotdet001tube001",90.,90,0.);
  rotd4det001tube001->RegisterYourself();
  TGeoCombiTrans *ctd4det001tube001 = new TGeoCombiTrans("ctd4det001tube001", detailbox1_X,1.7,0., rotd4det001tube001);
  ctd4det001tube001->RegisterYourself();

  TGeoCombiTrans *ctd4det001tube001b = new TGeoCombiTrans("ctd4det001tube001b", -detailbox1_X,1.7,0., rotd4det001tube001);
  ctd4det001tube001b->RegisterYourself();

  TGeoCompositeShape *ssboxes = new TGeoCompositeShape ("d4ssboxes",
  "d4ssbox1:d4tr_ssbox1 + d4ssbox2:d4tr_ssbox2 + d4ssbox3:d4tr_ssbox3 + d4ssbox4:d4tr_ssbox4 + d4ssbox5:d4tr_ssbox5 + d4ssbox6:d4tr_ssbox6 + d4ssbox7:d4tr_ssbox7 + d4ssbox8:d4tr_ssbox8 + d4ssbox9:d4tr_ssbox9 + d4detailbox1:d4tr_detailbox1");


  // Disk cuts

  TGeoBBox   *d4_cut1 = new TGeoBBox("d4_cut1",rMax+ t_delta, 1.4, thickness+ t_delta);
  TGeoBBox   *d4_cut2 = new TGeoBBox("d4_cut2",31.8/2, 13.82/2,thickness+ t_delta);
  TGeoBBox   *d4_cut3 = new TGeoBBox("d4_cut3",8.0/2, 10.0/2,thickness+ t_delta);

  TGeoTranslation *trd4_cut3  = new TGeoTranslation(21.5, 0., 0.);
  trd4_cut3->SetName("trd4_cut3"); trd4_cut3->RegisterYourself();
  TGeoTranslation *trd4_cut3b  = new TGeoTranslation(-21.5, 0., 0.);
  trd4_cut3b->SetName("trd4_cut3b"); trd4_cut3b->RegisterYourself();

  TGeoBBox   *d4_cut4 = new TGeoBBox("d4_cut4",23.1/2, 19.84/2,thickness+ t_delta);
  TGeoBBox   *d4_cut5 = new TGeoBBox("d4_cut5",18.7/2, 25.86/2,thickness+ t_delta);
  TGeoBBox   *d4_cut6 = new TGeoBBox("d4_cut6",8.5/2, 29.46/2,thickness+ t_delta);
  TGeoBBox   *d4_cut7 = new TGeoBBox("d4_cut7",5.1/2, 31.92/2,thickness+ t_delta);
  TGeoBBox   *d4_cut8 = new TGeoBBox("d4_cut8",19.8/2, 2.0/2,thickness+ t_delta);

  TGeoTranslation *trd4_cut8  = new TGeoTranslation(0, -24.5, 0.);
  trd4_cut8->SetName("trd4_cut8"); trd4_cut8->RegisterYourself();

  TGeoBBox   *d4_cut9 = new TGeoBBox("d4_cut9",3.0/2, 6.4/2,thickness+ t_delta);
  TGeoTranslation *trd4_cut9  = new TGeoTranslation(23.5, -9.70, 0.);
  trd4_cut9->SetName("trd4_cut9"); trd4_cut9->RegisterYourself();
  TGeoTranslation *trd4_cut9b  = new TGeoTranslation(-23.5, -9.70, 0.);
  trd4_cut9b->SetName("trd4_cut9b"); trd4_cut9b->RegisterYourself();


  //Reliefs
  //Screwholes Area #1
  Double_t AX=12.15, AY=7.41; Int_t N_holes = 1;
  TGeoTranslation *tr_d4holes1= new TGeoTranslation ("d4tr_holes1",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes1-> RegisterYourself();
  TGeoCompositeShape * d4holes1 = new TGeoCompositeShape();
  d4holes1 = screw_array(N_holes); d4holes1->SetName("d4holes1");

  //Screwholes Area #2
  AX=10.45; AY=10.42; N_holes = 1;
  TGeoTranslation *tr_d4holes2= new TGeoTranslation ("d4tr_holes2",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes2-> RegisterYourself();
  TGeoCompositeShape * d4holes2 = new TGeoCompositeShape();
  d4holes2 = screw_array(N_holes,-1.7); d4holes2->SetName("d4holes2");

  //Screwholes Area #3
  AX=8.75; AY=13.43; N_holes = 3;
  TGeoTranslation *tr_d4holes3= new TGeoTranslation ("d4tr_holes3",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes3-> RegisterYourself();
  TGeoCompositeShape * d4holes3 = new TGeoCompositeShape();
  d4holes3 = screw_array(N_holes,-1.7); d4holes3->SetName("d4holes3");

  //Screwholes Area #4 (3 pairs out of alligment)
  AX=1.95; AY=16.46; N_holes = 1;
  TGeoTranslation *tr_d4holes4a = new TGeoTranslation ("d4tr_holes4a",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes4a-> RegisterYourself();
  TGeoCompositeShape * d4holes4a = new TGeoCompositeShape();
  d4holes4a = screw_array(N_holes,-1.7); d4holes4a->SetName("d4holes4a");

  AX=0.25; AY=16.63; N_holes = 1;
  TGeoTranslation *tr_d4holes4b = new TGeoTranslation ("d4tr_holes4b",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes4b-> RegisterYourself();
  TGeoCompositeShape * d4holes4b = new TGeoCompositeShape();
  d4holes4b = screw_array(N_holes,-1.7); d4holes4b->SetName("d4holes4b");

  AX=-1.45; AY=16.57; N_holes = 1;
  TGeoTranslation *tr_d4holes4c = new TGeoTranslation ("d4tr_holes4c",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes4c-> RegisterYourself();
  TGeoCompositeShape * d4holes4c = new TGeoCompositeShape();
  d4holes4c = screw_array(N_holes,-1.7); d4holes4c->SetName("d4holes4c");

  //Screwholes Area #5
  AX=3.65; AY=15.23; N_holes = 1;
  TGeoTranslation *tr_d4holes5= new TGeoTranslation ("d4tr_holes5",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes5-> RegisterYourself();
  TGeoCompositeShape * d4holes5 = new TGeoCompositeShape();
  d4holes5 = screw_array(N_holes,-1.7); d4holes5->SetName("d4holes5");

  //Screwholes Area #6
  AX=-3.15; AY=15.69; N_holes = 1;
  TGeoTranslation *tr_d4holes6= new TGeoTranslation ("d4tr_holes6",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes6-> RegisterYourself();
  TGeoCompositeShape * d4holes6 = new TGeoCompositeShape();
  d4holes6 = screw_array(N_holes,-1.7); d4holes6->SetName("d4holes6");

  //Screwholes Area #7
  AX=-4.85, AY=13.43; N_holes = 3;
  TGeoTranslation *tr_d4holes7= new TGeoTranslation ("d4tr_holes7",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes7-> RegisterYourself();
  TGeoCompositeShape * d4holes7 = new TGeoCompositeShape();
  d4holes7 = screw_array(N_holes,-1.7); d4holes7->SetName("d4holes7");

  //Screwholes Area #8
  AX=-9.95, AY=10.42; N_holes = 2;
  TGeoTranslation *tr_d4holes8= new TGeoTranslation ("d4tr_holes8",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes8-> RegisterYourself();
  TGeoCompositeShape * d4holes8 = new TGeoCompositeShape();
  d4holes8 = screw_array(N_holes,-1.7); d4holes8->SetName("d4holes8");

  //Screwholes Area #9
  AX=-13.35, AY=7.41; N_holes = 1;
  TGeoTranslation *tr_d4holes9= new TGeoTranslation ("d4tr_holes9",AX,AY, 2*sup_box_dZ+thickness);
  tr_d4holes9-> RegisterYourself();
  TGeoCompositeShape * d4holes9 = new TGeoCompositeShape();
  d4holes9 = screw_array(N_holes); d4holes9->SetName("d4holes9");

  //Through Hole001 disk3 - Radius 3.25 mm
  AX=16.6, AY=2.0;
  TGeoTube *Th001 = new TGeoTube("d4Th001",0, 0.325, thickness+t_delta);
  TGeoTranslation * tr_Th001 = new TGeoTranslation("d4tr_Th001",AX,AY, 0.);
  tr_Th001->RegisterYourself();

  //Through Hole002 disk3 - Radius 3.0 mm
  AX=16.6, AY=3.0;
  TGeoTube *Th002 = new TGeoTube("d4Th002",0, 0.3, thickness+t_delta);
  TGeoTranslation * tr_Th002 = new TGeoTranslation("d4tr_Th002",AX,AY, 0.);
  tr_Th002->RegisterYourself();

  //Through Hole003 disk3 - Radius 1.2295 mm
  AX=16.0, AY=10.0;
  TGeoTube *Th003 = new TGeoTube("d4Th003",0, 0.12295, thickness+t_delta);
  TGeoTranslation * tr_Th003 = new TGeoTranslation("d4tr_Th003",AX,AY, 0.);
  tr_Th003->RegisterYourself();

  //Through Hole004 disk3 - Radius 1.5 mm
  AX=19.5, AY=10;
  TGeoTube *Th004 = new TGeoTube("d4Th004",0, 0.15, thickness+t_delta);
  TGeoTranslation * tr_Th004 = new TGeoTranslation("d4tr_Th004",AX,AY, 0.);
  tr_Th004->RegisterYourself();

  //Through Hole005 disk3 - Radius 1.2295 mm
  AX=11.0, AY=22.0;
  TGeoTube *Th005 = new TGeoTube("d4Th005",0, 0.12295, thickness+t_delta);
  TGeoTranslation * tr_Th005 = new TGeoTranslation("d4tr_Th005",AX,AY, 0.);
  tr_Th005->RegisterYourself();

  //Boolean operations

  //Combining all relief holes of one side
  TGeoCompositeShape *d4holes = new TGeoCompositeShape ("d4holes", "d4holes1:d4tr_holes1 + d4holes2:d4tr_holes2 + d4holes3:d4tr_holes3 + d4holes4a:d4tr_holes4a+ d4holes4b:d4tr_holes4b+ d4holes4c:d4tr_holes4c + d4holes5:d4tr_holes5 + d4holes6:d4tr_holes6 + d4holes7:d4tr_holes7 + d4holes8:d4tr_holes8 + d4holes9:d4tr_holes9 + d4Th001:d4tr_Th001 + d4Th002:d4tr_Th002 + d4Th003:d4tr_Th003 + d4Th004:d4tr_Th004 + d4Th005:d4tr_Th005");

  //Adding reliefs
  TGeoCompositeShape *d4base2 = new TGeoCompositeShape ("d4base2", "d4base+d4ssboxes+d4ssboxes:rotback");

  //Removing inner cuts and holes
  TGeoCompositeShape *d4base3 = new TGeoCompositeShape
  ("d4base3", "(d4base2-d4_cut1 - d4_cut2 - d4_cut3:trd4_cut3 - d4_cut3:trd4_cut3b - d4_cut4-d4_cut5 - d4_cut6 - d4_cut7-d4_cut8:trd4_cut8 - d4_cut9:trd4_cut9 - d4_cut9:trd4_cut9b - d4holes -d4holes:rotback)");

  TGeoVolume *vol = new TGeoVolume("Support04", d4base3);

  return vol  ;
}

//=============================================================================================
TGeoCompositeShape * AliMFTSupport::screw_array (Int_t N, Double_t gap){
  //Function that creates the holes for the MFT PCB Support
  // Return an array of pairs corresponding to screws A and B.

  //TGeoCompositeShape *base_pair;
TGeoCompositeShape *screw_pairs[4];

Double_t a_cone1_dz= .05/2,
a_cone1_r_top = .2567/2,
a_h_radius = .156657/2,
a_tube1_dz = .55/2,
a_cone2_dz = .0452354/2;

Double_t b_cone1_dz= .05/2,
b_cone1_r_top = .30/2,
b_h_radius = .20/2,
b_tube1_dz = .35/2,
b_cone2_dz = 0.057735/2;

Double_t t_excess = 0.1,
       screw_distance=+0.8;

// Making ScrewholeA

 TGeoCone *sc_cone1_a = new TGeoCone(Form("sc_cone1_a"),a_cone1_dz+t_excess, 0, a_h_radius, 0, a_cone1_r_top);
 TGeoTranslation *tr_sc_cone1_a= new TGeoTranslation ("tr_sc_cone1_a",0., 0., -a_cone1_dz);
 tr_sc_cone1_a-> RegisterYourself();

 TGeoTube *sc_tube1_a = new TGeoTube("sc_tube1_a",0, a_h_radius, a_tube1_dz+2*t_excess);
 TGeoTranslation *tr_sc_tube1_a= new TGeoTranslation ("tr_sc_tube1_a",0., 0., -a_tube1_dz-2*a_cone1_dz);
 tr_sc_tube1_a-> RegisterYourself();

 TGeoCone *sc_cone2_a = new TGeoCone("sc_cone2_a",a_cone2_dz+t_excess, 0, 0, 0, a_h_radius);
 TGeoTranslation *tr_sc_cone2_a= new TGeoTranslation ("tr_sc_cone2_a",0., 0., -a_cone2_dz-2*a_tube1_dz-2*a_cone1_dz);
 tr_sc_cone2_a-> RegisterYourself();

 //TGeoTube *sc_excess_a = new TGeoTube("sc_excess_a",0, a_cone1_r_top, t_excess);
 //TGeoTranslation *tr_sc_excess_a= new TGeoTranslation ("tr_sc_excess_a",0., 0., t_excess);
 //tr_sc_excess_a-> RegisterYourself();

 //Creating composition for ScrewholeA

 TGeoCompositeShape *screwa = new TGeoCompositeShape
 ("sca", "(sc_cone1_a:tr_sc_cone1_a+sc_tube1_a:tr_sc_tube1_a+sc_cone2_a:tr_sc_cone2_a)");


 // Making ScrewholeB

 TGeoCone *sc_cone1_b = new TGeoCone(Form("sc_cone1_b"),b_cone1_dz+t_excess, 0, b_h_radius, 0, b_cone1_r_top);
 TGeoTranslation *tr_sc_cone1_b= new TGeoTranslation ("tr_sc_cone1_b",0., 0., -b_cone1_dz);
 tr_sc_cone1_b-> RegisterYourself();

 TGeoTube *sc_tube1_b = new TGeoTube("sc_tube1_b",0, b_h_radius, b_tube1_dz+2*t_excess);
 TGeoTranslation *tr_sc_tube1_b= new TGeoTranslation ("tr_sc_tube1_b",0., 0., -b_tube1_dz-2*b_cone1_dz+t_excess);
 tr_sc_tube1_b-> RegisterYourself();

 TGeoCone *sc_cone2_b = new TGeoCone("sc_cone2_b",b_cone2_dz+t_excess, 0, 0, 0, b_h_radius);
 TGeoTranslation *tr_sc_cone2_b= new TGeoTranslation ("tr_sc_cone2_b",0., 0., -b_cone2_dz-2*b_tube1_dz-2*b_cone1_dz);
 tr_sc_cone2_b-> RegisterYourself();

 //TGeoTube *sc_excess_b = new TGeoTube("sc_excess_b",0, b_cone1_r_top, t_excess);
 //TGeoTranslation *tr_sc_excess_b= new TGeoTranslation ("tr_sc_excess_b",0., 0., t_excess);
 //tr_sc_excess_b-> RegisterYourself();



 //Translations for building hole array
 TGeoTranslation *tr0= new TGeoTranslation ("tr0",0.,0.,0.);
 tr0->RegisterYourself();

 //Distance between screws a and b
 TGeoTranslation *tr_pair = new TGeoTranslation("tr_pair",0,screw_distance,0.);
 tr_pair->RegisterYourself();

 //Creating screwhole b at origin
 TGeoCompositeShape *screwb = new TGeoCompositeShape
 ("scb", "( (sc_cone1_b:tr_sc_cone1_b+sc_tube1_b:tr_sc_tube1_b+sc_cone2_b:tr_sc_cone2_b))");

 //Base pair at origin:
 TGeoCompositeShape *base_pair = new TGeoCompositeShape("base_pair","sca:tr_pair+scb");
 //base_pair->Draw();

    //Screw pairs at final position:
    TGeoTranslation *tr_pairx = new TGeoTranslation("tr_pairx",0,screw_distance,0.);
    tr_pairx->RegisterYourself();
    screw_pairs[0] = new TGeoCompositeShape(Form("Pair_%d",0),"sca:tr_pair+scb");
    if(N==1) return screw_pairs[0];

     //Creating array
     //Column 1

     Int_t id=1;
     TGeoTranslation *tr_row1 = new TGeoTranslation("tr_row1",id*gap,0.,0.);
     tr_row1->RegisterYourself();
     TGeoUnion *Pairs_union = new TGeoUnion(screw_pairs[id-1], base_pair, tr0, tr_row1);
     screw_pairs[id] = new TGeoCompositeShape(Form("Pair_%d",id),Pairs_union);
     if(N==2) return screw_pairs[id];


     //Column 2
     id++;
     TGeoTranslation *tr_row2 = new TGeoTranslation("tr_row2",id*gap,0.,0.);
     tr_row2->RegisterYourself();
     TGeoUnion *Pairs_union2 = new TGeoUnion(screw_pairs[id-1], base_pair, tr0, tr_row2);
     screw_pairs[id] = new TGeoCompositeShape(Form("Pair_%d",id),Pairs_union2);
     //screw_pairs[id]->Draw();
     if(N==3) return screw_pairs[id];

     return screw_pairs[0];
}


//=============================================================================================
TGeoCompositeShape * AliMFTSupport::screw_C (){

  Double_t cone1_dz= .05/2,
   cone1_r_top = .7/2,
   h_radius = .6/2,
   tube1_dz = .75/2,
   cone2_dz = .173205/2;

   Double_t t_excess = 0.1;


 // Making ScrewholeC

    TGeoCone *sc_cone1_c = new TGeoCone(Form("sc_cone1_c"),cone1_dz, 0, h_radius, 0, cone1_r_top);
    TGeoTranslation *tr_sc_cone1_c= new TGeoTranslation ("tr_sc_cone1_c",0., 0., -cone1_dz);
    tr_sc_cone1_c-> RegisterYourself();

    TGeoTube *sc_tube1_c = new TGeoTube("sc_tube1_c",0, h_radius, tube1_dz);
    TGeoTranslation *tr_sc_tube1_c= new TGeoTranslation ("tr_sc_tube1_c",0., 0., -tube1_dz-2*cone1_dz+t_excess);
    tr_sc_tube1_c-> RegisterYourself();

    TGeoCone *sc_cone2_c = new TGeoCone("sc_cone2_c",cone2_dz, 0, 0, 0, h_radius);
    TGeoTranslation *tr_sc_cone2_c= new TGeoTranslation ("tr_sc_cone2_c",0., 0., -cone2_dz-2*tube1_dz-2*cone1_dz+t_excess);
    tr_sc_cone2_c-> RegisterYourself();

    TGeoTube *sc_excess_c = new TGeoTube("sc_excess_c",0, cone1_r_top, t_excess);
    TGeoTranslation *tr_sc_excess_c= new TGeoTranslation ("tr_sc_excess_c",0., 0., t_excess);
    tr_sc_excess_c-> RegisterYourself();

    //Creating composition for ScrewholeC

    TGeoCompositeShape *screwc = new TGeoCompositeShape
    ("scc", "(sc_cone1_c:tr_sc_cone1_c+sc_tube1_c:tr_sc_tube1_c+sc_cone2_c:tr_sc_cone2_c+sc_excess_c:tr_sc_excess_c)");

    //screwc->Draw();

    return screwc;

}

//=============================================================================================
TGeoCompositeShape * AliMFTSupport::screw_D (){
  Double_t h_radius = .15,
  tube1_dz = .6/2,
  cone2_dz = .0866024/2;

  Double_t t_excess = 0.1;


// Making ScrewholeD

   TGeoTube *sc_tube1_d = new TGeoTube("sc_tube1_d",0, h_radius, tube1_dz);
   TGeoTranslation *tr_sc_tube1_d= new TGeoTranslation ("tr_sc_tube1_d",0., 0., -tube1_dz+t_excess);
   tr_sc_tube1_d-> RegisterYourself();

   TGeoCone *sc_cone2_d = new TGeoCone("sc_cone2_d",cone2_dz, 0, 0, 0, h_radius);
   TGeoTranslation *tr_sc_cone2_d= new TGeoTranslation ("tr_sc_cone2_d",0., 0., -cone2_dz-2*tube1_dz+t_excess);
   tr_sc_cone2_d-> RegisterYourself();

   TGeoTube *sc_excess_d = new TGeoTube("sc_excess_d",0, h_radius, t_excess);
   TGeoTranslation *tr_sc_excess_d= new TGeoTranslation ("tr_sc_excess_d",0., 0., t_excess);
   tr_sc_excess_d-> RegisterYourself();

   //Creating composition for ScrewholeD

   TGeoCompositeShape *screwd = new TGeoCompositeShape
   ("scd", "(sc_tube1_d:tr_sc_tube1_d+sc_cone2_d:tr_sc_cone2_d+sc_excess_d:tr_sc_excess_d)");

   //screwd->Draw();

   return screwd;

}

//=============================================================================================
TGeoCompositeShape * AliMFTSupport::screw_E (){

  Double_t h_radius = .1621,
   tube1_dz = 1.0/2,
   cone2_dz = .0935884/2;

   Double_t t_excess = 0.1;


 // Making ScrewholeE

    TGeoTube *sc_tube1_e = new TGeoTube("sc_tube1_e",0, h_radius, tube1_dz);
    TGeoTranslation *tr_sc_tube1_e= new TGeoTranslation ("tr_sc_tube1_e",0., 0., -tube1_dz+t_excess);
    tr_sc_tube1_e-> RegisterYourself();

    TGeoCone *sc_cone2_e = new TGeoCone("sc_cone2_e",cone2_dz, 0, 0, 0, h_radius);
    TGeoTranslation *tr_sc_cone2_e= new TGeoTranslation ("tr_sc_cone2_e",0., 0., -cone2_dz-2*tube1_dz+t_excess);
    tr_sc_cone2_e-> RegisterYourself();

    TGeoTube *sc_excess_e = new TGeoTube("sc_excess_e",0, h_radius, t_excess);
    TGeoTranslation *tr_sc_excess_e= new TGeoTranslation ("tr_sc_excess_e",0., 0., t_excess);
    tr_sc_excess_e-> RegisterYourself();

    //Creating composition for ScrewholeE

    TGeoCompositeShape *screwe = new TGeoCompositeShape
    ("sce", "(sc_tube1_e:tr_sc_tube1_e+sc_cone2_e:tr_sc_cone2_e+sc_excess_e:tr_sc_excess_e)");

    //screwe->Draw();

    return screwe;

}


//=============================================================================================
TGeoCompositeShape * AliMFTSupport::through_hole_a (Double_t thickness){
  Double_t cone1_dz= .05/2,
   cone1_r_top = .375,
   h_radius = .325,
   tube1_dz = thickness/2.-2*cone1_dz,
   cone2_dz = cone1_dz;

   Double_t t_excess = 0.1;


    TGeoCone *sc_cone1_c = new TGeoCone(Form("sc_cone1_tha"),cone1_dz, 0, h_radius, 0, cone1_r_top);
    TGeoTranslation *tr_sc_cone1_c= new TGeoTranslation ("tr_sc_cone1_tha",0., 0., +thickness/2-cone1_dz);
    tr_sc_cone1_c-> RegisterYourself();

    TGeoTube *sc_tube1_c = new TGeoTube("sc_tube1_tha",0, h_radius, tube1_dz+ t_excess);

    TGeoCone *sc_cone2_c = new TGeoCone("sc_cone2_tha",cone2_dz, 0, cone1_r_top, 0, h_radius);
    TGeoTranslation *tr_sc_cone2_c= new TGeoTranslation ("tr_sc_cone2_tha",0., 0., -thickness/2+cone1_dz);
    tr_sc_cone2_c-> RegisterYourself();

    TGeoTube *sc_excess_c = new TGeoTube("sc_excess_tha",0, cone1_r_top, t_excess);
    TGeoTranslation *tr_sc_excess_c= new TGeoTranslation ("tr_sc_excess_tha",0., 0., t_excess+thickness/2);
    tr_sc_excess_c-> RegisterYourself();

    TGeoTranslation *tr_sc_excess_cb= new TGeoTranslation ("tr_sc_excess_thab",0., 0., -t_excess-thickness/2);
    tr_sc_excess_cb-> RegisterYourself();

    TGeoCompositeShape *through_a = new TGeoCompositeShape
    ("through_a", "(sc_cone1_tha:tr_sc_cone1_tha+sc_tube1_tha+sc_cone2_tha:tr_sc_cone2_tha+sc_excess_tha:tr_sc_excess_tha+sc_excess_tha:tr_sc_excess_thab)");

    //through_a->Draw();

    return through_a;

}


//=============================================================================================
TGeoCompositeShape * AliMFTSupport::through_hole_b (Double_t thickness){
  Double_t cone1_dz= .05/2,
   cone1_r_top = .35,
   h_radius = .30,
   tube1_dz = thickness/2.-2*cone1_dz,
   cone2_dz = cone1_dz;

   Double_t t_excess = 0.1;


    TGeoCone *sc_cone1_c = new TGeoCone(Form("sc_cone1_thb"),cone1_dz, 0, h_radius, 0, cone1_r_top);
    TGeoTranslation *tr_sc_cone1_c= new TGeoTranslation ("tr_sc_cone1_thb",0., 0., +thickness/2-cone1_dz);
    tr_sc_cone1_c-> RegisterYourself();

    TGeoTube *sc_tube1_c = new TGeoTube("sc_tube1_thb",0, h_radius, tube1_dz+t_excess);

    TGeoCone *sc_cone2_c = new TGeoCone("sc_cone2_thb",cone2_dz, 0, cone1_r_top, 0, h_radius);
    TGeoTranslation *tr_sc_cone2_c= new TGeoTranslation ("tr_sc_cone2_thb",0., 0., -thickness/2+cone1_dz);
    tr_sc_cone2_c-> RegisterYourself();

    TGeoTube *sc_excess_c = new TGeoTube("sc_excess_thb",0, cone1_r_top, t_excess);
    TGeoTranslation *tr_sc_excess_c= new TGeoTranslation ("tr_sc_excess_thb",0., 0., t_excess+thickness/2);
    tr_sc_excess_c-> RegisterYourself();

    TGeoTranslation *tr_sc_excess_cb= new TGeoTranslation ("tr_sc_excess_thbb",0., 0., -t_excess-thickness/2);
    tr_sc_excess_cb-> RegisterYourself();

    //Creating composition for ScrewholeC

    TGeoCompositeShape *through_b = new TGeoCompositeShape
    ("through_b", "(sc_cone1_thb:tr_sc_cone1_thb+sc_tube1_thb+sc_cone2_thb:tr_sc_cone2_thb+sc_excess_thb:tr_sc_excess_thb+sc_excess_thb:tr_sc_excess_thbb)");

   //through_b->Draw();

    return through_b;

}


//=============================================================================================
TGeoCompositeShape * AliMFTSupport::through_hole_c (Double_t thickness){
  Double_t cone1_dz= .05/2,
  cone1_r_top = .29585,
  h_radius = .24585,
  tube1_dz = thickness/2.-2*cone1_dz,
  cone2_dz = cone1_dz;

  Double_t t_excess = 0.1;


   TGeoCone *sc_cone1_c = new TGeoCone(Form("sc_cone1_thc"),cone1_dz, 0, h_radius, 0, cone1_r_top);
   TGeoTranslation *tr_sc_cone1_c= new TGeoTranslation ("tr_sc_cone1_thc",0., 0., +thickness/2-cone1_dz);
   tr_sc_cone1_c-> RegisterYourself();

   TGeoTube *sc_tube1_c = new TGeoTube("sc_tube1_thc",0, h_radius, tube1_dz+t_excess);

   TGeoCone *sc_cone2_c = new TGeoCone("sc_cone2_thc",cone2_dz, 0, cone1_r_top, 0, h_radius);
   TGeoTranslation *tr_sc_cone2_c= new TGeoTranslation ("tr_sc_cone2_thc",0., 0., -thickness/2+cone1_dz);
   tr_sc_cone2_c-> RegisterYourself();

   TGeoTube *sc_excess_c = new TGeoTube("sc_excess_thc",0, cone1_r_top, t_excess);
   TGeoTranslation *tr_sc_excess_c= new TGeoTranslation ("tr_sc_excess_thc",0., 0., t_excess+thickness/2);
   tr_sc_excess_c-> RegisterYourself();

   TGeoTranslation *tr_sc_excess_cb= new TGeoTranslation ("tr_sc_excess_thcb",0., 0., -t_excess-thickness/2);
   tr_sc_excess_cb-> RegisterYourself();

   //Creating composition for ScrewholeC

   TGeoCompositeShape *through_c = new TGeoCompositeShape
   ("through_c", "(sc_cone1_thc:tr_sc_cone1_thc+sc_tube1_thc+sc_cone2_thc:tr_sc_cone2_thc+sc_excess_thc:tr_sc_excess_thc+sc_excess_thc:tr_sc_excess_thcb)");

   //through_c->Draw();

   return through_c;

}


//=============================================================================================
TGeoCompositeShape * AliMFTSupport::through_hole_d (Double_t thickness){
  Double_t cone1_dz= .05/2,
  cone1_r_top = .2,
  h_radius = .15,
  tube1_dz = thickness/2.-2*cone1_dz,
  cone2_dz = cone1_dz;

  Double_t t_excess = 0.1;


   TGeoCone *sc_cone1_c = new TGeoCone(Form("sc_cone1_thd"),cone1_dz, 0, h_radius, 0, cone1_r_top);
   TGeoTranslation *tr_sc_cone1_c= new TGeoTranslation ("tr_sc_cone1_thd",0., 0., +thickness/2-cone1_dz);
   tr_sc_cone1_c-> RegisterYourself();

   TGeoTube *sc_tube1_c = new TGeoTube("sc_tube1_thd",0, h_radius, tube1_dz);

   TGeoCone *sc_cone2_c = new TGeoCone("sc_cone2_thd",cone2_dz, 0, cone1_r_top, 0, h_radius);
   TGeoTranslation *tr_sc_cone2_c= new TGeoTranslation ("tr_sc_cone2_thd",0., 0., -thickness/2+cone1_dz);
   tr_sc_cone2_c-> RegisterYourself();

   TGeoTube *sc_excess_c = new TGeoTube("sc_excess_thd",0, cone1_r_top, t_excess);
   TGeoTranslation *tr_sc_excess_c= new TGeoTranslation ("tr_sc_excess_thd",0., 0., t_excess+thickness/2);
   tr_sc_excess_c-> RegisterYourself();

   TGeoTranslation *tr_sc_excess_cb= new TGeoTranslation ("tr_sc_excess_thdb",0., 0., -t_excess-thickness/2);
   tr_sc_excess_cb-> RegisterYourself();

   //Creating composition for ScrewholeC

   TGeoCompositeShape *through_d = new TGeoCompositeShape
   ("through_d", "(sc_cone1_thd:tr_sc_cone1_thd+sc_tube1_thd+sc_cone2_thd:tr_sc_cone2_thd+sc_excess_thd:tr_sc_excess_thd+sc_excess_thd:tr_sc_excess_thdb)");

   //through_d->Draw();

   return through_d;

}


//=============================================================================================
TGeoCompositeShape * AliMFTSupport::through_hole_e (Double_t thickness){
  Double_t cone1_dz= .05/2,
   cone1_r_top = .17295,
   h_radius = .12295,
   tube1_dz = thickness/2.-2*cone1_dz,
   cone2_dz = cone1_dz;

   Double_t t_excess = 0.1;


    TGeoCone *sc_cone1_c = new TGeoCone(Form("sc_cone1_the"),cone1_dz, 0, h_radius, 0, cone1_r_top);
    TGeoTranslation *tr_sc_cone1_c= new TGeoTranslation ("tr_sc_cone1_the",0., 0., +thickness/2-cone1_dz);
    tr_sc_cone1_c-> RegisterYourself();

    TGeoTube *sc_tube1_c = new TGeoTube("sc_tube1_the",0, h_radius, tube1_dz);

    TGeoCone *sc_cone2_c = new TGeoCone("sc_cone2_the",cone2_dz, 0, cone1_r_top, 0, h_radius);
    TGeoTranslation *tr_sc_cone2_c= new TGeoTranslation ("tr_sc_cone2_the",0., 0., -thickness/2+cone1_dz);
    tr_sc_cone2_c-> RegisterYourself();

    TGeoTube *sc_excess_c = new TGeoTube("sc_excess_the",0, cone1_r_top, t_excess);
    TGeoTranslation *tr_sc_excess_c= new TGeoTranslation ("tr_sc_excess_the",0., 0., t_excess+thickness/2);
    tr_sc_excess_c-> RegisterYourself();

    TGeoTranslation *tr_sc_excess_cb= new TGeoTranslation ("tr_sc_excess_theb",0., 0., -t_excess-thickness/2);
    tr_sc_excess_cb-> RegisterYourself();

    //Creating composition for ScrewholeC

    TGeoCompositeShape *through_e = new TGeoCompositeShape
    ("through_e", "(sc_cone1_the:tr_sc_cone1_the+sc_tube1_the+sc_cone2_the:tr_sc_cone2_the+sc_excess_the:tr_sc_excess_the+sc_excess_the:tr_sc_excess_theb)");

    //through_e->Draw();

    return through_e;

}
