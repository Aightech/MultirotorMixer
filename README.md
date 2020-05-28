# Install
## Ubuntu
cd ~/Download
wget https://raw.githubusercontent.com/PX4/Devguide/v1.9.0/build_scripts/ubuntu_sim_ros_melodic.sh
sudo chmod +x ubuntu_sim_ros_melodic.sh
./ubuntu_sim_ros_melodic.sh


### clone custom Multirotor Mixer
cd ~/src/Firmware/src/lib/mixer/
rm MultirotorMixer/* -rf
git clone git@github.com:Aightech/MultirotorMixer.git



# New config

## Gazebo 3D model
In Tools/sitl_gazebo/models/ create directory with the name of your model (ex:phexa) and add in this directory: 
- [ ] Config file (ex: model.config)
- [ ] SDF file describing the 3D model (ex: phexa.sdf). You can use the erb LINK file to generate a parametrized multirotor easily

In platforms/posix/cmake/sitl_target.cmake add your model to the list starting with set(models ... 

## Init files
In ROMFS/px4fmu_common/init.d-posix/ create a file with the name of your model suffixed with an unused ID (ex: 10040_phexa):
- [ ] Specify the init.d file (ex: sh /etc/init.d/rc.mc_defaults )
- [ ] Specify the MAV_TYPE of the model (ex: set MAV_TYPE 13) 2:quad 13:hexa (impact the way output/pwm are processed)
- [ ] Specify the mixer (ROMFS/px4fmu_common/mixer/*.main.mix files) it will use (ex: set MIXER phexa)

## Mixer files
In ROMFS/px4fmu_common/mixers/ create a file with the name of your mixer and the extension .main.mix (ex: phexa.main.mix):
- [ ] Specify the geometry (/src/lib/mixer/MultirotorMixer/geometries/.toml) it will use (ex 6xp).

## Geometry files
In /src/lib/mixer/MultirotorMixer/geometries/ create a file with the name of your geometry and the extension .toml (ex: phexa.toml):
- [ ] Specify the key of your geometry (the one specified in the mixer file) (ex: key = "6xp")
- [ ] Specify the geometry of your different rotors.

In /src/lib/mixer/MultirotorMixer/CMakeLists.txt add your geometry to the list starting with set(geometry_files ...


# ERB files
You can use those erb files to generate .toml and .sdf model for parametred multirotors

erb mc.sdf.erb > phexa.sdf

erb mc.toml.erb > phexa.toml

