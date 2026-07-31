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
#include "vulkan/vulkan.hpp"
#include <SDL3/SDL_video.h>
#include <vector>
#include "VkBootstrap.h"
#include "SDL3/SDL.h"

#define DEBUG false

namespace ezvk::core {
    struct Device {
        vkb::PhysicalDevice phys;
        vkb::Device logical;
    };

    struct Window {
        SDL_Window* win;
        vkb::Swapchain* swap;
    };

    struct Instance {
        vkb::Instance instance;
        Device* active_device;
        std::vector<Device*> devices;
        
    };

    struct Handle {
        Instance* instance;
        Window* window;
    };


    void init(Handle*);
    void destroy(Handle*);

    void initWindow(Handle*);

    namespace low {
        void initInstance(Instance*);
        void destroy(Instance*);

        vkb::PhysicalDevice selectGPU(vkb::Instance);
        void initDevice(vkb::PhysicalDevice, Device*);
    };
};
