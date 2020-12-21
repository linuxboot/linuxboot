// Copyright 2020 the u-root Authors. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// This is an example uinit for a board which has special requirements.
// First, it fixes the RSDP so that it will be found by whatever
// kernel comes next. Next, it mounts in this case (e.g.) /dev/sda5.
// Finally, it read /mnt/boot/bootcmds, a file which contains 0 or more
// commands, one per line.
// As we do not wish to implement shell-like parsing, we separate arguments
// on each line by a :.
// e.g.
// fitboot:-c:arg0 arg1 arg2 arg:/mnt/boot/someuImage
// debug prints are on by default but can be turned off via
// -d=false
// The location of the command file is controlled by -cmd switch.
package main

import (
	"flag"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"strings"
	"time"
)

var (
	bootcmd = flag.String("cmd", "/mnt/boot/bootcmds", "File containing boot command line")
	debug   = flag.Bool("d", true, "Print debug statement")
	v       = func(string, ...interface{}) {}
)

func main() {
	if *debug {
		v = log.Printf
	}
	var bootcmds = []*exec.Cmd{
		exec.Command("fixrsdp"),
		exec.Command("mount", "/dev/sda5", "/mnt"),
	}

	for _, c := range bootcmds {
		v("Run %v", c)
		c.Stdout, c.Stderr = os.Stdout, os.Stderr
		if err := c.Run(); err != nil {
			log.Printf("cmd '%q' fails: %v", c, err)
		}
	}

	time.Sleep(10 * time.Second)
	if cmds, err := ioutil.ReadFile(*bootcmd); err != nil {
		log.Printf("Can not open %q: %v; will not attempt to boot", *bootcmd, err)
	} else {
		bootcmds = []*exec.Cmd{}
		lines := strings.Split(string(cmds), "\n")
		for _, l := range lines {
			f := strings.Split(l, ":")
			var args []string
			if len(f) > 1 {
				args = f[1:]
			}
			v("Add command %q %q", f[0], args)
			bootcmds = append(bootcmds, exec.Command(f[0], args...))
		}
	}
	for _, c := range bootcmds {
		v("Run %v", c)
		c.Stdout, c.Stderr = os.Stdout, os.Stderr
		if err := c.Run(); err != nil {
			log.Fatalf("cmd '%q' fails: %v", c, err)
		}
	}

}
