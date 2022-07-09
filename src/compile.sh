#!/bin/bash

# If you are extremely based you can compile the project using this script (MSBuild)

PROJECT="src/NullHooks.sln"
RELEASE_CONFIG="-p:Configuration=Release"
DEBUG_CONFIG="-p:Configuration=Debug"
PLATFORM="-p:Plataform=x86"
REBUILD_PARAM=""

# First check if there are no arguments
if [[ $# == 0 ]]; then
    echo "No arguments detected! Use './compile.sh --help' to see the help!"
    echo "Exiting..."
    exit 1;
fi

# Check the commands that will only be executed once, in this case help
if [[ " $* " == *" help "* || " $* " == *" --help "* ]]; then
    echo "Showing help for compile.sh"
    echo "  ./compile.sh help            | Show this help"
    echo "  ./compile.sh release         | Compile src/NullHooks.sln as [Release | x86]"
    echo "  ./compile.sh debug           | Compile src/NullHooks.sln as [Debug | x86]"
    echo "  ./compile.sh <config> clean  | Compile src/NullHooks.sln with the rebuild parameter"
    exit 1;
fi


if [[ " $* " == *" clean "* ]]; then
     REBUILD_PARAM="-t:Rebuild"
fi

# Check config
if [[ " $* " == *" release "* ]]; then
    MSBuild.exe "$PROJECT" $REBUILD_PARAM $RELEASE_CONFIG $PLATFORM
elif [[ " $* " == *" debug "* ]]; then
    MSBuild.exe $PROJECT $REBUILD_PARAM $DEBUG_CONFIG $PLATFORM
fi
