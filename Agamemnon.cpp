// Agamemnon.cpp : Defines the exported functions for the DLL application.
//

#define STRICT
#define ORBITER_MODULE
#include "orbitersdk.h"
#include "UMmuSDK.h"
#include "Agamemnon.h"
#include "OrbiterSoundSDK35.h"

#define MYGRAVSTARTSOUND 1
#define MYGRAVSTOPSOUND  2


HINSTANCE g_hDLL;

//constructor
Agamemnon::Agamemnon(OBJHANDLE hObj, int fmodel)
    :VESSEL3(hObj, fmodel)
{
    DefineAnimations();

}

Agamemnon::~Agamemnon()
{
}

//Define animation for gravity wheel
void Agamemnon::DefineAnimations()
{
    static UINT gravwheel_groups[16] = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 40, 69};
    static MGROUP_ROTATE gravwheel (0,gravwheel_groups, 16,_V(0, 55,0),_V(0,0,-1),(float)(360*RAD));
    anim_grav = CreateAnimation(0.0);
    AddAnimationComponent (anim_grav, 0, 1, &gravwheel);
}


DLLCLBK void InitModule (HINSTANCE hModule)
{
g_hDLL = hModule;

}

////////////////////////////////////////////////////////
// SendHudMessage - UMMU 2.0
////////////////////////////////////////////////////////
char *Agamemnon::SendHudMessage()
{
dHudMessageDelay=15;
return cUmmuHudDisplay;
}

