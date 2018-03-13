/*!
 * \file body.c
 *
 * \author Copyright (C) 2013, 2014, 2015, 2016, 2017
 * by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for a DXF body entity (\c BODY).
 *
 * \warning The \c BODY entity requires AutoCAD version R13 or higher.\n
 *
 * \version According to DXF R10 (backward compatibility).
 * \version According to DXF R11 (backward compatibility).
 * \version According to DXF R12 (backward compatibility).
 * \version According to DXF R13.
 * \version According to DXF R14.
 *
 * <hr>
 * <h1><b>Copyright Notices.</b></h1>\n
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.\n
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.\n
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to:\n
 * the Free Software Foundation, Inc.,\n
 * 51 Franklin Street,\n
 * Fifth Floor,\n
 * Boston,\n
 * MA 02110-1301 USA.\n
 *
 * Drawing eXchange Format (DXF) is a defacto industry standard for the
 * exchange of drawing files between various Computer Aided Drafting
 * programs.\n
 * DXF is an industry standard designed by Autodesk(TM).\n
 * For more details see http://www.autodesk.com.
 * <hr>
 */


#include "body.h"


/*!
 * \brief Allocate memory for a \c DxfBody.
 *
 * Fill the memory contents with zeros.
 */
DxfBody *
dxf_body_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfBody *body = NULL;
        size_t size;

        size = sizeof (DxfBody);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((body = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfBody struct.\n")),
                  __FUNCTION__);
                body = NULL;
        }
        else
        {
                memset (body, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c BODY
 * entity.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfBody *
dxf_body_init
(
        DxfBody *body
                /*!< DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                body = dxf_body_new ();
        }
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfBody struct.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        dxf_body_set_id_code (body, 0);
        dxf_body_set_linetype (body, strdup (DXF_DEFAULT_LINETYPE));
        dxf_body_set_layer (body, strdup (DXF_DEFAULT_LAYER));
        dxf_body_set_elevation (body, 0.0);
        dxf_body_set_thickness (body, 0.0);
        dxf_body_set_linetype_scale (body, DXF_DEFAULT_LINETYPE_SCALE);
        dxf_body_set_visibility (body, DXF_DEFAULT_VISIBILITY);
        dxf_body_set_color (body, DXF_COLOR_BYLAYER);
        dxf_body_set_paperspace (body, DXF_MODELSPACE);
        dxf_body_set_graphics_data_size (body, 0);
        dxf_body_set_shadow_mode (body, 0);
        dxf_body_set_binary_graphics_data (body, (DxfBinaryGraphicsData *) dxf_binary_graphics_data_init (body->binary_graphics_data));
        dxf_binary_graphics_data_init ((DxfBinaryGraphicsData *) dxf_body_get_binary_graphics_data (body));
        dxf_body_set_dictionary_owner_soft (body, strdup (""));
        dxf_body_set_material (body, strdup (""));
        dxf_body_set_dictionary_owner_hard (body, strdup (""));
        dxf_body_set_lineweight (body, 0);
        dxf_body_set_plot_style_name (body, strdup (""));
        dxf_body_set_color_value (body, 0);
        dxf_body_set_color_name (body, strdup (""));
        dxf_body_set_transparency (body, 0);
        dxf_body_set_proprietary_data (body, (DxfProprietaryData *) dxf_proprietary_data_init (body->proprietary_data));
        dxf_body_set_proprietary_data (body, (DxfProprietaryData *) dxf_proprietary_data_init (body->additional_proprietary_data));
        dxf_body_set_modeler_format_version_number (body, 1);
        dxf_body_set_next (body, NULL);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Read data from a DXF file into a DXF \c BODY entity.
 *
 * The last line read from file contained the string "BODY". \n
 * Now follows some data for the \c BODY, to be terminated with a "  0"
 * string announcing the following entity, or the end of the \c ENTITY
 * section marker \c ENDSEC. \n
 * While parsing the DXF file store data in \c body. \n
 *
 * \return a pointer to \c body.
 */
DxfBody *
dxf_body_read
(
        DxfFile *fp,
                /*!< DXF file pointer to an input file (or device). */
        DxfBody *body
                /*!< DXF \c BODY entity. */
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
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                body = dxf_body_new ();
                body = dxf_body_init (body);
        }
        if (fp->acad_version_number < AutoCAD_13)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this entity.\n")),
                  __FUNCTION__);
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
                        fscanf (fp->fp, "%s\n", body->proprietary_data->line);
                        body->proprietary_data->order = i;
                        i++;
                        dxf_proprietary_data_init ((DxfProprietaryData *) body->proprietary_data->next);
                        body->proprietary_data = (DxfProprietaryData *) body->proprietary_data->next;
                }
                else if (strcmp (temp_string, "  3") == 0)
                {
                        /* Now follows a string containing additional
                         * proprietary data. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", body->additional_proprietary_data->line);
                        body->additional_proprietary_data->order = i;
                        i++;
                        dxf_proprietary_data_init ((DxfProprietaryData *) body->additional_proprietary_data->next);
                        body->additional_proprietary_data = (DxfProprietaryData *) body->additional_proprietary_data->next;
                }
                if (strcmp (temp_string, "5") == 0)
                {
                        /* Now follows a string containing a sequential
                         * id number. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%x\n", &body->id_code);
                }
                else if (strcmp (temp_string, "6") == 0)
                {
                        /* Now follows a string containing a linetype
                         * name. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", body->linetype);
                }
                else if (strcmp (temp_string, "8") == 0)
                {
                        /* Now follows a string containing a layer name. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", body->layer);
                }
                else if ((fp->acad_version_number <= AutoCAD_11)
                  && DXF_FLATLAND
                  && (strcmp (temp_string, "38") == 0))
                {
                        /* Now follows a string containing the
                         * elevation. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &body->elevation);
                }
                else if (strcmp (temp_string, "39") == 0)
                {
                        /* Now follows a string containing the
                         * thickness. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%lf\n", &body->thickness);
                }
                else if (strcmp (temp_string, "62") == 0)
                {
                        /* Now follows a string containing the
                         * color value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &body->color);
                }
                else if (strcmp (temp_string, "67") == 0)
                {
                        /* Now follows a string containing the
                         * paperspace value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &body->paperspace);
                }
                else if ((fp->acad_version_number >= AutoCAD_13)
                        && (strcmp (temp_string, "70") == 0))
                {
                        /* Now follows a string containing the modeler
                         * format version number. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%d\n", &body->modeler_format_version_number);
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
                                fprintf (stderr, "Warning in dxf_body_read () found a bad subclass marker in: %s in line: %d.\n",
                                        fp->filename, fp->line_number);
                        }
                }
                else if (strcmp (temp_string, "330") == 0)
                {
                        /* Now follows a string containing Soft-pointer
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", body->dictionary_owner_soft);
                }
                else if (strcmp (temp_string, "360") == 0)
                {
                        /* Now follows a string containing Hard owner
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", body->dictionary_owner_hard);
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
        if (strcmp (body->linetype, "") == 0)
        {
                body->linetype = strdup (DXF_DEFAULT_LINETYPE);
        }
        if (strcmp (body->layer, "") == 0)
        {
                body->layer = strdup (DXF_DEFAULT_LAYER);
        }
        if (body->modeler_format_version_number == 0)
        {
                fprintf (stderr,
                  (_("Warning: in %s () illegal modeler format version number found while reading from: %s in line: %d.\n")),
                  __FUNCTION__, fp->filename, fp->line_number);
                fprintf (stderr,
                  (_("\tmodeler format version number is reset to 1.\n")));
                body->modeler_format_version_number = 1;
        }
        /* Clean up. */
        free (temp_string);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Write DXF output to a file for a DXF \c BODY entity.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred while reading from the input file.
 */
int
dxf_body_write
(
        DxfFile *fp,
                /*!< DXF file pointer to an output file (or device). */
        DxfBody *body
                /*!< DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *dxf_entity_name = strdup ("BODY");
        DxfProprietaryData *iter = NULL;
        DxfProprietaryData *additional_iter = NULL;
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
        if (body == NULL)
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
                  __FUNCTION__, dxf_entity_name, dxf_body_get_id_code (body));
        }
        if (strcmp (dxf_body_get_linetype (body), "") == 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () empty linetype string for the %s entity with id-code: %x\n")),
                  __FUNCTION__, dxf_entity_name, dxf_body_get_id_code (body));
                fprintf (stderr,
                  (_("\t%s entity is reset to default linetype")),
                  dxf_entity_name);
                dxf_body_set_linetype (body, strdup (DXF_DEFAULT_LAYER));
        }
        if (strcmp (dxf_body_get_layer (body), "") == 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () empty layer string for the %s entity with id-code: %x\n")),
                  __FUNCTION__, dxf_entity_name, dxf_body_get_id_code (body));
                fprintf (stderr,
                  (_("\t%s entity is relocated to layer 0")),
                  dxf_entity_name);
                dxf_body_set_layer (body, strdup (DXF_DEFAULT_LAYER));
        }
        /* Start writing output. */
        i = 1;
        fprintf (fp->fp, "  0\n%s\n", dxf_entity_name);
        if (dxf_body_get_id_code (body) != -1)
        {
                fprintf (fp->fp, "  5\n%x\n", dxf_body_get_id_code (body));
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
        if ((strcmp (dxf_body_get_dictionary_owner_soft (body), "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_REACTORS\n");
                fprintf (fp->fp, "330\n%s\n", dxf_body_get_dictionary_owner_soft (body));
                fprintf (fp->fp, "102\n}\n");
        }
        if ((strcmp (dxf_body_get_dictionary_owner_hard (body), "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_XDICTIONARY\n");
                fprintf (fp->fp, "360\n%s\n", dxf_body_get_dictionary_owner_hard (body));
                fprintf (fp->fp, "102\n}\n");
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, "100\nAcDbEntity\n");
        }
        if (dxf_body_get_paperspace (body) == DXF_PAPERSPACE)
        {
                fprintf (fp->fp, " 67\n%d\n", DXF_PAPERSPACE);
        }
        fprintf (fp->fp, "  8\n%s\n", dxf_body_get_layer (body));
        if (strcmp (dxf_body_get_linetype (body), DXF_DEFAULT_LINETYPE) != 0)
        {
                fprintf (fp->fp, "  6\n%s\n", dxf_body_get_linetype (body));
        }
        if ((fp->acad_version_number <= AutoCAD_11)
          && DXF_FLATLAND
          && (dxf_body_get_elevation (body) != 0.0))
        {
                fprintf (fp->fp, " 38\n%f\n", dxf_body_get_elevation (body));
        }
        if (dxf_body_get_thickness (body) != 0.0)
        {
                fprintf (fp->fp, " 39\n%f\n", dxf_body_get_thickness (body));
        }
        if (dxf_body_get_linetype_scale (body) != 1.0)
        {
                fprintf (fp->fp, " 48\n%f\n", dxf_body_get_linetype_scale (body));
        }
        if (dxf_body_get_visibility (body) != 0)
        {
                fprintf (fp->fp, " 60\n%d\n", dxf_body_get_visibility (body));
        }
        if (dxf_body_get_color (body) != DXF_COLOR_BYLAYER)
        {
                fprintf (fp->fp, " 62\n%d\n", dxf_body_get_color (body));
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, "100\nAcDbModelerGeometry\n");
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, " 70\n%d\n", dxf_body_get_modeler_format_version_number (body));
        }
        iter = (DxfProprietaryData *) dxf_body_get_proprietary_data (body);
        additional_iter = (DxfProprietaryData *) dxf_body_get_additional_proprietary_data (body);
        while ((iter != NULL) || (additional_iter != NULL))
        {
                if (iter->order == i)
                {
                        fprintf (fp->fp, "  1\n%s\n", dxf_proprietary_data_get_line (iter));
                        iter = (DxfProprietaryData *) dxf_proprietary_data_get_next (iter);
                        i++;
                }
                if (additional_iter->order == i)
                {
                        fprintf (fp->fp, "  3\n%s\n", dxf_proprietary_data_get_line (additional_iter));
                        additional_iter = (DxfProprietaryData *) dxf_proprietary_data_get_next (additional_iter);
                        i++;
                }
        }
        /* Clean up. */
        free (dxf_entity_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a DXF \c BODY and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_body_free
(
        DxfBody *body
                /*!< Pointer to the memory occupied by the DXF
                 * \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->next != NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () pointer to next was not NULL.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        free (body->linetype);
        free (body->layer);
        dxf_proprietary_data_free_chain (body->proprietary_data);
        dxf_proprietary_data_free_chain (body->additional_proprietary_data);
        free (body->dictionary_owner_soft);
        free (body->dictionary_owner_hard);
        free (body);
        body = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a chain of DXF \c BODY
 * entities and all their data fields.
 */
void
dxf_body_free_chain
(
        DxfBody *bodies
                /*!< pointer to the chain of DXF \c BODY entities. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (bodies == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
        }
        while (bodies != NULL)
        {
                struct DxfBody *iter = bodies->next;
                dxf_body_free (bodies);
                bodies = (DxfBody *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
}


/*!
 * \brief Get the ID code from a DXF \c BODY entity.
 *
 * \return ID code.
 */
int
dxf_body_get_id_code
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->id_code < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found in the id-code member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->id_code);
}


/*!
 * \brief Set the ID code for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_id_code
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        int id_code
                /*!< Identification number for the entity.\n
                 * This is to be an unique (sequential) number in the DXF
                 * file. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (id_code < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative id-code value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->id_code = id_code;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the linetype from a DXF \c BODY entity.
 *
 * \return linetype when sucessful, \c NULL when an error occurred.
 */
char *
dxf_body_get_linetype
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->linetype ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the linetype member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (body->linetype));
}


/*!
 * \brief Set the linetype for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_linetype
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        char *linetype
                /*!< a string containing the linetype for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
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
        body->linetype = strdup (linetype);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the layer from a DXF \c BODY entity.
 *
 * \return layer when sucessful, \c NULL when an error occurred.
 */
char *
dxf_body_get_layer
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->layer ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the layer member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (body->layer));
}


