
# Change this to your current input files
apkBase=$(HOME)/Documents/GTASAv210.apk
androidSdk=$(HOME)/Android/Sdk

APKTOOL=$(HOME)/tools/apktool_2.7.0.jar

signerKSpath=$(ANDROID_KEY_FILEPATH)
signerKSAlias=$(ANDROID_KEY_ALIAS)
signerKSPassword=$(ANDROID_KEY_PASS)

# --- Try build now!

# Don't touch in nothing below, at least if you know exactly what are you doing!
ndkPath=$(shell find '$(androidSdk)/ndk' -mindepth 1 -maxdepth 1 -type d | head -n 1)
buildTools=$(shell find '$(androidSdk)/build-tools' -mindepth 1 -maxdepth 1 -type d | head -n 1)
androidCMake=$(shell find '$(androidSdk)/cmake' -mindepth 1 -maxdepth 1 -type d | head -n 1)
platformTools=$(androidSdk)/platform-tools

CMAKE=$(androidCMake)/bin/cmake
NINJA=$(androidCMake)/bin/ninja

androidModel=c++_shared

androidToolchain=$(ndkPath)/build/cmake/android.toolchain.cmake
androidShared=$(ndkPath)/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/lib$(androidModel).so

androidMin=23
androidMax=29

androidTarget=android-$(androidMax)
androidMicroABI=arm64-v8a

APKSIGNER=$(buildTools)/apksigner
ADB=$(platformTools)/adb

installDir=factory
buildMode=Release
buildDir=build.dbg

mtmVersion=0.0.1
apkOut=$(installDir)/gtasabase
outputMtmFile=$(installDir)/mtm-v$(mtmVersion).apk

libName=mtm
maliSmali=GTASA.smali

ifeq ($(buildMode), Release)
	override buildDir=build.re
endif

all: build genapk installapk

build: $(buildDir)
	cd $<;\
		$(NINJA) install
	cd ..

genapk: $(installDir)
	if [ ! -d $(apkOut) ]; then\
		java -jar $(APKTOOL) d --output $(apkOut) $(apkBase);\
		cp $(maliSmali) $(apkOut)/smali/com/rockstargames/gtasa;\
	fi\

	rm -rf $(shell find $(apkOut)/lib -mindepth 1 -maxdepth 1 ! -name $(androidMicroABI));\

	@# Copying all needed libraries
	if [ ! -f $(apkOut)/lib/$(androidMicroABI)/lib$(androidModel).so ]; then\
		cp $(androidShared) $(apkOut)/lib/$(androidMicroABI);\
	fi
	cp $(shell find $< -mindepth 1 -maxdepth 1 | grep lib) $(apkOut)/lib/$(androidMicroABI)

	java -jar $(APKTOOL) b --output $(outputMtmFile).unsigned $(apkOut)
	$(APKSIGNER) sign\
		--min-sdk-version=$(androidMin)\
		--max-sdk-version=$(androidMax)\
		-v\
		--ks=$(signerKSpath)\
		--ks-key-alias=$(signerKSAlias)\
		--ks-pass=pass:$(signerKSPassword)\
		--in  $(outputMtmFile).unsigned\
		--out $(outputMtmFile)
	$(APKSIGNER) verify -v $(outputMtmFile)
	rm -f unsigned.$(outputMtmFile)

installapk: $(outputMtmFile)
	$(ADB) devices
	$(ADB) install $(outputMtmFile)

$(buildDir):
	mkdir $@
	cd $@;\
		$(CMAKE)\
		-DANDROID_NDK=$(ndkPath)\
		-DANDROID_ABI=$(androidMicroABI)\
		-DANDROID_PLATFORM=$(androidMin)\
		-DCMAKE_ANDROID_ARCH_ABI=$(androidMicroABI)\
		-DANDROID_STL=$(androidModel)\
		-DCMAKE_SYSTEM_NAME=Android\
		-DCMAKE_SYSTEM_NAME=$(androidMax)\
		-DCMAKE_TOOLCHAIN_FILE=$(androidToolchain)\
		\
		-DMTM_OUTRELDIR=$(installDir)\
		-DMTM_LIBNAME=$(libName)\
		-DMTM_VERSION=$(mtmVersion)\
		\
		-DCMAKE_BUILD_TYPE=$(buildMode)\
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON\
		-GNinja ..;\
	cd ..

clean: $(buildDir)
	cd $<;\
		$(NINJA) clean;\
	cd ..
	rm -f $(installDir)/lib$(libName).so

.PHONY: all build genapk installapk clean