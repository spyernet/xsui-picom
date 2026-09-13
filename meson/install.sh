#!/bin/sh

if [ ! -e "${MESON_INSTALL_DESTDIR_PREFIX}/bin/xsui-picom" ]; then
	echo "Linking xsui-picom to ${MESON_INSTALL_DESTDIR_PREFIX}/bin/xsui-picom"
	ln -s xsui-picom "${MESON_INSTALL_DESTDIR_PREFIX}/bin/xsui-picom"
fi

if [ ! -e "${MESON_INSTALL_DESTDIR_PREFIX}/bin/xsui-picom-trans" ]; then
	echo "Linking xsui-picom-trans to ${MESON_INSTALL_DESTDIR_PREFIX}/bin/xsui-picom-trans"
	ln -s xsui-picom-trans "${MESON_INSTALL_DESTDIR_PREFIX}/bin/xsui-picom-trans"
fi
