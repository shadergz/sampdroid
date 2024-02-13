#! /bin/python3

import os
import argparse
import pathlib
import subprocess
import shutil
import glob
import platform
import json as voorhees

# You can change these fields too!
with open('env.json', 'r') as jf:
    env = voorhees.load(jf)
    install_in = env['install_dir']
    build_in = env['build_basedir']
    enb_debug = env['enable_debug']
    apk_origin = env['baseapk_file']
    apktool = env['apktool_program']
    envvar_keypath = env['android_keypath_env']
    envvar_keyalias = env['android_keyalias_env']
    envvar_keypass = env['android_keypass_env']

# Change this to your current input files
USER_PATH = os.path.expanduser('~').replace('\\', '/')

APK_BASE = f'{apk_origin}'

APKTOOL = f'{apktool}'

SIGNER_KSPATH = os.getenv(envvar_keypath).replace('\\', '/')
SIGNER_KSALIAS = os.getenv(envvar_keyalias)
SIGNER_KSPASSWORD = os.getenv(envvar_keypass)

# --- Try build now!

# Don't touch anything below, unless you know exactly what you are doing!
ANDROID_SDK = f'{USER_PATH}/AppData/Local/Android/Sdk' if platform.system() == 'Windows' else f'{USER_PATH}/Android/Sdk'

NDK_PATH = f'{ANDROID_SDK}/ndk/' + os.listdir(f'{ANDROID_SDK}/ndk')[0]
BUILD_TOOLS = f'{ANDROID_SDK}/build-tools/' + os.listdir(f'{ANDROID_SDK}/build-tools')[0]
ANDROID_CMAKE = f'{ANDROID_SDK}/cmake/' + os.listdir(f'{ANDROID_SDK}/cmake')[0]
PLATFORM_TOOLS = f'{ANDROID_SDK}/platform-tools'
CMAKE_BIN = f'{ANDROID_CMAKE}/bin/cmake'
NINJA_BIN = f'{ANDROID_CMAKE}/bin/ninja'

ANDROID_MODEL = 'c++_shared'

ANDROID_TOOLCHAIN = f'{NDK_PATH}/build/cmake/android.toolchain.cmake'

ANDROID_PREBUILT = 'windows-x86_64' if platform.system() == 'Windows' else 'linux-x86_64'
ANDROID_SHARED = f'{NDK_PATH}/toolchains/llvm/prebuilt/{ANDROID_PREBUILT}/\
sysroot/usr/lib/aarch64-linux-android/lib{ANDROID_MODEL}.so'

ANDROID_MIN = 31
ANDROID_MAX = 33

ANDROID_TARGET = f'core-{ANDROID_MAX}'
ANDROID_MICRO_ABI = 'arm64-v8a'

APKSIGNER = f'{BUILD_TOOLS}/' + ('apksigner.bat' if platform.system() == 'Windows' else 'apksigner')
ADB = f'{PLATFORM_TOOLS}/adb'
ZIPALIGN = f'{BUILD_TOOLS}/zipalign'

BUILD_TYPES = ('Release', 'Debug')
BUILD_DIR = '{}.{}'.format(build_in, 'dbg' if enb_debug else 'rel')

MOD_VERSION = '0.104'
APK_OUT = f'{install_in}/gtasa-dir'

LIB_BASENAME = 'samp20'

OUTPUT_MOD_FILE = f'{install_in}/{LIB_BASENAME} v{MOD_VERSION}.apk'
MALICIOUS_SMALI = 'smali/GTASA.smali'

parser = argparse.ArgumentParser()

parser.add_argument('-b', '--build', action='store_true')
parser.add_argument('-g', '--genapk', action='store_true')
parser.add_argument('-i', '--install', action='store_true')
parser.add_argument('-d', '--devices', action='store_true')
parser.add_argument('-C', '--connect', type=str)
parser.add_argument('-l', '--logcat', action='store_true')
parser.add_argument('-c', '--clean', action='store_true')

args = parser.parse_args()


