



// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// PLEASE READ PDF DOC IN "ORBITER/DOC/UMMU_SDK" IT CONTAIN A COMPLETE WALKTHROUGHT
// TO ADD UMMU TO YOUR SHIP AND SEVERAL USEFULL TIPS AND CODE SNIPSET.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!





////////////////////////////////////////////////////////////////////////////////////
// UNIVERSAL MMU SDK 2.0 ©Daniel Polli aka Dansteph see http://orbiter.dansteph.com
// **********************
//
// WHAT'S NEW IN VERSION 2.0
//--------------------------
//
// - Fixed an odd bug that caused CTD in some scenario with more than 20 vessels
//   I STRONGLY ADVISE TO COMPILE AGAIN YOUR ADDON AND RELEASE A NEW VERSION. THANKS !
// - UMMU 2.0 is now UCGO compatible, mean an UMMU can pack/unpack UCGO cargo see http://orbiter.dansteph.com
//   UMMU is also distributed with UCGO so if you want both addon features you have only one
//   link to give: UCGO.
// - Reworked movement code
// - Now can wear off spacesuit and breath in UCGO bases or permanent area (See F10 utility in UMMU)
// - See function "WarnUserUMMUNotInstalled()" for an easy way to warn your users if UMMU is not installed
//   OR if it's outdated.
// - New action area function allow you to catch UMMU action key inside area around your vessel
//   to trigger several things: door opening, failure repair etc etc. See "DeclareActionArea()"
// - By defaut UMMU eva now with "standard" spacesuit if you want to use your own mesh please 
//	 read the pdf doc as you'll need now to provide also a "flightsuit" mesh.
// - Changed the Id/function/meshname tables for a flexible and editable one, see PDF doc.
//
// INTRODUCTION:
// -------------
// Welcome to Universal Ummu SDK. Ummu is a sdk that will allow your addon to have
// a crew with full EVA capacity in about 1 hours. See how work DGIV for an example
// of all features available, you can do exactly the same and even more.
// 
// HOW TO ADD UMMU TO YOUR ADDON ?
// *******************************
// This SDK is pretty simple to use, please read the PDF doc in Orbiter/Doc/UMMu_SDK
// it contain a complete step by step tutorial to help you integrate UMMU in your addon.
// For more informations about the functions please look below.
//
// LICENSE & COPYRIGHT
//
// You must NOT redistribute this SDK. Instead link on my website (this to ensure that no 
// outdated version are available everywhere)
// You must NOT sold this SDK or make any money by using this SDK
// UMmu SDK ©Daniel Polli 2007-2009
///////////////////////////////////////////////////////////////////////////

// warn if forgetting to add UMmuSDK.lib file
#pragma comment(lib,"UMmuSDK.lib")


#ifndef __UMMU20_H
#define __UMMU20_H

//-------------------------------------------------
//EvaCrewMember() function Return Code
#define TRANSFER_TO_DOCKED_SHIP_OK				2
#define EVA_OK									1
#define ERROR_NO_ONE_ON_BOARD					0	
#define ERROR_AIRLOCK_CLOSED					-1
#define ERROR_DOCKED_SHIP_HAVE_AIRLOCK_CLOSED	-2
#define ERROR_DOCKED_SHIP_IS_FULL				-3	
#define ERROR_CREW_MEMBER_NOT_FOUND				-4  
#define ERROR_DOCKEDSHIP_DONOT_USE_UMMU			-5	
#define ERROR_MISC_ERROR_EVAFAILED				-6
//--------------------------------------------------

//--------------------------------------------------
// ProcessUniversalMMu() function Return Code
#define UMMU_RETURNED_TO_OUR_SHIP				1	
#define UMMU_TRANSFERED_TO_OUR_SHIP				2
//--------------------------------------------------

//--------------------------------------------------
// By default UMM 2.0 use the misc id table below to 
// choose the UMMU mesh to eva. See PDF doc or README.txt
// in folder config/UMMUIdConfig about those Id/name/meshes table.
// those tables are filled at runtime. You must check if UmmuMiscID
// is empty or look "iNbrOfIdConfigLoaded" below before using them, 
// there is 255 ID possible but only a few are filled by default 
// at runtime (during init).
#define NUMBER_OF_MMU_TYPE	255
// tables of Id
extern char UmmuMiscID[NUMBER_OF_MMU_TYPE][5];		    // example: "Capt" "Sci"
// table of corresponding mesh to use with UmmuMiscID
extern char UmmuMeshUsed[NUMBER_OF_MMU_TYPE][25];	    // example: "UMmu/UMmuP" "UMmu/UMmuSci"
// Name corresponding to Misc ID
extern char UmmuFunctionName[NUMBER_OF_MMU_TYPE][25];   // example: "Captain" "Scientific"
extern int iNbrOfIdConfigLoaded;						// the number of Id loaded

