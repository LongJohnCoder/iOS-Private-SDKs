/*
 * ramrod_update.h
 *   This header declares and documents the interfaces used for updating
 *   the device from a ramrod plugin.
 */

#if !defined(RAMROD_UPDATE_H)
#define RAMROD_UPDATE_H

#include <sys/cdefs.h>

#include <CoreFoundation/CoreFoundation.h>

#include <ramrod/ramrod_base.h>


__BEGIN_DECLS

/*
 * Keys into the ramrod_update_device_firmware options dictionary.
 *
 * Format below is:
 * OPTION_NAME (value type/default value if not present) - description
 *
 * This first set of options controls what updates are actually performed.  The
 * initial request for this interface expressed a desire that as new things
 * to update are added, that the clients get them for free.  So all updates
 * will be attempted unless explicitly disabled.  This means that clients need
 * to gracefully handle the introduction of new update operations
 * Obviously, if a new updater requires data from the client (via the get_data 
 * callback), the client will need to be updated.
 * RAMROD_UPDATE_OPT_UPDATE_IBOOT (CFBooleanRef/kCFBooleanTrue) - If true,
 *  the update function will update the installed iBoot
 * RAMROD_UPDATE_OPT_UPDATE_GAS_GAUGE (CFBooleanRef/kCFBooleanTrue) - If true,
 *  the update function will update the gas gauge firmware
 * RAMROD_UPDATE_OPT_UPDATE_IR_MCU (CFBooleanRef/kCFBooleanTrue) - If true,
 *  the update function will update the IR MCU firmware
 * RAMROD_UPDATE_OPT_UPDATE_BASEBAND (CFBooleanRef/kCFBooleanTrue) - If true,
 *  the update function will update the baseband software
 * RAMROD_UPDATE_OPT_UPDATE_ORION (CFBooleanRef/kCFBooleanTrue) - If true,
 *  the update function will update the Orion firmware
 * RAMROD_UPDATE_OPT_STOCKHOLM_TERMINATE_APPLETS (CFBooleanRef/kCFBooleanFalse)
 *  Set to true on an erase install. If Erase installing, we must terminate applets
 *  during the stockholm update.
 * RAMROD_UPDATE_OPT_STOCKHOLM_FACTORY_INSTALL (CFBooleanRef/kCFBooleanFalse)
 *  If true, set the kNfUpdateParamFactoryInstall flag to true for stockholm installer
 * RAMROD_UPDATE_OPT_STOCKHOLM_POSTFLIGHT_SCRIPT (CFStringRef/empty)
 *  This option provides the path the stokholm updater postflight script
 * RAMROD_UPDATE_OPT_STOCKHOLM_JCOP_TYPE (CFStringRef/empty)
 *  This option set the JCOP type of stockholm updates
 * RAMROD_UPDATE_OPT_STOCKHOLM_ADVANCED_INSTALL (CFBooleanRef/KCFBooleanFalse)
 *  If true, enable advanced installation for stockholm updater
 * RAMROD_UPDATE_OPT_STOCKHOLM_VALIDATE_JCOP (CFBooleanRef/kCFBooleanFalse)
 *  If true, perform validation of a stockholm JCOP upgrade
 * RAMROD_UPDATE_OPT_SKIP_S3E_UPDATE (CFBooleanRef/kCFBooleanFalse)
 *  If true, skip S3E FW update
 * RAMROD_UPDATE_OPT_S3E_OVERRIDE (CFStringRef/empty)
 *  This option provides the path to the SWE firmware override directory
 * RAMROD_UPDATE_OPT_ENABLE_GRAPE_FW_UPDATE (CFBooleanRef/kCFBooleanTrue)
 *  If true, perform the grape FW update during the restore
 * RAMROD_UPDATE_OPT_ENABLE_OPAL_FW_UPDATE (CFBooleanRef/kCFBooleanTrue)
 *  If true, perform the opal FW update during the restore.
 * RAMROD_UPDATE_OPT_IGNORE_SWD_FAILURE (CFBooleanRed/kCFBooleanFalse)
 *  It true, ignore any failures from the SWD updater and continue the restore
 * RAMROD_UPDATE_OPT_GRAPE_FW_DATA (CFDataRef/empty)
 *  This is the data containing the grape fw override data.
 * RAMROD_UPDATE_OPT_OPAL_FW_DATA
 *  This is the data containing the opal fw override data.
 * RAMROD_UPDATE_OPT_STOCKHOLM_PERFORM_INSTALL (CFBooleanRef/kCFBooleanTrue)
 *  If true, perform the stockholm installer
 * RAMROD_UPDATE_OPT_STOCKHOLM_SKIP_JCOP (CFBooleaRef/kCFBooleanFalse)
 *  If true, skip the JCOP portion of the Stockholm updater
 * RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTIONS (CFDictionaryRef/empty) - This
 *  option provides a dictionary of baseband updater options that will be 
 *  passed directly to BBUpdaterSetOptions().  See the baseband updater library
 *  for the possible keys/values.
 * RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTION_SYSTEM_ROOT (CFStringRef/empty)
 *  This option provides the path to the mounted system filesystem. If not present
 *  the system filesystem is not mounted.
 * RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTION_DATA_ROOT (CFStringRef/empty)
 *  This option provides the path to the mounted data filesystem.
 * RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTION_BASEBAND_DATA_ROOT (CFStringRef/empty)
 *  This option provides the path to the mounted baseband data filesystem.
 * RAMROD_UPDATE_OPT_SEP_UPDATER_OPTION_SYSTEM_ROOT (CFStringRef/empty)
 *  Path to the filesystem root for the system partition.  Required
 *  to support restoring the SEP.
 * RAMROD_UPDATE_OPT_SEP_UPDATE_REQUIRES_SUCCESS (CFBooleanRef/kCFBooleanTrue)
 *  If false, a failure to update the SEP should not result in an error being
 *  returned.  If true (or missing), a SEP update failure should result in
 *  an error being returned.
 * RAMROD_UPDATE_OPT_SEP_RESTORE (CFBooleanRef/kCFBooleanFalse)
 *  If true, the SEP firmware is being loaded during a restore.
 * RAMROD_UPDATE_OPT_SEP_LOAD_REQUIRES_ART (CFBooleanRef/kCFBooleanFalse)
 *  If true, the ART is needed when the SEP OS is loaded.  When this is present
 *  and true, we will load the SEP firmware with the ART.
 */

