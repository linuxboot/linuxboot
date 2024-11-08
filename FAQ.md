Frequently Asked Questions about LinuxBoot
===

Why replace UEFI with LinuxBoot?
---
While Intel's edk2 tree that is the base of UEFI firmware is open source,
the firmware that vendors install on their machines is proprietary and
closed source.  Updates for bugs fixes or security vulnerabilities
are at the vendor's convienence; user specific enhancements are likely not
possible; and the code is not auditable.

UEFI is much more complex than the BIOS that it replaced.  It consists of
millions of lines of code and is an entire operating system,
with network device drivers, graphics, USB, TCP, https, etc, etc, etc.
All of these features represents increased "surface area" for attacks,
as well as unnecessary complexity in the boot process.

LinuxBoot replaces both UEFI as well as grub, which is has its own OS
worth of drivers, with the fairly well tested and trusted Linux kernel.
Self-help is possible if custom features are required or if security
vulnerabilities need to be patched, and the flexibility of the `initrd`
runtime allows users to tailor their "firmware" to their needs.


Why keep the vendor blobs?
---
LinuxBoot retains the vendor's PEI section of their UEFI firmware, which
is necessary for doing things like chipset initialization, memory controller
setup, CPU interconnect.  These functions are not documented by the CPU vendors,
so it is necessary to reuse the proprietary pieces -- even coreboot uses
the Intel provided FSP for these functions on modern laptops.

Additionally the Bootguard ACM, on systems that support it, must be signed by
Intel and as a result we are not able to replace that part of the early
startup code.


Where can I learn more about LinuxBoot?
---
* [LinuxBoot.org](https://linuxboot.org)
* [Ron Minnich's Open Source Summit presentation](https://www.youtube.com/watch?v=iffTJ1vPCSo)
* [Trammell Hudson's 34C3 presentation](https://trmm.net/LinuxBoot_34c3)
* [Install instructions for the R630](https://trmm.net/NERF/#installing-on-a-dell-r630)


What's wrong with UEFI Secure Boot?
---
Can't audit it, signing keys are controlled by vendors,
doesn't handle hand off in all cases, depends on possible leaked keys.


Why use Linux instead of vboot2?
---
vboot2 is part of the coreboot tree and is used by Google in the
Chromebook system to provide boot time security by verifying the
hashes on the coreboot payload.  This works well for the specialized
Chrome OS on the Chromebook, but is not as flexible as a measured
boot solution.


What about Trusted GRUB?
---
The mainline grub doesn't have support for TPM and signed kernels, but
there is a Trusted grub fork that does.  Due to philosophical differences
the code might not be merged into the mainline.  And due to problems
with secure boot (which Trusted Grub builds on), many distributions have
signed insecure kernels that bypass all of the protections secure
boot promised.

Additionally, grub is closer to UEFI in that it must have device
drivers for all the different boot devices, as well as filesystems.
This duplicates the code that exists in the Linux kernel and has its
own attack surface.

Using Linux as a boot loader allows us to restrict the signature
validation to keys that we control.  We also have one code base for the
device drivers in the Linux-as-a-boot-loader as well as Linux in the
operating system.


What is the concern with the Intel Management Engine?
---
"Rootkit in your chipset", "x86 considered harmful", etc.
The [ME cleaner](https://github.com/corna/me_cleaner) can be used to reduce
the size of the ME firmware, although we can't turn it off entirely.


How about the other embedded devices in the system?
---
#goodbios, funtenna, etc.


Should we be concerned about the binary blobs?
---
Maybe.  x230 has very few (MRC) since it has native vga init.


