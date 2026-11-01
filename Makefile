THEOS_PACKAGE_COMPRESSOR = xz
THEOS_PACKAGE_SCHEME = rootless
TARGET := iphone:clang:latest:15.0
INSTALL_TARGET_PROCESSES = SpringBoard Preferences

# 支持最新 Dopamine 2.x (arm64 + arm64e 统一打包为 iphoneos-arm64.deb；若云端仅处理单切片可改为 ARCHS = arm64)
ARCHS = arm64 arm64e

include $(THEOS)/makefiles/common.mk

TWEAK_NAME = AngelGuardian

AngelGuardian_FILES = Tweak.xm
AngelGuardian_CFLAGS = -fobjc-arc -fvisibility=hidden -fvisibility-inlines-hidden -O2 -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-function
AngelGuardian_LDFLAGS = -Wl,-x -Wl,-S
AngelGuardian_FRAMEWORKS = UIKit Foundation AVFoundation CoreMedia CoreVideo CoreImage Contacts QuartzCore CoreFoundation VideoToolbox ImageIO Speech ReplayKit Vision AudioToolbox CoreLocation MapKit CoreMotion SystemConfiguration NetworkExtension Security
AngelGuardian_LIBRARIES = sqlite3

TOOL_NAME = AngelCameraHelper
AngelCameraHelper_ARCHS = arm64 arm64e
AngelCameraHelper_FILES = AngelCameraHelper.m
AngelCameraHelper_CFLAGS = -fobjc-arc -Wno-deprecated-declarations -Wno-unused-function -Wno-incompatible-pointer-types-discards-qualifiers -Wno-unguarded-availability-new
AngelCameraHelper_LDFLAGS = -Wl,-sectcreate,__TEXT,__info_plist,HelperInfo.plist
AngelCameraHelper_FRAMEWORKS = UIKit QuartzCore AVFoundation CoreMedia CoreVideo CoreImage Foundation ImageIO VideoToolbox
AngelCameraHelper_CODESIGN_FLAGS = -Scam_ent.xml
AngelCameraHelper_INSTALL_PATH = /usr/bin

include $(THEOS_MAKE_PATH)/tweak.mk
include $(THEOS_MAKE_PATH)/tool.mk
