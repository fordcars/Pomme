#pragma once

//-----------------------------------------------------------------------------
// Filesystem permission char

enum EFSPermissions
{
    fsCurPerm = 0,
    fsRdPerm = 1,
    fsWrPerm = 2,
    fsRdWrPerm = 3,
};

//-----------------------------------------------------------------------------
// Position mode constants

enum {
	fsAtMark = 0,
	fsFromStart = 1,
	fsFromLEOF = 2,
	fsFromMark = 3
};

//-----------------------------------------------------------------------------
// Folder types

enum
{
    kSystemFolderType                   = 'macs',
    kDesktopFolderType                  = 'desk',
    kSystemDesktopFolderType            = 'sdsk',
    kTrashFolderType                    = 'trsh',
    kSystemTrashFolderType              = 'strs',
    kWhereToEmptyTrashFolderType        = 'empt',
    kPrintMonitorDocsFolderType         = 'prnt',
    kStartupFolderType                  = 'strt',
    kShutdownFolderType                 = 'shdf',
    kAppleMenuFolderType                = 'amnu',
    kControlPanelFolderType             = 'ctrl',
    kSystemControlPanelFolderType       = 'sctl',
    kExtensionFolderType                = 'extn',
    kFontsFolderType                    = 'font',
    kPreferencesFolderType              = 'pref',
    kSystemPreferencesFolderType        = 'sprf',
    kTemporaryFolderType                = 'temp'
};

enum {
    kOnSystemDisk                       = -32768L,
    kOnAppropriateDisk                  = -32767,
    kSystemDomain                       = -32766,
    kLocalDomain                        = -32765,
    kNetworkDomain                      = -32764,
    kUserDomain                         = -32763,
    kClassicDomain                      = -32762
};

#define kCreateFolder       true
#define kDontCreateFolder   false

//-----------------------------------------------------------------------------
// Error codes (OSErr)

enum EErrors
{
    noErr           = 0,

    unimpErr        = -4, // unimplemented core routine

    controlErr      = -17, // I/O System Errors
    statusErr       = -18, // I/O System Errors
    readErr         = -19, // I/O System Errors
    writErr         = -20, // I/O System Errors
    badUnitErr      = -21, // I/O System Errors
    unitEmptyErr    = -22, // I/O System Errors
    openErr         = -23, // I/O System Errors
    closErr         = -24, // I/O System Errors

    abortErr        = -27, // IO call aborted by KillIO
    notOpenErr      = -28, // Couldn't rd/wr/ctl/sts cause driver not opened
    dirFulErr       = -33, // File directory full
    dskFulErr       = -34, // Disk or volume full
    nsvErr          = -35, // Volume doesn't exist
    ioErr           = -36, // I/O error
    bdNamErr        = -37, // Bad file or volume name
    fnOpnErr        = -38, // File not open
    eofErr          = -39, // End-of-file reached
    posErr          = -40, // Attempt to position mark before start of file
    mFulErr         = -41, // Memory full (open) or file won't fit (load)
    tmfoErr         = -42, // Too many files open
    fnfErr          = -43, // File not found (FSSpec is still valid)
    wPrErr          = -44, // Volume is hardware-locked
    fLckdErr        = -45, // File is locked
    vLckdErr        = -46, // Volume is software-locked
    fBsyErr         = -47, // File is busy
    dupFNErr        = -48, // Duplicate filename
    opWrErr         = -49, // File already open for writing
    paramErr        = -50, // Invalid value passed in parameter
    rfNumErr        = -51, // Invalid reference number
    gfpErr          = -52, // Error during a GetFPos family function
    volOffLinErr    = -53, // Volume is offline
    permErr         = -54, // Permission error
    volOnLinErr     = -55, // Volume already online
    nsDrvErr        = -56, // No such Drive
    noMacDskErr     = -57, // Foreign disk
    extFSErr        = -58, // Volume belongs to external file system
    fsRnErr         = -59, // Couldn't rename
    badMDBErr       = -60, // Bad master directory block
    wrPermErr       = -61, // Read/write permission doesn't allow writing
    
