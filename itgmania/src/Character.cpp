#include "global.h"

#include "Character.h"

#include "ActorUtil.h"
#include "IniFile.h"
#include "RageTextureID.h"
#include "RageUtil.h"
=======
#include "Character.h"
>>>>>>> upstream/release

  static int GetIconPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetIconPath());
    return 1;
  }

  static int GetSongSelectIconPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetSongSelectIconPath());
    return 1;
  }

  static int GetStageIconPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetStageIconPath());
    return 1;
  }

  static int GetModelPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetModelPath());
    return 1;
  }

  static int GetRestAnimationPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetRestAnimationPath());
    return 1;
  }

  static int GetWarmUpAnimationPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetWarmUpAnimationPath());
    return 1;
  }

  static int GetDanceAnimationPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetDanceAnimationPath());
    return 1;
  }

  static int GetCharacterDir(T* p, lua_State* L) {
    lua_pushstring(L, p->character_dir_);
    return 1;
  }

  static int GetCharacterID(T* p, lua_State* L) {
    lua_pushstring(L, p->character_id_);
    return 1;
  }

  static int GetDisplayName(T* p, lua_State* L) {
    lua_pushstring(L, p->GetDisplayName());
    return 1;
  }

=======
class LunaCharacter : public Luna<Character> {
 public:
  static int GetCardPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetCardPath().c_str());
    return 1;
  }
  static int GetIconPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetIconPath().c_str());
    return 1;
  }
  static int GetSongSelectIconPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetSongSelectIconPath().c_str());
    return 1;
  }
  static int GetStageIconPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetStageIconPath().c_str());
    return 1;
  }
  static int GetModelPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetModelPath().c_str());
    return 1;
  }
  static int GetRestAnimationPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetRestAnimationPath().c_str());
    return 1;
  }
  static int GetWarmUpAnimationPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetWarmUpAnimationPath().c_str());
    return 1;
  }
  static int GetDanceAnimationPath(T* p, lua_State* L) {
    lua_pushstring(L, p->GetDanceAnimationPath().c_str());
    return 1;
  }
  static int GetCharacterDir(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sCharDir.c_str());
    return 1;
  }
  static int GetCharacterID(T* p, lua_State* L) {
    lua_pushstring(L, p->m_sCharacterID.c_str());
    return 1;
  }
  static int GetDisplayName(T* p, lua_State* L) {
    lua_pushstring(L, p->GetDisplayName().c_str());
    return 1;
  }

>>>>>>> upstream/release
  LunaCharacter() {
    ADD_METHOD(GetCardPath);
    ADD_METHOD(GetIconPath);
    ADD_METHOD(GetSongSelectIconPath);
    ADD_METHOD(GetStageIconPath);
    // sm-ssc adds:
    ADD_METHOD(GetModelPath);
    ADD_METHOD(GetRestAnimationPath);
    ADD_METHOD(GetWarmUpAnimationPath);
    ADD_METHOD(GetDanceAnimationPath);
    ADD_METHOD(GetCharacterDir);
    ADD_METHOD(GetCharacterID);
    ADD_METHOD(GetDisplayName);
  }
};

LUA_REGISTER_CLASS(Character)
// lua end

/*
 * (c) 2003 Chris Danford
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
