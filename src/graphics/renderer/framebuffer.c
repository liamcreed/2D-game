#include "graphics/graphics.h"

void framebuffer_create(framebuffer_t* framebuffer)
{
    if (framebuffer->texture.width == 0 || framebuffer->texture.height == 0)
        printf(LOG_WARNING"[Framebuffer]: resolution is x: 0, y: 0 !!\n");

    GL(glGenFramebuffers(1, &framebuffer->fbo));
    GL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo));

    GL(glGenTextures(1, &framebuffer->texture.id));
    GL(glBindTexture(GL_TEXTURE_2D, framebuffer->texture.id));

    GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebuffer->texture.width, framebuffer->texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer->texture.id, 0));

    GL(glGenRenderbuffers(1, &framebuffer->rbo));
    GL(glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->rbo));
    GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, framebuffer->texture.width, framebuffer->texture.height));
    GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->rbo));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf(LOG_ERROR "Failed to create framebuffer!\n");
        exit(-1);
    }

    framebuffer_unbind();
}

void framebuffer_bind(framebuffer_t* framebuffer)
{
    GL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo));
    GL(glEnable(GL_DEPTH_TEST));
    GL(glViewport(0, 0, framebuffer->texture.width, framebuffer->texture.height));
}
void framebuffer_unbind()
{
    GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
void framebuffer_delete(framebuffer_t* framebuffer)
{
    GL(glDeleteFramebuffers(1, &framebuffer->fbo));
    GL(glDeleteRenderbuffers(1, &framebuffer->rbo));
    GL(glDeleteTextures(1, &framebuffer->texture.id));
}