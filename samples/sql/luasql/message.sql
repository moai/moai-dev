----------------------------------------------------------------
--
--  message.sql
--  This file contains SQL code to generate a sample SQLite
--  message database that stores localized strings.  It contains
--  two messages for a number of languages.
--
--  Written in 2012 by Brendan A R Sechter <bsechter@sennue.com>
--
--  To the extent possible under law, the author(s) have
--  dedicated all copyright and related and neighboring rights
--  to this software to the public domain worldwide. This
--  software is distributed without any warranty.
--
--  You should have received a copy of the CC0 Public Domain
--  Dedication along with this software. If not, see
--  <http://creativecommons.org/publicdomain/zero/1.0/>.
--
----------------------------------------------------------------


----------------------------------------------------------------
-- Schema
----------------------------------------------------------------

-- Arabic
CREATE TABLE ar (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- German
CREATE TABLE de (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- English
CREATE TABLE en (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- Spanish
CREATE TABLE es (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- French
CREATE TABLE fr (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- Japanese
CREATE TABLE ja (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- Korean
CREATE TABLE ko (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- Russian
CREATE TABLE ru (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- Chinese, Simplified
CREATE TABLE zh_Hans (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- Chinese, Traditional
CREATE TABLE zh_Hant (
	id VARCHAR(128) PRIMARY KEY,
	text VARCHAR(512)
);

-- Default
CREATE VIEW `default` AS
	SELECT id, text FROM en;


----------------------------------------------------------------
-- Data
----------------------------------------------------------------

-- Arabic
INSERT INTO ar ( id, text ) VALUES ('HELLO_WORLD',	'مرحبا العالم');
INSERT INTO ar ( id, text ) VALUES ('GOOD_BYE',		'مع السلامة');

-- German
INSERT INTO de ( id, text ) VALUES ('HELLO_WORLD',	'Hallo Welt');
INSERT INTO de ( id, text ) VALUES ('GOOD_BYE',		'Auf Wiedersehen');

-- English
INSERT INTO en ( id, text ) VALUES ('HELLO_WORLD',	'Hello world');
INSERT INTO en ( id, text ) VALUES ('GOOD_BYE',		'Good bye');

-- Spanish
INSERT INTO es ( id, text ) VALUES ('HELLO_WORLD',	'Hola mundo');
INSERT INTO es ( id, text ) VALUES ('GOOD_BYE',		'Adiós');

-- French
INSERT INTO fr ( id, text ) VALUES ('HELLO_WORLD',	'Bonjour tout le monde');
INSERT INTO fr ( id, text ) VALUES ('GOOD_BYE',		'Au revoir');

-- Japanese
INSERT INTO ja ( id, text ) VALUES ('HELLO_WORLD',	'こんにちは世界');
INSERT INTO ja ( id, text ) VALUES ('GOOD_BYE',		'さようなら');

-- Korean
INSERT INTO ko ( id, text ) VALUES ('HELLO_WORLD',	'안녕하세요');
INSERT INTO ko ( id, text ) VALUES ('GOOD_BYE',		'안녕');

-- Russian
INSERT INTO ru ( id, text ) VALUES ('HELLO_WORLD',	'привет мир');
INSERT INTO ru ( id, text ) VALUES ('GOOD_BYE',		'До свидания');

-- Chinese, Simplified
INSERT INTO zh_Hans ( id, text ) VALUES ('HELLO_WORLD',	'你好世界');
INSERT INTO zh_Hans ( id, text ) VALUES ('GOOD_BYE',	'再見');

-- Chinese, Traditional
INSERT INTO zh_Hant ( id, text ) VALUES ('HELLO_WORLD',	'您好世界');
INSERT INTO zh_Hant ( id, text ) VALUES ('GOOD_BYE',	'再見');

