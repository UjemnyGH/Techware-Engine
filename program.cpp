#include "engine/src/entry_point.hpp"
#include "engine/src/renderer.hpp"

class MainScene : public te::Scene {
public:
    MainScene() {
        mName = "MainScene";
        mInitializeWithWindow = true;
        te::SceneHandler::pGlobal->AddScene(this);
        TE_INFO("Added main scene")
        te::SceneHandler::pGlobal->SwitchScene(this);
    }

    te::SimpleRenderer renderer = te::SimpleRenderer("MainSceneRenderer");
    te::GLShader defVS, defFS;

    te::GLUniformBuffer cameraUniform;

    Matrix4<real> mProjection;
    Matrix4<real> mView;

    virtual void Start() override {
        defVS.LoadShader("engine/shader_default/def_simple_shader_light.vs");
        defFS.LoadShader("engine/shader_default/def_simple_shader_light.fs");

        renderer.AttachShaders({defVS, defFS});

        cameraUniform.Allocate(2 * sizeof(real) * 16);

        cameraUniform.UniformBinding(1);

        cameraUniform.BindData(0, sizeof(Matrix4<real>), mProjection.m);
        cameraUniform.BindData(16 * sizeof(real), sizeof(Matrix4<real>), mView.m);

        te::LayerHandler::pGlobal->AddLayer(&renderer);

        TE_INFO("Main scene started")
    }

    virtual void Update() override {

    }
};

MainScene gMainScene;