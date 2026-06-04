#include "global.h"

#include "Font.h"
=======
#include "Font.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "FontCharAliases.h"
#include "FontCharmaps.h"
#include "FontManager.h"
#include "IniFile.h"
#include "RageLog.h"
#include "RageTextureManager.h"
#include "RageUtil.h"
#include "ThemeManager.h"
#include "arch/Dialog/Dialog.h"

FontPage::FontPage()
    : height_(0),
      line_spacing_(0),
      vert_shift_(0),
      draw_extra_pixels_left_(0),
      draw_extra_pixels_right_(0),
      font_page_textures_(),
      texture_path_(""),
      glyphs_(),
      char_to_glyph_no_() {}

void FontPage::Load(const FontPageSettings& cfg) {
  texture_path_ = cfg.texture_path_;

  // Load texture.
  RageTextureID id1(texture_path_);
  if (cfg.texture_hints_ != "default") {
    id1.AdditionalTextureHints = cfg.texture_hints_;
  }

  font_page_textures_.texture_main_ = TEXTUREMAN->LoadTexture(id1);
  if (font_page_textures_.texture_main_ == nullptr) {
    LuaHelpers::ReportScriptErrorFmt(
        "Failed to load main texture %s for font page.", texture_path_.c_str());
    font_page_textures_.texture_main_ =
        TEXTUREMAN->LoadTexture(TEXTUREMAN->GetDefaultTextureID());
  }

  RageTextureID id2 = id1;
  // "arial 20 16x16 [main].png" => "arial 20 16x16 [main-stroke].png"
  if (id2.filename.find("]") != std::string::npos) {
    id2.filename.Replace("]", "-stroke]");
    if (IsAFile(id2.filename)) {
      font_page_textures_.texture_stroke_ = TEXTUREMAN->LoadTexture(id2);
      if (font_page_textures_.texture_stroke_ == nullptr) {
        LuaHelpers::ReportScriptErrorFmt(
            "Failed to load stroke texture %s for font page.",
            id2.filename.c_str());
        font_page_textures_.texture_stroke_ = font_page_textures_.texture_main_;
      }
      // NOTE(kyz): If the source frame dimensions or number of frames don't
      // match, set the stroke layer to be the same as the main layer so that
      // the assumptions based on the frames are still safe.
      if (font_page_textures_.texture_main_->GetSourceFrameWidth() !=
          font_page_textures_.texture_stroke_->GetSourceFrameWidth()) {
        LuaHelpers::ReportScriptErrorFmt(
            "'%s' and '%s' must have the same frame widths",
            id1.filename.c_str(), id2.filename.c_str());
        font_page_textures_.texture_stroke_ = font_page_textures_.texture_main_;
      }
      if (font_page_textures_.texture_main_->GetNumFrames() !=
          font_page_textures_.texture_stroke_->GetNumFrames()) {
        LuaHelpers::ReportScriptErrorFmt(
            "'%s' and '%s' must have the same frame dimensions",
            id1.filename.c_str(), id2.filename.c_str());
        font_page_textures_.texture_stroke_ = font_page_textures_.texture_main_;
      }
    }
  }

  // Load character widths.
  std::vector<int> frame_widths;

  int default_width = font_page_textures_.texture_main_->GetSourceFrameWidth();
  if (cfg.default_width_ != -1) {
    default_width = cfg.default_width_;
  }

  // Assume each character is the width of the frame by default.
  for (int i = 0; i < font_page_textures_.texture_main_->GetNumFrames(); ++i) {
    std::map<int, int>::const_iterator it = cfg.glyph_widths_map_.find(i);
    if (it != cfg.glyph_widths_map_.end()) {
      frame_widths.push_back(it->second);
    } else {
      frame_widths.push_back(default_width);
    }
  }

  if (cfg.add_to_all_widths_) {
    for (int i = 0; i < font_page_textures_.texture_main_->GetNumFrames();
         i++) {
      frame_widths[i] += cfg.add_to_all_widths_;
    }
  }

  if (cfg.scale_all_widths_by_ != 1) {
    for (int i = 0; i < font_page_textures_.texture_main_->GetNumFrames();
         i++) {
      frame_widths[i] = std::lrint(frame_widths[i] * cfg.scale_all_widths_by_);
    }
  }

  char_to_glyph_no_ = cfg.char_to_glyph_no_;

  line_spacing_ = cfg.line_spacing_;
  if (line_spacing_ == -1) {
    line_spacing_ = font_page_textures_.texture_main_->GetSourceFrameHeight();
  }

  int baseline = 0;
  // If we don't have a top and/or baseline, assume we're centered in the
  // frame, and that line_spacing_ is the total height.
  baseline = cfg.baseline_;
  if (baseline == -1) {
    float center =
        font_page_textures_.texture_main_->GetSourceFrameHeight() / 2.0f;
    baseline = int(center + line_spacing_ / 2);
  }

  int top = cfg.top_;
  if (top == -1) {
    float center =
        font_page_textures_.texture_main_->GetSourceFrameHeight() / 2.0f;
    top = int(center - line_spacing_ / 2);
  }
  height_ = baseline - top;
  draw_extra_pixels_left_ = cfg.draw_extra_pixels_left_;
  draw_extra_pixels_right_ = cfg.draw_extra_pixels_right_;

  // Shift the character up so the top will be rendered at the baseline.
  vert_shift_ = (float)-baseline;

  SetTextureCoords(frame_widths, cfg.advance_extra_pixels_);
  SetExtraPixels(cfg.draw_extra_pixels_left_, cfg.draw_extra_pixels_right_);
}

