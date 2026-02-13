[settings]
os=Linux
arch=x86_64
compiler=gcc
compiler.version=13
compiler.libcxx=libstdc++11
compiler.cppstd=gnu23
build_type=Release

[conf]
tools.system.package_manager:tool=apt=get
tools.system.package_manager:mode=install
tools.system.package_manager:sudo=True
tools.system.package_manager:sudo_askpass=True