/*!
 * \brief Set the layer for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_layer
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        char *layer
                /*!< a string containing the layer for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
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
        body->layer = strdup (layer);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the elevation from a DXF \c BODY entity.
 *
 * \return elevation.
 */
double
dxf_body_get_elevation
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->elevation);
}


/*!
 * \brief Set the elevation for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_elevation
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        double elevation
                /*!< the elevation to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->elevation = elevation;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the thickness from a DXF \c BODY entity.
 *
 * \return thickness.
 */
double
dxf_body_get_thickness
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->thickness < 0.0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found in the thickness member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->thickness);
}


/*!
 * \brief Set the thickness for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_thickness
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        double thickness
                /*!< the thickness to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (thickness < 0.0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative thickness value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->thickness = thickness;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the linetype scale from a DXF \c BODY entity.
 *
 * \return linetype scale.
 */
double
dxf_body_get_linetype_scale
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->linetype_scale < 0.0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found in the linetype scale member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->linetype_scale);
}


/*!
 * \brief Set the linetype scale for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_linetype_scale
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        double linetype_scale
                /*!< the linetype scale to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (linetype_scale < 0.0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative linetype scale value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->linetype_scale = linetype_scale;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the visibility from a DXF \c BODY entity.
 *
 * \return visibility.
 */
