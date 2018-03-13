/*!
 * \file dictionary.c
 *
 * \author Copyright (C) 2015, 2016, 2017 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for a DXF dictionary object (\c DICTIONARY).
 *
 * \version The \c DICTIONARY object was introduced in DXF R13.
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


#include "dictionary.h"


/*!
 * \brief Allocate memory for a DXF \c DICTIONARY object.
 *
 * Fill the memory contents with zeros.
 */
DxfDictionary *
dxf_dictionary_new ()
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfDictionary *dictionary = NULL;
        size_t size;

        size = sizeof (DxfDictionary);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((dictionary = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfDictionary struct.\n")),
                  __FUNCTION__);
                dictionary = NULL;
        }
        else
        {
                memset (dictionary, 0, size);
        }
#ifdef DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c DICTIONARY
 * object.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfDictionary *
dxf_dictionary_init
(
        DxfDictionary *dictionary
                /*!< a pointer to the DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                dictionary = dxf_dictionary_new ();
        }
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfDictionary struct.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        dxf_dictionary_set_id_code (dictionary, 0);
        dxf_dictionary_set_dictionary_owner_soft (dictionary,  strdup (""));
        dxf_dictionary_set_dictionary_owner_hard (dictionary,  strdup (""));
        dxf_dictionary_set_entry_name (dictionary,  strdup (""));
        dxf_dictionary_set_entry_object_handle (dictionary,  strdup (""));
        dxf_dictionary_set_next (dictionary,  NULL);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary);
}


/*!
 * \brief Read data from a DXF file into a DXF \c DICTIONARY object.
 *
 * The last line read from file contained the string "DICTIONARY". \n
 * Now follows some data for the \c DICTIONARY object, to be terminated
 * with a "  0" string announcing the following object. \n
 * While parsing the DXF file store data in \c dictionary. \n
 *
 * \return \c a pointer to \c dictionary.
 */
DxfDictionary *
dxf_dictionary_read
(
        DxfFile *fp,
                /*!< DXF file pointer to an input file (or device). */
        DxfDictionary *dictionary
                /*!< a pointer to the DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *temp_string = NULL;

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
        if (fp->acad_version_number < AutoCAD_13)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this entity.\n")),
                  __FUNCTION__);
        }
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                dictionary = dxf_dictionary_new ();
                dictionary = dxf_dictionary_init (dictionary);
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
                        /* Clean up. */
                        free (temp_string);
                        fclose (fp->fp);
                        return (NULL);
                }
                else if (strcmp (temp_string, "3") == 0)
                {
                        /* Now follows a string containing additional
                         * proprietary data. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", dictionary->entry_name);
                }
                if (strcmp (temp_string, "5") == 0)
                {
                        /* Now follows a string containing a sequential
                         * id number. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%x\n", &dictionary->id_code);
                }
                else if ((fp->acad_version_number >= AutoCAD_13)
                        && (strcmp (temp_string, "100") == 0))
                {
                        /* Now follows a string containing the
                         * subclass marker value. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", temp_string);
                        if (strcmp (temp_string, "AcDbDictionary") != 0)
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
                        fscanf (fp->fp, "%s\n", dictionary->dictionary_owner_soft);
                }
                else if (strcmp (temp_string, "350") == 0)
                {
                        /* Now follows a string containing a handle to ae
                         * entry object. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", dictionary->entry_object_handle);
                }
                else if (strcmp (temp_string, "360") == 0)
                {
                        /* Now follows a string containing Hard owner
                         * ID/handle to owner dictionary. */
                        (fp->line_number)++;
                        fscanf (fp->fp, "%s\n", dictionary->dictionary_owner_hard);
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
        return (dictionary);
}


/*!
 * \brief Write DXF output to a file for a DXF \c DICTIONARY object.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_dictionary_write
(
        DxfFile *fp,
                /*!< DXF file pointer to an output file (or device). */
        DxfDictionary *dictionary
                /*!< a pointer to the DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        char *dxf_entity_name = strdup ("DICTIONARY");

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
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (strcmp (dxf_dictionary_get_entry_name (dictionary), "") == 0)
        {
                fprintf (stderr,
                  (_("Error in %s () empty entry name string for the %s entity with id-code: %x\n")),
                  __FUNCTION__, dxf_entity_name, dxf_dictionary_get_id_code (dictionary));
                /* Clean up. */
                free (dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (fp->acad_version_number < AutoCAD_13)
        {
                fprintf (stderr,
                  (_("Warning in %s () illegal DXF version for this %s entity with id-code: %x.\n")),
                  __FUNCTION__, dxf_entity_name, dxf_dictionary_get_id_code (dictionary));
        }
        /* Start writing output. */
        fprintf (fp->fp, "  0\n%s\n", dxf_entity_name);
        if (dxf_dictionary_get_id_code (dictionary) != -1)
        {
                fprintf (fp->fp, "  5\n%x\n", dxf_dictionary_get_id_code (dictionary));
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
        if ((strcmp (dxf_dictionary_get_dictionary_owner_soft (dictionary), "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_REACTORS\n");
                fprintf (fp->fp, "330\n%s\n", dxf_dictionary_get_dictionary_owner_soft (dictionary));
                fprintf (fp->fp, "102\n}\n");
        }
        if ((strcmp (dictionary->dictionary_owner_hard, "") != 0)
          && (fp->acad_version_number >= AutoCAD_14))
        {
                fprintf (fp->fp, "102\n{ACAD_XDICTIONARY\n");
                fprintf (fp->fp, "360\n%s\n", dxf_dictionary_get_dictionary_owner_hard (dictionary));
                fprintf (fp->fp, "102\n}\n");
        }
        if (fp->acad_version_number >= AutoCAD_13)
        {
                fprintf (fp->fp, "100\nAcDbDictionary\n");
        }
        fprintf (fp->fp, "  3\n%s\n", dxf_dictionary_get_entry_name (dictionary));
        fprintf (fp->fp, "350\n%s\n", dxf_dictionary_get_entry_object_handle (dictionary));
        /* Clean up. */
        free (dxf_entity_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a DXF \c DICTIONARY and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_dictionary_free
(
        DxfDictionary *dictionary
                /*!< a pointer to the memory occupied by the DXF
                 * \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (dictionary->next != NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () pointer to next was not NULL.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        free (dxf_dictionary_get_dictionary_owner_soft (dictionary));
        free (dxf_dictionary_get_dictionary_owner_hard (dictionary));
        free (dxf_dictionary_get_entry_name (dictionary));
        free (dxf_dictionary_get_entry_object_handle (dictionary));
        free (dictionary);
        dictionary = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a chain of DXF \c DICTIONARY
 * objects and all their data fields.
 */
void
dxf_dictionary_free_chain
(
        DxfDictionary *dictionaries
                /*!< a pointer to the chain of DXF \c DICTIONARY objects. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (dictionaries == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
        }
        while (dictionaries != NULL)
        {
                struct DxfDictionary *iter = dictionaries->next;
                dxf_dictionary_free (dictionaries);
                dictionaries = (DxfDictionary *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
}


/*!
 * \brief Get the ID code from a DXF \c DICTIONARY object.
 *
 * \return ID code.
 */
int
dxf_dictionary_get_id_code
(
        DxfDictionary *dictionary
                /*!< a pointer to a DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (dictionary->id_code < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found in the id-code member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary->id_code);
}


/*!
 * \brief Set the ID code for a DXF \c DICTIONARY object.
 */
DxfDictionary *
dxf_dictionary_set_id_code
(
        DxfDictionary *dictionary,
                /*!< a pointer to a DXF \c DICTIONARY object. */
        int id_code
                /*!< Identification number for the object.\n
                 * This is to be an unique (sequential) number in the DXF
                 * file. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
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
        dictionary->id_code = id_code;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary);
}


/*!
 * \brief Get the soft pointer to the dictionary owner from a DXF 
 * \c DICTIONARY object.
 *
 * \return soft pointer to the dictionary owner.
 *
 * \warning No checks are performed on the returned pointer (string).
 */
char *
dxf_dictionary_get_dictionary_owner_soft
(
        DxfDictionary *dictionary
                /*!< a pointer to a DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary->dictionary_owner_soft ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the dictionary_owner_soft member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (dictionary->dictionary_owner_soft));
}


/*!
 * \brief Set the pointer to the dictionary_owner_soft for a DXF
 * \c DICTIONARY object.
 */
DxfDictionary *
dxf_dictionary_set_dictionary_owner_soft
(
        DxfDictionary *dictionary,
                /*!< a pointer to a DXF \c DICTIONARY object. */
        char *dictionary_owner_soft
                /*!< a string containing the pointer to the
                 * dictionary_owner_soft for the object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
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
        dictionary->dictionary_owner_soft = strdup (dictionary_owner_soft);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary);
}


/*!
 * \brief Get the hard pointer to the dictionary owner from a DXF 
 * \c DICTIONARY object.
 *
 * \return hard pointer to the dictionary owner.
 *
 * \warning No checks are performed on the returned pointer (string).
 */
char *
dxf_dictionary_get_dictionary_owner_hard
(
        DxfDictionary *dictionary
                /*!< a pointer to a DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary->dictionary_owner_hard ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the dictionary_owner_hard member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (dictionary->dictionary_owner_hard));
}


/*!
 * \brief Set the pointer to the dictionary_owner_hard for a DXF
 * \c DICTIONARY object.
 */
DxfDictionary *
dxf_dictionary_set_dictionary_owner_hard
(
        DxfDictionary *dictionary,
                /*!< a pointer to a DXF \c DICTIONARY object. */
        char *dictionary_owner_hard
                /*!< a string containing the pointer to the
                 * dictionary_owner_hard for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
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
        dictionary->dictionary_owner_hard = strdup (dictionary_owner_hard);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary);
}


/*!
 * \brief Get the entry name from a DXF \c DICTIONARY object.
 *
 * \return entry name when sucessful, \c NULL when an error occurred.
 */
char *
dxf_dictionary_get_entry_name
(
        DxfDictionary *dictionary
                /*!< a pointer to a DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary->entry_name ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the entry_name member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (dictionary->entry_name));
}


/*!
 * \brief Set the entry name for a DXF \c DICTIONARY object.
 */
DxfDictionary *
dxf_dictionary_set_entry_name
(
        DxfDictionary *dictionary,
                /*!< a pointer to a DXF \c DICTIONARY object. */
        char *entry_name
                /*!< a string containing the entry name for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (entry_name == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        dictionary->entry_name = strdup (entry_name);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary);
}


/*!
 * \brief Get the entry object handle from a DXF \c DICTIONARY object.
 *
 * \return entry object handle when sucessful, \c NULL when an error occurred.
 */
char *
dxf_dictionary_get_entry_object_handle
(
        DxfDictionary *dictionary
                /*!< a pointer to a DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary->entry_object_handle ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the entry_object_handle member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (dictionary->entry_object_handle));
}


/*!
 * \brief Set the entry object handle for a DXF \c DICTIONARY object.
 */
DxfDictionary *
dxf_dictionary_set_entry_object_handle
(
        DxfDictionary *dictionary,
                /*!< a pointer to a DXF \c DICTIONARY object. */
        char *entry_object_handle
                /*!< a string containing the entry object handle for the
                 * entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (entry_object_handle == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        dictionary->entry_object_handle = strdup (entry_object_handle);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary);
}


/*!
 * \brief Get the pointer to the next \c DICTIONARY object from a DXF 
 * \c DICTIONARY object.
 *
 * \return pointer to the next \c DICTIONARY object.
 *
 * \warning No checks are performed on the returned pointer.
 */
DxfDictionary *
dxf_dictionary_get_next
(
        DxfDictionary *dictionary
                /*!< a pointer to a DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary->next == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the next member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfDictionary *) dictionary->next);
}


/*!
 * \brief Set the pointer to the next \c DICTIONARY object for a DXF
 * \c DICTIONARY object.
 */
DxfDictionary *
dxf_dictionary_set_next
(
        DxfDictionary *dictionary,
                /*!< a pointer to a DXF \c DICTIONARY object. */
        DxfDictionary *next
                /*!< a pointer to the next \c DICTIONARY object for the
                 * object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
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
        dictionary->next = (struct DxfDictionary *) next;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (dictionary);
}


/*!
 * \brief Get the pointer to the last \c DICTIONARY object from a linked
 * list of DXF \c DICTIONARY objects.
 *
 * \return pointer to the last \c DICTIONARY object.
 *
 * \warning No checks are performed on the returned pointer.
 */
DxfDictionary *
dxf_dictionary_get_last
(
        DxfDictionary *dictionary
                /*!< a pointer to a DXF \c DICTIONARY object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (dictionary == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (dictionary->next == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was found in the next member.\n")),
                  __FUNCTION__);
                return ((DxfDictionary *) dictionary);
        }
        DxfDictionary *iter = (DxfDictionary *) dictionary->next;
        while (iter->next != NULL)
        {
                iter = (DxfDictionary *) iter->next;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfDictionary *) dictionary->next);
}


/* EOF*/
