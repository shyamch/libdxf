/*!
 * \file binary_entity_data.h
 *
 * \author Copyright (C) 2016 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 *
 * \brief Header file for a DXF binary entity data object.
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


#ifndef LIBDXF_SRC_BINARY_ENTITY_DATA_H
#define LIBDXF_SRC_BINARY_ENTITY_DATA_H


#include "global.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
 * \brief DXF definition of an AutoCAD binary entity data object.
 */
typedef struct
dxf_binary_entity_data_struct
{
        char *data_line;
                /*!< Binary entity data.\n
                 * Multiple lines of 256 characters maximum per line
                 * (optional).\n
                 * Group code = 310. */
        int length;
                /*!< Length of the data_line member. */
        struct DxfBinaryEntityData *next;
                /*!< Pointer to the next DxfBinaryEntityData.\n
                 * \c NULL if the last DxfBinaryEntityData. */
} DxfBinaryEntityData;


DxfBinaryEntityData *
dxf_binary_entity_data_new ();
DxfBinaryEntityData *
dxf_binary_entity_data_init
(
        DxfBinaryEntityData *data
);
int
dxf_binary_entity_data_write
(
        DxfFile *fp,
        DxfBinaryEntityData *data
);
int
dxf_binary_entity_data_free
(
        DxfBinaryEntityData *data
);
void
dxf_binary_entity_data_free_chain
(
        DxfBinaryEntityData *data
);
char *
dxf_binary_entity_data_get_data_line
(
        DxfBinaryEntityData *data
);
DxfBinaryEntityData *
dxf_binary_entity_data_set_data_line
(
        DxfBinaryEntityData *data,
        char *data_line
);
int
dxf_binary_entity_data_get_length
(
        DxfBinaryEntityData *data
);
DxfBinaryEntityData *
dxf_binary_entity_data_set_length
(
        DxfBinaryEntityData *data,
        int length
);
DxfBinaryEntityData *
dxf_binary_entity_data_get_next
(
        DxfBinaryEntityData *data
);
DxfBinaryEntityData *
dxf_binary_entity_data_set_next
(
        DxfBinaryEntityData *data,
        DxfBinaryEntityData *next
);
DxfBinaryEntityData *
dxf_binary_entity_data_get_last
(
        DxfBinaryEntityData *data
);


#ifdef __cplusplus
}
#endif


#endif /* LIBDXF_SRC_BINARY_ENTITY_DATA_H */


/* EOF */
