include $(TOPDIR)/rules.mk
 
PKG_NAME:=pin_control
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/pin_control
	CATEGORY:=Utilities
	TITLE:=esp pin control
	DEPENDS:= +libubus +libubox +libblobmsg-json +libserialport
endef

define Package/pin_control/description
	Ubus object for pin control
endef

define Package/pin_control/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pin_control $(1)/usr/bin
	$(INSTALL_BIN) ./files/pin_control.init $(1)/etc/init.d/pin_control

endef

$(eval $(call BuildPackage,pin_control))