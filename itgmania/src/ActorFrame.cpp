#include "global.h"

#include "ActorFrame.h"

#include "ActorUtil.h"
#include "arch/Dialog/Dialog.h"
#include "LuaBinding.h"
#include "RageDisplay.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "ScreenDimensions.h"
#include "XmlFile.h"

#include <cstdint>
#include <vector>

// Tricky: We need ActorFrames created in Lua to auto delete their children,
// but we don't want classes that derive from ActorFrame to auto delete their 
// children. The name "ActorFrame" is widely used in Lua, so we'll have
// that string instead create an ActorFrameAutoDeleteChildren object.
REGISTER_ACTOR_CLASS_WITH_NAME( ActorFrameAutoDeleteChildren, ActorFrame);
ActorFrame *ActorFrame::Copy() const { return new ActorFrame(*this); }

ActorFrame::ActorFrame() {
  m_bPropagateCommands = false;
  m_bDeleteChildren = false;
  m_bDrawByZPosition = false;
  m_DrawFunction.SetFromNil();
  m_UpdateFunction.SetFromNil();
  m_fUpdateRate = 1;
  m_fFOV = -1;
  m_fVanishX = SCREEN_CENTER_X;
  m_fVanishY = SCREEN_CENTER_Y;
  m_bOverrideLighting = false;
  m_bLighting = false;
  m_ambientColor = RageColor(1, 1, 1, 1);
  m_diffuseColor = RageColor(1, 1, 1, 1);
  m_specularColor = RageColor(1, 1, 1, 1);
  m_lightDirection = RageVector3(0, 0, 1);
}

ActorFrame::~ActorFrame() {
  if (m_bDeleteChildren) {
    DeleteAllChildren();
  }
}

ActorFrame::ActorFrame(const ActorFrame& cpy) : Actor(cpy) {
#define CPY(x) this->x = cpy.x;
  CPY(m_bPropagateCommands);
  CPY(m_bDeleteChildren);
  CPY(m_bDrawByZPosition);
  CPY(m_DrawFunction);
  CPY(m_UpdateFunction);
  CPY(m_fUpdateRate);
  CPY(m_fFOV);
  CPY(m_fVanishX);
  CPY(m_fVanishY);
  CPY(m_bOverrideLighting);
  CPY(m_bLighting);
  CPY(m_ambientColor);
  CPY(m_diffuseColor);
  CPY(m_specularColor);
  CPY(m_lightDirection);
#undef CPY

  // If m_bDeleteChildren, we own our children and it's up to us to copy them.
  // If not, the derived class owns the children. This must preserve the current
  // order of m_SubActors.
  if (m_bDeleteChildren) {
    for(unsigned i = 0; i < cpy.m_SubActors.size(); ++i) {
      Actor* pActor = cpy.m_SubActors[i]->Copy();
      this->AddChild(pActor);
    }
  }
}

void ActorFrame::InitState() {
  std::for_each(
      m_SubActors.begin(), m_SubActors.end(), [](Actor* a) { a->InitState(); });
  Actor::InitState();
}

void ActorFrame::LoadFromNode(const XNode* pNode) {
  if (AutoLoadChildren()) {
    LoadChildrenFromNode(pNode);
  }

  Actor::LoadFromNode(pNode);

}

void ActorFrame::LoadChildrenFromNode(const XNode* pNode) {
  // Shouldn't be calling this unless we're going to delete our children.
  ASSERT(m_bDeleteChildren);

  // Load children
  const XNode* pChildren = pNode->GetChild("children");
  bool bArrayOnly = false;
  if (pChildren == nullptr) {
    bArrayOnly = true;
    pChildren = pNode;
  }

  FOREACH_CONST_Child(pChildren, pChild) {
    if (bArrayOnly && !IsAnInt(pChild->GetName())) {
      continue;
    }

    Actor* pChildActor = ActorUtil::LoadFromNode(pChild, this);
    if (pChildActor) {
      AddChild(pChildActor);
    }
  }
  SortByDrawOrder();
}

