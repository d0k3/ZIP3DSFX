# ZIP3DSFX v0.6 by d0k3 
ZIP3DSFX is a ZIP-based SFX extractor for the 3DS console. It has two main build modes plus multiple configuration options.

**SFX Hard Mode:**
This mode hardcodes the archive.zip into the ZIP3DSFX.3dsx executable. To use this, rename your ZIP archive to 'archive.zip', put it into the data directory and compile with 'make sfx_hard'.
* This will use the smallest amount of memory and will work anywhere.
* The archives content can not be changed without compiling anew.

**SFX Stub Mode:**
This mode creates a SFX stub, the actual archive.zip has to be attached to the end of the ZIP3DSFX.3dsx. Compile this with 'make sfx_stub'. Files are attached (on Windows) via 'copy /b ZIP3DSFX.3dsx + archive.zip myZIP3DSFX.3dsx'.
* You can simply copy any ZIP archive to the end of the 3DSX to create a new SFX archive.
* The resulting .3DSX can (in standard ZIP mode) still be opened in any archiver program on any platform.
* This uses more memory and will not properly work with some loading methods.

If you want to customize the SMDH file, edit AppInfo and icon.png in the resources.dir. If you want to further customize ZIP3DSFX overwrite behaviour (among other settings) edit config.h inside the include directory or use predefined config settings def_overwrite_always, def_overwrite_ask, def_overwrite_never, theme_pack or boot_installer. The correct syntax is: 'make [sfx_stub|sfx_hard] CONFIG=[config_name]'. This is untested with large files and archives. Use at your own risk! 

Contains the MiniZ library, which was written by Rich Geldreich: https://code.google.com/p/miniz/

## License
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any
  damages arising from the use of this software.

  Permission is granted to anyone to use this software for any
  purpose, including commercial applications, and to alter it and
  redistribute it freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you
     must not claim that you wrote the original software. If you use
     this software in a product, an acknowledgment in the product
     documentation would be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and
     must not be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
     distribution.
