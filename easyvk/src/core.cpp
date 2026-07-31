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

#include "core.hpp"
#include "VkBootstrap.h"
#include <SDL3/SDL_video.h>
#include <algorithm>
#include <cstdio>

namespace ezvk::core {
    void init(Handle* handle) {
        if (handle->instance)
        Instance instance = low::initInstance();

        vkb::PhysicalDevice phys_dev = low::selectGPU(instance.instance);
        Device _dev = low::initDevice(phys_dev);

        instance.devices.push_back(std::move(_dev));
        instance.active_device = &instance.devices.back();

        handle->instance = instance;
    }

    Window createWindow(const char* title, int w, int h, SDL_WindowFlags) {
        SDL_CreateWindow("", int w, int h, SDL_WindowFlags flags)   
    }


    namespace low {
        Instance initInstance() {
            vkb::InstanceBuilder builder;
            auto build_block = builder.set_app_name("easyvk");

            #ifdef DEBUG
                build_block.request_validation_layers(true)
                .use_default_debug_messenger();
            #endif

            auto inst_ret = build_block.build();            
            if (!inst_ret) printf("easyvk: failed to initiate vulkan instance");
            return {.instance = inst_ret.value()};
        }

        vkb::PhysicalDevice selectGPU(vkb::Instance instance, vk::SurfaceKHR surface) {
            vkb::PhysicalDeviceSelector selector{instance};
            auto phys_ret = selector.set_surface(surface)
            .set_minimum_version(1, 2)
            .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
            .select();
            if (!phys_ret) printf("easyvk: failed to select physical device\n");
            return phys_ret.value();
        }

        Device initDevice(vkb::PhysicalDevice phys) {
            vkb::DeviceBuilder builder(phys);
            auto dev_ret = builder.build();
            if (!dev_ret) printf("easyvk: failed to initiate logical device\n");
            vkb::Device vkb_dev = dev_ret.value();
            return {.phys = phys, .logical = vkb_dev};
        }        
    };
};