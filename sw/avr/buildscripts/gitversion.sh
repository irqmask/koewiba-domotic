#!/bin/bash

set -e

outdir="$1"

version_major="0"
version_minor="0"
version_bugfix="0"
modified_flag="0"

timestamp=$(date --iso-8601=seconds)
buildtime=$(date --date=$timestamp +%T)
builddate=$(date --date=$timestamp +%Y-%m-%d)

ismodified=$(git status --porcelain)
commitshorthash=$(git log -n1 --pretty=format:%h)
commitfullhash=$(git log -n1 --pretty=format:%H)
modifiedFiles=$(git status --porcelain)

tag=$(git describe --long)
if [ $? -eq 0 ]; then
    tag=${tag/v/}   # remove leading 'v'
    tag=${tag//-/.} # turn '-' into '.'
    version_major=$(echo "$tag" | cut -d'.' -f1)
    version_minor=$(echo "$tag" | cut -d'.' -f2)
    version_bugfix=$(echo "$tag" | cut -d'.' -f3)
fi

echo "build date $builddate time $buildtime"
echo "version: $version_major.$version_minor.$version_bugfix #$commitshorthash"
if [ -n "$modifiedFiles" ]; then
    echo "local modified files detected. setting modied flag!"
    modified_flag="1"
fi

# generate the C header file
rm -f "$outdir/version.h"
cat << EOF > "$outdir/version.h"
/**
 * This file contains the version information for the KWB project.
 * @note This file is automatically generated! Do not change anything in here!
 *
 * @file version.h
 */

#ifndef _VERSION_H_
#define _VERSION_H_

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

//! Major version number of the KWB package.
#define VERSION_MAJOR $version_major

//! Minor version number of the KWB package.
#define VERSION_MINOR $version_minor

//! Bugfix version number of the KWB package.
#define VERSION_BUGFIX $version_bugfix

//! Version hash of the KWB package.
#define VERSION_HASH 0x$commitshorthash

//! Repository is locally modified
#define VERSION_LOCALLY_MODIFIED $modified_flag

//! Version string representing the version major.minor.bugfix-commit revision-development stage.
#define VERSION_PRODUCT STRINGIZE(VERSION_MAJOR) "." STRINGIZE(VERSION_MINOR) "." STRINGIZE(VERSION_BUGFIX) "-#$commitshorthash"
#endif // _VERSION_H_

EOF
