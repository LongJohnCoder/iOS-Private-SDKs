/*
 * ramrod.h
 *   This header declares and documents all the interfaces available to ramrod
 *   plugins.
 */

#if !defined(RAMROD_H)
#define RAMROD_H

#include <ramrod/ramrod_base.h>
#include <ramrod/ramrod_update.h>

#include <sys/cdefs.h>

#include <CoreFoundation/CoreFoundation.h>


__BEGIN_DECLS

/*
 * ramrod_message_handler
 *   Function pointer to a message handler in a ramrod plugin.  The function
 *   return value indicates how the process proceeds:
 *     RAMROD_MSG_HANDLER_CONTINUE - message was handled, continue processing 
 *       messages from this client
 *     RAMROD_MSG_HANDLER_DONE - message was handled, this client connection
 *       can be terminated, but ramrod will continue servicing other clients
 *     RAMROD_MSG_HANDLER_SUCCESS - the operation completed successfully and
 *       ramrod should shutdown.
 *     RAMROD_MSG_HANDLER_FAILED - an error was encountered while handling the
 *       message.  ramrod should shutdown
 */
enum {
    RAMROD_MSG_HANDLER_CONTINUE = 0,
    RAMROD_MSG_HANDLER_DONE,
    RAMROD_MSG_HANDLER_SUCCESS,
    RAMROD_MSG_HANDLER_FAILED,
};
typedef int (*ramrod_message_handler)(int sock, CFDictionaryRef message);

/*
 * ramrod_value_query_handler
 *   The ramrod protocol allows for simple by-key lookups from the host that
 *   are called "value queries".  The host specifies a key to look up and the
 *   device sends back a dictionary with a value for that key.
 *   Plugins can register value query handlers that will be used to handle
 *   keys that ramrod itself doesn't recognize.
 */   
typedef CFTypeRef (*ramrod_value_query_handler)(CFStringRef key);

/*
 * ramrod_autonomous_handler
 *   If a plugin registers a handler of this type, the handler will be
 *   invoked when ramrod enters its main connection handling loop.  If a
 *   plugin registers multiple handlers of this type (or multiple plugins
 *   register handles of this type) the order they are run is undefined (and
 *   they may run in parallel).
 *   The return codes are similar those for ramrod_message_handler, but since
 *   there is no client, the distinction between "continue" and "done" is 
 *   unnecessary.  The return values are:
 *     RAMROD_MAIN_CONTINUE - processing completed, ramrod should continue
 *       running
 *     RAMROD_MAIN_SUCCESS - the operation completed successfully and ramrod
 *       should reset the device
 *     RAMROD_MAIN_FAILED - an error was encountered and ramrod should reset the
 *       device
 */
enum {
    RAMROD_AUTONOMOUS_HANDLER_CONTINUE = 0,
    RAMROD_AUTONOMOUS_HANDLER_SUCCESS,
    RAMROD_AUTONOMOUS_HANDLER_FAILED,
};
typedef int (*ramrod_autonomous_handler)(void);


enum {
    RAMROD_MESSAGE_HANDLER,
    RAMROD_VALUE_QUERY_HANDLER,
    RAMROD_AUTONOMOUS_HANDLER,
};
struct ramrod_handler_map {
    int rhm_type;
    CFStringRef rhm_key;
    union {
        ramrod_message_handler message;
        ramrod_value_query_handler value_query;
        ramrod_autonomous_handler autonomous;
    } rhm_handler;
};


/* 
 * Convenience macros for declaring handlers.  Using these to initialize
 * the rhm_handler field should help avoid mismatches between the type and the
 * handler.  Don't use RAMROD_HANDLER directly.
 */
#define RAMROD_HANDLER(type, key, member, handler) \
    { .rhm_type = type, .rhm_key = key, .rhm_handler.member = handler }
#define RAMROD_MESSAGE_HANDLER(key, handler) \
    RAMROD_HANDLER(RAMROD_MESSAGE_HANDLER, key, message, handler)
