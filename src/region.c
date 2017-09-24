/*!
 * \file region.c
 *
 * \author Copyright (C) 2013, 2014, 2015, 2017
 * by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for a DXF region entity (\c REGION).
 *
 * \since The region entity was introduced in DXF version R13.\n
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13.
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


#include "region.h"


/*!
 * \brief Allocate memory for a \c DxfRegion.
 *
 * Fill the memory contents with zeros.
 */
DxfRegion *
dxf_region_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfRegion *region = NULL;
        size_t size;

        size = sizeof (DxfRegion);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((region = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfRegion struct.\n")),
                  __FUNCTION__);
                region = NULL;
        }
        else
        {
                memset (region, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c REGION
 * entity.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfRegion *
dxf_region_init
(
        DxfRegion *region
                /*!< a pointer to the DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        int i;

        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                region = dxf_region_new ();
        }
        if (region == NULL)
        {
              fprintf (stderr,
                (_("Error in %s () could not allocate memory for a DxfRegion struct.\n")),
                __FUNCTION__);
              return (NULL);
        }
        region->modeler_format_version_number = 0;
        region->id_code = 0;
        region->linetype = strdup (DXF_DEFAULT_LINETYPE);
        region->layer = strdup (DXF_DEFAULT_LAYER);
        region->elevation = 0.0;
        region->thickness = 0.0;
        region->linetype_scale = DXF_DEFAULT_LINETYPE_SCALE;
        region->visibility = DXF_DEFAULT_VISIBILITY;
        region->color = DXF_COLOR_BYLAYER;
        region->paperspace = DXF_MODELSPACE;
        region->modeler_format_version_number = 1;
        for (i = 0; i < DXF_MAX_PARAM; i++)
        {
                region->proprietary_data[i] = strdup ("");
                region->additional_proprietary_data[i] = strdup ("");
        }
        region->dictionary_owner_soft = strdup ("");
        region->dictionary_owner_hard = strdup ("");
        region->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Read data from a DXF file into a DXF \c REGION entity.
 *
 * The last line read from file contained the string "REGION". \n
 * Now follows some data for the \c REGION, to be terminated with a "  0"
 * string announcing the following entity, or the end of the \c ENTITY
 * section marker \c ENDSEC. \n
 * While parsing the DXF file store data in \c region. \n
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
DxfRegion *
dxf_region_read
(
        DxfFile *fp,
                /*!< DXF file pointer to an input file (or device). */
        DxfRegion *region
                /*!< a pointer to the DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *temp_string = NULL;
        int i;
        int j;

        i = 0;
        j = 0;
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
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                region = dxf_region_new ();
                region = dxf_region_init (region);
        }
        (fp->line_number)++;
        fscanf (fp->fp, "%[^\n]", temp_string);
        while (strcmp (temp_string, "0") != 0)
        {
                if (ferror (fp->fp))
                {
                        fprintf (stderr,
                          (_("Error in %s () while reading from: %s in line: %d.\n")),
                          __FUNCTION__, fp->filename, fp->line_number);
                        fclose (fp->fp);
                        /* Clean up. */
                        free (temp_string);
                        return (NULL);
                }
                else if (strcmp (temp_string, "  1") == 0)
                {
                        /* Now follows a string containing proprietary
                         * data. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", region->proprietary_data[i]);
                        i++;
                }
                else if (strcmp (temp_string, "  3") == 0)
                {
                        /* Now follows a string containing additional
                         * proprietary data. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", region->additional_proprietary_data[j]);
                        i++;
                }
                if (strcmp (temp_string, "5") == 0)
                {
                        /* Now follows a string containing a sequential
                         * id number. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%x\n", &region->id_code);
                }
                else if (strcmp (temp_string, "6") == 0)
                {
                        /* Now follows a string containing a linetype
                         * name. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", region->linetype);
                }
                else if (strcmp (temp_string, "8") == 0)
                {
                        /* Now follows a string containing a layer name. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", region->layer);
                }
                else if ((fp->acad_version_number <= AutoCAD_11)
                        && (strcmp (temp_string, "38") == 0)
                        && (region->elevation != 0.0))
                {
                        /* Now follows a string containing the
                         * elevation. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &region->elevation);
                }
                else if (strcmp (temp_string, "39") == 0)
                {
                        /* Now follows a string containing the
                         * thickness. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &region->thickness);
                }
                else if (strcmp (temp_string, "48") == 0)
                {
                        /* Now follows a string containing the linetype
                         * scale. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &region->linetype_scale);
                }
                else if (strcmp (temp_string, "60") == 0)
                {
                        /* Now follows a string containing the
                         * visibility value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%hd\n", &region->visibility);
                }
                else if (strcmp (temp_string, "62") == 0)
                {
                        /* Now follows a string containing the
                         * color value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &region->color);
                }
                else if (strcmp (temp_string, "67") == 0)
                {
                        /* Now follows a string containing the
                         * paperspace value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &region->paperspace);
                }
                else if ((fp->acad_version_number >= AutoCAD_13)
                        && (strcmp (temp_string, "70") == 0))
                {
                        /* Now follows a string containing the modeler
                         * format version number. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &region->modeler_format_version_number);
                }
                else if ((fp->acad_version_number >= AutoCAD_13)
                        && (strcmp (temp_string, "100") == 0))
                {
                        /* Now follows a string containing the
                         * subclass marker value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", temp_string);
                        if (strcmp (temp_string, "AcDbModelerGeometry") != 0)
                        {
                                fprintf (stderr, "Error in dxf_region_read () found a bad subclass marker in: %s in line: %d.\n",
                                        fp->filename, fp->line_number);
                        }
                }
                else if (strcmp (temp_string, "330") == 0)
                {
                        /* Now follows a string containing Soft-pointer
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", region->dictionary_owner_soft);
                }
                else if (strcmp (temp_string, "360") == 0)
                {
                        /* Now follows a string containing Hard owner
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", region->dictionary_owner_hard);
                }
                else if (strcmp (temp_string, "999") == 0)
                {
                        /* Now follows a string containing a comment. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", temp_string);
                        fprintf (stdout, "DXF comment: %s\n", temp_string);
                }
                else
                {
                        fprintf (stderr,
                          (_("Warning in %s () unknown string tag found while reading from: %s in line: %d.\n")),
                          __FUNCTION__, fp->filename, fp->line_number);
                }
        }
        /* Handle omitted members and/or illegal values. */
        if (strcmp (region->linetype, "") == 0)
        {
                region->linetype = strdup (DXF_DEFAULT_LINETYPE);
        }
        if (strcmp (region->layer, "") == 0)
        {
                region->layer = strdup (DXF_DEFAULT_LAYER);
        }
        /* Clean up. */
        free (temp_string);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Write DXF output to a file for a DXF \c REGION entity.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred while reading from the input file.
 */
int
dxf_region_write
(
        DxfFile *fp,
                /*!< DXF file pointer to an output file (or device). */
        DxfRegion *region
                /*!< a pointer to the DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *dxf_entity_name = strdup ("REGION");
        int i;

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
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (fp->acad_version_number < AutoCAD_13)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this %s entity with id-code: %x.\n")),
                  __FUNCTION__, dxf_entity_name, region->id_code);
        }
        if (strcmp (region->linetype, "") == 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () empty linetype string for the %s entity with id-code: %x\n")),
                  __FUNCTION__, dxf_entity_name, region->id_code);
                fprintf (stderr,
                  (_("\t%s entity is reset to default linetype")),
                  dxf_entity_name);
                region->linetype = strdup (DXF_DEFAULT_LINETYPE);
        }
        if (strcmp (region->layer, "") == 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () empty layer string for the %s entity with id-code: %x\n")),
                  __FUNCTION__, dxf_entity_name, region->id_code);
                fprintf (stderr,
                  (_("\t%s entity is relocated to layer 0")),
                  dxf_entity_name);
                region->layer = strdup (DXF_DEFAULT_LAYER);
        }
        /* Start writing output. */
        fprintf (fp->fp, "  0\n%s\n", dxf_entity_name);
        if (region->id_code != -1)
        {
                fprintf (fp->fp, "  5\n%x\n", region->id_code);
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
        if ((strcmp (region->dictionary_owner_soft, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_REACTORS\n");
                fprintf (fp->fp, "330\n%s\n", region->dictionary_owner_soft);
                fprintf (fp->fp, "102\n}\n");
        }
        if ((strcmp (region->dictionary_owner_hard, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_XDICTIONARY\n");
                fprintf (fp->fp, "360\n%s\n", region->dictionary_owner_hard);
                fprintf (fp->fp, "102\n}\n");
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, "100\nAcDbEntity\n");
        }
        if (region->paperspace == DXF_PAPERSPACE)
        {
                fprintf (fp->fp, " 67\n%d\n", DXF_PAPERSPACE);
        }
        fprintf (fp->fp, "  8\n%s\n", region->layer);
        if (strcmp (region->linetype, DXF_DEFAULT_LINETYPE) != 0)
        {
                fprintf (fp->fp, "  6\n%s\n", region->linetype);
        }
        if ((fp->acad_version_number <= AutoCAD_11)
          && DXF_FLATLAND
          && (region->elevation != 0.0))
        {
                fprintf (fp->fp, " 38\n%f\n", region->elevation);
        }
        if (region->color != DXF_COLOR_BYLAYER)
        {
                fprintf (fp->fp, " 62\n%d\n", region->color);
        }
        if (region->linetype_scale != 1.0)
        {
                fprintf (fp->fp, " 48\n%f\n", region->linetype_scale);
        }
        if (region->visibility != 0)
        {
                fprintf (fp->fp, " 60\n%d\n", region->visibility);
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, "100\nAcDbModelerGeometry\n");
        }
        if (region->thickness != 0.0)
        {
                fprintf (fp->fp, " 39\n%f\n", region->thickness);
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, " 70\n%d\n", region->modeler_format_version_number);
        }
        i = 0;
        while (strlen (region->proprietary_data[i]) > 0)
        {
                fprintf (fp->fp, "  1\n%s\n", region->proprietary_data[i]);
                i++;
        }
        i = 0;
        while (strlen (region->additional_proprietary_data[i]) > 0)
        {
                fprintf (fp->fp, "  3\n%s\n", region->additional_proprietary_data[i]);
                i++;
        }
        /* Clean up. */
        free (dxf_entity_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a DXF \c REGION and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_region_free
(
        DxfRegion *region
                /*!< a pointer to the memory occupied by the DXF
                 * \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        int i;

        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (region->next != NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () pointer to next was not NULL.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        free (region->linetype);
        free (region->layer);
        for (i = 0; i < DXF_MAX_PARAM; i++)
        {
                free (region->proprietary_data[i]);
                free (region->additional_proprietary_data[i]);
        }
        free (region->dictionary_owner_soft);
        free (region->dictionary_owner_hard);
        free (region);
        region = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a chain of DXF \c REGION
 * entities and all their data fields.
 */
void
dxf_region_free_chain
(
        DxfRegion *regions
                /*!< a pointer to the chain of DXF \c REGION entities. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (regions == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
        }
        while (regions != NULL)
        {
                struct DxfRegion *iter = regions->next;
                dxf_region_free (regions);
                regions = (DxfRegion *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
}


/*!
 * \brief Get the \c id_code from a DXF \c REGION entity.
 *
 * \return \c id_code.
 */
int
dxf_region_get_id_code
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (region->id_code < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was found.\n")),
                  __FUNCTION__);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region->id_code);
}


/*!
 * \brief Set the \c id_code for a DXF \c REGION entity.
 */
DxfRegion *
dxf_region_set_id_code
(
        DxfRegion *region,
                /*!< a pointer to a DXF \c REGION entity. */
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
        if (region == NULL)
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
        region->id_code = id_code;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Get the \c linetype from a DXF \c REGION entity.
 *
 * \return \c linetype when sucessful, \c NULL when an error occurred.
 */
char *
dxf_region_get_linetype
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (region->linetype ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (region->linetype));
}


/*!
 * \brief Set the \c linetype for a DXF \c REGION entity.
 */
DxfRegion *
dxf_region_set_linetype
(
        DxfRegion *region,
                /*!< a pointer to a DXF \c REGION entity. */
        char *linetype
                /*!< a pointer to a string containing the \c linetype
                 * to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (linetype == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        region->linetype = strdup (linetype);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Get the \c layer from a DXF \c REGION entity.
 *
 * \return \c layer when sucessful, \c NULL when an error occurred.
 */
char *
dxf_region_get_layer
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (region->layer ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (region->layer));
}


/*!
 * \brief Set the \c layer for a DXF \c REGION entity.
 */
DxfRegion *
dxf_region_set_layer
(
        DxfRegion *region,
                /*!< a pointer to a DXF \c REGION entity. */
        char *layer
                /*!< a pointer to a string containing the \c layer to be
                 * set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (layer == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        region->layer = strdup (layer);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Get the \c elevation from a DXF \c REGION entity.
 *
 * \return \c elevation.
 */
double
dxf_region_get_elevation
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region->elevation);
}


/*!
 * \brief Set the \c elevation for a DXF \c REGION entity.
 */
DxfRegion *
dxf_region_set_elevation
(
        DxfRegion *region,
                /*!< a pointer to a DXF \c REGION entity. */
        double elevation
                /*!< the \c elevation to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        region->elevation = elevation;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Get the \c thickness from a DXF \c REGION entity.
 *
 * \return \c thickness.
 */
double
dxf_region_get_thickness
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (region->thickness < 0.0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region->thickness);
}


/*!
 * \brief Set the \c thickness for a DXF \c REGION entity.
 */
DxfRegion *
dxf_region_set_thickness
(
        DxfRegion *region,
                /*!< a pointer to a DXF \c REGION entity. */
        double thickness
                /*!< the \c thickness to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (thickness < 0.0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        region->thickness = thickness;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Get the \c linetype_scale from a DXF \c REGION entity.
 *
 * \return \c linetype_scale.
 */
double
dxf_region_get_linetype_scale
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (region->linetype_scale < 0.0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region->linetype_scale);
}


/*!
 * \brief Set the \c linetype_scale for a DXF \c REGION entity.
 */
DxfRegion *
dxf_region_set_linetype_scale
(
        DxfRegion *region,
                /*!< a pointer to a DXF \c REGION entity. */
        double linetype_scale
                /*!< the \c linetype_scale to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (linetype_scale < 0.0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        region->linetype_scale = linetype_scale;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Get the \c visibility from a DXF \c REGION entity.
 *
 * \return \c visibility.
 */
int16_t
dxf_region_get_visibility
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (region->visibility < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (region->visibility > 1)
        {
                fprintf (stderr,
                  (_("Error in %s () an out of range value was found.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region->visibility);
}


/*!
 * \brief Set the \c visibility for a DXF \c REGION entity.
 */
DxfRegion *
dxf_region_set_visibility
(
        DxfRegion *region,
                /*!< a pointer to a DXF \c REGION entity. */
        int16_t visibility
                /*!< the \c visibility to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (visibility < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (visibility > 1)
        {
                fprintf (stderr,
                  (_("Error in %s () an out of range value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        region->visibility = visibility;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Get the \c color from a DXF \c REGION entity.
 *
 * \return \c color.
 */
int
dxf_region_get_color
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (region->color < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was found.\n")),
                  __FUNCTION__);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region->color);
}


/*!
 * \brief Set the \c color for a DXF \c REGION entity.
 */
DxfRegion *
dxf_region_set_color
(
        DxfRegion *region,
                /*!< a pointer to a DXF \c REGION entity. */
        int color
                /*!< the \c color to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (color < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was passed.\n")),
                  __FUNCTION__);
        }
        region->color = color;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region);
}


/*!
 * \brief Get the \c paperspace flag value from a DXF \c REGION entity.
 *
 * \return \c paperspace flag value.
 */
int
dxf_region_get_paperspace
(
        DxfRegion *region
                /*!< a pointer to a DXF \c REGION entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (region == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (region->paperspace < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was found.\n")),
                  __FUNCTION__);
        }
        if (region->paperspace > 1)
        {
                fprintf (stderr,
                  (_("Warning in %s () an out of range value was found.\n")),
                  __FUNCTION__);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (region->paperspace);
}


/* EOF */
