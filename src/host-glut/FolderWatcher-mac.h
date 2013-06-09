//
//  FolderWatcher.h
//

#ifndef FolderWatcher_H
#define FolderWatcher_H

/*-----------------------------------------------------------------------------
	FolderWatcher API
		1. Call FWStart to start watching a directory tree
				Start watching a directory tree.  It's possible to watch 
				multiple directory trees at the same time by repeatedly calling 
				this function.
				
				Want to watch lua files and art asset files at the same time
				but they are in different directory trees?  No problem.
 
		2. Call FWEnumChangedFiles to poll for changed files
				Report changed files to caller through a polling model.
				This allows the game engine to reload files when it has knows
				it is safe to do so.
 
				Notes:	
					- Caller is responsible for freeing the returned filename.
					- Only new files and modified files will be available.
					- The number of changed files tracked is unbounded and
					  should be emptied via this function regularly to avoid
					  memory problems.
				
		3. Call FWStop to stop watching a directory tree
				Once FWStart is used to start watching a directory tree,
				it's possible to stop watching a specific directory tree with 
				FWStop.
 
				Note that this is not used to stop/ignore updates from any
				aribitrary directory tree/subtree, but only those already being
				watched by FWStart
				
 ------------------------------------------------------------------------------*/

// Start watching a directory or single file
void FWStart(const char* path);

// Stop watching a directory or single file
void FWStop(const char* path);

// Stop watching all directories or files
void FWStopAll(void);



// Enumerate a single file that has changed
// Returns null if there are no more changed files
// The caller is responsible for freeing the returned string
char* FWEnumChangedFile(void);


/*-----------------------------------------------------------------------------
	Convenience Functions
		FWWatchFolder
			Given the name of the file, this function starts watching the
			containing directory.  Given the name of a directory, this function
			starts watching the specified directory.
 
			For example, you can pass the path to "main.lua" to this function
			and it will setup Folder Watcher to watch that directory, presumably
			containing all of the lua scripts for your game.
 
		FWReloadAllLuaFiles
			Exhausts FWEnumChangedFile when called and reloads any new or
			modified lua files.
 
			Replace this function to incorporate other kinds of dynamic asset
			loading.
 ------------------------------------------------------------------------------*/

void FWWatchFolder(const char* filename);
void FWReloadChangedLuaFiles(void);

#endif