#define RAMROD_VALUE_QUERY_HANDLER(key, handler) \
    RAMROD_HANDLER(RAMROD_VALUE_QUERY_HANDLER, key, value_query, handler)
#define RAMROD_AUTONOMOUS_HANDLER(key, handler) \
    RAMROD_HANDLER(RAMROD_AUTONOMOUS_HANDLER, key, autonomous, handler)

struct ramrod_plugin {
    int rp_version;
    const char *rp_name;
    int rp_num_handlers;
    struct ramrod_handler_map rp_handler_map[];
};


/*
 * ramrod_register_plugin_function
 *   ramrod plugins must implement the function 'ramrod_register_plugin' that
 *   that matches the type ramrod_register_plugin_function.  This function is
 *   called when the plugin is loaded and the implementation is expect to 
 *   populate plugin.
 *   If this function returns false, ramrod will continue as if the plugin
 *   was not present.
 */
typedef Boolean (*ramrod_register_plugin_function)(const struct ramrod_plugin **plugin);

/*
 * ramrod_register_plugin_opt_function
 *   'ramrod_register_plugin_opt' will be called by ramrod instead of 'ramrod_register_plugin' 
 *   when implemented by the plugin.  The semantics are the same except that 
 *   'ramrod_register_plugin_opt' allows the plugin to optionally return a dictionary of
 *   options to ramrod.  The dictionary should be retained on behalf of ramrod, and ramrod
 *   will release it when no longer needed.  For possible plugin options see below.
 */
typedef Boolean (*ramrod_register_plugin_opt_function)(const struct ramrod_plugin **plugin, CFDictionaryRef *plugin_options);

/*
 * plugin register options
 *
 *  RAMROD_REGISTER_OPTION_LOAD_SYSTEM_SEP
 *    A boolean indicating whether ramrod should attempt to load the
 *    SEP OS from the system partition.  The default is true for backwards
 *    compatibility with legacy plugins.  If any one of the plugin loaded
 *    by ramrod sets this option to false, the option will apply.
 */
RAMROD_EXPORT CFStringRef const RAMROD_REGISTER_OPTION_LOAD_SYSTEM_SEP;

/*
 * API available to ramrod plugins
 */

/* Error codes */
RAMROD_EXPORT CFStringRef const kCFErrorDomainRamrod;
typedef enum {
    RAMROD_ERR_NO_ERROR = 0,
    RAMROD_ERR_NAND_TIMEOUT = 1,
    RAMROD_ERR_INVALID_OPTION = 2,
    RAMROD_ERR_BAD_DATA = 3,
    RAMROD_ERR_SERVICE_NOT_FOUND = 4,
    RAMROD_ERR_STRING_CONVERSION = 5,
    RAMROD_ERR_ALLOCATION_FAILED = 6,
    RAMROD_ERR_COMMAND_FAILED = 7,
    RAMROD_ERR_NOT_FOUND = 8,
    
    /* Errors specific to the interface in ramrod_update.h */
    RAMROD_ERR_IMAGE3_WRITE_FAILURE = 1001,
    RAMROD_ERR_GAS_GAUGE_UPDATE_FAILURE = 1002,
    RAMROD_ERR_IR_MCU_UPDATE_FAILURE = 1003,
    RAMROD_ERR_BASEBAND_UPDATE_FAILURE = 1004,
    RAMROD_ERR_BASEBAND_QUERY_FAILURE = 1005,
    RAMROD_ERR_BASEBAND_SYSTEM_FILESYSTEM_NOT_MOUNTED = 1006,
    RAMROD_ERR_STOCKHOLM_UPDATE_FAILURE = 1007,
    RAMROD_ERR_SWD_UPDATE_FAILED = 1008,
    RAMROD_ERR_TCOP_UPDATE_FAILED = 1009,
    RAMROD_ERR_ORION_UPDATE_FAILED = 1010,
} ramrod_err_t;


