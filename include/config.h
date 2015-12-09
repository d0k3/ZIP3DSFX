#pragma once

// -- ZIP3DSFX configuration --
// ----------------------------

// This has to be #defined, it contains the current version number.
#define ZIP3DSFX_VER 6

// Only one of these two can be #defined at one time
// If both are activated, behaviour is undefined
// If none are activated, the user is asked
// #define OVERWRITE_NEVER
#define OVERWRITE_ALWAYS

// If this is #defined, output to the root dir is forced
// Otherwise, the output dir is the dir of the SFX.3DSX
#define FORCE_ROOT

// This #define prevents new directories from being created,
// files in such dirs are automatically skipped.
// This is useful f.e. for icon packs, where only existing
// dirs should be used to extract.
// #define NO_CREATE_DIRS

// If #defined this has to be set to something. Helps in
// cases where sfx_stub would otherwise not be compatible.
// Only does something when ZIP3DSFX is built as sfx_stub
// #define FORCE_MY_NAME "boot.3dsx"
