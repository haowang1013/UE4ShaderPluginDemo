#!/bin/bash

CURR_DIR=$(dirname "$0")

export Platform=IOS
export ClientConfig=Debug

$CURR_DIR/DoPackageGame.command
