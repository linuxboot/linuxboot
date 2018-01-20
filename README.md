# linuxboot
The LinuxBoot project is working to enable Linux to replace your firmware on all platforms.


Extracting vendor ROM
===
For the Intel S2600WF we need quite a few files from the vendor ROM right now.
Using `UEFITool` extract the recovery image firmware volume
`CDBB7B35-6833-4ED6-9AB2-57D2ACDDF6F0` into `blobs/s2600wf/s2600wf-dxe.vol`
and then run:

```
uefi-firmware-parser --extract s2600wf-dxe.vol
```

This will create `blobs/s2600wf/volume-0/file-*`, which will be merged
into our firmware volume during the build.
