/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2016-2021 Ivailo Monev
**
** This file is part of the QtGui module of the Katie Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

/***************************************************************************/
/*                                                                         */
/*  ftimage.h                                                              */
/*                                                                         */
/*    FreeType glyph image formats and default raster interface            */
/*    (specification).                                                     */
/*                                                                         */
/*  Copyright 1996-2001, 2002, 2003, 2004 by                               */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#ifndef __QT_FTIMAGE_H__
#define __QT_FTIMAGE_H__

/*
//  W A R N I N G
//  -------------
//
// This file is not part of the Katie API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
*/

  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    basic_types                                                        */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    QT_FT_Pos                                                             */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The type QT_FT_Pos is a 32-bit integer used to store vectorial        */
  /*    coordinates.  Depending on the context, these can represent        */
  /*    distances in integer font units, or 16,16, or 26.6 fixed float     */
  /*    pixel coordinates.                                                 */
  /*                                                                       */
  typedef signed int  QT_FT_Pos;


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    QT_FT_Vector                                                          */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A simple structure used to store a 2D vector; coordinates are of   */
  /*    the QT_FT_Pos type.                                                   */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    x :: The horizontal coordinate.                                    */
  /*    y :: The vertical coordinate.                                      */
  /*                                                                       */
  typedef struct  QT_FT_Vector_
  {
    QT_FT_Pos  x;
    QT_FT_Pos  y;

  } QT_FT_Vector;

  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    QT_FT_BBox                                                            */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A structure used to hold an outline's bounding box, i.e., the      */
  /*    coordinates of its extrema in the horizontal and vertical          */
  /*    directions.                                                        */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    xMin :: The horizontal minimum (left-most).                        */
  /*                                                                       */
  /*    yMin :: The vertical minimum (bottom-most).                        */
  /*                                                                       */
  /*    xMax :: The horizontal maximum (right-most).                       */
  /*                                                                       */
  /*    yMax :: The vertical maximum (top-most).                           */
  /*                                                                       */
  typedef struct  QT_FT_BBox_
  {
    QT_FT_Pos  xMin, yMin;
    QT_FT_Pos  xMax, yMax;

  } QT_FT_BBox;

  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    outline_processing                                                 */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    QT_FT_Outline                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  /*    This structure is used to describe an outline to the scan-line     */
  /*    converter.                                                         */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    n_contours :: The number of contours in the outline.               */
  /*                                                                       */
  /*    n_points   :: The number of points in the outline.                 */
  /*                                                                       */
  /*    points     :: A pointer to an array of `n_points' QT_FT_Vector        */
  /*                  elements, giving the outline's point coordinates.    */
  /*                                                                       */
  /*    tags       :: A pointer to an array of `n_points' chars, giving    */
  /*                  each outline point's type.  If bit 0 is unset, the   */
  /*                  point is `off' the curve, i.e. a Bezier control      */
  /*                  point, while it is `on' when set.                    */
  /*                                                                       */
  /*                  Bit 1 is meaningful for `off' points only.  If set,  */
  /*                  it indicates a third-order Bezier arc control point; */
  /*                  and a second-order control point if unset.           */
  /*                                                                       */
  /*    contours   :: An array of `n_contours' shorts, giving the end      */
  /*                  point of each contour within the outline.  For       */
  /*                  example, the first contour is defined by the points  */
  /*                  `0' to `contours[0]', the second one is defined by   */
  /*                  the points `contours[0]+1' to `contours[1]', etc.    */
  /*                                                                       */
  /*    flags      :: A set of bit flags used to characterize the outline  */
  /*                  and give hints to the scan-converter and hinter on   */
  /*                  how to convert/grid-fit it.  See QT_FT_Outline_Flags.   */
  /*                                                                       */
  typedef struct  QT_FT_Outline_
  {
    int       n_contours;      /* number of contours in glyph        */
    int       n_points;        /* number of points in the glyph      */

    QT_FT_Vector*  points;          /* the outline's points               */
    char*       tags;            /* the points flags                   */
    int*      contours;        /* the contour end points             */

    int         flags;           /* outline masks                      */

  } QT_FT_Outline;


  /*************************************************************************/
  /*                                                                       */
  /* <Enum>                                                                */
  /*   QT_FT_OUTLINE_FLAGS                                                 */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A list of bit-field constants use for the flags in an outline's    */
  /*    `flags' field.                                                     */
  /*                                                                       */
  /* <Values>                                                              */
  /*    QT_FT_OUTLINE_NONE           :: Value 0 is reserved.               */
  /*                                                                       */
  /*   QT_FT_OUTLINE_EVEN_ODD_FILL   :: By default, outlines are filled using */
  /*                                 the non-zero winding rule.  If set to */
  /*                                 1, the outline will be filled using   */
  /*                                 the even-odd fill rule (only works    */
  /*                                 with the smooth raster).              */
#define QT_FT_OUTLINE_NONE             0x0
#define QT_FT_OUTLINE_EVEN_ODD_FILL    0x2

  /* */