//----------------------------------------
//Set vessel class capabilities/capacities
//----------------------------------------
void Agamemnon::clbkSetClassCaps (FILEHANDLE cfg) 
{ 

	//UMMu Capabilities
	Crew.InitUmmu(GetHandle());// Must always be the first one
/*	iActiveDockNumber=0;
    iActiveDockNumberForHud = 0; */
//    SetUMMUAirlockPos();
	Crew.DefineAirLockShape(AirLockStatus,-5,5,37.5,47.5,479.8,489.8);// Airlock open, 10 meter large 10 meter high 10 meter long
	Crew.SetMembersPosRotOnEVA(_V(0,-42.5,486.8),_V(0,0,0)); //position when EVA
	Crew.SetMaxSeatAvailableInShip(100);// Max Numbers of UMMU for this ship
	// Add four default members for when the ship is spawned by scenario editor
	Crew.AddCrewMember("Jack Maynard",41,65,74,"Capt");
	Crew.AddCrewMember("Ensign Ogilvie",27,67,55);
	Crew.AddCrewMember("Doctor Hunter",36,70,45,"Doc");
	Crew.AddCrewMember("Lieutenant Patrick",39,70,45);
	iSelectedUmmuMember=0; // Initialize of select member variable
	// The HUD display method variables
	cUmmuHudDisplay[0] =0;// Initialisation of UMmu hud char variable
	dHudMessageDelay =0;// Initialisation of UMmu delay variable
	strcpy(SendHudMessage(),"Welcome aboard! E=EVA   1,2=Previous/Next Crew Member   Shift+1,2=Previous/Next Airlock   S=info   M=add crew   K=Start/Stop Crew Hab Wheel Rotation");

    // The “Add Mmu without scenario editor” variable see PDF doc
	cAddUMmuToVessel[0]=0;
    THRUSTER_HANDLE th_main, th_rcs[14], th_group[4];

    SetSize (EXP_SIZE);
    SetEmptyMass (EXP_EMPTYMASS);
    SetPMI (EXP_PMI);
    SetCrossSections (EXP_CS);

	// SetDockParams (_V(0,-10,1400.8), _V(0,0,1), _V(0,1,0));

    PROPELLANT_HANDLE ph_main = CreatePropellantResource(EXP_FUELMASS);

    //main engine definition and exhaust locations
    th_main = CreateThruster(_V(0,0,-710.6), _V(0,0,1), EXP_MAXMAINTH, ph_main, EXP_ISP);
    CreateThrusterGroup(&th_main, 1, THGROUP_MAIN);
    AddExhaust(th_main, 1, 16, _V(-46,17.5,-714.5), _V(0,0,-1));
    AddExhaust(th_main, 1, 16, _V(46,17,-714.5), _V(0,0,-1));
    AddExhaust(th_main, 1, 16, _V(-46,95.9,-714.5), _V(0,0,-1));
    AddExhaust(th_main, 1, 16, _V(46,95.9,-714.5), _V(0,0,-1));
    

    //RCS Engine Definitions
    // **Port-side Aft**
    th_rcs[0] = CreateThruster(_V(-310, 0, -1847.05),_V(1,0,0), EXP_RCSTH, ph_main, EXP_ISP);

    // **Starboard-side Aft**
    th_rcs[1] = CreateThruster(_V(310, 0, -1847.05),_V(-1,0,0), EXP_RCSTH, ph_main, EXP_ISP);
    
    // **Top Aft**
    th_rcs[2] = CreateThruster(_V(-85.28, 190.9, -1374.62), _V(0,-1,0), EXP_RCSTH, ph_main, EXP_ISP);
    th_rcs[3] = CreateThruster(_V(85.28, 190.9, -1374.62), _V(0,-1,0), EXP_RCSTH, ph_main, EXP_ISP);
    
    // **Bottom Aft**
    th_rcs[4] = CreateThruster(_V(-85.28, -190.9, -1374.62), _V(0,1,0), EXP_RCSTH, ph_main, EXP_ISP);
    th_rcs[5] = CreateThruster(_V(85.28, -190.9, -1374.62), _V(0,1,0), EXP_RCSTH, ph_main, EXP_ISP);

    // **Port-side Front**
    th_rcs[6] = CreateThruster(_V(-310,0,1847.05), _V(1,0,0), EXP_RCSTH, ph_main, EXP_ISP);

    // **Starboard-side Front**
    th_rcs[7] = CreateThruster(_V(310.97,0,1847.05), _V(-1,0,0), EXP_RCSTH, ph_main, EXP_ISP);

    // **Bow, Topside**
    th_rcs[8] = CreateThruster(_V(185.28,101.08,1374.62), _V(0,-1,0), EXP_RCSTH, ph_main, EXP_ISP);
    th_rcs[9] = CreateThruster(_V(-185.28,101.08,1374.62), _V(0,-1,0), EXP_RCSTH, ph_main, EXP_ISP);

    // **Bow, Bottom**
    th_rcs[10] = CreateThruster(_V(185.28,-101.08,1374.62), _V(0,1,0), EXP_RCSTH, ph_main, EXP_ISP);
    th_rcs[11] = CreateThruster(_V(-185.28,-101.08,1374.62), _V(0,1,0), EXP_RCSTH, ph_main, EXP_ISP);

    // **forward translation thruster (located on stern of ship)
    th_rcs[12] = CreateThruster(_V(0,0,-2300), _V(0,0,1), EXP_RCSTH, ph_main, EXP_ISP);

    // **reverse translation thruster (located on bow of ship)
    th_rcs[13] = CreateThruster(_V(0,0,2300), _V(0,0,-1), EXP_RCSTH, ph_main, EXP_ISP);

//Define Thruster Groups for Attitude Control

    //**Pitch Up**
    th_group[0] = th_rcs[2];
    th_group[1] = th_rcs[3];
    th_group[2] = th_rcs[10];
    th_group[3] = th_rcs[11];
    CreateThrusterGroup (th_group, 4, THGROUP_ATT_PITCHUP);

    //**Pitch Down**
    th_group[0] = th_rcs[4];
    th_group[1] = th_rcs[5];
    th_group[2] = th_rcs[8];
    th_group[3] = th_rcs[9];
    CreateThrusterGroup (th_group, 4, THGROUP_ATT_PITCHDOWN);

    //**Bank Left**
    th_group[0] = th_rcs[2];
    th_group[1] = th_rcs[9];
    th_group[2] = th_rcs[10];
    th_group[3] = th_rcs[5];
    CreateThrusterGroup (th_group, 4, THGROUP_ATT_BANKLEFT);

    //**Bank Right**
    th_group[0] = th_rcs[3];
    th_group[1] = th_rcs[8];
    th_group[2] = th_rcs[4];
    th_group[3] = th_rcs[11];
    CreateThrusterGroup (th_group, 4, THGROUP_ATT_BANKRIGHT);

    //**Attitude Up**
    th_group[0] = th_rcs[4];
    th_group[1] = th_rcs[5];
    th_group[2] = th_rcs[10];
    th_group[3] = th_rcs[11];
    CreateThrusterGroup (th_group, 4, THGROUP_ATT_UP);

    //**Attitude Down**
    th_group[0] = th_rcs[2];
    th_group[1] = th_rcs[3];
    th_group[2] = th_rcs[8];
    th_group[3] = th_rcs[9];
    CreateThrusterGroup (th_group, 4, THGROUP_ATT_DOWN);

    //**Yaw Left**
    th_group[0] = th_rcs[7];
    th_group[1] = th_rcs[0];
    CreateThrusterGroup (th_group, 2, THGROUP_ATT_YAWLEFT);

    //**Yaw Right**
    th_group[0] = th_rcs[6];
    th_group[1] = th_rcs[1];
    CreateThrusterGroup (th_group, 2, THGROUP_ATT_YAWRIGHT);

    //**Attitude Left**
    th_group[0] = th_rcs[1];
    th_group[1] = th_rcs[7];
    CreateThrusterGroup (th_group, 2, THGROUP_ATT_LEFT);

    //**Attitude Right**
    th_group[0] = th_rcs[0];
    th_group[1] = th_rcs[6];
    CreateThrusterGroup (th_group, 2, THGROUP_ATT_RIGHT);

    //**Attitude Forward**
    CreateThrusterGroup (th_rcs+12, 1, THGROUP_ATT_FORWARD);

	//**Attitude Reverse**
    CreateThrusterGroup (th_rcs+13, 1, THGROUP_ATT_BACK);

	

	
	//**Hangar Bay Marker Lights (end of antennae)
	//************************************

			//Lower starboard
			static BEACONLIGHTSPEC baymarker1; 
			static VECTOR3 baymarker1pos = {27.6, 41.8, 635};
			static VECTOR3 baymarker1col = {1,0.8,0};
			baymarker1.shape = BEACONSHAPE_DIFFUSE;
			baymarker1.pos = &baymarker1pos;
			baymarker1.col = &baymarker1col;
			baymarker1.size = 0.5;
			baymarker1.falloff = 0.4;
			baymarker1.period = 1.8;
			baymarker1.duration = 0.1;
			baymarker1.tofs = 1.8;
			baymarker1.active = true; 
			AddBeacon (&baymarker1);

			static BEACONLIGHTSPEC baymarker2; 
			static VECTOR3 baymarker2pos = {27.6, 41.8, 635};
			static VECTOR3 baymarker2col = {1,0.8,0};
			baymarker2.shape = BEACONSHAPE_DIFFUSE;
			baymarker2.pos = &baymarker2pos;
			baymarker2.col = &baymarker2col;
			baymarker2.size = 0.5;
			baymarker2.falloff = 0.4;
			baymarker2.period = 1.8;
			baymarker2.duration = 0.1;
			baymarker2.tofs = 1.45;
			baymarker2.active = true; 
			AddBeacon (&baymarker2);


			//Upper starboard
			static BEACONLIGHTSPEC baymarker3; 
			static VECTOR3 baymarker3pos = {27.6, 71.2, 635};
			static VECTOR3 baymarker3col = {1,0.8,0};
			baymarker3.shape = BEACONSHAPE_DIFFUSE;
			baymarker3.pos = &baymarker3pos;
			baymarker3.col = &baymarker3col;
			baymarker3.size = 0.5;
			baymarker3.falloff = 0.4;
			baymarker3.period = 1.8;
			baymarker3.duration = 0.1;
			baymarker3.tofs = 1.8;
			baymarker3.active = true; 
			AddBeacon (&baymarker3);

			
			static BEACONLIGHTSPEC baymarker4; 
			static VECTOR3 baymarker4pos = {27.6, 71.2, 635};
			static VECTOR3 baymarker4col = {1,0.8,0};
			baymarker4.shape = BEACONSHAPE_DIFFUSE;
			baymarker4.pos = &baymarker4pos;
			baymarker4.col = &baymarker4col;
			baymarker4.size = 0.5;
			baymarker4.falloff = 0.4;
			baymarker4.period = 1.8;
			baymarker4.duration = 0.1;
			baymarker4.tofs = 1.45;
			baymarker4.active = true; 
			AddBeacon (&baymarker4);


			//Lower port
			static BEACONLIGHTSPEC baymarker5; 
			static VECTOR3 baymarker5pos = {-27.6, 41.8, 635};
			static VECTOR3 baymarker5col = {1,0.8,0};
			baymarker5.shape = BEACONSHAPE_DIFFUSE;
			baymarker5.pos = &baymarker5pos;
			baymarker5.col = &baymarker5col;
			baymarker5.size = 0.5;
			baymarker5.falloff = 0.4;
			baymarker5.period = 1.8;
			baymarker5.duration = 0.1;
			baymarker5.tofs = 1.0;
			baymarker5.active = true; 
			AddBeacon (&baymarker5);

			static BEACONLIGHTSPEC baymarker6; 
			static VECTOR3 baymarker6pos = {-27.6, 41.8, 635};
			static VECTOR3 baymarker6col = {1,0.8,0};
			baymarker6.shape = BEACONSHAPE_DIFFUSE;
			baymarker6.pos = &baymarker6pos;
			baymarker6.col = &baymarker6col;
			baymarker6.size = 0.5;
			baymarker6.falloff = 0.4;
			baymarker6.period = 1.8;
			baymarker6.duration = 0.1;
			baymarker6.tofs = 0.65;
			baymarker6.active = true; 
			AddBeacon (&baymarker6);


			//Upper port
			static BEACONLIGHTSPEC baymarker7; 
			static VECTOR3 baymarker7pos = {-27.6, 71.2, 635};
			static VECTOR3 baymarker7col = {1,0.8,0};
			baymarker7.shape = BEACONSHAPE_DIFFUSE;
			baymarker7.pos = &baymarker7pos;
			baymarker7.col = &baymarker7col;
			baymarker7.size = 0.5;
			baymarker7.falloff = 0.4;
			baymarker7.period = 1.8;
			baymarker7.duration = 0.1;
			baymarker7.tofs = 1.0;
			baymarker7.active = true; 
			AddBeacon (&baymarker7);

			
			static BEACONLIGHTSPEC baymarker8; 
			static VECTOR3 baymarker8pos = {-27.6, 71.2, 635};
			static VECTOR3 baymarker8col = {1,0.8,0};
			baymarker8.shape = BEACONSHAPE_DIFFUSE;
			baymarker8.pos = &baymarker8pos;
			baymarker8.col = &baymarker8col;
			baymarker8.size = 0.5;
			baymarker8.falloff = 0.4;
			baymarker8.period = 1.8;
			baymarker8.duration = 0.1;
			baymarker8.tofs = 0.65;
			baymarker8.active = true; 
			AddBeacon (&baymarker8);
//========================================================

			//Front Antenna Markers
			//=====================
			static BEACONLIGHTSPEC frontmarker1; 
			static VECTOR3 frontmarker1pos = {0, -120.4, 686.1};
			static VECTOR3 frontmarker1col = {1,0,0};
			frontmarker1.shape = BEACONSHAPE_DIFFUSE;
			frontmarker1.pos = &frontmarker1pos;
			frontmarker1.col = &frontmarker1col;
			frontmarker1.size = 0.7;
			frontmarker1.falloff = 0.4;
			frontmarker1.period = 0;
			frontmarker1.duration = 1;
			frontmarker1.tofs = 0;
			frontmarker1.active = true; 
			AddBeacon (&frontmarker1);

			static BEACONLIGHTSPEC frontmarker2; 
			static VECTOR3 frontmarker2pos = {37.8, 140.8, 675.8};
			static VECTOR3 frontmarker2col = {1,0,0};
			frontmarker2.shape = BEACONSHAPE_DIFFUSE;
			frontmarker2.pos = &frontmarker2pos;
			frontmarker2.col = &frontmarker2col;
			frontmarker2.size = 0.7;
			frontmarker2.falloff = 0.4;
			frontmarker2.period = 0;
			frontmarker2.duration = 1;
			frontmarker2.tofs = 0;
			frontmarker2.active = true; 
			AddBeacon (&frontmarker2);

			static BEACONLIGHTSPEC frontmarker3; 
			static VECTOR3 frontmarker3pos = {0, -218.9, 560};
			static VECTOR3 frontmarker3col = {1,0,0};
			frontmarker3.shape = BEACONSHAPE_DIFFUSE;
			frontmarker3.pos = &frontmarker3pos;
			frontmarker3.col = &frontmarker3col;
			frontmarker3.size = 0.7;
			frontmarker3.falloff = 0.4;
			frontmarker3.period = 0;
			frontmarker3.duration = 1;
			frontmarker3.tofs = 0;
			frontmarker3.active = true; 
			AddBeacon (&frontmarker3);

			static BEACONLIGHTSPEC frontmarker4; 
			static VECTOR3 frontmarker4pos = {0, -177, 540.1};
			static VECTOR3 frontmarker4col = {1,0,0};
			frontmarker4.shape = BEACONSHAPE_DIFFUSE;
			frontmarker4.pos = &frontmarker4pos;
			frontmarker4.col = &frontmarker4col;
			frontmarker4.size = 0.7;
			frontmarker4.falloff = 0.4;
			frontmarker4.period = 0;
			frontmarker4.duration = 1;
			frontmarker4.tofs = 0;
			frontmarker4.active = true; 
			AddBeacon (&frontmarker4);


			//rear antenna on bottom of shields
			//Left
			static BEACONLIGHTSPEC frontmarker5; 
			static VECTOR3 frontmarker5pos = {-6.5, -110.1, 444.9};
			static VECTOR3 frontmarker5col = {1,0,0};
			frontmarker5.shape = BEACONSHAPE_DIFFUSE;
			frontmarker5.pos = &frontmarker5pos;
			frontmarker5.col = &frontmarker5col;
			frontmarker5.size = 0.7;
			frontmarker5.falloff = 0.4;
			frontmarker5.period = 0;
			frontmarker5.duration = 1;
			frontmarker5.tofs = 0;
			frontmarker5.active = true; 
			AddBeacon (&frontmarker5);
			//Right
			static BEACONLIGHTSPEC frontmarker6; 
			static VECTOR3 frontmarker6pos = {6.5, -110.1, 444.9};
			static VECTOR3 frontmarker6col = {1,0,0};
			frontmarker6.shape = BEACONSHAPE_DIFFUSE;
			frontmarker6.pos = &frontmarker6pos;
			frontmarker6.col = &frontmarker6col;
			frontmarker6.size = 0.7;
			frontmarker6.falloff = 0.4;
			frontmarker6.period = 0;
			frontmarker6.duration = 1;
			frontmarker6.tofs = 0;
			frontmarker6.active = true; 
			AddBeacon (&frontmarker6);

			//Top of bow


			static BEACONLIGHTSPEC topmarker2; 
			static VECTOR3 topmarker2pos = {0, 218.9, 522.9};
			static VECTOR3 topmarker2col = {1,0,0};
			topmarker2.shape = BEACONSHAPE_DIFFUSE;
			topmarker2.pos = &topmarker2pos;
			topmarker2.col = &topmarker2col;
			topmarker2.size = 1.0;
			topmarker2.falloff = 0.4;
			topmarker2.period = 0;
			topmarker2.duration = 1;
			topmarker2.tofs = 0;
			topmarker2.active = true; 
			AddBeacon (&topmarker2);

			//Side antennae
			//Port
			static BEACONLIGHTSPEC sidemarker1; 
			static VECTOR3 sidemarker1pos = {-92.5, 56.4, 446.9};
			static VECTOR3 sidemarker1col = {1,1,1};
			sidemarker1.shape = BEACONSHAPE_DIFFUSE;
			sidemarker1.pos = &sidemarker1pos;
			sidemarker1.col = &sidemarker1col;
			sidemarker1.size = 0.7;
			sidemarker1.falloff = 0.4;
			sidemarker1.period = 0;
			sidemarker1.duration = 1;
			sidemarker1.tofs = 0;
			sidemarker1.active = true; 
			AddBeacon (&sidemarker1);

			//Port
			static BEACONLIGHTSPEC sidemarker2; 
			static VECTOR3 sidemarker2pos = {92.5, 56.4, 446.9};
			static VECTOR3 sidemarker2col = {1,1,1};
			sidemarker2.shape = BEACONSHAPE_DIFFUSE;
			sidemarker2.pos = &sidemarker2pos;
			sidemarker2.col = &sidemarker2col;
			sidemarker2.size = 0.7;
			sidemarker2.falloff = 0.4;
			sidemarker2.period = 0;
			sidemarker2.duration = 1;
			sidemarker2.tofs = 0;
			sidemarker2.active = true; 
			AddBeacon (&sidemarker2);


			

	


	//**Camera Offset**
    SetCameraOffset (_V(0,101.08,1874.62));

    //**Add the mesh**
    AddMesh("Agamemnon"); 


}

