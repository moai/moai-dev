
GUI for MOAI
Derick Dong (derickdong@hotmail.com)

This is a fairly simple GUI system, but should be enough to handle the necessary tasks of most
developers. Its still in an early state, so expect bugs, quirks, and incomplete features.

There is currently support for the following widget types:

- Button - A basic push button
- Check Box - Can be set to checked or unchecked
- Edit Box - Allows entry of text
- Image - A static image
- Label - Simple static text
- Progress Bar - Displays current progress
- Radio Button - Can be selected or unselected; only one in the group may be selected at a time
- Scroll Bar - Scrolls through a list of items
- Slider - Selection of a numeric value by adjusting the slider
- Text Box - Displays multiple lines of text, with a scrollbar
- Widget List - Like a list box; supports multiple columns and multiple selections; different widget
	types can be associated with each column
- Window - A basic window; more of a container for other widgets than anything else


Some things to note:

- The GUI layer needs to be the topmost layer in order to receive input properly. Otherwise, layers
above will intercept it. The system creates its own layer, but will need to be added manually to
MOAIRenderMgr. The GUI layer can be retrieved by calling GUI:layer().

- The system uses a relative scale for the positioning and sizing of widgets, ranging from 0 to 100.
A widget's anchor point is at its top left. A position of (0, 0) would set the anchor to the top left
of the window, (100, 100) would be the lower right (and would normally take it out of the view area),
and (50, 50) would be the dead center. A size of (50, 50) makes the widget half the dimensions of the
screen in both width and height (ie. if the screen were (320, 480), the widget would be (160, 240)).
