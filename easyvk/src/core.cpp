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
#include <vulkan/vulkan_core.h>
#include "SDL3/SDL_vulkan.h"

namespace ezvk::core {
    void init(Handle* handle) {
        if (!handle->instance) {
            Instance* instance;
            low::initInstance(instance);
        }
        if (handle->instance->devices.empty()) {
            Device* dev;
            vkb::PhysicalDevice phys = low::selectGPU(handle->instance->instance);

        }
    }

    void initWindow(WindowCreateInfo CI, Handle* handle) {
        Window win;
        low::initWindow(CI, &win, handle);
        if (!win.surface) return;
        low::initSwapchain(&win, false);
        handle->instance->windows.push_back(win);
    }


    namespace low {

        void initWindow(WindowCreateInfo CI, Window* win, Handle* handle) {
            CI.flags |= SDL_WINDOW_VULKAN;
            win->handle = SDL_CreateWindow(CI.title, CI.w, CI.h, CI.flags);
            if (!win->handle) printf("easyvk: failed to create window");

            bool result = SDL_Vulkan_CreateSurface(win->handle, handle->instance->instance, nullptr, reinterpret_cast<VkSurfaceKHR*>(&win->surface));
            if (!result) {printf("easyvk: failed to create surface %s\n", SDL_GetError()); return;}
        };

        void initSwapchain(Window * win, bool recreate) {
            vkb::SwapchainBuilder swap_builder(win->device->logical, win->surface);
            if (recreate) swap_builder.set_old_swapchain(win->swap);
            auto swap_ret = swap_builder.build();
            if (!swap_ret) printf("easyvk: failed to create swapchain");
            win->swap = swap_ret.value();
            return;
        };

        void initInstance(Instance* instance) {
            vkb::InstanceBuilder builder;
            auto build_block = builder.set_app_name("easyvk");

            if (debug_enabled) {
                build_block.request_validation_layers(true)
                .use_default_debug_messenger();
            }

            auto inst_ret = build_block.build();            
            if (!inst_ret) printf("easyvk: failed to initiate vulkan instance");

            instance = new Instance();
            instance->instance = inst_ret.value();
            instance->table = inst_ret->make_table();
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

        void initDevice(vkb::PhysicalDevice phys, Device* device) {
            vkb::DeviceBuilder builder(phys);
            auto dev_ret = builder.build();
            if (!dev_ret) printf("easyvk: failed to initiate logical device\n");
            vkb::Device vkb_dev = dev_ret.value();
            device->logical = vkb_dev;
            device->phys = phys;
            device->table = vkb_dev.make_table();
        }

        void extractQueues(Device*dev) {
                // graphics
            auto queue = dev->logical.get_queue_and_index(vkb::QueueType::graphics);
            if (queue.has_value()) dev->graphics = std::make_pair(static_cast<vk::Queue>(queue.value().first), queue.value().second);
            else printf("easyvk: failed to extract graphics queue\n");
                // present
            queue = dev->logical.get_queue_and_index(vkb::QueueType::present);
            if (queue.has_value()) dev->present = std::make_pair(static_cast<vk::Queue>(queue.value().first), queue.value().second);
            else printf("easyvk: failed to extract present queue\n");
                // transfer
            queue = dev->logical.get_queue_and_index(vkb::QueueType::transfer);
            if (queue.has_value()) dev->transfer = std::make_pair(static_cast<vk::Queue>(queue.value().first), queue.value().second);
            else printf("easyvk: failed to extract transfer queue\n");
                // compute
            queue = dev->logical.get_queue_and_index(vkb::QueueType::compute);
            if (queue.has_value()) dev->compute = std::make_pair(static_cast<vk::Queue>(queue.value().first), queue.value().second);
            else printf("easyvk: failed to extract compute queue\n");
        }
    };
};