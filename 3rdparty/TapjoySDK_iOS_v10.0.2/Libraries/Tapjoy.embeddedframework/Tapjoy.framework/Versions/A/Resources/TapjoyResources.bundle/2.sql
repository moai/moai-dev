/*
 * Used to track creative resources used.
 * NOTE: This is the original creative loaded; all subsequent requests for
 *       cached resources will become part of this creative.
 *       The hash value will become the name of the cache directory as well as
 *       the identifier used to determine the identity of a given creative.
 *       The initial creative will be placed inside this new directory as the
 *       index.html file. Additional resources will be placed in subdirectories
 *       of this cache directory based on the full URL of the resource including
 *       the host name.
 *       An example directory structure might be:
 *       321252/
 *       321252/index.html
 *       321252/www.example.com/
 *       321252/www.example.com/images/
 *       321252/www.example.com/images/logo.png
 *       321252/www.example.com/images/bg.png
 *       321252/www.example.com/images/fg.png
 */
CREATE TABLE creatives (
   hash          NUMBER NOT NULL,                            -- the hash of the resource (also the directory name)
   size          NUMBER NOT NULL DEFAULT 0,                  -- the total size of all the resources in the directory
   cached_on     TEXT   NOT NULL DEFAULT CURRENT_TIMESTAMP,  -- when the cache was originally created
   last_accessed TEXT   NOT NULL DEFAULT CURRENT_TIMESTAMP,  -- when the cache was last accessed
   
   CONSTRAINT base_url_pk PRIMARY KEY( hash )
);



/*
 * Index based on when resources were last accessed.
 */
CREATE INDEX base_urls_access_idx ON base_urls( last_accessed );
