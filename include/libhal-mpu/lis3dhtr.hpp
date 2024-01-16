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

#pragma once

#include <libhal-util/bit.hpp>
#include <libhal-util/i2c.hpp>
#include <libhal-util/map.hpp>
#include <libhal/accelerometer.hpp>

namespace hal::mpu {
class lis3dhtr : public hal::accelerometer
{

  // send slave address first, then sub address is sent
  // the 7 lsb's are the actual address, while the MSb 
  // indicates autoincrement of the address
public:
  /// The device address when SDO/SA0 is connected to GND.
  static constexpr hal::byte low_address = 0b0000'1100;
  /// The device address when SDO/SA0 is connected to 3v3.
  static constexpr hal::byte high_address = 0b0000'1101;

  /// @brief power_mode_config is used to set the data rates of the
  /// high operating, normal operating, and low operating frequency modes
  enum class data_rate_config : hal::byte
  {
    // the following set all modes to the same data rates listed
    mode_0 = 0b0001'0000;
    // 10Hz
    mode_1 = 0b0010'0000,
    // 25Hz
    mode_2 = 0b0011'0000,
    // 50Hz
    mode_3 = 0b0100'0000,
    // 100Hz
    mode_4 = 0b0101'0000,
    // 200Hz
    mode_5 = 0b0110'0000,
    // 400Hz
    mode_6 = 0b0111'0000,
    // just low power mode is configured in this one to 1.6kHz
    // this is also the default mode set by power_on
    mode_7 = 0b1000'0000,
    // High resolution = normal = 1.344kHz; low power mode = 5.376kHz
    mode_8 = 0b1001'0000,  
  };

  /**
   * @brief Constructs and returns lis object
   *
   * @param p_i2c - I2C bus the lis is connected to
   * @param p_device_address - address of the lis3dhtr
   * @return lis3dhtr object
   * @throws std::errc::invalid_byte_sequence - when ID register does not match
   * the expected ID for the lis3dhtr device.
   */
  static result<lis3dhtr> create(hal::i2c& p_i2c,
                                hal::byte p_device_address = low_address);

  /**
   * @brief Re-enables acceleration readings from the lis
   *
   * @return hal::status - success or errors from i2c communication
   * @throws std::errc::invalid_byte_sequence - when ID register does not match
   * the expected ID for the lis3dhtr device.
   */
  [[nodiscard]] hal::status power_on();

  /**
   * @brief Disables acceleration reading from the lis3dhtr.
   *
   * @return hal::status - success or errors from i2c communication
   */
  [[nodiscard]] hal::status power_off();

  [[nodiscard]] hal::status configure_data_rates(data_rate_config p_data_rate);

private:
  /**
   * @brief lis3dhtr Constructor
   *
   * @param p_i2c - i2c peripheral used to commnicate with device.
   * @param p_address - lis3dhtr device address.
   */
  lis3dhtr(i2c& p_i2c, hal::byte p_device_address);

  hal::result<accelerometer::read_t> driver_read() override;

  /// The I2C peripheral used for communication with the device.
  hal::i2c* m_i2c;
  /// The configurable device address used for communication.
  hal::byte m_address;
  /// this will hold the value of the ctrl_reg1 to eliminate having to save time reading the current state of the register before updating it.
  hal::byte m_ctrl_reg1 = 0b0000'0111;
};

}  // namespace hal::mpu
