///////////////////////////////////////////////////////////////////////////////////////////////
// UNIVERSAL CARGO SDK  2.0  (UCGO) ©Daniel Polli aka Dansteph see http://orbiter.dansteph.com
// ********************************
//
// Welcome in UCGO 2.0 SDK for vessel !
//
// SEE PDF DOC AND SHUTTLEPB EXAMPLE FIRST TO MAKE 
// YOUR VESSEL UCGO COMPATIBLE !
//
// FUNCTION LIST AND REFERENCE
// ----------------------------
// This header is THE place to look if you want to know more about 
// a function (parameter, call, return code) or see all the functions 
// that are not mentionned in the pdf doc. It's the reference of the SDK.
//
// TIPS: In Msvc++ right click on any Ucgo function and select "go to definition"
//       This will open this header on the function line so you can see the comments.
//
///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __UCGO_SDK
#define __UCGO_SDK
#pragma comment(lib,"UCGOCargoSDK.lib")


//----------------------------------------------------------
// UCGO CLASS READ THIS FOR FUNCTIONS LIST AND MORE DETAILS
//----------------------------------------------------------
class UCGO{
public:
	UCGO(void){};
	//--------------------------------------------------------------
	// MANDATORY
	// Just put those in correct Orbiter callback place 
	//--------------------------------------------------------------
	BOOL Init(OBJHANDLE Vessel);					// must be called in clbSetClassCap AFTER SetEmptyMass(). return: TRUE=okay FALSE= UCGO not installed
	BOOL LoadCargoFromScenario(char* line);			// must be called in clbkLoadStateEx (see example)
	void SaveCargoToScenario(FILEHANDLE scn);		// must be called in clbkSaveState (se example)
	void SetUcgoVisual(VISHANDLE vis);				// must be called in clbkVisualCreated.


	//--------------------------------------------------------------
	// IMPORTANT FUNCTIONS
	// not mandatory but you must know that those functions exist
	//--------------------------------------------------------------
	// This function will automatically put a warning to user during 30 seconds if UCGO 
	// is not installed. Simply call it from clbkPostStep or clbkPreStep with your addon's 
	// name in parameter. text is: "[Name] is compatible with "Universal Cars & Cargos" (UCGO) addon. Download at www.orbiter.dansteph.com (second remaining)"
	void WarnUserUCGONotInstalled(char* YourAddonName);

	// FUNCTION: UpdateEmptyMass() to call after each "SetEmptyMass()" at run time.
	// UCGO update automatically your vessel's empty mass with cargo mass, problem
	// will occure if you update your vessel mass yourself at run time with SetEmptyMass (wich
	// is the case for multistage vessel for example)
	// The rule is simple: ALWAYS call this function AFTER each call of SetEmptyMass();
	// outside of clbkSetClassCap (after init)
	// UCGO will recalculate all cargo mass and add them to your new empty mass. NEVER call it
	// in another case else mass may go wrong.
	void UpdateEmptyMass(void);

	//--------------------------------------------------------------
	// SETTING
	// Position orientation of slot, grapple distance, release pos etc
	// Note: It's recommended that you call those in clbkSetClassCap
	// just after Init().
	//--------------------------------------------------------------

	// Declare one slot with it's position and orientation 
	// Num = Id of slot from 0 to 39
	// Pos = Position of slot relative to vessel's center
	// Rot = Rotation of slot in degree around x,y,z axe.
	// Example: UCGO->DeclareCargoSlot(0,_V(0.0,10.0,5.0),_V(0,0,0)); 
	void DeclareCargoSlot(int Num,VECTOR3 Pos,VECTOR3 Rot);  

	// Set the overall maximum cargos mass your vessel can carry, 
	// by default 1000000 kg (1000 metric ton, almost unlimited) 
	void SetMaxCargoMassAcceptable(double MaxCargosMassKg);

	// Distance at wich cargo can be grappled
	// DistInMeter = Distance in meter at wich a cargo can be grappled (defaut 50 meter from center of ship)
	void SetGrappleDistance(int DistInMeter);	

	// Place at wich will be placed the cargo when released  -on ground -
	// if you set only the global position they will be ordered by 4 columns 
	// with a 10cm space beetween each cargo.
	// calling this replace the setting of SetSlotReleasePos below.
	void SetGlobalGroundReleasePos(VECTOR3 Pos);