/*
 * ramrod_log_msg
 * ramrod_log_msgv
 *   Log a message via ramrod.  It will be logged to both stdout and ramrod's
 *   internal buffer where it can be queried by the host.
 * ramrod_log_msg_noecho
 *   Behaves like ramrod_log_msg but does not log to stdout, only the internal
 *   log buffer.
 */
RAMROD_EXPORT void ramrod_log_msg(char const * const format, ...) __printflike(1, 2);
RAMROD_EXPORT void ramrod_log_msgv(char const * const format, va_list args);
RAMROD_EXPORT void ramrod_log_msg_noecho(char const * const format, ...) __printflike(1, 2);

/*
 * ramrod_add_log_fd
 * ramrod_remove_log_fd
 *   Add a file descriptor that will receive all the messages
 *   sent to ramrod_log_msg(...) call. Upon adding a new file descriptor
 *   all previously logged messages will be written to the file descriptor.
 *   If a file descriptor cannot be written to it is automatically passed to
 *   ramrod_remove_log_fd(...) and the file descriptor is closed.
 */
RAMROD_EXPORT int ramrod_add_log_fd(int fd);
RAMROD_EXPORT int ramrod_remove_log_fd(int fd);

/*
 * ramrod_log_set_buffermsgs
 *   Store logs from ramrod_log_msg() in memory buffers. If usebuffering is FALSE, the
 *   messages are only sent to registered fds and no buffering of the content.
 *   If usebuffering is TRUE(default), then ramrod_logmsg stores the log messages
 *   in memory buffers(consumes physical memory)
 */
RAMROD_EXPORT void ramrod_log_set_buffermsgs(bool usebuffering);

/*
 * ramrod_copy_log_buffer
 *   Returns a copy of the log buffer to this point.  The caller must free the 
 *   returned buffer.
 */
RAMROD_EXPORT char *ramrod_copy_log_buffer(void);

/*
 * ramrod_send_message
 *   Serialize the message and send it over the provided socket.  Returns 0
 *   on success, or an errno value on failure.
 */
RAMROD_EXPORT int ramrod_send_message(int sock, CFDictionaryRef message);

/*
 * ramrod_receive_message
 *   Block reading from the provided socket until a message is received.  The
 *   message is returned and the caller is responsible for releasing it. This
 *   function returns NULL if the socket is closed or an error occurs.
 */
RAMROD_EXPORT CFDictionaryRef ramrod_receive_message(int sock);

/*
 * ramrod_execute_command
 *   Execute a command.  This call waits for the command to finish.  Any output
 *   from the command is collected in the internal ramrod logging buffer.
 */
RAMROD_EXPORT int ramrod_execute_command(char const * const * const argv);

RAMROD_EXPORT int ramrod_execute_command_with_input_data(char const * const * const argv, void *input_data, size_t input_data_len);

RAMROD_EXPORT int ramrod_execute_command_get_cstring_result(char const * const * const argv, char *buf);
/*
 * ramrod_wait_for_internal_media
 *   Waits for all internal media (ie, NAND) to be available before returning.
 *   Note that this function calls ramrod_probe_media (see below) to determine
 *   the partition device nodes.
 */
RAMROD_EXPORT ramrod_err_t ramrod_wait_for_internal_media(void);

/*
 * ramrod_nand_requires_partitioning
 *
 *   Returns true iff the unit is intended to boot from nand and the
 *   nand device has not yet been partitioned. The return value is 
 *   cached on each call since repeated calls can become expensive.
 *   You can force refresh the cached result by passing false in 
 *   useCachedValue parameter. 
 */
RAMROD_EXPORT bool ramrod_nand_requires_partitioning(bool useCachedValue);

