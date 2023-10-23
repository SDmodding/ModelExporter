# Model Exporter
This is primitve model exporter that takes `.perm.bin` file and export models & materials to `.obj` & `.mtl` files.

## Args
`-perm`: Path to `.perm.bin` file.

`-outdir`: Path to directory where it output the files.

`-silent`: Disable wait for key when finished. (Useful for batch exporting)

## Usage
Simple launch the executable and select perm file or use launch args.

This tool doesn't export textures, but does export materials and textures are pointing to parent directory under `Textures` ie. `../Textures/<Hash>.png`.

You need to export the textures yourself. Here is one tool: https://github.com/sneakyevil/SleepingDogs-UltimateTexTool after loading textures in that tool you need to use `Texture > Rename to Hashes` or move all textures in output directory and if the hash will match the file name of texture it will automatically assign it.

After you got the textures with the hashed names you just place them in that parent directory: `Textures` and while loading the model it will load the textures.
