/******************************************************************************
 *   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
 *
 *   This file is part of Real VMX.
 *   Copyright (C) 2013 Surplus Users Ham Society
 *
 *   Real VMX is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Real VMX is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Real VMX.  If not, see <http://www.gnu.org/licenses/>.
 */

/* udgen.h - Universal graphics library generic driver */

#ifndef _udgen_h
#define _udgen_h

#ifndef _ASMLANGUAGE

#ifdef __cplusplus
extern "C" {
#endif

#include "ugl.h"
#include "driver/graphics/common/udcclr.h"

typedef struct ugl_gen_bitmap {
    UGL_BMAP_HEADER  header;
} UGL_GENERIC_BMAP;

typedef UGL_GENERIC_BMAP * UGL_BMAP_ID;

typedef struct ugl_gen_ddb {
    UGL_BMAP_HEADER  header;            /* Header */
    UGL_UINT16       colorDepth;        /* Bits-per-pixel */
    UGL_UINT16       stride;            /* Pixels per line */
    UGL_UINT32       transColorKey;     /* Transparent color key */
    void *           pData;             /* Image data */
} UGL_GEN_DDB;

typedef struct ugl_gen_mddb {
    UGL_BMAP_HEADER  header;            /* Header */
    UGL_UINT16       stride;            /* Pixels per line */
    void *           pData;             /* Image data */
} UGL_GEN_MDDB;

typedef struct ugl_gen_tddb {
    UGL_BMAP_HEADER  header;            /* Header */
    UGL_MDDB *       mask;              /* Bitmask */
    UGL_DDB *        ddb;               /* Bitmap */
} UGL_GEN_TDDB;

typedef struct ugl_generic_cddb {
    UGL_GEN_TDDB  tddb;                 /* Transparent cursor bitmap */
    UGL_POINT     hotSpot;              /* Coordinates for hot-spot */
} UGL_GEN_CDDB;

typedef struct ugl_gen_cursor_data {
    UGL_GC_ID      gc;                  /* Graphics context for cursor */
    UGL_SIZE       maxWidth;            /* Maximum width */
    UGL_SIZE       maxHeight;           /* Maxiumu height */
    UGL_POINT      position;            /* Coordinates for cursor position */
    UGL_BOOL       on;                  /* Cursor on-off */
    UGL_BOOL       hidden;              /* Cursor hidden-visible */
    UGL_GEN_CDDB * imageBitmap;         /* Cursor bitmap */
    UGL_DDB *      screenBitmap;        /* Screen bitmap for cursor */
    UGL_DDB *      scratchBitmap;       /* Scratch bitmao for cursor */
} UGL_GEN_CURSOR_DATA;

typedef struct ugl_generic_driver {
    UGL_UGI_DRIVER  ugi;                /* UGI driver (required) */
    void *          fbAddress;          /* Frame buffer address */
    UGL_PAGE *      pDrawPage;          /* Drawing page */
    UGL_PAGE *      pVisiblePage;       /* Visible page */
    UGL_GC_ID       gc;                 /* Graphics context */
    UGL_GEN_DDB *   scratchBitmap;      /* Scratch bitmap */
    UGL_ORD         transBitmapCount;   /* Scratch bitmap reference count */
    UGL_CLUT *      pClut;              /* Palette */
    UGL_BOOL        gpBusy;             /* Wait for graphics processor */
    void *          pCursorData;        /* Cursor data storage */

    /* Generic methods */
    UGL_STATUS      (*hLine) (
        struct ugl_generic_driver * pDrv,
        UGL_POS                     y,
        UGL_POS                     x1,
        UGL_POS                     x2,
        UGL_COLOR                   c
        );

    UGL_STATUS      (*vLine) (
        struct ugl_generic_driver * pDrv,
        UGL_POS                     x,
        UGL_POS                     y1,
        UGL_POS                     y2,
        UGL_COLOR                   c
        );

    UGL_STATUS      (*bresenhamLine) (
        struct ugl_generic_driver * pDrv,
        UGL_POINT *                 pStartPoint,
        UGL_SIZE                    numPoints,
        UGL_BOOL                    xMajor,
        UGL_ORD                     majorInc,
        UGL_ORD                     minorInc,
        UGL_ORD                     errorValue,
        UGL_ORD                     majorErrorInc,
        UGL_ORD                     minorErrorInc
        );

    UGL_STATUS      (*fill) (
        struct ugl_generic_driver * pDrv,
        UGL_POS                     y1,
        UGL_POS                     y2,
        UGL_POS **                  ppFillData
        );

    UGL_STATUS      (*gpWait) (
        struct ugl_generic_driver * pDrv
        );

} UGL_GENERIC_DRIVER;

/* Generic mode support functions */

/******************************************************************************
 *
 * uglGenericModeFind- Find graphics mode
 *
 * RETURNS: Graphics mode index or UGL_STATUS_ERROR
 */

UGL_INT32 uglGenericModeFind(
    UGL_MODE *  pList,
    UGL_MODE *  pReqMode,
    UGL_UINT32  numModes
    );

/* Generic graphics context support functions */

/******************************************************************************
 *
 * uglGenericGcCreate - Create graphics context
 *
 * RETURNS: Graphics context id or UGL_NULL
 */

UGL_GC_ID uglGenericGcCreate (
    UGL_DEVICE_ID  devId
    );

/******************************************************************************
 *
 * uglGenericGcCopy - Copy graphics context
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericGcCopy (
    UGL_DEVICE_ID  devId,
    UGL_GC_ID      srcGcId,
    UGL_GC_ID      destGcId
    );

/******************************************************************************
 *
 * uglGenericGcDestroy - Free graphics context
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericGcDestroy (
    UGL_DEVICE_ID  devId,
    UGL_GC_ID      gc
    );

/******************************************************************************
 *
 * uglGenericGcSet - Set current graphics context
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericGcSet (
    UGL_DEVICE_ID  devId,
    UGL_GC_ID      gc
    );

/* Generic palette support functions */

/******************************************************************************
 *
 * uglGenericClutCreate - Create palette
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericClutCreate (
    UGL_GENERIC_DRIVER * pDrv,
    UGL_SIZE             numColors
    );

/******************************************************************************
 *
 * uglGenericClutSet - Set generic indexed palette
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericClutSet (
    UGL_GENERIC_DRIVER * pDrv,
    UGL_ORD              offset,
    UGL_ARGB *           pColors,
    UGL_SIZE             numColors
    );

/******************************************************************************
 *
 * uglGenericClutGet - Get palette
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericClutGet (
    UGL_GENERIC_DRIVER * pDrv,
    UGL_ORD              offset,
    UGL_ARGB *           pColors,
    UGL_SIZE             numColors
    );

/******************************************************************************
 *
 * uglGenericClutMapNearest - Map to nearest match
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericClutMapNearest (
    UGL_GENERIC_DRIVER * pDrv,
    UGL_ARGB *           pMapColors,
    UGL_ARGB  *          pActualColors,
    UGL_COLOR *          pUglColors,
    UGL_SIZE             numColors
    );

/******************************************************************************
 *
 * uglGenericClutAllocIndexed - Allocate color
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericColorAllocIndexed (
    UGL_DEVICE_ID  devId,
    UGL_ARGB *     pReqColors,
    UGL_ORD *      pIndex,
    UGL_ARGB *     pActualColors,
    UGL_COLOR *    pUglColors,
    UGL_SIZE       numColors
    );

/******************************************************************************
 *
 * uglGenericClutFreeIndexed - Free color
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericColorFreeIndexed (
    UGL_DEVICE_ID  devId,
    UGL_COLOR *    pColors,
    UGL_SIZE       numColors
    );

/******************************************************************************
 *
 * uglGenericClutDestroy - Free palette
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericClutDestroy (
    UGL_GENERIC_DRIVER * pDrv
    );

/* Generic clipt support functions */

/******************************************************************************
 *
 * uglGenericClipDdbToDdb - Generic clip of device dependent bitmap
 *
 * RETURNS: UGL_TRUE or UGL_FALSE
 */

UGL_STATUS uglGenericClipDdbToDdb (
    UGL_DEVICE_ID  devId,
    UGL_RECT *     pClipRect,
    UGL_BMAP_ID *  pSrcBmpId,
    UGL_RECT *     pSrcRect,
    UGL_BMAP_ID *  pDestBmpId,
    UGL_POINT *    pDestPoint
    );

/******************************************************************************
 *
 * uglGenericClipDibToDdb - Generic clip of device independent to dependent
 *
 * RETURNS: UGL_TRUE or UGL_FALSE
 */

UGL_BOOL uglGenericClipDibToDdb (
    UGL_DEVICE_ID  devId,
    UGL_DIB *      pDib,
    UGL_RECT *     pSrcRect,
    UGL_BMAP_ID *  pBmpId,
    UGL_POINT *    pDestPoint
    );

/******************************************************************************
 *
 * uglGenericClipDdbToDib - Generic clip of device dependent to independent
 *
 * RETURNS: UGL_TRUE or UGL_FALSE
 */

UGL_BOOL uglGenericClipDdbToDib (
    UGL_DEVICE_ID  devId,
    UGL_BMAP_ID *  pBmpId,
    UGL_RECT *     pSrcRect,
    UGL_DIB *      pDib,
    UGL_POINT *    pDestPoint
    );

/* Generic line drawing support functions */

/******************************************************************************
 *
 * uglGenericLine - Draw a generic line
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericLine (
    UGL_DEVICE_ID  devId,
    UGL_POINT *    p1,
    UGL_POINT *    p2
    );

/* Generic fill support functions */

/******************************************************************************
 *
 * uglGenericFill - Generic fill area
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericFill (
    UGL_GENERIC_DRIVER * pDrv,
    UGL_POS              y1,
    UGL_POS              y2,
    UGL_POS **           ppFillData
    );

/* Generic rectangle drawing support functions */

/******************************************************************************
 *
 * uglGenericRectangle - Generic rectangle drawing
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericRectangle (
    UGL_DEVICE_ID  devId,
    UGL_RECT *     pRect
    );

/* Generic polygon drawing support functions */

/******************************************************************************
 *
 * uglGenericPolygon - Draw generic polygon
 *
 * RETURNS: UGL_TDDB_ID or UGL_NULL
 */

UGL_STATUS uglGenericPolygon (
    UGL_DEVICE_ID     devId,
    const UGL_POINT * pointArray,
    UGL_ORD           numPoints
    );

/* Generic ellipse drawing support functions */

/******************************************************************************
 *
 * uglGenericEllipse - Generic ellipse draw
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericEllipse (
    UGL_DEVICE_ID  devId,
    UGL_RECT *     pBoundRect,
    UGL_POINT *    pArcStart,
    UGL_POINT *    pArcEnd
    );

/* Generic transparent bitmap support functions */

/******************************************************************************
 *
 * uglGenericTransBitmapCreate - Create generic transparent bitmap
 *
 * RETURNS: UGL_TDDB_ID or UGL_NULL
 */

UGL_TDDB_ID uglGenericTransBitmapCreate (
    UGL_DEVICE_ID        devId,
    UGL_DIB *            pDib,
    UGL_MDIB *           pMdib,
    UGL_DIB_CREATE_MODE  createMode,
    UGL_COLOR            initValue,
    UGL_MEM_POOL_ID      poolId
    );

/******************************************************************************
 *
 * uglGenericTransBitmapCreateFromDdb - Create generic transparent bitmap ddb
 *
 * RETURNS: UGL_TDDB_ID or UGL_NULL
 */

UGL_TDDB_ID uglGenericTransBitmapCreateFromDdb (
    UGL_DEVICE_ID    devId,
    UGL_DDB_ID       ddbId,
    UGL_MDDB_ID      mDdbId,
    UGL_MEM_POOL_ID  poolId
    );

/******************************************************************************
 *
 * uglGenericTransBitmapDestroy - Destroy generic transparent bitmap
 *
 * RETURNS: UGL_STATUS_OK
 */

UGL_STATUS uglGenericTransBitmapDestroy (
    UGL_DEVICE_ID    devId,
    UGL_TDDB_ID      tDdbId
    );

/******************************************************************************
 *
 * uglGenericTransBitmapLinearBlt - Draw transparent bitmap for linear display
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericTransBitmapLinearBlt (
    UGL_DEVICE_ID  devId,
    UGL_TDDB_ID    srcBmpId,
    UGL_RECT *     pSrcRect,
    UGL_DDB_ID     destBmpId,
    UGL_POINT *    pDestPoint
    );

/******************************************************************************
 *
 * uglGeneric8BitMonoBitmapCreate - Create 8-bit monochrome bitmap
 *
 * RETURNS: Bitmap id or UGL_NULL
 */

UGL_MDDB_ID uglGeneric8BitMonoBitmapCreate (
    UGL_DEVICE_ID        devId,
    UGL_MDIB *           pMdib,
    UGL_DIB_CREATE_MODE  createMode,
    UGL_UINT8            initValue,
    UGL_MEM_POOL_ID      poolId
    );

/******************************************************************************
 *
 * uglGeneric8BitMonoBitmapDestroy - Free monochrome bitmap
 *
 * RETURNS: UGL_STATUS_OK
 */

UGL_STATUS uglGeneric8BitMonoBitmapDestroy (
    UGL_DEVICE_ID   devId,
    UGL_MDDB_ID     mDdbId
    );

/******************************************************************************
 *
 * uglGeneric8BitMonoBitmapWrite - Write monochrome bitmap
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGeneric8BitMonoBitmapWrite (
    UGL_DEVICE_ID  devId,
    UGL_MDIB *     pMdib,
    UGL_RECT *     pSrcRect,
    UGL_MDDB_ID    mDdbId,
    UGL_POINT *    pDestPoint
    );

/* Generic cursor support functions */

/******************************************************************************
 *
 * uglGenericCursorInit - Initialize generic cursor
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorInit (
    UGL_DEVICE_ID  devId,
    UGL_SIZE       maxWidth,
    UGL_SIZE       maxHeight,
    UGL_POS        xPosition,
    UGL_POS        yPosition
    );

/******************************************************************************
 *
 * uglGenericCursorDeinit - Deinitialize generic cursor
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorDeinit (
    UGL_DEVICE_ID  devId
    );

/******************************************************************************
 *
 * uglGenericCursorBitmapCreate - Create generic cursor bitmap
 *
 * RETURNS: UGL_CDDB_ID or UGL_NULL
 */

UGL_CDDB_ID uglGenericCursorBitmapCreate (
    UGL_DEVICE_ID  devId,
    UGL_CDIB *     pCdib
    );

/******************************************************************************
 *
 * uglGenericCursorBitmapDestroy - Destroy generic cursor bitmap
 *
 * RETURNS: UGL_STATUS_OK
 */

UGL_STATUS uglGenericCursorBitmapDestroy (
    UGL_DEVICE_ID  devId,
    UGL_CDDB_ID    cDdbId
    );

/******************************************************************************
 *
 * uglGenericCursorImageSet - Set cursor bitmap image
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorImageSet (
    UGL_DEVICE_ID  devId,
    UGL_CDDB *     pCddb
    );

/******************************************************************************
 *
 * uglGenericCursorImageGet - Get cursor bitmap image
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorImageGet (
    UGL_DEVICE_ID  devId,
    UGL_CDDB **    ppCddb
    );

/******************************************************************************
 *
 * uglGenericCursorOn - Turn on cursor
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorOn (
    UGL_DEVICE_ID  devId
    );

/******************************************************************************
 *
 * uglGenericCursorOff - Turn off cursor
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorOff (
    UGL_DEVICE_ID  devId
    );

/******************************************************************************
 *
 * uglGenericCursorHide - Hide cursor
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorHide (
    UGL_DEVICE_ID  devId,
    UGL_RECT *     pRect
    );

/******************************************************************************
 *
 * uglGenericCursorShow - Show cursor
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorShow (
    UGL_DEVICE_ID  devId
    );

/******************************************************************************
 *
 * uglGenericCursorMove - Move cursor
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGenericCursorMove (
    UGL_DEVICE_ID  devId,
    UGL_POINT *    pCursorPos
    );

/* Generic color support methods */

/******************************************************************************
 *
 * uglGeneric8BitColorConvert - Color convert for 8-bit color
 *
 * RETURNS: UGL_STATUS_OK or UGL_STATUS_ERROR
 */

UGL_STATUS uglGeneric8BitColorConvert (
    UGL_DEVICE_ID     devId,
    void *            srcArray,
    UGL_COLOR_FORMAT  srcFormat,
    void *            destArray,
    UGL_COLOR_FORMAT  destFormat,
    UGL_SIZE          arraySize
    );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ASMLANGUAGE */

#endif /* _udgen_h */

