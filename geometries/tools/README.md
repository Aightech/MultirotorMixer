# ERB files
you can use those erb file to generate .toml and .sdf model for parametred multirotors

erb mc.sdf.erb > phexa.sdf

erb mc.toml.erb > phexa.toml


# Use new model + simu gazebo

 
- [ ] in Tools/sitl_gazebo/models/ create dir m-name 
    - [ ] model.config
    - [ ] m-name.sdf
    
- [ ] in ROMFS/px4fmu_common/init.d-posix/
    - [ ] IDnum_m-name  (don't forget the MAV_TYPE)
          refering to .mix (in ROMFS/px4fmu_common/mixers and rc. in ROMFS/px4fmu_common/init.d
- [ ] add to the platforms/posix/smake/sitl_target.cmake in set(models ... 
          
## add geometry 
- add .toml in src/lib/mixer/Multirotors/geometries/
- add .toml in the src/lib/mixer/Multirotors/cmakelist 
        
 