int16_t
dxf_body_get_visibility
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->visibility < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found in the visibility member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->visibility > 1)
        {
                fprintf (stderr,
                  (_("Error in %s () an out of range value was found in the visibility member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->visibility);
}


/*!
 * \brief Set the visibility for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_visibility
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        int16_t visibility
                /*!< the visibility to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (visibility < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative visibility value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (visibility > 1)
        {
                fprintf (stderr,
                  (_("Error in %s () an out of range visibility value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->visibility = visibility;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the color from a DXF \c BODY entity.
 *
 * \return color.
 */
int
dxf_body_get_color
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->color < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was found in the color member.\n")),
                  __FUNCTION__);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->color);
}


/*!
 * \brief Set the color for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_color
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        int color
                /*!< the color to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (color < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative color value was passed.\n")),
                  __FUNCTION__);
                fprintf (stderr,
                  (_("\teffectively turning this entity it's visibility off.\n")));
        }
        body->color = color;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the paperspace flag value from a DXF \c BODY entity.
 *
 * \return paperspace flag value.
 */
int
dxf_body_get_paperspace
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->paperspace < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was found in the paperspace member.\n")),
                  __FUNCTION__);
        }
        if (body->paperspace > 1)
        {
                fprintf (stderr,
                  (_("Warning in %s () an out of range value was found in the paperspace member.\n")),
                  __FUNCTION__);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->paperspace);
}


