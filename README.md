# branch-pred-fl
Branch Predictors
==========================================================================

This is the code repository for my branch predictor functional level model project. The repository contains 
- **functional level models** for a 2-bit saturating counter, a 1-level BHT which keeps track of local history, and a parametrized 2-level BHT which keeps track of local and global history
- **Pin**, Intel's just-in-time compiler used to instrument branches
- **unit tests** for all the functinal level models

### Instructions

#### Setup
You can set up the project by running the setup script in the top directory by using the following command:

```bash
$ source pred-setup.sh
```

This script will set up environment variables, build demo programs and the pintools. 

#### Using the Pintools to Instrument Branches

Once the setup script has been sourced, the pintools can be invoked by using the following command:

```bash
$ $PIN/pin -ifeellucky -t $PIN/source/tools/ManualExamples/obj-intel64/branch-tracker-demo.so -- <application-name>
```
where `\<application-name\>` is the name of the application you wish to instrument.

If you wish to rebuild the Pintools, use the following set of commands:

```bash
$ cd $PINTOOLS
$ make obj-intel64/branch-tracker-demo.so TARGET=intel64
```
