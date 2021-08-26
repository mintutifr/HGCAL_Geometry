#include <iostream>
#include <fstream>
#include <cmath>

#include "TCanvas.h"
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"
#include "TGeoManager.h"

void Wold_Vacuum()
{
   Double_t a = 10.16;   //Side length od the Hexagone
   Double_t Width_Pb_layer_Typ1 = 0.387;
   Double_t Width_Pb_layer_Typ2 = 0.607;
   Double_t Width_Pb_layer_Typ3 = 0.932;
   Double_t Width_Cu_layer = 0.62;
   Double_t Width_active_layer = 0.4;
   Double_t Width_active_si_wefer = 0.031;
   Double_t Width_vacuum_layer = 0.51;
   Double_t length_layer = 5.1*a/2;

   Double_t CEE_Casset_Typ1_width = Width_Pb_layer_Typ1+2*Width_vacuum_layer+2*Width_active_layer+Width_Cu_layer;
   Double_t CEE_Casset_Typ2_width = Width_Pb_layer_Typ2+2*Width_vacuum_layer+2*Width_active_layer+Width_Cu_layer;
   Double_t CEE_Casset_Typ3_width = Width_Pb_layer_Typ3+2*Width_vacuum_layer+2*Width_active_layer+Width_Cu_layer;

   Double_t detector_width = CEE_Casset_Typ1_width+CEE_Casset_Typ2_width+CEE_Casset_Typ3_width;

   gSystem->Load("libGeom");
   //--- Definition of a simple geometry
   TGeoManager *geom = new TGeoManager("world", "the simplest geometry");
 
   Int_t i;

    //--- define some materials
    TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
    //   //--- define some media
    TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);

   //--define detecter volume
   TGeoVolume *detec = geom->MakeBox("Detector", Vacuum, length_layer, length_layer,detector_width/2);

   //set top volume
   geom->SetTopVolume(detec);
   geom->CloseGeometry();

   geom->SetTopVisible(); // the TOP is invisible
   geom->Export("mygeometry_dd4ENV_Vacuum_world.xml");
   geom->Export("mygeometry_dd4ENV_Vacuum_world.gdml");
   detec->Draw();
   //myhex->Draw();
   TView *view = gPad->GetView();
   view->ShowAxis();
}