/*
void Agamemnon::SetUMMUAirlockPos(void)
{
		int AirlockStatus=Crew.GetAirlockDoorState();
		Crew.SetActiveDockForTransfer(iActiveDockNumber);

		switch(iActiveDockNumber)
		{

		case 0:
			Crew.DefineAirLockShape(AirLockStatus,-5,5,37.5,47.5,479.8,489.8);
			Crew.SetMembersPosRotOnEVA(_V(0,-42.5,486.8),_V(0,0,0));
			break;

		}
} */

// --------------------------------------------------------------
// Write status to scenario file
// --------------------------------------------------------------
void Agamemnon::clbkSaveState (FILEHANDLE scn)
{
	char cbuf[256];
	// default vessel parameters
	VESSEL2::clbkSaveState (scn);
	// Save all UMMU member to scenario
	Crew.SaveAllMembersInOrbiterScenarios(scn);
}

// --------------------------------------------------------------
// Read status from scenario file
// --------------------------------------------------------------
void Agamemnon::clbkLoadStateEx (FILEHANDLE scn, void *vs)
{
char *line;
while (oapiReadScenario_nextline (scn, line))
{
// UMMU, Load all saved member from scenario.
if(Crew.LoadAllMembersFromOrbiterScenario(line)==TRUE)
continue;
ParseScenarioLineEx (line, vs);// unrecognised option - pass to Orbiter's generic parser
}
}