	// if the global release pos is not enough precise you can set a release pos per slot.
	void SetSlotGroundReleasePos(int Num,VECTOR3 Pos);

	// speed at wich the cargo will go relative to ship when released in space
	// by defaut 0.05 (m/s)
	void SetReleaseSpeedInSpace(double Speed); 

	// Set if the grappled payload must increase your vessel's mass or not
	// Param: TRUE Increase your vessel's mass FALSE don't change mass
	// DEFAUT: any grappled/released cargo change your vessel's mass (see UpdateEmptyMass)
	void SetCargoMassChangeVesselMass(BOOL Param);

	//--------------------------------------------------------------
	// RUNTIME
	// Grapple release, set door state etc.
	//--------------------------------------------------------------

	// By defaut slots are always "open" (can grapple/release)
	// but you can manage state in run time with this function
	// a closed slot cannot grapple or release anything.
	// if you set Num=-1 all door state are changed else you can change one slot's door state
	// DoorState= FALSE=closed TRUE=open
	void SetSlotDoorState(BOOL DoorState,int Num=-1);

	// Grapple one cargo, if you don't put a parameter (ie: "GrappleOneCargo();")
	// the lower free slot will be loaded, else you can put the number of one free slot.
	// this will probably be called from clbkConsumeBufferedKey, don't forget to put in
	// documentation the key you use. (by defaut UCGO use "C" I recommande this key)
	// RETURN: 
	// 1 = Ok, cargo grappled 
	// 0 =no cargo in range
	// -1=cargo exceed maximum mass permited for vessel (total cargo mass see SetMaxCargoMassAcceptable)
	// -2=missing parameter in config, mesh not found or slot not declared
	// -3= slot not empty
	// -4= door closed 
	// -5= no slots available (case with no parameters, all slots are full or doors closed)
	int GrappleOneCargo(int Num=-1);

	// Release one cargo, if you don't put a parameter (ie: "ReleaseOneCargo();")
	// the first loaded slot will be released first, else you can put the number of one loaded slot.
	// This will probably be called from clbkConsumeBufferedKey, don't forget to put in
	// documentation the key you use. (by defaut UCGO use "SHIFT+C" I recommande this key)
	// Return: TRUE if cargo was released FALSE else
	BOOL ReleaseOneCargo(int Num=-1);

	// Grapple one cargo but only of a defined type (see below)
	// Common types are: oxygen fuel food water hydrogen gold helium metal rock
	// RETURN: 
	// 1 = Ok, cargo grappled 
	// 0 =no cargo in range
	// -1=cargo exceed maximum mass permited for vessel (total cargo mass see SetMaxCargoMassAcceptable)
	// -2=missing parameter in config, mesh not found or slot not declared
	// -3= slot not empty
	// -4= door closed 
	// -5= no slots available (case with no parameters, all slots are full or doors closed)
	int GrappleOneCargoByType(char *Type,int Num=-1);

	// You might want to delete one loaded cargo for managment purpose, this do nothing appart
	// deleting one cargo loaded in a slot.
	// return TRUE if cargo was deleted FALSE if not (slot not loaded or declared for example)
	BOOL   DeleteLoadedCargo(int Num);

	// this set the cargo mesh visibility as defined in Orbiter SDK. 
	// by defaut loaded cargo mesh have "MESHVIS_EXTERNAL". You can use
	// this function for example to disable cargo display when door closed (optimisation)
	// or to allow them to be visible also in cockpit or vc mode. (case of the forklift cars for example)
	// See SetMeshVisibilityMode() function in orbiter's SDK.
	void SetCargoMeshVisibility(int Num,int iMeshVisibility);


	//--------------------------------------------------------------
	// RUNTIME INFORMATION
	// Get slots, cargos, door state, UCGO information etc.
	//--------------------------------------------------------------

	// to get the state of one slot's door. See SetSlotDoorState above.
	BOOL GetSlotDoorState(int Num);

	// to know is a slot is loaded with a cargo return TRUE if yes FALSE else
	BOOL GetSlotLoaded(int Num);

	// return the total number of slot loaded
	int GetNbrCargoLoaded(void);
	
	// RETURN: "none" if no cargo found at this slot else the description of content of cargo. (30 char maximum)
	// Notice a cargo can contain "eatable" ressource as oxygen or fuel, in this case the description
	// will contain one standard keyword of the following list: oxygen fuel food water hydrogen gold helium metal rock
	char *GetCargoDescriptionString(int Num);

