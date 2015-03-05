#! /bin/sh

if [ $# -ne 1 ] ; then
    echo "Error: Must provide exactly one input! (submission version number)"
else
    echo "Creating subdirectory: ./submission/v"$1
    DEST=./submission/v$1
    mkdir $DEST

    echo "Adding header files to submission subdirectory"
    cp include/warlight-dev/* $DEST
    
    echo "Adding source files to submission subdirectory"
    cp src/* $DEST
    
    echo "Compressed submission subdirectory"
    zip -j $DEST/v$1.zip $DEST/*
    
    echo "Success: Submission subdirectory ready to be submitted"
fi