void FontPage::SetTextureCoords(
    const std::vector<int>& widths, int advance_extra_pixels) {
  for (int i = 0; i < font_page_textures_.texture_main_->GetNumFrames(); ++i) {
    glyph g;

    g.page_ = this;

    // Make a copy of each texture rect, reducing each to the actual dimensions
    // of the character (most characters don't take a full block).
    g.texture_rect_ =
        *font_page_textures_.texture_main_->GetTextureCoordRect(i);

    // Set the width and height to the width and line spacing, respectively.
    g.width_ = float(widths[i]);
    g.height_ =
        float(font_page_textures_.texture_main_->GetSourceFrameHeight());

    g.horiz_advance_ = int(g.width_) + advance_extra_pixels;

    // Do the same thing with X. Do this by changing the actual rendered
    // texture_rect_, instead of shifting it, so we don't render more than we
    // need to.
    g.horiz_shift_ = 0;
    {
      int source_pixels_to_chop_off =
          font_page_textures_.texture_main_->GetSourceFrameWidth() - widths[i];
      if ((source_pixels_to_chop_off % 2) == 1) {
        // We don't want to chop off an odd number of pixels, since that'll
        // put our texture coordinates between texels and make things blurrier.
        // Note that, since we set m_iHadvance above, this merely expands what
        // we render; it doesn't advance the cursor further.  So, glyphs
        // that have an odd width should err to being a pixel offcenter left,
        // not right.
        --source_pixels_to_chop_off;
        ++g.width_;
      }

      const float fTexCoordsToChopOff =
          source_pixels_to_chop_off *
          font_page_textures_.texture_main_->GetSourceToTexCoordsRatioX();

      g.texture_rect_.left += fTexCoordsToChopOff / 2;
      g.texture_rect_.right -= fTexCoordsToChopOff / 2;
    }

    g.font_page_textures_ = font_page_textures_;

    glyphs_.push_back(g);
  }
}

