#define STRICT

#include "..\..\include\orbitersdk.h"
#include "UMmuSDK.h"

// Vessel Parameters
const double EXP_SIZE = 1500; // mean radius in meters
const VECTOR3 EXP_CS = {4327830,2655468,29516712}; //Agamemnon cross section in m^2
const VECTOR3 EXP_PMI = {15.5,22.1,7.7}; //Principal Moments of Inertia, normalized, m^2
const double EXP_EMPTYMASS = 1e6; //empty vessel mass in kg
const double EXP_FUELMASS =  4e7; //max fuel mass in kg
const double EXP_ISP = 5e5; //fuel-specific impulse in m/s
const double EXP_MAXMAINTH = 44.3e7; 
const double EXP_RCSTH = 6000; 

double gravwheel_proc = 0.0;
double gravwheel_speed = 0.0;
double gravwheel_run = 0; 

UINT anim_grav = 0;

// Agamemnon vessel class interface
class Agamemnon: public VESSEL3 { 
public: 
    Agamemnon (OBJHANDLE hObj, int fmodel);  
    ~Agamemnon()  ;
    void clbkSetClassCaps(FILEHANDLE cfg); 
    void DefineAnimations();
	void clbkPreStep (double simt, double simdt, double mjd);
	void clbkPostStep (double simt, double simdt, double mjd);
	int  clbkConsumeBufferedKey (DWORD key, bool down, char *kstate);
	void clbkSaveState (FILEHANDLE scn);
	void clbkLoadStateEx (FILEHANDLE scn, void *vs);
	// void clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, HDC hDC );
    bool clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp);
	// The “Add Mmu without scenery editor” declaration see PDF doc
	char cAddUMmuToVessel[255];
	void AddUMmuToVessel(BOOL bStartAdding=FALSE);
	void clbkPostCreation (void);
	//void SetUMMUAirlockPos(void);
	int AirLockStatus;
	int MyID;

	// UMMU 2.0 DECLARATION

	UMMUCREWMANAGMENT Crew;
	int iActiveDockNumber;
    int iActiveDockNumberForHud;
	int iSelectedUmmuMember;// for the SDK demo, select the member to eva
	// The HUD display method variable
	char cUmmuHudDisplay[255];// UMmu hud char variable
	double dHudMessageDelay;// UMmu hud display delay
	char *SendHudMessage(void);// UMmu hud display function
}; 