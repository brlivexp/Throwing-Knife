/*
 * =============================================================================================== *
 * =============================================================================================== *
 * =============================================================================================== *
 * ================================                               ================================ *
 * ================================           Codectile's         ================================ *
 * ================================         Throwing Knives       ================================ *
 * ================================        -Script Library-       ================================ *
 * ================================        -Version [ALPHA]-      ================================ *
 * ================================                               ================================ *
 * =============================================================================================== *
 * =============================================================================================== *
 * =============================================================================================== *
 */

 /*

*****************************************************************************************************************
 Introduction [PRE-RELEASE]	: This include let you to create some deadly throwing knives for each player, these knives
 			   			can reduce target's health percentage whenever the target gets hit by these knives. The
 			   			reduction in the health percentage of the target varies from a range of 0 to 10.
 			   			This is more a script rather than an include. Sorry for bad optimization.


 			   			NOTE: • This include is still under development and testing.
 			   			      • Uses PlayerAttachedObject slot '0'.
 			   			      
 			   			UPDATE 2: • Improved code.
 			   				  • Improved crosshair.
 			   			 	  • Improved Damage status (0 -100)

 			   			UPDATE 1: • Added crosshair for accuracy.
 			   				  • Improved functions.
 			   				  • Added callback "OnPlayerThrowKnife(playerid)"



Functions: 				• EquipPlayerThrowingKnife(playerid)
					• UnEquipPlayerThrowingKnife(playerid)
					• RemovePlayerThrowingKnife(playerid)
					• GivePlayerThrowingKnife(playerid,amount)
					• LetPlayerThrowKnife(playerid, clienterror[], errorcolor)	



Source Code: 			https://github.com/codectile/Throwing-Knife




Dependencies: 			Streamer Plug-in (Incognito)



Bugs: 					Need to be tested with "two or more" players.



Credits: Incognito - streamer plug-in

*****************************************************************************************************************
*/


/* The following codes are too much fragile, please "do not edit them". */

/* Defines */
#define KNIFE_FINAL_VELOCITY		30
#define KNIFE_INITIAL_VELOCITY		20


/* Variables/Indentifiers Initialization */
new _@TKNF[MAX_PLAYERS];
new knife@_counter[MAX_PLAYERS];
new knife@_movetime[MAX_PLAYERS];
new _@equiped[MAX_PLAYERS];
new bool:_@has[MAX_PLAYERS];
new PlayerText:crosshair[MAX_PLAYERS];


/* Core Function */
stock RemovePlayerThrowingKnife(playerid)
{
	if(IsPlayerConnected(playerid))
	{
	    	if(_@has[playerid] == true)
    		{
			if(IsPlayerAttachedObjectSlotUsed(playerid, 0)) RemovePlayerAttachedObject(playerid, 0);
			knife@_counter = 0;
			_@equiped[playerid]=0;
			_@has[playerid] = false;
		}
		return 1;
	}
	else return 0;
}


stock UnEquipPlayerThrowingKnife(playerid)
{
	if(IsPlayerConnected(playerid))
	{
	    	if(_@has[playerid] == true)
    		{
			if(IsPlayerAttachedObjectSlotUsed(playerid, 0)) RemovePlayerAttachedObject(playerid, 0);
			PlayerTextDrawDestroy(playerid, crosshair[playerid]);
			_@equiped[playerid]=0;
		}
		else SendClientMessage(playerid,-1,"SERVER: You don't have throwing knives.");
		return 1;
	}
	else return 0;
}


stock EquipPlayerThrowingKnife(playerid)
{
	if(IsPlayerConnected(playerid))
	{
	    if(_@equiped[playerid] == 0)
	    {
	    		if(_@has[playerid] == true)
    			{
				if(IsPlayerAttachedObjectSlotUsed(playerid, 0)) RemovePlayerAttachedObject(playerid, 0);
				SetPlayerAttachedObject(playerid, 0, 335, 6, 0.127,-0.017,0.281,-1.50,174.20,-0.50);
				crosshair[playerid]=CreatePlayerTextDraw(playerid, 320.0-15.0, 240.0-70.0, "(  )");
				PlayerTextDrawSetShadow(playerid, crosshair[playerid], 0);
				PlayerTextDrawColor(playerid,crosshair[playerid], 0xFFFFFFFF);
				PlayerTextDrawFont(playerid, crosshair[playerid], 1);
		 		PlayerTextDrawShow(playerid, crosshair[playerid]);
				_@equiped[playerid]=1;
			}
			else SendClientMessage(playerid,-1,"SERVER: You don't have throwing knives.");
		}
		else SendClientMessage(playerid,-1,"SERVER: You have already equipped throwing knives.");
		return 1;
	}
	else return 0;
}


stock GivePlayerThrowingKnife(playerid,amount)
{
	if(IsPlayerConnected(playerid))
	{
	    knife@_counter[playerid]+=amount;
	    return 1;
	}
	else return 0;
}


