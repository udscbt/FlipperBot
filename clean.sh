#!/bin/bash

echo -e "\e[1mCleaning using makefiles\e[0m"
tab="--"

clean ()
{
  for d in *
  do
    if [ -d $d ]
    then
      echo -e "${indent}Entering $d"
      cd $d
      if [ -e Makefile ]
      then
        echo -e "${indent}\e[1;92mMakefile found\e[91m"
        make clean 1> /dev/null
        echo -e "\e[0m"
      fi
      indent="$tab$indent"
      clean
      indent=${indent#$tab}
      cd ..
      echo -e "${indent}Exiting"
    fi
  done
}

indent=">"

clean

#################################

echo -e "\e[1mCleaning flipperbot folder\e[0m"
rm -R flipperbot/board/logs/*
mkdir flipperbot/board/logs/audio

echo -e "\e[1mCleaning python cache\e[0m"
tab="--"
clean2()
{
  for d in *
  do
    if [ -d $d ]
    then
      echo -e "${indent}Entering $d"
      cd $d
      if [ -e __pycache__ ]
      then
        echo -e "${indent}\e[1;92m__pycache__ found\e[91m"
        rm -R __pycache__
        echo -e "\e[0m"
      fi
      if [ -e *.pyc ]
      then
        echo -e "${indent}\e[1;92m*.pyc found\e[91m"
        rm *.pyc
        echo -e "\e[0m"
      fi
      indent="$tab$indent"
      clean2
      indent=${indent#$tab}
      cd ..
      echo -e "${indent}Exiting"
    fi
  done
}
indent=">"

clean2
