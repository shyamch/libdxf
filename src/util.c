/*!
 * \file util.c
 *
 * \author Copyright (C) 2010 by Luis Matos <gass@otiliamatos.ath.cx>.
 *
 * \author Copyright (C) 2010, 2012, 2015, 2017
 * by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Functions for libDXF coding utilities.
 *
 * <hr>
 * <h1><b>Copyright Notices.</b></h1>\n
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.\n\n
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


#include <stdarg.h>
#include "util.h"


/*!
 * \brief Allocate memory for a \c DxfChar.
 *
 * Fill the memory contents with zeros.
 */
DxfChar *
dxf_char_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfChar *c = NULL;
        size_t size;

        size = sizeof (DxfChar);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((c = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfChar struct.\n")),
                  __FUNCTION__);
                c = NULL;
        }
        else
        {
                memset (c, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (c);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c DxfChar
 * object.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfChar *
dxf_char_init
(
        DxfChar *c
                /*!< a pointer to the DxfChar object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (c == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                c = dxf_char_new ();
        }
        if (c == NULL)
        {
              fprintf (stderr,
                (_("Error in %s () could not allocate memory for a DxfChar struct.\n")),
                __FUNCTION__);
              return (NULL);
        }
        c->value = strdup ("");
        c->length = 0;
        c->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (c);
}


/*!
 * \brief Free the allocated memory for a DxfChar object and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_char_free
(
        DxfChar *c
                /*!< a pointer to the memory occupied by the DxfChar
                 * object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (c == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (c->next != NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () pointer to next was not NULL.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        free (c->value);
        free (c);
        c = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a single linked list of DxfChar
 * objects and all their data fields.
 */
int
dxf_char_free_chain
(
        DxfChar *chars
                /*!< a pointer to the single linked list of DxfChar
                 * objects. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (chars == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        while (chars != NULL)
        {
                struct DxfChar *iter = chars->next;
                dxf_char_free (chars);
                chars = (DxfChar *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Allocate memory for a \c DxfDouble.
 *
 * Fill the memory contents with zeros.
 */
DxfDouble *
dxf_double_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfDouble *d = NULL;
        size_t size;

        size = sizeof (DxfDouble);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((d = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfDouble struct.\n")),
                  __FUNCTION__);
                d = NULL;
        }
        else
        {
                memset (d, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (d);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c DxfDouble
 * object.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfDouble *
dxf_double_init
(
        DxfDouble *d
                /*!< a pointer to the DxfDouble object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (d == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                d = dxf_double_new ();
        }
        if (d == NULL)
        {
              fprintf (stderr,
                (_("Error in %s () could not allocate memory for a DxfDouble struct.\n")),
                __FUNCTION__);
              return (NULL);
        }
        d->value = 0.0;
        d->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (d);
}


/*!
 * \brief Allocate memory for a \c DxfInt.
 *
 * Fill the memory contents with zeros.
 */
DxfInt *
dxf_int_new ()
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfInt *i = NULL;
        size_t size;

        size = sizeof (DxfInt);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((i = malloc (size)) == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () could not allocate memory for a DxfInt struct.\n")),
                  __FUNCTION__);
                i = NULL;
        }
        else
        {
                memset (i, 0, size);
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (i);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c DxfInt
 * object.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfInt *
dxf_int_init
(
        DxfInt *i
                /*!< a pointer to the DxfInt object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (i == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                i = dxf_int_new ();
        }
        if (i == NULL)
        {
              fprintf (stderr,
                (_("Error in %s () could not allocate memory for a DxfInt struct.\n")),
                __FUNCTION__);
              return (NULL);
        }
        i->value = 0;
        i->next = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (i);
}


/*!
 * \brief Free the allocated memory for a DxfInt object and all it's
 * data fields.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_int_free
(
        DxfInt *i
                /*!< a pointer to the memory occupied by the DxfInt
                 * object. */
)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /* Do some basic checks. */
        if (i == NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        if (i->next != NULL)
        {
                fprintf (stderr,
                  (_("Error in %s () pointer to next was not NULL.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        free (i);
        i = NULL;
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Free the allocated memory for a single linked list of DxfInt
 * objects and all their data fields.
 */
int
dxf_int_free_chain
(
        DxfInt *ints
                /*!< a pointer to the single linked list of DxfInt
                 * objects. */
)
{
#ifdef DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (ints == NULL)
        {
                fprintf (stderr,
                  (_("Warning in %s () a NULL pointer was passed.\n")),
                  __FUNCTION__);
                return (EXIT_FAILURE);
        }
        while (ints != NULL)
        {
                struct DxfInt *iter = ints->next;
                dxf_int_free (ints);
                ints = (DxfInt *) iter;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return (EXIT_SUCCESS);
}


int
dxf_read_is_double (int type)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (type >=10 && type < 60)
        {
#if DEBUG
        DXF_DEBUG_END
#endif
                return TRUE;
        }
        else
        {
#if DEBUG
        DXF_DEBUG_END
#endif
                return FALSE;
        }
}


int
dxf_read_is_int (int type)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (type >= 60 && type < 80)
        {
#if DEBUG
        DXF_DEBUG_END
#endif
                return TRUE;
        }
        else
        {
#if DEBUG
        DXF_DEBUG_END
#endif
                return FALSE;
        }
}


int
dxf_read_is_string (int type)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        if (type >= 0 && type <= 9)
        {
#if DEBUG
        DXF_DEBUG_END
#endif
                return TRUE;
        }
        else
        {
#if DEBUG
        DXF_DEBUG_END
#endif
                return FALSE;
        }
}

/*!
 * \brief Opens a DxfFile, does error checking and resets the line number
 * counter.
 * 
 * Reset the line counting to 0.
 * 
 */
DxfFile *
dxf_read_init (const char *filename)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        DxfFile * file = NULL;
        FILE *fp;
        if (!filename)
        {
                fprintf (stderr,
                  (_("Error: filename is not initialised (NULL pointer).\n")));
                return (NULL);
        }
        if (strcmp (filename, "") == 0)
        {
                fprintf (stderr,
                  (_("Error: filename contains an empty string.\n")));
                return (NULL);
        }
        fp = fopen (filename, "r");
        if (!fp)
        {
                fprintf (stderr,
                  (_("Error: could not open file: %s for reading (NULL pointer).\n")),
                  filename);
                return (NULL);
        }
        file = malloc (sizeof(DxfFile));
        file->fp = fp;
        file->filename = strdup(filename);
        file->line_number = 0;
        /*! \todo FIXME: dxf header and blocks need initialized ?
        dxf_header_init (file->dxf_header);
        dxf_block_init (file->dxf_block);
        */
#if DEBUG
        DXF_DEBUG_END
#endif
        return file;
}


void
dxf_read_close (DxfFile *file)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        /*! \todo FIXME: how to free other sub structures */
        if (file == NULL)
        {
                fprintf (stderr,
                  (_("Error: file is not initialised (NULL pointer).\n")));
        }
        else
        {
                fclose (file->fp);
                free (file->filename);
                free (file);
                file = NULL;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
}


/*!
 * \brief Reads a line from a file.
 * 
 * Reads the next line from \c fp file and stores it into the temp_string.
 * 
 */
int
dxf_read_line (char * temp_string, DxfFile *fp)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        int ret;

        ret = fscanf (fp->fp, "%[^\n]\n", temp_string);
        if (ferror (fp->fp))
        {
                fprintf (stderr,
                  (_("Error: while reading from: %s in line: %d.\n")),
                  fp->filename, fp->line_number);
                return (EXIT_FAILURE);
        }
        if (ret)
        {
                fp->line_number++;
        }
#if DEBUG
        DXF_DEBUG_END
#endif
        return ret;
}

/*!
 * \brief Uses of fscanf with other features.
 * 
 * Uses fscanf for file parsing, but also tracks the lines it reads;
 * 
 */
int
dxf_read_scanf (DxfFile *fp, const char *template, ...)
{
#if DEBUG
        DXF_DEBUG_BEGIN
#endif
        int ret;
        char * search_result;
        va_list lst;
        va_start (lst, template);
        ret = vfscanf (fp->fp, template, lst);
        if (ferror (fp->fp))
        {
                fprintf (stderr,
                  (_("Error: while reading from: %s in line: %d.\n")),
                  fp->filename, fp->line_number);
                return (EXIT_FAILURE);
        }
        va_end (lst);
        if (ret)
        {
                /*
                 * we have to find each \n from the template to know how many lines will we read;
                 */
                search_result = (char *) template;
                while (TRUE)
                {
                        search_result = strstr (search_result, "\n");
                        if (search_result == NULL)
                                break;
                        fp->line_number++;
                        *++search_result;
                }
        }
        return ret;
#if DEBUG
        DXF_DEBUG_END
#endif
}


/* EOF */
