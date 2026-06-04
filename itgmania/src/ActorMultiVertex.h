#include "Actor.h"
#include "CubicSpline.h"
#include "RageDisplay.h"
#include "RageMath.h"
#include "RageTexture.h"
#include "RageTextureID.h"
#include "XmlFile.h"

#include <cstddef>
#include <vector>

enum DrawMode {
  DrawMode_Quads = 0,
  DrawMode_QuadStrip,
  DrawMode_Fan,
  DrawMode_Strip,
  DrawMode_Triangles,
  DrawMode_LineStrip,
  DrawMode_SymmetricQuadStrip,
  NUM_DrawMode,
  DrawMode_Invalid
};

const RString& DrawModeToString(DrawMode cat);
const RString& DrawModeToLocalizedString(DrawMode cat);
LuaDeclareType(DrawMode);

// An actor with mutiple vertices. Can be used to create shapes that quads can't.
class ActorMultiVertex : public Actor {
 public:
  static const size_t num_vert_splines = 4;
  ActorMultiVertex();
  ActorMultiVertex(const ActorMultiVertex& cpy);
  virtual ~ActorMultiVertex();

=======
>>>>>>> Stashed changes
};

/**
 * @file
 * @author Matthew Gardner and Eric Reese (c) 2014
 * @section LICENSE
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
