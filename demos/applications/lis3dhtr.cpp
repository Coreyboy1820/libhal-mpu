// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <libhal-mpu/lis3dhtr.hpp>
#include <libhal-mpu/mpu6050.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>

#include "../hardware_map.hpp"

hal::status application(hal::mpu::hardware_map& p_map)
{
  using namespace std::chrono_literals;
  using namespace hal::literals;

  auto& clock = *p_map.clock;
  auto& console = *p_map.console;
  auto& i2c = *p_map.i2c;
  hal::print(console, "lis Application Starting1...\n");
  (void)hal::delay(clock, 500ms);
  auto lis = HAL_CHECK(hal::mpu::lis3dhtr::create(
    i2c, console, &clock, hal::mpu::lis3dhtr::low_address));
  hal::print(console, "before main loop\n");
  HAL_CHECK(
    lis.configure_full_scale(hal::mpu::lis3dhtr::max_acceleration::g2, &clock));
  (void)hal::delay(clock, 500ms);
  while (true) {
    (void)hal::delay(clock, 500ms);
    HAL_CHECK(lis.read());
    auto acceleration = HAL_CHECK(lis.read());
    hal::print<128>(console,
                    "Scale: 2g \t x = %fg, y = %fg, z = %fg \n",
                    acceleration.x,
                    acceleration.y,
                    acceleration.z);
  }
  return hal::success();
}
