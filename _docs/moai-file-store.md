---
title: "Moai File Store"
---

Moai Storage
------------

Within Moai Cloud your services have access to two areas of file storage. Each services is provided with it's own private code storage area. That location can be accessed through the `moai.code` table and it stores all of you code and assets that you have uploaded through the dashboard. A second global storage area called the Moai Content Store is accessible to all of your services. You can access this storage area throug the dashboard under the Moai Cloud Data section or through the `moai.content` table.

The following operations are available to your services. The can be called through either the `moai.code` or `moai.content` table.

#### Save a file:

:   `store_data(data, path)`

    :   **data:** A lua string containing the data you want stored in the file
    :   **path:** The path and filename within the file store where the data should be saved. All paths are absolute from the root of the file store. The leading '/' is optional
    :   **returns:** A boolean. true if the file was saved, false if it was not.

#### Load a file:

:   `find_file(path)`

    :   **path:** The path and filename to the file you want to load. All paths are absolute from the root of the file store. The leading '/' is optional
    :   **returns:** The data contained in the file

#### Delete a file:

:   `remove_file(path)`

    :   **path:** The path and filename within the file store where the data should be saved. All paths are absolute from the root of the file store. The leading '/' is optional
    :   **returns:** Nothing

#### List files:

:   `list(path)`

    :   **path:** The path within the file store to start looking for files. If no path then all files are returned. All paths are absolute from the root of the file store. The leading '/' is optional
    :   **returns:** An array of full paths to files within the file store

#### GridFS files:

:   `gridfs_file(path)`

    :   **path:** The path and filename to the file you want to load. All paths are absolute from the root of the file store. The leading '/' is optional
    :   **returns:** A gridfsfile object


