################################################################################
#
# our-script
#
################################################################################

define OUR_SCRIPT_EXTRACT_CMDS
	cp -avr $(OUR_SCRIPT_PKGDIR)/src/* $(@D)/
endef

OUR_SCRIPT_MAKE_OPTS = \
	CC="$(TARGET_CC)" \
	CXX="$(TARGET_CXX)"

define OUR_SCRIPT_BUILD_CMDS
	$(MAKE) $(OUR_SCRIPT_MAKE_OPTS) -C $(@D)
endef

define OUR_SCRIPT_INSTALL_TARGET_CMDS
	$(INSTALL) -m 755 -d $(TARGET_DIR)/usr/bin
	cp $(@D)/our-script $(TARGET_DIR)/usr/bin/our-script
endef

$(eval $(generic-package))