	// return the mass of a loaded cargo slot or 0 if no cargo loaded.
	double GetCargoSlotMass(int Num); 

	// return the total mass of all loaded cargo slot.
	double GetCargoTotalMass(void); 

	// return TRUE if UCGO is installed in user's Orbiter install else FALSE. (see WarnUserUCGONotInstalled())
	BOOL IsUCGOInstalledInOrbiter(void);

	// return the UCGO version installed in user's orbiter
	double GetUCGOVersion(void);


	//--------------------------------------------------------------
	// RUNTIME REFUELLING/RESSOURCES GRABBING
	// Get fuel, oxygen, gold, hydrogen etc. from cargos, stations, factory or mine
	// (new cargos, stations factory or mines can be designed by anyone, 
	// they require only one mesh and one config file)
	//--------------------------------------------------------------

	// if a cargo designed as containing fuel is in the vicinity or aboard (see SetGrappleDistance and GetCargoDescriptionString)
	// the cargo's mass will be decreased by the requested value and destroyed if it's empty.
	// this function search first in cargo in vincinity then in cargo loaded aboard.
	// It's your responsability to increase your ship's tank fuel value. (see "SetPropellantMass" in Orbiter SDK)
	// RETURN: Total fuel Cargo mass "eaten" in kg or -1.0 if no fuel cargo was found aboard or in vincinity.
	double EatCloserFuelCargo(double RequestedFuelMass);

	// if a cargo designed as containing oxygen is in the vicinity or aboard (see SetGrappleDistance and GetCargoDescriptionString)
	// the cargo's mass will be decreased by the requested value and destroyed if it's empty.
	// this function search first in cargo in vincinity then in cargo loaded aboard.
	// It's your responsability to increase your own oxygen tank by the requested value and maybe increase your
	// vessel's empty mass if it's supposed to be still aboard (see SetEmptyMass() of Orbiter SDK and IMPORTANT UpdateEmptyMass() above )
	// RETURN: Total oxygen Cargo mass "eaten" in kg or -1.0 if no oxygen cargo was found aboard or in vincinity.
	double EatCloserOxygenCargo(double RequestedOxygenMass);

	// if a cargo designed as containing the requested item is in the vicinity or aboard (see SetGrappleDistance and GetCargoDescriptionString)
	// the cargo's mass will be decreased by the requested value and destroyed if it's empty.
	// Standard keyword for "eatable" cargo are: "oxygen fuel food water hydrogen gold helium metal rock"
	// It's your responsability to increase your own item value by the returned value and maybe increase your
	// vessel's empty mass if it's supposed to be still aboard (see SetEmptyMass() of Orbiter SDK and IMPORTANT UpdateEmptyMass() above )
	// RETURN: Total item Cargo mass "eaten" in kg or -1.0 if no cargo of this type was found aboard or in vincinity.
	double EatCloserCargoByType(char *keyword,double RequestedMass);

	// the most common usage for this function is to refill your oxygen or fuel tank at UCGO station/mine or factory.
	// their ressources are infinites, they cannot be grappled or destroyed.
	// Standard keyword for such ressources are: "oxygen fuel food water hydrogen gold helium metal rock"
	// Radius is the distance in meter from the center of your ship at which such station will be searched.
	// It's your responsability to increase your own tank by the returned value and maybe increase your
	// vessel's empty mass if it's supposed to be still aboard (see SetEmptyMass() SetPropellantMass() of Orbiter SDK and IMPORTANT UpdateEmptyMass() above )
	// RETURN: Total mass "eaten" in kg or -1.0 if no station mine or factory of this type was found in the given radius.
	double GetRessourceFromStationMineOrFactory(char *keyword,double RequestedMass, double SearchRadius);


	//---------------------------------------------------------------
	// USER HELPER: ADD CARGO TO SHIP WITHOUT NEED OF SCENARIO EDITOR
	// Using scenario editor to add several cargo in Orbiter can be a pain, 
	// those two functions allow to list and select one available cargo in Orbiter 
	// installation and add it in one slot.
	// the cargo's config filename must be in "config\vessels\UCGO\Cargos" folder 
	// to be available in the list.
	//---------------------------------------------------------------

