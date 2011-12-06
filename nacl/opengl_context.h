// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXAMPLES_TUMBLER_OPENGL_CONTEXT_H_
#define EXAMPLES_TUMBLER_OPENGL_CONTEXT_H_

///
/// @file
/// OpenGLContext manages the OpenGL context in the browser that is associated
/// with a @a pp::Instance instance.
///

#include <assert.h>
#include <pthread.h>

#include <algorithm>
#include <string>

#include "ppapi/c/ppb_opengles2.h"
#include "ppapi/cpp/graphics_3d_client.h"
#include "ppapi/cpp/graphics_3d.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/size.h"

class OpenGLContext : public pp::Graphics3DClient {
 public:
  explicit OpenGLContext(pp::Instance* instance);

  /// Release all the in-browser resources used by this context, and make this
  /// context invalid.
  virtual ~OpenGLContext();

  virtual void Graphics3DContextLost() {
    assert(!"Unexpectedly lost graphics context");
  }

  /// Make @a this the current 3D context in @a instance.
  /// @param instance The instance of the NaCl module that will receive the
  ///                 the current 3D context.
  /// @return success.
  bool MakeContextCurrent(pp::Instance* instance);

  /// Flush the contents of this context to the browser's 3D device.
  void FlushContext();

  /// Make the underlying 3D device invalid, so that any subsequent rendering
  /// commands will have no effect.  The next call to MakeContextCurrent() will
  /// cause the underlying 3D device to get rebound and start receiving
  /// receiving rendering commands again.  Use InvalidateContext(), for
  /// example, when resizing the context's viewing area.
  void InvalidateContext(pp::Instance* instance);

  /// Resize the context.
  void ResizeContext(const pp::Size& size);

  /// The OpenGL ES 2.0 interface.
  const struct PPB_OpenGLES2* gles2() const {
    return gles2_interface_;
  }

  /// The PP_Resource needed to make GLES2 calls through the Pepper interface.
  const PP_Resource gl_context() const {
    return context_.pp_resource();
  }

  /// Indicate whether a flush is pending.  This can only be called from the
  /// main thread; it is not thread safe.
  bool flush_pending() const {
    return flush_pending_;
  }
  void set_flush_pending(bool flag) {
    flush_pending_ = flag;
  }

 private:
  pp::Size size_;
  pp::Graphics3D context_;
  bool flush_pending_;

  const struct PPB_OpenGLES2* gles2_interface_;
};

#endif  // EXAMPLES_TUMBLER_OPENGL_CONTEXT_H_

