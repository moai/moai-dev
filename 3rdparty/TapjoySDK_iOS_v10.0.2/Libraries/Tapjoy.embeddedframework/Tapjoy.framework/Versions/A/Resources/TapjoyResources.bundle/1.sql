/*
 * Internal table used to track schema changes
 *
 * NOTE: the "schema_version" table is a special case in that the MRAID code
 *       expects it to be present and will directly manipulate the table
 *       contents. The creation of this table MUST be done in the very first
 *       schema file.
 */
CREATE TABLE schema_version (
   version  NUMBER NOT NULL  -- the current version of the MRAID schema
);



/*
 * Initialize the table with a known value.
 *
 * NOTE: The MRAID code will set the version to the correct value once the
 *       schema file has been correctly processed. All we need to do here is
 *       to ensure that there is a known value present.
 */
INSERT INTO schema_version( version ) VALUES( 0 );