void ActorFrame::AddChild(Actor *pActor) {
#ifdef DEBUG
  // check that this Actor isn't already added.
  std::vector<Actor*>::iterator iter =
      std::find(m_SubActors.begin(), m_SubActors.end(), pActor);
  if (iter != m_SubActors.end()) {
    Dialog::OK(ssprintf(
        "Actor \"%s\" adds child \"%s\" more than once",
        GetLineage().c_str(), pActor->GetName().c_str()));
  }
#endif

  ASSERT(pActor != nullptr);
  ASSERT(reinterpret_cast<uintptr_t>(pActor) !=
         static_cast<uintptr_t>(0xC0000005));
  m_SubActors.push_back(pActor);
=======
void ActorFrame::AddChild(Actor* pActor) {
#ifdef DEBUG
  // check that this Actor isn't already added.
  std::vector<Actor*>::iterator iter =
      find(m_SubActors.begin(), m_SubActors.end(), pActor);
  if (iter != m_SubActors.end()) {
    Dialog::OK(ssprintf(
        "Actor \"%s\" adds child \"%s\" more than once", GetLineage().c_str(),
        pActor->GetName().c_str()));
  }
#endif

  ASSERT(pActor != nullptr);
  ASSERT(
      reinterpret_cast<uintptr_t>(pActor) !=
      static_cast<uintptr_t>(0xC0000005));
  m_SubActors.push_back(pActor);
>>>>>>> upstream/release
}

void ActorFrame::RemoveAllChildren() {
  m_SubActors.clear();
}

void ActorFrame::MoveToTail(Actor* pActor) {
  auto iter = std::find(m_SubActors.begin(), m_SubActors.end(), pActor);
  if (iter == m_SubActors.end()) {
=======
}

void ActorFrame::RemoveAllChildren() { m_SubActors.clear(); }

void ActorFrame::MoveToTail(Actor* pActor) {
  std::vector<Actor*>::iterator iter =
      find(m_SubActors.begin(), m_SubActors.end(), pActor);
  if (iter == m_SubActors.end()) {  // didn't find
    FAIL_M("Nonexistent actor");
  }

  m_SubActors.erase(iter);
  m_SubActors.push_back(pActor);
}

void ActorFrame::MoveToHead(Actor* pActor) {
  std::vector<Actor*>::iterator iter =
      find(m_SubActors.begin(), m_SubActors.end(), pActor);
  if (iter == m_SubActors.end()) {  // didn't find
>>>>>>> upstream/release
    FAIL_M("Nonexistent actor");
  }

  m_SubActors.erase(iter);
  m_SubActors.push_back(pActor);
}

void ActorFrame::MoveToHead(Actor* pActor) {
  auto iter = std::find(m_SubActors.begin(), m_SubActors.end(), pActor);
  if (iter == m_SubActors.end()) {
    FAIL_M("Nonexistent actor");
  }

  m_SubActors.erase(iter);
  m_SubActors.insert(m_SubActors.begin(), pActor);
}

void ActorFrame::BeginDraw() {
  Actor::BeginDraw();
  if (m_fFOV != -1) {
    DISPLAY->CameraPushMatrix();
    DISPLAY->LoadMenuPerspective(
        m_fFOV, SCREEN_WIDTH, SCREEN_HEIGHT, m_fVanishX, m_fVanishY);
  }

  if (m_bOverrideLighting) {
    DISPLAY->SetLighting(m_bLighting);
    if(m_bLighting)
      DISPLAY->SetLightDirectional(
          0,m_ambientColor, m_diffuseColor, m_specularColor, m_lightDirection);
  }
}

void ActorFrame::EndDraw() {
  if (m_bOverrideLighting) {
    // TODO: pop state instead of turning lighting off
    DISPLAY->SetLightOff(0);
    DISPLAY->SetLighting(false);
  }

void ActorFrame::DrawPrimitives() {
  if (m_bClearZBuffer) {
    LuaHelpers::ReportScriptErrorFmt(
        "ClearZBuffer not supported on ActorFrames");
    m_bClearZBuffer = false;
  }

  // Don't set Actor-defined render states because we won't be drawing
  // any geometry that belongs to this object.
  // Actor::DrawPrimitives();

  if (unlikely(!m_DrawFunction.IsNil())) {
    Lua* L = LUA->Get();
    m_DrawFunction.PushSelf(L);
    if (lua_isnil(L, -1)) {
      LUA->Release(L);
      LuaHelpers::ReportScriptErrorFmt("Error compiling DrawFunction");
      return;
    }
    this->PushSelf(L);
    RString Error= "Error running DrawFunction: ";
    LuaHelpers::RunScriptOnStack(L, Error, 1, 0, true); // 1 arg, 0 results
    LUA->Release(L);
    return;
  }

  RageColor diffuse = m_pTempState->diffuse[0];
  RageColor glow = m_pTempState->glow;

  // NOTE(Kyz): Word of warning: Actor::Draw duplicates the structure of how an
  // Actor is drawn inside of an ActorFrame for its wrapping feature. So if
  // you're adding something new to ActorFrames that affects how Actors are
  // drawn, make sure to also apply it in Actor::Draw's handling of the
  // wrappers.

  // Draw all sub-ActorFrames while we're in the ActorFrame's local coordinate space
  if (m_bDrawByZPosition) {
    std::vector<Actor*> subs = m_SubActors;
    ActorUtil::SortByZPosition(subs);
    for (unsigned i = 0; i < subs.size(); ++i) {
      subs[i]->SetInternalDiffuse(diffuse);
      subs[i]->SetInternalGlow(glow);
      subs[i]->Draw();
    }
  } else {
    for (unsigned i = 0; i < m_SubActors.size(); ++i) {
      m_SubActors[i]->SetInternalDiffuse(diffuse);
      m_SubActors[i]->SetInternalGlow(glow);
      m_SubActors[i]->Draw();
    }
  }
}

void ActorFrame::EndDraw() {
  if (m_bOverrideLighting) {
    // TODO: Pop state instead of turning lighting off
    DISPLAY->SetLightOff(0);
    DISPLAY->SetLighting(false);
  }

  if (m_fFOV != -1) {
    DISPLAY->CameraPopMatrix();
  }
  Actor::EndDraw();
}

// This exists solely as a helper for children table.
// This applies the desired function to te first child in the table.
static int IdenticalChildrenSingleApplier(lua_State* L) {
  // First arg is the table of items, get the last object.
  // It is the one that would have been in the children table in the old
  // version.
  // The other args are meant for the function.
  // The upvalue for this function is the function the theme tried to call.
  lua_rawgeti(L, 1, lua_objlen(L, 1)); // stack: table, args, obj
  lua_insert(L, 2); // stack: table, obj, args
  lua_pushvalue(L, lua_upvalueindex(1)); // stack: table, obj, args, func
  lua_insert(L, 2); // stack: table, func, obj, args
  int args_count = lua_gettop(L) - 2;
  // Not using RunScriptOnStack because we're inside a lua call already and
  // we want an error to propagate up.
  lua_call(L, args_count, LUA_MULTRET); // stack: table, return_values
  return lua_gettop(L) - 1;
}

// This exists solely as a helper for children table.
// This is the __index function for the table of all children with the same name.
static int IdenticalChildrenIndexLayer(lua_State* L) {
  if (lua_isnumber(L, 2)) {
    lua_rawget(L, 1);
  } else {
    lua_pushvalue(L, 1);
    lua_pushvalue(L, 2);
    lua_pop(L, 2);
    // Get the last object in the table.
    // Its meta table contains the function the theme wanted to run.
    // The function is then pushed as an upvalue for ICSA as a closure.
    // The closure is then returned so that when the function call is performed, ICSA is actually called.
    lua_pushnumber(L, lua_objlen(L, 1)); // stack: 1
    lua_gettable(L, 1); // stack: object
    lua_getmetatable(L, -1); // stack: object, obj_meta
    lua_getfield(L, -1, "__index"); // stack: object, obj_meta, obj_index
    lua_pushvalue(L, 2); // stack: object, obj_meta, obj_index, func_name
    lua_gettable(L, -2); // stack: object, obj_meta, obj_index, obj_function
    lua_pushcclosure(L, IdenticalChildrenSingleApplier, 1); // stack: object, obj_meta, obj_index, closure
    lua_insert(L, -4); // stack: closure, object, obj_meta, obj_index
    lua_pop(L, 3); // stack: closure
  }
  return 1;
}

static void CreateChildTable(lua_State* L, Actor* a) {
  // Old PushChildrenTable assumed that all children had unique names, so only the last one of a name ended up in the table that was returned.
  // Create a table that will hold all the children that have this name and act as a pass through layer for function calls.
  // stack: old_entry
  lua_createtable(L, 0, 0); // stack: old_entry, table_entry
  lua_insert(L, -2); // stack: table_entry, old_entry
  lua_rawseti(L, -2, 1); // stack: table_entry
  a->PushSelf(L); // stack: table_entry, new_entry
  lua_rawseti(L, -2, 2); // stack: table_entry
  lua_createtable(L, 0, 1); // stack: table_entry, table_meta
  lua_pushcfunction(L, IdenticalChildrenIndexLayer); // stack: table_entry, table_meta, ICIL
  lua_setfield(L, -2, "__index"); // stack: table_entry, table_meta
  lua_setmetatable(L, -2); // stack: table_entry
}

static void AddToChildTable(lua_State* L, Actor* a) {
  // stack: table_entry
  int next_index= lua_objlen(L, -1) + 1;
  a->PushSelf(L); // stack: table_entry, actor
  lua_rawseti(L, -2, next_index); // stack: table_entry
}

void ActorFrame::PushChildrenTable(lua_State* L) {
  lua_newtable(L); // stack: all_actors
  for (Actor* a: m_SubActors) {
    LuaHelpers::Push(L, a->GetName()); // stack: all_actors, name
    lua_gettable(L, -2); // stack: all_actors, entry
    if (lua_isnil(L, -1)) {
      lua_pop(L, 1); // stack: all_actors
      LuaHelpers::Push(L, a->GetName()); // stack: all_actors, name
      a->PushSelf(L); // stack: all_actors, name, actor
      lua_rawset(L, -3); // stack: all_actors
    } else {
      // Fun fact: PushSelf pushes a table.
      if (lua_objlen(L, -1) > 0) {
        // stack: all_actors, table_entry
        AddToChildTable(L, a); // stack: all_actors, table_entry
        lua_pop(L, 1); // stack: all_actors
      } else {
        // stack: all_actors, old_entry
        CreateChildTable(L, a); // stack: all_actors, table_entry
        LuaHelpers::Push(L, a->GetName()); // stack: all_actors, table_entry, name
        lua_insert(L, -2); // stack: all_actors, name, table_entry
        lua_rawset(L, -3); // stack: all_actors
      }
    }
  }
}

=======
>>>>>>> Stashed changes
};

LUA_REGISTER_DERIVED_CLASS(ActorFrame, Actor)
// lua end

/*
 * (c) 2001-2004 Chris Danford
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
