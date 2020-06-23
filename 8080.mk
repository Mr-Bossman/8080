################################################################################
#
# 8080
#
################################################################################

8080_VERSION = 1.0
8080_SITE = $(8080_PKGDIR)src
8080_SITE_METHOD = local

define 8080_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define 8080_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/8080 $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