//--------------------------------------------------
// Crew structure definition
typedef struct {
	char Name[40];
	int Age;
	int Pulse;
	double Weight;
	char MiscId[5];
}CREWMEMBERDEF;
//--------------------------------------------------


//----------------------------------------------------------
// UMMU 2.0 CLASS READ THIS FOR FUNCTION LIST AND MORE DETAILS
class UMMUCREWMANAGMENT{
public:

	//INIT!  ALL THOSE FUNCTIONS ARE REQUIRED
	int InitUmmu(OBJHANDLE Handle);						// return 1=ok -999= Ummu is not installed in Orbiter or outdated -1=misc error.
	BOOL DefineAirLockShape(BOOL OpenState,float MinX, float MaxX, float MinY, float MaxY,float MinZ, float MaxZ); //return TRUE if ok
	BOOL SetMembersPosRotOnEVA(VECTOR3 Pos,VECTOR3 Rot);	// set member position and rotation when EVA. Return TRUE of ok
	BOOL SaveAllMembersInOrbiterScenarios(FILEHANDLE scn);	// to be called in clbkSaveState (see example)
	BOOL LoadAllMembersFromOrbiterScenario(char *line);		// to be called in clbkLoadStateEx (see example)
	BOOL SetMaxSeatAvailableInShip(int MaxSeat);			// by default 8, can be set from 1 to 100, set it after init and before adding any crew. return TRUE if ok.

	// RUNTIME
	int EvaCrewMember(char*CrewName);		// EVA a Crew Member see "EvaCrewMember() function Return Code" above for returned value.
	int EjectCrewMember(char*CrewName);		// Eject a crew member (require "SetEjectPositionOrientationSpeed" to have been set or will fail) (same return code than EvaCrewMember())
	int ProcessUniversalMMu();				// Detect ingress of MMu, return 0 if no action else see "ProcessUniversalMMu() function Return Code" above.

	// CREW MANAGMENT
	BOOL AddCrewMember(char*CrewName,int CrewAge,int CrewPulse,int CrewWeight,char* MiscId=NULL);	// crew list and ship's weight are updatd automatically (you can disable ship's weight update) (a-z A-Z 0-9 characters only)

	// COMPATIBILITY
	void WarnUserUMMUNotInstalled(char* YourAddonName);	//Put this in clbkPostStep or clbkPreStep and it will automatically warn the user
														// if UMMU is not installed or outdated (see pdf doc) 
	float GetUserUMmuVersion(void);			// return the UMmu version installed in user's orbiter directory, return -1.0 if not installed 
											// else return floating point number (ie: 2.0 for version "2.0" etc etc.)
											// you may want to check a particular version in case future version have new functions.



	////////////////////////////////////////////////////////////////////
	// THAT ABOVE IS BASICALLY ALL YOU NEED - SEE BELOW IF YOU WANT MORE
	////////////////////////////////////////////////////////////////////

	// ADVANCED GET INFO AND CREW MANAGMENT
	int		GetCrewTotalNumber();					// return total men aboard
	int		GetCrewAgeByName(char*CrewName);		// return: -1 on error or age (a-z A-Z 0-9 characters only)
	int		GetCrewWeightByName(char*CrewName);		// return: -1 on error or weight (a-z A-Z 0-9 characters only)
	char*	GetCrewMiscIdByName(char*CrewName);		// return: Empty string on error or MiscID (a-z A-Z 0-9 characters only)
	int		GetCrewPulseByName(char*CrewName);		// return: -1 on error or pulse (a-z A-Z 0-9 characters only)
	int     SetCrewMemberPulseByName(char*CrewName,int NewPulse);// often used to kill a crew (0 pulse) return: -1 on error 1 if success.
	char*	GetCrewNameBySlotNumber(int Slot);		// return: Empty string on error or name
	int		GetCrewAgeBySlotNumber(int Slot);		// return: -1 on error or age
	int		GetCrewPulseBySlotNumber(int Slot);		// return: -1 on error or pulse
	int		GetCrewWeightBySlotNumber(int Slot);	// return: -1 on error or weight
	char*	GetCrewMiscIdBySlotNumber(int Slot);	// return: Empty string on error or MiscID
	char*	GetLastEnteredCrewName(void);			// return: Empty string on error or name of last MMu that entered your ship (EVA or Transfer)
	char*	GetLastEvaedCrewName(void);				// return: Empty string or error or name of last member that performed an EVA or a transfer
	int     SetCrewMemberPulseBySlotNumber(int Slot,int NewPulse);// often used to kill a crew (0 pulse) return: -1 on error 1 if success.