    memROZWarn      = -99,  // soft error in ROZ
    memROZError     = -99,  // hard error in ROZ
    memROZErr       = -99,  // hard error in ROZ
    memFullErr      = -108, // Not enough room in heap zone
    nilHandleErr    = -109, // Master Pointer was NIL in HandleZone or other
    memWZErr        = -111, // WhichZone failed (applied to free block)
    memPurErr       = -112, // trying to purge a locked or non-purgeable block
    memAdrErr       = -110, // address was odd; or out of range
    memAZErr        = -113, // Address in zone check failed
    memPCErr        = -114, // Pointer Check failed
    memBCErr        = -115, // Block Check failed
    memSCErr        = -116, // Size Check failed
    memLockedErr    = -117, // trying to move a locked block (MoveHHi)

    dirNFErr        = -120, // Directory not found
    tmwdoErr        = -121, // Too many working directories open
    badMovErr       = -122, // Couldn't move
    wrgVolTypErr    = -123, // Unrecognized volume (not HFS)
    volGoneErr      = -124, // Server volume disconnected
    fsDSIntErr      = -127, // Non-hardware internal file system error

    userCanceledErr = -128,

    badExtResource  = -185, // extended resource has a bad format
    CantDecompress  = -186, // resource bent ("the bends") - can't decompress a compressed resource
    resourceInMemory= -188, // Resource already in memory
    writingPastEnd  = -189, // Writing past end of file
    inputOutOfBounds= -190, // Offset of Count out of bounds
    resNotFound     = -192, // Resource not found
    resFNotFound    = -193, // Resource file not found
    addResFailed    = -194, // AddResource failed
    addRefFailed    = -195, // AddReference failed
    rmvResFailed    = -196, // RmveResource failed
    rmvRefFailed    = -197, // RmveReference failed
    resAttrErr      = -198, // attribute inconsistent with operation
    mapReadErr      = -199, // map inconsistent with operation

    // Sound Manager error codes

    notEnoughHardwareErr    = -201,
    queueFull               = -203,
    resProblem              = -204,
    badChannel              = -205, // channel is corrupt or unusable
    badFormat               = -206, // resource is corrupt or unusable
    notEnoughBufferSpace    = -207, // insufficient memory available
    badFileFormat           = -208, // file is corrupt or unusable, or not AIFF or AIFF-C
    channelBusy             = -209,
    buffersTooSmall         = -210,
    siInvalidCompression    = -223,

	// NetSprocket error codes
	kNSpInitializationFailedErr = -30360,
	kNSpAlreadyInitializedErr   = -30361,
	kNSpTopologyNotSupportedErr = -30362,
	kNSpMessageSizeTooBigErr    = -30363,
	kNSpBufferTooSmallErr       = -30364,
	kNSpReceiveDataErr          = -30365,
	kNSpProtocolNotAvailableErr = -30366,
	kNSpInvalidGameRefErr       = -30367,
	kNSpInvalidNetMessageErr    = -30368,
	kNSpInvalidParameterErr     = -30369,
	kNSpOTNotPresentErr         = -30370,
	kNSpHostFailedErr           = -30371,
	kNSpNotHostAddressErr       = -30372,
	kNSpMemAllocationErr        = -30373,
	kNSpAlreadyAdvertisingErr   = -30374,
	kNSpNoTypeSpecifiedErr      = -30375,
	kNSpNotAdvertisingErr       = -30376,
	kNSpInvalidAddressErr       = -30377,
	kNSpFreeQExhaustedErr       = -30378,
	kNSpRemovePlayerFailedErr   = -30379,
	kNSpAddressInUseErr         = -30380,
	NSpFeatureNotImplementedErr = -30381,
	NSpNameRequiredErr          = -30382,
	NSpInvalidPlayerIDErr       = -30383,
	NSpInvalidGroupIDErr        = -30384,
	NSpNoPlayersErr             = -30385,
	NSpNoGroupsErr              = -30386,
	NSpNoHostVolunteersErr      = -30387,
	kNSpCreateGroupFailedErr    = -30388,
	NSpAddPlayerFailedErr       = -30389,
	NSpInvalidDefinitionErr     = -30390,
	NSpInvalidProtocolRefErr    = -30391,
	NSpInvalidProtocolListErr   = -30392,
	kNSpTimeoutErr              = -30393,
	kNSpGameTerminatedErr       = -30394,
	kNSpConnectFailedErr        = -30395,
	kNSpSendFailedErr           = -30396,
	kNSpPortTakenErr            = -30397,
	kNSpNotPlayingErr           = -30398
};