/* Top level options that control what updates are performed */
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_IBOOT;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_GAS_GAUGE;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_IR_MCU;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_BASEBAND;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_STOCKHOLM;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_SWDHID;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_S3E;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_TCON;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_ORION;


/* Stockholm installer options */
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_STOCKHOLM_TERMINATE_APPLETS;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_STOCKHOLM_FACTORY_INSTALL;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_STOCKHOLM_ADVANCED_INSTALL;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_STOCKHOLM_PERFORM_INSTALL;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_STOCKHOLM_SKIP_JCOP;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_STOCKHOLM_POSTFLIGHT_SCRIPT;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_STOCKHOLM_JCOP_TYPE;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_STOCKHOLM_VALIDATE_JCOP;

/* Grape/Opal FW  */
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_ENABLE_GRAPE_FW_UPDATE;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_ENABLE_OPAL_FW_UPDATE;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_ENABLE_HPM_FW_UPDATE;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_IGNORE_SWD_FAILURE;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_GRAPE_FW_DATA;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_OPAL_FW_DATA;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_HPM_FW_DATA;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_ENABLE_TCON_FW_UPDATE;

/* S3E update options */
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_SKIP_S3E_UPDATE;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_S3E_OVERRIDE;

/* Options that control the behavior of the baseband updater */
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTIONS;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTION_SYSTEM_ROOT;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTION_DATA_ROOT;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTION_BASEBAND_DATA_ROOT;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_BASEBAND_UPDATER_OPTION_RESTORE_NV;

RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_SEP_UPDATER_OPTION_SYSTEM_ROOT;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_SEP_UPDATE_REQUIRES_SUCCESS;

RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_SEP_RESTORE;
RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_SEP_LOAD_REQUIRES_ART;

RAMROD_EXPORT CFStringRef const RAMROD_UPDATE_OPT_UPDATE_ERASE_INSTALL;

/* Progress operations reported to the progress callback */
enum {
    RAMROD_IBOOT_UPDATE_PROGRESS = 0,
    RAMROD_IR_MCU_UPDATE_PROGRESS = 1,
    RAMROD_GAS_GAUGE_UPDATE_PROGRESS = 2,
    RAMROD_BASEBAND_UPDATE_PROGRESS = 3,
    RAMROD_STOCKHOLM_UPDATE_PROGRESS = 4,
    RAMROD_SWDHID_UPDATE_PROGRESS = 5,
};

