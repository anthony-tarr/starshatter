/*  Starshatter OpenSource Distribution
    Copyright (c) 1997-2004, Destroyer Studios LLC.
    All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name "Destroyer Studios" nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

    SUBSYSTEM:    nGenEx.lib
    FILE:         CameraView.h
    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    3D Projection Camera View class
*/

#ifndef CameraView_h
#define CameraView_h

#include "Types.h"
#include "View.h"
#include "Camera.h"
#include "Projector.h"
#include "Video.h"
#include "List.h"

// +--------------------------------------------------------------------+

class Video;
class Scene;
class Bitmap;

class Graphic;

// +--------------------------------------------------------------------+

class CameraView : public View
{
public:
    static const char* TYPENAME() { return "CameraView"; }

    CameraView(Window* c, Camera* cam, Scene* s);
    virtual ~CameraView();

    // Operations:
    virtual void   Refresh();
    virtual void   OnWindowMove();
    virtual void   UseCamera(Camera* cam);
    virtual void   UseScene(Scene* scene);
    virtual void   LensFlareElements(Bitmap* halo, Bitmap* e1=0, Bitmap* e2=0, Bitmap* e3=0);
    virtual void   LensFlare(int on, double dim = 1);
    virtual void   SetDepthScale(float scale);

    // accessors:
    Camera*        GetCamera()                   const { return camera;      }
    Projector*     GetProjector()                      { return &projector;  }
    Scene*         GetScene()                    const { return scene;       }
    virtual void   SetFieldOfView(double fov);
    virtual double GetFieldOfView()              const;
    virtual void   SetProjectionType(DWORD pt);
    virtual DWORD  GetProjectionType()           const;

    Point          Pos() const { return camera->Pos(); }
    Point          vrt()       { return camera->vrt(); }
    Point          vup()       { return camera->vup(); }
    Point          vpn()       { return camera->vpn(); }
    const Matrix&  Orientation() const { return camera->Orientation(); }

    Point          SceneOffset() const { return camera_loc; }

    // projection and clipping geometry:
    virtual void   TranslateScene();
    virtual void   UnTranslateScene();
    virtual void   MarkVisibleObjects();
    virtual void   MarkVisibleLights(Graphic* g, DWORD flags);

    virtual void   RenderScene();
    virtual void   RenderSceneObjects(bool distant=false);
    virtual void   RenderForeground();
    virtual void   RenderBackground();
    virtual void   RenderSprites();
    virtual void   RenderLensFlare();
    virtual void   Render(Graphic* g, DWORD flags);

    virtual void   FindDepth(Graphic* g);
    virtual int    SetInfinite(int i);

protected:
    Camera*        camera;
    Scene*         scene;
    Video*         video;

    virtual void   WorldPlaneToView(Plane& plane);

    Point          camera_loc;
    Vec3           cvrt;
    Vec3           cvup;
    Vec3           cvpn;

    Projector      projector;
    int            infinite;
    int            width;
    int            height;
    DWORD          projection_type;

    // lens flare:
    int            lens_flare_enable;
    double         lens_flare_dim;
    Bitmap*        halo_bitmap;
    Bitmap*        elem_bitmap[3];

    List<Graphic>  graphics;
};

#endif CameraView_h

