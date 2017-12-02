
#ifndef STAR_FIGHTER_UI_CONTEXT_H_
#define STAR_FIGHTER_UI_CONTEXT_H_

#include "canvas/rendering/font.h"
#include "elastic/context.h"
#include "nucleus/Memory/ScopedPtr.h"

class UIContext : public el::Context {
public:
  ca::Font* getFont(const std::string& name) override;

private:
  nu::ScopedPtr<ca::Font> m_font;
};

#endif  // STAR_FIGHTER_UI_CONTEXT_H_
