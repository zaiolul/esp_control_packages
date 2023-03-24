include $(TOPDIR)/rules.mk
 
PKG_NAME:=esp_control
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/esp_control
	CATEGORY:=Utilities
	TITLE:=esp pin control
	DEPENDS:= +libubus +libubox +libblobmsg-json +libserialport
endef

define Package/esp_control/description
	Ubus object for pin control
endef

define Package/esp_control/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esp_control $(1)/usr/bin
	$(INSTALL_BIN) ./files/esp_control.init $(1)/etc/init.d/esp_control

endef

$(eval $(call BuildPackage,esp_control))