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
    FILE:         TexDX9.cpp
    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Direct3D Texture Cache
*/

#include "MemDebug.h"
#include "TexCubeDX9.h"
#include "VideoDX9.h"
#include "Bitmap.h"
#include "Color.h"

// +--------------------------------------------------------------------+

void  Print(const char* fmt, ...);
void  VideoDX9Error(const char* msg, HRESULT err);

#ifndef RELEASE
#define RELEASE(x) if (x) { x->Release(); x=NULL; }
#endif

// +--------------------------------------------------------------------+

TexCubeDX9::TexCubeDX9(VideoDX9* v)
: video(v), texture(0)
{
    d3d       = video->Direct3D();
    d3ddevice = video->D3DDevice();

    for (int i = 0; i < 6; i++) {
        faces[i] = 0;
        last_modified[i] = 0;
    }
}

TexCubeDX9::~TexCubeDX9()
{
    RELEASE(texture);
}

// +--------------------------------------------------------------------+

bool
TexCubeDX9::LoadTexture(Bitmap* bmp, int face_index)
{
    if (!d3ddevice) return false;

    if (faces[face_index] == bmp && last_modified[face_index] >= bmp->LastModified())
    return true; // already loaded and hasn't been modified

    HRESULT hr = D3D_OK;

    // create the texture, if necessary
    if (!texture) {
        hr = d3ddevice->CreateCubeTexture(bmp->Width(),
        1,                // one mip level
        0,                // no specific usage
        D3DFMT_A8R8G8B8,  // format matching Color::rgba
        D3DPOOL_MANAGED,
        &texture,
        0);

        if (FAILED(hr) || !texture) {
            VideoDX9Error("LoadTexture - could not create cube texture", hr);
            return false;
        }
    }

    // lock the surface for writing
    D3DLOCKED_RECT    locked_rect;
    D3DCUBEMAP_FACES  face = (D3DCUBEMAP_FACES) face_index;
    hr = texture->LockRect(face, 0, &locked_rect, 0, 0);

    if (FAILED(hr)) {
        VideoDX9Error("LoadTexture - could not lock texture surface", hr);
        RELEASE(texture);
        return false;
    }

    // load the bitmap into the texture surface
    for (int i = 0; i < bmp->Height(); i++) {
        BYTE* src = (BYTE*) (bmp->HiPixels()  + i * bmp->Width());
        BYTE* dst = (BYTE*) locked_rect.pBits + i * locked_rect.Pitch;

        CopyMemory(dst, src, bmp->Width() * sizeof(Color));
    }

    // unlock the surface
    texture->UnlockRect(face, 0);

    faces[face_index]         = bmp;
    last_modified[face_index] = bmp->LastModified();

    return true;
}

// +--------------------------------------------------------------------+

IDirect3DCubeTexture9*
TexCubeDX9::GetTexture()
{
    if (texture) {
        // need to refresh anything?
        for (int i = 0; i < 6; i++) {
            if (faces[i] && last_modified[i] < faces[i]->LastModified()) {
                LoadTexture(faces[i], i);
            }
        }
    }

    return texture;
}
