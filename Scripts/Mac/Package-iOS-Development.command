#!/bin/bash

CURR_DIR=$(dirname "$0")

export Platform=IOS
export ClientConfig=Development

$CURR_DIR/DoPackageGame.command
