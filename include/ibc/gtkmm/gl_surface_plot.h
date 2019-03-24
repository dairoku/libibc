// =============================================================================
//  gl_surface_plot.h
//
//  MIT License
//
//  Copyright (c) 2019 Dairoku Sekiguchi
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
// =============================================================================
/*!
  \file     ibc/gtkmm/gl_surface_plot.h
  \author   Dairoku Sekiguchi
  \version  1.0.0
  \date     2019/03/24
  \brief    Header file for ImageViewBase widget

  This file defines the class for the image widget
*/

#ifndef IBC_GTKMM_IMAGE_GL_SURFACE_PLOT_H_
#define IBC_GTKMM_IMAGE_GL_SURFACE_PLOT_H_

// Includes --------------------------------------------------------------------
#include <math.h>
#include <cstring>
#include <gtkmm.h>
#include "ibc/gl/matrix.h"
#include "ibc/gl/utils.h"
#include "ibc/gl/trackball.h"
#include "ibc/gtkmm/gl_obj_view.h"
#include "ibc/gtkmm/image_data.h"
#include "ibc/gtkmm/view_data_interface.h"

// Namespace -------------------------------------------------------------------
namespace ibc
{
 namespace gtkmm
 {
  // ---------------------------------------------------------------------------
  // ImageView class
  // ---------------------------------------------------------------------------
  class GLSurfacePlot : virtual public GLObjView, virtual public ViewDataInterface
  {
  public:
    // Constructors and Destructor ---------------------------------------------
    // -------------------------------------------------------------------------
    // GLSurfacePlot
    // -------------------------------------------------------------------------
    GLSurfacePlot() :
      Glib::ObjectBase("GLSurfacePlot")
    {
      mImageDataPtr = NULL;
      mIsImageSizeChanged = false;
    }
    // -------------------------------------------------------------------------
    // ~GLSurfacePlot
    // -------------------------------------------------------------------------
    virtual ~GLSurfacePlot()
    {
    }
    // ViewDataInterface -------------------------------------------------------
    // -------------------------------------------------------------------------
    // queueRedrawWidget
    // -------------------------------------------------------------------------
    virtual void  queueRedrawWidget()
    {
      queue_render();
    }
    // -------------------------------------------------------------------------
    // markAsImageSizeChanged
    // -------------------------------------------------------------------------
    virtual void  markAsImageSizeChanged()
    {
      mIsImageSizeChanged = true;
      queue_render();
    }
    // Member functions --------------------------------------------------------
    // -------------------------------------------------------------------------
    // setImageDataPtr
    // -------------------------------------------------------------------------
    void  setImageDataPtr(ibc::gtkmm::ImageData *inImageDataPtr)
    {
      mImageDataPtr = inImageDataPtr;
      mImageDataPtr->addWidget(this);
      markAsImageSizeChanged();
    }
    // -------------------------------------------------------------------------
    // isImageSizeChanged
    // -------------------------------------------------------------------------
    bool  isImageSizeChanged() const
    {
      return mIsImageSizeChanged;
    }
  protected:
    // structs -----------------------------------------------------------------
    // -------------------------------------------------------------------------
    // vertex_info
    // -------------------------------------------------------------------------
    struct vertex_info
    {
      GLfloat position[3];
      GLfloat color[3];
    };

    // -------------------------------------------------------------------------
    // glDisplay
    // -------------------------------------------------------------------------
    virtual void  glDisplay()
    {
      GLfloat glMat[16];

      make_current();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glUseProgram(mShaderProgram);
      mTrackball.getGLRotationMatrix(glMat);
      glUniformMatrix4fv(mModelViewLocation, 1, GL_FALSE, &(glMat[0]));
      mProjection.getTransposedMatrix(glMat);
      glUniformMatrix4fv(mProjectionLocation, 1, GL_FALSE, &(glMat[0]));
      glBindVertexArray(mVertexArrayObject);
      glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

glBindVertexArray(mVertexArrayObject2);
glDrawArrays(GL_TRIANGLES, 0, 3);

      glBindVertexArray(0);
      //glFlush();
    }

    // -------------------------------------------------------------------------
    // prepareVertexObjects
    // -------------------------------------------------------------------------
    virtual void  prepareVertexObjects()
    {
      glGenVertexArrays(1, &mVertexArrayObject);
      glBindVertexArray(mVertexArrayObject);

      glGenBuffers(1, &mVertexBufferObject);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glBufferData(GL_ARRAY_BUFFER, getVertexDataSize(), getVertexData(), GL_STATIC_DRAW);

      glGenBuffers(1, &mIndexBufferObject);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexDataSize(), getIndexData(), GL_STATIC_DRAW);

