# BTRFS Tree Dump

## What is it?
This is a tool / set of utilities for dumping (to various destinations) the b-tree
structure in an unmounted btrfs filesystem.

## Why is it?
A little bit of backstory is needed.  Dell, in the never ending wisdom, didn't include
a CMOS battery on the XPS-9570.  The regular battery supplies the voltage to persist
bios and other CMOS based configurations.  When the battery dies completely, CMOS is
cleared and all boot options are dropped.  When Dell's bios detects that CMOS has been
cleared, it finds the best course correction is to only register the Windows Bootloader
as a boot option in the BIOS.  There is a relatively simple fix for this: boot into a
live iso and reinstall GRUB in a chroot.

Having had to do this over a dozen times, I was going through the motions in autopilot.
And then... I forgot to unmount my main partition before booting into the os.

Through some combination of the hard poweroff (when the battery died), shutting down
without unmounting, and trying to run some package manager operations once in the os,
the btrfs partition became corrupted beyond recovery with `btrfs rescue` or `btrfs recover`.

This tool is my attempt to find the corruption and manually restore the file system enough
that `btrfs recover` can exfiltrate my data from the partition.

## Dependencies
This project currently requires the following packages

### Debian-based
```
* libbtrfs-dev (btrfs/ctree.h for btrfs metadata structures)
```

If you are unable to locate `libbtrfs-dev` on your distro, ctree.h can also be found in the
Linux kernel source tree at fs/btrfs/ctree.h.

## Building
This project uses [premake5](https://premake.github.io/) as its build generator.  
  
To build, run the following commands in the root directory of the project
```sh
premake5 gmake2
make
```
