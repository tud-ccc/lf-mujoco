
#include "mujoco_simulator/MujocoSimulator.hh"

// PREAMBLE

reactor::PhysicalAction<KeyboardInput>* keyboard_action = nullptr;
reactor::PhysicalAction<MouseButtonInput>* mouse_button_action = nullptr;
reactor::PhysicalAction<MouseMoveInput>* mouse_move_action = nullptr;
reactor::PhysicalAction<ScrollInput>* scroll_action = nullptr;

// keyboard callback
void keyboard(GLFWwindow* window, int key, int scancode, int act, int mods) {
  KeyboardInput keyboard_struct{window, key, scancode, act, mods};
  keyboard_action->schedule(keyboard_struct);
}

void mouse_button(GLFWwindow* window, int button, int act, int mods) {
  MouseButtonInput mouse_struct{window, button, act, mods};
  mouse_button_action->schedule(mouse_struct);
}

void mouse_move(GLFWwindow* window, double xpos, double ypos) {
  MouseMoveInput mouse_move_struct{window, xpos, ypos};
  mouse_move_action->schedule(mouse_move_struct);
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
  ScrollInput scroll_struct{window, xoffset, yoffset};
  scroll_action->schedule(scroll_struct);
}

// REACTIONS

void MujocoSimulator::Inner::initialize([[maybe_unused]] const reactor::StartupTrigger& startup,
                                        reactor::Output<mjModel*>& output_model,
                                        reactor::PhysicalAction<KeyboardInput>& keyboard_physical_action,
                                        reactor::PhysicalAction<MouseButtonInput>& mouse_button_physical_action,
                                        reactor::PhysicalAction<MouseMoveInput>& mouse_move_physical_action,
                                        reactor::PhysicalAction<ScrollInput>& scroll_physical_action) {
  std::cout << "Starting Simulator with: " << file << " and time resolution: " << time_resolution << std::endl;

  // load model from file and check for errors
  model = mj_loadXML(file.c_str(), NULL, error.data(), 1000);
  if (model == nullptr) {
    std::string str(std::begin(error), std::end(error));
    std::cerr << "ERROR: " << str << std::endl;
    std::exit(1);
  } else {
    // make data corresponding to model
    data = mj_makeData(model);

    if (visualize) {
      // init GLFW, create window, make OpenGL contexttext current, request v-sync
      glfwInit();
      window = glfwCreateWindow(1200, 900, "MujocoSimulator", NULL, NULL);
      glfwMakeContextCurrent(window);
      glfwSwapInterval(1);

      // initialize visualization data structures
      mjv_defaultCamera(&camera);
      mjv_defaultOption(&options);
      mjr_defaultContext(&context);

      // create scene and contexttext
      mjv_makeScene(model, &scene, 2000);
      mjr_makeContext(model, &context, mjFONTSCALE_150);

      // set handlers for physical actions
      keyboard_action = &keyboard_physical_action;
      mouse_button_action = &mouse_button_physical_action;
      mouse_move_action = &mouse_move_physical_action;
      scroll_action = &scroll_physical_action;

      // install GLFW mouse and keyboard callbacks
      glfwSetKeyCallback(window, keyboard);
      glfwSetCursorPosCallback(window, mouse_move);
      glfwSetMouseButtonCallback(window, mouse_button);
      glfwSetScrollCallback(window, scroll);

      // TODO: remove until bodyless methods are implemented
      // get framebuffer viewport
      mjrRect viewport = {0, 0, 0, 0};
      glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

      // update scene and render
      mjv_updateScene(model, data, &options, NULL, &camera, mjCAT_ALL, &scene);
      mjr_render(viewport, &scene, &context);

      // swap OpenGL buffers (blocking call due to v-sync)
      glfwSwapBuffers(window);

      output_model.set(model);
    }
  }
}

