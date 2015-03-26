This folder contains metadata and descriptions for all games in the "Games" section. **To add a new game, do the following:**

* Duplicate `template.md`. The new file name should consist of the timestamp when the game was released (or the current date if it is not yet released) and the name of the game in `dash-separated-lower-case`.
* Create an image with the dimensions 480x360px and store it in `img/games` using the same file name you used for the .md file.
* If you want to feature a video, create an additional image with the dimensions 120x68px with the suffix `_video`.
* Fill in the information in the .md file.

Here are some pointers about the format of the .md file:

* As described above, both images use a fixed naming scheme. You are free to choose a file format (jpg/png are sensible); set the `image_type` field accordingly.
* If you don't want to feature a video, delete the whole `video` part.
* The entries in the `platforms` list should be kept in the same order they are in the template file; just delete those that don't apply. To add a new platform, have a look at `/_data/platforms.yml`.
* If the game isn't released yet, add this line: `coming_soon: true`
* Add a description below the second triple-dash (`---`). Feel free to use Markdown formatting. 