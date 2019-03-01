export DEVKITPRO=/opt/devkitpro
export DEVKITARM=/opt/devkitpro/devkitARM
export DEVKITPPC=/opt/devkitpro/devkitPPC
dkp-pacman -Syyu --noconfirm 3ds-dev nds-dev gp32-dev gba-dev nds-portlibs
apt update
apt install zip
cd /root/kernel/bin2h/
bash make.sh
cd /root/
bash Build.sh