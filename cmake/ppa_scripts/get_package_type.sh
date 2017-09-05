#!/bin/bash

echo "Is package to upload a svn snapshot or a release ?"
echo -n "Enter 1 for a svn snapshot or 2 for a release : "

read package_type

if [ "$package_type" == "2" ]
then
  echo -n release 1>&2
else
  echo -n snapshot 1>&2
fi
