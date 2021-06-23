rm -r tmp 
mkdir tmp 
cp initramfs.linux_amd64.cpio tmp 
cd tmp 
sudo cpio -idv < initramfs.linux_amd64.cpio 
sudo mkdir etc/sshd 
cd etc/sshd 
sudo ssh-keygen -t rsa -P "$2" -f id_rsa 2>&1 >/dev/null
test -f "$1" && sudo cp $1 authorized_keys 
cd ../.. 
rm initramfs.linux_amd64.cpio 
rm /tmp/initramfs.linux_amd64.cpio 
sudo find . 2>/dev/null | sudo cpio -o -H newc -R root:root | xz -9 --format=lzma > /tmp/initramfs.linux_amd64.cpio.lzma  
cd .. 
sudo rm -r tmp
cp /tmp/initramfs.linux_amd64.cpio.lzma . 
cp *lzma linux
exit 0
