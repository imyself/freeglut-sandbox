#include "shaderprogram.h"
#include <stdio.h>
#include "global_defines.h"

ShaderProgram init_ShaderProgram(const char *vertfile, const char *fragfile) {
    ShaderProgram s;
    s.attrPos = s.attrNor = s.attrCol = -1;
    s.unifViewProj = -1;

    s.vertShader = glCreateShader(GL_VERTEX_SHADER);
    s.fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    s.prog = glCreateProgram();

    FILE *vf = fopen(vertfile, "r");
    GLchar *vert_source = (GLchar*) malloc(MAX_SHADER_SOURCE_LEN);
    size_t chars_read = fread((void*)vert_source, 1, MAX_SHADER_SOURCE_LEN, vf);
    vert_source[chars_read] = '\0';
    glShaderSource(s.vertShader, 1, &vert_source, 0);
    free(vert_source);
    fclose(vf);

    FILE *ff = fopen(fragfile, "r");
    GLchar *frag_source = (GLchar*) malloc(MAX_SHADER_SOURCE_LEN);
    chars_read = fread((void*)frag_source, 1, MAX_SHADER_SOURCE_LEN, ff);
    frag_source[chars_read] = '\0';
    glShaderSource(s.fragShader, 1, &frag_source, 0);
    free(frag_source);
    fclose(ff);

    glCompileShader(s.vertShader);
    glCompileShader(s.fragShader);

    GLint compiled;
    glGetShaderiv(s.vertShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        printShaderInfoLog(s.vertShader);
    }
    glGetShaderiv(s.fragShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        printShaderInfoLog(s.fragShader);
    }

    glAttachShader(s.prog, s.vertShader);
    glAttachShader(s.prog, s.fragShader);
    glLinkProgram(s.prog);

    GLint linked;
    glGetProgramiv(s.prog, GL_LINK_STATUS, &linked);
    if (!linked) {
        printLinkInfoLog(s.prog);
    }

    initShaderVarHandles(&s);

    return s;
}

ShaderProgram* new_ShaderProgram(const char *vertfile, const char *fragfile) {
    ShaderProgram *s = (ShaderProgram*) malloc(sizeof(ShaderProgram));
    *s = init_ShaderProgram(vertfile, fragfile);
    return s;
}


void initShaderVarHandles(ShaderProgram *s) {
    s->attrPos = glGetAttribLocation(s->prog, "vs_Pos");
    s->attrNor = glGetAttribLocation(s->prog, "vs_Nor");
    s->attrCol = glGetAttribLocation(s->prog, "vs_Col");

    s->unifViewProj = glGetUniformLocation(s->prog, "u_ViewProj");
}


void setUniformMat4(ShaderProgram *s, int unifHandle, mat4 *m) {
    glUseProgram(s->prog);
    if(unifHandle != -1) {
        glUniformMatrix4fv(unifHandle, 1, false, m->data);
    }
}

void draw(ShaderProgram *s, Drawable *d) {
    glUseProgram(s->prog);

    if(s->attrPos != -1 && bindPos(d)) {
        glEnableVertexAttribArray(s->attrPos);
        glVertexAttribPointer(s->attrPos, 3, GL_FLOAT, false, 0, NULL);
    }

    if(s->attrNor != -1 && bindNor(d)) {
        glEnableVertexAttribArray(s->attrNor);
        glVertexAttribPointer(s->attrNor, 3, GL_FLOAT, false, 0, NULL);
    }

    if(s->attrCol != -1 && bindCol(d)) {
        glEnableVertexAttribArray(s->attrCol);
        glVertexAttribPointer(s->attrCol, 3, GL_FLOAT, false, 0, NULL);
    }

    bindIdx(d);
    glDrawElements(GL_TRIANGLES, d->count, GL_UNSIGNED_INT, 0);

    if (s->attrPos != -1) glDisableVertexAttribArray(s->attrPos);
    if (s->attrNor != -1) glDisableVertexAttribArray(s->attrNor);
    if (s->attrCol != -1) glDisableVertexAttribArray(s->attrCol);
}

void printShaderInfoLog(int shader) {
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    // should additionally check for OpenGL errors here

    if (infoLogLen > 0) {
        infoLog = (GLchar*) malloc(infoLogLen);
        // error check for fail to allocate memory omitted
        glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
        printf("ShaderInfoLog:%s\n", infoLog);
        free(infoLog);
    }
}


void printLinkInfoLog(int prog) {
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLogLen);

    // should additionally check for OpenGL errors here

    if (infoLogLen > 0) {
        infoLog = (GLchar*) malloc(infoLogLen);
        // error check for fail to allocate memory omitted
        glGetProgramInfoLog(prog, infoLogLen, &charsWritten, infoLog);
        printf("LinkInfoLog:%s\n", infoLog);
        free(infoLog);
    }
}