/*!
 * \brief Set the paperspace flag for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_paperspace
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        int paperspace
                /*!< the paperspace flag value to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (paperspace < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative paperspace value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (paperspace > 1)
        {
                fprintf (stderr,
                  (_("Error in %s () an out of range paperspace value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->paperspace = paperspace;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the \c graphics_data_size value from a DXF \c BODY entity.
 *
 * \return \c graphics_data_size value when successful, or
 * \c EXIT_FAILURE when an error occurred.
 */
int
dxf_body_get_graphics_data_size
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->graphics_data_size < 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a negative value was found in the graphics_data_size member.\n")),
                  __FUNCTION__);
        }
        if (body->graphics_data_size == 0)
        {
                fprintf (stderr,
                  (_("Warning in %s () a zero value was found in the graphics_data_size member.\n")),
                  __FUNCTION__);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->graphics_data_size);
}


/*!
 * \brief Set the \c graphics_data_size value for a DXF \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_graphics_data_size
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        int graphics_data_size
                /*!< the \c graphics_data_size value to be set for the
                 * entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (graphics_data_size < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative graphics_data_size value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (graphics_data_size == 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a zero graphics_data_size value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->graphics_data_size = graphics_data_size;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the \c shadow_mode from a DXF \c BODY entity.
 *
 * \return \c shadow_mode when successful, or \c EXIT_FAILURE when an
 * error occurred.
 */
