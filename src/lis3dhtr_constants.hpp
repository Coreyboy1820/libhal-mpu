#include <libhal/units.hpp>

namespace hal::mpu{

  enum class registers : hal::byte
  {
  // Device identification register
  who_am_i_register = 0x0F,
  // Used to set data rate selection,
  // power mode, and z, y, and x axis toggling
  ctrl_reg1 = 0x20,
  // Used to reboot memory and toggle fifo
  ctrl_reg4 = 0x23,
  // low and high bits of x accelerations data
  out_x_l = 0x28,
  out_x_h = 0x29,
  // low and high bits of y accelerations data
  out_y_l = 0x2A,
  out_y_h = 0x2B,
  // low and high bits of z accelerations data
  out_z_l = 0x2C,
  out_z_h = 0x2D,
  }
}