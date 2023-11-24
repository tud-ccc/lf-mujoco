#include <fstream>
#include <iostream>
#include <queue>

#include "mujoco_simulator/MujocoSimulator.hh"
#include "world.hpp"
// PREAMBLE

mjvCamera global_camera{};   // abstract camera
mjvOption global_options{};  // visualization options
mjvScene global_scene{};     // abstract scene
mjrContext global_context{}; // custom GPU context
mjModel* global_model = nullptr;
mjData* global_data = nullptr;
bool global_button_left = false;
bool global_button_middle = false;
bool global_button_right = false;
double global_lastx = 0;
double global_lasty = 0;

// keyboard callback
void keyboard(GLFWwindow* window, int key, int scancode, int act, int mods) {
  std::cout << "KEYBOARD ACTION" << std::endl;

  // backspace: reset simulation
  if (act == GLFW_PRESS && key == GLFW_KEY_BACKSPACE) {
    mj_resetData(global_model, global_data);
    mj_forward(global_model, global_data);
  }
}

void mouse_button(GLFWwindow* window, int button, int act, int mods) {
  std::cout << "MOUSE BUTTON ACTION" << std::endl;
  // update button state
  global_button_left = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
  global_button_middle = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
  global_button_right = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

  // update mouse position
  glfwGetCursorPos(window, &global_lastx, &global_lasty);
}

void mouse_move(GLFWwindow* window, double xpos, double ypos) {
  std::cout << "MOUSE MOVE" << std::endl;
  // no buttons down: nothing to do
  if (!global_button_left && !global_button_middle && !global_button_right) {
    return;
  }

  // compute mouse displacement, save
  double dx = xpos - global_lastx;
  double dy = ypos - global_lasty;
  global_lastx = xpos;
  global_lasty = ypos;

  // get current window size
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  // get shift key state
  bool mod_shift =
      (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

  // determine action based on mouse button
  mjtMouse action;
  if (global_button_right) {
    action = mod_shift ? mjMOUSE_MOVE_H : mjMOUSE_MOVE_V;
  } else if (global_button_left) {
    action = mod_shift ? mjMOUSE_ROTATE_H : mjMOUSE_ROTATE_V;
  } else {
    action = mjMOUSE_ZOOM;
  }

  // move camera
  mjv_moveCamera(global_model, action, dx / height, dy / height, &global_scene, &global_camera);
}

void scroll(GLFWwindow* window, double xoffset, double yoffset) {
  std::cout << "SCROLL" << std::endl;
  // emulate vertical mouse motion = 5% of window height
  mjv_moveCamera(global_model, mjMOUSE_ZOOM, 0, -0.05 * yoffset, &global_scene, &global_camera);
}

// REACTIONS

void MujocoSimulator::Inner::initialize([[maybe_unused]] const reactor::StartupTrigger& startup,
                                        reactor::PhysicalAction<WorldData>& world_data) {
  std::cout << "Starting Simulator with: " << file << std::endl;

  // load model from file and check for errors
  global_model = mj_loadXML(file.c_str(), NULL, error.data(), 1000);
  if (global_model == nullptr) {
    std::string str(std::begin(error), std::end(error));
    std::cerr << "ERROR: " << str << std::endl;
    std::exit(1);
  } else {
    // make data corresponding to model
    global_data = mj_makeData(global_model);


    if (collect_data) {
      auto data = WorldData{global_data, global_model}; 
      data.write_csv_header("data.csv", global_model);
    }
  }

  thread = std::thread([&]() {
    if (visualize) {
      // init GLFW, create window, make OpenGL contexttext current, request v-sync
      glfwInit();
      //gladLoadGL();
      window = glfwCreateWindow(1200, 900, "MujocoSimulator", NULL, NULL);
      glfwMakeContextCurrent(window);
      glfwSwapInterval(1);

      // initialize visualization data structures
      mjv_defaultCamera(&global_camera);
      mjv_defaultOption(&global_options);
      mjr_defaultContext(&global_context);

      // create scene and contexttext
      mjv_makeScene(global_model, &global_scene, 2000);
      mjr_makeContext(global_model, &global_context, mjFONTSCALE_150);

      // install GLFW mouse and keyboard callbacks
      glfwSetKeyCallback(window, keyboard);
      glfwSetCursorPosCallback(window, mouse_move);
      glfwSetMouseButtonCallback(window, mouse_button);
      glfwSetScrollCallback(window, scroll);

      // TODO: remove until bodyless methods are implemented
      // get framebuffer viewport
      mjrRect viewport = {0, 0, 0, 0};
      glfwGetFramebufferSize(window, &viewport.width, &viewport.height);
    }

    while (!this->terminate) {
      mj_step(global_model, global_data);

      std::cout << "ADVANCING TIME: " << global_data->time << std::endl;

      {
        std::lock_guard<std::mutex> lock(this->last_controll_input_lock);
        for (auto i = 0; i < this->last_controll_input.size(); i++) {
          global_data->ctrl[i] = last_controll_input[i];
        }

        last_controll_input.clear();
      }

      if (visualize) {
        // get framebuffer viewport
        mjrRect viewport = {0, 0, 0, 0};
        glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

        // update scene and render
        mjv_updateScene(global_model, global_data, &global_options, NULL, &global_camera, mjCAT_ALL, &global_scene);
        mjr_render(viewport, &global_scene, &global_context);

        // swap OpenGL buffers (blocking call due to v-sync)
        glfwSwapBuffers(window);
        // process pending GUI events, call GLFW callbacks
        glfwPollEvents();

        if (glfwWindowShouldClose(window)) {
          exit(0);
        }
      }

      mj_step1(global_model, global_data);

      auto data = WorldData{global_data, global_model}; 

      if (collect_data) {
        data.write_to_csv("data.csv");
      }

      world_data.schedule(data);
    }
  });
}

void MujocoSimulator::Inner::deconstruct([[maybe_unused]] const reactor::ShutdownTrigger& shutdown) {
  terminate = false;
  thread.join();

  // free model and data
  mj_deleteData(global_data);
  mj_deleteModel(global_model);

  if (visualize) {
    // close GLFW, free visualization storage
    glfwTerminate();
    mjv_freeScene(&global_scene);
    mjr_freeContext(&global_context);
  }
}

void MujocoSimulator::Inner::handle_controll_signals(
    [[maybe_unused]] const reactor::Input<std::vector<double>>& controller_signals) {
  std::lock_guard<std::mutex> lock(this->last_controll_input_lock);
  last_controll_input = *controller_signals.get();
}

void MujocoSimulator::Inner::forward_world_data([[maybe_unused]] const reactor::PhysicalAction<WorldData>& world_data,
                                                [[maybe_unused]] reactor::Output<WorldData>& simulator_world_data) {
  simulator_world_data.set(*world_data.get());
}

