#include "projects/wave/wave_project.hh"
//C system files
//C++ system files
#include <memory>
//Other libraries' .h files
#include "utils/imgui/imgui_glfw.h"
//Your project's .h files
#include "project_dictionary.hh"
#include "utils/timer.hh"
#include "utils/shader_module.hh"
#include "utils/shader_factory.hh"
#include "utils/texture_module.hh"
#include "utils/camera.hh"
#include "utils/imgui/imgui_log.h"
#include "utils/imgui/imgui_property_editor.h"
#include "utils/light_module.hh"
#include "graphic_contexts/opengl_context.hh"
#include "core/particle_module.hh"

// Specific project particle blueprints
#include "projects/wave/wave_event_handler.hh"
#include "projects/wave/obstacle_collision.hh"
#include "projects/wave/wave_blueprint.hh"
#include "projects/wave/wave_scene.hh"

namespace gem { namespace particle {
namespace wave_project {
namespace {
// A pointer to interface, to enable flexibility over
// window management system or 3D API (GLFW/Windows
// & OpenGL/Direct3D)
std::shared_ptr<GraphicContext> graphic_context;
}

void RegisterProject() {
  project_dict::AddStage("wave", std::bind(&Init));
  project_dict::AddStage("wave", std::bind(&Run));
  project_dict::AddStage("wave", std::bind(&Terminate));
}

void Init() {
  // OpenGL setup
  graphic_context = std::make_shared<OpenGLContext>();
  graphic_context->Init();

  // ImGui initialization
  ImGui_ImplGlfwGL3_Init(static_cast<GLFWwindow*>(graphic_context->GetWindowHandle()), true);
  ImGui::StyleColorsClassic();

  shader::module::Init();
  shader::factory::SetShadersFolderBasePath("src/projects/lit_particles/shaders/");
  texture::module::Init();

  // Camera initialization
  camera::Init();
  camera::LookAt( 
    glm::vec3(0, 10, 10),   // Camera is at (4,4,4), in World Space
    glm::vec3(0.0f, 0.0f, -1.5f),   // and looks at the origin
    glm::vec3(0, 1, 0));  // Head is up (set to 0,-1,0 to look upside-down)
  camera::SetPerspectiveProjection( 
    glm::radians(45.0f), 
    4.0f, 3.0f, // TODO: This fits the hardcoded 640/480 in the opengl_context.cc file, change this accordingly to changes made in the other file
    0.1f, 100.0f);

  // Light module initialization
  light::module::Init();

  // Scene initialization
  scene::Init();
  scene::SetDebugOption(true);

  // Particle system initialization
  particle_module::Init();
  blueprint::wave_system_builder::SetParticleSystemName("wave particles system");
  blueprint::wave_system_builder::SetTexture("textures/welding_sparkle.png");
  blueprint::wave_system_builder::SetParticleCount(5001u);
  blueprint::wave_system_builder::Create();

  // Event handler initialization
  event_handler::Init(graphic_context, blueprint::wave_system_builder::GetObstacleHandle());
}

void Run() {
  glfwSetWindowTitle(static_cast<GLFWwindow*>(
    graphic_context->GetWindowHandle()), "GemParticles");
  while (!graphic_context->PollWindowClosedEvent()) {
    const double dt = timer::Chrono::GetInstance().GetTimeElapsedInSeconds();
    ImGui_ImplGlfwGL3_NewFrame();

    ImGui::Begin("Stats");
    ImGui::Text("Application average %.3f ms / frame(%.1f FPS)\n", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Total active particles = %d\n", particle_module::GetActiveParticlesCount());
    ImGui::End();

    scene::Render();
    particle_module::Update(dt);
    event_handler::Update(); // Has to be placed before before clearing depth buffer bit
    light::module::FlushDataToGPU();

    ImGuiPropertyEditor::GetInstance().Draw("Property editor");
    ImGuiLog::GetInstance().Draw("Debugging logs");
    ImGui::Render();

    graphic_context->Update();
    timer::Chrono::GetInstance().Update();
  }
}

void Terminate() {
  // App destruction
  particle_module::Terminate();
  light::module::Terminate();
  scene::Terminate();
  event_handler::Terminate();
  texture::module::Terminate();
  shader::module::Terminate();
  ImGui_ImplGlfwGL3_Shutdown();
  graphic_context->Terminate();
}
} /* namespace lit particles_project */
} /* namespace particle */
} /* namespace gem */