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
    TGeoMaterial *matSi = new TGeoMaterial("Si", 28.09,14,2.3296);
    TGeoMaterial *matCu = new TGeoMaterial("Cu", 63.546,29,8.96);
    TGeoMaterial *matPb = new TGeoMaterial("Pb", 207.2,82,11.342);
    //   //--- define some media
    TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
    TGeoMedium *Si = new TGeoMedium("Si Material",2, matSi);
    TGeoMedium *Cu = new TGeoMedium("Cu Material",3, matCu);
    TGeoMedium *Pb = new TGeoMedium("Pb Material",4, matPb);

   //--define detector volume
   TGeoVolume *detac = geom->MakeBox("Detector", Vacuum, 400., 400.,30.);
   //--define layer Volume
   TGeoVolume *layer_active = geom->MakeBox("ACTIVELAYER", Vacuum,400,400,7.1);
   TGeoVolume *layer_Pb = geom->MakeBox("PbLAYER", Pb,400,400,7.1);
   TGeoVolume *layer_vacuum = geom->MakeBox("VACUUMLAYER", Vacuum,400,400,7.1);
   //set top volume
   geom->SetTopVolume(detac);


   //--- make the hexagon container volume
   TGeoVolume *hexa_si = geom->MakePgon("HEXA_Si", Si, 0.0,360.0,6,2);
   hexa_si->SetLineColor(kGreen);
   TGeoVolume *hexa_cu = geom->MakePgon("HEXA_cu", Si, 0.0,360.0,6,2);
   hexa_cu->SetLineColor(kOrange);

   //--side of the hexagon--
   double Sqrt3 = sqrt(3.0);
   Double_t a = 101.6;
   //Double_t a = 4.0;
   Double_t dr = a*Sqrt3/2.0; 
   cout<<a<<endl;
   TGeoPgon *pgon_si_front = (TGeoPgon*)(hexa_si->GetShape());
   pgon_si_front->DefineSection(0,7.1,0,dr);
   pgon_si_front->DefineSection(1,3.1,0,dr);

   TGeoPgon *pgon_cu = (TGeoPgon*)(hexa_cu->GetShape());
   pgon_cu->DefineSection(0,3.1,0,dr);
   pgon_cu->DefineSection(1,-3.1,0,dr);


   layer_active->AddNode(hexa_si, 1, new TGeoTranslation(0., 0., 0.));
   layer_active->AddNode(hexa_cu, 1, new TGeoTranslation(0., 0., 0.));
   layer_active->AddNode(hexa_si, 1, new TGeoTranslation(0., 0., -10.2));

   //geom->CheckPoint(a*Sqrt3*sin(M_PI/3), a*Sqrt3*cos(M_PI/3), 0.0);

   TGeoRotation   *rot1 = new TGeoRotation("rot1", 90,180,90,90,0,30);

   int num_crl=1;
   TGeoTranslation *trns[3*6*num_crl];
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
   	                layer_active->AddNode(hexa_si, 1, trns[i+6*(crl-1)]);
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
   	                layer_active->AddNode(hexa_si, 1, trns[i+6*(crl-1)]);
   	                trns[i+6+6*(crl-1)] = new TGeoTranslation(R*sin(angle),R*cos(angle), 0.);
   	                layer_active->AddNode(hexa_cu, 1, trns[i+6+6*(crl-1)]);
   	                trns[i+12+6*(crl-1)] = new TGeoTranslation(R*sin(angle),R*cos(angle), -10.2);
   	                layer_active->AddNode(hexa_si, 1, trns[i+12+6*(crl-1)]);
   	                angle+=M_PI/3.0;
   	            }
   	            X+=2*dr;
   	            cout<<"crl : "<<crl<<" 2dr+X: "<<X<<" Y : "<<Y<<" odd"<<endl;
   	       }
       }

   detac->AddNode(layer_active, 1, new TGeoTranslation(0., 0., 0.));
   geom->CloseGeometry();

   geom->SetTopVisible(); // the TOP is invisible
   detac->Draw();
   //myhex->Draw();
   TView *view = gPad->GetView();
   view->ShowAxis();
}