/*
 * ramrod_probe_media
 *   This function requests that ramrod re-discover any partitions on the
 *   device.  Plugins *must* call this function any time they perform an
 *   operation that might change the existence of partitions (for example,
 *   it would be necessary to call this function if one were to write a new
 *   partition map to the storage media)
 */
RAMROD_EXPORT Boolean ramrod_probe_media(CFErrorRef *error);

/*
 * ramrod_device_is_formatted_for_LwVM
 *   Returns true if the device has been formatted for LwVM 
 *   (LightweightVolumeManager)
 */
RAMROD_EXPORT Boolean ramrod_device_is_formatted_for_LwVM(void);

/*
 * ramrod_get_storage_media_device_node
 *   Populates the provided buffer with with the device node path for the
 *   unpartitioned storage media.
 */
RAMROD_EXPORT Boolean ramrod_get_storage_media_device_node(char *buf, size_t buflen);

/*
 * ramrod_get_system_partition_device_node
 * ramrod_get_data_partition_device_node
 * ramrod_get_update_partition_device_node
 * ramrod_get_baseband_data_partition_device_node
 * ramrod_get_log_partition_device_node
 *   Populates the provided buffer with with the device node path for the
 *   requested partition.
 *   These functions return false if the partition does not exist at all.
 */
RAMROD_EXPORT Boolean ramrod_get_system_partition_device_node(char *buf, size_t buflen);
RAMROD_EXPORT Boolean ramrod_get_data_partition_device_node(char *buf, size_t buflen);
RAMROD_EXPORT Boolean ramrod_get_update_partition_device_node(char *buf, size_t buflen);
RAMROD_EXPORT Boolean ramrod_get_baseband_data_partition_device_node(char *buf, size_t buflen);
RAMROD_EXPORT Boolean ramrod_get_log_partition_device_node(char *buf, size_t buflen);

/*
 * ramrod_mount_filesystem
 *   Mount a filesystem at a specified path.  fsck's the device prior to
 *   attempting to mount it.  Read-only variant mounts the filesystem as read-only.
 *   Returns 0 on success or an errno value on failure.
 *   XXX: Assumes the partition is HFS
 */
RAMROD_EXPORT int ramrod_mount_filesystem(const char *device_node, const char *mount_point);
RAMROD_EXPORT int ramrod_mount_filesystem_opt(const char *device_node, const char *mount_point, Boolean read_only);
RAMROD_EXPORT int ramrod_mount_filesystem_opt_err(const char *device_node, const char *mount_point, Boolean read_only, CFErrorRef *out_error);

/*
 * ramrod_unmount_filesystem
 *   Unmount a filesystem based on its mountpoint.  Returns 0 on success or an
 *   errno value on failure.
 */
RAMROD_EXPORT int ramrod_unmount_filesystem(const char *mount_point);

/*
 * ramrod_roll_media_keys
 *    Triggers the process to roll new media keys for encrypted lwvm partitions.
 *    This will only roll media keys on the Data partition if it is necessary.
 *    Returns -1 on immediate failure and the result of the call to rollMediaKey() if no immediate failure occurs 
 *    (returns 0 on success).
 */
RAMROD_EXPORT int ramrod_roll_media_keys(CFDictionaryRef options);

/*
 * ramrod_display_set_background_color
 *   Sets the background color of the display.  The RGB values are specified in
 *   the range 0-255
 */
RAMROD_EXPORT void ramrod_display_set_background_color(uint8_t r, uint8_t g, uint8_t b);

/*
 * ramrod_display_set_showprogress
 *   Setup whether progress bar should be displayed or not
 */
RAMROD_EXPORT void ramrod_display_set_showprogress(bool showprogress);


/*
 * ramrod_display_set_progress
 *   Update the displayed progress to the specified percent (0-100)
 */
RAMROD_EXPORT void ramrod_display_set_progress(int progress);

/*
 * ramrod_display_show_image
 *   Load the image at the specified path and draw it to the display device
 */
RAMROD_EXPORT void ramrod_display_show_image(CFStringRef path);

