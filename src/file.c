/*!
 * \file file.c
 * \author Copyright (C) 2008, 2009 by Bert Timmerman <bert.timmerman@xs4all.nl>.\n
 * \brief Generate dxf output for a complete DXF file.
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


#include "global.h"
#include "class.h"
#include "header.h"
#include "section.c"
#include "table.h"
#include "block.h"
#include "entity.h"


/*!
 * \brief Function generates dxf output to a file for a complete DXF file.
 */
int
dxf_write_file
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        DxfHeader dxf_header,
                /*!< a dxf header. */
        DxfClass dxf_classes_list,
                /*!< pointer to a list of CLASSES. */
        DxfTable dxf_tables_list,
                /*!< pointer to a list of TABLES. */
        int acad_version_number
                /*!< AutoCAD version number. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_write_file () function.\n", __FILE__, __LINE__);
#endif
        dxf_init_header (dxf_header, acad_version_number);
        dxf_write_header (dxf_header, acad_version_number);
        dxf_write_classes (dxf_classes_list, acad_version_number);
        dxf_write_tables (fp, dxf_tables_list, acad_version_number);
        dxf_write_blocks (dxf_blocks_list, acad_version_number);
        dxf_write_entities (dxf_entities_list, acad_version_number);
        dxf_write_objects (dxf_objects_list, acad_version_number);
        dxf_write_thumbnail (dxf_thumbnail, acad_version_number);
        dxf_write_eof();
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_write_file () function.\n", __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}


/* EOF */
