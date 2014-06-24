#ifndef BLTUSFILESTREAMWRAPPER_H_
# define BLTUSFILESTREAMWRAPPER_H_

// This is a wrapper made to support USFileStream for BlueTune audio decoder

#include <uslscore/USFileStream.h>

class SeekableUSFileStream : public USFileStream {
public:
    int SetCursor(long offset) {
        return USFileStream::SetCursor(offset);
    };
};

bool BltUSFileStream_Create(const char* filename, ATX_InputStream** stream);


#endif // BLTUSFILESTREAMWRAPPER_H_