      enableVertexDataAttributes();

glGenVertexArrays(1, &mVertexArrayObject2);
glBindVertexArray(mVertexArrayObject2);

glGenBuffers(1, &mVertexBufferObject2);
glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject2);
glBufferData(GL_ARRAY_BUFFER, getVertexDataSize2(), getVertexData2(), GL_STATIC_DRAW);

// get the location of the "position" and "color" attributes
guint positionLocation = glGetAttribLocation (mShaderProgram, "position");
guint colorLocation = glGetAttribLocation (mShaderProgram, "color");

glEnableVertexAttribArray(0);
glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject2);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

// enable and set the position attribute
glEnableVertexAttribArray (positionLocation);
glVertexAttribPointer (positionLocation, 3, GL_FLOAT, GL_FALSE,
                       sizeof (struct vertex_info),
                       (GLvoid *) (G_STRUCT_OFFSET (struct vertex_info, position)));

// enable and set the color attribute
glEnableVertexAttribArray (colorLocation);
glVertexAttribPointer (colorLocation, 3, GL_FLOAT, GL_FALSE,
                       sizeof (struct vertex_info),
                       (GLvoid *) (G_STRUCT_OFFSET (struct vertex_info, color)));
    }
    // -------------------------------------------------------------------------
    // getVertexDataSize
    // -------------------------------------------------------------------------
    virtual GLsizeiptr getVertexDataSize()
    {
      return 8 * 3 * 2 * sizeof(GLfloat);
    }
    // -------------------------------------------------------------------------
    // getVertexData
    // -------------------------------------------------------------------------
    virtual const GLvoid *getVertexData()
    {
      static const struct vertex_info vertexData[] =
      {
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f } },
        { { -1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f, 0.8f } },
        { { -1.0f,  1.0f,  1.0f }, { 0.0f, 0.8f, 0.0f } },
        { { -1.0f,  1.0f, -1.0f }, { 0.0f, 0.8f, 0.8f } },
        { {  1.0f,  1.0f, -1.0f }, { 0.8f, 0.0f, 0.0f } },
        { {  1.0f, -1.0f, -1.0f }, { 0.8f, 0.0f, 0.8f } },
        { {  1.0f, -1.0f,  1.0f }, { 0.8f, 0.8f, 0.0f } },
        { {  1.0f,  1.0f,  1.0f }, { 0.8f, 0.8f, 0.8f } }
      };
      return vertexData;
    }
// -------------------------------------------------------------------------
// getVertexDataSize
// -------------------------------------------------------------------------
virtual GLsizeiptr getVertexDataSize2()
{
  return 9 * 2 * sizeof(GLfloat);
}
// -------------------------------------------------------------------------
// getVertexData
// -------------------------------------------------------------------------
virtual const GLvoid *getVertexData2()
{
  static const struct vertex_info vertexData[] =
  {
    { {  0.0f,  0.500f, 0.0f }, { 1.f, 0.f, 0.f } },
    { {  0.5f, -0.366f, 0.0f }, { 0.f, 1.f, 0.f } },
    { { -0.5f, -0.366f, 0.0f }, { 0.f, 0.f, 1.f } },
  };
  return vertexData;
}
    // -------------------------------------------------------------------------
    // getIndexDataSize
    // -------------------------------------------------------------------------
    virtual GLsizeiptr getIndexDataSize()
    {
      return 12 * 2 * sizeof(GLuint);
    }
    // -------------------------------------------------------------------------
    // getIndexData
    // -------------------------------------------------------------------------
    virtual const GLvoid *getIndexData()
    {
      static const GLuint indexData[] =
      {
        1, 0,
        2, 7,
        3, 0,
        4, 7,
        5, 0,
        6, 7,
        1, 2,
        2, 3,
        3, 4,
        4, 5,
        5, 6,
        6, 1
      };
      return indexData;
    }
    // -------------------------------------------------------------------------
    // enableVertexDataAttributes
    // -------------------------------------------------------------------------
    virtual void enableVertexDataAttributes()
    {
      // get the location of the "modelview" uniform
      mModelViewLocation = glGetUniformLocation (mShaderProgram, "modelview");

      // get the location of the "projection" uniform
      mProjectionLocation = glGetUniformLocation (mShaderProgram, "projection");

      // get the location of the "position" and "color" attributes
      guint positionLocation = glGetAttribLocation (mShaderProgram, "position");
      guint colorLocation = glGetAttribLocation (mShaderProgram, "color");

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      // enable and set the position attribute
      glEnableVertexAttribArray (positionLocation);
      glVertexAttribPointer (positionLocation, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct vertex_info),
                             (GLvoid *) (G_STRUCT_OFFSET (struct vertex_info, position)));

      // enable and set the color attribute
      glEnableVertexAttribArray (colorLocation);
      glVertexAttribPointer (colorLocation, 3, GL_FLOAT, GL_FALSE,
                             sizeof (struct vertex_info),
                             (GLvoid *) (G_STRUCT_OFFSET (struct vertex_info, color)));
    }
    // -------------------------------------------------------------------------
    // getVertexShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getVertexShaderStr()
    {
      const char *vertexShaderStr = "#version 130\n"
                                    "in vec3 position;"
                                    "in vec3 color;"
                                    "uniform mat4 modelview;"
                                    "uniform mat4 projection;"
                                    "smooth out vec4 vertexColor;"
                                    "void main() {"
                                    "  gl_Position = projection * modelview * vec4(position, 1.0);"
                                    "  vertexColor = vec4(color, 1.0);"
                                    "}";
      return vertexShaderStr;
    }
    // -------------------------------------------------------------------------
    // getFragmentShaderStr
    // -------------------------------------------------------------------------
    virtual const char *getFragmentShaderStr()
    {
      const char *fragmentShaderStr = "#version 130\n"
                                      "smooth in vec4 vertexColor;"
                                      "out vec4 outputColor;"
                                      "void main() {"
                                      "  outputColor = vertexColor;"
                                      "}";
      return fragmentShaderStr;
    }

    // Member variables --------------------------------------------------------
    GLuint mIndexBufferObject;
GLuint mVertexArrayObject2;
GLuint mVertexBufferObject2;

    ImageData *mImageDataPtr;
    bool      mIsImageSizeChanged;
  };
 };
};

#endif  // #ifdef IBC_GTKMM_IMAGE_GL_SURFACE_PLOT_H_

