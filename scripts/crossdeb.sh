#!/bin/sh

set -e

cwd="$(readlink -f $(dirname $0))"
crossarch=s390x
crossdir="$cwd/../crossdeb"

unmount_pseudo() {
    for i in dev/pts dev proc sys tmp;do
        if mountpoint -q "$crossdir/$i" ;then
            umount -fl "$crossdir/$i"
        fi
    done

    for i in $(grep "$crossdir/$i" /proc/mounts | cut -d' ' -f2 | sed 's|\040| |g');do
        if mountpoint -q "$i" ;then
            umount -fl "$i"
        fi
    done
}
trap unmount_pseudo EXIT INT TERM

rm -rf "$crossdir"

debootstrap $@ stable "$crossdir"

for i in dev dev/pts proc sys tmp;do
    if ! mountpoint -q "$crossdir/$i" ;then
        mkdir -vp "$crossdir/$i"
        mount --bind "/$i" "$crossdir/$i"
    fi
done

cp -vf /etc/resolv.conf "$crossdir/etc/"
ln -vsf /proc/mounts "$crossdir/etc/mtab"

cat > "$crossdir/crossbuild.sh" << EOF
#!/bin/sh

export DEBIAN_FRONTEND=noninteractive

dpkg --add-architecture $crossarch
apt-get update
apt-get install --yes git crossbuild-essential-$crossarch qemu-user-static

git clone --depth=1 git://github.com/fluxer/katie
cd katie
ln -sv package/debian .
apt-get build-dep --yes -a $crossarch .
dpkg-buildpackage -uc -nc -b --no-sign -a $crossarch
EOF
chmod -v +x "$crossdir/crossbuild.sh"

chroot "$crossdir" "/crossbuild.sh"
