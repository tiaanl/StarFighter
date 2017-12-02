
#include "StarFighter/UIContext.h"

#include "nucleus/files/file_path.h"
#include "nucleus/streams/file_input_stream.h"

#include "nucleus/MemoryDebug.h"

ca::Font* UIContext::getFont(const std::string& name) {
  if (!m_font) {
    nu::FileInputStream fontStream{nu::FilePath{FILE_PATH_LITERAL("C:\\Windows\\Fonts\\Arial.ttf")}};

    m_font.reset(new ca::Font{});
    m_font->loadFromStream(&fontStream);
  }

  return m_font.get();
}