	//ADVANCED FUNCTIONS - CONFIGURATION
	BOOL SetAirlockDoorState(BOOL AirlockState);					// FALSE=Closed (no EVA or transfer) TRUE=Open
	BOOL GetAirlockDoorState(void);									// FALSE=Closed (no EVA or transfer) TRUE=Open
	int RemoveCrewMember(char*CrewName);							// Update of list is made automatically when eva or reenter no really need for this one. (a-z A-Z 0-9 characters only)
	BOOL SetCrewWeightUpdateShipWeightAutomatically(BOOL Update);	// by default TRUE. WARNING: this will fail if there is any crew aboard. Call it just after init.
	BOOL SetAlternateMeshToUseForEVASpacesuit(char* MeshName);		// by default UMMU use standard spacesuit (see UmmuMeshUsed) you can define your own mesh here. SEE PDF DOC !!!
	BOOL SetO2ReserveWhenEvaing(int O2Reserve);						// by default 100 about 1h30mn reserve. Call this before EVA or after initialisation
	BOOL SetO2ReserveWhenEjecting(int O2EjectReserve);				// by default 1000 about 20h reserve. Call this before EJECTION or after initialisation
	BOOL SetEjectPosRotRelSpeed(VECTOR3 Pos,VECTOR3 Rot,VECTOR3 RVel); //by default eject at 2 m/s vertical from midle of ship, even docked crew will be ejected.
	char* GetObjNameOfLastEvaedCrew();								// return empty string on error or complete name of Orbiter object name of last EVAed or Ejected crew. (eg: "UMmu_Capt-Peter_John")
	OBJHANDLE GetObjHandleOfLastEVACrew();							// Return NULL on error or handle of last EVAED or EJECTED member. usefull if you want to control the object after creation (add thruster to it after ejection for example) DANGER ! Always check  the pointer before use (NULL pointer) 
	BOOL SetActiveDockForTransfer(int DockNum);						// Set the active vessel dock for transfer. By default Ummu will check the dock 0 and send the Ummu
																	// to the ship docked to this dock if there is one, else it will do a normal eva by the normal way.
																	// you can choose a different Dock number with this function. (usefull if your vessel have more than one dock.
																	// NOTE: you can disable tranfer and always force EVA by setting DockNum to -1.
																	// Return TRUE if active dock number changed otherwise FALSE.

	// ADVANCED UMMU 2.0 "ACTION AREA"
	// -------------------------------
	// You can define up to 30 "action area" around your vessel and get if a UMMU
	// pressed the "action key" inside those area, this can serve multi-purpose:
	// repairing something, opening a door, set a light on, activate a system etc etc.
	// When a UMMU press action key in those area, user inside UMMU will hear the 
	// defined sound or/and see the feedback text and "DetectActionaAreaActivated()" 
	// will return you the area ID that was activated. Charge for you to execute the 
	// action planned. See source code example in Doc/Ummu_sdk.
	// IMPORTANT NOTE: Don't forget to give clear explaination/feedback to user about 
	// where are the active area and what he should do exactly. "go repair the engine" 
	// would be probably frustrating if the user take 1/2 hour to find the correct area.

