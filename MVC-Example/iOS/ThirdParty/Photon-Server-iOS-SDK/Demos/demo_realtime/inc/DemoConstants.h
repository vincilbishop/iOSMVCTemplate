/**
* DemoConstants.h - Exit Games Photon "Realtime Demo"
* Copyright (C) 2007-2008 Exit Games GmbH
* http://www.exitgames.com
*
* @author developer@exitgames.com
*/

#ifndef __DEMO_CONSTANTS_H
#define __DEMO_CONSTANTS_H

// type of transfer for inGame-data ()
const unsigned char PHOTON_UNRELIABLE = 0;
const unsigned char PHOTON_RELIABLE   = 1;

const unsigned char EV_PLAYERINFO = 0;
const unsigned char EV_MOVE       = 1;

// Keys for PlayerStatus-Events
const unsigned char STATUS_PLAYER_POS_X = 0;
const unsigned char STATUS_PLAYER_POS_Y = 1;
const unsigned char STATUS_PLAYER_NAME  = 2;
const unsigned char STATUS_PLAYER_COLOR = 3;

#endif