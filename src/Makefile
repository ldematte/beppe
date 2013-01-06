#
# Makefile : last updated 06/06
#

INCDIR = -I/usr/X11R6/include -I../include/fox -I../include/freetype -I../include/freetype -I../include/ftgl
LIBDIR = -L/usr/X11R6/lib/ -L../lib/linux 
NFLAGS = -O4 -ansi -felide-constructors
CC = g++
DFLAGS = -D_DEBUG -DDEBUG -g
FLAGS = $(NFLAGS) #$(DFLAGS)

#INCDIR = -I/usr/X11R6/include -I/tmp/dema/fox-1.0.7/include -I.
#LIBDIR = -L/usr/X11R6/lib/ -L/tmp/dema/fox-1.0.7/src/.libs/ 

#INCDIR = -I/usr/X11R6/include -I/tmp/dema/fox-1.0.11/include -I.
#LIBDIR = -L/usr/X11R6/lib/ -L/tmp/dema/fox-1.0.11/src/.libs/ 

all: LogWindow.o Logger.o MaterialMgr.o MaterialInfo.o TextureMgr.o TexResource.o TexCached.o \
Image.o Viewport.o Viewport2d.o Viewport3d.o FogMgr.o FogInfo.o ImageTGA.o Trackball.o \
PatchMgr.o Picker.o MatWindow.o Bezier.o EditorBezierPatch.o Mtxlib.o Utils.o Camera.o AboutWindow.o TexWindow.o RenderedBezier.o
	g++ $(INCDIR) $(LIBDIR) -lFOX -lGL -lGLU -lfreetype -lftgl Editor.cpp LogWindow.o Logger.o MaterialMgr.o MaterialInfo.o TextureMgr.o TexResource.o \
TexCached.o FogMgr.o FogInfo.o Viewport.o Viewport2d.o Viewport3d.o Image.o ImageTGA.o Trackball.o \
PatchMgr.o Picker.o MatWindow.o Bezier.o Camera.o EditorBezierPatch.o Mtxlib.o Utils.o AboutWindow.o TexWindow.o RenderedBezier.o -o ../Editor -lFOX -lftgl -lfreetype

LogWindow.o: LogWindow.cpp LogWindow.h
	$(CC) $(FLAGS) $(INCDIR) -c LogWindow.cpp
        
Logger.o: Logger.cpp Logger.h
	$(CC) $(FLAGS) $(INCDIR) -c Logger.cpp
        
MaterialMgr.o: MaterialMgr.cpp MaterialMgr.h
	$(CC) $(FLAGS) $(INCDIR) -c MaterialMgr.cpp
        
MaterialInfo.o: MaterialInfo.cpp MaterialInfo.h
	$(CC) $(FLAGS) $(INCDIR) -c MaterialInfo.cpp
        
TextureMgr.o: TextureMgr.cpp TextureMgr.h
	$(CC) $(FLAGS) $(INCDIR) -c TextureMgr.cpp
     
TexResource.o: TexResource.cpp TexResource.h
	$(CC) $(FLAGS) $(INCDIR) -c TexResource.cpp
       
TexCached.o: TexCached.cpp TexCached.h
	$(CC) $(FLAGS) $(INCDIR) -c TexCached.cpp
        
Image.o: Image.cpp Image.h
	$(CC) $(FLAGS) $(INCDIR) -c Image.cpp
        
ImageTGA.o: ImageTGA.cpp ImageTGA.h
	$(CC) $(FLAGS) $(INCDIR) -c ImageTGA.cpp

Viewport.o: Viewport.h Viewport.cpp
	$(CC) $(FLAGS) $(INCDIR) -c Viewport.cpp
        
Viewport2d.o: Viewport2d.h Viewport2d.cpp
	$(CC) $(FLAGS) $(INCDIR) -c Viewport2d.cpp
        
Viewport3d.o: Viewport2d.h Viewport3d.cpp Camera.h
	$(CC) $(FLAGS) $(INCDIR) -c Viewport3d.cpp

FogMgr.o: FogMgr.h FogMgr.cpp
	$(CC) $(FLAGS) $(INCDIR) -c FogMgr.cpp

FogInfo.o: FogInfo.h FogInfo.cpp
	$(CC) $(FLAGS) $(INCDIR) -c FogInfo.cpp

Trackball.o: Trackball.h Trackball.cpp
	$(CC) $(FLAGS) $(INCDIR) -c Trackball.cpp

PatchMgr.o: PatchMgr.h PatchMgr.cpp
	$(CC) $(FLAGS) $(INCDIR) -c PatchMgr.cpp

Picker.o: Picker.h Picker.cpp
	$(CC) $(FLAGS) $(INCDIR) -c Picker.cpp

MatWindow.o: MatWindow.h MatWindow.cpp
	$(CC) $(FLAGS) $(INCDIR) -c MatWindow.cpp

Bezier.o: Bezier.h Bezier.cpp
	$(CC) $(FLAGS) $(INCDIR) -c Bezier.cpp

EditorBezierPatch.o: EditorBezierPatch.h EditorBezierPatch.cpp
	$(CC) $(FLAGS) $(INCDIR) -c EditorBezierPatch.cpp

Mtxlib.o: Mtxlib.h Mtxlib.cpp
	$(CC) $(FLAGS) $(INCDIR) -c Mtxlib.cpp

Utils.o: Utils.h Utils.cpp
	$(CC) $(FLAGS) $(INCDIR) -c Utils.cpp

Camera.o: Camera.h Camera.cpp
	$(CC) $(FLAGS) $(INCDIR) -c Camera.cpp

AboutWindow.o: AboutWindow.h AboutWindow.cpp
	$(CC) $(FLAGS) $(INCDIR) -c AboutWindow.cpp

RenderedBezier.o: RenderedBezier.h RenderedBezier.cpp
	$(CC) $(FLAGS) $(INCDIR) -c RenderedBezier.cpp

TexWindow.o: TexWindow.cpp TexWindow.h
	$(CC) $(FLAGS) $(INCDIR) -c TexWindow.cpp


clean:
	rm -f *.o Editor core

