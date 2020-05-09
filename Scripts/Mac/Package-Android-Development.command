#!/bin/bash

CURR_DIR=$(dirname "$0")

export Platform=Android
export ClientConfig=Development
export CookFlavor=ASTC

$CURR_DIR/DoPackageGame.command
