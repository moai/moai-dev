This folder contains metadata and descriptions for all games in the "Games" section. **To add a new game, do the following:**

* Duplicate `template.md`. The new file name should consist of the timestamp when the game was released (or the current date if it is not yet released) and the name of the game in `dash-separated-lower-case`.
* Create an image with the dimensions 480x360px and store it in `img/games` using the same file name you used for the .md file.
* If you want to feature a video, create an additional image with the dimensions 120x68px with the suffix `_video`.
* If you want to add the game to the carousel, create the carousel image *in two sizes*: 1280x400px and 640x200px. This allows devices with a small screen (smartphones) to save on bandwidth by downloading the smaller file. Name them as described above, with the suffixes `_carousel_1280w` and `_carousel_640w`, respectively.
* Fill in the information in the .md file.

Here are some pointers about the format of the .md file:

* As described above, all images use a fixed naming scheme. You are free to choose a file format (jpg/png are sensible); set the `image_type` field in the relevant section accordingly.
* If you don't want to feature a video, delete the `video` entry and its children.
* Similarly, if you don't want to add the game to the carousel, delete the `carousel` entry and its children.
* To add the game to the carousel, enter some Markdown-formatted text in the `blurb` entry. Then add the game to the `_data/carousel.yml` file.
* The entries in the `platforms` list should be kept in the same order they are in the template file; just delete those that don't apply. To add a new platform, have a look at `/_data/platforms.yml`.
* If the game isn't released yet, add this line: `coming_soon: true`
* Add a description below the second triple-dash (`---`). Feel free to use Markdown formatting. 