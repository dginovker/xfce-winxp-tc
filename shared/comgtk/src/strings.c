#include <glib.h>
#include <string.h>

#include "strings.h"

//
// PUBLIC FUNCTIONS
//
gint wintc_strstr_count(
    const gchar* haystack,
    const gchar* needle
)
{
    gint   advance = strlen(needle);
    gint   count;
    gchar* pchar   = haystack;

    while (pchar != NULL)
    {
        pchar = strstr(pchar, needle);

        if (pchar != NULL)
        {
            count++;
            pchar += advance;
        }
    }

    return count;
}

gboolean wintc_strsteal(
    gchar** dest,
    gchar** src
)
{
    if (dest != NULL)
    {
        g_free((*dest));
        *dest = *src;
    }

    *src = NULL;
}

gchar* wintc_strsubst(
    const gchar* str,
    const gchar* findwhat,
    const gchar* replacewith
)
{
    gchar* buffer;
    gint   findsize     = strlen(findwhat);
    gint   replacecount = wintc_strstr_count(str, findwhat);
    gint   replacesize  = strlen(replacewith);
    gchar* pbuffer;
    gchar* pchar        = str;
    gchar* pnext        = NULL;
    gint   required;
    gint   strsize      = strlen(str);
    gint   tocopy;

    // Work out how much space we need
    //
    required = strsize - (findsize * replacecount) + (replacesize * replacecount) + 1;
    buffer   = g_new0(gchar, required);

    pbuffer = buffer;

    // Build the string now
    //
    while (pchar != NULL)
    {
        // Find where the replacement begins
        //
        pnext = strstr(pchar, findwhat);

        if (pnext == NULL)
        {
            // No more? Add the rest of the string
            //
            tocopy = strsize - (pchar - str);

            memcpy(pbuffer, pchar, tocopy);
        }
        else
        {
            // Add the next chunk with the replacement and advance
            //
            tocopy = pnext - pchar;

            memcpy(pbuffer, pchar, tocopy);
            pbuffer += tocopy;

            memcpy(pbuffer, replacewith, replacesize);
            pbuffer += replacesize;

            pnext += findsize;
        }

        pchar = pnext;
    }

    return buffer;
}
