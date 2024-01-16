#include "lis3dhtr_constants.hpp"
#include <libhal-mpu/lis3dhtr.hpp>

namespace hal::mpu {
    result<lis3dhtr> lis3dhtr::create(hal::i2c& p_i2c,
                                hal::byte p_device_address) {
        return lis3dhtr(&p_i2c, p_device_address)
    }

    lis3dhtr::lis3dhtr(hal::i2c& p_i2c,
                            hal::byte p_device_address) : m_i2c(&p_i2c), m_address(p_device_address) {
    }

    hal::result<accelerometer::read_t> lis3dhtr::driver_read() {
        HAL_CHECK(hal::write_then_read(*m_i2c,
                                m_address,
                                ));
    }

    // configuration functions 

    hal::status lis3dhtr::power_on() {
        HAL_CHECK(hal::write(*m_i2c, m_address, p_ctrl_reg1));
        return hal::success();
    }

    hal::status lis3dhtr::power_off() {
        HAL_CHECK(hal::write());
        return hal::success();
    }

    hal::status lis3dhtr::configure_data_rates(data_rate_config p_data_rate){
        HAL_CHECK(hal::write());
        return hal::success();
    }
} // namespace hal::mpu