int16_t
dxf_body_get_shadow_mode
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->shadow_mode < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found in the shadow_mode member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->shadow_mode > 3)
        {
                fprintf (stderr,
                  (_("Error in %s () an out of range value was found in the shadow_mode member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->shadow_mode);
}


/*!
 * \brief Set the \c shadow_mode for a DXF \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_shadow_mode
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        int16_t shadow_mode
                /*!< the shadow mode to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (shadow_mode < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative shadow_mode value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (shadow_mode > 3)
        {
                fprintf (stderr,
                  (_("Error in %s () an out of range shadow_mode value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->shadow_mode = shadow_mode;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the pointer to the binary graphics data from a DXF 
 * \c BODY entity.
 *
 * \return pointer to the binary graphics data.
 *
 * \warning No checks are performed on the returned pointer.
 */
DxfBinaryGraphicsData *
dxf_body_get_binary_graphics_data
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->binary_graphics_data ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the binary_graphics_data member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfBinaryGraphicsData *) body->binary_graphics_data);
}


/*!
 * \brief Set the pointer to the binary_graphics_data for a DXF
 * \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_binary_graphics_data
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        DxfBinaryGraphicsData *data
                /*!< a string containing the pointer to the
                 * binary_graphics_data for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->binary_graphics_data = (DxfBinaryGraphicsData *) data;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the soft pointer to the dictionary owner from a DXF 
 * \c BODY entity.
 *
 * \return soft pointer to the dictionary owner.
 *
 * \warning No checks are performed on the returned pointer (string).
 */
