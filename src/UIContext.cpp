
#include "StarFighter/UIContext.h"

#include "nucleus/Files/FilePath.h"
#include "nucleus/Streams/FileInputStream.h"

#include "nucleus/MemoryDebug.h"

ca::Font* UIContext::getFont(const std::string& name) {
  if (!m_font) {
#if OS(WIN)
    nu::FileInputStream fontStream{nu::FilePath{"C:\\Windows\\Fonts\\Arial.ttf"}};
#elif OS(MACOSX)
    nu::FileInputStream fontStream{nu::FilePath{"/Library/Fonts/Arial.ttf"}};
#endif  // OS(MACOSX)

    m_font.reset(new ca::Font{});
    m_font->loadFromStream(&fontStream);
  }

  return m_font.get();
}
