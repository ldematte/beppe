/********************************************************************************
*                                                                               *
*                      O p e n G L   V i e w e r   W i d g e t                  *
*                                                                               *
*********************************************************************************
* Copyright (C) 1997,2002 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or                 *
* modify it under the terms of the GNU Lesser General Public                    *
* License as published by the Free Software Foundation; either                  *
* version 2.1 of the License, or (at your option) any later version.            *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
* Lesser General Public License for more details.                               *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public              *
* License along with this library; if not, write to the Free Software           *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.    *
*********************************************************************************
* $Id: FXGLViewer.h,v 1.50 2002/01/18 22:42:53 jeroen Exp $                     *
********************************************************************************/
#ifndef FXGLVIEWER_H
#define FXGLVIEWER_H

#ifndef FXGLCANVAS_H
#include "FXGLCanvas.h"
#endif



class FXDCPrint;
class FXGLObject;


// Pick tolerance
#define PICK_TOL  3


// GL Viewer options
enum {
  VIEWER_LOCKED      = 0x00008000,    /// Mouse based view manipulation is locked
  VIEWER_LIGHTING    = 0x00010000,    /// Lighting is on
  VIEWER_FOG         = 0x00020000,    /// Fog mode on
  VIEWER_DITHER      = 0x00040000     /// Dithering
  };


/*******************************  Viewer  Structs  *****************************/


/// OpenGL Viewer Viewport
struct FXViewport {
  FXint      w,h;               // Viewport dimensions
  FXdouble   left,right;        // World box
  FXdouble   bottom,top;
  FXdouble   hither,yon;
  };


// OpenGL Light Source
struct FXAPI FXLight {
  FXHVec     ambient;           // Ambient light color
  FXHVec     diffuse;           // Diffuse light color
  FXHVec     specular;          // Specular light color
  FXHVec     position;          // Light position
  FXVec      direction;         // Spot direction
  FXfloat    exponent;          // Spotlight exponent
  FXfloat    cutoff;            // Spotlight cutoff angle
  FXfloat    c_attn;            // Constant attenuation factor
  FXfloat    l_attn;            // Linear attenuation factor
  FXfloat    q_attn;            // Quadratic attenuation factor
  };


// OpenGL Material Description
struct FXAPI FXMaterial {
  FXHVec     ambient;           // Ambient material color
  FXHVec     diffuse;           // Diffuse material color
  FXHVec     specular;          // Specular material color
  FXHVec     emission;          // Emissive material color
  FXfloat    shininess;         // Specular shininess
  };


// Feedback buffer sort routine
typedef FXbool (*FXZSortFunc)(FXfloat*& buffer,FXint& used,FXint& size);


/********************************  Viewer  Class  ******************************/


/// Canvas, an area drawn by another object
class FXAPI FXGLViewer : public FXGLCanvas {
  FXDECLARE(FXGLViewer)
  friend class FXGLObject;
protected:
  FXViewport      wvt;              // Window viewport transform
  FXHMat          transform;        // Current transformation matrix
  FXHMat          itransform;       // Inverse of current transformation matrix
  FXuint          projection;       // Projection mode
  FXQuat          rotation;         // Viewer orientation
  FXdouble        fov;              // Field of view
  FXdouble        zoom;             // Zoom factor
  FXdouble        offset;           // Offset for lines on surfaces
  FXVec           center;           // Model center
  FXVec           scale;            // Model scale
  FXdouble        worldpx;          // Pixel size in world
  FXdouble        modelpx;          // Pixel size in model
  FXuint          op;               // Operation being performed
  FXint           maxhits;          // Maximum number of hits
  FXdouble        ax,ay;            // Quick view->world coordinate mapping
  FXdouble        diameter;         // Size of model diameter ( always > 0)
  FXdouble        distance;         // Distance of PRP to target
  FXHVec          background;       // Background color
  FXHVec          ambient;          // Global ambient light
  FXLight         light;            // Light source
  FXMaterial      material;         // Base material properties
  FXint           dial[3];          // Dial positions
  FXTimer        *timer;            // Motion timer
  FXString        help;             // Status help
  FXString        tip;              // Tooltip for background
  FXbool          doesturbo;        // Doing turbo mode
  FXbool          turbomode;        // Turbo mode
  FXGLObject     *dropped;          // Object being dropped on
  FXGLObject     *selection;        // Current object
  FXZSortFunc     zsortfunc;        // Routine to sort feedback buffer
  FXGLObject     *scene;            // What we're looking at
public:

