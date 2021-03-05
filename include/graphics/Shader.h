#pragma once

//
//const char* LEARNOPENGL_VERT_SHADER =
//        "#version 330 core\n"
//        "layout (location = 0) in vec4 vertex;\n"
//        "\n"
//        "out vec2 TexCoords;\n"
//        "uniform mat4 projection;"
//        "uniform mat4 model;"
//        "\n"
//        "void main()\n"
//        "{\n"
//        "    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n"
//        "    TexCoords = vertex.zw;\n"
//        "}";
//
//const char* LEARNOPENGL_FRAG_SHADER =
//        "#version 330 core\n"
//        "in vec2 TexCoords;\n"
//        "out vec4 color;\n"
//        "\n"
//        "uniform sampler2D image;\n"
//        "\n"
//        "void main()\n"
//        "{    \n"
//        "    color = texture(image, TexCoords);\n"
//        "}";