void MujocoSimulator::Inner::deconstruct([[maybe_unused]] const reactor::ShutdownTrigger& shutdown) {
  // free model and data
  mj_deleteData(data);
  mj_deleteModel(model);

  if (visualize) {
    // close GLFW, free visualization storage
    glfwTerminate();
    mjv_freeScene(&scene);
    mjr_freeContext(&context);
  }
}

void MujocoSimulator::Inner::draw_frame([[maybe_unused]] const reactor::Timer& draw_frame_timer) {
  std::cout << "draw frame" << std::endl;

  // get framebuffer viewport
  mjrRect viewport = {0, 0, 0, 0};
  glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

  // update scene and render
  mjv_updateScene(model, data, &options, NULL, &camera, mjCAT_ALL, &scene);
  mjr_render(viewport, &scene, &context);

  // swap OpenGL buffers (blocking call due to v-sync)
  glfwSwapBuffers(window);
  // process pending GUI events, call GLFW callbacks
  glfwPollEvents();
}

void MujocoSimulator::Inner::simulation_advance([[maybe_unused]] const reactor::Timer& simulation_step_timer,
                                                reactor::Output<mjData*>& call_controller) {
  mj_step(model, data);
  call_controller.set(data);
}

void MujocoSimulator::Inner::simulation_continue([[maybe_unused]] const reactor::Input<void>& return_controller) {
  mj_step1(model, data);
}

void MujocoSimulator::Inner::handle_keyboard_input(
    [[maybe_unused]] const reactor::PhysicalAction<KeyboardInput>& keyboard_physical_action) {
  std::cout << "KEYBOARD ACTION" << std::endl;
  auto user_input = *keyboard_physical_action.get();

  // backspace: reset simulation
  if (user_input.act == GLFW_PRESS && user_input.key == GLFW_KEY_BACKSPACE) {
    mj_resetData(model, data);
    mj_forward(model, data);
  }
}

void MujocoSimulator::Inner::handle_mouse_button_input(
    [[maybe_unused]] const reactor::PhysicalAction<MouseButtonInput>& mouse_button_physical_action) {
  std::cout << "MOUSE BUTTON ACTION" << std::endl;
  // update button state
  button_left = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
  button_middle = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
  button_right = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

  // update mouse position
  glfwGetCursorPos(window, &lastx, &lasty);
}

void MujocoSimulator::Inner::handle_mouse_move_input(
    const reactor::PhysicalAction<MouseMoveInput>& mouse_move_physical_action) {
  std::cout << "MOUSE MOVE ACTION" << std::endl;
  auto user_input = *mouse_move_physical_action.get();

  // no buttons down: nothing to do
  if (!button_left && !button_middle && !button_right) {
    return;
  }

  // compute mouse displacement, save
  double dx = user_input.xpos - lastx;
  double dy = user_input.ypos - lasty;
  lastx = user_input.xpos;
  lasty = user_input.ypos;

  // get current window size
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  // get shift key state
  bool mod_shift =
      (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

  // determine action based on mouse button
  mjtMouse action;
  if (button_right) {
    action = mod_shift ? mjMOUSE_MOVE_H : mjMOUSE_MOVE_V;
  } else if (button_left) {
    action = mod_shift ? mjMOUSE_ROTATE_H : mjMOUSE_ROTATE_V;
  } else {
    action = mjMOUSE_ZOOM;
  }

  // move camera
  mjv_moveCamera(model, action, dx / height, dy / height, &scene, &camera);
}

void MujocoSimulator::Inner::handle_scroll_input(
    [[maybe_unused]] const reactor::PhysicalAction<ScrollInput>& scroll_physical_action) {
  std::cout << "SCROLL ACTION" << std::endl;
  auto user_input = *scroll_physical_action.get();
  // emulate vertical mouse motion = 5% of window height
  mjv_moveCamera(model, mjMOUSE_ZOOM, 0, -0.05 * user_input.yoffset, &scene, &camera);
}
