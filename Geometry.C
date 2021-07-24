#include <iostream>
#include <fstream>
#include <cmath>

#include "TCanvas.h"
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"
#include "TGeoManager.h"

void Geometry()
{
   gSystem->Load("libGeom");
   //--- Definition of a simple geometry
   TGeoManager *geom = new TGeoManager("world", "the simplest geometry");
   Int_t i;
    //--- define some materials
   TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
    //   //--- define some media
   TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
   //--gefine detector volume	
   TGeoVolume *detac = geom->MakeBox("Detector", Vacuum, 30., 30.,4.);
   //--sey top valuome to be dectro volume
   //geom->SetTopVolume(detac);
   //--define layer Volume
   TGeoVolume *layer = geom->MakeBox("LAYER", Vacuum,15,15,1.5);
   //layer->SetVisibility(kFALSE);
   geom->SetTopVolume(layer);
   //--- define some materials
   TGeoMaterial *matAl = new TGeoMaterial("Al", 26.98,13,2.7);
   
   //   //--- define some media
   TGeoMedium *Al = new TGeoMedium("Root Material",2, matAl);
 
   //--- make the hexagon container volume
   TGeoVolume *hexa = geom->MakePgon("HEXA", Al, 0.0,360.0,6,2);
   hexa->SetLineColor(kGreen);
   //--side of the hexagon--
   double Sqrt3 = sqrt(3.0);
   Double_t a = 7.5/Sqrt3; 
   TGeoPgon *pgon = (TGeoPgon*)(hexa->GetShape());
   pgon->DefineSection(0,0,0,a);
   pgon->DefineSection(1,1,0,a);
   geom->SetTopVolume(hexa);
   
   TGeoTranslation *tr1 = new TGeoTranslation(0., 0., 0.);
   layer->AddNode(hexa, 1, tr1);
 
   TGeoRotation   *rot1 = new TGeoRotation("rot1", 90,180,90,90,0,30);

   TGeoCombiTrans *combi[6];
   for(int i=1;i<=6;i++){
	double angle = i*M_PI/3.0;
	combi[i] = new TGeoCombiTrans(a*Sqrt3*sin(angle),a*Sqrt3*cos(angle), 0., rot1);
	layer->AddNode(hexa, 1, combi[i]);
   }

   /*TGeoCombiTrans *combi1 = new TGeoCombiTrans(13.7, 7.7, 0., rot1);
   TGeoCombiTrans *combi2 = new TGeoCombiTrans(13.5, -8, 0., rot1);
   TGeoCombiTrans *combi3 = new TGeoCombiTrans(0, -15.5, 0., rot1);
   TGeoCombiTrans *combi4 = new TGeoCombiTrans(-13.5, -7.5, 0., rot1);
   TGeoCombiTrans *combi5 = new TGeoCombiTrans(-13.5, 7.7, 0., rot1);
   TGeoCombiTrans *combi6 = new TGeoCombiTrans(0, 15.5, 0., rot1);
	*/
   //TGeoTranslation *tr2 = new TGeoTranslation(20., 0, 0.);
   //detac->AddNode(layer, 1, tr2);
   geom->CloseGeometry();

   geom->SetTopVisible(); // the TOP is invisible
   layer->Draw();
   //myhex->Draw();
}
