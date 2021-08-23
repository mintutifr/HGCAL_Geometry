# HGCAL_Geometry
Draw Geometry using xml file:

    root -l

    gGeoManager = new TGeoManager()

    gGeoManager->Import("mygeometry.xml")

    gGeoManager->SetTopVisible()

    gGeoManager->GetTopVolume()->Draw() 
