#include <libhal/units.hpp>

namespace hal::mpu {

// Device identification register
static constexpr hal::byte who_am_i_register = 0x0F;
// Used to set data rate selection,
// power mode, and z, y, and x axis toggling
static constexpr hal::byte ctrl_reg1 = 0x20;
// Used to reboot memory and toggle fifo
static constexpr hal::byte ctrl_reg4 = 0x23;
// Used to toggle fifo
static constexpr hal::byte ctrl_reg5 = 0x24;
// Used to change fifo modes
static constexpr hal::byte fifo_ctrl_reg = 0x2E;

static constexpr hal::byte read_xyz_axis = 0xA8;

// low and high bits of x accelerations data
static constexpr hal::byte out_x_l = 0x28;
static constexpr hal::byte out_x_h = 0x29;
// low and high bits of y accelerations data
static constexpr hal::byte out_y_l = 0x2A;
static constexpr hal::byte out_y_h = 0x2B;
// low and high bits of z accelerations data
static constexpr hal::byte out_z_l = 0x2C;
static constexpr hal::byte out_z_h = 0x2D;

}  // namespace hal::mpu
