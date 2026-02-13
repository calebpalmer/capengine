import os

from conan import ConanFile


class CompressorRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        # to make transitives work with each other
        # self.requires("libpng/1.6.38")
        # self.requires("zlib/1.2.13")
        # self.requires("libiconv/1.17")
        # self.requires("libalsa/1.2.10")
        # self.requires("xz_utils/5.4.5")

        # top level deps
        self.requires("sdl/2.28.3@")
        # self.requires("sdl_image/2.8.2")
        # self.requires("sdl_ttf/2.0.18")
        self.requires("libxml2/2.15.0")
        self.requires("libsndfile/1.2.2")
        self.requires("boost/1.89.0")
        self.requires("gsl-lite/0.42.0")

        # dev deps
        self.requires("cppunit/1.15.1")


    # def build_requirements(self):
    #     self.tool_requires("cmake/3.31.9")


    def configure(self):
     self.options["sdl"].wayland = False

    def layout(self):
        # right now linux only
        output_folder = "conan"
        self.folders.build = os.path.join(output_folder, str(self.settings.build_type))
        self.folders.generators = os.path.join(self.folders.build, "generators")
        
    def validate(self):
        if self.settings.os != "Linux" or self.settings.arch != "x86_64":
            raise ConanInvalidConfiguration("Only Linux and x86_64 currently supported")
