#!/bin/bash

CURR_DIR=$(dirname "$0")

source $CURR_DIR/SetupEnvironment.sh

if [ -d $ENGINE_PATH_FULL ]
then
    echo "Found UnrealEngine directory:" $ENGINE_PATH_FULL
else
    echo "Error: UnrealEngine is expected to be at:" $ENGINE_PATH_FULL
    exit 1
fi

export ArchiveDirectory=$PROJECT_PATH/PackagedGame

cd $ENGINE_PATH_FULL/Engine/Build/BatchFiles
sh RunUAT.sh BuildCookRun -project=$PROJECT_PATH/$PROJECT_FILE -Platform=$Platform -ClientConfig=$ClientConfig -Cook -CookFlavor=$CookFlavor -Package -Compressed -UnversionedCookedContent -NoCompileEditor -build -stage -pak -archive -ArchiveDirectory=$ArchiveDirectory
