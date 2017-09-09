/*!
 * \file rastervariables.c
 *
 * \author Copyright (C) 2015, 2017 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for a DXF rastervariables object (\c RASTERVARIABLES).
 *
 * \since The \c RASTERVARIABLES object was introduced in DXF R14.
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13 (backward compatibility).
 * \version According to DXF R14.
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
 * For more details see http://www.autodesk.com.
 * <hr>
 */


#include "rastervariables.h"


/*!
 * \brief Allocate memory for a \c DxfRasterVariables.
 *
 * Fill the memory contents with zeros.
 */
DxfRasterVariables *
dxf_rastervariables_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfRasterVariables *rastervariables = NULL;
        size_t size;

        size = sizeof (DxfRasterVariables);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((rastervariables = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfRasterVariables struct.\n")),
                  __FUNCTION__);
                rastervariables = NULL;
        }
        else
        {
                memset (rastervariables, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (rastervariables);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c RASTERVARIABLES
 * object.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfRasterVariables *
dxf_rastervariables_init
(
        DxfRasterVariables *rastervariables
                /*!< a pointer to the DXF \c RASTERVARIABLES object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (rastervariables == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                rastervariables = dxf_rastervariables_new ();
        }
        if (rastervariables == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfRasterVariables struct.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        rastervariables->id_code = 0;
        rastervariables->dictionary_owner_soft = strdup ("");
        rastervariables->dictionary_owner_hard = strdup ("");
        rastervariables->display_image_frame = 0;
        rastervariables->display_quality = 0;
        rastervariables->units = 0;
        rastervariables->class_version = 0;
        rastervariables->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (rastervariables);
}


/*!
 * \brief Read data from a DXF file into a DXF \c RASTERVARIABLES object.
 *
 * The last line read from file contained the string "RASTERVARIABLES". \n
 * Now follows some data for the \c RASTERVARIABLES, to be terminated with a
 * "  0" string announcing the following entity, or the end of the
 * \c OBJECTS section marker \c ENDSEC. \n
 * While parsing the DXF file store data in \c rastervariables. \n
 *
 * \return a pointer to \c rastervariables.
 */
DxfRasterVariables *
dxf_rastervariables_read
(
        DxfFile *fp,
                /*!< DXF file pointer to an input file (or device). */
        DxfRasterVariables *rastervariables
                /*!< a pointer to the DXF \c RASTERVARIABLES object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *temp_string = NULL;
        int i;

        /* Do some basic checks. */
        if (fp == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL file pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (temp_string);
                return (NULL);
        }
        if (fp->acad_version_number < AutoCAD_14)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this entity.\n")),
                  __FUNCTION__);
        }
        if (rastervariables == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                rastervariables = dxf_rastervariables_new ();
                rastervariables = dxf_rastervariables_init (rastervariables);
        }
        i = 0;
        (fp->line_number)++;
        fscanf (fp->fp, "%[^\n]", temp_string);
        while (strcmp (temp_string, "0") != 0)
        {
                if (ferror (fp->fp))
                {
                        fprintf (stderr,
                          (_("Error in %s () while reading from: %s in line: %d.\n")),
                          __FUNCTION__, fp->filename, fp->line_number);
                        /* Clean up. */
                        free (temp_string);
                        fclose (fp->fp);
                        return (NULL);
                }
                if (strcmp (temp_string, "5") == 0)
                {
                        /* Now follows a string containing a sequential
                         * id number. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%x\n", &rastervariables->id_code);
                }
                if (strcmp (temp_string, "70") == 0)
                {
                        /* Now follows a string containing a display
                         * image frame flag value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &rastervariables->display_image_frame);
                }
                if (strcmp (temp_string, "71") == 0)
                {
                        /* Now follows a string containing a display
                         * quality flag value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &rastervariables->display_quality);
                }
                if (strcmp (temp_string, "72") == 0)
                {
                        /* Now follows a string containing a units
                         * value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &rastervariables->units);
                }
                else if (strcmp (temp_string, "90") == 0)
                {
                        /* Now follows a string containing the
                         * value of class version. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &rastervariables->class_version);
                }
                else if ((fp->acad_version_number >= AutoCAD_13)
                        && (strcmp (temp_string, "100") == 0))
                {
                        /* Now follows a string containing the
                         * subclass marker value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", temp_string);
                        if (strcmp (temp_string, "AcDbIdBuffer") != 0)
                        {
                                fprintf (stderr,
                                  (_("Warning in %s () found a bad subclass marker in: %s in line: %d.\n")),
                                  __FUNCTION__, fp->filename, fp->line_number);
                        }
                }
                else if (strcmp (temp_string, "330") == 0)
                {
                        /* Now follows a string containing Soft-pointer
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", rastervariables->dictionary_owner_soft);
                        i++;
                }
                else if (strcmp (temp_string, "360") == 0)
                {
                        /* Now follows a string containing Hard owner
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", rastervariables->dictionary_owner_hard);
                }
                else if (strcmp (temp_string, "999") == 0)
                {
                        /* Now follows a string containing a comment. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", temp_string);
                        fprintf (stdout, (_("DXF comment: %s\n")), temp_string);
                }
                else
                {
                        fprintf (stderr,
                          (_("Warning in %s () unknown string tag found while reading from: %s in line: %d.\n")),
                          __FUNCTION__, fp->filename, fp->line_number);
                }
        }
        /* Clean up. */
        free (temp_string);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (rastervariables);
}


/*!
 * \brief Write DXF output to a file for a DXF \c RASTERVARIABLES object.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_rastervariables_write
(
        DxfFile *fp,
                /*!< DXF file pointer to an output file (or device). */
        DxfRasterVariables *rastervariables
                /*!< a pointer to the DXF \c RASTERVARIABLES object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *dxf_entity_name = strdup ("RASTERVARIABLES");

        /* Do some basic checks. */
        if (fp == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL file pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (rastervariables == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (fp->acad_version_number < AutoCAD_14)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this %s entity with id-code: %x.\n")),
                  __FUNCTION__, dxf_entity_name, rastervariables->id_code);
        }
        /* Start writing output. */
        fprintf (fp->fp, "  0\n%s\n", dxf_entity_name);
        if (rastervariables->id_code != -1)
        {
                fprintf (fp->fp, "  5\n%x\n", rastervariables->id_code);
        }
        /*!
         * \todo for version R14.\n
         * Implementing the start of application-defined group
         * "{application_name", with Group code 102.\n
         * For example: "{ACAD_REACTORS" indicates the start of the
         * AutoCAD persistent reactors group.\n\n
         * application-defined codes: Group codes and values within the
         * 102 groups are application defined (optional).\n\n
         * End of group, "}" (optional), with Group code 102.
         */
        if ((strcmp (rastervariables->dictionary_owner_soft, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_REACTORS\n");
                fprintf (fp->fp, "330\n%s\n", rastervariables->dictionary_owner_soft);
                fprintf (fp->fp, "102\n}\n");
        }
        if ((strcmp (rastervariables->dictionary_owner_hard, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_XDICTIONARY\n");
                fprintf (fp->fp, "360\n%s\n", rastervariables->dictionary_owner_hard);
                fprintf (fp->fp, "102\n}\n");
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, "100\nAcDbRasterVariables\n");
        }
        fprintf (fp->fp, " 90\n%d\n", rastervariables->class_version);
        fprintf (fp->fp, " 70\n%d\n", rastervariables->display_image_frame);
        fprintf (fp->fp, " 71\n%d\n", rastervariables->display_quality);
        fprintf (fp->fp, " 72\n%d\n", rastervariables->units);
        /* Clean up. */
        free (dxf_entity_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a DXF \c RASTERVARIABLES and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_rastervariables_free
(
        DxfRasterVariables *rastervariables
                /*!< a pointer to the memory occupied by the DXF
                 * \c RASTERVARIABLES object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (rastervariables == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (rastervariables->next != NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () pointer to next was not NULL.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        free (rastervariables->dictionary_owner_soft);
        free (rastervariables->dictionary_owner_hard);
        free (rastervariables);
        rastervariables = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a chain of DXF \c RASTERVARIABLES
 * objects and all their data fields.
 */
void
dxf_rastervariables_free_chain
(
        DxfRasterVariables *rastervariables
                /*!< a pointer to the chain of DXF \c RASTERVARIABLES
                 * objects. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (rastervariables == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
        }
        while (rastervariables != NULL)
        {
                struct DxfRasterVariables *iter = rastervariables->next;
                dxf_rastervariables_free (rastervariables);
                rastervariables = (DxfRasterVariables *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
}


/*!
 * \brief Get the \c id_code from a DXF \c RASTERVARIABLES object.
 *
 * \return \c id_code.
 */
int
dxf_rastervariables_get_id_code
(
        DxfRasterVariables *rastervariables
                /*!< a pointer to a DXF \c RASTERVARIABLES object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (rastervariables == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (rastervariables->id_code < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was found.\n")),
                  __FUNCTION__);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (rastervariables->id_code);
}


/*!
 * \brief Set the \c id_code for a DXF \c RASTERVARIABLES object.
 */
DxfRasterVariables *
dxf_rastervariables_set_id_code
(
        DxfRasterVariables *rastervariables,
                /*!< a pointer to a DXF \c RASTERVARIABLES object. */
        int id_code
                /*!< the \c id_code to be set for the entity.\n
                 * This is to be an unique (sequential) number in the
                 * DXF file. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (rastervariables == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (id_code < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was passed.\n")),
                  __FUNCTION__);
        }
        rastervariables->id_code = id_code;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (rastervariables);
}


/* EOF*/
