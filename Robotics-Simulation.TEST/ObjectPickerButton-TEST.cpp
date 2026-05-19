#include "pch.h"
#include "input/ObjectPickerButton.h"
#include "MockInputService.h"
#include "MockLogger.h"
#include "core/Scene.h"

namespace
{
    class GhostCreatedSpy final : public IObserver<GhostCreatedEvent>
    {
    public:
        void onNotify(GhostCreatedEvent* event) override
        {
            wasNotified = true;
            createdGhost = event ? event->createdGhost : nullptr;
        }

        bool wasNotified = false;
        GameObject* createdGhost = nullptr;
    };

    GameObject CreateTargetObject(MockLogger& logger, const std::string& name = "Target")
    {
        return GameObject(logger, 42, Transform{}, name);
	}

    // Simulates a click on `owner`'s ObjectPickerButton, then flushes both the
    // owner's component updates and the scene's deferred-deletion queue so that
    // objects marked for removal in onClick() are actually erased before the
    // caller inspects scene state.
    void ClickAndUpdate(MockInputService& inputService, GameObject& owner, Scene& scene, const Vector2& clickPos = { 0.f, 0.f })
    {
        ClickEvent click(KeyCode::LEFT_MOUSE_BUTTON, clickPos);
        auto* clickObserver = static_cast<IObserver<ClickEvent>*>(owner.GetComponent<ObjectPickerButton>());
        clickObserver->onNotify(&click);
        owner.Update();
        // Flush markedForDelete so RemoveGameObject calls take effect immediately.
        scene.UpdateGameObjects();
    }
}

TEST(ObjectPickerButtonTest, ClickSpawnsTargetObjectAndBroadcastsCreatedEvent)
{
    MockLogger logger;
    MockInputService inputService;
    CentralMessageDispatcher dispatcher;
    auto scene = std::make_unique<Scene>("test-scene");

    GameObject owner(logger, 1, Transform{ Vector2{0.f, 0.f}, Radian(0.f), Vector2{100.f, 100.f} }, "ButtonOwner");
    owner.EmplaceComponent<ObjectPickerButton>(
        logger,
        inputService,
        dispatcher,
        &scene,
        CreateTargetObject(logger));

    GhostCreatedSpy createdSpy;
    dispatcher.ghostCreatedEventBroadcast.subscribe(&createdSpy);

    ClickAndUpdate(inputService, owner, *scene);

    ASSERT_EQ(scene->GetGameObjects().size(), 1u);
    EXPECT_TRUE(createdSpy.wasNotified);
    EXPECT_NE(createdSpy.createdGhost, nullptr);
    EXPECT_EQ(createdSpy.createdGhost, &scene->GetGameObjects().back());
    EXPECT_EQ(scene->GetGameObjects().back().GetName(), "Target");
}

TEST(ObjectPickerButtonTest, SecondClickReplacesPreviouslyPickedGhost)
{
    MockLogger logger;
    MockInputService inputService;
    CentralMessageDispatcher dispatcher;
    auto scene = std::make_unique<Scene>("test-scene");

    GameObject owner(logger, 2, Transform{ Vector2{0.f, 0.f}, Radian(0.f), Vector2{100.f, 100.f} }, "ButtonOwner");
    owner.EmplaceComponent<ObjectPickerButton>(
        logger,
        inputService,
        dispatcher,
        &scene,
        CreateTargetObject(logger));

    ClickAndUpdate(inputService, owner, *scene);
    ASSERT_EQ(scene->GetGameObjects().size(), 1u);
    auto* firstSpawn = &scene->GetGameObjects().back();

    ClickAndUpdate(inputService, owner, *scene);

    EXPECT_EQ(scene->GetGameObjects().size(), 1u);
    auto* secondSpawn = &scene->GetGameObjects().back();
    EXPECT_NE(firstSpawn, secondSpawn);
}

TEST(ObjectPickerButtonTest, GhostPlacedEventClearsSelectionSoNextClickDoesNotReplace)
{
    MockLogger logger;
    MockInputService inputService;
    CentralMessageDispatcher dispatcher;
    auto scene = std::make_unique<Scene>("test-scene");

    GameObject owner(logger, 3, Transform{ Vector2{0.f, 0.f}, Radian(0.f), Vector2{100.f, 100.f} }, "ButtonOwner");
    owner.EmplaceComponent<ObjectPickerButton>(
        logger,
        inputService,
        dispatcher,
        &scene,
        CreateTargetObject(logger));

    ClickAndUpdate(inputService, owner, *scene);
    ASSERT_EQ(scene->GetGameObjects().size(), 1u);

    GhostPlacedEvent placed;
    dispatcher.ghostPlacedEventBroadcast.broadcast(&placed);

    ClickAndUpdate(inputService, owner, *scene);

    EXPECT_EQ(scene->GetGameObjects().size(), 2u);
}

TEST(ObjectPickerButtonTest, DisabledButtonDoesNotSpawnObject)
{
    MockLogger logger;
    MockInputService inputService;
    CentralMessageDispatcher dispatcher;
    auto scene = std::make_unique<Scene>("test-scene");

    GameObject owner(logger, 4, Transform{ Vector2{0.f, 0.f}, Radian(0.f), Vector2{100.f, 100.f} }, "ButtonOwner");
    ObjectPickerButton* button = owner.EmplaceComponent<ObjectPickerButton>(
        logger,
        inputService,
        dispatcher,
        &scene,
        CreateTargetObject(logger));

    button->Disable();
    ClickAndUpdate(inputService, owner, *scene);

    EXPECT_TRUE(scene->GetGameObjects().empty());
}