	// Select and return a pointer to a string with the number, description, mass and [config name] of the selected cargo 
	// examples: "2-Oxygen mass:1236kg [Oxygen]", "1-Fuel mass:456kg [Fuel]".
	// (NOTE: The config name is extracted from config filename ie: "UCGO\\Cargos\\CargoOxygen.cfg" would give "CargoOxygen")
	// Each call to this function will return one cargo and restart to first when list is finished. 
	// Allow the user to call this function with one key and display to him the returned string.
	// the next function ScnEditor_AddLastSelectedCargoToSlot() will add the last type selected by this function.
	// RETURN: a pointer to the string as described above or "none" if no cargo are available in 
	// Orbiter directory. (case if UCGO is not installed)
	// NOTE: By defaut the first cargo in list is selected after init.
	char *ScnEditor_SelectNextCargoAvailableOnDisk();

	// Add the last selected cargo on disk (see function above) to one slot or 
	// the first free slot if "-1" is given for "num" parameter.
	// RETURN: TRUE=Cargo added, FALSE: Cargo not added (no free slot)
	BOOL ScnEditor_AddLastSelectedCargoToSlot(int Num=-1);

	// only for advanced user:
	// Add a cargo by giving the config name (example: "CargoBarrelFuel.cfg")
	// this will load a cargo into one slot or the first free slot if no
	// parameter given. If the slot is already loaded cargo will be replaced
	// by the new one.
	// NOTE: Cargo cfg file must be in "config\vessels\UCGO\Cargos\"
	// Return:
	// 1 = Ok, cargo added to slot
	// 0 =bad parameter, no config file found or no slot declared found
	// -1=cargo exceed maximum mass permited for vessel (total cargo mass see SetMaxCargoMassAcceptable)
	// -2=missing parameter in config, mesh not found
	BOOL ScnEditor_AddCargoByConfigName(char* ConfigName,int Num=-1);






//****************************************************************************************************
//** NOTHING FOR USER BELOW ** END OF SDK ** NOTHING FOR USER BELOW ** END OF SDK ** NOTHING FOR USER BELOW ** END OF SDK 
//** NOTHING FOR USER BELOW ** END OF SDK ** NOTHING FOR USER BELOW ** END OF SDK ** NOTHING FOR USER BELOW ** END OF SDK 
//****************************************************************************************************
private:
	typedef struct{
		double	  dDistance;
		double	  dObjectRealMass;
		BOOL	  bIsUnpackableCargo;
		char	  *cConfigFileName;
		OBJHANDLE hObjVesselFound;
		VESSEL	  *hVessel; 
	}SEARCHRESULT;
	typedef struct
	{
		BOOL Declared;
		BOOL Loaded;
		BOOL DoorState;
		VECTOR3 Pos;
		VECTOR3 ReleasePos;
		VECTOR3 Rot;
		char Type[31];
		double Mass;
		char ConfigFilename[255];
		char cForceName[20];
		int Visibility;
	}UCGOSLOT;
	UCGOSLOT UCGOSlot[41];
	BOOL bINIT_OK_UCO_PRESENT;
	double  dUserUGCOVersionInstalled;
	double	dGrappleDistance;
	double	dReleaseSpeedInSpace;
	BOOL	bCargoMassChangeVesselMass;
	int		iNumberOfCargoAboard;
	double  dVesselEmptyMassWithoutCargo;
	double	dTotalMassOfCargoAboard;
	double  dMaxCargosMassKg;
	char	TypeToLoad[31];
	OBJHANDLE hUCGOVessel;
	VESSEL *vUcgo;
	char	cCargoOnDiskFilename[100][100];
	char	cSelectedCargoOnDisk[100];
	char    cReplyForId[100];
	int     iSelectedCargoOnDisk;
	int		iNbrCargoOnDisk;
	double  dTimerUCGONotInstalled;
	void	ResetSlot(int Num);
	void	RotateMesh(int Num,VISHANDLE vis);
	BOOL	LoadCargoBayConfig(char *ConfigName,int Num);
	void	Rotate (int axis, double da,OBJHANDLE hVessel);
	double  EatCloserCargoLoop(char *Type,double RequestedMass);
	OBJHANDLE SearchCargoVesselObjHandle(SEARCHRESULT *Result,char *cTypeSearch=NULL);
	void	RecalculateEmptyMass(void);
	int     iInitPassedSuccesfully;
};
#endif //#ifndef __UCGO_SDK