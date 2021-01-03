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
/*  qgrayraster.c, derived from ftgrays.c                                  */
/*                                                                         */
/*    A new `perfect' anti-aliasing renderer (body).                       */
/*                                                                         */
/*  Copyright 2000-2001, 2002, 2003 by                                     */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, ../../3rdparty/freetype/docs/FTL.TXT.  By continuing to use,  */
/*  modify, or distribute this file you indicate that you have read        */
/*  the license and understand and accept it fully.                        */
/*                                                                         */
/***************************************************************************/

  /*************************************************************************/
  /*                                                                       */
  /* This file can be compiled without the rest of the FreeType engine, by */
  /* defining the _STANDALONE_ macro when compiling it.  You also need to  */
  /* put the files `ftgrays.h' and `ftimage.h' into the current            */
  /* compilation directory.  Typically, you could do something like        */
  /*                                                                       */
  /* - copy `src/smooth/ftgrays.c' (this file) to your current directory   */
  /*                                                                       */
  /* - copy `include/freetype/ftimage.h' and `src/smooth/ftgrays.h' to the */
  /*   same directory                                                      */
  /*                                                                       */
  /* - compile `ftgrays' with the _STANDALONE_ macro defined, as in        */
  /*                                                                       */
  /*     cc -c -D_STANDALONE_ ftgrays.c                                    */
  /*                                                                       */
  /* The renderer can be initialized with a call to                        */
  /* `qt_ft_gray_raster.raster_new'; an anti-aliased bitmap can be generated  */
  /* with a call to `qt_ft_gray_raster.raster_render'.                        */
  /*                                                                       */
  /* See the comments and documentation in the file `ftimage.h' for more   */
  /* details on how the raster works.                                      */
  /*                                                                       */
  /*************************************************************************/

  /*************************************************************************/
  /*                                                                       */
  /* This is a new anti-aliasing scan-converter for FreeType 2.  The       */
  /* algorithm used here is _very_ different from the one in the standard  */
  /* `ftraster' module.  Actually, `ftgrays' computes the _exact_          */
  /* coverage of the outline on each pixel cell.                           */
  /*                                                                       */
  /* It is based on ideas that I initially found in Raph Levien's          */
  /* excellent LibArt graphics library (see http://www.levien.com/libart   */
  /* for more information, though the web pages do not tell anything       */
  /* about the renderer; you'll have to dive into the source code to       */
  /* understand how it works).                                             */
  /*                                                                       */
  /* Note, however, that this is a _very_ different implementation         */
  /* compared to Raph's.  Coverage information is stored in a very         */
  /* different way, and I don't use sorted vector paths.  Also, it doesn't */
  /* use floating point values.                                            */
  /*                                                                       */
  /* This renderer has the following advantages:                           */
  /*                                                                       */
  /* - It doesn't need an intermediate bitmap.  Instead, one can supply a  */
  /*   callback function that will be called by the renderer to draw gray  */
  /*   spans on any target surface.  You can thus do direct composition on */
  /*   any kind of bitmap, provided that you give the renderer the right   */
  /*   callback.                                                           */
  /*                                                                       */
  /* - A perfect anti-aliaser, i.e., it computes the _exact_ coverage on   */
  /*   each pixel cell.                                                    */
  /*                                                                       */
  /* - It performs a single pass on the outline (the `standard' FT2        */
  /*   renderer makes two passes).                                         */
  /*                                                                       */
  /* - It can easily be modified to render to _any_ number of gray levels  */
  /*   cheaply.                                                            */
  /*                                                                       */
  /* - For small (< 20) pixel sizes, it is faster than the standard        */
  /*   renderer.                                                           */
  /*                                                                       */
  /*************************************************************************/

#include <setjmp.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#define ErrRaster_Invalid_Outline   -1
#define ErrRaster_Invalid_Mode      -2
#define ErrRaster_Invalid_Argument  -3
#define ErrRaster_Memory_Overflow   -4
#define ErrRaster_OutOfMemory       -6

#include <qrasterdefs_p.h>

  /* define this to dump debugging information */
#define xxxDEBUG_GRAYS


#define RAS_ARG   PWorker  worker
#define RAS_ARG_  PWorker  worker,

#define RAS_VAR   worker
#define RAS_VAR_  worker,

#define ras       (*worker)


  /* must be at least 6 bits! */
#define PIXEL_BITS  8

#define ONE_PIXEL       ( 1L << PIXEL_BITS )
#define TRUNC( x )      ( (TCoord)( (x) >> PIXEL_BITS ) )
#define SUBPIXELS( x )  ( (TPos)(x) << PIXEL_BITS )