// THIS IS FOR ADDING CREW SEE PDF doc "Allow user to add crew to your ship
// without scenery editor"
bool UMmuCrewAddCallback(void *id, char *str, void *data)
	{
		if(strlen(str)<2||strlen(str)>38)
		return false;
		char *cPtr=(char*)data;if(*cPtr==2){*cPtr=3;strcpy(cPtr+2,str);}
		else if(*cPtr==4){*cPtr=5;strcpy(cPtr+42,str);}
		else if(*cPtr==6){*cPtr=7;strcpy(cPtr+82,str);}return true;
	}
void Agamemnon::AddUMmuToVessel(BOOL bStartAdding)
{
	if(bStartAdding==FALSE&&cAddUMmuToVessel[0]==0)
	return;
	if(bStartAdding==TRUE){
	int salut=sizeof(cAddUMmuToVessel);
	memset(cAddUMmuToVessel,0,sizeof(cAddUMmuToVessel));
	cAddUMmuToVessel[0]=1;
	}
	else if(cAddUMmuToVessel[0]==1){
	cAddUMmuToVessel[0]=2;
	oapiOpenInputBox ("Enter new crew's name (or escape)",UMmuCrewAddCallback,0,30,(void*)cAddUMmuToVessel);
	}
	else if(cAddUMmuToVessel[0]==3){
	cAddUMmuToVessel[0]=4;
	oapiOpenInputBox ("Enter crew's age",UMmuCrewAddCallback,0,30,(void*)cAddUMmuToVessel);
	}
	else if(cAddUMmuToVessel[0]==5){
	cAddUMmuToVessel[0]=6;
	oapiOpenInputBox ("Enter function (Capt,Sec,Vip,Sci,Doc,Tech,Crew,Pax)",UMmuCrewAddCallback,0,30,(void*)cAddUMmuToVessel);
	}
	else if(cAddUMmuToVessel[0]==7){
	cAddUMmuToVessel[0]=0;
	int Age=max(5,min(100,atoi(&cAddUMmuToVessel[42])));
	if(Crew.AddCrewMember(&cAddUMmuToVessel[2],Age,70,70,&cAddUMmuToVessel[82])==TRUE){
	sprintf(SendHudMessage(),"Crew \"%s\" aged %i added to vessel",&cAddUMmuToVessel[2],Age);
	}
	else{
	strcpy(SendHudMessage(),"ERROR: Crew not added (vessel full?)");
	}
	}
}