  // Common DND types
  static FXDragType objectType;     // GL Object type

protected:

  // Mouse actions when in viewing window
  enum {
    HOVERING,                       // Hovering mouse w/o doing anything
    PICKING,                        // Pick mode
    ROTATING,                       // Rotating camera around target
    POSTING,                        // Posting right-mouse menu
    TRANSLATING,                    // Translating camera
    ZOOMING,                        // Zooming
    FOVING,                         // Change field-of-view
    DRAGGING,                       // Dragging objects
    TRUCKING,                       // Trucking camera
    GYRATING,                       // Rotation of camera around eye
    DO_LASSOSELECT,                 // Lasso select when mouse pressed
    LASSOSELECT,                    // Anchor of lasso rectangle
    DO_LASSOZOOM,                   // Zoom when mouse pressed
    LASSOZOOM                       // Zoom rectangle
    };

protected:
  FXGLViewer();
  void glsetup();
  void updateProjection();
  void updateTransform();
  FXVec spherePoint(FXint px,FXint py);
  FXQuat turn(FXint fx,FXint fy,FXint tx,FXint ty);
  void drawWorld(FXViewport& wv);
  void drawAnti(FXViewport& wv);
  void drawLasso(FXint x0,FXint y0,FXint x1,FXint y1);
  FXint selectHits(FXuint*& hits,FXint& nhits,FXint x,FXint y,FXint w,FXint h);
  FXint renderFeedback(FXfloat *buffer,FXint x,FXint y,FXint w,FXint h,FXint maxbuffer);
  void drawFeedback(FXDCPrint& pdc,const FXfloat* buffer,FXint used);
  virtual FXGLObject* processHits(FXuint *pickbuffer,FXint nhits);
  void setOp(FXuint o);
  virtual void layout();
private:
  FXGLViewer(const FXGLViewer&);
  FXGLViewer &operator=(const FXGLViewer&);
  void initialize();
public:

