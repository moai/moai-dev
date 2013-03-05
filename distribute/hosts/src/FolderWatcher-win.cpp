#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <FolderWatcher-win.h>
#include <aku/AKU.h>

#pragma warning ( disable : 4996 )

const int _SIZE = 1024;

struct FileInfo {
	FILETIME lastwrite;
	char * fileName;
	bool shouldDelete;
};

struct DirInfo {
	HANDLE notifyHandle;
	const char * dir;
};

static struct FileInfo * filesInfo[_SIZE];
const char * baseDirectoryPath;
const char * directoryQueryString;
static struct DirInfo * notifyHandles[_SIZE];

//-------- Utility Functions -----------//

static bool isLuaFile(const char * fileName) {
	size_t size = strlen(fileName);
	return fileName[size-3] == 'l' &&
		   fileName[size-2] == 'u' &&
		   fileName[size-1] == 'a';
}

static size_t findLastOccuranceOfDirectorySeparator(const char * path) {
	for (int i = strlen(path) - 1; i > 0; i--) 
	{
		if (path[i] == '\\') {
			return i;
			break;
		}
	}
	return 0;
}

static const char *  createDirectoryQueryString(const char * fullFileName) {
	size_t dirPathSize = findLastOccuranceOfDirectorySeparator(fullFileName);

	char * temp = (char *) calloc(dirPathSize+3,sizeof(char));
	strncpy(temp,fullFileName,dirPathSize);
	temp[dirPathSize] = '\\';
	temp[dirPathSize+1] = '*';
	temp[dirPathSize+2] = '\0';
	return temp;
}

static bool findFilePosition(const char * fileName,int * pos) {
	int markFreeOne = -1;
	for(unsigned int i=0; i < _SIZE; i++) {
		if (filesInfo[i] != NULL) {
			if (strcmp(filesInfo[i]->fileName,fileName) == 0) {
				*pos = i;
				return false;
			}
		} else if(markFreeOne == -1) {
			markFreeOne = i;
		} else {
			break;
		}
	}

	*pos = markFreeOne;
	assert(*pos != -1);
	return true;
}

static const char * concatDirAndFileName(const char * dir,const char * file,bool trailingSlash) {
	size_t fileSize = strlen(file);
	size_t directoryPathSize = strlen(dir);
	size_t extraSize = 1;

	if (trailingSlash)
		extraSize++;

	size_t fullPathSize = directoryPathSize + fileSize + extraSize;
	char * fullPath = (char *) calloc(fullPathSize,sizeof(char));
	strcpy(fullPath,dir);

	strcpy(fullPath + directoryPathSize,file);
	
	if(trailingSlash) 
		fullPath[fullPathSize-2] = '\\';
	fullPath[fullPathSize-1] = '\0';

	return (const char *)fullPath;
}
//------------------------------------//

static void deleteFileInfoOnPos(int pos) {
	free((void*)filesInfo[pos]->fileName);
	filesInfo[pos] = NULL;
}

static void markAllForDeletion() {
	for(unsigned int i=0; i < _SIZE; i++) {
		if (filesInfo[i] != NULL)
			filesInfo[i]->shouldDelete = true;
	}
}

static void deleteAllMarked() {
	for(unsigned int i=0; i < _SIZE; i++) {
		if (filesInfo[i] != NULL && filesInfo[i]->shouldDelete) 
			deleteFileInfoOnPos(i);
	}
}

static void reloadLuaFile(const char * file) {
	AKURunScript(file);
	printf("%s reloaded.\n",file);
}

static void newFileInfoOnPos(const char * fileName,WIN32_FIND_DATA * newLuaFile,int pos) {
	if (filesInfo[pos] == NULL) {
		filesInfo[pos] = (struct FileInfo *) malloc(sizeof(struct FileInfo));
		filesInfo[pos]->fileName = (char *) calloc(sizeof(char),MAX_PATH);
	}

	filesInfo[pos]->shouldDelete = false;
	filesInfo[pos]->lastwrite = newLuaFile->ftLastWriteTime;
	strcpy(filesInfo[pos]->fileName,fileName);
}

