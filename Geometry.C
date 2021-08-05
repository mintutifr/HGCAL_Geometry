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
   TGeoVolume *layer = geom->MakeBox("LAYER", Vacuum,30,30,1.5);
   //layer->SetVisibility(kFALSE);
   geom->SetTopVolume(layer);
   //--- define some materials
   TGeoMaterial *matAl = new TGeoMaterial("Al", 26.98,13,2.7);
   
   //   //--- define some media
   TGeoMedium *Al = new TGeoMedium("Root Material",2, matAl);
 
   //--- make the hexagon container volume
   TGeoVolume *hexa = geom->MakePgon("HEXA", Al, 0.0,360.0,6,2);
   hexa->SetLineColor(kGreen);
   
   //TGeoVolume *hexa1 = geom->MakePgon("HEXA1", Al, 0.0,360.0,6,2);
   //hexa1->SetLineColor(kGreen);

   //--side of the hexagon--
   double Sqrt3 = sqrt(3.0);
   Double_t a = 6;
   //Double_t a = 4.0;
   Double_t dr = a*Sqrt3/2.0; 
   cout<<a<<endl;
   TGeoPgon *pgon = (TGeoPgon*)(hexa->GetShape());
   pgon->DefineSection(0,0,0,dr);
   pgon->DefineSection(1,1,0,dr);
   geom->SetTopVolume(hexa);
   
   TGeoTranslation *tr1 = new TGeoTranslation(0., 0., 0.);
   layer->AddNode(hexa, 1, tr1);
	
   //geom->CheckPoint(a*Sqrt3*sin(M_PI/3), a*Sqrt3*cos(M_PI/3), 0.0);

   TGeoRotation   *rot1 = new TGeoRotation("rot1", 90,180,90,90,0,30);

   int num_crl=3;
   TGeoCombiTrans *combi[6*num_crl];
   for(int crl=1;crl<=num_crl;crl++){
   	for(int i=1;i<=6;i++){
		double angle = i*M_PI/3.0+(crl-1)*M_PI/6.0;
		if(crl%2==0){
		       	combi[i+6*(crl-1)] = new TGeoCombiTrans((crl/2+2)*a*sin(angle),(crl/2+2)*a*cos(angle), 0., rot1);
			layer->AddNode(hexa, 1, combi[i+6*(crl-1)]);
		}
		else{
			combi[i+6*(crl-1)] = new TGeoCombiTrans((crl+1)*dr*sin(angle),(crl+1)*dr*cos(angle), 0., rot1);
			layer->AddNode(hexa, 1, combi[i+6*(crl-1)]);
		}
		//cout<<i<<endl;
   	}
   }

   TGeoCombiTrans *combi2[6];
   double angle = atan(3.0*Sqrt3/8.0);
   double R = sqrt(27.0/4.0+16.0)*dr;
   combi2[0] = new TGeoCombiTrans(R*sin(angle),R*cos(angle),0.,rot1);//((crl/2+2)*a*sin(an),(crl/2+2)*a*cos(angle), 0., rot1);
   layer->AddNode(hexa, 1, combi2[0]);
   //TGeoTranslation *tr2 = new TGeoTranslation(20., 0, 0.);
   //detac->AddNode(layer, 1, tr2);
   geom->CloseGeometry();

   geom->SetTopVisible(); // the TOP is invisible
   layer->Draw();
   //myhex->Draw();
   TView *view = gPad->GetView();
   view->ShowAxis();
}