#if PIXEL_BITS >= 6
#define UPSCALE( x )    ( (x) << ( PIXEL_BITS - 6 ) )
#define DOWNSCALE( x )  ( (x) >> ( PIXEL_BITS - 6 ) )
#else
#define UPSCALE( x )    ( (x) >> ( 6 - PIXEL_BITS ) )
#define DOWNSCALE( x )  ( (x) << ( 6 - PIXEL_BITS ) )
#endif

  /*************************************************************************/
  /*                                                                       */
  /*   TYPE DEFINITIONS                                                    */
  /*                                                                       */

  /* don't change the following types to QT_FT_Int or QT_FT_Pos, since we might */
  /* need to define them to "float" or "double" when experimenting with   */
  /* new algorithms                                                       */

  typedef int   TCoord;   /* integer scanline/pixel coordinate */
  typedef int   TPos;     /* sub-pixel coordinate              */

  /* determine the type used to store cell areas.  This normally takes at */
  /* least PIXEL_BITS*2 + 1 bits.  On 16-bit systems, we need to use      */
  /* `long' instead of `int', otherwise bad things happen                 */

#if PIXEL_BITS <= 7

  typedef int  TArea;

#else /* PIXEL_BITS >= 8 */

  /* approximately determine the size of integers using an ANSI-C header */
#if UINT_MAX == 0xFFFFU
  typedef long  TArea;
#else
  typedef int   TArea;
#endif

#endif /* PIXEL_BITS >= 8 */


  /* maximal number of gray spans in a call to the span callback */