static void watchDirectory(const char * dir) {

	for (int i=0;  i<_SIZE; i++) {
		if (notifyHandles[i] != NULL) {
			if (!strcmp(dir,notifyHandles[i]->dir))
				break;
		} else {
			notifyHandles[i] = (struct DirInfo *)malloc(sizeof(struct DirInfo));
			notifyHandles[i]->dir = (const char*)calloc(strlen(dir)+1,sizeof(char));
			strcpy((char*)notifyHandles[i]->dir,dir);
			notifyHandles[i]->notifyHandle = FindFirstChangeNotification(dir,false,FILE_NOTIFY_CHANGE_LAST_WRITE);
			break;
		}
	}
}

static void listDirectory(const char * directory) {
	WIN32_FIND_DATA ffd;
	HANDLE fHandle;

	watchDirectory(directory);

	const char * directoryQueryString = createDirectoryQueryString(directory);
	fHandle = FindFirstFile(directoryQueryString, &ffd);
	if (INVALID_HANDLE_VALUE == fHandle) {
		printf("Failed to list directory!\n");
		return;
	}
	do {
		const char * fullPath;
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && 
			strcmp(ffd.cFileName,".") && 
			strcmp(ffd.cFileName,"..")) 
		{
			fullPath = concatDirAndFileName(directory,ffd.cFileName,true);
			listDirectory(fullPath);
			free((void*)fullPath);
		}
		else if (isLuaFile(ffd.cFileName)) {
			int pos;
			fullPath = concatDirAndFileName(directory,ffd.cFileName,false);
			bool isNew = findFilePosition(fullPath,&pos);
			if (isNew) {
				newFileInfoOnPos(fullPath,&ffd,pos);
			} 
			else if (CompareFileTime(&ffd.ftLastWriteTime,&filesInfo[pos]->lastwrite) == 1) {
				reloadLuaFile(filesInfo[pos]->fileName);
				filesInfo[pos]->lastwrite = ffd.ftLastWriteTime;
			}
			filesInfo[pos]->shouldDelete = false;
			free((void*)fullPath);
		}
	}
	while (FindNextFile(fHandle, &ffd) != 0);

	free((void*)directoryQueryString);
}

static void listProjectDirectory() {
	markAllForDeletion();
	listDirectory(baseDirectoryPath);
	deleteAllMarked();
}

static void setStartupDir(const char * startupScript ) {
	size_t dirPathSize = findLastOccuranceOfDirectorySeparator(startupScript);
	char * temp = (char *) calloc(max(dirPathSize+2,3),sizeof(char));
	if ( dirPathSize ) {
		strncpy(temp,startupScript,dirPathSize+1);
		temp[dirPathSize+1] = '\0';
	}
	else {
		strncpy(temp,".\\",2);
		temp [ 2 ] = '\0';
	}
	baseDirectoryPath = (const char *) temp;
}

void winhostext_WatchFolder(const char* startupScript) {
	for(unsigned int i=0; i < _SIZE; i++) {
		filesInfo[i] = NULL;
		notifyHandles[i] = NULL;
	}

	setStartupDir(startupScript);
	
	listProjectDirectory();
}

static bool safeGuard = true;
void winhostext_Query() {

	for (int i = 0; i<_SIZE; i++) {
		struct DirInfo * dirInfo = notifyHandles[i];

		if (dirInfo == NULL)
			break;

		if (dirInfo->notifyHandle == INVALID_HANDLE_VALUE)
			continue;

		DWORD singleWaitStatus = WaitForSingleObject(dirInfo->notifyHandle,0);
		if (singleWaitStatus == WAIT_OBJECT_0) {
			if (FindNextChangeNotification(dirInfo->notifyHandle)) {
				safeGuard = !safeGuard;
				if (safeGuard) {
					listProjectDirectory();
				}
			}
		} else if (singleWaitStatus == WAIT_TIMEOUT) {
		} else {
			printf("\n ERROR: Unhandled dwWaitStatus.\n");
		}
	}
}

void winhostext_CleanUp() {
	for (int i=0; i< _SIZE; i++) {
		if (filesInfo[i] != NULL)
			deleteFileInfoOnPos(i);

		if(notifyHandles[i] != NULL) {
			FindCloseChangeNotification (notifyHandles[i]->notifyHandle);
			free((void*)notifyHandles[i]->dir);
			free((void*)notifyHandles[i]);
		}
			
	}

	free((void*)directoryQueryString);
	free((void*)baseDirectoryPath);
}