void Agamemnon::clbkPreStep(double simt, double simdt, double mjd)
{
	

}
void Agamemnon::clbkPostStep(double simt, double simdt, double mjd)
{



	//-----------------------------------------------------
	//Gravity Wheel Animation
	//-----------------------------------------------------
	if (gravwheel_run == 1) //Test - if wheel is supposed to be running, then execute animation based on variable params
	{
		if (gravwheel_speed < 0.040) gravwheel_speed = gravwheel_speed + 0.00001;

		double da = oapiGetSimStep() * gravwheel_speed; 
				

		if (gravwheel_proc >= 1) gravwheel_proc = 0; //Check animation state.  If complete, reset (keep wheel running)
		gravwheel_proc = gravwheel_proc + da; //Increase animation state variable for sim step based on defined wheel speed

		SetAnimation (anim_grav, gravwheel_proc); //Set animation state based on animation state variable value
		
	}
	
	if (gravwheel_run == 0)  //Test - if wheel is supposed to stop, then set state based on animation variable params
	{
		if (gravwheel_speed > 0) gravwheel_speed = gravwheel_speed - 0.00001; 
		double da = oapiGetSimStep() * gravwheel_speed;
		gravwheel_proc = gravwheel_proc + da;
		SetAnimation (anim_grav, gravwheel_proc);
	}




	// The “Add Mmu without scenario editor” process see PDF doc
	AddUMmuToVessel();

	

	//-----------------------------------------------------
	//Detection of UMMU Event
	//-----------------------------------------------------
	int ReturnCode=Crew.ProcessUniversalMMu();
	switch(ReturnCode)
	{
		case UMMU_TRANSFERED_TO_OUR_SHIP:
			sprintf(SendHudMessage(),"%s \"%s\" aged %i was transfered to our ship",
			Crew.GetCrewMiscIdByName(Crew.GetLastEnteredCrewName()),Crew.GetLastEnteredCrewName()
			,Crew.GetCrewAgeByName(Crew.GetLastEnteredCrewName()));
		break;
		case UMMU_RETURNED_TO_OUR_SHIP:
			sprintf(SendHudMessage(),"%s \"%s\" aged %i entered into our ship",
			Crew.GetCrewMiscIdByName(Crew.GetLastEnteredCrewName()),
			Crew.GetLastEnteredCrewName(),Crew.GetCrewAgeByName(Crew.GetLastEnteredCrewName()));
		break;
	}
	
	//Warn user if UMMU 2.0 not installed or outdated
	Crew.WarnUserUMMUNotInstalled("Agamemnon");

}