stock LetPlayerThrowKnife(playerid, clienterror[], errorcolor)
{
	if(_@has[playerid] == true)
	{
		if(_@equiped[playerid] == 1)
		{
			if(knife@_counter[playerid] > 0)
			{

	    		if(!IsValidDynamicObject(_@TKNF[playerid]))
				{
					ApplyAnimation(playerid, "GRENADE", "WEAPON_THROW", 3.5, 0, 1, 1, 0, 500, 1);
					new Float:vecx,Float:vecy,Float:vecz,Float:Angle,Float:vx,Float:vy;
					GetPlayerPos(playerid, vecx, vecy, vecz);
					GetPlayerFacingAngle(playerid,Angle);
					vx=vecx+floatsin(-(Angle+360.0),degrees)*30;
					vy=vecy+floatcos(-(Angle+360.0),degrees)*30;
					_@TKNF[playerid]=CreateDynamicObject(335, vecx,vecy,vecz+0.7,-1.5+180.0, 270.0, Angle+270,GetPlayerVirtualWorld(playerid),GetPlayerInterior(playerid),-1,700.0,1000.0);
					knife@_movetime[playerid]=MoveDynamicObject(_@TKNF[playerid],vx,vy,vecz+0.7,KNIFE_INITIAL_VELOCITY,-1.5+180.0,270.0-360.0, Angle+270);
					SetTimerEx("knife@_track",knife@_movetime[playerid]-90,0,"if",playerid,Angle);
					#if defined OnPlayerThrowKnife
					OnPlayerThrowKnife(playerid);
					#endif
					knife@_counter[playerid]-=1;
				}
				else SendClientMessage(playerid,errorcolor, clienterror);
			}
		}
		else SendClientMessage(playerid,-1,"SERVER: You are not eqipped with throwing knives.");
	}
	else SendClientMessage(playerid,-1,"SERVER: You don't have throwing knives.");
	return 1;
}

forward knife@_track(playerid,Float:Angle);
public knife@_track(playerid,Float:Angle)
{
	StopDynamicObject(_@TKNF[playerid]);
	new Float:vecx,Float:vecy,Float:vecz;
	new Float:rx,Float:ry,Float:rz;
	GetDynamicObjectRot(_@TKNF[playerid], rx, ry,rz);
	GetDynamicObjectPos(_@TKNF[playerid], vecx, vecy,vecz);
	new Float:vx,Float:vy;
	vx=vecx+floatsin(-(Angle+360.0),degrees)*50;
	vy=vecy+floatcos(-(Angle+360.0),degrees)*50;
	knife@_movetime[playerid]=MoveDynamicObject(_@TKNF[playerid],vx,vy,vecz,KNIFE_FINAL_VELOCITY,rx,ry,rz);
	SetTimerEx("destroy@_knives",knife@_movetime[playerid]-90,0,"i",playerid);
	return 1;
}


forward destroy@_knives(playerid);
public destroy@_knives(playerid)
{
	DestroyDynamicObject(_@TKNF[playerid]);
	_@TKNF[playerid]=-1;
	knife@_movetime[playerid]=-1;
	return 1;
}




/* Core Hooks */

/* OnPlayerConnect */
public OnPlayerConnect(playerid)
{
	_@TKNF[playerid]=-1;
	knife@_counter[playerid]=0;
	knife@_movetime[playerid]=-1;
	_@equiped[playerid]=0;
	
	#if defined CRF_OnPlayerConnect
      CRF_OnPlayerConnect(playerid);
    #endif
	return 1;
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect CRF_OnPlayerConnect
#if defined CRF_OnPlayerConnect
    forward CRF_OnPlayerConnect(playerid);
#endif

/* OnPlayerDisconnect */
public OnPlayerDisconnect(playerid, reason)
{
    _@TKNF[playerid]=-1;
	knife@_counter[playerid]=0;
	knife@_movetime[playerid]=-1;
	_@equiped[playerid]=0;

	#if defined CRF_OnPlayerDisconnect
      CRF_OnPlayerDisconnect(playerid, reason);
    #endif
	return 1;
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect CRF_OnPlayerDisconnect
#if defined CRF_OnPlayerDisconnect
    forward CRF_OnPlayerDisconnect(playerid, reason);
#endif

/* OnPlayerDeath */
public OnPlayerDeath(playerid, killerid, reason)
{
    _@equiped[playerid]=0;
    PlayerTextDrawDestroy(playerid, crosshair[playerid]);

	#if defined CRF_OnPlayerDeath
      CRF_OnPlayerDeath(playerid, killerid, reason);
    #endif
	return 1;
}

#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath CRF_OnPlayerDeath
#if defined CRF_OnPlayerDeath
    forward CRF_OnPlayerDeath(playerid, killerid, reason);
#endif

/* OnPlayerUpdate */
public OnPlayerUpdate(playerid)
{
	if(IsValidDynamicObject(_@TKNF[playerid]))
	{
	    for(new i=0;i<MAX_PLAYERS;i++)
	    {
	        if( i!= playerid)
	    	{
			new Float:x,Float:y,Float:z;
	    		GetDynamicObjectPos(_@TKNF[playerid],x,y,z);
	    		if(IsPlayerInRangeOfPoint(i,3.2,x,y,z))
	    		{
	        		new Float:health;
	        		GetPlayerHealth(i,health);
	        		SetPlayerHealth(i,health-random(100));
	        		x=-1;
	        		y=-1;
	        		z=-1;
	        		DestroyDynamicObject(_@TKNF[playerid]);
					_@TKNF[playerid]=-1;
				}
			}
		}
	}

	if(knife@_counter[playerid] == 0)
	{
		RemovePlayerAttachedObject(playerid, 0);
		knife@_movetime[playerid] = -1;
		_@equiped[playerid] = 0;
	}

	if(knife@_counter[playerid] > 0)
	{
	    _@has[playerid] = true;
	}
	else
	{
		_@has[playerid] = false;
		_@equiped[playerid] = 0;
		PlayerTextDrawDestroy(playerid, crosshair[playerid]);
	}

	 #if defined CRF_OnPlayerUpdate
        CRF_OnPlayerUpdate(playerid);
    #endif
		
	return 1;
}

#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate CRF_OnPlayerUpdate
#if defined CRF_OnPlayerUpdate
    forward CRF_OnPlayerUpdate(playerid);
#endif

/* Callback */
#if defined OnPlayerThrowKnife
forward OnPlayerThrowKnife(playerid);
#endif


/*******************************************************************************************/
