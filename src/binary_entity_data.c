/*!
 * \file binary_entity_data.c
 *
 * \author Copyright (C) 2016, 2017 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for a DXF binary entity data object.
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


#include "binary_entity_data.h"


/*!
 * \brief Allocate memory for a binary entity data object.
 *
 * Fill the memory contents with zeros.
 */
DxfBinaryEntityData *
dxf_binary_entity_data_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfBinaryEntityData *data = NULL;
        size_t size;

        size = sizeof (DxfBinaryEntityData);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((data = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfBinaryEntityData struct.\n")),
                  __FUNCTION__);
                data = NULL;
        }
        else
        {
                memset (data, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (data);
}


/*!
 * \brief Allocate memory and initialize data fields in a binary
 * entity data entity.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfBinaryEntityData *
dxf_binary_entity_data_init
(
        DxfBinaryEntityData *data
                /*!< a pointer to the binary entity data object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                data = dxf_binary_entity_data_new ();
        }
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfBinaryEntityData struct.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        dxf_binary_entity_data_set_data_line (data, strdup (""));
        dxf_binary_entity_data_set_next (data, NULL);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (data);
}


/*!
 * \brief Write DXF output to fp for a binary entity data object.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_binary_entity_data_write
(
        DxfFile *fp,
                /*!< file pointer to output file (or device). */
        DxfBinaryEntityData *data
                /*!< a pointer to the binary entity data object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (fp == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL file pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        /* Start writing output. */
        fprintf (fp->fp, "310\n%s\n", data->data_line);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a binary entity data object
 * and all it's data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_binary_entity_data_free
(
        DxfBinaryEntityData *data
                /*!< Pointer to the memory occupied by the binary
                 * entity data entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (data->next != NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () pointer to next was not NULL.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        free (data->data_line);
        free (data);
        data = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a chain of binary entity data
 * bobjects and all their data fields.
 */
void
dxf_binary_entity_data_free_chain
(
        DxfBinaryEntityData *data
                /*!< pointer to the chain of DXF \c HATCH binary
                 * entity data objects. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
        }
        while (data != NULL)
        {
                struct DxfBinaryEntityData *iter = data->next;
                dxf_binary_entity_data_free (data);
                data = (DxfBinaryEntityData *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
}


/*!
 * \brief Get the data_line from a binary entity data object.
 *
 * \return data_line when sucessful, \c NULL when an error occurred.
 */
char *
dxf_binary_entity_data_get_data_line
(
        DxfBinaryEntityData *data
                /*!< a pointer to a binary entity data object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (data->data_line ==  NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the data_line member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (strdup (data->data_line));
}


/*!
 * \brief Set the data_line for a binary entity data object.
 */
DxfBinaryEntityData *
dxf_binary_entity_data_set_data_line
(
        DxfBinaryEntityData *data,
                /*!< a pointer to a binary entity data object. */
        char *data_line
                /*!< a string containing the data_line for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (data_line == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        data->data_line = strdup (data_line);
#if DEBUG
        DXF_DEBUG_END
#endif
        return (data);
}


/*!
 * \brief Get the \c length from a binary entity data object.
 *
 * \return \c length.
 */
int
dxf_binary_entity_data_get_length
(
        DxfBinaryEntityData *data
                /*!< a pointer to a binary entity data object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (data->length < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative value was found in the length member.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (data->length);
}


/*!
 * \brief Set the \c length for a binary entity data object.
 *
 * \return a pointer to \c data when sucessful, \c NULL when an error
 * occurred.
 */
DxfBinaryEntityData *
dxf_binary_entity_data_set_length
(
        DxfBinaryEntityData *data,
                /*!< a pointer to a binary entity data object. */
        int length
                /*!< Length to be set for the entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (length < 0)
        {
                fprintf (stderr,
                  (_("Error in %s () a negative length value was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        data->length = length;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (data);
}


/*!
 * \brief Get the pointer to the next binary entity data object from a
 * binary entity data object.
 *
 * \return pointer to the next binary entity data object.
 *
 * \warning No checks are performed on the returned pointer.
 */
DxfBinaryEntityData *
dxf_binary_entity_data_get_next
(
        DxfBinaryEntityData *data
                /*!< a pointer to a binary entity data object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (data->next == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was found in the next member.\n")),
                  __FUNCTION__);
                return (NULL);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfBinaryEntityData *) data->next);
}


/*!
 * \brief Set the pointer to the next binary entity data for a binary
 * entity data entity.
 */
DxfBinaryEntityData *
dxf_binary_entity_data_set_next
(
        DxfBinaryEntityData *data,
                /*!< a pointer to a binary entity data object. */
        DxfBinaryEntityData *next
                /*!< a pointer to the next binary entity data for the
                 * entity. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
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
        data->next = (struct DxfBinaryEntityData *) next;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (data);
}


/*!
 * \brief Get the pointer to the last binary entity data object from a
 * linked list of binary entity data objects.
 *
 * \return pointer to the last binary entity data object.
 *
 * \warning No checks are performed on the returned pointer.
 */
DxfBinaryEntityData *
dxf_binary_entity_data_get_last
(
        DxfBinaryEntityData *data
                /*!< a pointer to a binary entity data object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (data == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (NULL);
        }
        if (data->next == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was found in the next member.\n")),
                  __FUNCTION__);
                return ((DxfBinaryEntityData *) data);
        }
        DxfBinaryEntityData *iter = (DxfBinaryEntityData *) data->next;
        while (iter->next != NULL)
        {
                iter = (DxfBinaryEntityData *) iter->next;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ((DxfBinaryEntityData *) data->next);
}


/* EOF */
