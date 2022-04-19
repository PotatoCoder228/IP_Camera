################################################################################
#
# zerotier-one
#
################################################################################

ZEROTIER_ONE_VERSION = 1294767b44c20faf069f5ddbe287fb96ad0750d1
ZEROTIER_ONE_SITE = $(call github,zerotier,ZeroTierOne,$(ZEROTIER_ONE_VERSION))
# ZEROTIER_ONE_DEPENDENCIES += libminiupnpc libnatpmp
ZEROTIER_ONE_LICENSE = BUSL-1.1
ZEROTIER_ONE_LICENSE_FILES = LICENSE.txt

ZEROTIER_ONE_MAKE_OPTS = \
	CC="$(TARGET_CC)" \
	CXX="$(TARGET_CXX)"

define ZEROTIER_ONE_BUILD_CMDS
	$(MAKE) $(ZEROTIER_ONE_MAKE_OPTS) -C $(@D) all
endef

define ZEROTIER_ONE_INSTALL_TARGET_CMDS
#	$(MAKE) -C $(@D) DESTDIR=$(TARGET_DIR) install

	$(INSTALL) -m 755 -d $(TARGET_DIR)/etc/init.d
	cp ../general/package/zerotier-one/files/S90zerotier $(TARGET_DIR)/etc/init.d
endef

$(eval $(generic-package))