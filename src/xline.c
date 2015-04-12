/*!
 * \file xline.c
 *
 * \author Copyright (C) 2015 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for a DXF xline entity (\c XLINE).
 *
 * <hr>
 * <h1><b>Copyright Notices.</b></h1>\n
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.\n\n
 * This program is distributed in the hope that it will be useful, but
 * <b>WITHOUT ANY WARRANTY</b>; without even the implied warranty of
 * <b>MERCHANTABILITY</b> or <b>FITNESS FOR A PARTICULAR PURPOSE</b>.\n
 * See the GNU General Public License for more details.\n\n
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to:\n
 * Free Software Foundation, Inc.,\n
 * 59 Temple Place,\n
 * Suite 330,\n
 * Boston,\n
 * MA 02111 USA.\n
 * \n
 * Drawing eXchange Format (DXF) is a defacto industry standard for the
 * exchange of drawing files between various Computer Aided Drafting
 * programs.\n
 * DXF is an industry standard designed by Autodesk(TM).\n
 * For more details see http://www.autodesk.com .
 * <hr>
 */


#include "xline.h"


/*!
 * \brief Allocate memory for a \c DxfXLine.
 *
 * Fill the memory contents with zeros.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13.
 * \version According to DXF R14.
 */
DxfXLine *
dxf_xline_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfXLine *xline = NULL;
        size_t size;

        size = sizeof (DxfXLine);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((xline = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfXLine struct.\n")),
                  __FUNCTION__);
                xline = NULL;
        }
        else
        {
                memset (xline, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (xline);
}


/*!
 * \brief Allocate memory and initialize data fields in an \c XLINE entity.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13.
 * \version According to DXF R14.
 */
DxfXLine *
dxf_xline_init
(
        DxfXLine *xline
                /*!< DXF line entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (xline == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                xline = dxf_xline_new ();
        }
        if (xline == NULL)
        {
              fprintf (stderr,
                (_("Error in %s () could not allocate memory for a DxfXLine struct.\n")),
                __FUNCTION__);
              return (NULL);
        }
        xline->id_code = 0;
        xline->linetype = strdup (DXF_DEFAULT_LINETYPE);
        xline->layer = strdup (DXF_DEFAULT_LAYER);
        xline->x0 = 0.0;
        xline->y0 = 0.0;
        xline->z0 = 0.0;
        xline->x1 = 0.0;
        xline->y1 = 0.0;
        xline->z1 = 0.0;
        xline->elevation = 0.0;
        xline->thickness = 0.0;
        xline->linetype_scale = DXF_DEFAULT_LINETYPE_SCALE;
        xline->visibility = DXF_DEFAULT_VISIBILITY;
        xline->color = DXF_COLOR_BYLAYER;
        xline->paperspace = DXF_MODELSPACE;
        xline->dictionary_owner_soft = strdup ("");
        xline->dictionary_owner_hard = strdup ("");
        xline->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (xline);
}


/*!
 * \brief Free the allocated memory for a DXF \c XLINE and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13.
 * \version According to DXF R14.
 */
int
dxf_xline_free
(
        DxfXLine *xline
                /*!< Pointer to the memory occupied by the DXF \c XLINE
                 * entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (xline->next != NULL)
        {
              fprintf (stderr,
                (_("Error in %s () pointer to next DxfXLine was not NULL.\n")),
                __FUNCTION__);
              return (EXIT_FAILURE);
        }
        free (xline->linetype);
        free (xline->layer);
        free (xline->dictionary_owner_soft);
        free (xline->dictionary_owner_hard);
        free (xline);
        xline = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/* EOF */
