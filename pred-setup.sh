#!/bin/bash
echo "-- Setting environment variables...";
export TOPDIR=$PWD;
export PIN=$TOPDIR/pin-3.13-98189-g60a6ef199-gcc-linux;
export PINTOOLS=$PIN/source/tools/ManualExamples
echo "...done"
echo "-- Making build directory..."
mkdir -p $TOPDIR/build;
export BUILD=$TOPDIR/build;
echo "...done"
echo "-- Building demo programs..."
cd $BUILD;
gcc ../ubmark/single-unique.c -o single-unique -std=c99;
gcc ../ubmark/multi-unique.c -o multi-unique -std=c99;
echo "...done"
echo "-- Building pintools..."
cd $PINTOOLS;
make obj-intel64/branch-tracker-demo.so TARGET=intel64 >/dev/null;
make obj-intel64/branch-tracker.so TARGET=intel64 >/dev/null;
echo "...done"
cd $TOPDIR;
