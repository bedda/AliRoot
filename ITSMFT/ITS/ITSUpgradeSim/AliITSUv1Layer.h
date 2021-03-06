#ifndef ALIITSUV1LAYER_H
#define ALIITSUV1LAYER_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */


//*************************************************************************
// This class Defines the Geometry for the ITS Upgrade using TGeo
// This is a work class used to study different configurations
// during the development of the new ITS structure.
//
//  Mario Sitta <sitta@to.infn.it>
//*************************************************************************


/*
  $Id: AliITSUv1Layer.h
 */

#include "AliITSUv1.h"
#include <TGeoManager.h>
#include <TGeoCompositeShape.h>
#include <TGeoXtru.h>

class TGeoVolume;

class AliITSUv1Layer : public TObject {
  public:
  enum {kStave,kHalfStave,kModule,kChip,kNHLevels};

  public:
    AliITSUv1Layer();
    AliITSUv1Layer(Int_t debug);
    AliITSUv1Layer(Int_t lay, Int_t debug);
    AliITSUv1Layer(Int_t lay, Bool_t turbo, Int_t debug);
    AliITSUv1Layer(const AliITSUv1Layer &source);
    AliITSUv1Layer& operator=(const AliITSUv1Layer &source);
    virtual ~AliITSUv1Layer();
    //
    Bool_t    IsTurbo() const {return fIsTurbo;};

    Double_t  GetChipThick()   const {return fChipThick;};
    Double_t  GetStaveTilt()   const {return fStaveTilt;};
    Double_t  GetStaveWidth()  const {return fStaveWidth;};
    Double_t  GetSensorThick() const {return fSensorThick;};
    Double_t  GetNStaves()     const {return fNStaves;};
    Double_t  GetNChips()      const {return fNChips;};
    Double_t  GetRadius()      const {return fLayRadius;};
    Double_t  GetPhi0()        const {return fPhi0;};
    Double_t  GetZLength()     const {return fZLength;};
    Int_t     GetChipType()    const {return fChipTypeID;}
    //
    Int_t     GetNStavesPerParent()     const {return fHierarchy[kStave];}
    Int_t     GetNHalfStavesPerParent() const {return fHierarchy[kHalfStave];}
    Int_t     GetNModulesPerParent()    const {return fHierarchy[kModule];}
    Int_t     GetNChipsPerParent()      const {return fHierarchy[kChip];}
    //
    Int_t     GetBuildLevel()           const {return fBuildLevel;}
    AliITSUv1::AliITSUModel_t GetStaveModel() const {return fStaveModel;}
    //
    void      SetChipThick(Double_t t)       {fChipThick = t;};
    void      SetStaveTilt(Double_t t);
    void      SetStaveWidth(Double_t w);
    void      SetSensorThick(Double_t t)     {fSensorThick = t;};
    void      SetNStaves(Int_t n)            {fHierarchy[kStave] = fNStaves = n;};
    void      SetNUnits(Int_t u);
    void      SetRadius(Double_t r)          {fLayRadius = r;};
    void      SetPhi0(Double_t phi)          {fPhi0 = phi;}
    void      SetZLength(Double_t z)         {fZLength   = z;};
    void      SetChipType(Int_t tp)          {fChipTypeID = tp;}
    void      SetBuildLevel(Int_t buildLevel){fBuildLevel=buildLevel;}
    void      SetStaveModel(AliITSUv1::AliITSUModel_t model) {fStaveModel=model;}
    virtual void CreateLayer(TGeoVolume *moth);

    // Define Trig functions for use with degrees (standerd TGeo angles).
    // Sine function
    Double_t SinD(Double_t deg)const{return TMath::Sin(deg*TMath::DegToRad());}
    // Cosine function
    Double_t CosD(Double_t deg)const{return TMath::Cos(deg*TMath::DegToRad());}
    // Tangent function
    Double_t TanD(Double_t deg)const{return TMath::Tan(deg*TMath::DegToRad());}

  protected:
    // Units, Convert from k?? to cm,degree,GeV,seconds,
    static const Double_t fgkmicron; // Convert micron to TGeom's cm.
    static const Double_t fgkmm; // Convert mm to TGeom's cm.
    static const Double_t fgkcm; // Convert cm to TGeom's cm.

  private:
    void CreateLayerTurbo(TGeoVolume *moth);

    TGeoVolume* CreateStave(const TGeoManager *mgr=gGeoManager);
    //TGeoVolume* CreateChip(Double_t x, Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateModuleInnerB(Double_t x,Double_t y, Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateChipInnerB(Double_t x,Double_t y, Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateModuleOuterB(const TGeoManager *mgr=gGeoManager);


