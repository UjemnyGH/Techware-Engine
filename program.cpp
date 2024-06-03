#include "engine/src/entry_point.hpp"

class MainScene : public te::Scene {
public:
    MainScene() {
        mName = "MainScene";
        te::SceneHandler::pGlobal->AddScene(this);
        TE_INFO("Added main scene")
        te::SceneHandler::pGlobal->SwitchScene(this);
    }

    virtual void Start() override {
        printf("Start\n");

        uint8_t bytes[8] = {
            1, 2, 3, 4, 5, 6, 7, 8
        };

        uint64_t val = *(uint64_t*)&bytes[0];

        printf("%lx\n", val);
    }

    virtual void Update() override {

    }
};

MainScene gMainScene;