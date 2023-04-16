#! /bin/python3

import os, argparse, pathlib, subprocess, shutil, glob
from typing import Final

# We can change these fields too!

ENB_DEBUG: Final[bool]=False

INSTALL_DIR: Final[str]='factory'
BUILD_BASEDIR: Final[str]='build'

# Change this to your current input files
USER_PATH: Final[str]=os.path.expanduser('~')
APK_BASE: Final[str]=f'{USER_PATH}/Documents/GTASAv210.apk'
ANDROID_SDK: Final[str]=f'{USER_PATH}/Android/Sdk'

APKTOOL: Final[str]= f'{USER_PATH}/tools/apktool_2.7.0.jar'
SIGNER_KSPATH: Final[str]=os.getenv('ANDROID_KEY_FILEPATH')
SIGNER_KSALIAS: Final[str]=os.getenv('ANDROID_KEY_ALIAS')
SIGNER_KSPASSWORD: Final[str]=os.getenv('ANDROID_KEY_PASS')

# --- Try build now!

# Don't touch in nothing below, at least if you know exactly what are you doing!
NDK_PATH: Final[str]=f'{ANDROID_SDK}/ndk/'+os.listdir(f'{ANDROID_SDK}/ndk')[0]
BUILD_TOOLS: Final[str]=f'{ANDROID_SDK}/build-tools/'+os.listdir(f'{ANDROID_SDK}/build-tools')[0]
ANDROID_CMAKE: Final[str]=f'{ANDROID_SDK}/cmake/'+os.listdir(f'{ANDROID_SDK}/cmake')[0]
PLATFORM_TOOLS: Final[str]=f'{ANDROID_SDK}/platform-tools'
CMAKE_BIN: Final[str]=f'{ANDROID_CMAKE}/bin/cmake'
NINJA_BIN: Final[str]=f'{ANDROID_CMAKE}/bin/ninja'

ANDROID_MODEL='c++_shared'

ANDROID_TOOLCHAIN: Final[str]=f'{NDK_PATH}/build/cmake/android.toolchain.cmake'
ANDROID_SHARED: Final[str]=f'{NDK_PATH}/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/lib{ANDROID_MODEL}.so'

ANDROID_MIN: Final[int]=23
ANDROID_MAX: Final[int]=29

ANDROID_TARGET: Final[str]=f'android-{ANDROID_MAX}'
ANDROID_MICRO_ABI: Final[str]='arm64-v8a'

APKSIGNER: Final[str]=f'{BUILD_TOOLS}/apksigner'
ADB: Final[str]=f'{PLATFORM_TOOLS}/adb'

BUILD_TYPES: Final[tuple]=('Release', 'Debug')
BUILD_MODE: Final[str]=BUILD_TYPES[int(ENB_DEBUG)]

BUILD_DIR: Final[str]='{}.{}'.format(BUILD_BASEDIR, 'dbg' if BUILD_TYPES == False else 'rel')

MTM_VERSION: Final[str]='0.0.1'
APK_OUT: Final[str]=f'{INSTALL_DIR}/gtasabase'

LIB_BASENAME: Final[str]='mtm'

OUTPUT_MTM_FILE: Final[str]=f'{INSTALL_DIR}/{LIB_BASENAME.upper()}-v{MTM_VERSION}.apk'
MALICIOUS_SMALI: Final[str]='GTASA.smali'

parser = argparse.ArgumentParser()

parser.add_argument('-b', '--build', action='store_true')
parser.add_argument('-g', '--genapk', action='store_true')
parser.add_argument('-i', '--install', action='store_true')
parser.add_argument('-c', '--clean', action='store_true')
parser.add_argument('-d', '--devices', action='store_true')

args = parser.parse_args()

