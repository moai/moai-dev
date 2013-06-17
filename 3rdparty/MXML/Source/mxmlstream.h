/****************************************************************************

 Copyright 2003 Matthew Riek
 See legal notice in copyright.txt

 Description : micro xml stream

 ****************************************************************************/

#ifndef _MXMLSTREAM_H_
#define _MXMLSTREAM_H_

#include "mxmldom.h"
#include <stdio.h>
#include <istream>
#include <ostream>
using std::istream;
using std::ostream;

class MXMLStreamFile : public MXMLStream
{
public:

  MXMLStreamFile(FILE * a_fp)
  {
    m_fp = a_fp;
  }

  MXMLStreamFile(const char * a_fileName, const char * a_flags)
  {
    m_fp = fopen(a_fileName, a_flags);
  }

  ~MXMLStreamFile()
  {
    if(m_fp)
    {
      fclose(m_fp);
    }
  }

  inline bool IsOpen() const { return (m_fp != NULL); }

  virtual size_t Read(void * a_buffer, size_t a_bytes)
  {
    if(m_fp)
    {
      return fread(a_buffer, 1, a_bytes, m_fp);
    }
    return 0;
  }

  virtual size_t Write(const void * a_buffer, size_t a_bytes)
  {
    if(m_fp)
    {
      return fwrite(a_buffer, 1, a_bytes, m_fp);
    }
    return 0;
  }

private:

  FILE * m_fp;
};


class MXMLStreamIStream : public MXMLStream
{
public:

  MXMLStreamIStream(istream &a_stream) { m_stream = &a_stream; }

  virtual size_t Read(void * a_buffer, size_t a_bytes) 
  { 
    m_stream->read((char *) a_buffer, (std::streamsize) a_bytes); 
    return (size_t) m_stream->gcount();
  }

  virtual size_t Write(const void * a_buffer, size_t a_bytes) { a_buffer; a_bytes; return 0; }

private:

  istream * m_stream;
};


class MXMLStreamOStream : public MXMLStream
{
public:

  MXMLStreamOStream(ostream &a_stream) { m_stream = &a_stream; }

  virtual size_t Read(void * a_buffer, size_t a_bytes) { a_buffer; a_bytes; return 0; }

  virtual size_t Write(const void * a_buffer, size_t a_bytes) 
  {
    size_t b = (size_t) m_stream->tellp();
    m_stream->write((char *) a_buffer, (std::streamsize) a_bytes);
    size_t e = (size_t) m_stream->tellp();
    if(m_stream->fail())
    {
      return 0;
    }
    return (e - b);
  }

private:

 ostream * m_stream;
};

#endif