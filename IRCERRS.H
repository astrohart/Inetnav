//
// (C) Copyright 1995 Fairgate Technologies.
//
// This file is Copyrighted and is subject to the rules outlined in the
// software agreement accompanying this CDROM and book. You are free to
// modify this code as you see fit according to those rules.
//
// THIS IS SAMPLE CODE! No warranties are expressed or implied. Use it
// at your own risk. The resulting program may not be fully functional. 
//  Support for some features are left out so that they can be added by you, 
//  the reader.
//

// IRC errors
#define ERR_NORECIPIENT		411
#define ERR_NOTEXTTOSEND	412
#define ERR_NICKNAMEINUSE	433
#define ERR_NICKCOLLISION	436
#define ERR_CHANNELISFULL	471
#define ERR_YOUREBANNEDCREEP 465
#define ERR_INVITEONLYCHAN	473
#define ERR_BANNEDFROMCHAN	474

#define RPL_NOTOPIC			331
#define RPL_TOPIC			332
#define RPL_START_MOTD		375
#define RPL_MORE_MOTD		372
#define RPL_END_MOTD		376
#define RPL_NAMREPLY		353     
#define RPL_ENDOFNAMES 		366

// codes we ignore
#define IGN_STATUS1			001
#define IGN_STATUS2			002
#define IGN_STATUS3			003
#define IGN_STATUS4			004
#define RPL_LUSERCLIENT     251     
                       
#define RPL_LUSEROP			252     
#define RPL_LUSERUNKNOWN	253     
#define RPL_LUSERCHANNELS	254     
#define RPL_LUSERME			255     