	// DeclareActionArea()
	// Id			= Any unique ID number you want per area, this number will be returned to you by the function
	//				  "DetectActionAreaActivated" if a UMMU pressed the action key inside the area.
	// LocalPos		= Center of action area in local coordinate (meter). Example: _V(0,5,10) (5m up, 10m above center of your ship)
	// Radius		= Radius of action area in meter, don't make it too small else user will have pain to find it.
	// OnOffState	= If this area is active or not. You can change it's state at runtime with "SetActionAreaState"
	// WavFilename	= Feedback Wav sound that will be played if UMMU press action key inside this area.
	//				  Example: "MyAddonName\\MyRepairedSound.wav" you should put the wav "MyRepairedSound.wav"
	//				  in folder "Sound\\MyAddonName". If you enter NULL no sound will be played.
	// FeedbackText	= Text that will be displayed in UMMU VC if UMMU press action key inside this area.
	//				  enter NULL if you don't want a text. Max characters allowed: 50.
	// RETURN:		  TRUE if area declared, FALSE otherwise. (Can be the case if UMMU 2.0 or higher 
	//				  is not installed in user's Orbiter, if init was not done (see "InitUmmu" above) if 
	//				  the limit of 30 areas per vessel reached or radius less than 1 meter)
	//
	// IMPORTANT NOTE: Do NOT declare areas each timestep. If you need to edit one area see "Set.." function below.
	//				   If two area overlapp only the first declared will be detected if UMMU is inside both.
	// DEFAULT SOUND: UMMU 2.0 come with a set of predefined sound that you can use instead of your own
	//				  in this case enter the filename exactly as shown below:
	//				  "action_door.wav"     = UMMU will play a "door activated" voice when action key pressed
	//				  "action_doorOpen.wav" = UMMU will play a "door opening" voice when action key pressed
	//				  "action_doorClose.wav"= UMMU will play a "door closing" voice when action key pressed
	//				  "action_repaired.wav" = UMMU will play "system repaired" voice when action key pressed
	//				  "action_activated.wav"= UMMU will play "system activated" voice  when action key pressed
	//				  "action_click.wav"	= UMMU will play a simple click when action key pressed
	//				  "action_clickOk.wav"	= UMMU will play a two tone sound "tidoo" when action key pressed
	// VOICE SOUND NOTE: You can use text-to-speech program to make wav voices, search google about "text to speech"
	BOOL DeclareActionArea(int Id,VECTOR3 LocalPos,double Radius,BOOL OnOffState,char* WavFilename,char *FeedbackText);

	// This below are function that allow you to edit action area during runtime.
	// Those function will return FALSE if the Id doesn't correspond to a previously declared area.
	BOOL SetActionAreaState(int Id,BOOL OnOffState);				// If OnOffState=FALSE this area will be inactive
	BOOL SetActionAreaPos(int Id,VECTOR3 LocalPos,double Radius);	// this relocate the area
	BOOL SetActionAreaWav(int Id,char* WavFilename);				// this change the wav that will be played. Enter NULL if you want no sound
	BOOL SetWavPlayStyle(int Id,BOOL RadioState);					// If TRUE wav will play as radio sound in external view and in space, 
																	// if FALSE (Default) sound will only play in UMMU cockpit view.
	BOOL SetActionAreaText(int Id,char* FeedbackText);				// this change the text that will be displayed. Enter NULL if you want no text
	BOOL DeleteActionArea(int Id);									// this delete one action area previoulsy declared

	// DetectActionaAreaActivated()
	// this is the function that will tell you if one UMMU pressed the "action"key
	// inside one of your area. 
	// It should be called inside "clbkPostStep" or "clbkPreStep" orbiter's callback and each timestep.
	// RETURN: -1 if nothing happened or no area defined else the Id of the area that 
	// was activated by a UMMU (action key pressed)
	// This function take almost no time, only one test (if) is made per frame
	//
	// IMPORTANT NOTE: NOT calling it each timestep if you have active(s) area is not a good idea,
	// user would still have sound or text feedback inside UMMU but nothing would happen.
	// if you want to disable all your areas see DeleteActionArea() or SetActionAreaState() above.
	int DetectActionAreaActivated(void);



private:
	CREWMEMBERDEF CrewMember[102];
	int			MMuInitReady;
	int			TotalCrewNumber;
	int			MaxSeatAvailable;
	double		TotalWeight;
	int			LoadedCrew;
	BOOL		CrewWeightUpdateShipWeight;
	BOOL		AirlockState;
	int			EVAO2Reserve;
	int			EjectO2Reserve;
	char		AlternateMeshToUse[200];
	VECTOR3		EvaPosition;
	VECTOR3		EvaRotation;
	VECTOR3		EjectPosition;
	VECTOR3		EjectRotation;
	VECTOR3		EjectRelSpeed;
	char		EmptyStringToReturn[2];
	char		LastEvaedCrewName[50];
	char		LastReturnedCrewName[50];
	OBJHANDLE   VesselHandleUmmu;
	char		ObjNameOfLastEVAEDCrew[200];		 
	OBJHANDLE	hObjHandleLastEvaed;
	int			MyUMmuClass;
	void		*ptrUmmuStruct;
	int			iNbrActionAreaDeclared;
	int			iNbrActionAreaActive;
	double		dTimerUMMUNotInstalled;
	BOOL		bUMMUIsOutdated;
	void        ChooseMeshBeforeEVA(char *MiscId);
};

#endif //__UMMU20_H