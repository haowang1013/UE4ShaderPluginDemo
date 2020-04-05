#!/bin/bash

CURR_DIR=$(dirname "$0")
export PROJECT_PATH=$CURR_DIR/../../

for f in $(ls $PROJECT_PATH/*.uproject); 
do export PROJECT_FILE=$(basename $f)
done

export PROJECT_NAME=${PROJECT_FILE%.*}
export EDITOR_TARGET=$PROJECT_NAME"Editor"

function readJson {  
  UNAMESTR=`uname`
  if [[ "$UNAMESTR" == 'Linux' ]]; then
    SED_EXTENDED='-r'
  elif [[ "$UNAMESTR" == 'Darwin' ]]; then
    SED_EXTENDED='-E'
  fi; 

  VALUE=`grep -m 1 "\"${2}\"" ${1} | sed ${SED_EXTENDED} 's/^ *//;s/.*: *"//;s/",?//'`

  if [ ! "$VALUE" ]; then
    echo "Error: Cannot find \"${2}\" in ${1}" >&2;
    exit 1;
  else
    echo $VALUE ;
  fi; 
}

export ENGINE_PATH=`readJson $PROJECT_PATH/$PROJECT_FILE EngineAssociation`
export ENGINE_PATH_FULL=$PROJECT_PATH$ENGINE_PATH
