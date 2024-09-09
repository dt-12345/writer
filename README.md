Created with [exlaunch](https://github.com/shadowninja108/exlaunch), thank you shadow very cool

This is a restoration of `sead::TextWriter` for *Tears of the Kingdom* primarily for research + debugging purposes. `callbacks.h` declares six draw callbacks (one for each layer in TotK) that you can implement however you want. There are sample implementations in `callbacks.cpp` but you can change/move those however you want. Currently works on all versions (1.0.0, 1.1.0, 1.1.1, 1.1.2, 1.2.0, and 1.2.1).

To build, install [devkitPro](https://devkitpro.org/wiki/Getting_Started) (make sure to add it to your environment path) and then run `make`. This can of course be adjusted to work with other build systems. This mod requires the following files to placed in your mod's romfs as they are not included in TotK (you can find them in *Splatoon 3* and probably other games):
```
romfs
└───Lib
    └───sead
        ├───nvn_font
        │       nvn_font.ntx
        │       nvn_font_jis1.ntx
        │       nvn_font_jis1_mipmap.xtx
        │       nvn_font_jis1_tbl.bin
        │       nvn_font_shader.bin
        │       nvn_font_shader_jis1.bin
        │       nvn_font_shader_jis1_mipmap.bin
        │
        └───primitive_renderer
                primitive_drawer_nvn_shader.bin
```