void FontPage::SetExtraPixels(
    int draw_extra_pixels_left, int draw_extra_pixels_right) {
  // Most fonts don't take the stroke into account, so if it shows up, it'll
  // be cut off. I now understand why this code was here before. -freem
  ++draw_extra_pixels_right;
  ++draw_extra_pixels_left;

  if ((draw_extra_pixels_left % 2) == 1) {
    ++draw_extra_pixels_left;
  }

  // Adjust for draw_extra_pixels_left and draw_extra_pixels_right.
  for (unsigned i = 0; i < glyphs_.size(); ++i) {
    int frame_width = font_page_textures_.texture_main_->GetSourceFrameWidth();
    float char_width = glyphs_[i].width_;

    // Extra pixels to draw to the left and right. We don't have to worry about
    // alignment here; char_width is always even (by SetTextureCoords) and
    // frame_width are almost always even.
    float extra_left = std::min(
        float(draw_extra_pixels_left), (frame_width - char_width) / 2.0f);
    float extra_right = std::min(
        float(draw_extra_pixels_right), (frame_width - char_width) / 2.0f);

    // Move left and expand right.
    glyphs_[i].texture_rect_.left -=
        extra_left *
        font_page_textures_.texture_main_->GetSourceToTexCoordsRatioX();
    glyphs_[i].texture_rect_.right +=
        extra_right *
        font_page_textures_.texture_main_->GetSourceToTexCoordsRatioX();
    glyphs_[i].horiz_shift_ -= extra_left;
    glyphs_[i].width_ += extra_left + extra_right;
  }
}

FontPage::~FontPage() {
  if (font_page_textures_.texture_main_ != nullptr) {
    TEXTUREMAN->UnloadTexture(font_page_textures_.texture_main_);
    font_page_textures_.texture_main_ = nullptr;
  }
  if (font_page_textures_.texture_stroke_ != nullptr) {
    TEXTUREMAN->UnloadTexture(font_page_textures_.texture_stroke_);
    font_page_textures_.texture_stroke_ = nullptr;
  }
}

int Font::GetLineWidthInSourcePixels(const std::wstring& line) const {
  int line_width = 0;

  for (unsigned i = 0; i < line.size(); ++i) {
    line_width += GetGlyph(line[i]).horiz_advance_;
  }

  return line_width;
}

int Font::GetLineHeightInSourcePixels(const std::wstring& line) const {
  int line_height = 0;

  // The height of a line is the height of its tallest used font page.
  for (unsigned i = 0; i < line.size(); ++i) {
    line_height = std::max(line_height, GetGlyph(line[i]).page_->height_);
  }

  return line_height;
}

// width is a pointer so that we can return the used width through it.
std::size_t Font::GetGlyphsThatFit(const std::wstring& line, int* width) const {
  if (*width == 0) {
    *width = GetLineWidthInSourcePixels(line);
    return line.size();
  }
  int cur_width = 0;
  std::size_t i = 0;
  for (i = 0; i < line.size() && cur_width < *width; ++i) {
    cur_width += GetGlyph(line[i]).horiz_advance_;
  }
  *width = cur_width;
  return i;
}

Font::Font()
    : ref_count_(1),
      path_(""),
      pages_(),
      default_font_page_(nullptr),
      char_to_glyph_map_(),
      right_to_left_(false),
      distance_field_(false),
      // Strokes aren't shown by default, hence the Color.
      default_stroke_color_(RageColor(0, 0, 0, 0)),
      chars_("") {}
Font::~Font() { Unload(); }

void Font::Unload() {
  for (unsigned i = 0; i < pages_.size(); ++i) {
    delete pages_[i];
  }
  pages_.clear();

  char_to_glyph_map_.clear();
  default_font_page_ = nullptr;

  // Don't clear the refcount. We've unloaded, but that doesn't mean things
  // aren't still pointing to us.
}

void Font::Reload() {
  if (path_.empty()) {
    LuaHelpers::ReportScriptError(
        "Cannot reload a font that has an empty path.");
    return;
  }
  Unload();
  Load(path_, chars_);
}

void Font::AddPage(FontPage* page) {
  pages_.push_back(page);

  for (auto it = page->char_to_glyph_no_.begin();
       it != page->char_to_glyph_no_.end(); ++it) {
    char_to_glyph_map_[it->first] = &page->glyphs_[it->second];
  }
}

