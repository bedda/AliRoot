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

/*
$Log$
*/

#include "AliGenHijing.h"
#include "AliRun.h"

#include <TArrayI.h>
#include <TParticle.h>
#include <THijing.h>

 ClassImp(AliGenHijing)

AliGenHijing::AliGenHijing()
                 :AliGenerator()
{
// Constructor
}

AliGenHijing::AliGenHijing(Int_t npart)
                 :AliGenerator(npart)
{
// Default PbPb collisions at 5. 5 TeV
//
    SetEnergyCMS();
    SetImpactParameterRange();
    SetTarget();
    SetProjectile();
    fKeep=0;
    fQuench=1;
    fShadowing=1;
    fTrigger=0;
    fDecaysOff=1;
    fEvaluate=0;
    fSelectAll=0;
}

AliGenHijing::AliGenHijing(const AliGenHijing & Hijing)
{
// copy constructor
}


AliGenHijing::~AliGenHijing()
{
// Destructor
}

void AliGenHijing::Init()
{
// Initialisation
    SetMC(new THijing(fEnergyCMS, fFrame, fProjectile, fTarget, 
		      fAProjectile, fZProjectile, fATarget, fZTarget, 
		      fMinImpactParam, fMaxImpactParam));

    fHijing=(THijing*) fgMCEvGen;
    fHijing->Initialize();
    fHijing->SetIHPR2(3,  fTrigger);
    fHijing->SetIHPR2(4,  fQuench);
    fHijing->SetIHPR2(6,  fShadowing);
    fHijing->SetIHPR2(12, fDecaysOff);    
    fHijing->SetIHPR2(21, fKeep);
//
    if (fEvaluate) EvaluateCrossSections();
}

void AliGenHijing::Generate()
{
// Generate one event

    Float_t polar[3] =   {0,0,0};
    Float_t origin[3]=   {0,0,0};
    Float_t origin0[3]=  {0,0,0};
    Float_t p[3], random[6];
    Float_t tof;

    static TClonesArray *particles;
//  converts from mm/c to s
    const Float_t kconv=0.001/2.999792458e8;
//
    Int_t nt=0;
    Int_t jev=0;
    Int_t j, kf, ks, imo;

    if(!particles) particles=new TClonesArray("TParticle",10000);
    
    fTrials=0;
    for (j=0;j<3;j++) origin0[j]=fOrigin[j];
    if(fVertexSmear==perEvent) {
	gMC->Rndm(random,6);
	for (j=0;j<3;j++) {
	    origin0[j]+=fOsigma[j]*TMath::Cos(2*random[2*j]*TMath::Pi())*
		TMath::Sqrt(-2*TMath::Log(random[2*j+1]));
//	    fHijing->SetMSTP(151,0);
	}
    } else if (fVertexSmear==perTrack) {
//	fHijing->SetMSTP(151,0);
	for (j=0;j<3;j++) {
//	    fHijing->SetPARP(151+j, fOsigma[j]*10.);
	}
    }
    while(1)
    {

	fHijing->GenerateEvent();
	fTrials++;
	fHijing->ImportParticles(particles,"All");
	Int_t np = particles->GetEntriesFast();
	printf("\n **************************************************%d\n",np);
	Int_t nc=0;
	if (np == 0 ) continue;
	Int_t i;
	Int_t * newPos = new Int_t[np];
	for (i = 0; i<np-1; i++) *(newPos+i)=i;
	
	for (i = 0; i<np-1; i++) {
	    TParticle *  iparticle       = (TParticle *) particles->At(i);

	    Bool_t  hasMother            =  (iparticle->GetFirstMother()   >=0);
	    Bool_t  hasDaughter          =  (iparticle->GetFirstDaughter() >=0);
	    Bool_t  selected             =  kTRUE;
	    Bool_t  hasSelectedDaughters =  kFALSE;

	    if (!fSelectAll) selected = KinematicSelection(iparticle);
	    kf        = iparticle->GetPdgCode();
//	    Int_t id1=iparticle->GetFirstDaughter();
//	    Int_t id2=iparticle->GetLastDaughter();	    
//	    printf("\n particle  %d %d %d %d %d %d\n",i, kf, id1, id2, hasDaughter, selected);

	    if (hasDaughter && !selected) hasSelectedDaughters = DaughtersSelection(iparticle, particles);
//
// Put particle on the stack if it is either selected or it is the mother of at least one seleted particle
//

	    if (selected || hasSelectedDaughters) {
		nc++;
		ks        = iparticle->GetStatusCode();
		p[0]=iparticle->Px();
		p[1]=iparticle->Py();
		p[2]=iparticle->Pz();
		origin[0]=origin0[0]+iparticle->Vx()/10;
		origin[1]=origin0[1]+iparticle->Vy()/10;
		origin[2]=origin0[2]+iparticle->Vz()/10;
		tof=kconv*iparticle->T();
		imo=-1;
		if (hasMother) {
		    imo=iparticle->GetFirstMother();
		    imo=*(newPos+imo);
		}
		
//		printf("\n selected iparent %d %d %d \n",i, kf, imo);
		if (hasDaughter) {
		    gAlice->SetTrack(0,imo,kf,p,origin,polar,
				     tof,"Primary",nt);
		} else {
		    gAlice->SetTrack(fTrackIt,imo,kf,p,origin,polar,
				     tof,"Secondary",nt);
		}
		*(newPos+i)=nt;
	    } // selected
	} // particle loop 
	delete newPos;
	printf("\n I've put %i particles on the stack \n",nc);
	if (nc > 0) {
	    jev+=nc;
	    if (jev >= fNpart || fNpart == -1) {
		fKineBias=Float_t(fNpart)/Float_t(fTrials);
		printf("\n Trials: %i %i %i\n",fTrials, fNpart, jev);
		break;
	    }
	}
    } // event loop
}

