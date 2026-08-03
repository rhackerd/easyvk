// Copyright 2026 rhacker
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     https://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include "VkBootstrapDispatch.h"
#include "vulkan/vulkan.hpp"
#include <SDL3/SDL_video.h>
#include <utility>
#include <vector>
#include "VkBootstrap.h"
#include "SDL3/SDL.h"

// Kanye West is the goat

#define DEBUG false

namespace ezvk::core {

    constexpr bool debug_enabled = false;

    using GPU_Queue = std::pair<vk::Queue, unsigned int>;

    struct Device {
        vkb::PhysicalDevice phys;
        vkb::Device         logical;
        vkb::DispatchTable  table;
        // Queue family
        GPU_Queue graphics;
        GPU_Queue present;
        GPU_Queue transfer;
        GPU_Queue compute;
    };

    struct WindowCreateInfo {
        const char*     title   = "easyvk window";
        int             w       = 1280;
        int             h       = 720;
        SDL_WindowFlags flags   = 0;
    };

    struct Window {
        SDL_Window      *handle = nullptr;
        vk::SurfaceKHR  surface = nullptr;
        vkb::Swapchain  swap;
        Device          *device = nullptr;
    };

    struct Instance {
        vkb::Instance               instance;
        std::vector<Device>         devices;
        std::vector<Window>         windows;
        vkb::InstanceDispatchTable  table;
    };

    // Mainly for future use
    struct Handle {
        Instance* instance;
    };


    void init(Handle*);
    void destroy(Handle*);

    void initWindow(WindowCreateInfo, Handle*);

    namespace low {
        // struct Window
        void initWindow(WindowCreateInfo, Window*, Handle*);
        void initSwapchain(Window*, bool recreate = true);

        void initInstance(Instance*);
        void destroy(Instance*);


        // struct Device
        vkb::PhysicalDevice selectGPU(vkb::Instance, vk::SurfaceKHR surface);
        void initDevice(vkb::PhysicalDevice, Device*);
        void extractQueues(Device*);
    };
};