#define QT_FT_MAX_GRAY_SPANS  256


  typedef struct TCell_*  PCell;

  typedef struct  TCell_
  {
    int    x;
    int    cover;
    TArea  area;
    PCell  next;

  } TCell;


  typedef struct  TWorker_
  {
    TCoord  ex, ey;
    TPos    min_ex, max_ex;
    TPos    min_ey, max_ey;
    TPos    count_ex, count_ey;

    TArea   area;
    int     cover;
    int     invalid;

    PCell   cells;
    int     max_cells;
    int     num_cells;

    TPos    x,  y;

    TPos    last_ey;

    QT_FT_Vector   bez_stack[32 * 3 + 1];
    int            lev_stack[32];

    QT_FT_Outline  outline;
    QT_FT_BBox     clip_box;

    QT_FT_Span     gray_spans[QT_FT_MAX_GRAY_SPANS];
    int            num_gray_spans;

    QT_FT_SpanFunc render_span;
    void*          render_span_data;

    int  band_size;
    int  band_shoot;
    int  conic_level;
    int  cubic_level;

    jmp_buf  jump_buffer;

    char*       buffer;
    int         buffer_size;

    PCell*     ycells;
    int        ycount;

  } TWorker, *PWorker;


  typedef struct TRaster_
  {
    char*    buffer;
    int      buffer_size;
    int      band_size;
    PWorker  worker;

  } TRaster, *PRaster;

  /*************************************************************************/
  /*                                                                       */
  /* Record the current cell in the table.                                 */
  /*                                                                       */
  static void
  gray_record_cell( RAS_ARG )
  {
    PCell  *pcell, cell;
    int     x = ras.ex;

    if ( ras.invalid || !( ras.area | ras.cover ) )
        return;

    if ( x > ras.max_ex )
      x = ras.max_ex;

    pcell = &ras.ycells[ras.ey];

    for (;;)
    {
      cell = *pcell;
      if ( cell == NULL || cell->x > x )
        break;

      if ( cell->x == x ) {
          cell->area  += ras.area;
          cell->cover += ras.cover;
          return;
      }

      pcell = &cell->next;
    }

    if ( ras.num_cells >= ras.max_cells )
      longjmp( ras.jump_buffer, 1 );

    cell        = ras.cells + ras.num_cells++;
    cell->x     = x;
    cell->area  = ras.area;
    cell->cover = ras.cover;

    cell->next  = *pcell;
    *pcell      = cell;
  }


  /*************************************************************************/
  /*                                                                       */
  /* Set the current cell to a new position.                               */
  /*                                                                       */
  static void
  gray_set_cell( RAS_ARG_ TCoord  ex,
                          TCoord  ey )
  {
    /* Move the cell pointer to a new position.  We set the `invalid'      */
    /* flag to indicate that the cell isn't part of those we're interested */
    /* in during the render phase.  This means that:                       */
    /*                                                                     */
    /* . the new vertical position must be within min_ey..max_ey-1.        */
    /* . the new horizontal position must be strictly less than max_ex     */
    /*                                                                     */
    /* Note that if a cell is to the left of the clipping region, it is    */
    /* actually set to the (min_ex-1) horizontal position.                 */

    /* All cells that are on the left of the clipping region go to the */
    /* min_ex - 1 horizontal position.                                 */
    ey -= ras.min_ey;

    if ( ex > ras.max_ex )
      ex = ras.max_ex;

    ex -= ras.min_ex;
    if ( ex < 0 )
      ex = -1;

    /* are we moving to a different cell ? */
    if ( ex != ras.ex || ey != ras.ey )
    {
      /* record the current one if it is valid */
      if ( !ras.invalid )
        gray_record_cell( RAS_VAR );

      ras.area  = 0;
      ras.cover = 0;
    }

    ras.ex      = ex;
    ras.ey      = ey;
    ras.invalid = ( (unsigned)ey >= (unsigned)ras.count_ey ||
                              ex >= ras.count_ex           );
  }


  /*************************************************************************/
  /*                                                                       */
  /* Render a scanline as one or more cells.                               */
  /*                                                                       */
  static void
  gray_render_scanline( RAS_ARG_ TCoord  ey,
                                 TPos    x1,
                                 TCoord  y1,
                                 TPos    x2,
                                 TCoord  y2 )
  {
    TCoord  ex1, ex2, fx1, fx2, delta;
    int     p, first, dx;
    int     incr, mod;


    dx = x2 - x1;

    ex1 = TRUNC( x1 );
    ex2 = TRUNC( x2 );
    fx1 = (TCoord)( x1 - SUBPIXELS( ex1 ) );
    fx2 = (TCoord)( x2 - SUBPIXELS( ex2 ) );

    /* trivial case.  Happens often */
    if ( y1 == y2 )
    {
      gray_set_cell( RAS_VAR_ ex2, ey );
      return;
    }

    /* everything is located in a single cell.  That is easy! */
    /*                                                        */
    if ( ex1 == ex2 )
    {
      delta      = y2 - y1;
      ras.area  += (TArea)( fx1 + fx2 ) * delta;
      ras.cover += delta;
      return;
    }

    /* ok, we'll have to render a run of adjacent cells on the same */
    /* scanline...                                                  */
    /*                                                              */
    p     = ( ONE_PIXEL - fx1 ) * ( y2 - y1 );
    first = ONE_PIXEL;
    incr  = 1;

    if ( dx < 0 )
    {
      p     = fx1 * ( y2 - y1 );
      first = 0;
      incr  = -1;
      dx    = -dx;
    }

    delta = (TCoord)( p / dx );
    mod   = (TCoord)( p % dx );
    if ( mod < 0 )
    {
      delta--;
      mod += (TCoord)dx;
    }

    ras.area  += (TArea)( fx1 + first ) * delta;
    ras.cover += delta;

    ex1 += incr;
    gray_set_cell( RAS_VAR_ ex1, ey );
    y1  += delta;

    if ( ex1 != ex2 )
    {
      p    = ONE_PIXEL * ( y2 - y1 + delta );
      int lift = (TCoord)( p / dx );
      int rem  = (TCoord)( p % dx );
      if ( rem < 0 )
      {
        lift--;
        rem += (TCoord)dx;
      }

      mod -= (int)dx;

      while ( ex1 != ex2 )
      {
        delta = lift;
        mod  += rem;
        if ( mod >= 0 )
        {
          mod -= (TCoord)dx;
          delta++;
        }

        ras.area  += (TArea)ONE_PIXEL * delta;
        ras.cover += delta;
        y1        += delta;
        ex1       += incr;
        gray_set_cell( RAS_VAR_ ex1, ey );
      }
    }

    delta      = y2 - y1;
    ras.area  += (TArea)( fx2 + ONE_PIXEL - first ) * delta;
    ras.cover += delta;
  }


  /*************************************************************************/
  /*                                                                       */
  /* Render a given line as a series of scanlines.                         */
  /*                                                                       */
  static void
  gray_render_line( RAS_ARG_ TPos  to_x,
                             TPos  to_y )
  {
    TCoord  ey1, ey2, fy1, fy2;
    TPos    dx, dy, x, x2;
    int     p, first;
    int     delta, rem, mod, lift, incr;


    ey1 = TRUNC( ras.last_ey );
    ey2 = TRUNC( to_y );     /* if (ey2 >= ras.max_ey) ey2 = ras.max_ey-1; */
    fy1 = (TCoord)( ras.y - ras.last_ey );
    fy2 = (TCoord)( to_y - SUBPIXELS( ey2 ) );

    dx = to_x - ras.x;
    dy = to_y - ras.y;

    /* XXX: we should do something about the trivial case where dx == 0, */
    /*      as it happens very often!                                    */

    /* perform vertical clipping */
    {
      TCoord  min, max;


      min = ey1;
      max = ey2;
      if ( ey1 > ey2 )
      {
        min = ey2;
        max = ey1;
      }
      if ( min >= ras.max_ey || max < ras.min_ey )
        goto End;
    }

    /* everything is on a single scanline */
    if ( ey1 == ey2 )
    {
      gray_render_scanline( RAS_VAR_ ey1, ras.x, fy1, to_x, fy2 );
      goto End;
    }

    /* vertical line - avoid calling gray_render_scanline */
    incr = 1;

    if ( dx == 0 )
    {
      TCoord  ex     = TRUNC( ras.x );
      TCoord  two_fx = (TCoord)( ( ras.x - SUBPIXELS( ex ) ) << 1 );
      TPos    area;


      first = ONE_PIXEL;
      if ( dy < 0 )
      {
        first = 0;
        incr  = -1;
      }

      delta      = (int)( first - fy1 );
      ras.area  += (TArea)two_fx * delta;
      ras.cover += delta;
      ey1       += incr;

      gray_set_cell( &ras, ex, ey1 );

      delta = (int)( first + first - ONE_PIXEL );
      area  = (TArea)two_fx * delta;
      while ( ey1 != ey2 )
      {
        ras.area  += area;
        ras.cover += delta;
        ey1       += incr;

        gray_set_cell( &ras, ex, ey1 );
      }

      delta      = (int)( fy2 - ONE_PIXEL + first );
      ras.area  += (TArea)two_fx * delta;
      ras.cover += delta;

      goto End;
    }

    /* ok, we have to render several scanlines */
    p     = ( ONE_PIXEL - fy1 ) * dx;
    first = ONE_PIXEL;
    incr  = 1;

    if ( dy < 0 )
    {
      p     = fy1 * dx;
      first = 0;
      incr  = -1;
      dy    = -dy;
    }

    delta = (int)( p / dy );
    mod   = (int)( p % dy );
    if ( mod < 0 )
    {
      delta--;
      mod += (TCoord)dy;
    }

    x = ras.x + delta;
    gray_render_scanline( RAS_VAR_ ey1, ras.x, fy1, x, (TCoord)first );

    ey1 += incr;
    gray_set_cell( RAS_VAR_ TRUNC( x ), ey1 );

    if ( ey1 != ey2 )
    {
      p     = ONE_PIXEL * dx;
      lift  = (int)( p / dy );
      rem   = (int)( p % dy );
      if ( rem < 0 )
      {
        lift--;
        rem += (int)dy;
      }
      mod -= (int)dy;

      while ( ey1 != ey2 )
      {
        delta = lift;
        mod  += rem;
        if ( mod >= 0 )
        {
          mod -= (int)dy;
          delta++;
        }

        x2 = x + delta;
        gray_render_scanline( RAS_VAR_ ey1, x,
                                       (TCoord)( ONE_PIXEL - first ), x2,
                                       (TCoord)first );
        x = x2;

        ey1 += incr;
        gray_set_cell( RAS_VAR_ TRUNC( x ), ey1 );
      }
    }

    gray_render_scanline( RAS_VAR_ ey1, x,
                                   (TCoord)( ONE_PIXEL - first ), to_x,
                                   fy2 );

  End:
    ras.x       = to_x;
    ras.y       = to_y;
    ras.last_ey = SUBPIXELS( ey2 );
  }


  static void
  gray_render_cubic( RAS_ARG_ const QT_FT_Vector*  control1,
                              const QT_FT_Vector*  control2,
                              const QT_FT_Vector*  to )
  {
    TPos        dx, dy, da, db;
    int         top, level;
    int*        levels;
    QT_FT_Vector*  arc;


    dx = DOWNSCALE( ras.x ) + to->x - ( control1->x << 1 );
    if ( dx < 0 )
      dx = -dx;
    dy = DOWNSCALE( ras.y ) + to->y - ( control1->y << 1 );
    if ( dy < 0 )
      dy = -dy;
    if ( dx < dy )
      dx = dy;
    da = dx;

    dx = DOWNSCALE( ras.x ) + to->x - 3 * ( control1->x + control2->x );
    if ( dx < 0 )
      dx = -dx;
    dy = DOWNSCALE( ras.y ) + to->y - 3 * ( control1->y + control2->y );
    if ( dy < 0 )
      dy = -dy;
    if ( dx < dy )
      dx = dy;
    db = dx;

    level = 1;
    da    = da / ras.cubic_level;
    db    = db / ras.conic_level;
    while ( da > 0 || db > 0 )
    {
      da >>= 2;
      db >>= 3;
      level++;
    }

    if ( level <= 1 )
    {
      TPos   to_x, to_y, mid_x, mid_y;


      to_x  = UPSCALE( to->x );
      to_y  = UPSCALE( to->y );
      mid_x = ( ras.x + to_x +
                3 * UPSCALE( control1->x + control2->x ) ) / 8;
      mid_y = ( ras.y + to_y +
                3 * UPSCALE( control1->y + control2->y ) ) / 8;

      gray_render_line( RAS_VAR_ mid_x, mid_y );
      gray_render_line( RAS_VAR_ to_x, to_y );
      return;
    }

    arc      = ras.bez_stack;
    arc[0].x = UPSCALE( to->x );
    arc[0].y = UPSCALE( to->y );
    arc[1].x = UPSCALE( control2->x );
    arc[1].y = UPSCALE( control2->y );
    arc[2].x = UPSCALE( control1->x );
    arc[2].y = UPSCALE( control1->y );
    arc[3].x = ras.x;
    arc[3].y = ras.y;

    levels    = ras.lev_stack;
    top       = 0;
    levels[0] = level;

    while ( top >= 0 )
    {
      level = levels[top];
      if ( level > 1 )
      {
        /* check that the arc crosses the current band */
        TPos  min, max, y;


        min = max = arc[0].y;
        y = arc[1].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        y = arc[2].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        y = arc[3].y;
        if ( y < min ) min = y;
        if ( y > max ) max = y;
        if ( TRUNC( min ) >= ras.max_ey || TRUNC( max ) < 0 )
          goto Draw;

        /* split cubic */
        TPos  a, b, c, d;

        arc[6].x = arc[3].x;
        c = arc[1].x;
        d = arc[2].x;
        arc[1].x = a = ( arc[0].x + c ) / 2;
        arc[5].x = b = ( arc[3].x + d ) / 2;
        c = ( c + d ) / 2;
        arc[2].x = a = ( a + c ) / 2;
        arc[4].x = b = ( b + c ) / 2;
        arc[3].x = ( a + b ) / 2;

        arc[6].y = arc[3].y;
        c = arc[1].y;
        d = arc[2].y;
        arc[1].y = a = ( arc[0].y + c ) / 2;
        arc[5].y = b = ( arc[3].y + d ) / 2;
        c = ( c + d ) / 2;
        arc[2].y = a = ( a + c ) / 2;
        arc[4].y = b = ( b + c ) / 2;
        arc[3].y = ( a + b ) / 2;

        arc += 3;
        top ++;
        levels[top] = levels[top - 1] = level - 1;
        continue;
      }

    Draw:
      {
        TPos  to_x, to_y, mid_x, mid_y;


        to_x  = arc[0].x;
        to_y  = arc[0].y;
        mid_x = ( ras.x + to_x + 3 * ( arc[1].x + arc[2].x ) ) / 8;
        mid_y = ( ras.y + to_y + 3 * ( arc[1].y + arc[2].y ) ) / 8;

        gray_render_line( RAS_VAR_ mid_x, mid_y );
        gray_render_line( RAS_VAR_ to_x, to_y );
        top --;
        arc -= 3;
      }
    }
  }


  static inline void
  gray_move_to( const QT_FT_Vector*  to,
                PWorker           worker )
  {
    TPos  x, y;

    /* record current cell, if any */
    gray_record_cell( worker );

    /* start to a new position */
    x = UPSCALE( to->x );
    y = UPSCALE( to->y );

    /* start a new contour at a given cell */
    TCoord ex = TRUNC( x );
    TCoord ey = TRUNC( y );

    if ( ex > ras.max_ex )
      ex = (TCoord)( ras.max_ex );

    if ( ex < ras.min_ex )
      ex = (TCoord)( ras.min_ex - 1 );

    ras.area    = 0;
    ras.cover   = 0;
    ras.ex      = ex - ras.min_ex;
    ras.ey      = ey - ras.min_ey;
    ras.last_ey = SUBPIXELS( ey );
    ras.invalid = 0;

    gray_set_cell( RAS_VAR_ ex, ey );

    worker->x = x;
    worker->y = y;
  }


  static inline void
  gray_line_to( const QT_FT_Vector*  to,
                PWorker           worker )
  {
    gray_render_line( worker, UPSCALE( to->x ), UPSCALE( to->y ) );
  }

  static void
  gray_hline( RAS_ARG_ TCoord  x,
                       TCoord  y,
                       TPos    area,
                       int     acount )
  {
    int       coverage;


    /* compute the coverage line's coverage, depending on the    */
    /* outline fill rule                                         */
    /*                                                           */
    /* the coverage percentage is area/(PIXEL_BITS*PIXEL_BITS*2) */
    /*                                                           */
    coverage = (int)( area >> ( PIXEL_BITS * 2 + 1 - 8 ) );
                                                    /* use range 0..256 */
    if ( coverage < 0 )
      coverage = -coverage;

    if ( ras.outline.flags & QT_FT_OUTLINE_EVEN_ODD_FILL )
    {
      coverage &= 511;

      if ( coverage > 256 )
        coverage = 512 - coverage;
      else if ( coverage == 256 )
        coverage = 255;
    }
    else
    {
      /* normal non-zero winding rule */
      if ( coverage >= 256 )
        coverage = 255;
    }

    y += (TCoord)ras.min_ey;
    x += (TCoord)ras.min_ex;

    /* QT_FT_Span.x is a 16-bit short, so limit our coordinates appropriately */
    if ( x >= 32768 )
      x = 32767;

    if ( coverage )
    {
      /* see whether we can add this span to the current list */
      QT_FT_Span* span  = ras.gray_spans + ras.num_gray_spans - 1;
      if ( ras.num_gray_spans > 0             &&
           span->y == y                       &&
           (int)span->x + span->len == (int)x &&
           span->coverage == coverage         )
      {
        span->len = (unsigned short)( span->len + acount );
        return;
      }

      if ( ras.num_gray_spans >= QT_FT_MAX_GRAY_SPANS )
      {
        ras.render_span( ras.num_gray_spans,
                         ras.gray_spans,
                         ras.render_span_data );

        /* ras.render_span( span->y, ras.gray_spans, count ); */

#ifdef DEBUG_GRAYS
        fprintf( stderr, "y=%3d ", y );
        span = ras.gray_spans;
        int n;
        for ( n = 0; n < count; n++, span++ )
          fprintf( stderr, "[%d..%d]:%02x ",
                   span->x, span->x + span->len - 1, span->coverage );
        fprintf( stderr, "\n" );
#endif /* DEBUG_GRAYS */

        ras.num_gray_spans = 0;

        span  = ras.gray_spans;
      }
      else
        span++;

      /* add a gray span to the current list */
      span->x        = (short)x;
      span->len      = (unsigned short)acount;
      span->y        = (short)y;
      span->coverage = (unsigned char)coverage;

      ras.num_gray_spans++;
    }
  }


