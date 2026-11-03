THEOS_PACKAGE_COMPRESSOR = xz
TARGET := iphone:clang:16.5:15.0

ifeq ($(ROOTHIDE),1)
    THEOS_PACKAGE_SCHEME = roothide
    ARCHS = arm64e
    EXTRA_SCHEME_CFLAGS = -DROOTHIDE=1
else ifeq ($(THEOS_PACKAGE_SCHEME),roothide)
    THEOS_PACKAGE_SCHEME = roothide
    ARCHS = arm64e
    EXTRA_SCHEME_CFLAGS = -DROOTHIDE=1
else
    THEOS_PACKAGE_SCHEME = rootless
    ARCHS = arm64 arm64e
    EXTRA_SCHEME_CFLAGS =
endif

INSTALL_TARGET_PROCESSES = SpringBoard Preferences

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = AngelGuardian
AngelGuardian_FILES = Tweak.x
AngelGuardian_CFLAGS = -fobjc-arc -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unguarded-availability-new -Wno-incomplete-implementation -Wno-unused-function -Wno-arc-performSelector-leaks -Wno-nonnull -Wno-pointer-bool-conversion $(EXTRA_SCHEME_CFLAGS)
AngelGuardian_FRAMEWORKS = UIKit Foundation AVFoundation Contacts QuartzCore CoreGraphics CoreMedia CoreVideo CoreImage VideoToolbox ImageIO Speech AudioToolbox Vision ReplayKit CoreLocation CoreMotion MapKit SystemConfiguration NetworkExtension

TOOL_NAME = AngelCameraHelper
AngelCameraHelper_ARCHS = $(ARCHS)
AngelCameraHelper_FILES = AngelCameraHelper.m
AngelCameraHelper_CFLAGS = -fobjc-arc -Wno-deprecated-declarations -Wno-unused-function -Wno-incompatible-pointer-types-discards-qualifiers -Wno-unguarded-availability-new $(EXTRA_SCHEME_CFLAGS)
AngelCameraHelper_LDFLAGS = -Wl,-sectcreate,__TEXT,__info_plist,HelperInfo.plist
AngelCameraHelper_FRAMEWORKS = UIKit QuartzCore AVFoundation CoreMedia CoreVideo CoreImage Foundation ImageIO VideoToolbox
AngelCameraHelper_CODESIGN_FLAGS = -Scam_ent.xml
AngelCameraHelper_INSTALL_PATH = /usr/bin

include $(THEOS_MAKE_PATH)/tweak.mk
include $(THEOS_MAKE_PATH)/tool.mk

after-stage::
	@chmod 0666 $(THEOS_STAGING_DIR)/Library/MobileSubstrate/DynamicLibraries/AngelGuardian.plist 2>/dev/null || true
	@chmod 0666 $(THEOS_STAGING_DIR)/var/jb/Library/MobileSubstrate/DynamicLibraries/AngelGuardian.plist 2>/dev/null || true

.PHONY: both
both:
	@echo "========== [1/2] 正在编译 Dopamine (rootless) 版本 =========="
	@$(MAKE) clean && $(MAKE) package
	@echo "========== [2/2] 正在编译 Relaxin (roothide) 版本 =========="
	@$(MAKE) clean && $(MAKE) package ROOTHIDE=1
	@echo "✅ 双版本编译完成！请查看 packages/ 目录下的两个 .deb 安装包。"
