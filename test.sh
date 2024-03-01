#! /bin/bash
# Test, that is compile and run a c++11 program
# *********************************************
# *            _                              *
# *           | | __ _ _ __   ___  ___        *
# *        _  | |/ _` | '_ \ / _ \/ __|       *
# *       | |_| | {_) | | | | ( ) \__ \       *
# *        \___/ \__,_|_| |_|\___/|___/       *
# *                                           *
# *********************************************
if [ "$1" == "" -o "$1" == "/?" -o "${1%%-*}" == "" -o "$1" == "--help" ]; then
  echo $
  echo $ Usage:
  echo $
  echo $ ./test.sh cpp_filename [more options] [-args options_for_the_executable] -- to compile and run
  echo $ ./test.sh [ --help ]                                                        -- for this help
  echo
  echo $ The [ ] square brackets denote the optionality!
  echo $
  echo $ Example for compiling and running hell.cpp with option --help:
  echo $  "./test.sh hell.cpp -args --help"
  echo $
  echo $ Example for compiling and running   hello.cpp:
  echo $  "./test.sh hello.cpp -std=c++23 -O3"
  echo $
  echo $ Example for compiling and debugging hello.cpp:
  echo $ "./test.sh hello -D VERBOSE=11 c++14 -g3"
  echo
  echo $ You can specify the:
  echo $  - language version by typing [-std=]c++11, c++14, ... c++23.
  echo $  - optimization level by typing -O0, -O1, -O2, or -O3.
  echo $  - debug level        by typing -g -g0 -g1, -g2, -g3, or gdb.
  echo $
elif [ -f "$1" -o -f "$1.cpp" ]; then
  # Use filepath for file name, and remove the first parameter from the argument list:
  filepath="$1";shift
  ## Extract the pathname from the file path
  #pathname=$(dirname  "$filepath")
  # Extract the filename with extension from the file path
  filename=$(basename "$filepath")
  # Extract the filename with path, but without extension
  pfn=$(echo "$filepath" | sed -E 's/([^.]+)\.?(.*)/\1/')
  # Extract the target filename without extension
  tn=$(echo "$filename" | sed -E 's/([^.]+)\.?(.*)/\1/')
  # Extract the extension from filename
  ext=$(echo "$filename" | sed -E 's/([^.]+)\.?(.*)/\2/')
  # if no extension is specified then use the .cpp extension:

  if [ -z $ext ]; then
      ext=.cpp;
  else
      # otherwise insert a dot before the extension:
      ext=.$ext;
  fi
  if [ -f $pfn$ext ]; then
    if [ ! -d "bin" ]; then mkdir -p bin; fi

    Language=c++11
    Args=""
	AppArgs=
    Opt=-O2
    Dln=
    Deb=

    # Populate Args and extract Language specification
    for Arg in "$@"; do
      if [ -n "$AppArgs" ]; then
        # collect the rermaining part for passing to the executable
        AppArgs=$(echo "$AppArgs" "$Arg")
        continue
      elif [ $Arg == -args ]; then
        # handle the -args option, initiate the collection
        AppArgs=" "
        continue
      fi
      opt_std="$Arg"
      # Leave out the leading "-std=" if it's there, and keep the rest
      # otherwise keep $opt_std unchanged
      opt_std=$(echo "$opt_std" | sed -E 's/^-std=(.*)/\1/')
      # if there was no change, then let's set $opt_std to "Default"
      if [ $opt_std == $Arg ]; then
        opt_std="Default"
      fi
      opt_cpv="$Arg"
      # Leave out the leading "c++" if it's there, and keep the following two digits
      opt_cpv=$(echo "$opt_cpv" | sed -E 's/^c[+]{2}([0-9]{2})/\1/')
      if [ $opt_cpv == $Arg ]; then
        opt_cpv="Default"
      fi
      if [ $opt_std != "Default" ]; then
        Language=$opt_std
      elif [ $opt_cpv != "Default" ]; then
        Language=c++$opt_cpv
      elif [ $Arg == gdb ]; then
        Opt=-O0
        Dln=-g2
        Deb=gdb
      elif [[ $Arg == -g3 || $Arg == -g2 || $Arg == -g1 || $Arg == -g0 || $Arg == -g ]]; then
        Opt=
        Dln=$Arg
        Deb=gdb
      elif [[ $Arg == -O3 || $Arg == -O2 || $Arg == -O1 || $Arg == -O0 ]]; then
        Opt=$Arg
      else
        Args=$(echo "$Args" "$Arg")
      fi
    done
    echo Compile and run "$pfn$ext" file using "$Language"...
    if [ -n "$Args" ]; then
      echo Additional options specified: "$Args"
    fi
    echo g++ -std=$Language $Opt $Dln -Wall -Wextra -Wpedantic -Werror -time "$Args" "$pfn$ext" -o "bin/$tn" \&\&  $Deb "./bin/$tn$AppArgs"
    g++ -std=$Language $Opt $Dln -Wall -Wextra -Wpedantic -Werror -time $Args $pfn$ext -o bin/$tn && $Deb ./bin/$tn$AppArgs
  else
      echo \"$pfn$ext file doesn\'t exist!\"
  fi
else
  if [ "${1##*.}" == "$1" ]; then
    echo \""$1.cpp" file doesn\'t exist!\"
  else
    echo \""$1" file doesn\'t exist!\"
  fi
fi
