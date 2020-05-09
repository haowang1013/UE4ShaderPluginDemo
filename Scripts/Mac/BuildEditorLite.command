#!/bin/bash

CURR_DIR=$(dirname "$0")

source $CURR_DIR/SetupEnvironment.sh
echo "Using engine path:" $ENGINE_PATH
echo "Using project name:" $PROJECT_NAME

if [ -d $ENGINE_PATH_FULL ]
then
    echo "Found UnrealEngine directory:" $ENGINE_PATH_FULL
else
    echo "Error: UnrealEngine is expected to be at:" $ENGINE_PATH_FULL
    exit 1
fi

cd $ENGINE_PATH_FULL/Engine/Build/BatchFiles/Mac
sh GenerateProjectFiles.sh -project=$PROJECT_PATH/$PROJECT_FILE -game -engine

cd $ENGINE_PATH_FULL
sh Engine/Build/BatchFiles/Mac/Build.sh $EDITOR_TARGET Mac Development $PROJECT_PATH/$PROJECT_FILE
