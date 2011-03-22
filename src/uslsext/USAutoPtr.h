/*	All contents Copyright (c) 2007-10 by Patrick Meehan
    
    Permission is hereby granted irrevocably and in perpetuity
    to Groundspeak Inc., which, having obtained a copy of this
    software and associated documentation files (the "Software"),
    is free to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the
    Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef	STD_AUTOPTR_H
#define	STD_AUTOPTR_H

//----------------------------------------------------------------//
template < typename TYPE >
class USAutoPtr {
	public:

	TYPE*	mMem;

	//----------------------------------------------------------------//
	inline TYPE* Mem () {
		return mMem;
	};

	//----------------------------------------------------------------//
	inline void operator = ( const USAutoPtr < TYPE >& assign ) {
		if ( mMem ) delete mMem;
		mMem = assign.mMem;
		(( USAutoPtr < TYPE >* )&assign )->mMem = 0; // Cast the const away
	};

	//----------------------------------------------------------------//
	inline void operator = ( TYPE* assign ) {
		if ( mMem ) delete mMem;
		mMem = assign;
	};

	//----------------------------------------------------------------//
	inline TYPE* operator-> () const {
		return mMem;
	};

	//----------------------------------------------------------------//
	inline TYPE& operator*() const {
		return *mMem;
	};

	//----------------------------------------------------------------//
	inline operator TYPE* () {
		return mMem;
	};

	//----------------------------------------------------------------//
	inline operator void* () {
		return mMem;
	};

	//----------------------------------------------------------------//
	inline operator bool () {
		return ( mMem != 0 );
	};

	//----------------------------------------------------------------//
	inline USAutoPtr () :
		mMem ( 0 ) {
	};

	//----------------------------------------------------------------//
	inline USAutoPtr ( const USAutoPtr < TYPE >& assign ) {
		mMem = assign.mMem;
		(( USAutoPtr < TYPE >* )&assign )->mMem = 0;
	};

	//----------------------------------------------------------------//
	inline USAutoPtr ( TYPE* assign ) :
		mMem ( assign ) {
	};

	//----------------------------------------------------------------//
	inline ~USAutoPtr () {
		if ( mMem ) delete mMem;
	};
};

#endif