    TGeoVolume* CreateStaveInnerB(Double_t x, Double_t y, Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveStructInnerB(Double_t x,Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveModelInnerBDummy(Double_t x,Double_t z, const TGeoManager *mgr=gGeoManager) const;
    TGeoVolume* CreateStaveModelInnerB0(Double_t x,Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveModelInnerB1(Double_t x,Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveModelInnerB21(Double_t x,Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveModelInnerB22(Double_t x,Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveModelInnerB3(Double_t x,Double_t z, const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveModelInnerB4(Double_t x,Double_t z, const TGeoManager *mgr=gGeoManager);

    TGeoVolume* CreateStaveOuterB(const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveModelOuterBDummy(const TGeoManager *mgr=gGeoManager) const;
    TGeoVolume* CreateStaveModelOuterB0(const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateStaveModelOuterB12(const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateSpaceFrameOuterB(const TGeoManager *mgr=gGeoManager);
    TGeoVolume* CreateSpaceFrameOuterBDummy(const TGeoManager *mgr=gGeoManager) const;
    TGeoVolume* CreateSpaceFrameOuterB1(const TGeoManager *mgr=gGeoManager);
    void        CreateOBSpaceFrameObjects(const TGeoManager *mgr=gGeoManager);

    TGeoXtru* CreateStaveSide(const char *name,
			       Double_t dz, Double_t alpha, Double_t beta,
			       Double_t L, Double_t H, Bool_t top);
    TGeoCombiTrans* CreateCombiTrans( const char *name,
				      Double_t dy, Double_t dz, Double_t dphi,
				      Bool_t planeSym=kFALSE);
    void AddTranslationToCombiTrans( TGeoCombiTrans* ct,
				     Double_t dx=0, Double_t dy=0,
				     Double_t dz=0) const;


    Int_t     fLayerNumber; // Current layer number
    Double_t  fPhi0;        // lab phi of 1st stave, in degrees!!!
    Double_t  fLayRadius;   // Inner radius of this layer
    Double_t  fZLength;     // Z length of this layer
    Double_t  fSensorThick; // Sensor thickness
    Double_t  fChipThick;   // Chip thickness
    Double_t  fStaveWidth;  // Stave width (for turbo layers only)
    Double_t  fStaveTilt;   // Stave tilt angle (for turbo layers only) in degrees
    Int_t     fNStaves;     // Number of staves in this layer
    Int_t     fNModules;    // Number of modules per container if defined (HalfStave, Stave, whatever is container)
    Int_t     fNChips;      // N. chips per container (module, HalfStave, Stave, whatever is container)
    Int_t     fHierarchy[kNHLevels]; // array to query number of staves, hstaves, modules, chips per its parent volume
    //    
    UInt_t    fChipTypeID;  // detector type id
    Bool_t    fIsTurbo;     // True if this layer is a "turbo" layer
    Int_t     fBuildLevel;  // Used for material studies

    AliITSUv1::AliITSUModel_t fStaveModel; // The stave model

    // Parameters for the Upgrade geometry

    // General Parameters
    static const Int_t    fgkNumberOfInnerLayers;// Number of IB Layers

    static const Double_t fgkDefaultSensorThick; // Default sensor thickness
    static const Double_t fgkDefaultChipThick;   // Default chip thickness

    // Inner Barrel Parameters
    static const Int_t    fgkIBChipsPerRow;      // IB chips per row in module
    static const Int_t    fgkIBNChipRows;        // IB chip rows in module

    static const Double_t fgkIBFlexCableAlThick; // Thickness of FPC Aluminum
    static const Double_t fgkIBFlexCableKapThick;// Thickness of FPC Kapton
    static const Double_t fgkIBGlueThick;        // IB glue thickness
    static const Double_t fgkIBCarbonFleeceThick;// IB carbon fleece thickness
    static const Double_t fgkIBCarbonPaperThick; // IB Carbon Paper Thickness
    static const Double_t fgkIBK13D2UThick;      // IB k13d2u prepreg thickness
    static const Double_t fgkIBCoolPipeInnerD;   // IB cooling inner diameter
    static const Double_t fgkIBCoolPipeThick;    // IB cooling pipe thickness
    static const Double_t fgkIBCoolPipeXDist;    // IB cooling pipe separation
    static const Double_t fgkIBTopVertexWidth1;  // IB TopVertex width
    static const Double_t fgkIBTopVertexWidth2;  // IB TopVertex width
    static const Double_t fgkIBTopVertexHeight;  // IB TopVertex height
    static const Double_t fgkIBTopVertexAngle ;  // IB TopVertex aperture angle
    static const Double_t fgkIBSideVertexWidth;  // IB SideVertex width
    static const Double_t fgkIBSideVertexHeight; // IB SideVertex height
    static const Double_t fgkIBTopFilamentLength;// IB TopFilament length
    static const Double_t fgkIBTopFilamentSide;  // IB TopFilament side
    static const Double_t fgkIBTopFilamentAlpha; // IB TopFilament angle
    static const Double_t fgkIBTopFilamentGamma; // IB TopFilament angle

    static const Double_t fgkIBStaveHeight;      // IB Stave Total Y Height

    // Outer Barrel Parameters
    static const Int_t    fgkOBChipsPerRow;      // OB chips per row in module
    static const Int_t    fgkOBNChipRows;        // OB chip rows in module

    static const Double_t fgkOBHalfStaveWidth;   // OB Half Stave Width
    static const Double_t fgkOBModuleWidth;      // OB Module Width
    static const Double_t fgkOBModuleGap;        // Gap between OB modules
    static const Double_t fgkOBChipXGap;         // Gap between OB chips on X
    static const Double_t fgkOBChipZGap;         // Gap between OB chips on Z
    static const Double_t fgkOBFlexCableAlThick; // Thickness of FPC Aluminum
    static const Double_t fgkOBFlexCableCuThick; // Thickness of FPC Copper
    static const Double_t fgkOBFlexCableKapThick1;// Thickness of FPC Kapton
    static const Double_t fgkOBFlexCableKapThick;// Thickness of FPC Kapton
    static const Double_t fgkOBBusCableAlThick;  // Thickness of Bus Aluminum
    static const Double_t fgkOBBusCableKapThick; // Thickness of Bus Kapton
    static const Double_t fgkOBCarbonPlateThick; // OB Carbon Plate Thickness
    static const Double_t fgkOBColdPlateThick;   // OB Cold Plate Thickness
    static const Double_t fgkOBGlueThickM1;      // OB Glue total Thickness
    static const Double_t fgkOBGlueThick;        // OB Glue Thickness in Model2
    static const Double_t fgkOBModuleZLength;    // OB Chip Length along Z
    static const Double_t fgkOBHalfStaveYPos;    // OB half staves Y position
    static const Double_t fgkOBHalfStaveYTrans;  // OB half staves Y transl.
    static const Double_t fgkOBHalfStaveXOverlap;// OB half staves X overlap
    static const Double_t fgkOBGraphiteFoilThick;// OB graphite foil thickness
    static const Double_t fgkOBCarbonFleeceThick;// OB carbon fleece thickness
    static const Double_t fgkOBCoolTubeInnerDM1; // OB cooling inner diameter
    static const Double_t fgkOBCoolTubeInnerD;   // OB cooling inner diameter
    static const Double_t fgkOBCoolTubeThick;    // OB cooling tube thickness
    static const Double_t fgkOBCoolTubeXDist;    // OB cooling tube separation

    static const Double_t fgkOBSpaceFrameZLen[2];// OB Space Frame Length
    static const Int_t    fgkOBSpaceFrameNUnits[2];//OB Number of SF Units
    static const Double_t fgkOBSpaceFrameUnitLen;// OB Space Frame Unit length
    static const Double_t fgkOBSpaceFrameWidth;  // OB Space Frame Width
    static const Double_t fgkOBSpaceFrameHeight; // OB Space Frame Height
    static const Double_t fgkOBSpaceFrameTopVL;  // Parameters defining...
    static const Double_t fgkOBSpaceFrameTopVH;  // ...the Top V shape
    static const Double_t fgkOBSpaceFrameSideVL; // Parameters defining...
    static const Double_t fgkOBSpaceFrameSideVH; // ...the Side V shape
    static const Double_t fgkOBSpaceFrameVAlpha; // Angles of aperture...
    static const Double_t fgkOBSpaceFrameVBeta;  // ...of the V shapes
    static const Double_t fgkOBSFrameBaseRibDiam;// OB SFrame Base Rib Diam
    static const Double_t fgkOBSFrameBaseRibPhi; // OB SF base beam angle
    static const Double_t fgkOBSFrameSideRibDiam;// OB SFrame Side Rib Diam
    static const Double_t fgkOBSFrameSideRibPhi; // OB SF side beam angle
    static const Double_t fgkOBSFrameULegLen;    // OB SF U-Leg length
    static const Double_t fgkOBSFrameULegWidth;  // OB SF U-Leg width
    static const Double_t fgkOBSFrameULegHeight1;// OB SF U-Leg height
    static const Double_t fgkOBSFrameULegHeight2;// OB SF U-Leg height
    static const Double_t fgkOBSFrameULegThick;  // OB SF U-Leg thickness
    static const Double_t fgkOBSFrameULegXPos;   // OB SF U-Leg X position


  ClassDef(AliITSUv1Layer,0) // ITS Upgrade v1 geometry
};

#endif
