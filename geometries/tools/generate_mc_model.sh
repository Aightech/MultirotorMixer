#! /bin/bash

if [ $# -lt 2 ]
  then
      echo "usage: ./generate_model.sh model_name model_id"
      exit
fi
MODEL_NAME=$1
MODEL_ID=$2
cd ../../../../../..
firmware_dir=`pwd`
cd -

echo "Dir : ${firmware_dir}"
echo "### PARAMETERS ###"
echo "model name: $MODEL_NAME"
echo "model ID: $MODEL_ID"
echo ""

# Create model directory and files
mkdir -p ${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME
(cat mc_param.erb && cat functions.erb && cat mc_model.config.erb) | erb -T 1 > ${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/model.config #model.config
echo "[INFO] model.config file created. (${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/model.config)"
(cat mc_param.erb && cat functions.erb && cat mc.sdf.erb) | erb -T 1 > ${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/$MODEL_NAME.sdf #file.sdf
echo "[INFO] model.sdf file created. (${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/$MODEL_NAME.sdf)"

#ensure the model is include in sitl_target
if grep -Fq "$MODEL_NAME" ${firmware_dir}/platforms/posix/cmake/sitl_target.cmake
then
    echo "[INFO] Model already included in ${firmware_dir}/platforms/posix/cmake/sitl_target.cmake)"
else
    sed "/set(models none /a \\\t$MODEL_NAME" ${firmware_dir}/platforms/posix/cmake/sitl_target.cmake
    echo "[INFO] Model included in ${firmware_dir}/platforms/posix/cmake/sitl_target.cmake)"
fi

# create init file
(cat mc_param.erb && cat functions.erb && cat mc_init.erb) | erb -T 1 > ${firmware_dir}/ROMFS/px4fmu_common/init.d-posix/${MODEL_ID}_$MODEL_NAME #init file
echo "[INFO] Init file created. (${firmware_dir}/ROMFS/px4fmu_common/init.d-posix/${MODEL_ID}_$MODEL_NAME)"

# create mixer file
(cat mc_param.erb && cat functions.erb && cat mc.main.mix.erb) | erb -T 1 > ${firmware_dir}/ROMFS/px4fmu_common/mixers/$MODEL_NAME.main.mix #mixer file
echo "[INFO] mixer file created. (${firmware_dir}/ROMFS/px4fmu_common/mixers/$MODEL_NAME.main.mix)"

# create geometry file
(cat mc_param.erb && cat functions.erb && cat mc.toml.erb) | erb -T 1 > ${firmware_dir}/src/lib/mixer/MultirotorMixer/geometries/$MODEL_NAME.toml #mixer file
echo "[INFO] geometry file created. (${firmware_dir}/src/lib/mixer/MultirotorMixer/geometries/$MODEL_NAME.toml)"
touch  ${firmware_dir}/src/lib/mixer/MultirotorMixer/CMakeList.txt



