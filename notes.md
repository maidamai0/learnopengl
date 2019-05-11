# OpenGL learning notes

OpenGL is itself a big statemachine.All significant operations are done to change this machine's state.

## **Pipe line**

**Bold** items are what programmer can do to refine.

pipe line (or shaders) of graphics workflow.

3D data -> rendered pixels.

- **Vertex shader**
- Shape assembley
- **Geometry shader**
- Tests and blending
- **Fragment shader**
- Rasterization

## **Inut:Vertex Data**

list of 3D coordinates which can be formed to triangiles.

- so the number of *3D coordinate* is times of 3?

Vertex data is a list of vertices

- **vertex**

    a collection of data corresponds to a 3D coordinates.

- **vertex attributes**

  contaning many useful information such as *color* in addition to a *3D coordinate*.

- **primitives**

    a hint of render type to tell OpenGL how to make up of those vertices, such as *a line*, *a triangle* or *raw points*

    so, those vertices may not form a triangle?

## **Vertex shader**

- first step of the pipeline.
- takes a sigle vertex.
- transform one `3D coordinate` to another
- basic processing, such as `attributes`.

## **Primitve assemblely**

- take `vertices`(or `vertex` if **GL_POINTS** is specified)
- form a `primitive shape`
  
## **Geometry shader**

- takes a `primitive shape` and has the ability to generate new shape.

## **Raserization**

- mapping the primitive to pixels called `fragments`.
- clipping fragments that is outd side of your view.

## **Fragment Shader**

- a fragment is all the data needed for OpenGL to render a pixel.
- calculate the final color of one pixel.
- most advanced `effects` occured
- fragment **shader** contains data about the `3D scene`

## **Tests and blending**

- `alpha test`
- `blending`
- checks the depth value to decide if one fragment is behind another object.
- check `alpha` value and blends the objects.
- pixel value may entirely different with `Fragment shader` stage when render multiple objects.

In most cases, we only have to do with `Vertex shader` and `Fragment shader`,`Geometry shader` offen set to default.

In modern OpenGL, wo are required to define at least `vertex` and `fragment` shader.

## **Coordinates**

- Normalized Device Coordinates
- view port transform(glViewport)
- Screen space cooordinates

## **Vertex Buffer Object(VBO)** as input

1. `create buffer` glgenBuffers(num, store address) **
2. `bind buffer` glBindBuffer(type or target, source address)
3. `copy data to buffer` glBufferData(type, size, source, data_change_type)

## **Ceate vertex shader**

1. `create vertex shader source code` using glsl
2. `create vertex shader` glCreateShader(GL_VERTEX_SHADER)
3. `set source code of shader` glShaderSource()
4. `compile shader source` glCompileShader()
5. `check compile status` glgetShaderiv(), glGetShaderInfoLog

## **Create fragment shader**

1. `create vertex shader source code` using glsl
2. `create vertex shader` glCreateShader(GL_FRAGMENT_SHADER)
3. `set source code of shader` glShaderSource()
4. `compile shader source` glCompileShader()
5. `check compile status` glgetShaderiv(), glGetShaderInfoLog

## **Shader program**

A shader program is the final linked version of multiple shaders, like excutables linked with mutiple libraries.

1. `create shader program` glCreateProgram
2. `attach` glAttachShader(program, shader), order is important
3. `link` glLinkProgram
4. `check link errors` glGetProgramiv(), glGetProgramInfoLog()
5. `activate` glUseProgram

*You can delete shaders after link*

## **Link vertex attributes**

1. `how OpenGL should interpret data` glVertexAttribPointer
2. glEnableVertexAttribArray

## **Vertex Array Object(VAO)**

1. `create a vao` glGenVertexArrays
2. `bind vao` glBindVertexArray
3. `create and configure vbo` as we said
4. 'unbind vao' glBindVertexArray(0)
5. when use,re-bind vao

## **Element Buffer Object(EBO)**

1. `create ebo` glGenBuffers()
2. `bind buffer` glBindBuffer
3. `copy indices` glBufferData
4. `draw` use **glDrawElements** instead of **glDrawArray**