char *
dxf_body_get_dictionary_owner_soft
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->dictionary_owner_soft ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the dictionary_owner_soft member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (body->dictionary_owner_soft));
}


/*!
 * \brief Set the pointer to the dictionary_owner_soft for a DXF
 * \c BODY entity.
 */
DxfBody *
dxf_body_set_dictionary_owner_soft
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        char *dictionary_owner_soft
                /*!< a string containing the pointer to the
                 * dictionary_owner_soft for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary_owner_soft == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->dictionary_owner_soft = strdup (dictionary_owner_soft);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the pointer to the \c material from a DXF \c BODY
 * entity.
 *
 * \return a pointer to \c material when successful, or \c NULL when an
 * error occurred.
 *
 * \warning No checks are performed on the returned pointer.
 */
char *
dxf_body_get_material
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->material ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the material member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (body->material));
}


/*!
 * \brief Set the pointer to the \c material for a DXF \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_material
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        char *material
                /*!< a string containing the pointer to the \c
                 * material for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (material == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->material = strdup (material);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the hard pointer to the dictionary owner from a DXF 
 * \c BODY entity.
 *
 * \return hard pointer to the dictionary owner.
 *
 * \warning No checks are performed on the returned pointer (string).
 */
char *
dxf_body_get_dictionary_owner_hard
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->dictionary_owner_hard ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the dictionary_owner_hard member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (body->dictionary_owner_hard));
}


/*!
 * \brief Set the pointer to the dictionary_owner_hard for a DXF
 * \c BODY entity.
 */
DxfBody *
dxf_body_set_dictionary_owner_hard
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        char *dictionary_owner_hard
                /*!< a string containing the pointer to the
                 * dictionary_owner_hard for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary_owner_hard == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->dictionary_owner_hard = strdup (dictionary_owner_hard);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the \c lineweight from a DXF \c BODY entity.
 *
 * \return \c lineweight when successful, or \c EXIT_FAILURE when an
 * error occurred.
 */
int16_t
dxf_body_get_lineweight
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->lineweight);
}


/*!
 * \brief Set the \c lineweight for a DXF \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_lineweight
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        int16_t lineweight
                /*!< the \c lineweight to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->lineweight = lineweight;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the \c plot_style_name from a DXF \c BODY entity.
 *
 * \return \c plot_style_name when sucessful, or \c NULL when an error
 * occurred.
 */
char *
dxf_body_get_plot_style_name
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->plot_style_name ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the plot_style_name member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (body->plot_style_name));
}


/*!
 * \brief Set the \c plot_style_name for a DXF \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_plot_style_name
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        char *plot_style_name
                /*!< a string containing the \c plot_style_name for the
                 * entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (plot_style_name == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->plot_style_name = strdup (plot_style_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the \c color_value from a DXF \c BODY entity.
 *
 * \return \c color_value when successful, or \c EXIT_FAILURE when an
 * error occurred.
 */
long
dxf_body_get_color_value
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->color_value);
}


/*!
 * \brief Set the \c color_value for a DXF \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_color_value
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        long color_value
                /*!< the \c color_value to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->color_value = color_value;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the \c color_name from a DXF \c BODY entity.
 *
 * \return \c color_name when sucessful, or \c NULL when an error
 * occurred.
 */
char *
dxf_body_get_color_name
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->color_name ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the color_name member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (body->color_name));
}


/*!
 * \brief Set the \c color_name for a DXF \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_color_name
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        char *color_name
                /*!< a string containing the \c color_name for the
                 * entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (color_name == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->color_name = strdup (color_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the \c transparency from a DXF \c BODY entity.
 *
 * \return \c transparency when successful, or \c EXIT_FAILURE when an
 * error occurred.
 */
