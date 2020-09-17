#! /bin/bash

if [ $# -lt 3 ]
  then
      echo "usage: ./generate_model.sh model_name model_id model_parameters"
      exit
fi
MODEL_NAME=$1
MODEL_ID=$2
MODEL_PARAM=$3
cd ../../../../../../..
firmware_dir=`pwd`
cd -

echo "Dir : ${firmware_dir}"
echo "### PARAMETERS ###"
echo "model name: $MODEL_NAME"
echo "model ID: $MODEL_ID"
echo ""

# Create model directory and files
mkdir -p ${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME
(cat $MODEL_PARAM && cat functions.erb && cat indus_model.config.erb) | erb -T 1 > ${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/model.config #model.config
echo "[INFO] model.config file created. (${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/model.config)"
(cat $MODEL_PARAM && cat functions.erb && cat indus.sdf.erb) | erb -T 1 > ${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/$MODEL_NAME.sdf #file.sdf
echo "[INFO] model.sdf file created. (${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/$MODEL_NAME.sdf)"
if [ -d "confidential/meshes" ]; then
    cp -r confidential/meshes ${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/
    echo "[INFO] Confidential models have been copied in: (${firmware_dir}/Tools/sitl_gazebo/models/$MODEL_NAME/meshes)"
fi

#ensure the model is include in sitl_target
if grep -Fq "$MODEL_NAME" ${firmware_dir}/platforms/posix/cmake/sitl_target.cmake
then
    echo "[INFO] Model already included in ${firmware_dir}/platforms/posix/cmake/sitl_target.cmake)"
else
    sed -i "/set(models none /a \\\t$MODEL_NAME" ${firmware_dir}/platforms/posix/cmake/sitl_target.cmake
    echo "[INFO] Model included in ${firmware_dir}/platforms/posix/cmake/sitl_target.cmake)"
fi

#ensure the model is include in sitl_target
if grep -Fq "$MODEL_NAME" ${firmware_dir}/ROMFS/px4fmu_common/init.d-posix/CMakeLists.txt
then
    echo "[INFO] Model already included in ${firmware_dir}/ROMFS/px4fmu_common/init.d-posix/CMakeLists.txt"
else
    sed -i "/px4_add_romfs_files(/a \\\t${MODEL_ID}_$MODEL_NAME" ${firmware_dir}/ROMFS/px4fmu_common/init.d-posix/CMakeLists.txt
    echo "[INFO] Model included in ${firmware_dir}/ROMFS/px4fmu_common/init.d-posix/CMakeLists.txt"
fi

#ensure the model is include in mixers
if grep -Fq "$MODEL_NAME" ${firmware_dir}/ROMFS/px4fmu_common/mixers/CMakeLists.txt
then
    echo "[INFO] Model already included in ${firmware_dir}/ROMFS/px4fmu_common/mixers/CMakeLists.txt"
else
    sed -i "/px4_add_romfs_files(/a \\\t$$MODEL_NAME.main.mix" ${firmware_dir}/ROMFS/px4fmu_common/mixers/CMakeLists.txt
    echo "[INFO] Model included in ${firmware_dir}/ROMFS/px4fmu_common/mixers/CMakeLists.txt"
fi

# create init file
(cat $MODEL_PARAM && cat functions.erb && cat indus_init.erb) | erb -T 1 > ${firmware_dir}/ROMFS/px4fmu_common/init.d-posix/${MODEL_ID}_$MODEL_NAME #init file
echo "[INFO] Init file created. (${firmware_dir}/ROMFS/px4fmu_common/init.d-posix/${MODEL_ID}_$MODEL_NAME)"
(cat $MODEL_PARAM && cat functions.erb && cat indus_init.erb) | erb -T 1 > ${firmware_dir}/ROMFS/px4fmu_common/init.d/airframes/${MODEL_ID}_$MODEL_NAME #init file
echo "[INFO] Init file created. (${firmware_dir}/ROMFS/px4fmu_common/init.d/airfraimes/${MODEL_ID}_$MODEL_NAME)"

# create mixer file
(cat $MODEL_PARAM && cat functions.erb && cat indus.main.mix.erb) | erb -T 1 > ${firmware_dir}/ROMFS/px4fmu_common/mixers/$MODEL_NAME.main.mix #mixer file
echo "[INFO] mixer file created. (${firmware_dir}/ROMFS/px4fmu_common/mixers/$MODEL_NAME.main.mix)"

# create geometry file
#ensure the model is include in cmake file
if grep -Fq "$MODEL_NAME.toml" ${firmware_dir}/src/lib/mixer/MultirotorMixer/CMakeLists.txt
then
    echo "[INFO] Model already included in ${firmware_dir}/src/lib/mixer/MultirotorMixer/CMakeLists.txt"
else
    sed -i "/set(geometry_files/a \\\t$MODEL_NAME.toml" ${firmware_dir}/src/lib/mixer/MultirotorMixer/CMakeLists.txt
    echo "[INFO] Model included in ${firmware_dir}/src/lib/mixer/MultirotorMixer/CMakeLists.txt"
fi
(cat $MODEL_PARAM && cat functions.erb && cat indus.toml.erb) | erb -T 1 > ${firmware_dir}/src/lib/mixer/MultirotorMixer/geometries/$MODEL_NAME.toml #mixer file
echo "[INFO] geometry file created. (${firmware_dir}/src/lib/mixer/MultirotorMixer/geometries/$MODEL_NAME.toml)"
touch  ${firmware_dir}/src/lib/mixer/MultirotorMixer/CMakeLists.txt


#To enable the use of parameter in multirotors
cp CMakeLists_parameters.txt ${firmware_dir}/src/lib/parameters/CMakeLists.txt