int Agamemnon::clbkConsumeBufferedKey (DWORD key, bool down, char *kstate)
{
	if (!down) return 0; // only process keydown events	

	if(key==OAPI_KEY_M&&!KEYMOD_SHIFT(kstate)&&!KEYMOD_CONTROL (kstate))
		{
			AddUMmuToVessel(TRUE);
			return 1; 
		}

	
	
		// Ummu Key "E" perform the EVA of the selected member
		//
		if(key==OAPI_KEY_E&&!KEYMOD_SHIFT(kstate)&&!KEYMOD_CONTROL (kstate))
		{
			// PERFORM THE EVA
			int Returned=Crew.EvaCrewMember(Crew.GetCrewNameBySlotNumber(iSelectedUmmuMember));
			// we provide feedback to user (You can display a message on panel or wathewer)
			// here below all the return code possible:
			switch(Returned)
			{
			case TRANSFER_TO_DOCKED_SHIP_OK:
				sprintf(SendHudMessage(),"Transfer to docked ship Ok - %s transfered",
				Crew.GetLastEvaedCrewName());iSelectedUmmuMember=0;
			break;
			case EVA_OK:
				sprintf(SendHudMessage(),"EVA OK - %s left the ship",
				Crew.GetLastEvaedCrewName());iSelectedUmmuMember=0;
			break;
			case ERROR_NO_ONE_ON_BOARD:
				strcpy(SendHudMessage(),"Error, no one on board, unable to EVA");
			break;
			case ERROR_AIRLOCK_CLOSED:
				strcpy(SendHudMessage(),"Error, airlock is closed, unable to EVA");
			break;
			case ERROR_DOCKED_SHIP_HAVE_AIRLOCK_CLOSED:
				strcpy(SendHudMessage(),"Error, docked ship's airlock is closed, unable to transfer");
			break;
			case ERROR_DOCKED_SHIP_IS_FULL:
				strcpy(SendHudMessage(),"Error, docked ship is already full transfer failed");
			break;
			case ERROR_CREW_MEMBER_NOT_FOUND:
				strcpy(SendHudMessage(),"Error, no crew by this name in ship");
			break;
			case ERROR_DOCKEDSHIP_DONOT_USE_UMMU:
				strcpy(SendHudMessage(),"Error, docked ship do not use UMmu 2.0, ask author to add it");
			break;
			case ERROR_MISC_ERROR_EVAFAILED:
				strcpy(SendHudMessage(),"Misc error with UMMU install it again");
			break;
			}	
		return TRUE;
		}
		//---------------------------------------------------------------------------
		// Ummu	Key "1" Select next member This is just internal to the demo
		// you may do your own selection system by panel button, name etc etc
		if(key==OAPI_KEY_1&&!KEYMOD_SHIFT(kstate)&&!KEYMOD_CONTROL (kstate))
		{
			// we test there is someone aboard
			if(Crew.GetCrewTotalNumber()==0)
			{
				strcpy(SendHudMessage(),"Sorry no one aboard unable to select");
				return 1;
			}
			if(iSelectedUmmuMember>0)
				iSelectedUmmuMember--;
				char * Name=Crew.GetCrewNameBySlotNumber(iSelectedUmmuMember);
				sprintf(SendHudMessage(),"Slot %i %s \"%s\" aged %i Selected for EVA or Transfer"
				", please press \"E\" to EVA",iSelectedUmmuMember,
				Crew.GetCrewMiscIdBySlotNumber(iSelectedUmmuMember),Name,
				Crew.GetCrewAgeBySlotNumber(iSelectedUmmuMember));
				return 1;
		}
		//---------------------------------------------------------------------------
		// Ummu Key "2" Select previous member This is just internal to the demo
		// you may do your own selection system by panel button
		if(key==OAPI_KEY_2&&!KEYMOD_SHIFT(kstate)&&!KEYMOD_CONTROL (kstate))
		{
			// we test there is someone aboard
			if(Crew.GetCrewTotalNumber()==0)
			{
				strcpy(SendHudMessage(),"Sorry no one aboard unable to select");
				return 1;
			}
		// we test that we select existing member
		if(iSelectedUmmuMember<Crew.GetCrewTotalNumber()-1)
			iSelectedUmmuMember++;
			char * Name=Crew.GetCrewNameBySlotNumber(iSelectedUmmuMember);
			sprintf(SendHudMessage(),"Slot %i %s \"%s\" aged %i Selected for EVA or Transfer, please press \"E\" to EVA",
			iSelectedUmmuMember,Crew.GetCrewMiscIdBySlotNumber(iSelectedUmmuMember),
			Name,Crew.GetCrewAgeBySlotNumber(iSelectedUmmuMember));
			return 1;
		}

/*		Current configuration sets the airlock door state to always be open when the dock is active.
		This is to simulate that the bay is 'open' and crew enter and leave ships freely.  
		So this section currently commented out.  
		//---------------------------------------------------------------------------
		// Ummu Key "A" Switch the virtual UMMU airlock door on/off
		if(key==OAPI_KEY_A&&!KEYMOD_SHIFT(kstate)&&!KEYMOD_CONTROL (kstate))
		{
			// switch state
			Crew.SetAirlockDoorState(!Crew.GetAirlockDoorState());
			// display state
			if(Crew.GetAirlockDoorState()==TRUE)
				strcpy(SendHudMessage(),"Airlock is now open");
			else
				strcpy(SendHudMessage(),"Airlock is now closed");
			return 1;
		}

*/


		//---------------------------------------------------------------------------
		// Get some infos Name of ship and total soul aboard
		if(key==OAPI_KEY_S)
		{		
			sprintf(SendHudMessage(),"%i souls aboard ship %s",
			Crew.GetCrewTotalNumber(),GetName());
			return 1;
		} 
/*
		// change active dock
		if(key==OAPI_KEY_1&&KEYMOD_SHIFT(kstate)&&!KEYMOD_CONTROL (kstate))
		{
			if(iActiveDockNumber > 0)
			iActiveDockNumber--;
            iActiveDockNumberForHud = iActiveDockNumber + 1;
			 sprintf(SendHudMessage(),"Active dock number changed to: %i",iActiveDockNumberForHud);
			SetUMMUAirlockPos();
			Crew.SetAirlockDoorState(TRUE);

			return 1;
		}
		//---------------------------------------------------------------------------
		// change active dock
		if(key==OAPI_KEY_2&&KEYMOD_SHIFT(kstate)&&!KEYMOD_CONTROL (kstate))
		{
			if(iActiveDockNumber < 0)
			iActiveDockNumber++;
            iActiveDockNumberForHud = iActiveDockNumber + 1;
			 sprintf(SendHudMessage(),"Active dock number changed to: %i",iActiveDockNumberForHud);
			SetUMMUAirlockPos();
			Crew.SetAirlockDoorState(TRUE);
			return 1;
		} */

		// Start/Stop the gravity wheel
		if(key==OAPI_KEY_K&&!KEYMOD_SHIFT(kstate)&&!KEYMOD_CONTROL (kstate))
		{
			if (gravwheel_run == 0)
			{
				gravwheel_run = 1;
				PlayVesselWave3(MyID,MYGRAVSTARTSOUND); 
				sprintf(SendHudMessage(), "Initiating Crew Module Rotation");
				return 1;
			}

			if (gravwheel_run == 1)
			{
				gravwheel_run = 0;
				PlayVesselWave3(MyID,MYGRAVSTOPSOUND);
				sprintf(SendHudMessage(), "Stopping Crew Module Rotation");
				return 1;
			}
		}

	
	return 0;
}


bool Agamemnon::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp)
{
	// draw the default HUD
	VESSEL3::clbkDrawHUD (mode, hps, skp);

	// UMmu display messages
	if(dHudMessageDelay>0)
	{
		skp->Text(5,hps->H/60*15,cUmmuHudDisplay,strlen(cUmmuHudDisplay));
	//	dHudMessageDelay-=oapiGetSimStep();
	//	if(dHudMessageDelay<0)
	//		dHudMessageDelay=0;
	}
    return true; 
}

DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int flightmodel) 
{ 
    return new Agamemnon (hvessel, flightmodel);
}



DLLCLBK void ovcExit (VESSEL *vessel) 
{ 
    if (vessel) delete (Agamemnon*)vessel; 
}

void Agamemnon::clbkPostCreation (void)
{
	MyID=ConnectToOrbiterSoundDLL3(GetHandle());

	RequestLoadVesselWave3(MyID,MYGRAVSTARTSOUND,"Sound\\Agamemnon\\GravStart.wav",INTERNAL_ONLY);
	RequestLoadVesselWave3(MyID,MYGRAVSTOPSOUND,"Sound\\Agamemnon\\GravStop.wav",INTERNAL_ONLY);

}