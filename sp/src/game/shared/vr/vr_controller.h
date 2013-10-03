#include <math.h>
#include <sixense_utils\interfaces.hpp>

// hydra control modes
enum MotionControlMode_t
{
	TRACK_BOTH_HANDS = 0, 
	TRACK_RHAND_TORSO = 1,
	TRACK_RHAND_TORSO_CUSTOM = 2 // alternate control schemes for right hand 
};

enum MenuControlMode_t
{
	RIGHT_JOYSTICK = 0, 
	RIGHT_HAND_POS = 1,
	BOTH = 2 
};

class MotionTracker
{
 
public:
 
	MotionTracker( void );
	~MotionTracker( void );
	void shutDown( void );

	bool isTrackingWeapon( void );
	bool isTrackingTorso( void );
	
	void beginCalibration();
	void update(VMatrix& torsoMatrix);
		
	void	updateViewmodelOffset(Vector& vmorigin, QAngle& vmangles);		// hooks clientvr	for now, can move directly into viewmodel_shared
	void 	getViewOffset(Vector& offset);
	void	overrideViewOffset(VMatrix& viewMatrix);						// hooks clientvr,	updates the view matrix based on torso tracked offsets
	void	overrideWeaponMatrix(VMatrix& weaponMatrix);					// hooks clientvr	player motion, updates weapon matrix per tracked values
	QAngle	getTorsoAngles();
	void	overrideMovement(Vector& movement);								// hooks clientvr,	allows movement vector to be adjusted to account for tracked torso
	void	overrideJoystickInputs(float& lx, float& ly, float& rx, float& ry);		// in_joystick, allows hydra inputs to apply over others
	void	updateSixenseButtons();											// checks sixense controller for buttons states and emulates keypress events...
	void	getEyeToWeaponOffset(Vector& offset);

	float	getHudPanelAlpha(const Vector& hudPanelForward, const Vector& eyesForward, float fadeFactor);


protected:
	bool	_initialized;
	bool	_calibrate;
	float	_lastCalibrated;
	
	matrix3x4_t _sixenseToWorld;
	matrix3x4_t _eyesToTorsoTracker;
	matrix3x4_t	_torsoCalibration;
	VMatrix		_rhandCalibration;
	
	Vector	_vecBaseToTorso;

	float	_baseEngineYaw;
	float	_prevYawTorso;
	float	_accumulatedYawTorso;

	unsigned int _counter;
 
	matrix3x4_t getTrackedTorso();
	matrix3x4_t getTrackedRightHand(bool includeCalibration=true);
	void		calibrate(VMatrix& torsoMatrix);
	bool		writeDebug();
	

	// sixense related hooks
	bool		_sixenseInitialized;
	MotionControlMode_t _controlMode;
	bool		_strafeModifier;
	bool		_rightBumperPressed;
	Vector		_previousHandPosition;
	QAngle		_previousHandAngle; // for mouse emulation
	bool		_isGuiActive;

	struct		_sixenseAllControllerData *_sixenseControllerData;
	class		sixenseUtils::IButtonStates *_leftButtonStates, *_rightButtonStates;
	class		sixenseUtils::IControllerManager *_controllerManager;

	void		sixenseInitialize();
	void		sixenseUpdate();
	void		sixenseShutdown();
	void		sixenseGuiMouseControl();
	void		sixenseMapKeys();
	void		sixenseMapKeysCustom();
	sixenseControllerData getControllerData(sixenseUtils::IControllerManager::controller_desc controller);
};

MotionTracker* g_MotionTracker();
