/*
 *  ramrod_ticket.h
 *  PurpleRestore
 *
 *  Created by Jason Gosnell on 1/20/11.
 *  Copyright 2011 Apple Inc. All rights reserved.
 *
 */

#if !defined(RAMROD_TICKET_H)
#define RAMROD_TICKET_H

#include <sys/cdefs.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ramrod/ramrod_base.h>

__BEGIN_DECLS

/*
 * ramrod_ticket_update
 *  Not thread safe
 */
RAMROD_EXPORT Boolean ramrod_ticket_update(CFDictionaryRef options,
                                           CFDataRef (*copy_ticket)(void *context),
                                           void *context,
                                           CFErrorRef *error);

/*
 * ramrod_ticket_write
 *  Not thread safe
 */
RAMROD_EXPORT Boolean ramrod_ticket_write(char *system_root,
                                          CFErrorRef *error);

/*
 * ramrod_ticket_copy
 *  Not thread safe
 */
RAMROD_EXPORT CFDataRef ramrod_ticket_copy(void);

/*
 * ramrod_ticket_has_boolean_entitlement.
 *  Returns true if the current AP ticket has the entitlement for the specified
 *  Img4 tag.
 *  Only valid on devices with an Img4 ticket.
 */
RAMROD_EXPORT bool ramrod_ticket_has_boolean_entitlement(uint32_t tag);

/*
 * ramrod_ticket_copy_data_object_property.
 *  Returns the value of a data property of an object found in the Img4 ticket.
 *  Only valid on devices with an Img4 ticket.
 */
RAMROD_EXPORT CFDataRef ramrod_ticket_copy_data_object_property(uint64_t objectTag, uint64_t propertyTag);

__END_DECLS

#endif
