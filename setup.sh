#!/bin/bash
# Setup the configuration of platform and MOSEK

red='\033[0;31m'
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'
plain='\033[0m'

echo "# Configuration file for MOSEK and platform." > config.mk


echo "Creating 'build' directory..."
make_build=`mkdir build`
if [[ $? -eq 0 ]]; then
  echo -e "${green}[Succeed]${plain} 'build' directioy has been created"
else
  echo -e "${yellow}[Warning]${plain} Something wrong. "
  read -p "Press enter to continue."
fi

echo
echo "----------------------------------------"

echo "Setting up the configuration about MOSEK and platform"

if [[ $# -eq 1 ]]; then
  mskhome=$1
  echo -e "The path to MOSEK is set to ${blue}${mskhome}${plain}"
else
  echo -e "Please input your home folder of MOSEK ${blue}<MSKHOME>${plain}:"
  read mskhome
fi


echo "MSKHOME="${mskhome} >> config.mk


echo
echo "----------------------------------------"
echo "MOSEK versions in the given directory:"

versions=()
i=0
for dir in `ls ${mskhome}/mosek`
do 
  if [ -d ${mskhome}"/mosek/"$dir ]
  then
    let i++
    versions[$i]=$dir
    echo $i") "$dir
  fi
done
echo "----------------------------------------"

choose_version(){
  read -p "Found multiple versions, please choose one:" versionNum
  if [[ $versionNum -gt 0 && $versionNum -lt $i+1 ]]
  then
    mskversion=${versions[$versionNum]}
    echo -e "Selecting version: ${blue}${mskversion}${plain}"
    echo "MSKVERSION="${mskversion} >> config.mk
  else
    echo -e "${red}[Error]${plain} Input number is not valid, please try again."
    choose_version
  fi
}

if [ "$i" -eq "0" ]
then
  echo -e "${red}[Error]${plain} No MOSEK version is found"
  exit 1
elif [ "$i" -eq "1" ]
then
  mskversion=${versions[1]}
  echo -e "Selecting the only one version found: ${blue}${mskversion}${plain}"
  echo "MSKVERSION="${mskversion} >> config.mk
  read -p "Press enter to continue."
else
  choose_version
fi

echo

echo "----------------------------------------"
echo "Platforms folder in the MOSEK directory:"

platforms=()
j=0
for dir in `ls ${mskhome}/mosek/${mskversion}/tools/platform`
do 
  if [ -d ${mskhome}"/mosek/"${mskversion}"/tools/platform/"$dir ]
  then
    let j++
    platforms[$j]=$dir
    echo $j") "$dir
  fi
done
echo "----------------------------------------"

choose_platform(){
  read -p "Found multiple platforms, please choose one:" platformNum
  if [[ $platformNum -gt 0 && $platformNum -lt $j+1 ]]
  then
    platform=${platforms[$platformNum]}
    echo -e "Selecting platform: ${blue}${platform}${plain}"
    echo "MSKVERSION="${platform} >> config.mk
  else
    echo -e "${red}[Error]${plain} Input number is not valid, please try again."
    choose_platform
  fi
}
if [ "$j" -eq "0" ]
then
  echo -e "${red}[Error]${plain} No platform is found"
  exit 1
elif [ "$j" -eq "1" ]
then
  platform=${platforms[1]}
  echo -e "Selecting the only one version found: ${blue}${platform}${plain}"
  echo "PLATFORM="${platform} >> config.mk
  read -p "Press enter to continue."
else
  choose_platform
fi

echo
echo "----------------------------------------"
echo -e "${green}[Succeed]${plain} The MOSEK configuration has been saved as config.mk"
echo "========================================"


while true
do
  echo -e "Do you want compile the program now? [y/n]"
  read -p '(default: n):' compile_flag
  [ -z "$compile_flag" ] && compile_flag=n
  case "${compile_flag}" in
      y|Y|n|N)
      break
      ;;
      *)
      echo -e "${red}[Error]${plain} Please only enter [y/n]"
      ;;
  esac
done

if [ "${compile_flag}" == 'y' ] || [ "${compile_flag}" == 'Y' ]
then
  echo
  echo "Compiling..."
  echo
  make all
  if [[ $? -eq 0 ]]; then
    echo
    echo -e "${green}[Succeed]${plain} Program has been compiled."
    echo
  else
    echo
    echo -e "${red}[Error]${plain} Compiling failed."
    echo
  fi
  
else
  echo
  echo "You choose not to compile now."
  echo
fi


echo
echo -e "${blue}[Note]${plain} To compile again, please run 'make all' command."
echo
