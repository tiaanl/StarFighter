
#include <StarFighter/Components/CombatComponent.h>
#include <elastic/views/ColorView.h>
#include <elastic/views/LabelView.h>
#include "StarFighter/UIContext.h"
#include "StarFighter/World.h"
#include "canvas/app.h"
#include "canvas/math/transform.h"
#include "elastic/views/button_view.h"

class StartFighterApp : public ca::WindowDelegate {
public:
    bool onWindowCreated() override {
        // We set the first timestamp we have.
        m_lastFrameStartTime = getTimestamp();

        m_world = nu::MakeScopedPtr<World>();
        if (!m_world->create()) {
            return false;
        }

        m_ui = nu::MakeScopedPtr<UIContext>();
        buildUI(m_ui.get());

        return true;
    }

    void onWindowResized(const ca::Size<U32>& size) override {
        m_viewportSize = size;
        m_world->setViewportSize(size);
    }

    void onMouseMoved(const ca::MouseEvent& evt) override {
        ca::WindowDelegate::onMouseMoved(evt);

        m_world->onMouseMoved(evt);
    }

    void onMousePressed(const ca::MouseEvent& evt) override {
        ca::WindowDelegate::onMousePressed(evt);

        m_world->onMousePressed(evt);
    }

    void onMouseReleased(const ca::MouseEvent& evt) override {
        ca::WindowDelegate::onMouseReleased(evt);

        m_world->onMouseReleased(evt);
    }

    void onMouseWheel(const ca::MouseWheelEvent& evt) override {
        ca::WindowDelegate::onMouseWheel(evt);

        m_world->onMouseWheel(evt);
    }

    void onKeyPressed(const ca::KeyEvent& evt) override {
        ca::WindowDelegate::onKeyPressed(evt);

        m_world->onKeyPressed(evt);
    }

    void onKeyReleased(const ca::KeyEvent& evt) override {
        ca::WindowDelegate::onKeyReleased(evt);

        m_world->onKeyReleased(evt);
    }

    void onPaint(ca::Canvas* canvas) override {
        F64 timestamp = getTimestamp();
        F64 timeSinceLastFrame = timestamp - m_lastFrameStartTime;

        // We want a 60fps update rate, so calculate the adjustment.
        static constexpr F64 frameTime = 1.0 / 60.0 * 1000000.0;
        F32 adjustment = static_cast<float>(timeSinceLastFrame / frameTime);

        canvas->clear(ca::Color(0, 0, 0));

        // Render the world.
        m_world->update(canvas, m_viewportSize, adjustment);

        ju::Entity* starFighter = m_world->getPlayerEntity();
        auto combat = starFighter->getComponent<CombatComponent>();

        m_healthLabel->setLabel(std::to_string(combat->health));

        // Render the UI.
        m_ui->tick(adjustment);
        m_ui->render(canvas);

        canvas->render();

        m_lastFrameStartTime = timestamp;
    }

private:
    static F64 getTimestamp() {
        LARGE_INTEGER time;
        LARGE_INTEGER frequency;

        ::QueryPerformanceFrequency(&frequency);
        ::QueryPerformanceCounter(&time);

        return static_cast<F64>(time.QuadPart) * 1000000.0 / static_cast<F64>(frequency.QuadPart);
    }

    void buildUI(UIContext* context) {
        el::GroupView* root = context->getRoot();

        auto color1 = new el::ColorView(context, ca::Color(255, 0, 0));
        color1->setMinSize(ca::Size<I32>{100, 0});
        color1->setExpand(el::View::ExpandVertical);
        color1->setHorizontalAlign(el::View::AlignLeft);
        root->addChild(color1);

        auto color2 = new el::ColorView(context, ca::Color(0, 255, 0));
        color2->setMinSize(ca::Size<I32>{100, 0});
        color2->setExpand(el::View::ExpandVertical);
        color2->setHorizontalAlign(el::View::AlignRight);
        root->addChild(color2);

        auto container = new el::StackedSizerView(context);
        root->addChild(container);

        auto color3 = new el::ColorView(context, ca::Color(255, 255, 0));
        color3->setExpand(el::View::ExpandBoth);
        container->addChild(color3);

        m_healthLabel = new el::LabelView(context, "Testing");
        container->addChild(m_healthLabel);

        auto button1 = new el::ButtonView(context, std::string{"Help!"}, nullptr);
        root->addChild(button1);
    }

    F64 m_lastFrameStartTime{0.f};

    ca::Size<U32> m_viewportSize;

    nu::ScopedPtr<World> m_world;
    nu::ScopedPtr<UIContext> m_ui;

    el::LabelView* m_healthLabel;
};

CANVAS_APP(StartFighterApp);