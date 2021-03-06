
#include "canvas/App.h"
#include "StarFighter/UIContext.h"
#include "StarFighter/World.h"
#include "canvas/Math/Transform.h"
#include "canvas/Resources/ResourceCache.h"
#include "elastic/Views/ButtonView.h"
#include "elastic/Views/ColorView.h"
#include "elastic/Views/LabelView.h"
#include "nucleus/Files.h"

#include "nucleus/MemoryDebug.h"

class StarFighterApp : public ca::WindowDelegate {
public:
  StarFighterApp()
    : ca::WindowDelegate("Star Fighter"), m_resourceManager(nu::getDefaultAllocator()), m_ui(&m_resourceManager) {}
  ~StarFighterApp() = default;

  bool onWindowCreated() override {
    // We set the first timestamp we have.
    m_lastFrameStartTime = getTimestamp();

    // Set up the resource cache.
    // m_resourceCache.setRootPath(nu::getCurrentWorkingDirectory().append("assets"));

    // m_world = std::make_unique<World>(&m_resourceCache);
    if (!m_world.create()) {
      return false;
    }

    buildUI(&m_ui);

    return true;
  }

  void onWindowResized(const ca::Size<U32>& size) override {
    m_viewportSize = size;
    m_world.setViewportSize(size);
  }

  void onMouseMoved(const ca::MouseEvent& evt) override {
    ca::WindowDelegate::onMouseMoved(evt);

    m_world.onMouseMoved(evt);
  }

  void onMousePressed(const ca::MouseEvent& evt) override {
    ca::WindowDelegate::onMousePressed(evt);

    m_world.onMousePressed(evt);
  }

  void onMouseReleased(const ca::MouseEvent& evt) override {
    ca::WindowDelegate::onMouseReleased(evt);

    m_world.onMouseReleased(evt);
  }

  void onMouseWheel(const ca::MouseWheelEvent& evt) override {
    ca::WindowDelegate::onMouseWheel(evt);

    m_world.onMouseWheel(evt);
  }

  void onKeyPressed(const ca::KeyEvent& evt) override {
    ca::WindowDelegate::onKeyPressed(evt);

    m_world.onKeyPressed(evt);
  }

  void onKeyReleased(const ca::KeyEvent& evt) override {
    ca::WindowDelegate::onKeyReleased(evt);

    m_world.onKeyReleased(evt);
  }

  void onPaint(ca::Canvas* canvas) override {
    F64 timestamp = getTimestamp();
    F64 timeSinceLastFrame = timestamp - m_lastFrameStartTime;

    // We want a 60fps update rate, so calculate the adjustment.
    static constexpr F64 frameTime = 1.0 / 60.0 * 1000000.0;
    auto adjustment = static_cast<F32>(timeSinceLastFrame / frameTime);

    canvas->clear(ca::Color(0, 0, 0));

    // Render the world.
    m_world.update(adjustment);
    m_world.render(canvas);

    // Render the UI.
    m_ui.tick(adjustment);
    m_ui.render(canvas);

    canvas->render();

    m_lastFrameStartTime = timestamp;
  }

private:
  COPY_DELETE(StarFighterApp);
  MOVE_DELETE(StarFighterApp);

  static F64 getTimestamp() {
#if OS(MACOSX)
    return 1.0;
#elif OS(WIN)
    LARGE_INTEGER time;
    LARGE_INTEGER frequency;

    ::QueryPerformanceFrequency(&frequency);
    ::QueryPerformanceCounter(&time);

    return static_cast<F64>(time.QuadPart) * 1000000.0 / static_cast<F64>(frequency.QuadPart);
#endif  // OS(WIN32)
  }

  void buildUI(UIContext* context) {
    el::GroupView* root = context->getRoot();

    auto color1 = new el::ColorView(context, ca::Color{127, 127, 127});
    color1->setMinSize(ca::Size<I32>{100, 0});
    color1->setExpand(el::View::ExpandVertical);
    color1->setHorizontalAlign(el::View::AlignLeft);
    root->addChild(color1);

    auto color2 = new el::ColorView(context, ca::Color{127, 127, 127});
    color2->setMinSize(ca::Size<I32>{100, 0});
    color2->setExpand(el::View::ExpandVertical);
    color2->setHorizontalAlign(el::View::AlignRight);
    root->addChild(color2);

    auto container = new el::StackedSizerView(context);
    root->addChild(container);

    auto color3 = new el::ColorView(context, ca::Color{127, 127, 127});
    color3->setExpand(el::View::ExpandBoth);
    container->addChild(color3);

    auto button1 = new el::ButtonView(context, std::string{"Help!"}, nullptr);
    root->addChild(button1);
  }

  F64 m_lastFrameStartTime = 0.f;

  ca::Size<U32> m_viewportSize;

  World m_world;
  ca::ResourceManager m_resourceManager;
  UIContext m_ui;
};

CANVAS_APP(StarFighterApp);