def build_dir():
    os.mkdir(BUILD_DIR)
    CWD: Final[str] = os.getcwd()
    os.chdir(BUILD_DIR)
    
    CMAKE_BUILD_OPTIONS: Final[dict] = {
        '-DANDROID_NDK=': NDK_PATH,
        '-DANDROID_ABI=': ANDROID_MICRO_ABI,
        '-DANDROID_PLATFORM=': ANDROID_MIN,
        '-DCMAKE_ANDROID_ARCH_ABI=': ANDROID_MICRO_ABI,
        '-DANDROID_STL=': ANDROID_MODEL,
        '-DCMAKE_SYSTEM_NAME=': 'Android',
        '-DCMAKE_TOOLCHAIN_FILE=': ANDROID_TOOLCHAIN,
        
        '-DMTM_OUTRELDIR=': INSTALL_DIR,
        '-DMTM_LIBNAME=': LIB_BASENAME,
        '-DMTM_VERSION=': MTM_VERSION,
        
        '-DCMAKE_BUILD_TYPE=': BUILD_MODE,
        '-DCMAKE_EXPORT_COMPILE_COMMANDS=': 'On',
        '-G': 'Ninja'
    }
    
    FULL_CMAKE = ['{}{}'.format(*c) for c in sorted(CMAKE_BUILD_OPTIONS.items())]
    
    print('CMake options list: ', FULL_CMAKE)
    subprocess.run([CMAKE_BIN] + FULL_CMAKE + ['..'], shell=False)
    os.chdir(CWD)
    
def generate_apk():
    if not os.path.exists(APK_OUT):
        subprocess.run(['java', '-jar', APKTOOL, 'd', '--output', APK_OUT, APK_BASE], shell=False)
        shutil.copy(MALICIOUS_SMALI, f'{APK_OUT}/smali/com/rockstargames/gtasa/{MALICIOUS_SMALI}')
        
        # Removing all directories that we don't care inside of (lib) dir
        useless_libdirs = os.listdir(f'{APK_OUT}/lib')
        useless_libdirs.remove(ANDROID_MICRO_ABI)
        
        for useless in useless_libdirs:
            shutil.rmtree(f'{APK_OUT}/lib/{useless}')
    
    CWD: Final[str] = os.getcwd()
    os.chdir(BUILD_DIR)
    subprocess.run([NINJA_BIN, 'install'])
    os.chdir(CWD)
    
    # Copying all needed libraries
    CPP_LIB: Final[str] = f'{APK_OUT}/lib/{ANDROID_MICRO_ABI}/'+ANDROID_SHARED.split('/')[-1]
    
    if not pathlib.Path(CPP_LIB).is_file():
        shutil.copy(ANDROID_SHARED, CPP_LIB)
    
    for lib_file in glob.glob(f'{INSTALL_DIR}/lib*'):
        shutil.copy(lib_file, f'{APK_OUT}/lib/{ANDROID_MICRO_ABI}/'+lib_file.split('/')[-1])
    
    subprocess.run(['java', '-jar', APKTOOL, 'b', '--output', f'{OUTPUT_MTM_FILE}.un', APK_OUT])
    subprocess.run([
        APKSIGNER, 'sign', '-v',
        f'--min-sdk-version={ANDROID_MIN}',
        f'--max-sdk-version={ANDROID_MAX}',
        f'--ks={SIGNER_KSPATH}',
        f'--ks-key-alias={SIGNER_KSALIAS}',
        f'--ks-pass=pass:{SIGNER_KSPASSWORD}',
        f'--in={OUTPUT_MTM_FILE}.un',
        f'--out={OUTPUT_MTM_FILE}'  
    ])
    
    os.remove(f'{OUTPUT_MTM_FILE}.un')
    subprocess.run([APKSIGNER, 'verify', '-v', f'{OUTPUT_MTM_FILE}'])
        
def install_apk():
    subprocess.run([ADB, 'install', OUTPUT_MTM_FILE])

def clean():
    CWD: Final[str] = os.getcwd()
    os.chdir(BUILD_DIR)
    subprocess.run([NINJA_BIN, 'clean'])
    os.chdir(CWD)
    
    delete_files = [(f'{INSTALL_DIR}/{de}') for de in os.listdir(INSTALL_DIR)]
    delete_files.remove(APK_OUT)
    
    print(f'Cleaning files: {delete_files}')    

    for delete in delete_files:
        os.remove(delete)

if args.devices:
    subprocess.run([ADB, 'devices'], shell=False) 

if args.build and not os.path.exists(BUILD_DIR):
    build_dir()

if args.genapk:
    if not os.path.exists(BUILD_DIR):
        build_dir()
    generate_apk()  

if args.install:
    if not pathlib.Path(OUTPUT_MTM_FILE).is_file():
        generate_apk()
    install_apk()

if args.clean:
    clean()
    


