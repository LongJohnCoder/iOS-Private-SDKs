/*
 *  TiSerialFlasherLib.h
 *  TiSerialFlasher
 *
 *  Created by Dan Fletcher on 3/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#if TARGET_OS_EMBEDDED
#define TI_SERIAL_FLASHER_PORT_PATH				"/dev/cu.mcu-bsl0"
#define TI_SERIAL_FLASHER_FIRMWARE_DIR			"/usr/standalone/firmware"
#else
#define TI_SERIAL_FLASHER_PORT_PATH				"/dev/cu.usbserial-A8008irM"
#define TI_SERIAL_FLASHER_FIRMWARE_DIR			"."
#endif

// the filename should be "TiSerialFlasher-XX.YY.txt"

// K66
#define TI_SERIAL_FLASHER_FIRMWARE_PATH_K66         TI_SERIAL_FLASHER_FIRMWARE_DIR "/" "TiSerialFlasher-01.32.txt"

// J33
// J33 Proto is deprecated
#define TI_SERIAL_FLASHER_FIRMWARE_PATH_J33_EVTANDLATER TI_SERIAL_FLASHER_FIRMWARE_DIR "/" "TiSerialFlasher-02.07.txt"

// J33i
#define TI_SERIAL_FLASHER_FIRMWARE_PATH_J33i            TI_SERIAL_FLASHER_FIRMWARE_DIR "/" "TiSerialFlasher-03.05.txt"

// J34 - was 04
// J34m - was 04
// J34i - was 05

// J42
#define TI_SERIAL_FLASHER_FIRMWARE_PATH_J42 		    TI_SERIAL_FLASHER_FIRMWARE_DIR "/" "TiSerialFlasher-06.05.txt"

#define TI_SERIAL_FLASHER_VERSION                       "01.18"

#define TISERIALFLASHER_LIBNAME		"libTiSerialFlasher.dylib"
#define TISERIALFLASHER_CALLNAME	"TiSerialFlasher"

typedef void (*progressCallbackType)(int percent, void *context);
typedef void (*loggingCallbackType)(const char* fmt, va_list args, void *context);
typedef int (*flasherInvocation)(void* progressCallback, void* loggingCallback, const char* firmware, const char* device, void *context);

// return status of 0 if success, negative if failure
int TiSerialFlasher(progressCallbackType aProgressCallback,
					loggingCallbackType aLoggingCallback,
					const char* firmwareFileOverride,
					const char* devicePathOverride,
					void *context);

// callbacks
void reportProgress(int percent);
void logMessage(const char *fmt, ...);
