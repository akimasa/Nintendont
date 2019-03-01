export DEVKITPRO=/opt/devkitpro
export DEVKITARM=/opt/devkitpro/devkitARM
export DEVKITPPC=/opt/devkitpro/devkitPPC
dkp-pacman -Syyu --noconfirm devkitARM
apt-get update
apt-get install -y zip
cd /root/kernel/bin2h/
bash make.sh
cd /root/
bash Build.sh