#ifndef BITMAP_TEXT_H
#define BITMAP_TEXT_H

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "Actor.h"
#include "Font.h"
#include "RageTexture.h"

// An actor that holds a Font and draws text to the screen.
class BitmapText : public Actor {
 public:
  BitmapText();
  BitmapText(const BitmapText& cpy);
  BitmapText& operator=(const BitmapText& cpy);
  virtual ~BitmapText();

  virtual void LoadFromNode(const XNode* pNode) override;
  virtual BitmapText* Copy() const override;

  struct BMT_TweenState {
    // NOTE(Midiman): We'd be better off not adding strokes to things we can't
    // controlthemewise (ScreenDebugOverlay for example).
    BMT_TweenState() : m_stroke_color(RageColor(0, 0, 0, 0)) {}
    static void MakeWeightedAverage(
        BMT_TweenState& out, const BMT_TweenState& from,
        const BMT_TweenState& to, float between);
    bool operator==(const BMT_TweenState& other) const;
    bool operator!=(const BMT_TweenState& other) const {
      return !operator==(other);
    }
    void SetStrokeColor(const RageColor& c) { m_stroke_color = c; }
    const RageColor& GetStrokeColor() { return m_stroke_color; }

   private:
    RageColor m_stroke_color;
  };

  BMT_TweenState& BMT_DestTweenState() {
    if (BMT_Tweens.empty()) {
      return BMT_current;
    } else {
      return BMT_Tweens.back();
    }
  }
  const BMT_TweenState& BMT_DestTweenState() const {
    return const_cast<BitmapText*>(this)->BMT_DestTweenState();
  }


  Attribute GetDefaultAttribute() const;
  void AddAttribute(size_t iPos, const Attribute& attr);
  void ClearAttributes();

  // Commands
  virtual void PushSelf(lua_State* L) override;

 protected:
  Font* m_pFont;
  bool m_bUppercase;
  RString m_sText;
  std::vector<std::wstring> m_wTextLines;
  std::vector<int> m_iLineWidths;
  int m_iWrapWidthPixels;          // -1 = no wrap
  float m_fMaxWidth;               // 0 = no max
  float m_fMaxHeight;              // 0 = no max
  bool m_MaxDimensionUsesZoom;
  bool m_bRainbowScroll;
  bool m_bJitter;
  bool m_bUsingDistortion;
  bool m_mult_attrs_with_diffuse;
  float m_fDistortion;
  int m_iVertSpacing;


  std::vector<FontPageTextures*> m_vpFontPageTextures;
  std::map<size_t, Attribute> m_mAttributes;
  bool m_bHasGlowAttribute;

  TextGlowMode m_TextGlowMode;

  // Recalculate the items in SetText()
  void BuildChars();
  void DrawChars(bool bUseStrokeTexture);
  void UpdateBaseZoom();

 private:
  void SetTextInternal();
  std::vector<BMT_TweenState> BMT_Tweens;
  BMT_TweenState BMT_current;
  BMT_TweenState BMT_start;
};

#endif  // BITMAP_TEXT_H

/**
 * @file
 * @author Chris Danford, Charles Lohr, Steve Checkoway (c) 2001-2007
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