  // Events
  long onPaint(FXObject*,FXSelector,void*);
  long onEnter(FXObject*,FXSelector,void*);
  long onLeave(FXObject*,FXSelector,void*);
  long onMotion(FXObject*,FXSelector,void*);
  long onMouseWheel(FXObject*,FXSelector,void*);
  long onChanged(FXObject*,FXSelector,void*);
  long onPick(FXObject*,FXSelector,void*);
  long onClicked(FXObject*,FXSelector,void*);
  long onDoubleClicked(FXObject*,FXSelector,void*);
  long onTripleClicked(FXObject*,FXSelector,void*);
  long onLassoed(FXObject*,FXSelector,void*);
  long onSelected(FXObject*,FXSelector,void*);
  long onDeselected(FXObject*,FXSelector,void*);
  long onInserted(FXObject*,FXSelector,void*);
  long onDeleted(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onMiddleBtnPress(FXObject*,FXSelector,void*);
  long onMiddleBtnRelease(FXObject*,FXSelector,void*);
  long onRightBtnPress(FXObject*,FXSelector,void*);
  long onRightBtnRelease(FXObject*,FXSelector,void*);
  long onUngrabbed(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);
  long onClipboardLost(FXObject*,FXSelector,void*);
  long onClipboardGained(FXObject*,FXSelector,void*);
  long onClipboardRequest(FXObject*,FXSelector,void*);

  // Commands
  long onCmdPerspective(FXObject*,FXSelector,void*);
  long onUpdPerspective(FXObject*,FXSelector,void*);
  long onCmdParallel(FXObject*,FXSelector,void*);
  long onUpdParallel(FXObject*,FXSelector,void*);
  long onCmdFront(FXObject*,FXSelector,void*);
  long onUpdFront(FXObject*,FXSelector,void*);
  long onCmdBack(FXObject*,FXSelector,void*);
  long onUpdBack(FXObject*,FXSelector,void*);
  long onCmdLeft(FXObject*,FXSelector,void*);
  long onUpdLeft(FXObject*,FXSelector,void*);
  long onCmdRight(FXObject*,FXSelector,void*);
  long onUpdRight(FXObject*,FXSelector,void*);
  long onCmdTop(FXObject*,FXSelector,void*);
  long onUpdTop(FXObject*,FXSelector,void*);
  long onCmdBottom(FXObject*,FXSelector,void*);
  long onUpdBottom(FXObject*,FXSelector,void*);
  long onCmdResetView(FXObject*,FXSelector,void*);
  long onCmdFitView(FXObject*,FXSelector,void*);
  long onDNDEnter(FXObject*,FXSelector,void*);
  long onDNDLeave(FXObject*,FXSelector,void*);
  long onDNDMotion(FXObject*,FXSelector,void*);
  long onDNDDrop(FXObject*,FXSelector,void*);
  long onTipTimer(FXObject*,FXSelector,void*);
  long onQueryHelp(FXObject*,FXSelector,void*);
  long onQueryTip(FXObject*,FXSelector,void*);
  long onCmdXYZDial(FXObject*,FXSelector,void*);
  long onUpdXYZDial(FXObject*,FXSelector,void*);
  long onCmdRollPitchYaw(FXObject*,FXSelector,void*);
  long onUpdRollPitchYaw(FXObject*,FXSelector,void*);
  long onCmdXYZScale(FXObject*,FXSelector,void*);
  long onUpdXYZScale(FXObject*,FXSelector,void*);
  long onUpdCurrent(FXObject*,FXSelector,void*);
  long onCmdCutSel(FXObject*,FXSelector,void*);
  long onCmdCopySel(FXObject*,FXSelector,void*);
  long onCmdPasteSel(FXObject*,FXSelector,void*);
  long onCmdDeleteSel(FXObject*,FXSelector,void*);
  long onUpdDeleteSel(FXObject*,FXSelector,void*);
  long onCmdBackColor(FXObject*,FXSelector,void*);
  long onUpdBackColor(FXObject*,FXSelector,void*);
  long onCmdAmbientColor(FXObject*,FXSelector,void*);
  long onUpdAmbientColor(FXObject*,FXSelector,void*);
  long onCmdLock(FXObject*,FXSelector,void*);
  long onUpdLock(FXObject*,FXSelector,void*);
  long onCmdLighting(FXObject*,FXSelector,void*);
  long onUpdLighting(FXObject*,FXSelector,void*);
  long onCmdFog(FXObject*,FXSelector,void*);
  long onUpdFog(FXObject*,FXSelector,void*);
  long onCmdDither(FXObject*,FXSelector,void*);
  long onUpdDither(FXObject*,FXSelector,void*);
  long onCmdFov(FXObject*,FXSelector,void*);
  long onUpdFov(FXObject*,FXSelector,void*);
  long onCmdZoom(FXObject*,FXSelector,void*);
  long onUpdZoom(FXObject*,FXSelector,void*);
  long onCmdLightAmbient(FXObject*,FXSelector,void*);
  long onUpdLightAmbient(FXObject*,FXSelector,void*);
  long onCmdLightDiffuse(FXObject*,FXSelector,void*);
  long onUpdLightDiffuse(FXObject*,FXSelector,void*);
  long onCmdLightSpecular(FXObject*,FXSelector,void*);
  long onUpdLightSpecular(FXObject*,FXSelector,void*);
  long onCmdTurbo(FXObject*,FXSelector,void*);
  long onUpdTurbo(FXObject*,FXSelector,void*);
  long onCmdPrintImage(FXObject*,FXSelector,void*);
  long onCmdPrintVector(FXObject*,FXSelector,void*);
  long onCmdLassoZoom(FXObject*,FXSelector,void*);
  long onCmdLassoSelect(FXObject*,FXSelector,void*);
  virtual long onDefault(FXObject*,FXSelector,void*);

public:

  // Projection modes
  enum {
    PARALLEL,		  // Parallel projection
    PERSPECTIVE		  // Perspective projection
    };

  // Precompiled display lists [DEPRECATED]
  enum {
    OFFSETPROJECTION  = 1,
    SURFACEPROJECTION = 2
    };

  // Messages
  enum {
    ID_PERSPECTIVE=FXGLCanvas::ID_LAST,
    ID_PARALLEL,
    ID_FRONT,
    ID_BACK,
    ID_LEFT,
    ID_RIGHT,
    ID_TOP,
    ID_BOTTOM,
    ID_RESETVIEW,
    ID_FITVIEW,
    ID_TIPTIMER,
    ID_BACK_COLOR,
    ID_AMBIENT_COLOR,
    ID_LIGHT_AMBIENT,
    ID_LIGHT_DIFFUSE,
    ID_LIGHT_SPECULAR,
    ID_LIGHTING,
    ID_TURBO,
    ID_FOG,
    ID_DITHER,
    ID_SCALE_X,
    ID_SCALE_Y,
    ID_SCALE_Z,
    ID_DIAL_X,
    ID_DIAL_Y,
    ID_DIAL_Z,
    ID_ROLL,
    ID_PITCH,
    ID_YAW,
    ID_FOV,
    ID_ZOOM,
    ID_LOCK,
    ID_CUT_SEL,
    ID_COPY_SEL,
    ID_PASTE_SEL,
    ID_DELETE_SEL,
    ID_PRINT_IMAGE,
    ID_PRINT_VECTOR,
    ID_LASSO_ZOOM,
    ID_LASSO_SELECT,
    ID_LAST
    };

public:

  // Common DND type names
  static const FXchar objectTypeName[];

public:

  /// Construct GL viewer widget
  FXGLViewer(FXComposite* p,FXGLVisual *vis,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Construct GL viewer widget sharing display list with another GL viewer
  FXGLViewer(FXComposite* p,FXGLVisual *vis,FXGLViewer* sharegroup,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  /// Create all of the server-side resources for this window
  virtual void create();

  /// Detach server-side resources
  virtual void detach();

  /// Return size of pixel in world coordinates
  FXdouble worldPix() const { return worldpx; }

  /// Return size of pixel in model coordinates
  FXdouble modelPix() const { return modelpx; }

  /// Return a NULL-terminated list of all objects in the given rectangle, or NULL
  FXGLObject** lasso(FXint x1,FXint y1,FXint x2,FXint y2);

  /// Return a NULL-terminated list of all objects in the given rectangle, or NULL
  virtual FXGLObject** select(FXint x,FXint y,FXint w,FXint h);

  /// Perform a pick operation, returning the object at the given x,y position, or NULL
  virtual FXGLObject* pick(FXint x,FXint y);

  /// Change the model bounding box; this adjusts the viewer
  FXbool setBounds(const FXRange& box);

  /// Fit viewer to the given bounding box
  FXbool fitToBounds(const FXRange& box);

  /// Return the viewer's viewport
  void getViewport(FXViewport& v) const;

  /// Translate eye-coordinate to screen coordinate
  void eyeToScreen(FXint& sx,FXint& sy,FXVec e);

  /// Translate screen coordinate to eye coordinate at the given depth
  FXVec screenToEye(FXint sx,FXint sy,FXfloat eyez=0.0);

  /// Translate screen coordinate to eye coordinate at the target point depth
  FXVec screenToTarget(FXint sx,FXint sy);

  /// Translate world coordinate to eye coordinate
  FXVec worldToEye(FXVec w);

  /// Translate world coordinate to eye coordinate depth
  FXfloat worldToEyeZ(FXVec w);

  /// Translate eye coordinate to eye coordinate
  FXVec eyeToWorld(FXVec e);

  /// Calculate world coordinate vector from screen movement
  FXVec worldVector(FXint fx,FXint fy,FXint tx,FXint ty);

  ///  Change default object material setting
  void setMaterial(const FXMaterial &mtl);

  /// Return default object material setting
  void getMaterial(FXMaterial &mtl) const;

  /// Change camera field of view angle (in degrees)
  void setFieldOfView(FXdouble fv);

  /// Return camera field of view angle
  FXdouble getFieldOfView() const { return fov; }

  /// Change camera zoom factor
  void setZoom(FXdouble zm);

  /// Return camera zoom factor
  FXdouble getZoom() const { return zoom; }

  /// Change target point distance
  void setDistance(FXdouble ed);

  /// Return target point distance
  FXdouble getDistance() const { return distance; }

  /// Change unequal model scaling factors
  void setScale(FXVec s);

  /// Return current scaling factors
  FXVec getScale() const { return scale; }

  /// Change camera orientation from quaternion
  void setOrientation(FXQuat rot);

  /// Return current camera orientation quaternion
  FXQuat getOrientation() const { return rotation; }

  /// Change object center (tranlation)
  void setCenter(FXVec cntr);

  /// Return object center
  const FXVec& getCenter() const { return center; }

  /// Translate object center
  void translate(FXVec vec);

  /// Return boresight vector
  FXbool getBoreVector(FXint sx,FXint sy,FXVec& point,FXVec& dir);

  /// Return eyesight vector
  FXVec getEyeVector() const;

  /// Return eye position
  FXVec getEyePosition() const;

  /// Change help text
  void setHelpText(const FXString& text);

  /// Return help text
  FXString getHelpText() const { return help; }

  /// Change tip text
  void setTipText(const FXString&  text);

  /// Return tip text
  FXString getTipText() const { return tip; }

  /// Change line offset
  void setOffset(FXdouble offs);

  /// Return line offset
  FXdouble getOffset() const { return offset; }

  /// Return the current transformation matrix
  const FXHMat& getTransform() const { return transform; }

  /// Return the inverse of the current transformation matrix
  const FXHMat& getInvTransform() const { return itransform; }

  /// Change the scene, i.e. the object being displayed.
  void setScene(FXGLObject* sc);

  /// Return the current scene object
  FXGLObject* getScene() const { return scene; }

  /// Change selection
  void setSelection(FXGLObject* sel);

  /// Return selection
  FXGLObject* getSelection() const { return selection; }

  /// Change the projection mode, PERSPECTIVE or PARALLEL
  void setProjection(FXuint proj);

  /// Return the projection mode
  FXuint getProjection() const { return projection; }

  /// Lock the viewer, i.e. prevent mouse-based viewing operations
  void setViewLock(FXbool lock=TRUE);

  /// Return viewer lock status
  FXbool getViewLock() const;

  /// Change window background color
  void setBackgroundColor(const FXHVec& clr);

  /// Return window background color
  FXHVec getBackgroundColor() const { return background; }

  /// Change global ambient light color
  void setAmbientColor(const FXHVec& clr);

  /// Return global ambient light color
  FXHVec getAmbientColor() const { return ambient; }

  /// Read the pixels off the screen as R,G,B tuples.
  FXbool readPixels(FXuchar*& buffer,FXint x,FXint y,FXint w,FXint h);

  /// Read the feedback buffer containing the current scene, returning used and allocated size
  FXbool readFeedback(FXfloat*& buffer,FXint& used,FXint& size,FXint x,FXint y,FXint w,FXint h);

  /**
  * Change hidden-surface feedback buffer sorting algorithm.
  * This can be used for move/draw printed output depth sorting.
  */
  void setZSortFunc(FXZSortFunc func){ zsortfunc=func; }

  /// Return hidden surface sorting function.
  FXZSortFunc getZSortFunc() const { return zsortfunc; }

  /**
  * Change the maximum hits, i.e. the maximum size of the pick buffer.
  * When set to less than or equal to zero, picking is essentially turned off.
  */
  void setMaxHits(FXint maxh) { maxhits=maxh; }

  /// Return maximum pickbuffer size
  FXint getMaxHits() const { return maxhits; }

  /**
  * When drawing a GL object, if doesTurbo() is true, the object
  * may choose to perform a reduced complexity drawing as the user is
  * interactively manipulating; another update will be done later when
  * the full complexity drawing can be performed again.
  */
  FXbool doesTurbo() const { return doesturbo; }

  /// Return turbo mode setting
  FXbool getTurboMode() const { return turbomode; }

  /// Set turbo mode
  void setTurboMode(FXbool turbo=TRUE);

  /// Return light source settings
  void getLight(FXLight& lite) const;

  /// Change light source settings
  void setLight(const FXLight& lite);

  /// Save viewer to a stream
  virtual void save(FXStream& store) const;

  /// Load viewer from a stream
  virtual void load(FXStream& store);

  /// Destructor
  virtual ~FXGLViewer();
  };


#endif