//-----------------------------------------------------------------------------
// Script Manager enums

enum EScriptManager
{
	// Implicit script codes
	smSystemScript		= -1,
	smCurrentScript		= -2,
	smAllScripts		= -3,

	// Explicit script codes
	smRoman				= 0,

	// Language codes
	langEnglish			= 0,	// smRoman script
	langFrench			= 1,	// smRoman script
	langGerman			= 2,	// smRoman script
	langItalian			= 3,	// smRoman script
	langDutch			= 4,	// smRoman script
	langSwedish			= 5,	// smRoman script
	langSpanish			= 6,	// smRoman script
	langDanish			= 7,	// smRoman script
	langPortuguese		= 8,	// smRoman script
	langNorwegian		= 9,	// smRoman script
};

//-----------------------------------------------------------------------------

enum EEvents
{
    everyEvent = ~0
};

//-----------------------------------------------------------------------------
// Memory enums

enum EMemory
{
    maxSize = 0x7FFFFFF0 // the largest block possible
};

//-----------------------------------------------------------------------------
// Resource types

enum EResTypes
{
    rAliasType = 'alis',
};

//-----------------------------------------------------------------------------
// Sound Manager enums

enum ESndPitch
{
    kMiddleC = 60L,
};

enum ESndSynth
{
    squareWaveSynth = 1,
    waveTableSynth = 3,
    sampledSynth = 5
};

enum ESndInit
{
    initChanLeft = 0x0002,    // left stereo channel
    initChanRight = 0x0003,    // right stereo channel
    initMono = 0x0080,    // monophonic channel
    initStereo = 0x00C0,    // stereo channel
    initMACE3 = 0x0300,    // 3:1 compression
    initMACE6 = 0x0400,    // 6:1 compression
    initNoInterp = 0x0004,    // no linear interpolation
    initNoDrop = 0x0008,    // no drop-sample conversion
};

enum ESndVolume
{
	kFullVolume = 0x0100,
	kNoVolume = 0,
};

// Sound commands
enum ESndCmds
{
    nullCmd = 0,
    initCmd = 1,
    freeCmd = 2,
    quietCmd = 3,
    flushCmd = 4,
    reInitCmd = 5,
    waitCmd = 10,
    pauseCmd = 11,
    resumeCmd = 12,
    callBackCmd = 13,
    syncCmd = 14,
    availableCmd = 24,
    versionCmd = 25,
    totalLoadCmd = 26,
    loadCmd = 27,
    freqDurationCmd = 40,
    restCmd = 41,
    freqCmd = 42,
    ampCmd = 43,
    timbreCmd = 44,
    getAmpCmd = 45,
    volumeCmd = 46,       // sound manager 3.0 or later only
    getVolumeCmd = 47,       // sound manager 3.0 or later only
    clockComponentCmd = 50,       // sound manager 3.2.1 or later only
    getClockComponentCmd = 51,       // sound manager 3.2.1 or later only
    scheduledSoundCmd = 52,       // sound manager 3.3 or later only
    linkSoundComponentsCmd = 53,       // sound manager 3.3 or later only
    waveTableCmd = 60,
    phaseCmd = 61,
    soundCmd = 80,
    bufferCmd = 81,
    rateCmd = 82,
    continueCmd = 83,
    doubleBufferCmd = 84,
    getRateCmd = 85,
    rateMultiplierCmd = 86,
    getRateMultiplierCmd = 87,
    sizeCmd = 90,       // obsolete command
    convertCmd = 91,        // obsolete MACE command
    pommeSetLoopCmd = 0x7001,
    pommePausePlaybackCmd = 0x7002,  // pause playback ('pauseCmd' locks the channel, it doesn't pause playback)
    pommeResumePlaybackCmd = 0x7003,  // resume playback ('resumeCmd' unlocks the channel, it doesn't unpause playback)
    // Do not define commands above 0x7FFF -- the high bit means a 'snd ' resource has associated sound data
};

