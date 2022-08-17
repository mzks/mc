//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
//-----------------------------------------------------------------------------
//
// GEANT4 Class file 
//
// File name:  G4EmCaptureCascade
//
// Author:        V.Ivanchenko (Vladimir.Ivantchenko@cern.ch)
// 
// Creation date: 22 April 2012 on base of G4MuMinusCaptureCascade
//
//
//-----------------------------------------------------------------------------
//
// Modifications: 
//
//-----------------------------------------------------------------------------

#include "G4EmCaptureCascade.hh"
#include "CustomG4EmCaptureCascade.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh" 
#include "G4MuonMinus.hh"
#include "G4AntiProton.hh"
#include "G4AntiNeutron.hh"
#include "G4AntiDeuteron.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4NucleiProperties.hh"

#include "G4PrimaryParticle.hh"
#include "G4Track.hh"

#include <fstream>
#include <iostream>
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

CustomG4EmCaptureCascade::CustomG4EmCaptureCascade()
    : G4HadronicInteraction("emCaptureCascade")
{ 
    theElectron = G4Electron::Electron();
    theGamma = G4Gamma::Gamma();
    fMuMass = G4MuonMinus::MuonMinus()->GetPDGMass();
    fPbarMass = G4AntiProton::AntiProton()->GetPDGMass();
    fNbarMass = G4AntiNeutron::AntiNeutron()->GetPDGMass();
    fDbarMass = G4AntiDeuteron::AntiDeuteron()->GetPDGMass();

    fAntiParticleMass = 0.0;
    fAntiParticleType = 0;
    fTime = 0.0;

    const char* path = getenv("G4XRAYCASCADE");
    if ( !path) {
        G4Exception("CustomG4EmCaptureCascade::CustomG4EmCaptureCascade()", "mat521",
                FatalException, "G4XRAYCASCADE environment variable not set");
    }
    std::string dataPath(path);  

    //table for Silicon
    G4String pbarSiFilename(dataPath + "/cascade_pbarSi.data");
    //std::cout<<pbarSiFilename<<std::endl;   

    std::ifstream pbarcas_SiFile(pbarSiFilename.c_str());
    if(pbarcas_SiFile.bad()) 
    {
        std::cerr<<pbarSiFilename<<" does not exist!"<<std::endl;
        exit(-1);
    }
    for(G4int ii=0;ii<32;++ii)
    {
        for (G4int jj=0;jj<4;++jj)
        {
            if(pbarcas_SiFile.eof()) 
            {
                std::cerr<<pbarSiFilename<<" is not complete!"<<std::endl;	
                exit(-1);
            }
            else pbarcas_SiFile>>pbar_si[ii][jj];
        }
    }

    G4String dbarSiFilename(dataPath + "/cascade_dbarSi.data"); 
    //std::cout<<dbarSiFilename<<std::endl;   

    std::ifstream dbarcas_SiFile(dbarSiFilename.c_str());
    if(dbarcas_SiFile.bad()) 
    {
        std::cerr<<dbarSiFilename<<" does not exist!"<<std::endl;
        exit(-1);
    }
    for(G4int ii=0;ii<55;++ii)
    {
        for (G4int jj=0;jj<4;++jj)
        {
            if(dbarcas_SiFile.eof()) 
            {
                std::cerr<<dbarSiFilename<<" is not complete!"<<std::endl;	
                exit(-1);
            }
            else dbarcas_SiFile>>dbar_si[ii][jj];
        }
    }


    // tables for Al
    G4String pbarAlFilename(dataPath + "/cascade_pbarAl.data");
    std::cout<<pbarAlFilename<<std::endl;   

    std::ifstream pbarcas_AlFile(pbarAlFilename.c_str());
    if(pbarcas_AlFile.bad()) 
    {
        std::cerr<<pbarAlFilename<<" does not exist!"<<std::endl;
        exit(-1);
    }
    for(G4int ii=0;ii<31;++ii)
    {
        for (G4int jj=0;jj<4;++jj)
        {
            if(pbarcas_AlFile.eof()) 
            {
                std::cerr<<pbarAlFilename<<" is not complete!"<<std::endl;	
                exit(-1);
            }
            else pbarcas_AlFile>>pbar_al[ii][jj];
        }
    }

    G4String dbarAlFilename(dataPath + "/cascade_dbarAl.data");
    //std::cout<<dbarAlFilename<<std::endl;   

    std::ifstream dbarcas_AlFile(dbarAlFilename.c_str());
    if(dbarcas_AlFile.bad()) 
    {
        std::cerr<<dbarAlFilename<<" does not exist!"<<std::endl;
        exit(-1);
    }
    for(G4int ii=0;ii<52;++ii)
    {
        for (G4int jj=0;jj<4;++jj)
        {
            if(dbarcas_AlFile.eof()) 
            {
                std::cerr<<dbarAlFilename<<" is not complete!"<<std::endl;	
                exit(-1);
            }
            else dbarcas_AlFile>>dbar_al[ii][jj];
        }
    }  

    // tables for Carbon
    G4String pbarCFilename(dataPath + "/cascade_pbarC.data");
    //std::cout<<pbarCFilename<<std::endl;   

    std::ifstream pbarcas_CFile(pbarCFilename.c_str());
    if(pbarcas_CFile.bad()) 
    {
        std::cerr<<pbarCFilename<<" does not exist!"<<std::endl;
        exit(-1);
    }
    for(G4int ii=0;ii<27;++ii)
    {
        for (G4int jj=0;jj<4;++jj)
        {
            if(pbarcas_CFile.eof()) 
            {
                std::cerr<<pbarCFilename<<" is not complete!"<<std::endl;	
                exit(-1);
            }
            else pbarcas_CFile>>pbar_c[ii][jj];
        }
    }

    G4String dbarCFilename(dataPath + "/cascade_dbarC.data");
    //std::cout<<dbarCFilename<<std::endl;   

    std::ifstream dbarcas_CFile(dbarCFilename.c_str());
    if(dbarcas_CFile.bad()) 
    {
        std::cerr<<dbarCFilename<<" does not exist!"<<std::endl;
        exit(-1);
    }
    for(G4int ii=0;ii<48;++ii)
    {
        for (G4int jj=0;jj<4;++jj)
        {
            if(dbarcas_CFile.eof()) 
            {
                std::cerr<<dbarCFilename<<" is not complete!"<<std::endl;	
                exit(-1);
            }
            else dbarcas_CFile>>dbar_c[ii][jj];
        }
    }    

    // Calculate the Energy of K Mesoatom Level for this Element using
    // the Energy of Hydrogen Atom taken into account finite size of the
    // nucleus 
    static const G4int nlevels = 28;
    static const G4int listK[nlevels] = {1, 2,  4,  6,  8, 11, 14, 17, 18, 21, 24,26, 29, 32, 38, 40, 41, 44, 49, 53, 55,60, 65, 70, 75, 81, 85, 92};
    static const G4double listKEnergy[nlevels] = {0.00275, 0.011, 0.043, 0.098, 0.173, 0.326,0.524, 0.765, 0.853, 1.146, 1.472,1.708, 2.081, 2.475, 3.323, 3.627,3.779, 4.237, 5.016, 5.647, 5.966,
        6.793, 7.602, 8.421, 9.249, 10.222,10.923,11.984};

    fKLevelEnergy[0] = 0.0;
    fKLevelEnergy[1] = listKEnergy[0];
    G4int idx = 1;
    for(G4int i=1; i<nlevels; ++i) 
    {
        G4int z1 = listK[idx];
        G4int z2 = listK[i];
        if(z1+1 < z2) 
        {
            G4double dz = G4double(z2 - z1);
            G4double y1 = listKEnergy[idx]/G4double(z1*z1);
            G4double y2 = listKEnergy[i]/G4double(z2*z2);
            for(G4int z=z1+1; z<z2; ++z) 
            {
                fKLevelEnergy[z] = (y1 + (y2 - y1)*(z - z1)/dz)*z*z;
            }
        }
        fKLevelEnergy[z2] = listKEnergy[i];
        idx = i;  
    }
    //initial energy for all level, dbar=59, pbar=42, mu=14
    for(G4int i = 0; i< 59; ++i) { fLevelEnergy[i] = 0.0; }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

CustomG4EmCaptureCascade::~CustomG4EmCaptureCascade()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

    G4HadFinalState* 
CustomG4EmCaptureCascade::ApplyYourself(const G4HadProjectile& projectile,
        G4Nucleus& targetNucleus)
{
    result.Clear();
    result.SetStatusChange(isAlive);
    fTime = projectile.GetGlobalTime();
    fAntiParticleMass = projectile.GetDefinition()->GetPDGMass();
    fAntiParticleType = projectile.GetDefinition()->GetPDGEncoding();
    G4String fAntiParticleName = projectile.GetDefinition()->GetParticleName();
    //  G4cout<<"Name: "<<fAntiParticleName<<" "<<"PDGCode: "<<fAntiParticleType<<" "<<"Mass: "<<fAntiParticleMass<<"MeV"<<G4endl;

    G4int Z = targetNucleus.GetZ_asInt(); 
    G4int A = targetNucleus.GetA_asInt(); 
    G4double massA = G4NucleiProperties::GetNuclearMass(A, Z);
    G4double mass = fAntiParticleMass * massA / (fAntiParticleMass + massA) ;
    G4double e = 13.6 * eV * (Z * Z) * mass/ electron_mass_c2;

    // precise corrections of energy only for K-shell
    fLevelEnergy[0] = fKLevelEnergy[std::min(Z, 92)];
    for(G4int i=1; i<60; ++i) {
        fLevelEnergy[i] = e/(G4double)((i+1)*(i+1));
    }
    fLevelEnergy[0] = fLevelEnergy[1];

    G4int nElec  = Z;
    G4int nAuger = 1;
    G4int nLevel = 0;
    if (fAntiParticleType==-2212) nLevel = 42;
    else if (fAntiParticleType==-1000010020) nLevel = 59;
    else nLevel = 14;

    G4double pGamma = (Z*Z*Z*Z);

    // Capture on 14-th level for muon, 42-th for pbar, 60-th level for dbar
    G4double edep=0.;
    if (fAntiParticleType==-2212) edep = fLevelEnergy[41];
    else if (fAntiParticleType==-1000010020) edep = fLevelEnergy[58];
    else edep = fLevelEnergy[13];

    AddNewParticle(theElectron,edep);
    G4double deltaE = 0.;
    G4int iLevel = 0;

    if (fAntiParticleType==-2212)  // antiproton case, M.Xiao, May.31-2019
    {
        do {
            G4double var_pbar = 1.0*G4UniformRand();
            if (Z==14) // use pbar_Si table
            {
                for (G4int k=1;k<nLevel;k++)
                {
                    iLevel = k;
                    deltaE = fLevelEnergy[iLevel-1] -  fLevelEnergy[nLevel-1]; 
                    for (G4int kk=0;kk<32; kk++)
                    {
                        if (nLevel==pbar_si[kk][0]&&iLevel==pbar_si[kk][1]&&var_pbar<pbar_si[kk][2]) AddNewParticle(theGamma, deltaE);
                        else continue;
                    }
                }
            }
            if (Z==13) //use pbar-Al table
            {
                for (G4int k=1;k<nLevel;k++)
                {
                    iLevel = k;
                    deltaE = fLevelEnergy[iLevel-1] -  fLevelEnergy[nLevel-1]; 
                    for (G4int kk=0;kk<31; kk++)
                    {
                        if (nLevel==pbar_al[kk][0]&&iLevel==pbar_al[kk][1]&&var_pbar<pbar_al[kk][2]) AddNewParticle(theGamma, deltaE);
                        else continue;
                    }
                }
            }
            if (Z==6) //use pbar-Carbon table, added by M.Xiao, Jul.1-2019
            {
                for (G4int k=1;k<nLevel;k++)
                {
                    iLevel = k;
                    deltaE = fLevelEnergy[iLevel-1] -  fLevelEnergy[nLevel-1]; 
                    for (G4int kk=0;kk<27; kk++)
                    {
                        if (nLevel==pbar_c[kk][0]&&iLevel==pbar_c[kk][1]&&var_pbar<pbar_c[kk][2]) AddNewParticle(theGamma, deltaE);
                        else continue;
                    }
                }
            }

            --nLevel;
            edep += deltaE;
        } while( nLevel > 0 );
    }

    if (fAntiParticleType==-1000010020) // antideuteron case, M.Xiao, May.31-2019
    {
        do {
            G4double var_dbar = 1.0*G4UniformRand();
            if (Z==14) // use dbar-Si table
            {
                for (G4int k=1;k<nLevel;k++)
                {
                    iLevel = k;
                    deltaE = fLevelEnergy[iLevel-1] - fLevelEnergy[nLevel-1];
                    for (G4int kk=0; kk<55; kk++)
                    {
                        if (nLevel==dbar_si[kk][0]&&iLevel==dbar_si[kk][1]&&var_dbar<dbar_si[kk][2]) AddNewParticle(theGamma, deltaE);
                        else continue;
                    }	
                }
            }
            if (Z==13) // use dbr-Al table
            {
                for (G4int k=1;k<nLevel;k++)
                {
                    iLevel = k;
                    deltaE = fLevelEnergy[iLevel-1] - fLevelEnergy[nLevel-1];
                    for (G4int kk=0; kk<52; kk++)
                    {
                        if (nLevel==dbar_al[kk][0]&&iLevel==dbar_al[kk][1]&&var_dbar<dbar_al[kk][2]) AddNewParticle(theGamma, deltaE);
                        else continue;
                    }
                }
            }
            if (Z==6) // use dbar-Carbon table, added by M.Xiao, Jul.1-2019
            {
                for (G4int k=1;k<nLevel;k++)
                {
                    iLevel = k;
                    deltaE = fLevelEnergy[iLevel-1] - fLevelEnergy[nLevel-1];
                    for (G4int kk=0; kk<48; kk++)
                    {
                        if (nLevel==dbar_c[kk][0]&&iLevel==dbar_c[kk][1]&&var_dbar<dbar_c[kk][2]) AddNewParticle(theGamma, deltaE);
                        else continue;
                    }	
                }
            }

            --nLevel;
            edep += deltaE; 
        } while(nLevel > 0);
    }

    // Emit new photon or electron
    // Simplified model for probabilities
    // N.C.Mukhopadhyay Phy. Rep. 30 (1977) 1.
    if (fAntiParticleType==13)  // mu- case, use previous code
    {
        do {
            // case of Auger electrons
            if((nAuger < nElec) && ((pGamma + 10000.0) * G4UniformRand() < 10000.0) )
            {
                ++nAuger;
                deltaE =  fLevelEnergy[nLevel-1] -  fLevelEnergy[nLevel];
                --nLevel;
                AddNewParticle(theElectron, deltaE);
            } else {
                // Case of photon cascade, probabilities from
                // C.S.Wu and L.Wilets, Ann. Rev. Nuclear Sci. 19 (1969) 527.
                G4double var = (10.0 + G4double(nLevel - 1) ) * G4UniformRand();
                iLevel = nLevel - 1 ;
                if(var > 10.0) iLevel -= G4int(var-10.0) + 1;
                if( iLevel < 0 ) iLevel = 0;
                deltaE =  fLevelEnergy[iLevel] -  fLevelEnergy[nLevel];
                nLevel = iLevel;

                // MAR - 2021 switch on muonic x-rays
                //deltaE = 0; //comment this line to simulate muonic x-rays, M.Xiao
                AddNewParticle(theGamma, deltaE);
            }

            edep += deltaE;

            // Loop checking, 06-Aug-2015, Vladimir Ivanchenko
        } while( nLevel > 0 );
    }

    if (fAntiParticleType!=-2212&&fAntiParticleType!=-1000010020&&fAntiParticleType!=13) // for other antiparticles, no cascade x-rays, M.Xiao, May.31-2019
    {
        do {
            deltaE = 0.;
            AddNewParticle(theGamma, deltaE);

            --nLevel;
        } while( nLevel > 0 );
    }

    //  result.Clear(); // clear the above results
    result.SetLocalEnergyDeposit(edep);
    return &result;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void CustomG4EmCaptureCascade::ModelDescription(std::ostream& outFile) const
{
    outFile << "Simulation of electromagnetic cascade from capture level"
        << " to K-shell of the mesonic atom\n."
        << "Probabilities of gamma and Auger transitions from\n"
        << "  N.C.Mukhopadhyay Phys. Rep. 30 (1977) 1.\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

