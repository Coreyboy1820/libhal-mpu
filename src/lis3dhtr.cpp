#include "lis3dhtr_constants.hpp"
#include <libhal-mpu/lis3dhtr.hpp>
#include <libhal-util/bit.hpp>

namespace hal::mpu {
    result<lis3dhtr> lis3dhtr::create(hal::i2c& p_i2c,
                                hal::byte p_device_address) {
        lis lis3dhtr(&p_i2c, p_device_address);
        HAL_CHECK(lis.power_on());
        return lis;
    }

    lis3dhtr::lis3dhtr(hal::i2c& p_i2c,
                            hal::byte p_device_address) : m_i2c(&p_i2c), m_address(p_device_address) {
    }

    hal::result<accelerometer::read_t> lis3dhtr::driver_read() {
        accelerometer::read_t acceleration;
        constexpr auto number_of_axis = 3;
        constexpr auto bytes_per_axis = 2;
        auto xyz_acceleration = HAL_CHECK(hal::write_then_read<number_of_axis * bytes_per_axis>(*m_i2c,
                                m_address,
                                std::array{out_x_l, out_x_h, out_y_l, out_y_h, out_z_l, out_z_h},
                                hal::never_timeout()));
        // parsing data from accelerometer
        /** 
         all data is left justified which means the data will always have the lowest nibble be 0 so we shift it all to the right by 4 and or the 
         low and high bytes together
         0000'0000'0000'0000
        */
       acceleration.x = static_cast<int16_t>((xyz_acceleration[0] >> 4) | (static_cast<int16_t>(xyz_acceleration[1]) << 4));
       acceleration.y = static_cast<int16_t>((xyz_acceleration[2] >> 4) | (static_cast<int16_t>(xyz_acceleration[3]) << 4));
       acceleration.z = static_cast<int16_t>((xyz_acceleration[4] >> 4) | (static_cast<int16_t>(xyz_acceleration[5]) << 4));
       return acceleration;
    }

    // configuration functions 

    hal::status lis3dhtr::power_on() {
        HAL_CHECK(configure_data_rates(data_rate_configs::mode_8));
        return hal::success();
    }

    hal::status lis3dhtr::power_off() {
        HAL_CHECK(configure_data_rates(data_rate_configs::mode_0));
        return hal::success();
    }

    hal::status lis3dhtr::configure_data_rates(data_rate_configs p_data_rate){

        auto configure_reg_bit_mask = hal::bit_mask::from<3, 0>();

        auto ctrl_reg1_data = HAL_CHECK(hal::write_then_read<1>(*m_i2c,
                                m_address,
                                std::array{ctrl_reg1},
                                hal::never_timeout()));

        hal::bit_modify(ctrl_reg1_data[0]).insert<configure_reg_bit_mask>(p_data_rate);
        
        HAL_CHECK(hal::write(*m_i2c, m_address, 
                    std::array{ctrl_reg1, ctrl_reg1_data[0]}, 
                    hal::never_timeout()));
        return hal::success();
    }


} // namespace hal::mpu