/*
 * Used to store-and-forward off-line requests for URLs.
 */
CREATE TABLE proxy_requests (
   request_number INTEGER NOT NULL PRIMARY KEY,                -- alias for rowid (used for row identity)
   request        TEXT    NOT NULL,                            -- the URI to call
   submitted_on   TEXT    NOT NULL DEFAULT CURRENT_TIMESTAMP   -- when the request was originally requested
);



/*
 * Index based on when resources were submitted.
 */
CREATE INDEX proxy_requests_submitted_idx ON proxy_requests( submitted_on );
