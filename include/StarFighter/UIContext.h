
#ifndef STAR_FIGHTER_UI_CONTEXT_H_
#define STAR_FIGHTER_UI_CONTEXT_H_

#include <memory>

#include "canvas/Rendering/Font.h"
#include "elastic/Context.h"

class UIContext : public el::Context {
public:
  explicit UIContext(ca::ResourceManager* resourceManager);
  ~UIContext() = default;

  ca::Font* getFont(const std::string& name) override;

private:
  std::unique_ptr<ca::Font> m_font;
};

#endif  // STAR_FIGHTER_UI_CONTEXT_H_