enum
{
    kNVMeUpdateStatusInvalidUpdate		= -1,
    kNVMeUpdateStatusRegularUpdate		= 0,
    kNVMeUpdateStatusProductionUpdate 	= 1,
    kNVMeUpdateStatusEraseUpdate		= 2,
    kNVMeUpdateStatusCount			= 4
};

struct ramrod_update_callbacks {
    /* Version of the structure.  This field currently must be 0 */
    int version;
    
    /*
     * Callback is called to indicate progress at various points in the update
     * process.  Note that if the 'progress' value is -1, it means that the
     * operation has changed but no determinate progress is available (future
     * callbacks might update the determinate progress for that operation)
     */
    void (*progress)(int operation, int progress, void *context);

    /*
     * Callback should return a CFDataRef containing the data for the device
     * LLB.
     */
    CFDataRef (*copy_llb)(void *context);

    /*
     * Callback should return a CFArrayRef for legacy clients and CFDictionaryRef otherwise.
     * It containing the data for all the images in the all_flash *except the LLB(iBoot, Dali)*.
     * Each image should be a single CFDataRef in the array.
     */
    CFTypeRef (*copy_all_flash_images)(void *context);

    /*
     * Callback should return a CFData containing the bbfw data.
     * The 'info' argument is a CFDictionaryRef containing personalization 
     * paramaters, returned from  BBUpdaterExecCommand(...kBBUCmdQueryInfo...)
     */
    CFDataRef (*copy_baseband_firmware)(CFDictionaryRef info, void *context);
    
    /*
     * Callback is provided status about the on-going baseband update process
     * in the form of whether the last command was accepted and any output it
     * generated
     */
    void (*baseband_update_status)(Boolean accepted, CFDictionaryRef output, CFErrorRef error, void *context);
    
    /*
     * Callback should return a CFDataRef containing the data for the SEP OS 
     * firmware image.
     */
    CFDataRef (*copy_sep)(void *context);
};

/*
 * ramrod_update_device_firmware
 *   This function updates the firmware of many components of the device.
 *   'options' controls what is updated and how (see above for the keys/values
 *   and what they do).
 *   'callbacks' is a set of callbacks needed at various points in the update
 *   process.  They are used to request data from the caller, provide status
 *   updates, etc.
 *   'context' is passed through to each of the callbacks
 */
RAMROD_EXPORT Boolean ramrod_update_device_firmware(CFDictionaryRef options,
                                                    struct ramrod_update_callbacks *callbacks,
                                                    void *context,
                                                    CFErrorRef *error);

#if !TARGET_OS_NANO
/*
 *	update_s3e
 *  This function updated the S3E firmware. It cannot be calling inside of 
 *	ramrod_update_device_firmware because in some cases the update requres
 *  an erase install of the nand. 
 *   'options' controls what is updated and how (see above for the keys/values
 *   and what they do).
 *   'callbacks' is a set of callbacks needed at various points in the update
 *   process.  They are used to request data from the caller, provide status
 *   updates, etc.
 *   'context' is passed through to each of the callbacks
 *
 */
RAMROD_EXPORT Boolean update_s3e(CFDictionaryRef options,
                                 struct ramrod_update_callbacks *callbacks,
                                 void *context,
                                 CFErrorRef *error);
#endif

/*
 * ramrod_baseband_verify_sealed_manifest
 *  Interrogates the baseband to determine if it is satisfied with its
 *  provisioning and calibration manifests.
 *  'options' is a dictionary of options similar to what is passed to
 *  ramrod_update_device_firmware.
 *  'callbacks' is a set of callbacks needed at various points to provide
 *  progress information.
 *  'context' is passed to the callbacks provided.
 */
RAMROD_EXPORT Boolean ramrod_baseband_verify_sealed_manifest(CFDictionaryRef options,
                                                             struct ramrod_update_callbacks *callbacks,
                                                             void *context,
                                                             CFErrorRef *error);


RAMROD_EXPORT Boolean ramrod_device_has_sep();

RAMROD_EXPORT Boolean ramrod_load_sep_os(CFDictionaryRef options,
                                         CFDataRef sep_os_data,
                                         CFErrorRef *error);

RAMROD_EXPORT Boolean ramrod_load_sep_os_from_filesystem(CFDictionaryRef options,
                                                         const char *sep_os_path,
                                                         CFErrorRef *error);

