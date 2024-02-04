#include "lis3dhtr_constants.hpp"
#include <libhal-mpu/lis3dhtr.hpp>
#include <libhal-util/bit.hpp>
#include <libhal-util/serial.hpp>
#include <libhal-util/steady_clock.hpp>
#include <libhal/serial.hpp>

namespace hal::mpu {
using namespace std::chrono_literals;
using namespace hal::literals;
result<lis3dhtr> lis3dhtr::create(hal::i2c& p_i2c,
                                  hal::serial& p_console,
                                  hal::steady_clock* clock,
                                  hal::byte p_device_address)
{
  lis3dhtr lis(p_i2c, p_device_address);
  HAL_CHECK(lis.power_on(p_console, clock));
  return lis;
}

lis3dhtr::lis3dhtr(hal::i2c& p_i2c, hal::byte p_device_address)
  : m_i2c(&p_i2c)
  , m_address(p_device_address)
{
}

hal::result<accelerometer::read_t> lis3dhtr::driver_read()
{
  accelerometer::read_t acceleration;
  constexpr auto number_of_axis = 3;
  constexpr auto bytes_per_axis = 2;
  auto xyz_acceleration =
    HAL_CHECK(hal::write_then_read<number_of_axis * bytes_per_axis>(
      *m_i2c,
      m_address,
      std::array{ hal::mpu::read_xyz_axis },
      hal::never_timeout()));
  // parsing data from accelerometer
  /**
   all data is left justified which means the data will always have the lowest
   nibble be 0 so we shift it all to the right by 4 and or the low and high
   bytes together 0000'0000'0000'0000
  */
  int16_t x = static_cast<uint16_t>(xyz_acceleration[0]) |
              ((static_cast<uint16_t>(xyz_acceleration[1])) << 8);
  int16_t y = static_cast<uint16_t>(xyz_acceleration[2]) |
              ((static_cast<uint16_t>(xyz_acceleration[3])) << 8);
  int16_t z = static_cast<uint16_t>(xyz_acceleration[4]) |
              ((static_cast<uint16_t>(xyz_acceleration[5])) << 8);

  constexpr float max = static_cast<float>(std::numeric_limits<int16_t>::max());
  constexpr float min = static_cast<float>(std::numeric_limits<int16_t>::min());

  float output_limits =
    static_cast<float>(1 << (static_cast<int16_t>(m_gscale) + 1));
  auto input_range = std::make_pair(max, min);
  auto output_range = std::make_pair(-output_limits, output_limits);

  acceleration.x = hal::map(x, input_range, output_range);
  acceleration.y = hal::map(y, input_range, output_range);
  acceleration.z = hal::map(z, input_range, output_range);

  return acceleration;
}

hal::status lis3dhtr::power_on(hal::serial& p_console, hal::steady_clock* clock)
{
  HAL_CHECK(configure_data_rates(data_rate_configs::mode_7, p_console, clock));
  return hal::success();
}

hal::status lis3dhtr::power_off(hal::serial& p_console,
                                hal::steady_clock* clock)
{
  HAL_CHECK(configure_data_rates(data_rate_configs::mode_0, p_console, clock));
  return hal::success();
}

hal::status lis3dhtr::configure_data_rates(data_rate_configs p_data_rate,
                                           hal::serial& p_console,
                                           hal::steady_clock* clock)
{

  constexpr auto configure_reg_bit_mask = hal::bit_mask::from<7, 4>();
  auto ctrl_reg1_array = std::array{ ctrl_reg1 };
  auto ctrl_reg1_data = HAL_CHECK(hal::write_then_read<1>(
    *m_i2c, m_address, ctrl_reg1_array, hal::never_timeout()));
  (void)hal::delay(*clock, 10ms);

  hal::bit_modify(ctrl_reg1_data[0])
    .insert<configure_reg_bit_mask>(static_cast<hal::byte>(p_data_rate));

  HAL_CHECK(hal::write(*m_i2c,
                       m_address,
                       std::array{ ctrl_reg1, ctrl_reg1_data[0] },
                       hal::never_timeout()));
  return hal::success();
}

hal::status lis3dhtr::configure_full_scale(max_acceleration p_gravity_code,
                                           hal::steady_clock* clock)
{

  m_gscale = static_cast<hal::byte>(p_gravity_code);

  constexpr auto configure_reg_bit_mask = hal::bit_mask::from<5, 4>();
  auto ctrl_reg4_array = std::array{ ctrl_reg4 };
  auto ctrl_reg4_data = HAL_CHECK(hal::write_then_read<1>(
    *m_i2c, m_address, ctrl_reg4_array, hal::never_timeout()));
  (void)hal::delay(*clock, 10ms);
  hal::bit_modify(ctrl_reg4_data[0])
    .insert<configure_reg_bit_mask>(static_cast<hal::byte>(p_gravity_code));

  HAL_CHECK(hal::write(*m_i2c,
                       m_address,
                       std::array{ ctrl_reg4, ctrl_reg4_data[0] },
                       hal::never_timeout()));
  return hal::success();
}

}  // namespace hal::mpu