#define QT_FT_CURVE_TAG( flag )  ( flag & 3 )

#define QT_FT_CURVE_TAG_ON           1
#define QT_FT_CURVE_TAG_CUBIC        2

  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    basic_types                                                        */
  /*                                                                       */
  /*************************************************************************/

  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/
  /*****                                                               *****/
  /*****            R A S T E R   D E F I N I T I O N S                *****/
  /*****                                                               *****/
  /*************************************************************************/
  /*************************************************************************/
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* A raster is a scan converter, in charge of rendering an outline into  */
  /* a a bitmap.  This section contains the public API for rasters.        */
  /*                                                                       */
  /* Note that in FreeType 2, all rasters are now encapsulated within      */
  /* specific modules called `renderers'.  See `freetype/ftrender.h' for   */
  /* more details on renderers.                                            */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Section>                                                             */
  /*    raster                                                             */
  /*                                                                       */
  /* <Title>                                                               */
  /*    Scanline converter                                                 */
  /*                                                                       */
  /* <Abstract>                                                            */
  /*    How vectorial outlines are converted into bitmaps and pixmaps.     */
  /*                                                                       */
  /* <Description>                                                         */
  /*    This section contains technical definitions.                       */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Type>                                                                */
  /*    QT_FT_Raster                                                       */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A handle (pointer) to a raster object.  Each object can be used    */
  /*    independently to convert an outline into a bitmap or pixmap.       */
  /*                                                                       */
  typedef struct TRaster_ *QT_FT_Raster;


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    QT_FT_Span                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A structure used to model a single span of gray (or black) pixels  */
  /*    when rendering a monochrome or anti-aliased bitmap.                */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    x        :: The span's horizontal start position.                  */
  /*                                                                       */
  /*    len      :: The span's length in pixels.                           */
  /*                                                                       */
  /*    coverage :: The span color/coverage, ranging from 0 (background)   */
  /*                to 255 (foreground).  Only used for anti-aliased       */
  /*                rendering.                                             */
  /*                                                                       */
  /* <Note>                                                                */
  /*    This structure is used by the span drawing callback type named     */
  /*    QT_FT_SpanFunc which takes the y-coordinate of the span as a       */
  /*    a parameter.                                                       */
  /*                                                                       */
  /*    The coverage value is always between 0 and 255, even if the number */
  /*    of gray levels have been set through QT_FT_Set_Gray_Levels().      */
  /*                                                                       */
  typedef struct  QT_FT_Span_
  {
    short           x;
    unsigned short  len;
    short y;
    unsigned char   coverage;
  } QT_FT_Span;


  /*************************************************************************/
  /*                                                                       */
  /* <FuncType>                                                            */
  /*    QT_FT_SpanFunc                                                     */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A function used as a call-back by the anti-aliased renderer in     */
  /*    order to let client applications draw themselves the gray pixel    */
  /*    spans on each scan line.                                           */
  /*                                                                       */
  /* <Input>                                                               */
  /*    y     :: The scanline's y-coordinate.                              */
  /*                                                                       */
  /*    count :: The number of spans to draw on this scanline.             */
  /*                                                                       */
  /*    spans :: A table of `count' spans to draw on the scanline.         */
  /*                                                                       */
  /*    user  :: User-supplied data that is passed to the callback.        */
  /*                                                                       */
  /* <Note>                                                                */
  /*    This callback allows client applications to directly render the    */
  /*    gray spans of the anti-aliased bitmap to any kind of surfaces.     */
  /*                                                                       */
  /*    This can be used to write anti-aliased outlines directly to a      */
  /*    given background bitmap, and even perform translucency.            */
  /*                                                                       */
  /*    Note that the `count' field cannot be greater than a fixed value   */
  /*    defined by the QT_FT_MAX_GRAY_SPANS configuration macro in         */
  /*    ftoption.h.  By default, this value is set to 32, which means that */
  /*    if there are more than 32 spans on a given scanline, the callback  */
  /*    will be called several times with the same `y' parameter in order  */
  /*    to draw all callbacks.                                             */
  /*                                                                       */
  /*    Otherwise, the callback is only called once per scan-line, and     */
  /*    only for those scanlines that do have `gray' pixels on them.       */
  /*                                                                       */
  typedef void
  (*QT_FT_SpanFunc)(int        count,
                    const QT_FT_Span*  spans,
                    void*     worker);


  /*************************************************************************/
  /*                                                                       */
  /* <Struct>                                                              */
  /*    QT_FT_Raster_Params                                                */
  /*                                                                       */
  /* <Description>                                                         */
  /*    A structure to hold the arguments used by a raster's render        */
  /*    function.                                                          */
  /*                                                                       */
  /* <Fields>                                                              */
  /*    target      :: The target bitmap.                                  */
  /*                                                                       */
  /*    source      :: A pointer to the source glyph image (e.g. an        */
  /*                   QT_FT_Outline).                                     */
  /*                                                                       */
  /*    flags       :: The rendering flags.                                */
  /*                                                                       */
  /*    gray_spans  :: The gray span drawing callback.                     */
  /*                                                                       */
  /*    black_spans :: The black span drawing callback.                    */
  /*                                                                       */
  /*    user        :: User-supplied data that is passed to each drawing   */
  /*                   callback.                                           */
  /*                                                                       */
  /*    clip_box    :: An optional clipping box.  It is only used in       */
  /*                   direct rendering mode.  Note that coordinates here  */
  /*                   should be expressed in _integer_ pixels (and not in */
  /*                   26.6 fixed-point units).                            */
  /*                                                                       */
  /* <Note>                                                                */
  /*    An anti-aliased glyph bitmap is drawn if the QT_FT_RASTER_FLAG_AA bit */
  /*    flag is set in the `flags' field, otherwise a monochrome bitmap    */
  /*    will be generated.                                                 */
  /*                                                                       */
  /*    If the QT_FT_RASTER_FLAG_DIRECT bit flag is set in `flags', the    */
  /*    raster will call the `gray_spans' callback to draw gray pixel      */
  /*    spans, in the case of an aa glyph bitmap, it will call             */
  /*    `black_spans' in the case of a monochrome bitmap.  This allows     */
  /*    direct composition over a pre-existing bitmap through              */
  /*    user-provided callbacks to perform the span drawing/composition.   */
  /*                                                                       */
  /*    Note that the `bit_test' and `bit_set' callbacks are required when */
  /*    rendering a monochrome bitmap, as they are crucial to implement    */
  /*    correct drop-out control as defined in the TrueType specification. */
  /*                                                                       */
  typedef struct  QT_FT_Raster_Params_
  {
    QT_FT_Outline*                   source;
    QT_FT_SpanFunc             gray_spans;
    void*                   user;
    QT_FT_BBox                 clip_box;

  } QT_FT_Raster_Params;


  /*************************************************************************/
  /*                                                                       */
  /* <Func>                                                                */
  /*    gray_raster_reset                                                  */
  /*                                                                       */
  /* <Description>                                                         */
  /*    FreeType provides an area of memory called the `render pool',      */
  /*    available to all registered rasters.  This pool can be freely used */
  /*    during a given scan-conversion but is shared by all rasters.  Its  */
  /*    content is thus transient.                                         */
  /*                                                                       */
  /*    This function is called each time the render pool changes, or just */
  /*    after a new raster object is created.                              */
  /*                                                                       */
  /* <Note>                                                                */
  /*    Rasters can ignore the render pool and rely on dynamic memory      */
  /*    allocation if they want to (a handle to the memory allocator is    */
  /*    passed to the raster constructor).  However, this is not           */
  /*    recommended for efficiency purposes.                               */
  /*                                                                       */
