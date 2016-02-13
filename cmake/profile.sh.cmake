#!/bin/sh

if [ -n "$LD_LIBRARY_PATH" ];then
    export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:@QT_LIBRARIES_PATH@"
else
    export LD_LIBRARY_PATH="@QT_LIBRARIES_PATH@"
fi