def build_dir():
    os.mkdir(BUILD_DIR)
    command = os.getcwd()
    os.chdir(BUILD_DIR)

    cmake_build_options = {
        '-DANDROID_NDK=': NDK_PATH,
        '-DANDROID_ABI=': ANDROID_MICRO_ABI,
        '-DANDROID_PLATFORM=': ANDROID_MIN,
        '-DCMAKE_ANDROID_ARCH_ABI=': ANDROID_MICRO_ABI,
        '-DANDROID_STL=': ANDROID_MODEL,
        '-DCMAKE_SYSTEM_NAME=': 'Android',
        '-DCMAKE_TOOLCHAIN_FILE=': ANDROID_TOOLCHAIN,

        '-DSAMP20_OUTRELDIR=': install_in,
        '-DSAMP20_SHARED_NAME=': LIB_BASENAME,
        '-DSAMP20_VERSION=': MOD_VERSION,

        '-DCMAKE_BUILD_TYPE=': BUILD_TYPES[0] if not enb_debug else BUILD_TYPES[1],
        '-DCMAKE_MAKE_PROGRAM=': NINJA_BIN,
        '-DCMAKE_EXPORT_COMPILE_COMMANDS=': 'On',
        '-G': 'Ninja'
    }

    full_cmake = ['{}{}'.format(*c) for c in sorted(cmake_build_options.items())]

    print('CMake options list: ', full_cmake)
    subprocess.run([CMAKE_BIN] + full_cmake + ['../..'], shell=False)
    os.chdir(command)


def generate_apk():
    if not os.path.exists(APK_OUT):
        subprocess.run(['java', '-jar', APKTOOL, 'd', '--output', APK_OUT, APK_BASE], shell=False)
        shutil.copy(MALICIOUS_SMALI, f'{APK_OUT}/smali/com/rockstargames/gtasa/GTASA.smali')

        # Removing all directories that we don't care about inside the (lib) directory
        useless_libdirs = os.listdir(f'{APK_OUT}/lib')
        useless_libdirs.remove(ANDROID_MICRO_ABI)

        for useless in useless_libdirs:
            shutil.rmtree(f'{APK_OUT}/lib/{useless}')

    cwd = os.getcwd()
    os.chdir(BUILD_DIR)
    subprocess.run([NINJA_BIN, 'install'])
    os.chdir(cwd)

    # Copying all needed libraries
    cpp_lib = f'{APK_OUT}/lib/{ANDROID_MICRO_ABI}/' + ANDROID_SHARED.split('/')[-1]

    if not pathlib.Path(cpp_lib).is_file():
        shutil.copy(ANDROID_SHARED, cpp_lib)

    for lib_file in glob.glob(f'{install_in}/lib*'):
        lib_file = lib_file.replace('\\', '/')
        shutil.copy(lib_file, f'{APK_OUT}/lib/{ANDROID_MICRO_ABI}/' + lib_file.split('/')[-1])

    subprocess.run(['java', '-jar', APKTOOL, 'b', '--output', f'{OUTPUT_MOD_FILE}.un', APK_OUT])
    subprocess.run([ZIPALIGN, '-p', '-v', '-f', '4', f'{OUTPUT_MOD_FILE}.un', f'{OUTPUT_MOD_FILE}.aligned'], )
    subprocess.run([
        APKSIGNER, 'sign', '-v',
        f'--min-sdk-version={ANDROID_MIN}',
        f'--max-sdk-version={ANDROID_MAX}',
        f'--ks={SIGNER_KSPATH}',
        f'--ks-key-alias={SIGNER_KSALIAS}',
        f'--ks-pass=pass:{SIGNER_KSPASSWORD}',
        f'--in={OUTPUT_MOD_FILE}.aligned',
        f'--out={OUTPUT_MOD_FILE}'
    ])

    os.remove(f'{OUTPUT_MOD_FILE}.un')
    os.remove(f'{OUTPUT_MOD_FILE}.aligned')
    subprocess.run([APKSIGNER, 'verify', '-v', f'{OUTPUT_MOD_FILE}'])


def list_devices():
    subprocess.run([ADB, 'devices'], shell=False)


def connect_device(dev_device: str):
    subprocess.run([ADB, 'connect', dev_device])


def install_apk():
    subprocess.run([ADB, 'install', '-r', '--streaming', OUTPUT_MOD_FILE])


def logcat():
    try:
        subprocess.run([ADB, 'logcat', 'samp20,stargames.gtasa, *:S'])
    except KeyboardInterrupt:
        pass


def clean():
    local = os.getcwd()
    os.chdir(BUILD_DIR)
    subprocess.run([NINJA_BIN, 'clean'])
    os.chdir(local)

    delete_files = [f'{install_in}/{de}' for de in os.listdir(install_in)]
    delete_files.remove(APK_OUT)

    print(f'Cleaning files: {delete_files}')

    for delete in delete_files:
        os.remove(delete)


if args.build and not os.path.exists(BUILD_DIR):
    build_dir()

if args.genapk:
    if not os.path.exists(BUILD_DIR):
        build_dir()
    generate_apk()

if args.devices:
    list_devices()

if args.connect:
    connect_device(args.connect)

if args.install:
    if not pathlib.Path(OUTPUT_MOD_FILE).is_file():
        generate_apk()
    install_apk()

if args.logcat:
    logcat()

if args.clean:
    clean()