#ifdef DEBUG_GRAYS
  /* to be called while in the debugger */
  void gray_dump_cells( RAS_ARG )
  {
    int yindex;
    for ( yindex = 0; yindex < ras.ycount; yindex++ )
    {
      PCell  cell;

      printf( "%3d:", yindex );

      for ( cell = ras.ycells[yindex]; cell != NULL; cell = cell->next )
        printf( " (%3d, c:%4d, a:%6d)", cell->x, cell->cover, cell->area );
      printf( "\n" );
    }
  }
#endif /* DEBUG_GRAYS */


  /*************************************************************************/
  /*                                                                       */
  /*  The following function should only compile in stand_alone mode,      */
  /*  i.e., when building this component without the rest of FreeType.     */
  /*                                                                       */
  /*************************************************************************/

  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    QT_FT_Outline_Decompose                                               */
  /*                                                                       */
  /* <Description>                                                         */
  /*    Walks over an outline's structure to decompose it into individual  */
  /*    segments and Bezier arcs.  This function is also able to emit      */
  /*    `move to' and `close to' operations to indicate the start and end  */
  /*    of new contours in the outline.                                    */
  /*                                                                       */
  /* <Input>                                                               */
  /*    outline        :: A pointer to the source target.                  */
  /*                                                                       */
  /*    user           :: A typeless pointer which is passed to each       */
  /*                      emitter during the decomposition.  It can be     */
  /*                      used to store the state during the               */
  /*                      decomposition.                                   */
  /*                                                                       */
  /* <Return>                                                              */
  /*    Error code.  0 means success.                                      */
  /*                                                                       */
  static
  int  QT_FT_Outline_Decompose( const QT_FT_Outline*        outline,
                             void*                    user )
  {
    QT_FT_Vector   v_start;

    QT_FT_Vector*  point;
    QT_FT_Vector*  limit;
    char*          tags;

    int   n;         /* index of contour in outline     */
    int   first;     /* index of first point in contour */
    char  tag;       /* current point's state           */

    first = 0;

    for ( n = 0; n < outline->n_contours; n++ )
    {
      /* index of last point in contour */
      const int last  = outline->contours[n];
      limit = outline->points + last;

      v_start = outline->points[first];

      point = outline->points + first;
      tags  = outline->tags  + first;
      tag   = QT_FT_CURVE_TAG( tags[0] );

      /* A contour cannot start with a cubic control point! */
      if ( tag == QT_FT_CURVE_TAG_CUBIC )
        return ErrRaster_Invalid_Outline;

      gray_move_to( &v_start, user );

      while ( point < limit )
      {
        point++;
        tags++;

        tag = QT_FT_CURVE_TAG( tags[0] );
        switch ( tag )
        {
        case QT_FT_CURVE_TAG_ON:  /* emit a single line_to */
          {
            QT_FT_Vector  vec;


            vec.x = point->x;
            vec.y = point->y;

            gray_line_to( &vec, user );
            continue;
          }

        default:  /* QT_FT_CURVE_TAG_CUBIC */
          {
            QT_FT_Vector  vec1, vec2;


            if ( point + 1 > limit ||
                 QT_FT_CURVE_TAG( tags[1] ) != QT_FT_CURVE_TAG_CUBIC )
              return ErrRaster_Invalid_Outline;

            point += 2;
            tags  += 2;

            vec1.x = point[-2].x;
            vec1.y = point[-2].y;

            vec2.x = point[-1].x;
            vec2.y = point[-1].y;

            if ( point <= limit )
            {
              QT_FT_Vector  vec;


              vec.x = point->x;
              vec.y = point->y;

              gray_render_cubic( user, &vec1, &vec2, &vec );
              continue;
            }

            gray_render_cubic( user, &vec1, &vec2, &v_start );
            goto Close;
          }
        }
      }

      /* close the contour with a line segment */
      gray_line_to( &v_start, user );

   Close:
      first = last + 1;
    }

    return 0;
  }

  typedef struct  TBand_
  {
    TPos  min, max;

  } TBand;


  static int
  gray_convert_glyph( RAS_ARG )
  {
    TBand            bands[40];
    TBand* volatile  band;
    int volatile     n, num_bands;
    TPos volatile    min, max, max_y;
    int              yindex;

    ras.num_gray_spans = 0;

    /* Set up state in the raster object, compute the outline bounding box */
    QT_FT_Outline*  outline = &ras.outline;

    if ( outline->n_points <= 0 ) {
      ras.min_ex = ras.max_ex = 0;
      ras.min_ey = ras.max_ey = 0;
    } else {
      QT_FT_Vector*   vec     = outline->points;
      QT_FT_Vector*   limit   = vec + outline->n_points;
      ras.min_ex = ras.max_ex = vec->x;
      ras.min_ey = ras.max_ey = vec->y;

      vec++;

      for ( ; vec < limit; vec++ )
      {
        TPos  x = vec->x;
        TPos  y = vec->y;


        if ( x < ras.min_ex ) ras.min_ex = x;
        if ( x > ras.max_ex ) ras.max_ex = x;
        if ( y < ras.min_ey ) ras.min_ey = y;
        if ( y > ras.max_ey ) ras.max_ey = y;
      }

      /* truncate the bounding box to integer pixels */
      ras.min_ex = ras.min_ex >> 6;
      ras.min_ey = ras.min_ey >> 6;
      ras.max_ex = ( ras.max_ex + 63 ) >> 6;
      ras.max_ey = ( ras.max_ey + 63 ) >> 6;
    }

    /* clip to target bitmap, exit if nothing to do */
    if ( ras.max_ex <= ras.clip_box.xMin || ras.min_ex >= ras.clip_box.xMax ||
         ras.max_ey <= ras.clip_box.yMin || ras.min_ey >= ras.clip_box.yMax )
      return 0;

    if ( ras.min_ex < ras.clip_box.xMin ) ras.min_ex = ras.clip_box.xMin;
    if ( ras.min_ey < ras.clip_box.yMin ) ras.min_ey = ras.clip_box.yMin;

    if ( ras.max_ex > ras.clip_box.xMax ) ras.max_ex = ras.clip_box.xMax;
    if ( ras.max_ey > ras.clip_box.yMax ) ras.max_ey = ras.clip_box.yMax;

    ras.count_ex = ras.max_ex - ras.min_ex;
    ras.count_ey = ras.max_ey - ras.min_ey;

    /* simple heuristic used to speed-up the bezier decomposition -- see */
    /* the code in gray_render_cubic() for more  details                 */
    if ( ras.count_ex > 120 || ras.count_ey > 120 )
    {
      ras.conic_level = 128;
      ras.cubic_level = 64;
    }
    else if ( ras.count_ex > 24 || ras.count_ey > 24 )
    {
      ras.conic_level = 64;
      ras.cubic_level = 32;
    }
    else
    {
      ras.conic_level = 32;
      ras.cubic_level = 16;
    }

    /* setup vertical bands */
    num_bands = (int)( ( ras.max_ey - ras.min_ey ) / ras.band_size );
    if ( num_bands == 0 )  num_bands = 1;
    if ( num_bands >= 39 ) num_bands = 39;

    ras.band_shoot = 0;

    min   = ras.min_ey;
    max_y = ras.max_ey;

    for ( n = 0; n < num_bands; n++, min = max )
    {
      max = min + ras.band_size;
      if ( n == num_bands - 1 || max > max_y )
        max = max_y;

      bands[0].min = min;
      bands[0].max = max;
      band         = bands;

      while ( band >= bands )
      {
        TPos  bottom, top, middle;
        int   error;

        {
          PCell  cells_max;
          int    cell_start, cell_end, cell_mod;


          ras.ycells = (PCell*)ras.buffer;
          ras.ycount = band->max - band->min;

          cell_start = sizeof ( PCell ) * ras.ycount;
          cell_mod   = cell_start % sizeof ( TCell );
          if ( cell_mod > 0 )
            cell_start += sizeof ( TCell ) - cell_mod;

          cell_end  = ras.buffer_size;
          cell_end -= cell_end % sizeof( TCell );

          cells_max = (PCell)( ras.buffer + cell_end );
          ras.cells = (PCell)( ras.buffer + cell_start );
          if ( ras.cells >= cells_max )
            goto ReduceBands;

          ras.max_cells = (int)(cells_max - ras.cells);
          if ( ras.max_cells < 2 )
            goto ReduceBands;

          for ( yindex = 0; yindex < ras.ycount; yindex++ )
            ras.ycells[yindex] = NULL;
        }

        ras.num_cells = 0;
        ras.invalid   = 1;
        ras.min_ey    = band->min;
        ras.max_ey    = band->max;
        ras.count_ey  = band->max - band->min;

        if ( setjmp( ras.jump_buffer ) == 0 )
        {
          error = QT_FT_Outline_Decompose( &ras.outline, &ras );
          gray_record_cell( RAS_VAR );
        }
        else
        {
          error = ErrRaster_Memory_Overflow;
        }

        if ( !error && ras.num_cells != 0)
        {
          /* sweep */
          for ( yindex = 0; yindex < ras.ycount; yindex++ )
          {
            PCell   cell  = ras.ycells[yindex];
            TCoord  cover = 0;
            TCoord  x     = 0;


            for ( ; cell != NULL; cell = cell->next )
            {
              TArea  area;

              if ( cell->x > x && cover != 0 )
                gray_hline( RAS_VAR_ x, yindex, cover * ( ONE_PIXEL * 2 ),
                            cell->x - x );

              cover += cell->cover;
              area   = cover * ( ONE_PIXEL * 2 ) - cell->area;

              if ( area != 0 && cell->x >= 0 )
                gray_hline( RAS_VAR_ cell->x, yindex, area, 1 );

              x = cell->x + 1;
            }

            if ( ras.count_ex > x && cover != 0 )
              gray_hline( RAS_VAR_ x, yindex, cover * ( ONE_PIXEL * 2 ),
                          ras.count_ex - x );
          }

          band--;
          continue;
        }
        else if ( error != ErrRaster_Memory_Overflow )
          return 1;

      ReduceBands:
        /* render pool overflow; we will reduce the render band by half */
        bottom = band->min;
        top    = band->max;
        middle = bottom + ( ( top - bottom ) >> 1 );

        /* This is too complex for a single scanline; there must */
        /* be some problems.                                     */
        if ( middle == bottom )
        {
#ifdef DEBUG_GRAYS
          fprintf( stderr, "Rotten glyph!\n" );
#endif
          return ErrRaster_OutOfMemory;
        }

        if ( bottom-top >= ras.band_size )
          ras.band_shoot++;

        band[1].min = bottom;
        band[1].max = middle;
        band[0].min = middle;
        band[0].max = top;
        band++;
      }
    }

    ras.render_span( ras.num_gray_spans,
                     ras.gray_spans,
                     ras.render_span_data );

    if ( ras.band_shoot > 8 && ras.band_size > 16 )
      ras.band_size = ras.band_size / 2;

    return 0;
  }


  /**** RASTER OBJECT CREATION: In standalone mode, we simply use *****/
  /****                         a static object.                  *****/
  static TRaster raster;
  static char raster_pool[RASTER_POOL_SIZE];

  void gray_raster_reset( )
  {
    raster.worker      = (PWorker)raster_pool;
    raster.buffer      = raster_pool +
                          ( ( sizeof ( TWorker ) + sizeof ( TCell ) - 1 ) &
                          ~( sizeof ( TCell ) - 1 ) );
    raster.buffer_size = ( ( raster_pool + RASTER_POOL_SIZE ) -
                          raster.buffer ) & ~( sizeof ( TCell ) - 1 );
    raster.band_size   = raster.buffer_size / ( sizeof ( TCell ) * 8 );
  }

  int gray_raster_render( const QT_FT_Raster_Params*  params )
  {
    const QT_FT_Outline*  outline    = params->source;
    PWorker            worker;

    if ( !raster.buffer || !raster.buffer_size )
      return ErrRaster_Invalid_Argument;

    /* return immediately if the outline is empty */
    if ( !outline || outline->n_points == 0 || outline->n_contours <= 0 )
      return 0;

    if ( !outline->contours || !outline->points )
      return ErrRaster_Invalid_Outline;

    if ( outline->n_points !=
           outline->contours[outline->n_contours - 1] + 1 )
      return ErrRaster_Invalid_Outline;

    worker = raster.worker;

    /* compute clipping box */
    ras.clip_box = params->clip_box;

    /* initialize the cells table */
    ras.buffer      = raster.buffer;
    ras.buffer_size = raster.buffer_size;

    ras.ycells      = (PCell*) raster.buffer;
    ras.cells       = NULL;
    ras.max_cells   = 0;
    ras.num_cells   = 0;
    ras.area        = 0;
    ras.cover       = 0;
    ras.invalid     = 1;

    ras.outline   = *outline;
    ras.band_size = raster.band_size;

    ras.render_span      = params->gray_spans;
    ras.render_span_data = params->user;

    return gray_convert_glyph( worker );
  }

/* END */
