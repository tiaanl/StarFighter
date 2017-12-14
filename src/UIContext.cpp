
#include "StarFighter/UIContext.h"

#include "nucleus/files/file_path.h"
#include "nucleus/streams/file_input_stream.h"

#include "nucleus/MemoryDebug.h"

ca::Font* UIContext::getFont(const std::string& name) {
  if (!m_font) {
#if OS(WIN32)
    nu::FileInputStream fontStream{nu::FilePath{FILE_PATH_LITERAL("C:\\Windows\\Fonts\\Arial.ttf")}};
#elif OS(MACOSX)
    nu::FileInputStream fontStream{nu::FilePath{FILE_PATH_LITERAL("/Library/Fonts/Arial.ttf")}};
#endif  // OS(MACOSX)

    m_font.reset(new ca::Font{});
    m_font->loadFromStream(&fontStream);
  }

  return m_font.get();
}