void Font::MergeFont(Font& font) {
  // If we don't have a font page yet, and f does, grab the default font page.
  // It'll usually be overridden later on by one of our own font pages; this
  // will be used only if we don't have any font pages at all.
  if (default_font_page_ == nullptr) {
    default_font_page_ = font.default_font_page_;
  }

  for (auto it = font.char_to_glyph_map_.begin();
       it != font.char_to_glyph_map_.end(); ++it) {
    char_to_glyph_map_[it->first] = it->second;
  }

  pages_.insert(pages_.end(), font.pages_.begin(), font.pages_.end());

  font.pages_.clear();
}

const glyph& Font::GetGlyph(wchar_t c) const {
  // NOTE(Colby): This is kind of nasty, but the parts that touch this are dark
  // and scary.
  //
  // Snagged from OpenITG, original comment:
  // NOTE(infamouspat): shooting a blank really...DarkLink kept running into the
  // stupid assert with non-roman song titles, and looking at it, I'm gonna
  // guess that this is how ITG2 prevented crashing with them.
  if (c < 0 || c > 0xFFFFFF) {
    c = 1;
  }

  // Fast path:
  if (c < (int)ARRAYLEN(char_to_glyph_cache_) && char_to_glyph_cache_[c]) {
    return *char_to_glyph_cache_[c];
  }

  // Try the regular character.
  auto it = char_to_glyph_map_.find(c);

  // If that's missing, use the default glyph.
  if (it == char_to_glyph_map_.end()) {
    it = char_to_glyph_map_.find(FONT_DEFAULT_GLYPH);
  }

  if (it == char_to_glyph_map_.end()) {
    RageException::Throw(
        "The default glyph is missing from the font \"%s\".", path_.c_str());
  }

  return *it->second;
}

          continue;
        }
        // We must have either 1 match (just the codeset) or 4 (the whole
        // thing).
        int count = -1;
        unsigned int first = 0;
        if (!matches[2].empty()) {
          sscanf(matches[2].c_str(), "%x", &first);
          unsigned int last;
          sscanf(matches[3].c_str(), "%x", &last);
          if (last < first) {
            LuaHelpers::ReportScriptErrorFmt(
                "Font definition \"%s\" has an invalid range \"%s\": %i < %i.",
                ini.GetPath().c_str(), name.c_str(), last, first);
            continue;
          }
          count = last - first + 1;
        }
