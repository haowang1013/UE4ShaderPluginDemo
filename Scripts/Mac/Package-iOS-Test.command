#!/bin/bash

CURR_DIR=$(dirname "$0")

export Platform=IOS
export ClientConfig=Test

$CURR_DIR/DoPackageGame.command