//-----------------------------------------------------------------------------
// Keyboard enums

enum
{
    // key positions on US keyboard
    kVK_ANSI_A                    = 0x00,
    kVK_ANSI_S                    = 0x01,
    kVK_ANSI_D                    = 0x02,
    kVK_ANSI_F                    = 0x03,
    kVK_ANSI_H                    = 0x04,
    kVK_ANSI_G                    = 0x05,
    kVK_ANSI_Z                    = 0x06,
    kVK_ANSI_X                    = 0x07,
    kVK_ANSI_C                    = 0x08,
    kVK_ANSI_V                    = 0x09,
    kVK_ANSI_B                    = 0x0B,
    kVK_ANSI_Q                    = 0x0C,
    kVK_ANSI_W                    = 0x0D,
    kVK_ANSI_E                    = 0x0E,
    kVK_ANSI_R                    = 0x0F,
    kVK_ANSI_Y                    = 0x10,
    kVK_ANSI_T                    = 0x11,
    kVK_ANSI_1                    = 0x12,
    kVK_ANSI_2                    = 0x13,
    kVK_ANSI_3                    = 0x14,
    kVK_ANSI_4                    = 0x15,
    kVK_ANSI_6                    = 0x16,
    kVK_ANSI_5                    = 0x17,
    kVK_ANSI_Equal                = 0x18,
    kVK_ANSI_9                    = 0x19,
    kVK_ANSI_7                    = 0x1A,
    kVK_ANSI_Minus                = 0x1B,
    kVK_ANSI_8                    = 0x1C,
    kVK_ANSI_0                    = 0x1D,
    kVK_ANSI_RightBracket         = 0x1E,
    kVK_ANSI_O                    = 0x1F,
    kVK_ANSI_U                    = 0x20,
    kVK_ANSI_LeftBracket          = 0x21,
    kVK_ANSI_I                    = 0x22,
    kVK_ANSI_P                    = 0x23,
    kVK_ANSI_L                    = 0x25,
    kVK_ANSI_J                    = 0x26,
    kVK_ANSI_Quote                = 0x27,
    kVK_ANSI_K                    = 0x28,
    kVK_ANSI_Semicolon            = 0x29,
    kVK_ANSI_Backslash            = 0x2A,
    kVK_ANSI_Comma                = 0x2B,
    kVK_ANSI_Slash                = 0x2C,
    kVK_ANSI_N                    = 0x2D,
    kVK_ANSI_M                    = 0x2E,
    kVK_ANSI_Period               = 0x2F,
    kVK_ANSI_Grave                = 0x32,
    kVK_ANSI_KeypadDecimal        = 0x41,
    kVK_ANSI_KeypadMultiply       = 0x43,
    kVK_ANSI_KeypadPlus           = 0x45,
    kVK_ANSI_KeypadClear          = 0x47,
    kVK_ANSI_KeypadDivide         = 0x4B,
    kVK_ANSI_KeypadEnter          = 0x4C,
    kVK_ANSI_KeypadMinus          = 0x4E,
    kVK_ANSI_KeypadEquals         = 0x51,
    kVK_ANSI_Keypad0              = 0x52,
    kVK_ANSI_Keypad1              = 0x53,
    kVK_ANSI_Keypad2              = 0x54,
    kVK_ANSI_Keypad3              = 0x55,
    kVK_ANSI_Keypad4              = 0x56,
    kVK_ANSI_Keypad5              = 0x57,
    kVK_ANSI_Keypad6              = 0x58,
    kVK_ANSI_Keypad7              = 0x59,
    kVK_ANSI_Keypad8              = 0x5B,
    kVK_ANSI_Keypad9              = 0x5C
};