long
dxf_body_get_transparency
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->transparency);
}


/*!
 * \brief Set the \c transparency for a DXF \c BODY entity.
 *
 * \return a pointer to \c body when successful, or \c NULL when an
 * error occurred.
 */
DxfBody *
dxf_body_set_transparency
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        long transparency
                /*!< the \c transparency to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->transparency = transparency;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the pointer to the \c proprietary_data from a DXF \c BODY
 * entity.
 *
 * \return pointer to the \c proprietary_data.
 *
 * \warning No checks are performed on the returned pointer.
 */
DxfProprietaryData *
dxf_body_get_proprietary_data
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->proprietary_data ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the proprietary_data member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfProprietaryData *) body->proprietary_data);
}


/*!
 * \brief Set the proprietary data for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_proprietary_data
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        DxfProprietaryData *proprietary_data
                /*!< a pointer to a linked list containing the
                 * \c proprietary_data for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (proprietary_data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->proprietary_data = proprietary_data;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the \c additional_proprietary_data from a DXF \c BODY entity.
 *
 * \return pointer to the \c additional_proprietary_data.
 *
 * \warning No checks are performed on the returned pointer (string).
 */
DxfProprietaryData *
dxf_body_get_additional_proprietary_data
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->additional_proprietary_data ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfProprietaryData *) body->additional_proprietary_data);
}


/*!
 * \brief Set the pointer to the additional proprietary data for a DXF
 * \c BODY entity.
 */
DxfBody *
dxf_body_set_additional_proprietary_data
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        DxfProprietaryData *additional_proprietary_data
                /*!< an array containing the additional proprietary data
                 * for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (additional_proprietary_data ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->additional_proprietary_data = additional_proprietary_data;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the modeler format version number from a DXF \c BODY entity.
 *
 * \return ID code.
 */
int
dxf_body_get_modeler_format_version_number
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (body->modeler_format_version_number != 1)
        {
                fprintf (stderr,
                  (_("Error in %s () an invalid value was found in the modeler_format_version_number member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body->modeler_format_version_number);
}


/*!
 * \brief Set the modeler format version number for a DXF \c BODY entity.
 */
DxfBody *
dxf_body_set_modeler_format_version_number
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        int modeler_format_version_number
                /*!< Modeler format version number (currently = 1). */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (modeler_format_version_number != 1)
        {
                fprintf (stderr,
                  (_("Error in %s () an invalid modeler format version number value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->modeler_format_version_number = modeler_format_version_number;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the pointer to the next \c BODY entity from a DXF 
 * \c BODY entity.
 *
 * \return pointer to the next \c BODY entity.
 *
 * \warning No checks are performed on the returned pointer.
 */
DxfBody *
dxf_body_get_next
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->next == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the next member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfBody *) body->next);
}


/*!
 * \brief Set the pointer to the next \c BODY for a DXF \c BODY
 * entity.
 */
DxfBody *
dxf_body_set_next
(
        DxfBody *body,
                /*!< a pointer to a DXF \c BODY entity. */
        DxfBody *next
                /*!< a pointer to the next \c BODY for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (next == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        body->next = (struct DxfBody *) next;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (body);
}


/*!
 * \brief Get the pointer to the last \c BODY entity from a linked list
 * of DXF \c BODY entities.
 *
 * \return pointer to the last \c BODY entity.
 *
 * \warning No checks are performed on the returned pointer.
 */
DxfBody *
dxf_body_get_last
(
        DxfBody *body
                /*!< a pointer to a DXF \c BODY entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (body == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (body->next == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was found in the next member.\n")),
                  __FUNCTION__);
                return ((DxfBody *) body);
        }
        DxfBody *iter = (DxfBody *) body->next;
        while (iter->next != NULL)
        {
                iter = (DxfBody *) iter->next;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfBody *) iter);
}


/* EOF */
