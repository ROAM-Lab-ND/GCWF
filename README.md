# GCWF: Geometric Contact Wrench Feasibility
[![standard-readme compliant](https://img.shields.io/badge/RA--L-Reviewing-blue.svg?style=flat)](https://www.ieee-ras.org/publications/ra-l)

<!-- Verifying Contact Wrench Feasibility via Geometrical Approach. -->

Example code of \
_A Geometric Sufficient Condition for Contact Wrench Feasibility_\
Shenggao Li, Hua Chen, Wei Zhang, Patrick M. Wensing

## Background

In our paper, we introduced a geometry-based framework to approximate the exact Contact Wrench Cone (CWC) including two approaches with different accuracy-speed balance. By comparing to the exact CWC solved by [MOSEK][261d468d], the preferred approach 2 can find the yaw moment interval with **only 7.13% error but 140 times faster**. 

  [261d468d]: https://www.mosek.com "MOSEK"



## Dependencies and Requirements
- OS: MacOSX or Linux (x86/x64)
  - Tested on OSX Catalina 10.15 and Ubuntu 18.04
> TODO: Add support for OSX and Linux with ARM CPU.
- [MOSEK][261d468d] 9.3 - For solving the Second-Order-Cone-Program of exact CWC.
  - Please follow the [installation guide][24aab043] and make sure your license is properly configured as explained in [license guide][833fb562].
  - Our code is developed based on MOSEK 9.3. It should be compatible with newer 10.0 version but has not been tested (due to our license restriction).
  - Note: the folder `<MSKHOME>` in which the MOSEK Optimization Suite has been installed would be used in later compiling procedure.
- [MATLAB][4b980ec4] 2019a or later - For sample data generation and computing statistical result. 



  [4b980ec4]: https://matlab.mathworks.com "Matlab"
  [24aab043]: https://docs.mosek.com/9.3/cxxfusion/install-interface.html# "MOSEK-INSTALL"
  [833fb562]: https://docs.mosek.com/9.3/licensing/index.html "MOSEK-LICENSE"

## Install
Download this project
```
git clone https://github.com/ROAM-Lab-ND/GCWF.git
cd GCWF
```
Run the setup script and follow the instruction to configure the MOSEK library and compile this project. `<MSKHOME>` contains your MOSEK library like `<MSKHOME>/mosek/9.3/tools/...`
```
./setup.sh <MSKHOME>
```
##### Available Makefile commands
1. `make clean` to remove all compiling and executable files of this project (but keeps MOSEK library).
2. `make clean_MOSEK` to clean the MOSEK library.
3. `make all` to compile all (both MOSEK library and this project).


## Run Example 
Run the script `SampleAndCompute.m` in MATLAB.
