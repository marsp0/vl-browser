/*
 * Notes
 * 
 */

/********************/
/*     includes     */
/********************/

#include "time_utils.h"

#include <assert.h>

/********************/
/*      defines     */
/********************/


/********************/
/* static variables */
/********************/


/********************/
/* static functions */
/********************/


/********************/
/* public functions */
/********************/

struct timespec time_diff(struct timespec start, struct timespec end)
{
    assert((start.tv_sec <= end.tv_sec) && "start cannot be bigger than end");

    struct timespec diff = { 0 };
    diff.tv_sec = end.tv_sec - start.tv_sec;

    if (end.tv_nsec < start.tv_nsec)
    {
        diff.tv_sec--;
        diff.tv_nsec = 1000000000L - (start.tv_nsec - end.tv_nsec);
    }
    else
    {
        diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    return diff;
}