#ifdef __cplusplus
  extern "C"
#endif
  void gray_raster_reset( );


  /*************************************************************************/
  /*                                                                       */
  /* <Func>                                                                */
  /*    gray_raster_render                                                 */
  /*                                                                       */
  /* <Description>                                                         */
  /*   Invokes a given raster to scan-convert a given glyph image into a   */
  /*   target bitmap.                                                      */
  /*                                                                       */
  /* <Input>                                                               */
  /*    raster :: A handle to the raster object.                           */
  /*                                                                       */
  /*    params :: A pointer to a QT_FT_Raster_Params structure used to store  */
  /*              the rendering parameters.                                */
  /*                                                                       */
  /* <Return>                                                              */
  /*    Error code.  0 means success.                                      */
  /*                                                                       */
  /* <Note>                                                                */
  /*    The exact format of the source image depends on the raster's glyph */
  /*    format defined in its QT_FT_Raster_Funcs structure.  It can be an     */
  /*    QT_FT_Outline or anything else in order to support a large array of   */
  /*    glyph formats.                                                     */
  /*                                                                       */
  /*    Note also that the render function can fail and return a           */
  /*    QT_FT_Err_Unimplemented_Feature error code if the raster used does    */
  /*    not support direct composition.                                    */
  /*                                                                       */
  /*    XXX: For now, the standard raster doesn't support direct           */
  /*         composition but this should change for the final release (see */
  /*         the files demos/src/ftgrays.c and demos/src/ftgrays2.c for    */
  /*         examples of distinct implementations which support direct     */
  /*         composition).                                                 */
  /*                                                                       */
#ifdef __cplusplus
  extern "C"
#endif
  int gray_raster_render( const QT_FT_Raster_Params*  params );

  /*************************************************************************/
  /*                                                                       */
  /* Minimum buffer size for raster object, that accounts for TWorker and  */
  /* TCell sizes.                                                          */
  /*                                                                       */
  #define RASTER_POOL_SIZE 8192

  /*************************************************************************/
  /*                                                                       */
  /* Any higher and rasterization of shapes will produce incorrect         */
  /* results.                                                              */
  /*                                                                       */
  #define RASTER_COORD_LIMIT 32767

#endif /* __FTIMAGE_H__ */


/* END */
