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