/*
 * ramrod_display_set_aux_image_path
 *   Load the image at the specified path and draw it at a fixed location under
 *   the progress display.  If path is NULL, any existing image is cleared.
 */
RAMROD_EXPORT bool ramrod_display_set_aux_image_path(CFStringRef path, CFErrorRef *err);

/*
 * ramrod_device_has_baseband
 *   Returns true if the device has a baseband
 */
RAMROD_EXPORT Boolean ramrod_device_has_baseband(void);

/*
 * ramrod_device_has_hoover
 *   Returns true if the device can bootstrap hoover
 */
RAMROD_EXPORT Boolean ramrod_device_has_hoover(void);

/*
 * ramrod_device_has_stockholm
 *   Returns true if the device has stockholm 
 */
RAMROD_EXPORT Boolean ramrod_device_has_stockholm(void);

/*
 * ramrod_create_udid_string
 *   Returns the device's UDID
 */
RAMROD_EXPORT CFStringRef ramrod_create_udid_string(void);

/*
 * ramrod_set_NVRAM_variable
 *   Set an NVRAM variable to the given value.  Returns true on success or false
 *   on failure.  On failure, err points to a CFErrorRef describing the failure.
 *   The caller must release this object.
 */
RAMROD_EXPORT bool ramrod_set_NVRAM_variable(CFStringRef var, CFStringRef val, CFErrorRef *err);

/*
 * ramrod_set_NVRAM_variable_sync
 *   Set an NVRAM variable to the given value optionally forcing an NVRAM sync.
 *   Forced sync is only sanctioned for setting auto-boot because NVRAM write lockout is bypassed.
 *   Returns true on success or false
 *   on failure.  On failure, err points to a CFErrorRef describing the failure.
 *   The caller must release this object.
 */
RAMROD_EXPORT bool ramrod_set_NVRAM_variable_sync(CFStringRef var, CFStringRef val, CFErrorRef *err, bool sync);

/*
 * ramrod_delete_NVRAM_variable
 *   Delete the named NVRAM variable.  Returns true on success or false
 *   on failure.  On failure, err points to a CFErrorRef describing the failure.
 *   The caller must release this object.
 */
RAMROD_EXPORT bool ramrod_delete_NVRAM_variable(CFStringRef var, CFErrorRef *err);

/*
 * ramrod_copy_NVRAM_variable
 *   Copy the value of the named NVRAM variable.  Returns a CF object (which the
 *   caller must release) containing the value of the variable.  NULL is
 *   returned if the variable does not exist or an error occurs.  In the latter
 *   case, err points to a CFErrorRef describing the failure.  The caller must 
 *   release the error object.
 */
RAMROD_EXPORT CFTypeRef ramrod_copy_NVRAM_variable(CFStringRef var, CFErrorRef *err);

/*
 * ramrod_clear_NVRAM_variables
 *   Clear the values of all NVRAM variables.
 */
RAMROD_EXPORT bool ramrod_clear_NVRAM_variables(CFErrorRef *err);

/*
 * asp_nand_set_writable
 *   Sets the property ASPSetWriteable to true on the ASPStorage class
 */
RAMROD_EXPORT int asp_nand_set_writable(void);

/*
 * dump_panic_logs
 *   Runs the DumpPanic tool which collects kernel panic logs
 */
RAMROD_EXPORT Boolean dump_panic_logs(const char *mount_point, CFErrorRef *error);

/*
 * ramrod_reprobe_device_path
 *   Open and close the device path specified to allow the kernel to reprobe the device
 */
RAMROD_EXPORT int ramrod_reprobe_device_path(const char * device_path, CFErrorRef *error);

/*
 * ramrod_raw_device_for_block_device
 *   Get the raw device node for a block device
 */
RAMROD_EXPORT int ramrod_raw_device_for_block_device(const char *block_dev_node, char *raw_dev_node, size_t len);

__END_DECLS

#endif
