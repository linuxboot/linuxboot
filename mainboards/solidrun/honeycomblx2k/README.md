Using this mainboard with the solidrun.

This assumes you are running a reasonably new LSDK, if not, build one and put it on an SD.

You MUST have go1.17.

```
make fetch
make
```

Put the netbootkernel and the .dtb somewhere for ftp.

On the honeycomb:
```
setenv netboot 'mmc read 0x80d00000 0x6800 0x800; dhcp; tftpboot ${kernel_addr_r} netbootkernel; tftpboot ${fdt_addr_r} fsl-lx2160a-cex7.dtb; fsl_mc apply DPL 0x80d00000; bootefi ${kernel_addr_r} ${fdt_addr_r}'
saveenv
```

Let the honeycomb boot to the 
```
Hit any key to stop autoboot:
```

and hit any key.

run netboot

The kernel will boot. You will need to get an IP address:
```
dhclient -vv &
```

This takes a few minutes for some reason.

Then:
```
cpud -init &
```
And you can cpu to the board!

Note: you will need an ssh public key, I used ~/.ssh/cpu_rsa.pub

You can make getting to the board more convenient, e.g, in my .ssh/config I have this:
```
Host honeycomb
	HostName honeycomb
	Port 23
	User root
	IdentityFile ~/.ssh/cpu_rsa
```