Bool_t AliGenHijing::KinematicSelection(TParticle *particle)
{
// Perform kinematic selection
    Float_t px=particle->Px();
    Float_t py=particle->Py();
    Float_t pz=particle->Pz();
    Float_t  e=particle->Energy();

//
//  transverse momentum cut    
    Float_t pt=TMath::Sqrt(px*px+py*py);
    if (pt > fPtMax || pt < fPtMin) 
    {
//	printf("\n failed pt cut %f %f %f \n",pt,fPtMin,fPtMax);
	return kFALSE;
    }
//
// momentum cut
    Float_t p=TMath::Sqrt(px*px+py*py+pz*pz);
    if (p > fPMax || p < fPMin) 
    {
//	printf("\n failed p cut %f %f %f \n",p,fPMin,fPMax);
	return kFALSE;
    }
    
//
// theta cut
    Float_t  theta = Float_t(TMath::ATan2(Double_t(pt),Double_t(pz)));
    if (theta > fThetaMax || theta < fThetaMin) 
    {
	
//    	printf("\n failed theta cut %f %f %f \n",theta,fThetaMin,fThetaMax);
	return kFALSE;
    }

//
// rapidity cut
    Float_t y = 0.5*TMath::Log((e+pz)/(e-pz));
    if (y > fYMax || y < fYMin)
    {
//	printf("\n failed y cut %f %f %f \n",y,fYMin,fYMax);
	return kFALSE;
    }

//
// phi cut
    Float_t phi=Float_t(TMath::ATan2(Double_t(py),Double_t(px)));
    if (phi > fPhiMax || phi < fPhiMin)
    {
//	printf("\n failed phi cut %f %f %f \n",phi,fPhiMin,fPhiMax);
	return kFALSE;
    }

    return kTRUE;
}

void AliGenHijing::KeepFullEvent()
{
    fKeep=1;
}

void AliGenHijing::EvaluateCrossSections()
{
//     Glauber Calculation of geometrical x-section
//
    Float_t xTot=0.;          // barn
    Float_t xTotHard=0.;      // barn 
    Float_t xPart=0.;         // barn
    Float_t xPartHard=0.;     // barn 
    Float_t sigmaHard=0.1;    // mbarn
    Float_t bMin=0.;
    Float_t bMax=fHijing->GetHIPR1(34)+fHijing->GetHIPR1(35);
    const Float_t kdib=0.2;
    Int_t   kMax=Int_t((bMax-bMin)/kdib)+1;


    printf("\n Projectile Radius (fm): %f \n",fHijing->GetHIPR1(34));
    printf("\n Target     Radius (fm): %f \n",fHijing->GetHIPR1(35));    
    Int_t i;
    Float_t oldvalue=0.;
    
    for (i=0; i<kMax; i++)
    {
	Float_t xb=bMin+i*kdib;
	Float_t ov;
	ov=fHijing->Profile(xb);
	Float_t gb =  2.*0.01*fHijing->GetHIPR1(40)*kdib*xb*(1.-TMath::Exp(-fHijing->GetHINT1(12)*ov));
	Float_t gbh = 2.*0.01*fHijing->GetHIPR1(40)*kdib*xb*sigmaHard*ov;
	xTot+=gb;
	xTotHard+=gbh;
	if (xb > fMinImpactParam && xb < fMaxImpactParam)
	{
	    xPart+=gb;
	    xPartHard+=gbh;
	}
	
	if ((xTot-oldvalue)/oldvalue<0.0001) break;
	oldvalue=xTot;
	printf("\n Total cross section (barn): %d %f %f \n",i, xb, xTot);
	printf("\n Hard  cross section (barn): %d %f %f \n\n",i, xb, xTotHard);
    }
    printf("\n Total cross section (barn): %f \n",xTot);
    printf("\n Hard  cross section (barn): %f \n \n",xTotHard);
    printf("\n Partial       cross section (barn): %f %f \n",xPart, xPart/xTot*100.);
    printf("\n Partial  hard cross section (barn): %f %f \n",xPartHard, xPartHard/xTotHard*100.);
}

Bool_t AliGenHijing::DaughtersSelection(TParticle* iparticle, TClonesArray* particles)
{
//
// Looks recursively if one of the daughters has been selected
//
//    printf("\n Consider daughters %d:",iparticle->GetPdgCode());
    Int_t imin=-1;
    Int_t imax=-1;
    Int_t i;
    Bool_t hasDaughters= (iparticle->GetFirstDaughter() >=0);
    Bool_t selected=kFALSE;
    if (hasDaughters) {
	imin=iparticle->GetFirstDaughter();
	imax=iparticle->GetLastDaughter();       
//	printf("\n Has daughters %d %d:", imin, imax);
	for (i=imin; i<= imax; i++){
	    TParticle *  jparticle       = (TParticle *) particles->At(i);	
//	    Int_t ip=jparticle->GetPdgCode();
//	    printf("\n consider daughter %d %d", i,ip);
	    
	    if (KinematicSelection(jparticle)) {selected=kTRUE; break;}
	    if (DaughtersSelection(jparticle, particles)) {selected=kTRUE; break; }
	}
    } else {
	return kFALSE;
    }

    return selected;
}



AliGenHijing& AliGenHijing::operator=(const  AliGenHijing& rhs)
{
// Assignment operator
    return *this;
}
