/*
 * ramrod_base.h
 *   Basic declarations used by other headers
 */

#if !defined(RAMROD_BASE_H)
#define RAMROD_BASE_H

/* Any function that should be exported for plugin use should be marked */
#define RAMROD_EXPORT extern __attribute__((visibility ("default")))

#endif
