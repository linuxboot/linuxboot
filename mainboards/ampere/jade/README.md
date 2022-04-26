# Ampere Mt Jade (WiWynn) LinuxBoot Altra / Altra Max

# LinuxBoot + Plan9 via u-root/cpu

## Documentation

LinuxBoot [documention](https://book.linuxboot.org/cpu/) of the Plan9 cpu/cpud commands implemented in [u-root/cpu](https://github.com/u-root/cpu)

# Plan9 u-root/cpu Benefits

This tool allows you to have a very small kernel in SPI-NOR and use ssh with 9P remote filesystems on a remote Arm64 system to have full access to any Arm64 tools without installing them locally. In the example below any tool that is installed on the Raspberry Pi will be availble to Mt Jade once the cpu to cpud connection is made.

- Full access to remote filesystem
- Maintain minimal SPI-NOR usage
- Firmware and OS bringup
- Baseboard bring-up and debugging
- Security testing e.g. [chipsec](https://github.com/chipsec/chipsec)
- Factory diagnostics and manufacturing flow

## LinuxBoot Docker Example

There is an example project that will build LinuxBoot with 9P enabled and systemboot as the default for Mt Jade [here](https://gitlab.com/fryard/Ampere-Samples/-/tree/main/examples/edk2)

## Plan9 Client cpu

- An aarch64/Arm64 system (*e.g. Mt Jade, Cloud instance, QEMU, Raspberri Pi, etc.*)

    Option **A**: [Raspberry Pi 3 UEFI Firmware Images](https://github.com/pftf/RPi3) forked from [here](https://github.com/tianocore/edk2-platforms/tree/master/Platform/RaspberryPi/RPi3) with OS installation instructions [here](https://pete.akeo.ie/2019/07/installing-debian-arm64-on-raspberry-pi.html)

    Option **B**: Use one of the [prebuilt RasPiOS Arm64 images](https://downloads.raspberrypi.org/raspios_arm64/images/raspios_arm64-2022-04-07/) This is the option used for these examples.

        You'll need to copy or create ssh keys once the OS is up.

- Install Golang and uroot/cpu for Arm64

    Create a directory to install Golang for Arm64.

    `sudo su && mkdir -p /usr/local`

    The package manager version is likely outdated and some compatibility things changed around version 1.17 so wget is the best way to install for now.

    `cd /usr/local/ && wget -q -O - https://go.dev/dl/go1.17.4.linux-arm64.tar.gz | tar -xzi`

    Set your GOPATH when you login.

    `echo "export GOPATH=/home/pi/go" >> ~/.bashrc`

    This is an older Golang problem but shouldn't hurt for new installations.

    `echo "export GO111MODULE=off" >> ~/.bashrc`

    Update your PATH to include Golang and GOPATH.

    `echo "export PATH=/usr/local/go/bin:$GOPATH/bin:$PATH" >> ~/.bashrc`

    Logout and login again to reread your .bashrc to get the GO variables

    Create your GOPATH where u-root/cpu and others will be installed.

    `mkdir $GOPATH`

    Install the u-root/cpu package.

    `go get -u github.com/u-root/cpu/...`

- SSH client private key (.ssh/identity)

    If you want cpu to use defaults then you need to copy your ssh key to ~/.ssh/identity. If you'd rather specify the key then use the `-key string` option when starting cpu.

    `cat ~/.ssh/id_rsa > ~/.ssh/identity`

- Testing the client

    After completing the steps for the cpud server below you can test a simple command to return the date. If you add dbg9p to the cpu command _**cpu -dbg9p 192.168.0.192 date**_ you'll see logs of debug output.

    `cpu 192.168.0.192 date`

___
CLIENT
___

    ```
    1970/01/01 00:02:56 
    CPUD:Args [cpud -d -remote -bin cpud -port9p 46593 date] pid 692 *runasinit false *remote true
    1970/01/01 00:02:56 
    CPUD:Not server package: Running as remote: args ["date"], port9p 46593
    1970/01/01 00:02:56 CPUD:namespace is "/lib:/lib64:/usr:/bin:/etc:/home"
    1970/01/01 00:02:56 CPUD:Dial 127.0.0.1:46593
    1970/01/01 00:02:56 CPUD:Connected: write nonce 1b8dc12cfb4831fac97961fa55bcaf39
    1970/01/01 00:02:56 CPUD:Wrote the nonce
    1970/01/01 00:02:56 CPUD:fd is 10
    1970/01/01 00:02:56 CPUD: mount 127.0.0.1 on /tmp/cpu 9p 0x6 version=9p2000.L,trans=fd,rfdno=10,wfdno=10,uname=pi,debug=0,msize=1048576
    1970/01/01 00:02:56 CPUD: mount done
    1970/01/01 00:02:56 CPUD: mount /tmp/cpu/lib over /lib
    1970/01/01 00:02:56 CPUD:Mounted /tmp/cpu/lib on /lib
    1970/01/01 00:02:56 CPUD: mount /tmp/cpu/lib64 over /lib64
    1970/01/01 00:02:56 CPUD:Warning: mounting /tmp/cpu/lib64 on /lib64 failed: no such file or directory
    1970/01/01 00:02:56 CPUD: mount /tmp/cpu/usr over /usr
    1970/01/01 00:02:56 CPUD:Mounted /tmp/cpu/usr on /usr
    1970/01/01 00:02:56 CPUD: mount /tmp/cpu/bin over /bin
    1970/01/01 00:02:56 CPUD:Mounted /tmp/cpu/bin on /bin
    1970/01/01 00:02:56 CPUD: mount /tmp/cpu/etc over /etc
    1970/01/01 00:02:56 CPUD:Mounted /tmp/cpu/etc on /etc
    1970/01/01 00:02:56 CPUD: mount /tmp/cpu/home over /home
    1970/01/01 00:02:56 CPUD:Mounted /tmp/cpu/home on /home
    1970/01/01 00:02:56 CPUD: bind mounts done
    1970/01/01 00:02:56 CPUD:dropPrives: uid is 0
    1970/01/01 00:02:56 CPUD:dropPrivs: not dropping privs
    1970/01/01 00:02:56 CPUD:runRemote: command is "date"
    ```

```
Wed 31 Dec 1969 06:02:56 PM CST
```

```
1970/01/01 00:02:56 CPUD:Run /usr/bin/date returns <nil>
1970/01/01 00:02:56 CPUD: ignoring err <nil>
```

Establish an ssh connection to Mt Jade running cpud from the Raspberry Pi using cpu.

`pi@rpi3debug:~ $ cpu 192.168.0.192`

```
1970/01/01 00:11:43 CPUD:runRemote: command is "/bin/bash"
```

Run any command available on the Raspberry Pi but executing on Mt Jade using the 9P remoted mounted filesystem.

`root@(none):~ # uname -a`

```
Linux (none) 5.7.0 #1 SMP Fri Apr 22 15:57:28 CDT 2022 aarch64 GNU/Linux
```

`root@(none):~ # date`

```
Wed 31 Dec 1969 06:12:03 PM CST
```

Verify we are executing on an 80 core Ampere Altra Q80-30 CPU

`root@(none):~ # lscpu`

```
Architecture:                    aarch64
CPU op-mode(s):                  32-bit, 64-bit
Byte Order:                      Little Endian
CPU(s):                          80
On-line CPU(s) list:             0-79
Thread(s) per core:              1
Core(s) per socket:              80
Socket(s):                       1
Vendor ID:                       ARM
Model:                           1
Model name:                      Neoverse-N1
Stepping:                        r3p1
BogoMIPS:                        50.00
L1d cache:                       5 MiB
L1i cache:                       5 MiB
L2 cache:                        80 MiB
Vulnerability Itlb multihit:     Not affected
Vulnerability L1tf:              Not affected
Vulnerability Mds:               Not affected
Vulnerability Meltdown:          Not affected
Vulnerability Spec store bypass: Mitigation; Speculative Store Bypass disabled via prctl
Vulnerability Spectre v1:        Mitigation; __user pointer sanitization
Vulnerability Spectre v2:        Not affected
Vulnerability Tsx async abort:   Not affected
Flags:                           fp asimd aes pmull sha1 sha2 crc32 atomics fphp asimdhp cpuid asimdrdm lrcpc
                                  dcpop asimddp ssbs
```

___
SERVER
___

`cpud -init -d`

    ```
    1970/01/01 00:02:15 
    CPUD:Args [/bbin/cpud -init -d] pid 669 *runasinit true *remote false
    1970/01/01 00:02:15 
    CPUD:Running as Init
    1970/01/01 00:02:15 
    CPUD:Kicked off startup jobs, now serve ssh
    1970/01/01 00:02:15 
    CPUD:Start the process reaper
    1970/01/01 00:02:15 CPUD:starting ssh server on port 23
    1970/01/01 00:02:15 CPUD: All startup jobs exited
    1970/01/01 00:02:15 CPUD: Syncing filesystems
    1970/01/01 00:02:46 CPUD:attempt to bind 127.0.0.1 0 granted
    1970/01/01 00:02:46 CPUD:handler: cmd is ["cpud" "-d" "-remote" "-bin" "cpud" "-port9p" "45675" "date"]
    1970/01/01 00:02:46 command started with pty
    1970/01/01 00:02:47 wait for /bbin/cpud -d -remote -bin cpud -port9p 45675 date
    1970/01/01 00:02:47 cmd "/bbin/cpud -d -remote -bin cpud -port9p 45675 date" returns with <nil> exit status 0
    1970/01/01 00:02:47 
    CPUD:handler exits
    1970/01/01 00:02:56 CPUD:attempt to bind 127.0.0.1 0 granted
    1970/01/01 00:02:56 CPUD:handler: cmd is ["cpud" "-d" "-remote" "-bin" "cpud" "-port9p" "46593" "date"]
    1970/01/01 00:02:56 command started with pty
    1970/01/01 00:02:56 wait for /bbin/cpud -d -remote -bin cpud -port9p 46593 date
    1970/01/01 00:02:56 cmd "/bbin/cpud -d -remote -bin cpud -port9p 46593 date" returns with <nil> exit status 0
    1970/01/01 00:02:56 
    CPUD:handler exits
    ```

## Plan8 Server cpud (LinuxBoot Mt Jade)

- Booting Mt Jade

    After flashing your LinuxBoot image boot Mt Jade and either connect to the UEFI UART with a TTL cable or use ipmitool with the serial-over-lan (SOL) option.

    `ipmitool -I lanplus -H 192.168.0.156 -U admin -P admin sol activate`

- Interrupt systemboot within 5 seconds using CTRL+C

    There are also kernel options that allow you to configure the network and run cpud instead of systemboot. Look at the Makefile in this project to generate a kernel that automatically initializes the network and runs cpud as init.

    ```
                         ____            _                 _                 _
                        / ___| _   _ ___| |_ ___ _ __ ___ | |__   ___   ___ | |_
                        \___ \| | | / __| __/ _ \ '_ ` _ \| '_ \ / _ \ / _ \| __|
                         ___) | |_| \__ \ ||  __/ | | | | | |_) | (_) | (_) | |_
                        |____/ \__, |___/\__\___|_| |_| |_|_.__/ \___/ \___/ \__|
                                |___/
    1970/01/01 00:00:21 Watchdog is stopped.
    1970/01/01 00:00:21 VPD bmc_bootorder_override is 
    1970/01/01 00:00:21 System firmware version: TianoCore 2.06.100 (SYS: 2.06.20220308)
    1970/01/01 00:00:21 Failed to set system firmware version to BMC invalid response, expected first byte of response to be 0, got: 204.
    1970/01/01 00:00:21 No product name is matched for OEM commands.
    1970/01/01 00:00:21 **************************************************************************
    1970/01/01 00:00:21 Starting boot sequence, press CTRL-C within 5 seconds to drop into a shell
    1970/01/01 00:00:21 **************************************************************************

    ^CException: systemboot killed by signal interrupt
    [tty 1], line 1: systemboot
    /#                                                                   root@(none)
    ```

- Network setup

    Configure the network by using DHCP or static assignment.
    
    **Dynamic Address**
    
    `dhclient`

    ```
    1970/01/01 00:05:41 Bringing up interface eth0...
    1970/01/01 00:05:41 Bringing up interface eth1...
    1970/01/01 00:05:42 Attempting to get DHCPv4 lease on eth1
    1970/01/01 00:05:45 Got DHCPv4 lease on eth1: DHCPv4 Message
    opcode: BootReply
    hwtype: Ethernet
    hopcount: 0
    transaction ID: 0x79187472
    num seconds: 0
    flags: Unicast (0x00)
    client IP: 0.0.0.0
    your IP: 192.168.0.192
    server IP: 192.168.0.1
    gateway IP: 0.0.0.0
    client MAC: 0c:42:a1:5a:8a:9d
    server hostname: 
    bootfile name: 
    options:
        Subnet Mask: ffffff00
        Router: 192.168.0.1
        Domain Name Server: 192.168.0.11, 1.1.1.1
        Domain Name: lan
        Broadcast Address: 192.168.0.255
        IP Addresses Lease Time: 12h0m0s
        DHCP Message Type: ACK
        Server Identifier: 192.168.0.1
        Renew Time Value: [0 0 84 96]
        Rebinding Time Value: [0 0 147 168]
    1970/01/01 00:05:45 Configured eth1 with IPv4 DHCP Lease IP 192.168.0.192/24
    1970/01/01 00:06:11 Could not bring up interface eth0: link "eth0" still down after 30 seconds
    1970/01/01 00:06:12 Could not configure eth1 for IPv6: timeout after waiting for a non-tentative IPv6 address
    1970/01/01 00:06:12 Finished trying to configure all interfaces.
    ```

    **Static Address**
    
    Set the IP and subnet on ETH1

    `ip a add 192.168.0.193/24 dev eth1`

    Set ETH1 link up
    
    `ip link set eth1 up`

    Show the link status

    `ip link show`

    ```
    1: lo: <UP,LOOPBACK> mtu 65536 state UNKNOWN
        link/loopback 
    2: eth0: <BROADCAST,MULTICAST> mtu 1500 state DOWN
        link/ether 0c:42:a1:5a:8a:9c
    3: eth1: <UP,BROADCAST,MULTICAST> mtu 1500 state UP
        link/ether 0c:42:a1:5a:8a:9d
    4: tunl0: <0> mtu 1480 state DOWN
        link/ipip 
    ```

- SSH public key

    Create key.pub from your public key on the client. See The Makefile in this project specifies the default key.pub for the server.

    client (e.g. pi@rpi3debug): `cat ~/.ssh/id_rsa.pub`

    server (e.g. LinuxBoot MtJade): `echo "ssh-rsa AAAAB3Nza...4PiQ== hp 840g1 laptop" > key.pub`

- Starting cpud

    `cpud -init`

## Debugging 9P

1. Was your LinuxBoot kernel built with 9P enabled?

    `make CONFIG_NET_9P=y CONFIG_9P_FS=y`

    Note: tinyconfig, oldconfig, etc. may not have these symbols defined especially if you are using an unmodified Makefile. Also make sure it's not defined as a module *e.g. CONFIG_NET_9P=m* since LinuxBoot builds a small monolithic kernel due to the size of SPI-NOR typically available.

2. Is there a problem reading the SSH keys on the client or server?

    The default file cpu looks for on the client side is called *~/.ssh/identity*

3. CPUD(as remote):9p mount no such device

    It's a vague error that is difficult to understand where it came from since the u-root/cpu error printed [here](https://github.com/u-root/cpu/blob/main/cmds/cpud/main.go#L223) the actual message _**no such device**_ came from the call to [mount()](https://man7.org/linux/man-pages/man2/mount.2.html).

    The error is coming from the cpud side where the LinuxBoot kernel does not have a 9P driver in the kernel. See item #1 above.

    `    pi@rpi3debug:~ $ cpu -d -dbg9p 192.168.0.193 date`

    ```
    2022/04/22 01:38:04 
    Running as client, to host "192.168.0.193", args "date"
    2022/04/22 01:38:04 getKeyFile for ""
    2022/04/22 01:38:04 key file from config is "~/.ssh/identity"
    2022/04/22 01:38:04 getKeyFile returns "/home/pi/.ssh/identity"
    2022/04/22 01:38:04 getPort("192.168.0.193", "")
    2022/04/22 01:38:04 config.Get("192.168.0.193",""): "22"
    2022/04/22 01:38:04 getPort: return default "23"
    2022/04/22 01:38:04 returns "23"
    2022/04/22 01:38:05 listener *ssh.tcpListener &{0x1d802e8 0x1d98b80 0x1e88000} addr 127.0.0.1:43971 port 23
    2022/04/22 01:38:05 srv: try to accept
    2022/04/22 01:38:05 command is "cpud -remote -bin cpud -port9p 43971 date"
                                                                            2022/04/22 01:38:05 Start remote with command "cpud -remote -bin cpud -port9p 43971 date"
                                                    2022/04/22 01:38:05 srv got &{0x1d0a200 0x1d802e8 0x1cc34a0}
                                                                                                                1970/01/01 00:18:31 
    CPUD:Args [cpud -remote -bin cpud -port9p 43971 date] pid 759 *runasinit false *remote true
    1970/01/01 00:18:31 
    CPUD:Not server package: Running as remote: args ["date"], port9p 43971
    1970/01/01 00:18:31 CPUD:namespace is "/lib:/lib64:/usr:/bin:/etc:/home"
    1970/01/01 00:18:31 CPUD:Dial 127.0.0.1:43971
    1970/01/01 00:18:31 CPUD:Connected: write nonce 0799fc2a4ab42cfbf2c9e3535e0ed389
    1970/01/01 00:18:31 CPUD:Wrote the nonce
    1970/01/01 00:18:31 CPUD:fd is 10
    1970/01/01 00:18:31 CPUD: mount 127.0.0.1 on /tmp/cpu 9p 0x6 version=9p2000.L,trans=fd,rfdno=10,wfdno=10,uname=pi,debug=0,msize=1048576
    1970/01/01 00:18:31 CPUD(as remote):9p mount no such device
    2022/04/22 01:38:05 srv: read the nonce back got 0799fc2a4ab42cfbf2c9e3535e0ed389
                                                                                    2022/04/22 01:38:05 SSH error Process exited with status 1
    ```

4. Enabling debug options on the client (cpu) and server (cpud)

    Look at using -d -dbg9p -dump options

    `pi@rpi3debug:~ $ cpu -help`

    ```
    Usage of cpu:
    -bin string
            path of cpu binary (default "cpud")
    -cpudcmd string
            cpud invocation to run at remote, e.g. cpud -d -bin cpud
    -d	enable debug prints
    -dbg9p
            show 9p io
    -dump
            Dump copious output, including a 9p trace, to a temp file at exit
    -fstab string
            pass an fstab to the cpud
    -hk string
            file for host key
    -key string
            key file
    -mountopts string
            Extra options to add to the 9p mount
    -msize int
            msize to use (default 1048576)
    -namespace string
            Default namespace for the remote process -- set to none for none (default "/lib:/lib64:/usr:/bin:/etc:/home")
    -network string
            network to use (default "tcp")
    -new
            Use new cpu package for cpu client
    -port9p string
            port9p # on remote machine for 9p mount
    -root string
            9p root (default "/")
    -sp string
            cpu default port
    -timeout9p string
            time to wait for the 9p mount to happen. (default "100ms")
    ```