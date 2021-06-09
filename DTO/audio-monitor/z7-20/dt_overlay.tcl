set hdf miniproject_z7-20.hdf

set hw [hsi::open_hw_design $hdf]
hsi::set_repo_path ../../../device-tree-xlnx
hsi::create_sw_design sw1 -proc ps7_cortexa9_0 -os device_tree
hsi::set_property CONFIG.dt_overlay true [hsi::get_os]
hsi::generate_bsp -dir dtg_out
hsi::close_hw_design $hw