RAMROD_EXPORT Boolean ramrod_load_sep_os_from_system_partition(CFErrorRef *error);

/* 
 * ramrod_device_has_penguin
 *   Check if a device has an IOPenguinController in the IORegistry
 */
RAMROD_EXPORT Boolean ramrod_device_has_penguin();

/*
 * ramrod_device_update_penguin
 *   Update the IOPenguinController
 */
RAMROD_EXPORT Boolean ramrod_device_update_penguin();

/*
 * ramrod_update_fstab_with_devnodes
 *   This function modifies the device's fstab to correctly reflect the state
 *   of the device.
 *   This function needs to know explicitly the system and data partitions' device node
 *   paths. In all other respects this function behaves the same as ramrod_update_fstab.
 */
RAMROD_EXPORT Boolean ramrod_update_fstab_with_devnodes(char const * const fstab_path,
                                                        char const * const system_partition_device_node,
                                                        char const * const data_partition_device_node,
                                                        Boolean make_system_rw,
                                                        CFErrorRef *error);
/*
 * ramrod_update_fstab_with_additional_devnodes
 *   This function modifies the device's fstab to correctly reflect the state
 *   of the device.
 *   This function needs to know explicitly the system and data partitions' device node
 *   paths.
 *   Callers may optionally provide arrays of additional device node paths and
 *   the corresponding mount points.  fstab entries will be generated for these 
 *   partitions. If non-NULL, additional_device_nodes and 
 *   additional_mount_points must both have additional_entries_count entries.
 *   In all other respects this function behaves the same as ramrod_update_fstab.
 */
RAMROD_EXPORT Boolean ramrod_update_fstab_with_additional_devnodes(char const * const fstab_path,
                                                                   char const * const system_partition_device_node,
                                                                   char const * const data_partition_device_node,
                                                                   char const ** const additional_device_nodes,
                                                                   char const ** const additional_mount_points,
                                                                   size_t additional_entries_count,
                                                                   Boolean make_system_rw,
                                                                   CFErrorRef *error);

/*
 * ramrod_baseband_update_requires_system_filesystem
 *   This function returns true if the baseband update will require the system
 *   filesystem to be mounted.
 */
RAMROD_EXPORT bool ramrod_baseband_update_requires_system_filesystem(CFErrorRef *error);

/*
 * ramrod_baseband_update_requires_system_filesystem
 *   This function returns true if the baseband update will require the user
 *   filesystem to be mounted.
 */
RAMROD_EXPORT bool ramrod_baseband_update_requires_data_filesystem();

/*
 * ramrod_baseband_update_required_baseband_data_filesystem_size
 *   If the baseband update will require the baseband filesystem to be created
 *   and mounted, this function returns the required size of that partition in 
 *   megabytes. Otherwise, returns 0.
 */
RAMROD_EXPORT size_t ramrod_baseband_update_required_baseband_data_filesystem_size(CFErrorRef *error);

/*
 * ramrod_update_copy_baseband_migration_data
 *   This function returns migration data that the baseband would like to have
 *   persisted across any operations that might reformat the device's storage.
 *   The options dictionary may be used to specify the current mount point for
 *   the baseband data filesystems using the constant defined above.  If
 *   ramrod_baseband_update_required_baseband_data_filesystem_size returns
 *   non-zero, the baseband data filesystem mountpoint must be provided or the
 *   behavior is undefined.
 *   Returns NULL if no such data exists, or if an error occurs, in which case
 *   the error pointer, if provided, will be populated.
 */
RAMROD_EXPORT CFDictionaryRef ramrod_update_copy_baseband_migration_data(CFDictionaryRef options, CFErrorRef *error);

/*
 * ramrod_update_restore_baseband_migration_data
 *   This function restores the provided migration data to the baseband.
 *   The options dictionary may be used to specify the current mount point for
 *   the baseband data filesystems using the constant defined above.  If
 *   ramrod_baseband_update_required_baseband_data_filesystem_size returns
 *   non-zero, the baseband data filesystem mountpoint must be provided or the
 *   behavior is undefined.
 *   Returns TRUE on success, or FALSE if an error occurs, in which case the 
 *   error pointer, if provided, will be populated.
 */
RAMROD_EXPORT Boolean ramrod_update_restore_baseband_migration_data(CFDictionaryRef migration_data, CFDictionaryRef options, CFErrorRef *error);

__END_DECLS

#endif
