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
    TGeoMaterial *matSi = new TGeoMaterial("Si", 28.09,14,2.3296);
    TGeoMaterial *matCu = new TGeoMaterial("Cu", 63.546,29,8.96);
    TGeoMaterial *matPb = new TGeoMaterial("Pb", 207.2,82,11.342);
    //   //--- define some media
    TGeoMedium *Vacuum = new TGeoMedium("Vacuum",1, matVacuum);
    TGeoMedium *Si = new TGeoMedium("Si Material",2, matSi);
    TGeoMedium *Cu = new TGeoMedium("Cu Material",3, matCu);
    TGeoMedium *Pb = new TGeoMedium("Pb Material",4, matPb);

   //--define detecter volume
   TGeoVolume *detec = geom->MakeBox("Detector", Vacuum, length_layer, length_layer,detector_width/2);

   //--define Casset volumes volume
   TGeoVolume *CEE_Casset_Typ1 = geom->MakeBox("CEE_Casset_Typ1", Vacuum, length_layer, length_layer,CEE_Casset_Typ1_width/2);
   TGeoVolume *CEE_Casset_Typ2 = geom->MakeBox("CEE_Casset_Typ2", Vacuum, length_layer, length_layer,CEE_Casset_Typ2_width/2);
   TGeoVolume *CEE_Casset_Typ3 = geom->MakeBox("CEE_Casset_Typ3", Vacuum, length_layer, length_layer,CEE_Casset_Typ3_width/2);

   //--define layer Volume
   TGeoVolume *layer_Pb_Typ1 = geom->MakeBox("PbLAYER_Typ1", Pb,length_layer,length_layer,Width_Pb_layer_Typ1/2);
   TGeoVolume *layer_Pb_Typ2 = geom->MakeBox("PbLAYER_Type2", Pb,length_layer,length_layer,Width_Pb_layer_Typ2/2);
   TGeoVolume *layer_Pb_Typ3 = geom->MakeBox("PbLAYER_Type3", Pb,length_layer,length_layer,Width_Pb_layer_Typ3/2);
   TGeoVolume *layer_Cu = geom->MakeBox("PbLAYER", Cu,length_layer,length_layer,Width_Cu_layer/2);
   TGeoVolume *layer_active = geom->MakeBox("ACTIVELAYER", Vacuum,length_layer,length_layer,Width_active_layer/2);
   TGeoVolume *layer_vacuum = geom->MakeBox("VACUUMLAYER", Vacuum,length_layer,length_layer,Width_vacuum_layer/2);

   //set top volume
   geom->SetTopVolume(detec);


   //--- make the hexagon container volume
   TGeoVolume *hexa_si = geom->MakePgon("HEXA_Si", Si, 0.0,360.0,6,2);
   hexa_si->SetLineColor(kGreen);

   //--side of the hexagon--
   double Sqrt3 = sqrt(3.0);
   //Double_t a = 4.0;
   Double_t dr = a*Sqrt3/2.0; 
   cout<<a<<endl;
   TGeoPgon *pgon_si_front = (TGeoPgon*)(hexa_si->GetShape());
   pgon_si_front->DefineSection(0,Width_active_si_wefer/2,0,dr);
   pgon_si_front->DefineSection(1,-Width_active_si_wefer/2,0,dr);


   layer_active->AddNode(hexa_si, 1, new TGeoTranslation(0., 0., 0.));

   //geom->CheckPoint(a*Sqrt3*sin(M_PI/3), a*Sqrt3*cos(M_PI/3), 0.0);

   TGeoRotation   *rot1 = new TGeoRotation("rot1", 90,180,90,90,0,30);

   int num_crl=1;
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
   	                angle+=M_PI/3.0;
   	            }
   	            X+=2*dr;
   	            cout<<"crl : "<<crl<<" 2dr+X: "<<X<<" Y : "<<Y<<" odd"<<endl;
   	       }
       }
   layer_Pb_Typ1->SetLineColor(kBlue);
   layer_Pb_Typ2->SetLineColor(kBlue);
   layer_Pb_Typ3->SetLineColor(kBlue);
   layer_vacuum->SetLineColor(kRed);
   layer_Cu->SetLineColor(kOrange);
   // add volumes in casset type 1
   CEE_Casset_Typ1->AddNode(layer_Pb_Typ1, 1, new TGeoTranslation(0., 0., CEE_Casset_Typ1_width/2-Width_Pb_layer_Typ1/2));
   CEE_Casset_Typ1->AddNode(layer_vacuum, 2, new TGeoTranslation(0., 0., CEE_Casset_Typ1_width/2-Width_Pb_layer_Typ1-Width_vacuum_layer/2));
   CEE_Casset_Typ1->AddNode(layer_active, 3, new TGeoTranslation(0., 0., CEE_Casset_Typ1_width/2-Width_Pb_layer_Typ1-Width_vacuum_layer-Width_active_layer/2));
   CEE_Casset_Typ1->AddNode(layer_Cu, 4, new TGeoTranslation(0., 0., CEE_Casset_Typ1_width/2-Width_Pb_layer_Typ1-Width_vacuum_layer-Width_active_layer-Width_Cu_layer/2));
   CEE_Casset_Typ1->AddNode(layer_active, 5, new TGeoTranslation(0., 0., CEE_Casset_Typ1_width/2-Width_Pb_layer_Typ1-Width_vacuum_layer-Width_active_layer-Width_Cu_layer-Width_active_layer/2));
   CEE_Casset_Typ1->AddNode(layer_vacuum, 6, new TGeoTranslation(0., 0., CEE_Casset_Typ1_width/2-Width_Pb_layer_Typ1-Width_vacuum_layer-2*Width_active_layer-Width_Cu_layer-Width_vacuum_layer/2));

   // add volumes in casset type 2
   CEE_Casset_Typ2->AddNode(layer_Pb_Typ2, 1, new TGeoTranslation(0., 0., CEE_Casset_Typ2_width/2-Width_Pb_layer_Typ2/2));
   CEE_Casset_Typ2->AddNode(layer_vacuum, 2, new TGeoTranslation(0., 0., CEE_Casset_Typ2_width/2-Width_Pb_layer_Typ2-Width_vacuum_layer/2));
   CEE_Casset_Typ2->AddNode(layer_active, 3, new TGeoTranslation(0., 0., CEE_Casset_Typ2_width/2-Width_Pb_layer_Typ2-Width_vacuum_layer-Width_active_layer/2));
   CEE_Casset_Typ2->AddNode(layer_Cu, 4, new TGeoTranslation(0., 0., CEE_Casset_Typ2_width/2-Width_Pb_layer_Typ2-Width_vacuum_layer-Width_active_layer-Width_Cu_layer/2));
   CEE_Casset_Typ2->AddNode(layer_active, 5, new TGeoTranslation(0., 0., CEE_Casset_Typ2_width/2-Width_Pb_layer_Typ2-Width_vacuum_layer-Width_active_layer-Width_Cu_layer-Width_active_layer/2));
   CEE_Casset_Typ2->AddNode(layer_vacuum, 6, new TGeoTranslation(0., 0., CEE_Casset_Typ2_width/2-Width_Pb_layer_Typ2-Width_vacuum_layer-2*Width_active_layer-Width_Cu_layer-Width_vacuum_layer/2));

   // add volumes in casset type 2
   CEE_Casset_Typ3->AddNode(layer_Pb_Typ3, 1, new TGeoTranslation(0., 0., CEE_Casset_Typ3_width/2-Width_Pb_layer_Typ3/2));
   CEE_Casset_Typ3->AddNode(layer_vacuum, 2, new TGeoTranslation(0., 0., CEE_Casset_Typ3_width/2-Width_Pb_layer_Typ3-Width_vacuum_layer/2));
   CEE_Casset_Typ3->AddNode(layer_active, 3, new TGeoTranslation(0., 0., CEE_Casset_Typ3_width/2-Width_Pb_layer_Typ3-Width_vacuum_layer-Width_active_layer/2));
   CEE_Casset_Typ3->AddNode(layer_Cu, 4, new TGeoTranslation(0., 0., CEE_Casset_Typ3_width/2-Width_Pb_layer_Typ3-Width_vacuum_layer-Width_active_layer-Width_Cu_layer/2));
   CEE_Casset_Typ3->AddNode(layer_active, 5, new TGeoTranslation(0., 0., CEE_Casset_Typ3_width/2-Width_Pb_layer_Typ3-Width_vacuum_layer-Width_active_layer-Width_Cu_layer-Width_active_layer/2));
   CEE_Casset_Typ3->AddNode(layer_vacuum, 6, new TGeoTranslation(0., 0., CEE_Casset_Typ3_width/2-Width_Pb_layer_Typ3-Width_vacuum_layer-2*Width_active_layer-Width_Cu_layer-Width_vacuum_layer/2));

    // add casset volume in detector volume
   detec->AddNode(CEE_Casset_Typ1,1, new TGeoTranslation(0., 0., detector_width/2-CEE_Casset_Typ1_width/2));
   detec->AddNode(CEE_Casset_Typ2,2, new TGeoTranslation(0., 0., detector_width/2-CEE_Casset_Typ1_width-CEE_Casset_Typ2_width/2));
   detec->AddNode(CEE_Casset_Typ3,3, new TGeoTranslation(0., 0., detector_width/2-CEE_Casset_Typ1_width-CEE_Casset_Typ2_width-CEE_Casset_Typ3_width/2));

   geom->CloseGeometry();

   geom->SetTopVisible(); // the TOP is invisible
   geom->Export("mygeometry_dd4ENV.xml");
   geom->Export("mygeometry_dd4ENV.gdml");
   detec->Draw();
   //myhex->Draw();
   TView *view = gPad->GetView();
   view->ShowAxis();
}