// keycodes for keys that are independent of keyboard layout
enum
{
    kVK_Return                    = 0x24,
    kVK_Tab                       = 0x30,
    kVK_Space                     = 0x31,
    kVK_Delete                    = 0x33,
    kVK_Escape                    = 0x35,
    kVK_Command                   = 0x37,
    kVK_Shift                     = 0x38,
    kVK_CapsLock                  = 0x39,
    kVK_Option                    = 0x3A,
    kVK_Control                   = 0x3B,
    kVK_RightShift                = 0x3C,
    kVK_RightOption               = 0x3D,
    kVK_RightControl              = 0x3E,
    kVK_Function                  = 0x3F,
    kVK_F17                       = 0x40,
    kVK_VolumeUp                  = 0x48,
    kVK_VolumeDown                = 0x49,
    kVK_Mute                      = 0x4A,
    kVK_F18                       = 0x4F,
    kVK_F19                       = 0x50,
    kVK_F20                       = 0x5A,
    kVK_F5                        = 0x60,
    kVK_F6                        = 0x61,
    kVK_F7                        = 0x62,
    kVK_F3                        = 0x63,
    kVK_F8                        = 0x64,
    kVK_F9                        = 0x65,
    kVK_F11                       = 0x67,
    kVK_F13                       = 0x69,
    kVK_F16                       = 0x6A,
    kVK_F14                       = 0x6B,
    kVK_F10                       = 0x6D,
    kVK_F12                       = 0x6F,
    kVK_F15                       = 0x71,
    kVK_Help                      = 0x72,
    kVK_Home                      = 0x73,
    kVK_PageUp                    = 0x74,
    kVK_ForwardDelete             = 0x75,
    kVK_F4                        = 0x76,
    kVK_End                       = 0x77,
    kVK_F2                        = 0x78,
    kVK_PageDown                  = 0x79,
    kVK_F1                        = 0x7A,
    kVK_LeftArrow                 = 0x7B,
    kVK_RightArrow                = 0x7C,
    kVK_DownArrow                 = 0x7D,
    kVK_UpArrow                   = 0x7E
};

//-----------------------------------------------------------------------------
// QD2D

enum
{
    whiteColor      = 30,
    blackColor      = 33,
    yellowColor     = 69,
    magentaColor    = 137,
    redColor        = 205,
    cyanColor       = 273,
    greenColor      = 341,
    blueColor       = 409,
};

enum
{
	srcCopy = 0,
	srcOr = 1,
	srcXor = 2,
	srcBic = 3,
	notSrcCopy = 4,
	notSrcOr = 5,
	notSrcXor = 6,
	notSrcBic = 7,
	patCopy = 8,
	patOr = 9,
	patXor = 10,
	patBic = 11,
	notPatCopy = 12,
	notPatOr = 13,
	notPatXor = 14,
	notPatBic = 15,
	grayishTextOr = 49,
	hilitetransfermode = 50,
	hilite = 50,
	blend = 32,
	addPin = 33,
	addOver = 34,
	subPin = 35,
	addMax = 37,
	adMax = 37,
	subOver = 38,
	adMin = 39,
	ditherCopy = 64,
	transparent = 36
};

enum
{
	pmCourteous							= 0x0000,	// Courteous color
	pmTolerant							= 0x0002,	// Tolerant color
	pmAnimated							= 0x0004,	// Animated color
	pmExplicit							= 0x0008,	// Explicit color
	pmWhite								= 0x0010,	// Use on 1-bit devices
	pmBlack								= 0x0020,	// Use on 1-bit devices
	pmInhibitG2							= 0x0100,
	pmInhibitC2							= 0x0200,
	pmInhibitG4							= 0x0400,
	pmInhibitC4							= 0x0800,
	pmInhibitG8							= 0x1000,
	pmInhibitC8							= 0x2000,
};

//-----------------------------------------------------------------------------
// NetSprocket

enum
{
	kNSpGameFlag_DontAdvertise = 0x00000001,
	kNSpGameFlag_ForceTerminateGame = 0x00000002
};

typedef enum
{
	kNSpClientServer = 0x00000001
} NSpTopology;