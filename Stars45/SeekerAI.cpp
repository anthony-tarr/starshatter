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

    SUBSYSTEM:    Stars.exe
    FILE:         SeekerAI.cpp
    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Seeker Missile (low-level) Artificial Intelligence class
*/

#include "MemDebug.h"
#include "SeekerAI.h"
#include "Ship.h"
#include "Shot.h"
#include "System.h"
#include "WeaponDesign.h"
#include "Random.h"

#include "Game.h"

// +----------------------------------------------------------------------+

SeekerAI::SeekerAI(SimObject* s)
    : SteerAI(s), shot((Shot*) s), orig_target(0),
      pursuit(1), delay(0), overshot(false)
{
    ai_type = SEEKER;

    seek_gain = 25;
    seek_damp = 0.55;
}


// +--------------------------------------------------------------------+

SeekerAI::~SeekerAI()
{
    if (shot) {
        if (shot->Owner())
        ((Ship*) shot->Owner())->SetMissileEta(shot->Identity(), 0);
    }
}

// +--------------------------------------------------------------------+

void
SeekerAI::ExecFrame(double seconds)
{
    // setup:
    FindObjective();

    // adaptive behavior:
    Navigator();
}

// +--------------------------------------------------------------------+

void
SeekerAI::Navigator()
{
    if (delay > 0) {
        delay -= Game::FrameTime();
    }
    else {
        Steer s = SeekTarget();
        self->ApplyYaw((float) s.yaw);
        self->ApplyPitch((float) s.pitch);
    }
}

void
SeekerAI::SetTarget(SimObject* targ, System* sub)
{
    if (!orig_target && targ && targ->Type() == SimObject::SIM_SHIP) {
        orig_target = (Ship*) targ;
        Observe(orig_target);
    }

    SteerAI::SetTarget(targ, sub);

    if (!target) {
        shot->SetEta(0);

        if (shot->Owner())
        ((Ship*) shot->Owner())->SetMissileEta(shot->Identity(), 0);
    }
}

void
SeekerAI::FindObjective()
{
    if (!shot || !target) return;

    if (target->Life() == 0) {
        if (target != orig_target)
        SetTarget(orig_target,0);
        else
        SetTarget(0,0);

        return;
    }

    Point tloc = target->Location();
	tloc = Transform(tloc);

	// seeker head limit of 45 degrees:
	if (tloc.z < 0 || (tloc.z)*2 < fabs(tloc.x) || (tloc.z)*2 < fabs(tloc.y)) {      //double angle
		overshot = true;
		SetTarget(0,0);
		return;
    }

    // distance from self to target:
    distance = Point(target->Location() - self->Location()).length();

    // are we being spoofed?
    CheckDecoys(distance);

    Point cv = ClosingVelocity();

    // time to reach target:
    double time    = distance / cv.length();
    double predict = time;
    if (predict > 15)
    predict = 15;

    // pure pursuit:
    if (pursuit == 1 || time < 0.1) {
        obj_w = target->Location();
    }

    // lead pursuit:
    else {
        // where the target will be when we reach it:
        Point run_vec = target->Velocity();
        obj_w   = target->Location() + (run_vec * predict);
    }

    // subsystem offset:
    if (subtarget) {
        Point offset = target->Location() - subtarget->MountLocation();
        obj_w -= offset;
    }
    else if (target->Type() == SimObject::SIM_SHIP) {
        Ship* tgt_ship = (Ship*) target;

        if (tgt_ship->IsGroundUnit())
        obj_w += Point(0,150,0);
    }


    distance = Point(obj_w - self->Location()).length();
    time     = distance / cv.length();

    // where we will be when the target gets there:
    if (predict > 0.1 && predict < 15) {
        Point self_dest = self->Location() + cv * predict;
        Point err = obj_w - self_dest;

        obj_w += err;
    }

    // transform into camera coords:
    objective = Transform(obj_w);
    objective.Normalize();

    shot->SetEta((int) time);

    if (shot->Owner())
    ((Ship*) shot->Owner())->SetMissileEta(shot->Identity(), (int) time);
}

// +--------------------------------------------------------------------+

void
SeekerAI::CheckDecoys(double target_distance)
{
    // if the assigned target has the burner lit,
    // ignore the decoys:
    if (orig_target && orig_target->Augmenter()) {
        SetTarget(orig_target);
        return;
    }

    if (target && 
            target == orig_target && 
            orig_target->GetActiveDecoys().size()) {

        ListIter<Shot> decoy = orig_target->GetActiveDecoys();

        while (++decoy) {
			double decoy_distance = Point(decoy->Location() - self->Location()).length();

			if (decoy_distance < target_distance) {

				int ran	= rand();
				if (ran > 26000) {
					SetTarget(decoy.value(), 0);
					return;
				}
            }
        }
    }
}

// +--------------------------------------------------------------------+

bool
SeekerAI::Overshot()
{
    return overshot;
}

// +--------------------------------------------------------------------+

Steer
SeekerAI::AvoidCollision()
{
    return Steer();
}

// +--------------------------------------------------------------------+

Steer
SeekerAI::SeekTarget()
{
    if (!self || !target || Overshot())
    return Steer();

    return Seek(objective);
}

// +--------------------------------------------------------------------+

bool
SeekerAI::Update(SimObject* obj)
{
    if (obj == target) {
        if (obj->Type() == SimObject::SIM_SHOT && orig_target != 0)
        target = orig_target;
    }

    if (obj == orig_target)
    orig_target = 0;

    return SteerAI::Update(obj);
}

const char*
SeekerAI::GetObserverName() const
{
    static char name[64];
    sprintf_s(name, "SeekerAI(%s)", self->Name());
    return name;
}

