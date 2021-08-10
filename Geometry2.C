#include <iostream>
#include <fstream>
#include <cmath>

#include "TCanvas.h"
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"
#include "TGeoManager.h"

void Geometry2()
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
   TGeoVolume *layer = geom->MakeBox("LAYER", Vacuum,50,50,1.5);
   //layer->SetVisibility(kFALSE);
   geom->SetTopVolume(layer);
   //--- define some materials
   TGeoMaterial *matAl = new TGeoMaterial("Al", 26.98,13,2.7);
   
   //   //--- define some media
   TGeoMedium *Al = new TGeoMedium("Root Material",2, matAl);
 
   //--- make the hexagon container volume
   TGeoVolume *hexa = geom->MakePgon("HEXA", Al, 0.0,360.0,6,2);
   hexa->SetLineColor(kGreen);
   
   TGeoVolume *hexa1 = geom->MakePgon("HEXA1", Al, 0.0,360.0,6,2);
   hexa1->SetFillColor(kRed);
   hexa1->SetLineColor(kRed);

   //--side of the hexagon--
   double Sqrt3 = sqrt(3.0);
   Double_t a = 5;
   //Double_t a = 4.0;
   Double_t dr = a*Sqrt3/2.0; 
   cout<<a<<endl;
   TGeoPgon *pgon = (TGeoPgon*)(hexa->GetShape());
   pgon->DefineSection(0,0,0,dr);
   pgon->DefineSection(1,1,0,dr);
   //geom->SetTopVolume(hexa);

   TGeoPgon *pgon1 = (TGeoPgon*)(hexa1->GetShape());
   pgon1->DefineSection(0,0,0,dr);
   pgon1->DefineSection(1,1,0,dr);
   //geom->SetTopVolume(hexa);

   TGeoTranslation *tr1 = new TGeoTranslation(0., 0., 0.);
   layer->AddNode(hexa, 1, tr1);
	
   //geom->CheckPoint(a*Sqrt3*sin(M_PI/3), a*Sqrt3*cos(M_PI/3), 0.0);

   TGeoRotation   *rot1 = new TGeoRotation("rot1", 90,180,90,90,0,30);

   int num_crl=6;
   TGeoTranslation *trns[6*num_crl];
   double X = dr;
   double Y = (3.0/2.0)*(2*dr/Sqrt3);
   double R;
   double angle;
   cout<<"X : "<<X<<" dr : "<<dr<<" Y : "<<Y<<" a : "<<a<<endl;
   for(int crl=1;crl<=num_crl;crl++){
   	        if(crl%2==0){
   	            R = sqrt(X*X+Y*Y);
		        angle = atan(Y/X);
   	            for(int i=1;i<=6;i++){
   	                //cout<<"X: "<<X<<" R : "<<R<<" angle : "<<angle<<" Even"<<endl;
   	                trns[i+6*(crl-1)] = new TGeoTranslation(R*sin(angle),R*cos(angle), 0.);
   	                layer->AddNode(hexa, 1, trns[i+6*(crl-1)]);
   	                angle+=M_PI/3.0;
   	            }
   	            Y+=3.0*a/2.0;
   	            X-=dr;
   	            cout<<"crl :"<<crl<<" X-dr: "<<X<<" Y+3a/2 : "<<Y<<" Even"<<endl;
   	        }
   	        else{
   	            R = sqrt(X*X+Y*Y);
		        angle = atan(Y/X);
		        //cout<<"R : "<<R<<" angle : "<<angle<<endl;
   	            for(int i=1;i<=6;i++){
   	                //cout<<"X: "<<X<<" R : "<<R<<" angle : "<<angle<<" Even"<<endl;
   	                trns[i+6*(crl-1)] = new TGeoTranslation(R*sin(angle),R*cos(angle), 0.);
   	                layer->AddNode(hexa1, 1, trns[i+6*(crl-1)]);
   	                angle+=M_PI/3.0;
   	            }
   	            X+=2*dr;
   	            cout<<"crl : "<<crl<<" 2dr+X: "<<X<<" Y : "<<Y<<" odd"<<endl;
   	       }
       }

   geom->CloseGeometry();

   geom->SetTopVisible(); // the TOP is invisible
   layer->Draw();
   //myhex->Draw();
   TView *view = gPad->GetView();
   view->ShowAxis();
}