=======
>>>>>>> Stashed changes

        RString error_string =
            cfg.MapRange(matches[0], first, value->GetValue<int>(), count);
        if (!error_string.empty()) {
          LuaHelpers::ReportScriptErrorFmt(
              "Font definition \"%s\" has an invalid range \"%s\": %s.",
              ini.GetPath().c_str(), name.c_str(), error_string.c_str());
          continue;
        }

        continue;
      }


  // Check for recursion (recursive imports).
  for (unsigned i = 0; i < kLoadStack.size(); ++i) {
    if (kLoadStack[i] == ini_path) {
      RString str = join("\n", kLoadStack);
      str += "\nCurrent font: " + ini_path;
      LuaHelpers::ReportScriptErrorFmt(
          "Font import recursion detected\n%s", str.c_str());
      return;
    }
  }
  kLoadStack.push_back(ini_path);

  // The font is not already loaded. Figure out what we have.
  CHECKPOINT_M(
      ssprintf("Font::Load(\"%s\",\"%s\").", ini_path.c_str(), chars_.c_str()));

  path_ = ini_path;
  chars_ = chars;

  // Get the filenames associated with this font.
  std::vector<RString> texture_paths;
  GetFontPaths(ini_path, texture_paths);

  bool capitals_only = false;

  // If we have an INI, load it.
  IniFile ini;
  if (!ini_path.empty()) {
    ini.ReadFile(ini_path);
    ini.RenameKey("Char Widths", "main");  // backward compat
    ini.GetValue("common", "CapitalsOnly", capitals_only);
    ini.GetValue("common", "RightToLeft", right_to_left_);
    ini.GetValue("common", "DistanceField", distance_field_);
    RString s;
    if (ini.GetValue("common", "DefaultStrokeColor", s)) {
      default_stroke_color_.FromString(s);
    }
  }

  {
    std::vector<RString> import_list;

    bool is_top_level_font = kLoadStack.size() == 1;

    // If this is a top-level font (not a subfont), load the default font first.
    if (is_top_level_font) {
      import_list.push_back("Common default");
    }

    // Check to see if we need to import any other fonts.  Do this
    // before loading this font, so any characters in this font
    // override imported characters.
    RString imports;
    ini.GetValue("main", "import", imports);
    split(imports, ",", import_list, true);

    if (is_top_level_font && imports.empty() && texture_paths.empty()) {
      RString s = ssprintf(
          "Font \"%s\" is a top-level font with no textures or imports.",
          ini_path.c_str());
      Dialog::OK(s);
    }

    for (unsigned i = 0; i < import_list.size(); ++i) {
      RString path = THEME->GetPathF("", import_list[i], true);
      if (path == "") {
        RString s = ssprintf(
            "Font \"%s\" imports a font \"%s\" that doesn't exist",
            ini_path.c_str(), import_list[i].c_str());
        Dialog::OK(s);
        continue;
      }

      Font subfont;
      subfont.Load(path, "");
      MergeFont(subfont);
    }
  }

  // Load each font page.
  for (unsigned i = 0; i < texture_paths.size(); ++i) {
    const RString& texture_path = texture_paths[i];

    // Grab the page name, eg "foo" from "Normal [foo].png".
    RString page_name = GetPageNameFromFileName(texture_path);

    // Ignore stroke textures
    if (texture_path.find("-stroke") != std::string::npos) {
      continue;
    }

    // Create this down here so it doesn't leak if the continue gets triggered.
    FontPage* page = new FontPage;

    // Load settings for this page from the INI.
    FontPageSettings cfg;
    LoadFontPageSettings(cfg, ini, texture_path, "common", chars);
    LoadFontPageSettings(cfg, ini, texture_path, page_name, chars);

    // Load.
    page->Load(cfg);

    // Expect at least as many frames as we have premapped characters.
    // Make sure that we don't map characters to frames we don't actually
    // have. This can happen if the font is too small for an chars.
    for (auto it = page->char_to_glyph_no_.begin();
         it != page->char_to_glyph_no_.end(); ++it) {
      if (it->second <
          page->font_page_textures_.texture_main_->GetNumFrames()) {
        continue;
      }
      LuaHelpers::ReportScriptErrorFmt(
          "The font \"%s\" maps \"%s\" to frame %i, "
          "but the font only has %i frames.",
          texture_path.c_str(), WcharDisplayText(wchar_t(it->first)).c_str(),
          it->second, page->font_page_textures_.texture_main_->GetNumFrames());
      it->second = 0;
    }

    AddPage(page);

    // If this is the first font loaded, or it's called "main", this page's
    // properties become the font's properties.
    if (i == 0 || page_name == "main") {
      SetDefaultGlyph(page);
    }
  }

  if (capitals_only) {
    CapsOnly();
  }

  if (char_to_glyph_map_.empty()) {
    LuaHelpers::ReportScriptErrorFmt(
        "Font %s has no characters", ini_path.c_str());
  }

  kLoadStack.pop_back();

  if (kLoadStack.empty()) {
    // Cache ASCII glyphs.
    ZERO(char_to_glyph_cache_);
    for (auto it = char_to_glyph_map_.begin(); it != char_to_glyph_map_.end();
         ++it) {
      if (it->first < (int)ARRAYLEN(char_to_glyph_cache_)) {
        char_to_glyph_cache_[it->first] = it->second;
      }
    }
  }
}

/*
 * (c) 2001-2004 Glenn Maynard, Chris Danford
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
